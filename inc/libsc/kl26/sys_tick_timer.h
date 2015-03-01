/*
 * sys_tick_timer.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/kl26/sys_tick.h"

#include "libsc/timer.h"

namespace libsc
{
namespace kl26
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
	void OnTick(libbase::kl26::SysTick*);

	libbase::kl26::SysTick m_pit;
	volatile TimerInt m_ms;
};

}
}
