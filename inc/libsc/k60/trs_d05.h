/*
 * trs_d05.h
 * TRS-D05 RC servo
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libsc/k60/servo.h"

namespace libsc
{
namespace k60
{

class TrsD05 : public Servo
{
public:
	struct Config
	{
		uint8_t id;
	};

	explicit TrsD05(const Config &config);
};

}
}
