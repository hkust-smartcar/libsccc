/*
 * dwt.h
 * Data Watchpoint and Trace, currently only supports utilizing CYCCNT to delay
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
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
