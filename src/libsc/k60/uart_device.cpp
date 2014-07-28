/*
 * uart_device.cpp
 * UART device abstraction
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <functional>
#include <list>
#include <memory>
#include <string>
#include <vector>

#include <log.h>
#include <vectors.h>

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/uart.h"

#include "libsc/com/config.h"
#include "libsc/k60/uart_device.h"
#include "libutil/misc.h"

using namespace std;
using namespace libbase::k60;

#define RX_BUFFER_SIZE 128
#define MAX_TX_BUFFER 14
#define TX_BUFFER_UPPER_BOUND 16
#define TX_BUFFER_LOWER_BOUND 7

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_UART

struct UartDevice::RxBuffer
{
	RxBuffer()
			: start(0), end(0)
	{
		memset(data, 0, RX_BUFFER_SIZE);
	}

	Uint GetSize() const volatile
	{
		return (Uint)(end - start);
	}

	Byte data[RX_BUFFER_SIZE];
	Uint start;
	Uint end;
};

struct UartDevice::TxBuffer
{
	struct Slot
	{
		Slot(Byte* const data, const size_t size, const bool is_mem_owned)
				: type(BYTE_ARY), size(size), it(0), is_mem_owned(is_mem_owned)
		{
			this->data.byte_ = data;
		}

		Slot(Byte* const data, const size_t size)
				: Slot(data, size, true)
		{}

		explicit Slot(string* const data)
				: type(STRING), size(data->size()), it(0)
		{
			this->data.string_ = data;
		}

		explicit Slot(vector<Byte>* const data)
				: type(VECTOR), size(data->size()), it(0)
		{
			this->data.vector_ = data;
		}

		Slot()
				: type(BYTE_ARY), size(0), it(0), is_mem_owned(false)
		{
			this->data.byte_ = nullptr;
		}

		Slot(const Slot&) = delete;
		Slot(Slot &&rhs)
				: data(rhs.data), type(rhs.type), size(rhs.size), it(rhs.it),
				  is_mem_owned(rhs.is_mem_owned)
		{
			rhs.data.byte_ = nullptr;
			rhs.is_mem_owned = false;
		}

		~Slot()
		{
			Recycle();
		}

		Slot& operator=(const Slot&) = delete;
		volatile Slot& operator=(Slot &&rhs) volatile;

		Slot& operator=(Slot &&rhs)
		{
			return *const_cast<Slot*>(&const_cast<volatile Slot*>(this)
					->operator=(std::move(rhs)));
		}

		void Recycle() volatile
		{
			if (is_mem_owned && data.byte_)
			{
				switch (type)
				{
				case BYTE_ARY:
					delete[] data.byte_;
					break;

				case STRING:
					delete data.string_;
					break;

				case VECTOR:
					delete data.vector_;
					break;
				}
			}
		}

		union
		{
			Byte *byte_;
			string *string_;
			vector<Byte> *vector_;
		} data;
		enum
		{
			BYTE_ARY,
			STRING,
			VECTOR,
		} type;
		size_t size;
		uint8_t it;
		bool is_mem_owned = true;
	};

	TxBuffer()
			: sending_slot(nullptr), queued(0), sent(0)
	{}

	void PushBuffer(Slot &&slot);

	void Acquire()
	{
		sending_slot = &slots[sent++ % TX_BUFFER_UPPER_BOUND];
	}

	Uint GetSize() const
	{
		return (Uint)(queued - sent);
	}

	Slot slots[TX_BUFFER_UPPER_BOUND];
	volatile Slot *sending_slot;
	volatile Uint queued;
	volatile Uint sent;
};

volatile UartDevice::TxBuffer::Slot& UartDevice::TxBuffer::Slot::operator=(
		Slot &&rhs) volatile
{
	if (this != &rhs)
	{
		Byte* const data_ = rhs.data.byte_;
		bool is_mem_owned_ = rhs.is_mem_owned;
		Recycle();
		rhs.data.byte_ = nullptr;
		rhs.is_mem_owned = false;

		data.byte_ = data_;
		type = rhs.type;
		size = rhs.size;
		it = rhs.it;
		is_mem_owned = is_mem_owned_;
	}
	return *this;
}

void UartDevice::TxBuffer::PushBuffer(Slot &&slot)
{
	if (GetSize() > MAX_TX_BUFFER)
	{
		while (GetSize() > TX_BUFFER_LOWER_BOUND)
		{
			++sent;
		}
	}
	Slot *s;
	while ((s = &slots[queued++ % TX_BUFFER_UPPER_BOUND]) == sending_slot)
	{}
	*s = std::move(slot);
}

namespace
{

#if LIBSC_USE_UART == 1
inline PinConfig::Name GetTxPin(const uint8_t)
{
	return LIBSC_UART0_TX;
}

inline PinConfig::Name GetRxPin(const uint8_t)
{
	return LIBSC_UART0_RX;
}

#else
inline PinConfig::Name GetTxPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(0);

	case 0:
		return LIBSC_UART0_TX;

#if LIBSC_USE_UART > 1
	case 1:
		return LIBSC_UART1_TX;
#endif
	}
}

inline PinConfig::Name GetRxPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(0);

	case 0:
		return LIBSC_UART0_RX;

#if LIBSC_USE_UART > 1
	case 1:
		return LIBSC_UART1_RX;
#endif
	}
}

#endif

Uart::Config GetUartConfig(const uint8_t id,
		const Uart::Config::BaudRate baud_rate,
		const Uart::OnTxEmptyListener &tx_isr,
		const Uart::OnRxFullListener &rx_isr)
{
	Uart::Config config;
	config.tx_pin = GetTxPin(id);
	config.rx_pin = GetRxPin(id);
	config.baud_rate = baud_rate;
	config.config.set(Uart::Config::ConfigBit::kFifo);
	config.tx_isr = tx_isr;
	config.rx_isr = rx_isr;
	return config;
}

}

UartDevice::UartDevice(const uint8_t id, const Uart::Config::BaudRate baud_rate)
		: m_rx_buf{new RxBuffer}, m_listener(nullptr),
		  m_tx_buf{new TxBuffer}, m_is_tx_idle(true),
		  m_uart(GetUartConfig(id, baud_rate,
				  std::bind(&UartDevice::OnTxEmpty, this, placeholders::_1),
				  std::bind(&UartDevice::OnRxFull, this, placeholders::_1)))
{}

UartDevice::~UartDevice()
{}

inline void UartDevice::EnableTx()
{
	if (m_is_tx_idle)
	{
		m_is_tx_idle = false;
		m_uart.SetEnableTxIrq(true);
	}
}

inline void UartDevice::DisableTx()
{
	m_uart.SetEnableTxIrq(false);
	m_is_tx_idle = true;
}

void UartDevice::SendStr(const char *str)
{
	const size_t size = strlen(str);
	if (size == 0)
	{
		return;
	}
	Byte *data = new Byte[size];
	memcpy(data, str, size);

	m_tx_buf->PushBuffer(TxBuffer::Slot(data, size));
	EnableTx();
}

void UartDevice::SendStr(unique_ptr<char[]> &&str)
{
	const size_t size = strlen(str.get());
	if (size == 0)
	{
		return;
	}

	m_tx_buf->PushBuffer(TxBuffer::Slot(reinterpret_cast<Byte*>(str.release()),
			size));
	EnableTx();
}

void UartDevice::SendStr(string &&str)
{
	if (str.size() == 0)
	{
		return;
	}

	m_tx_buf->PushBuffer(TxBuffer::Slot(new string(std::move(str))));
	EnableTx();
}

void UartDevice::SendBuffer(const Byte *buf, const size_t len)
{
	if (len == 0)
	{
		return;
	}
	Byte *data = new Byte[len];
	memcpy(data, buf, len);

	m_tx_buf->PushBuffer(TxBuffer::Slot(data, len));
	EnableTx();
}

void UartDevice::SendBuffer(unique_ptr<Byte[]> &&buf, const size_t len)
{
	if (len == 0)
	{
		return;
	}

	m_tx_buf->PushBuffer(TxBuffer::Slot(buf.release(), len));
	EnableTx();
}

void UartDevice::SendBuffer(vector<Byte> &&buf)
{
	if (buf.size() == 0)
	{
		return;
	}

	m_tx_buf->PushBuffer(TxBuffer::Slot(new vector<Byte>(std::move(buf))));
	EnableTx();
}

void UartDevice::SendStrLiteral(const char *str)
{
	const size_t size = strlen(str);
	if (size == 0)
	{
		return;
	}

	m_tx_buf->PushBuffer(TxBuffer::Slot((Byte*)str, size, false));
	EnableTx();
}

void UartDevice::OnTxEmpty(Uart *uart)
{
	volatile TxBuffer::Slot *slot = m_tx_buf->sending_slot;
	if (m_tx_buf->GetSize() == 0 && (!slot || slot->it == slot->size))
	{
		DisableTx();
		return;
	}

	while (!slot || slot->it == slot->size)
	{
		m_tx_buf->Acquire();
		slot = m_tx_buf->sending_slot;
	}

	const size_t size = slot->size - slot->it;
	switch (slot->type)
	{
	case TxBuffer::Slot::BYTE_ARY:
		slot->it += uart->PutBytes(slot->data.byte_ + slot->it, size);
		break;

	case TxBuffer::Slot::STRING:
		slot->it += uart->PutBytes((const Byte*)slot->data.string_->data()
				+ slot->it, size);
		break;

	case TxBuffer::Slot::VECTOR:
		slot->it += uart->PutBytes(slot->data.vector_->data() + slot->it, size);
		break;
	}
}

void UartDevice::EnableRx(const OnReceiveListener &listener)
{
	DisableRx();
	m_listener = listener;
	m_uart.SetEnableRxIrq(true);
}

void UartDevice::DisableRx()
{
	m_uart.SetEnableRxIrq(false);
}

bool UartDevice::PeekChar(char *out_char)
{
	if (m_rx_buf->GetSize() == 0)
	{
		return false;
	}

	*out_char = m_rx_buf->data[m_rx_buf->start++ % RX_BUFFER_SIZE];
	return true;
}

void UartDevice::OnRxFull(Uart *uart)
{
	vector<Byte> bytes;
	if (!uart->PeekBytes(&bytes))
	{
		return;
	}

	if (!m_listener)
	{
		for (size_t i = 0; i < bytes.size(); ++i)
		{
			if (m_rx_buf->GetSize() + 1 >= RX_BUFFER_SIZE)
			{
				break;
			}

			m_rx_buf->data[m_rx_buf->end++ % RX_BUFFER_SIZE] = bytes[i];
		}
	}
	else
	{
		m_listener(bytes.data(), bytes.size());
	}
}

#else /* LIBSC_USE_UART */

struct UartDevice::RxBuffer
{};

struct UartDevice::TxBuffer
{};

UartDevice::UartDevice(const uint8_t, const Uart::Config::BaudRate)
{
	LOG_D("Configured not to use UartDevice");
}
UartDevice::~UartDevice() {}
void UartDevice::SendStr(const char*) {}
void UartDevice::SendStr(unique_ptr<char[]>&&) {}
void UartDevice::SendStr(string&&) {}
void UartDevice::SendBuffer(const Byte*, const size_t) {}
void UartDevice::SendBuffer(unique_ptr<Byte[]>&&, const size_t) {}
void UartDevice::SendBuffer(vector<Byte>&&) {}
void UartDevice::EnableRx(OnReceiveListener) {}
void UartDevice::DisableRx() {}
bool UartDevice::PeekChar(char*) { return false; }

#endif /* LIBSC_USE_UART */

}
}
