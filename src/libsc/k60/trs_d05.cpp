/*
 * trs_d05.cpp
 * TRS-D05 RC servo
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cstdint>

#include "libsc/k60/trs_d05.h"

// 300 Hz
#define PERIOD 3333
#define POS_WIDTH_MIN 1500 - 900
#define POS_WIDTH_MAX 1500 + 900

namespace libsc
{
namespace k60
{

namespace
{

Servo::Config GetServoConfig(const uint8_t id)
{
	Servo::Config config;
	config.id = id;
	config.period = PERIOD;
	config.min_pos_width = POS_WIDTH_MIN;
	config.max_pos_width = POS_WIDTH_MAX;
	return config;
}

}

TrsD05::TrsD05(const Config &config)
		: Servo(GetServoConfig(config.id))
{}

}
}
