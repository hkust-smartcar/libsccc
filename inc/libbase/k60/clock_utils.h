/*
 * clock_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBBASE_K60_CLOCK_UTILS_H_
#define LIBBASE_K60_CLOCK_UTILS_H_

#include <cstdint>

#include "libbase/k60/misc_utils.h"

namespace libbase
{
namespace k60
{

class ClockUtils
{
public:
	static uint32_t GetCoreClock();
	static uint32_t GetCoreClockKhz();
	static Uint GetCoreClockMhz();
	static uint32_t GetCoreTickPerS()
	{
		return GetCoreClock();
	}
	static uint32_t GetCoreTickPerMs()
	{
		return GetCoreClockKhz();
	}
	static Uint GetCoreTickPerUs()
	{
		return GetCoreClockMhz();
	}

	static uint32_t GetBusClock();
	static uint32_t GetBusClockKhz();
	static Uint GetBusClockMhz();
	static uint32_t GetBusTickPerS()
	{
		return GetBusClock();
	}
	static uint32_t GetBusTickPerMs()
	{
		return GetBusClockKhz();
	}
	static Uint GetBusTickPerUs()
	{
		return GetBusClockMhz();
	}
};

}
}

#endif /* LIBBASE_K60_CLOCK_UTILS_H_ */
