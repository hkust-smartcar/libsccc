/*
 * clock_utils.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/hardware.h"

#include <cstdint>

#include "libbase/k60/clock_utils.h"
#include "libbase/k60/mcg.h"
#include "libbase/k60/misc_utils.h"

namespace libbase
{
namespace k60
{

namespace
{

uint32_t GetBusClock_()
{
	const Uint core_div = ((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK)
			>> SIM_CLKDIV1_OUTDIV1_SHIFT) + 1;
	const Uint bus_div = ((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK)
			>> SIM_CLKDIV1_OUTDIV2_SHIFT) + 1;
	return Mcg::Get().GetCoreClock() * core_div / bus_div;
}

}

uint32_t ClockUtils::GetCoreClock()
{
	return Mcg::Get().GetCoreClock();
}

uint32_t ClockUtils::GetCoreClockKhz()
{
	static uint32_t cache = GetCoreClock() / 1000;
	return cache;
}

Uint ClockUtils::GetCoreClockMhz()
{
	static uint32_t cache = GetCoreClock() / 1000000;
	return cache;
}

uint32_t ClockUtils::GetBusClock()
{
	static uint32_t cache = GetBusClock_();
	return cache;
}

uint32_t ClockUtils::GetBusClockKhz()
{
	static uint32_t cache = GetBusClock() / 1000;
	return cache;
}

Uint ClockUtils::GetBusClockMhz()
{
	static uint32_t cache = GetBusClock() / 1000000;
	return cache;
}

}
}
