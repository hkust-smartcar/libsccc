/*
 * sys_tick_timer.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstdint>

#include <functional>

#include "libbase/kl26/clock_utils.h"
#include "libbase/kl26/sys_tick.h"

#include "libsc/kl26/sys_tick_timer.h"

using namespace libbase::kl26;

namespace libsc
{
namespace kl26
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
