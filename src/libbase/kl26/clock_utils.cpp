/*
 * clock_utils.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */


#include "libbase/kl26/hardware.h"

#include <cstdint>

#include "libbase/kl26/clock_utils.h"
#include "libbase/kl26/mcg.h"
#include "libbase/kl26/misc_utils.h"
#include "libbase/kl26/sim.h"

namespace libbase
{
namespace kl26
{

namespace
{

uint32_t GetBusClock_()
{
	return Mcg::Get().GetCoreClock() / Sim::GetBusClockDivider();
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

uint32_t ClockUtils::GetCoreClockMhz()
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

uint32_t ClockUtils::GetBusClockMhz()
{
	static uint32_t cache = GetBusClock() / 1000000;
	return cache;
}

}
}

