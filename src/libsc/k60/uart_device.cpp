/*
 * uart_device.cpp
 * UART device abstraction
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>
#include <hw_common.h>

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

#define CHUNK_SIZE 64
// Maximum number of chunk allocated for Tx
#define MAX_TX_CHUNK 8

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_UART

struct UartDevice::Chunk
{
	Chunk()
			: start(0), end(0)
	{
		memset(data, 0, CHUNK_SIZE);
	}

	Byte data[CHUNK_SIZE];
	uint8_t start;
	uint8_t end;
};

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

UartConfig GetUartConfig(const uint8_t id, const UartConfig::BaudRate baud_rate)
{
	UartConfig config;
	config.tx_pin = GetTxPin(id);
	config.rx_pin = GetRxPin(id);
	config.baud_rate = baud_rate;
	config.config.set(UartConfig::ConfigBit::FIFO);
	return config;
}

}

UartDevice::UartDevice(const uint8_t id, const UartConfig::BaudRate baud_rate)
		: m_listener(nullptr), m_tx_buf_size(0), m_is_tx_idle(true),
		  m_uart(GetUartConfig(id, baud_rate))
{}

UartDevice::~UartDevice()
{
	StopReceive();
}

void UartDevice::StartReceive(OnReceiveListener listener)
{
	m_listener = listener;
	m_uart.SetRxIsr(std::bind(&UartDevice::OnRxFull, this, std::placeholders::_1));
}

void UartDevice::StopReceive()
{
	m_listener = nullptr;
	m_uart.SetRxIsr(nullptr);
}

void UartDevice::SendByte(const Byte b)
{
	if (m_tx_buf_size == 0)
	{
		m_tx_buf.push_back(Chunk());
		++m_tx_buf_size;
	}
	if (m_tx_buf.back().end == CHUNK_SIZE)
	{
		m_tx_buf.push_back(Chunk());
		++m_tx_buf_size;
	}
	m_tx_buf.back().data[m_tx_buf.back().end++] = b;

	if (m_is_tx_idle)
	{
		m_is_tx_idle = false;
		m_uart.SetTxIsr(std::bind(&UartDevice::OnTxEmpty, this,
				std::placeholders::_1));
	}
}

bool UartDevice::PeekChar(char *out_char)
{
	if (m_rx_buf.empty() || m_rx_buf.front().start == m_rx_buf.front().end)
	{
		return false;
	}

	Chunk *front = &m_rx_buf.front();
	*out_char = front->data[front->start];
	if (++front->start == CHUNK_SIZE)
	{
		m_rx_buf.pop_front();
	}
	return true;
}

void UartDevice::OnTxEmpty(Uart *uart)
{
	if (m_tx_buf.empty() || m_tx_buf.front().start == m_tx_buf.front().end)
	{
		m_is_tx_idle = true;
		uart->SetTxIsr(nullptr);
		return;
	}

	while (m_tx_buf_size >= MAX_TX_CHUNK)
	{
		m_tx_buf.pop_front();
		--m_tx_buf_size;
	}

	Chunk *front = &m_tx_buf.front();
	const size_t size = front->end - front->start;
	front->start += uart->SendBytes(&front->data[front->start], size);
	if (front->start == CHUNK_SIZE)
	{
		m_tx_buf.pop_front();
		--m_tx_buf_size;
	}
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
		if (m_rx_buf.empty())
		{
			m_rx_buf.push_back(Chunk());
		}
		size_t i = 0;
		while (i < bytes.size())
		{
			Chunk *back = &m_rx_buf.back();
			const size_t size = std::min<size_t>(CHUNK_SIZE - back->end,
					bytes.size() - i);
			memcpy(&back->data[back->end], &bytes[i], size);
			back->end += size;
			if (back->end >= CHUNK_SIZE)
			{
				m_rx_buf.push_back(Chunk());
			}
			i += size;
		}
	}
	else
	{
		m_listener(bytes.data(), bytes.size());
	}
}

#else /* LIBSC_USE_UART */

struct UartDevice::Impl
{};

UartDevice::UartDevice(const uint8_t, const uint32_t)
{
	LOG_D("Configured not to use UartDevice");
}
UartDevice::~UartDevice() {}
void UartDevice::StartReceive(OnReceiveListener) {}
void UartDevice::StopReceive() {}
void UartDevice::SendByte(const Byte) {}
bool UartDevice::PeekChar(char*) { return false; }

#endif /* LIBSC_USE_UART */

}
}
