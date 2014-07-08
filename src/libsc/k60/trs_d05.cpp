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

TrsD05::TrsD05(const uint8_t id)
		: Servo(id, PERIOD, POS_WIDTH_MIN, POS_WIDTH_MAX)
{}

}
}
