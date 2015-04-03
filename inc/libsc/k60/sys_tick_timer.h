/*
 * sys_tick_timer.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/k60/sys_tick.h"

#include "libsc/k60/timer.h"

namespace libsc
{
namespace k60
{

class SysTickTimer : public Timer
{
public:
	SysTickTimer();

	TimerInt Time() override
	{
		return m_ms;
	}

private:
	void OnTick(libbase::k60::SysTick*);

	libbase::k60::SysTick m_pit;
	volatile TimerInt m_ms;
};

}
}
