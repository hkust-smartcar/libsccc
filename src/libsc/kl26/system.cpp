/*
 * system.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstdint>

#include "libsc/config.h"
#include "libsc/kl26/lptmr_timer.h"
#include "libsc/system.h"
#include "libsc/sys_tick_delay.h"

using namespace libbase::kl26;
using namespace libsc::kl26;

namespace libsc
{

System::Impl *System::m_instance = nullptr;

struct System::Impl
{
	SysTickDelay delay;
	LptmrTimer timer;
};

void System::Init()
{
	if (!m_instance)
	{
		m_instance = new Impl;
	}
}

void System::DelayUs(const uint16_t us)
{
	m_instance->delay.DelayUs(us);
}

void System::DelayMs(const uint16_t ms)
{
	m_instance->delay.DelayMs(ms);
}

void System::DelayS(const uint16_t s)
{
	m_instance->delay.DelayS(s);
}

Timer::TimerInt System::Time()
{
	return m_instance->timer.Time();
}

}
