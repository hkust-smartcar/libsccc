/*
 * system.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libsc/timer.h"

#define USE_TIME_IN_125US

namespace libsc
{

class System
{
public:
	static void Init();

	static void DelayUs(const uint16_t us);
	static void DelayMs(const uint16_t ms);
	static void DelayS(const uint16_t s);

	/**
	 * Return the time elapsed, in ms, since Init()
	 *
	 * @return
	 */
	static Timer::TimerInt Time();
#ifdef USE_TIME_IN_125US
	static Timer::TimerInt TimeIn125us();
#endif

private:
	struct Impl;
	static Impl *m_instance;
};

}
