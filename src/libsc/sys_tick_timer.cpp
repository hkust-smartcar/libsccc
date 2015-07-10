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

#include "libsc/sys_tick_timer.h"

using namespace LIBBASE_NS;

namespace libsc
{

namespace
{

SysTick::Config GetSysTickConfig(const SysTick::OnSysTickTriggerListener &isr)
{
	SysTick::Config config;
	config.count = ClockUtils::GetCoreTickPerMs();
	config.isr = isr;
	return config;
}

}

SysTickTimer::SysTickTimer()
		: m_pit(GetSysTickConfig(std::bind(&SysTickTimer::OnTick, this,
				std::placeholders::_1))),
		  m_ms(0)
{}

void SysTickTimer::OnTick(SysTick*)
{
	++m_ms;
}

}
