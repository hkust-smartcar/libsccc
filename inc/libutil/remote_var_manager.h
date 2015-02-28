/*
 * remote_var_manager.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>
#ifdef MK60D10 || MK60DZ10 || MK60F15
#include "libbase/k60/misc_utils.h"

namespace libsc
{
namespace k60
{

class UartDevice;

}
}
#endif

#ifdef MKL26Z4
#include "libbase/kl26/misc_utils.h"

namespace libsc
{
namespace kl26
{

class UartDevice;

}
}
#endif

namespace libutil
{

class RemoteVarManager
{
public:
	class Var
	{
	public:
		enum struct Type
		{
			kInt,
			kReal,
		};

		Var(Var &&rhs);

		Var& operator=(Var &&rhs);

		uint8_t GetId() const
		{
			return m_id;
		}

		uint32_t GetInt() const
		{
			return m_val;
		}

		float GetReal() const
		{
			return *reinterpret_cast<const volatile float*>(&m_val);
		}

		void SetInt(const uint32_t val)
		{
			m_val = val;
		}

		void SetReal(const float val)
		{
			m_val = *reinterpret_cast<const uint32_t*>(&val);
		}

	private:
		Var();

		std::string m_name;
		Type m_type;
		uint8_t m_id;
		volatile uint32_t m_val;

		friend class RemoteVarManager;
	};

	RemoteVarManager(UartDevice *uart, const size_t var_count);
	~RemoteVarManager();

	Var* Register(const std::string &name, const Var::Type type);
	Var* Register(std::string &&name, const Var::Type type);

	void Start(const bool is_broadcast);
	void Start()
	{
		Start(true);
	}
	void Stop();

private:
	void OnUartReceiveChar(const Byte *bytes, const size_t count);

	UartDevice *m_uart;
	std::vector<Var> m_vars;
	Byte m_buffer[5];
	int m_buffer_it;
};

}
