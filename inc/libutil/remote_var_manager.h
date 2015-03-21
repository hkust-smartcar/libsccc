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

#include "libbase/misc_types.h"
#if MK60D10 || MK60DZ10 || MK60F15
namespace libsc
{
namespace k60
{

class UartDevice;

}
}

#elif MKL26Z4
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
#if MK60D10 || MK60DZ10 || MK60F15
	typedef libsc::k60::UartDevice UartDevice;

#elif MKL26Z4
	typedef libsc::kl26::UartDevice UartDevice;

#endif

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

	explicit RemoteVarManager(const size_t var_count);
	~RemoteVarManager();

	Var* Register(const std::string &name, const Var::Type type);
	Var* Register(std::string &&name, const Var::Type type);

	/**
	 * Broadcast the currectly registered variables
	 *
	 * @param uart
	 */
	void Broadcast(UartDevice *uart);

	/**
	 * Process new data. You should set this method as the Rx ISR while
	 * initializing the UartDevice, or call it manually
	 *
	 * @param data
	 * @return true
	 */
	bool OnUartReceiveChar(const std::vector<Byte> &data);

private:
	std::vector<Var> m_vars;
	Byte m_buffer[5];
	int m_buffer_it;
};

}
