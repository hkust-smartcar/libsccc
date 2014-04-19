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

#include <list>

#include <vectors.h>
#include <MK60_uart.h>

#include "libsc/com/config.h"
#include "libsc/com/uart_device.h"
#include "libutil/misc.h"
#include "macro.h"

using namespace std;

#define UART_VECTOR(x) static_cast<VECTORn_t>((x << 1) + UART0_RX_TX_VECTORn)
#define UARTX(x) static_cast<UARTn_e>(x + UART0)

#define CHUNK_SIZE 64
// Maximum number of chunk allocated for Tx
#define MAX_TX_CHUNK 8

namespace libsc
{

#ifdef LIBSC_USE_UART

namespace
{

UartDevice* g_uart_instances[LIBSC_USE_UART];

}

struct UartDevice::Chunk
{
	Chunk()
			: start(0), end(0)
	{
		memset(data, 0, CHUNK_SIZE);
	}

	char data[CHUNK_SIZE];
	uint8_t start;
	uint8_t end;
};

UartDevice::UartDevice(const uint8_t uart_port, const uint32_t baud_rate)
		: m_listener(nullptr), m_send_buf_size(0), m_is_tx_idle(true),
		  m_uart_port(uart_port)
{
	for (m_device_id = 0; m_device_id < LIBSC_USE_UART; ++m_device_id)
	{
		if (!g_uart_instances[m_device_id])
		{
			g_uart_instances[m_device_id] = this;
			break;
		}
	}
	if (m_device_id >= LIBSC_USE_UART)
	{
		LOG_E("More UART device used than defined in config file");
		return;
	}

	uart_init(UARTX(uart_port), baud_rate);
	m_txfifo_size = 1 << (UART_PFIFO_TXFIFOSIZE(UART_PFIFO_REG(UARTN[m_uart_port])));
	if (m_txfifo_size != 1)
	{
		m_txfifo_size <<= 1;
	}
}

UartDevice::~UartDevice()
{
	if (m_device_id >= LIBSC_USE_UART)
	{
		return;
	}

	StopReceive();

	g_uart_instances[m_device_id] = nullptr;
}

void UartDevice::StartReceive(OnReceiveCharListener listener)
{
	SetIsr(UART_VECTOR(m_uart_port), IrqHandler);
	m_listener = listener;
	uart_rx_irq_en(UARTX(m_uart_port));
}

void UartDevice::StopReceive()
{
	uart_rx_irq_dis(UARTX(m_uart_port));
	m_listener = nullptr;
	SetIsr(UART_VECTOR(m_uart_port), DefaultIsr);
}

void UartDevice::SendChar(const char ch)
{
	if (m_send_buf_size == 0)
	{
		m_send_buf.push_back(Chunk());
		++m_send_buf_size;
	}
	if (m_send_buf.back().end == CHUNK_SIZE)
	{
		if (m_send_buf_size >= MAX_TX_CHUNK)
		{
			m_send_buf.pop_front();
			m_send_buf.push_back(Chunk());
		}
		else
		{
			m_send_buf.push_back(Chunk());
			++m_send_buf_size;
		}
	}
	m_send_buf.back().data[m_send_buf.back().end++] = ch;

	if (m_is_tx_idle)
	{
		m_is_tx_idle = false;
		uart_txc_irq_en(UARTX(m_uart_port));
	}
}

void UartDevice::SendStr(const char *str)
{
	while (*str)
	{
		SendChar(*str++);
	}
}

void UartDevice::SendBuffer(const uint8_t *buf, const uint32_t len)
{
	for (uint32_t i = 0; i < len; ++i)
	{
		SendChar(static_cast<char>(buf[i]));
	}
}

bool UartDevice::PeekChar(char *out_char)
{
	if (m_receive_buf.empty()
			|| m_receive_buf.front().start == m_receive_buf.front().end)
	{
		return false;
	}

	*out_char = m_receive_buf.front().data[m_receive_buf.front().start];
	if (++m_receive_buf.front().start == CHUNK_SIZE)
	{
		m_receive_buf.pop_front();
	}
	return true;
}

void UartDevice::IrqHandler()
{
	const VECTORn_t v = GetVectorX();
	const uint8_t uart_port = ((v - UART0_RX_TX_VECTORn) >> 1);

	for (int i = 0; i < LIBSC_USE_UART; ++i)
	{
		if (g_uart_instances[i] && g_uart_instances[i]->m_uart_port == uart_port)
		{
			if (UART_S1_REG(UARTN[g_uart_instances[i]->m_uart_port])
					& UART_S1_RDRF_MASK)
			{
				g_uart_instances[i]->OnInterruptRx();
			}
			else
			{
				g_uart_instances[i]->OnInterruptTx();
			}
			return;
		}
	}
}

void UartDevice::OnInterruptRx()
{
	char ch;
	while (uart_querychar(UARTX(m_uart_port), &ch))
	{
		if (!m_listener)
		{
			if (m_receive_buf.empty())
			{
				m_receive_buf.push_back(Chunk());
			}
			m_receive_buf.back().data[m_receive_buf.back().end] = ch;
			if (++m_receive_buf.back().end == CHUNK_SIZE)
			{
				m_receive_buf.push_back(Chunk());
			}
		}
		else
		{
			m_listener(ch);
		}
	}
}

void UartDevice::OnInterruptTx()
{
	if (m_send_buf_size == 0
			|| m_send_buf.front().start == m_send_buf.front().end)
	{
		m_is_tx_idle = true;
		uart_txc_irq_dis(UARTX(m_uart_port));
		return;
	}

	for (int i = UART_TCFIFO_TXCOUNT(UART_TCFIFO_REG(UARTN[m_uart_port]));
			i < m_txfifo_size; ++i)
	{
		//uart_putchar(UARTX(m_uart_port),
		//		m_send_buf.front().data[m_send_buf.front().start]);
		UART_D_REG(UARTN[m_uart_port]) =
				(uint8_t)m_send_buf.front().data[m_send_buf.front().start];
		if (++m_send_buf.front().start == CHUNK_SIZE)
		{
			--m_send_buf_size;
			m_send_buf.pop_front();
		}
	}
}

#else
UartDevice::UartDevice(const uint8_t)
		: m_listener(nullptr), m_send_buf_size(0), m_is_tx_idle(true),
		  m_uart_port(0), m_device_id(0), m_txfifo_size(1)
{}
UartDevice::~UartDevice() {}
void UartDevice::StartReceive() {}
void UartDevice::StopReceive() {}
void UartDevice::SendChar(const char) {}
void UartDevice::SendStr(const char*) {}
bool UartDevice::PeekChar(char*) { return false; }

#endif

}
