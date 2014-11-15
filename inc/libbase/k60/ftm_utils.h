/*
 * ftm_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include "libbase/k60/ftm.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/pinout.h"

namespace libbase
{
namespace k60
{

class FtmUtils
{
public:
	static Uint GetFtmModule(const Ftm::Name pin)
	{
		return static_cast<Uint>(pin) / PINOUT::GetFtmChannelCount();
	}

	static Uint GetFtmChannel(const Ftm::Name pin)
	{
		return static_cast<Uint>(pin) % PINOUT::GetFtmChannelCount();
	}
};

}
}
