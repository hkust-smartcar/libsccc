/*
 * tower_pro_mg995.cpp
 * TowerPro MG995 RC servo
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstdint>

#include "libsc/k60/tower_pro_mg995.h"

#define PERIOD 20000
#define POS_WIDTH_MIN 1000
#define POS_WIDTH_MAX 2000

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

TowerProMg995::TowerProMg995(const Config &config)
		: Servo(GetServoConfig(config.id))
{}

}
}
