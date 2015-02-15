/*
 * clock_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/kl26/misc_utils.h"

namespace libbase
{
namespace kl26
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

	static uint32_t GetCoreTickPerS(Uint s)
	{
		return s * GetCoreTickPerS();
	}

	static uint32_t GetCoreTickPerMs()
	{
		return GetCoreClockKhz();
	}

	static uint32_t GetCoreTickPerMs(Uint ms)
	{
		return ms * GetCoreTickPerMs();
	}

	static Uint GetCoreTickPerUs()
	{
		return GetCoreClockMhz();
	}

	static Uint GetCoreTickPerUs(Uint us)
	{
		return us * GetCoreTickPerUs();
	}

	static Uint GetCoreTickPerNs(Uint ns)
	{
		return ns * GetCoreTickPerUs() / 1000;
	}


	static uint32_t GetBusClock();
	static uint32_t GetBusClockKhz();
	static Uint GetBusClockMhz();

	static uint32_t GetBusTickPerS()
	{
		return GetBusClock();
	}

	static uint32_t GetBusTickPerS(Uint s)
	{
		return s * GetBusTickPerS();
	}

	static uint32_t GetBusTickPerMs()
	{
		return GetBusClockKhz();
	}

	static uint32_t GetBusTickPerMs(Uint ms)
	{
		return ms * GetBusTickPerMs();
	}

	static Uint GetBusTickPerUs()
	{
		return GetBusClockMhz();
	}

	static Uint GetBusTickPerUs(Uint us)
	{
		return us * GetBusTickPerUs();
	}

	static Uint GetBusTickPerNs(Uint ns)
	{
		return ns * GetBusTickPerUs() / 1000;
	}
};

}
}
