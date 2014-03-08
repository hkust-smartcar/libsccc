/*
 * tunable_int_manager.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBUTIL_TUNABLE_INT_MANAGER_H_
#define LIBUTIL_TUNABLE_INT_MANAGER_H_

#include <cstdint>

namespace libsc
{

class UartDevice;

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

private:
	TunableInt();

	void SetValue(const uint32_t val)
	{
		m_val = val;
	}

	const char *m_name;
	uint8_t m_id;
	volatile uint32_t m_val;

	template<uint8_t size> friend class TunableIntManager;
};

template<uint8_t size>
class TunableIntManager
{
public:
	explicit TunableIntManager(libsc::UartDevice *uart);

	const TunableInt* Register(const char *name, const uint32_t val);
	void Start();
	void Stop();

private:
	static void OnUartReceiveChar(const char ch);

	libsc::UartDevice *m_uart;
	TunableInt m_data[size];
	uint8_t m_curr_id;
	char m_buffer[5];
	int m_buffer_it;

	static TunableIntManager *m_instance;
};

template<uint8_t size>
TunableIntManager<size> *TunableIntManager<size>::m_instance = nullptr;

}

#endif /* LIBUTIL_TUNABLE_INT_MANAGER_H_ */
