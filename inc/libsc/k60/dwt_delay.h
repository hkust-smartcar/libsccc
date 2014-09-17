/*
 * dwt_delay.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <cstdint>

#include "libbase/k60/dwt.h"

#include "libsc/k60/delay.h"

namespace libsc
{
namespace k60
{

class DwtDelay : public Delay
{
public:
	void DelayUs(const uint16_t us) override
	{
		libbase::k60::Dwt::DelayUs(us);
	}
};

}
}
