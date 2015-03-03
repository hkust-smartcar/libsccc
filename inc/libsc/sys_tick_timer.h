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

	TimerInt Time() override
	{
		return m_ms;
	}

private:
	void OnTick(LIBBASE_MODULE(SysTick)*);

	LIBBASE_MODULE(SysTick) m_pit;
	volatile TimerInt m_ms;
};

}
