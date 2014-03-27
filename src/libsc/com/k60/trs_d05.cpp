/*
 * trs_d05.cpp
 * TRS-D05 servo abstraction
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cstdint>

#include "libsc/com/trs_d05.h"

// Found by trial and error!!!
#define PWM_MIN 300
#define PWM_MAX 1200

namespace libsc
{

TrsD05::TrsD05(const uint8_t id)
		: Servo(id, PWM_MIN, PWM_MAX)
{}

}
