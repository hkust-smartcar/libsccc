/*
 * trs_d05.cpp
 * TRS-D05 servo abstraction
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cstdint>

#include "libsc/com/trs_d05.h"

#define FREQ 300
#define PWM_MIN 4400 - 2700
#define PWM_MAX 4400 + 2700
// Found by trial and error!!!
// Freq = 50
//#define PWM_MIN 300
//#define PWM_MAX 1200
// Freq = 100
//#define PWM_MIN 1550 - 900
//#define PWM_MAX 1550 + 900

namespace libsc
{

TrsD05::TrsD05(const uint8_t id)
		: Servo(id, FREQ, PWM_MIN, PWM_MAX)
{}

}
