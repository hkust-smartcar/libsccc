/*
 * sys_tick.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBBASE_K60_SYS_TICK_H_
#define LIBBASE_K60_SYS_TICK_H_

#include <cstdint>

#ifdef SysTick
#undef SysTick
#endif

namespace libbase
{
namespace k60
{

class SysTick
{
public:
	static void DelayUs(const uint16_t us);
};

}
}

#endif /* LIBBASE_K60_SYS_TICK_H_ */
