/*
 * system_timer.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_K60_SYSTEM_TIMER_H_
#define LIBSC_K60_SYSTEM_TIMER_H_

#include <cstdint>

#include "libsc/k60/timer.h"

namespace libsc
{
namespace k60
{

class SystemTimer
{
public:
	static void Init(const uint8_t pit_channel)
	{
		if (!m_timer)
		{
			m_timer = new Timer(pit_channel);
		}
	}

	static Timer::TimerInt Time()
	{
		return m_timer ? m_timer->Time() : 0;
	}

private:
	static Timer *m_timer;
};

}
}

#endif /* LIBSC_K60_SYSTEM_TIMER_H_ */
