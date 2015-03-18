/*
 * system.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstdint>

#include "libbase/k60/cache.h"

#include "libsc/config.h"
#include "libsc/k60/dwt_delay.h"
#include "libsc/system.h"
#include "libsc/sys_tick_timer.h"

using namespace libbase::k60;
using namespace libsc::k60;

namespace libsc
{

System::Impl *System::m_instance = nullptr;

struct System::Impl
{
	Impl();

	DwtDelay delay;
	SysTickTimer timer;
};

System::Impl::Impl()
{
	// Enable cache unless otherwise disabled
#if !LIBSC_NOT_USE_CACHE && MK60F15
	Cache::Config cache_conf;
	cache_conf.is_enable = true;
	Cache::Get().Init(cache_conf);
#endif
}

void System::Init()
{
	if (!m_instance)
	{
		m_instance = new Impl;
	}
}

void System::DelayUs(const uint16_t us)
{
	assert(m_instance);
	m_instance->delay.DelayUs(us);
}

void System::DelayMs(const uint16_t ms)
{
	assert(m_instance);
	m_instance->delay.DelayMs(ms);
}

void System::DelayS(const uint16_t s)
{
	assert(m_instance);
	m_instance->delay.DelayS(s);
}

Timer::TimerInt System::Time()
{
	assert(m_instance);
	return m_instance->timer.Time();
}

}
