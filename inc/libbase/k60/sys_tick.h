/*
 * sys_tick.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

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
