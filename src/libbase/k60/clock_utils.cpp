/*
 * clock_utils.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include "libbase/k60/hardware.h"

#include <cstdint>

#include "libbase/k60/clock_utils.h"
#include "libbase/k60/misc_utils.h"

namespace libbase
{
namespace k60
{

uint32_t ClockUtils::GetCoreClock()
{
	static uint32_t cache = core_clk_khz * 1000;
	return cache;
}

uint32_t ClockUtils::GetCoreClockKhz()
{
	return core_clk_khz;
}

Uint ClockUtils::GetCoreClockMhz()
{
	return core_clk_mhz;
}

uint32_t ClockUtils::GetBusClock()
{
	static uint32_t cache = bus_clk_khz * 1000;
	return cache;
}

uint32_t ClockUtils::GetBusClockKhz()
{
	return bus_clk_khz;
}

Uint ClockUtils::GetBusClockMhz()
{
	static uint32_t cache = bus_clk_khz / 1000;
	return cache;
}

}
}
