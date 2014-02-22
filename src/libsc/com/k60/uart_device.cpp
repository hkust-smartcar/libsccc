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

#include <vectors.h>
#include <MK60_uart.h>

#include "macro.h"
#include "libutil/misc.h"
#include "libsc/com/config.h"
#include "libsc/com/uart_device.h"

#define UART_VECTOR(x) static_cast<VECTORn_t>((x << 1) + UART0_RX_TX_VECTORn)
#define UARTX(x) static_cast<UARTn_e>(x + UART0)

#define CHUNK_SIZE 16

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
			: start(0), end(0), next(nullptr)
	{
		memset(data, 0, CHUNK_SIZE);
	}

	char data[CHUNK_SIZE];
	uint8_t start;
	uint8_t end;
	Chunk *next;
};

UartDevice::UartDevice(const uint8_t uart_port, const uint32_t baud_rate)
		: m_uart_port(uart_port)
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

	m_tail = m_head = new Chunk;
	uart_init(UARTX(uart_port), baud_rate);
}

UartDevice::~UartDevice()
{
	if (m_device_id >= LIBSC_USE_UART)
	{
		return;
	}

	StopReceive();

	Chunk *next = nullptr;
	for (Chunk *c = m_head; c; c = next)
	{
		next = c->next;
		SAFE_DELETE(c);
	}

	g_uart_instances[m_device_id] = nullptr;
}

void UartDevice::StartReceive()
{
	SetIsr(UART_VECTOR(m_uart_port), IrqHandler);
	uart_rx_irq_en(UARTX(m_uart_port));
}

void UartDevice::StopReceive()
{
	uart_rx_irq_dis(UARTX(m_uart_port));
	SetIsr(UART_VECTOR(m_uart_port), DefaultIsr);
}

void UartDevice::SendChar(const char ch)
{
	uart_putchar(UARTX(m_uart_port), ch);
}

void UartDevice::SendStr(const char *str)
{
	uart_putstr(UARTX(m_uart_port), str);
}

void UartDevice::SendBuffer(const uint8_t *buf, const uint32_t len)
{
	uart_putbuff(UARTX(m_uart_port), buf, len);
}

bool UartDevice::PeekChar(char *out_char)
{
	Chunk *tail = const_cast<Chunk*>(m_tail);
	if (m_head == tail && m_head->start == m_head->end)
	{
		return false;
	}

	*out_char = m_head->data[m_head->start];
	if (++m_head->start == CHUNK_SIZE)
	{
		Chunk *pop = m_head;
		m_head = m_head->next;
		SAFE_DELETE(pop);
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
			Chunk *tail = const_cast<Chunk*>(g_uart_instances[i]->m_tail);
			while (uart_querychar(LIBSC_BT_UART, &tail->data[tail->end]))
			{
				if (++tail->end == CHUNK_SIZE)
				{
					g_uart_instances[i]->m_tail = tail = tail->next = new Chunk;
				}
			}
			return;
		}
	}
}

#else
UartDevice::UartDevice(const uint8_t) {}
UartDevice::~UartDevice() {}
void UartDevice::StartReceive() {}
void UartDevice::StopReceive() {}
void UartDevice::SendChar(const char) {}
void UartDevice::SendStr(const char*) {}
bool UartDevice::PeekChar(char*) { return false; }

#endif

}
