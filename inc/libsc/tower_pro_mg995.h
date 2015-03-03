/*
 * tower_pro_mg995.h
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
 * TowerPro MG995 RC servo
 */
class TowerProMg995 : public Servo
{
public:
	struct Config
	{
		uint8_t id;
	};

	explicit TowerProMg995(const Config &config);
};

}
