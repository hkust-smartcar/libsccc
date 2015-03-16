/*
 * i2c_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libbase/k60/i2c.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pinout.h"

namespace libbase
{
namespace k60
{

class I2cUtils
{
public:
	static Uint GetI2cModule(const I2c::Name pin)
	{
		return static_cast<Uint>(pin) / PINOUT::GetI2cCount();
	}
};

}
}
