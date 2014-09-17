/*
 * sys_tick_timer.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cstdint>

#include <functional>

#include "libbase/k60/clock_utils.h"
#include "libbase/k60/sys_tick.h"

#include "libsc/k60/sys_tick_timer.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
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
}
