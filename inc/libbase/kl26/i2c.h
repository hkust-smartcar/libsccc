/*
 * i2c.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libbase/kl26/pinout_macros.h"

namespace libbase
{
namespace kl26
{

class I2c
{
public:
	enum struct Name
	{
		kI2c0Scl = 0,
		kI2c0Sda,
		kI2c1Scl,
		kI2c1Sda,

		kDisable
	};
};

}
}
