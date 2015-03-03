/*
 * trs_d05.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libsc/servo.h"

namespace libsc
{

/**
 * TRS-D05 RC servo
 */
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
