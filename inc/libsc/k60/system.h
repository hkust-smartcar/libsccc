/*
 * system.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/k60/mcg.h"
#include "libbase/k60/watchdog.h"

#include "libsc/sys_tick_timer.h"
#include "libsc/k60/dwt_delay.h"

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

	__attribute__((__weak__))
	static libbase::k60::Watchdog::Config GetWatchdogConfig();

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

	/**
	 * Return the time elapsed, in ms, since Init()
	 *
	 * @return
	 */
	static Timer::TimerInt Time()
	{
		return m_instance->m_timer.Time();
	}

	static void FeedDog()
	{
		//m_instance->m_watchdog.Refresh();
	}

private:
	System();

	// Watchdog is currently broken
	//libbase::k60::Watchdog m_watchdog;

	DwtDelay m_delay;
	SysTickTimer m_timer;

	static System *m_instance;
};

}
}
