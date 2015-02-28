/*
 * uart_device.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstdint>
#include <cstdio>
#include <cstring>

#include <functional>
#include <list>
#include <memory>
#include <string>
#include <vector>

#include "libbase/log.h"
#include "libbase/k60/dma.h"
#include "libbase/k60/dma_manager.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/uart.h"

#include "libsc/config.h"
#include "libsc/k60/uart_device.h"
#include "libutil/dynamic_block_buffer.h"
#include "libutil/misc.h"

using namespace libbase::k60;
using namespace libutil;
using namespace std;

#define RX_BUFFER_SIZE 128

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

namespace
{

#if LIBSC_USE_UART == 1
inline Pin::Name GetTxPin(const uint8_t)
{
	return LIBSC_UART0_TX;
}

inline Pin::Name GetRxPin(const uint8_t)
{
	return LIBSC_UART0_RX;
}

#else
inline Pin::Name GetTxPin(const uint8_t id)
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

inline Pin::Name GetRxPin(const uint8_t id)
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

inline Uart::Config GetUartConfig_(Uart::Config &&config,
		const Uart::OnTxEmptyListener &tx_isr,
		const Uart::OnRxFullListener &rx_isr)
{
	config.tx_isr = tx_isr;
	config.rx_isr = rx_isr;
	return config;
}

}

Uart::Config UartDevice::Initializer::GetUartConfig() const
{
	Uart::Config product;
	product.tx_pin = GetTxPin(config.id);
	product.rx_pin = GetRxPin(config.id);
	product.baud_rate = config.baud_rate;
	product.config.set(Uart::Config::ConfigBit::kFifo);
	product.rx_irq_threshold = config.rx_irq_threshold;
	product.is_rx_irq_threshold_percentage =
			config.is_rx_irq_threshold_percentage;
	return product;
}

UartDevice::UartDevice(const Initializer &initializer)
		: m_rx_buf{new RxBuffer},
		  m_listener(nullptr),
		  m_tx_buf(initializer.config.tx_buf_size),
		  m_is_tx_idle(true),
		  m_tx_dma_channel(initializer.config.tx_dma_channel),
		  m_dma(nullptr),
		  m_uart(nullptr)
{
	Uart::Config &&uart_config = initializer.GetUartConfig();
	uart_config.rx_isr = std::bind(&UartDevice::OnRxFull, this, placeholders::_1);
	if (!IsUseDma())
	{
		uart_config.tx_isr = std::bind(&UartDevice::OnTxEmpty, this,
				placeholders::_1);
	}
	m_uart = Uart(uart_config);

	if (IsUseDma())
	{
		m_dma_config.reset(new Dma::Config);
		m_dma_config->src.addr = nullptr; // temp
		m_dma_config->src.offset = 1;
		m_dma_config->src.size = Dma::Config::TransferSize::k1Byte;
		m_dma_config->src.major_offset = 0;
		m_dma_config->major_count = 0;
		m_uart.ConfigTxAsDmaDst(m_dma_config.get());
		m_dma_config->complete_isr = std::bind(&UartDevice::OnTxDmaComplete,
				this, placeholders::_1);
	}

	m_uart.SetEnableRxIrq(true);
}

UartDevice::~UartDevice()
{
	if (m_dma)
	{
		DmaManager::Delete(m_dma);
	}
}

inline void UartDevice::EnableTx()
{
	if (m_is_tx_idle)
	{
		m_is_tx_idle = false;
		m_uart.SetEnableTxIrq(true);

		if (IsUseDma())
		{
			NextTxDma();
		}
	}
}

inline void UartDevice::DisableTx()
{
	m_uart.SetEnableTxIrq(false);
	m_is_tx_idle = true;
}

inline bool UartDevice::IsUseDma()
{
	return (m_tx_dma_channel != static_cast<uint8_t>(-1));
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

	m_tx_buf.PushBuffer(DynamicBlockBuffer::Block(data, size));
	EnableTx();
}

void UartDevice::SendStr(unique_ptr<char[]> &&str)
{
	const size_t size = strlen(str.get());
	if (size == 0)
	{
		return;
	}

	m_tx_buf.PushBuffer(DynamicBlockBuffer::Block(reinterpret_cast<Byte*>(
			str.release()), size));
	EnableTx();
}

void UartDevice::SendStr(string &&str)
{
	if (str.size() == 0)
	{
		return;
	}

	m_tx_buf.PushBuffer(DynamicBlockBuffer::Block(new string(std::move(str))));
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

	m_tx_buf.PushBuffer(DynamicBlockBuffer::Block(data, len));
	EnableTx();
}

void UartDevice::SendBuffer(unique_ptr<Byte[]> &&buf, const size_t len)
{
	if (len == 0)
	{
		return;
	}

	m_tx_buf.PushBuffer(DynamicBlockBuffer::Block(buf.release(), len));
	EnableTx();
}

void UartDevice::SendBuffer(vector<Byte> &&buf)
{
	if (buf.size() == 0)
	{
		return;
	}

	m_tx_buf.PushBuffer(DynamicBlockBuffer::Block(new vector<Byte>(std::move(buf))));
	EnableTx();
}

void UartDevice::SendStrLiteral(const char *str)
{
	const size_t size = strlen(str);
	if (size == 0)
	{
		return;
	}

	m_tx_buf.PushBuffer(DynamicBlockBuffer::Block((Byte*)str, size, false));
	EnableTx();
}

void UartDevice::OnTxEmpty(Uart *uart)
{
	volatile DynamicBlockBuffer::Block *block = m_tx_buf.GetActiveBlock();
	if (m_tx_buf.GetSize() == 0 && (!block || block->it == block->size))
	{
		DisableTx();
		return;
	}

	while (!block || block->it == block->size)
	{
		m_tx_buf.Acquire();
		block = m_tx_buf.GetActiveBlock();
	}

	const size_t size = block->size - block->it;
	switch (block->type)
	{
	case DynamicBlockBuffer::Block::kByteAry:
		block->it += uart->PutBytes(block->data.byte_ + block->it, size);
		break;

	case DynamicBlockBuffer::Block::kString:
		block->it += uart->PutBytes((const Byte*)block->data.string_->data()
				+ block->it, size);
		break;

	case DynamicBlockBuffer::Block::kVector:
		block->it += uart->PutBytes(block->data.vector_->data() + block->it,
				size);
		break;
	}
}

void UartDevice::OnTxDmaComplete(Dma*)
{
	volatile DynamicBlockBuffer::Block *block = m_tx_buf.GetActiveBlock();
	if (block)
	{
		block->it = block->size;
	}
	NextTxDma();
}

void UartDevice::NextTxDma()
{
	volatile DynamicBlockBuffer::Block *block = m_tx_buf.GetActiveBlock();
	if (m_tx_buf.GetSize() == 0 && (!block || block->it == block->size))
	{
		DisableTx();
		return;
	}

	while (!block || block->it == block->size)
	{
		m_tx_buf.Acquire();
		block = m_tx_buf.GetActiveBlock();
	}

	switch (block->type)
	{
	case DynamicBlockBuffer::Block::kByteAry:
		m_dma_config->src.addr = block->data.byte_;
		break;

	case DynamicBlockBuffer::Block::kString:
		m_dma_config->src.addr = (void*)block->data.string_->data();
		break;

	case DynamicBlockBuffer::Block::kVector:
		m_dma_config->src.addr = block->data.vector_->data();
		break;
	}
	m_dma_config->major_count = block->size;

	if (m_dma)
	{
		DmaManager::Delete(m_dma);
	}
	m_dma = DmaManager::New(*m_dma_config, m_tx_dma_channel);
	m_dma->Start();
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

UartDevice::UartDevice(const Initializer&)
		: UartDevice(nullptr)
{}
UartDevice::UartDevice(nullptr_t)
		: m_tx_buf(0), m_is_tx_idle(true), m_uart(nullptr)
{
	LOG_DL("Configured not to use UartDevice");
}
UartDevice::~UartDevice() {}
void UartDevice::SendStr(const char*) {}
void UartDevice::SendStr(unique_ptr<char[]>&&) {}
void UartDevice::SendStr(string&&) {}
void UartDevice::SendBuffer(const Byte*, const size_t) {}
void UartDevice::SendBuffer(unique_ptr<Byte[]>&&, const size_t) {}
void UartDevice::SendBuffer(vector<Byte>&&) {}
void UartDevice::EnableRx(const OnReceiveListener&) {}
void UartDevice::DisableRx() {}
bool UartDevice::PeekChar(char*) { return false; }

#endif /* LIBSC_USE_UART */

}
}
