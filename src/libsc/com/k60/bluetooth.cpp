/*
 * bluetooth.cpp
 * Bluetooth abstraction
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>
#include <hw_common.h>
#include <cstdint>
#include <cstring>

#include <vectors.h>
#include <MK60_uart.h>

#include "macro.h"
#include "libutil/misc.h"
#include "libsc/com/config.h"
#include "libsc/com/bluetooth.h"

#define UART_VECTOR_(x) LIBSC_JOIN(x, _RX_TX_VECTORn)
#define UART_VECTOR UART_VECTOR_(LIBSC_BT_UART)

#define CHUNK_SIZE 16

namespace libsc
{

#ifdef LIBSC_USE_BT

struct Bluetooth::Chunk
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

Bluetooth *Bluetooth::m_instance = nullptr;
bool Bluetooth::m_is_initialized = false;

Bluetooth::Bluetooth()
{
	m_tail = m_head = new Chunk;
	if (!m_is_initialized)
	{
		uart_init(LIBSC_BT_UART, 115200);
		m_is_initialized = true;
	}
}

Bluetooth::~Bluetooth()
{
	if (this == m_instance)
	{
		StopReceive();
	}

	Chunk *next = nullptr;
	for (Chunk *c = m_head; c; c = next)
	{
		next = c->next;
		SAFE_DELETE(c);
	}
}

void Bluetooth::StartReceive()
{
	m_instance = this;
	SetIsr(UART_VECTOR, IrqHandler);
	uart_rx_irq_en(LIBSC_BT_UART);
}

void Bluetooth::StopReceive()
{
	uart_rx_irq_dis(LIBSC_BT_UART);
	SetIsr(UART_VECTOR, DefaultIsr);
	m_instance = nullptr;
}

void Bluetooth::SendChar(const char ch)
{
	uart_putchar(LIBSC_BT_UART, ch);
}

void Bluetooth::SendStr(const char *str)
{
	uart_putstr(LIBSC_BT_UART, str);
}

void Bluetooth::SendBuffer(const uint8_t *buf, const uint32_t len)
{
	uart_putbuff(LIBSC_BT_UART, buf, len);
}

bool Bluetooth::PeekChar(char *out_char)
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

void Bluetooth::IrqHandler()
{
	Chunk *tail = const_cast<Chunk*>(m_instance->m_tail);
	while (uart_querychar(LIBSC_BT_UART, &tail->data[tail->end]))
	{
		if (++tail->end == CHUNK_SIZE)
		{
			m_instance->m_tail = tail = tail->next = new Chunk;
		}
	}
}

#else
Bluetooth::Bluetooth() {}
Bluetooth::~Bluetooth() {}
void Bluetooth::StartReceive() {}
void Bluetooth::StopReceive() {}
void Bluetooth::SendChar(const char ch) {}
void Bluetooth::SendStr(const char *str) {}
bool Bluetooth::PeekChar(char *out_char) { return false; }

#endif

}
