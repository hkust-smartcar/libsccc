/*
 * dac_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/k60/dac.h"
#include "libbase/k60/misc_utils.h"

namespace libbase
{
namespace k60
{

class DacUtils
{
public:
	static Uint GetModule(const Dac::Name dac)
	{
		return static_cast<Uint>(dac);
	}

	static Dac::Name GetPin(const Uint module)
	{
		return static_cast<Dac::Name>(module);
	}
};

}
}
