/*
 * tunable_int_manager.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBUTIL_TUNABLE_INT_MANAGER_H_
#define LIBUTIL_TUNABLE_INT_MANAGER_H_

#include <cstddef>
#include <cstdint>

#include "libbase/k60/misc_utils.h"

namespace libsc
{
namespace k60
{

class UartDevice;

}
}

namespace libutil
{

class TunableInt
{
public:
	uint32_t GetValue() const
	{
		return m_val;
	}

	static float AsFloat(const uint32_t val)
	{
		return *reinterpret_cast<const float*>(&val);
	}

	static int32_t AsSigned(const uint32_t val)
	{
		return *reinterpret_cast<const int32_t*>(&val);
	}

	static uint32_t AsUnsigned(const int32_t val)
	{
		return *reinterpret_cast<const uint32_t*>(&val);
	}

	static uint32_t AsUnsigned(const float val)
	{
		return *reinterpret_cast<const uint32_t*>(&val);
	}

private:
	TunableInt();

	void SetValue(const uint32_t val)
	{
		m_val = val;
	}

	const char *m_name;
	const char *m_type;
	uint8_t m_id;
	volatile uint32_t m_val;

	template<uint8_t size> friend class TunableIntManager;
};

template<uint8_t size>
class TunableIntManager
{
public:
	explicit TunableIntManager(libsc::k60::UartDevice *uart);

	const TunableInt* Register(const char *name, const char *type,
			const uint32_t val);
	void Start();
	void Stop();

private:
	void OnUartReceiveChar(const Byte *bytes, const size_t count);

	libsc::k60::UartDevice *m_uart;
	TunableInt m_data[size];
	uint8_t m_curr_id;
	char m_buffer[5];
	int m_buffer_it;
};

}

#endif /* LIBUTIL_TUNABLE_INT_MANAGER_H_ */
