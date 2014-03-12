/*
 * tunable_int_manager.tcc
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>
#include <hw_common.h>
#include <cstdint>

#include <assert.h>

#include "libsc/com/uart_device.h"

#include "libutil/string.h"
#include "libutil/tunable_int_manager.h"

namespace libutil
{

template<uint8_t size>
TunableIntManager<size>::TunableIntManager(libsc::UartDevice *uart)
		: m_uart(uart), m_curr_id(0), m_buffer_it(0)
{}

template<uint8_t size>
const TunableInt* TunableIntManager<size>::Register(const char *name,
		const char *type, const uint32_t val)
{
	if (m_curr_id >= size)
	{
		assert(false);
	}

	TunableInt *var = &m_data[m_curr_id];
	var->m_name = name;
	var->m_type = type;
	var->m_id = m_curr_id;
	var->m_val = val;
	++m_curr_id;
	return var;
}

template<uint8_t size>
void TunableIntManager<size>::Start()
{
	m_uart->StartReceive(OnUartReceiveChar);
	m_instance = this;

	for (int i = 0; i < size && m_data[i].m_name; ++i)
	{
		m_uart->SendStr(String::Format("%s,%s,%d\n", m_data[i].m_name,
				m_data[i].m_type, m_data[i].m_id).c_str());
	}
}

template<uint8_t size>
void TunableIntManager<size>::Stop()
{
	m_uart->StopReceive();
	m_instance = nullptr;
}

template<uint8_t size>
void TunableIntManager<size>::OnUartReceiveChar(const char ch)
{
	m_instance->m_buffer[m_instance->m_buffer_it] = ch;
	if (++m_instance->m_buffer_it >= 5)
	{
		m_instance->m_data[(Uint)m_instance->m_buffer[0]].SetValue(
				m_instance->m_buffer[1] << 24 | m_instance->m_buffer[2] << 16
				| m_instance->m_buffer[3] << 8 | m_instance->m_buffer[4]);
		m_instance->m_buffer_it = 0;
	}
}

}
