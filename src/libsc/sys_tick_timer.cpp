/*
 * sys_tick_timer.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstdint>

#include <functional>

#include "libbase/helper.h"
#include LIBBASE_H(clock_utils)
#include LIBBASE_H(sys_tick)

#include "libsc/system.h"
#include "libsc/sys_tick_timer.h"

using namespace LIBBASE_NS;

namespace libsc
{

namespace
{

SysTick::Config GetSysTickConfig(const SysTick::OnSysTickTriggerListener &isr)
{
	SysTick::Config config;

#ifndef USE_TIME_IN_125US
	config.count = ClockUtils::GetCoreTickPerMs();
#else
	config.count = (uint32_t)(ClockUtils::GetCoreTickPerMs() / 8.38);
#endif

	config.isr = isr;
	return config;
}

}

SysTickTimer::SysTickTimer()
		: m_pit(GetSysTickConfig(std::bind(&SysTickTimer::OnTick, this,
				std::placeholders::_1))),
#ifndef USE_TIME_IN_125US
		  m_ms(0)
#else
		  m_125us(0)
#endif
{}

void SysTickTimer::OnTick(SysTick*)
{
#ifndef USE_TIME_IN_125US
	++m_ms;
#else
	++m_125us;
#endif
}

}
