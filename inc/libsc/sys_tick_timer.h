/*
 * sys_tick_timer.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/helper.h"
#include LIBBASE_H(sys_tick)

#include "libsc/timer.h"

namespace libsc
{

class SysTickTimer : public Timer
{
public:
	SysTickTimer();

#ifndef USE_TIME_IN_125US
	TimerInt Time() override
	{
		return m_ms;
	}

#else
	TimerInt Time() override
	{
		return m_125us >> 3;
	}

	TimerInt TimeIn125us()
	{
		return m_125us;
	}
#endif

private:
	void OnTick(LIBBASE_MODULE(SysTick)*);

	LIBBASE_MODULE(SysTick) m_pit;

#ifndef USE_TIME_IN_125US
	volatile TimerInt m_ms;
#else
	volatile TimerInt m_125us;
#endif
};

}
