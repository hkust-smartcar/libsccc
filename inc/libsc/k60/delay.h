/*
 * delay.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <cstdint>

#include "libbase/k60/misc_utils.h"

namespace libsc
{
namespace k60
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
}
