/*
 * delay.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/misc_types.h"

namespace libsc
{

class Delay
{
public:
	virtual ~Delay()
	{}

	virtual void DelayUs(const uint16_t us) = 0;

	virtual void DelayMs(const uint16_t ms)
	{
		for (Uint i = 0; i < ms; ++i)
		{
			DelayUs(1000);
		}
	}

	virtual void DelayS(const uint16_t s)
	{
		for (Uint i = 0; i < s; ++i)
		{
			DelayMs(1000);
		}
	}
};

}
