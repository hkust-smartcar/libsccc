/*
 * remote_var_manager.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include "libbase/log.h"
#include "libbase/k60/misc_utils.h"

#include "libsc/k60/uart_device.h"
#include "libutil/remote_var_manager.h"
#include "libutil/string.h"

using namespace libsc::k60;
using namespace std;

namespace libutil
{

namespace
{

float AsFloat(const uint32_t val)
{
	return *reinterpret_cast<const float*>(&val);
}

}

RemoteVarManager::Var::Var()
		: m_type(Type::INT),
		  m_id(0),
		  m_val(0)
{}

RemoteVarManager::Var::Var(Var &&rhs)
		: Var()
{
	*this = std::move(rhs);
}

RemoteVarManager::Var& RemoteVarManager::Var::operator=(Var &&rhs)
{
	if (this != &rhs)
	{
		m_name = std::move(rhs.m_name);
		m_type = rhs.m_type;
		m_id = rhs.m_id;
		m_val = rhs.m_val;
	}
	return *this;
}

RemoteVarManager::RemoteVarManager(UartDevice *uart, const size_t var_count)
		: m_uart(uart), m_buffer_it(0)
{
	m_vars.reserve(var_count);
}

RemoteVarManager::~RemoteVarManager()
{
	m_uart->DisableRx();
}

RemoteVarManager::Var* RemoteVarManager::Register(const string &name,
		const Var::Type type)
{
	if (m_vars.size() >= m_vars.capacity())
	{
		LOG_WL("RemoteVarManager expanding");
		assert(false);
		return nullptr;
	}

	Var var;
	var.m_name = name;
	var.m_type = type;
	var.m_id = m_vars.size();
	m_vars.push_back(std::move(var));
	return &m_vars.back();
}

RemoteVarManager::Var* RemoteVarManager::Register(string &&name,
		const Var::Type type)
{
	if (m_vars.size() >= m_vars.capacity())
	{
		LOG_WL("RemoteVarManager expanding");
		assert(false);
		return nullptr;
	}

	Var var;
	var.m_name = std::move(name);
	var.m_type = type;
	var.m_id = m_vars.size();
	m_vars.push_back(std::move(var));
	return &m_vars.back();
}

void RemoteVarManager::Start(const bool is_broadcast)
{
	m_uart->EnableRx([this](const Byte *bytes, const size_t count)
			{
				OnUartReceiveChar(bytes, count);
			});

	if (is_broadcast)
	{
		for (size_t i = 0; i < m_vars.size(); ++i)
		{
			if (m_vars[i].m_type == Var::Type::INT)
			{
				m_uart->SendStr(String::Format("%s,int,%d,%d\n",
						m_vars[i].m_name.c_str(), m_vars[i].m_id,
						htobe32(m_vars[i].m_val)));
			}
			else
			{
				m_uart->SendStr(String::Format("%s,real,%d,%.3f\n",
						m_vars[i].m_name.c_str(), m_vars[i].m_id,
						AsFloat(htobe32(m_vars[i].m_val))));
			}
		}
	}
}

void RemoteVarManager::Stop()
{
	m_uart->DisableRx();
}

void RemoteVarManager::OnUartReceiveChar(const Byte *bytes, const size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		m_buffer[m_buffer_it] = bytes[i];
		if (++m_buffer_it >= 5)
		{
			const uint32_t val = m_buffer[1] << 24 | m_buffer[2] << 16
					| m_buffer[3] << 8 | m_buffer[4];
			m_vars[(size_t)m_buffer[0]].m_val = val;
			m_buffer_it = 0;
		}
	}
}

}
