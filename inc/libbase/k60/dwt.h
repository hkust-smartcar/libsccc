/*
 * dwt.h
 * Data Watchpoint and Trace, currently only supports utilizing CYCCNT to delay
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <cstdint>

namespace libbase
{
namespace k60
{

class Dwt
{
public:
	static void DelayUs(const uint16_t us);
};

}
}
