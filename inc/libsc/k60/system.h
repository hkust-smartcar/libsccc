/*
 * system.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_K60_SYSTEM_H_
#define LIBSC_K60_SYSTEM_H_

#include <cstdint>

#include "libsc/k60/pit_timer.h"
#include "libsc/k60/sys_tick_delay.h"

namespace libsc
{
namespace k60
{

class System
{
public:
	static void Init()
	{
		if (!m_instance)
		{
			m_instance = new System;
		}
	}

	static void DelayUs(const uint16_t us)
	{
		m_instance->m_delay.DelayUs(us);
	}

	static void DelayMs(const uint16_t ms)
	{
		m_instance->m_delay.DelayMs(ms);
	}

	static void DelayS(const uint16_t s)
	{
		m_instance->m_delay.DelayS(s);
	}

	static Timer::TimerInt Time()
	{
		return m_instance->m_timer.Time();
	}

private:
	System();

	SysTickDelay m_delay;
	PitTimer m_timer;

	static System *m_instance;
};

}
}

#endif /* LIBSC_K60_SYSTEM_H_ */
