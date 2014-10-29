/*
 * tower_pro_mg995.cpp
 * TowerPro MG995 RC servo
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
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

TowerProMg995::TowerProMg995(const uint8_t id)
		: Servo(id, PERIOD, POS_WIDTH_MIN, POS_WIDTH_MAX)
{}

}
}
