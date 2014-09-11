/*
 * spi_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"

namespace libbase
{
namespace k60
{

class SpiUtils
{
public:
	static Uint GetCsNumber(const Pin::Name pin);
};

}
}
