/*
 * sys_tick_delay.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_K60_SYS_TICK_DELAY_H_
#define LIBSC_K60_SYS_TICK_DELAY_H_

#include <cstdint>

#include "libbase/k60/sys_tick.h"

#include "libsc/k60/delay.h"

namespace libsc
{
namespace k60
{

class SysTickDelay : public Delay
{
public:
	void DelayUs(const uint16_t us) override
	{
		libbase::k60::SysTick::DelayUs(us);
	}
};

}
}

#endif /* LIBSC_K60_SYS_TICK_DELAY_H_ */
