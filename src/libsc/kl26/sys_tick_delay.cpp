/*
 * sys_tick_delay.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstdint>

#include "libbase/kl26/clock_utils.h"
#include "libbase/kl26/sys_tick.h"

#include "libsc/kl26/sys_tick_delay.h"

using namespace libbase::kl26;

namespace libsc
{
namespace kl26
{

namespace
{

SysTick::Config GetSysTickConfig()
{
	SysTick::Config sc;
	sc.is_enable = false;
	return sc;
}

}

SysTickDelay::SysTickDelay()
		: m_sys_tick(GetSysTickConfig())
{}

inline void SysTickDelay::DelayCount(const uint32_t count)
{
	m_sys_tick.SetCount(count);
	m_sys_tick.SetEnable(true);
	m_sys_tick.ConsumeInterrupt();
	while (!m_sys_tick.IsInterruptRequested())
	{}
	m_sys_tick.SetEnable(false);
}

void SysTickDelay::DelayUs(const uint16_t us)
{
	DelayCount(ClockUtils::GetCoreTickPerUs(us));
}

}
}
