/*
 * adc_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/k60/adc.h"
#include "libbase/k60/misc_utils.h"

namespace libbase
{
namespace k60
{

class AdcUtils
{
public:
	static Uint GetModule(const Adc::Name pin)
	{
		return static_cast<Uint>(pin) / Adc::kModuleChannelCount;
	}

	static Uint GetChannel(const Adc::Name pin)
	{
		return static_cast<Uint>(pin) % Adc::kModuleChannelCount;
	}

	static Uint GetChannelNumber(const Adc::Name pin)
	{
		const Uint channel = GetChannel(pin);
		if (channel >= (Uint)Adc::Name::kAdc0Ad4B)
		{
			return channel - 4;
		}
		else
		{
			return channel;
		}
	}

	static Adc::Name GetPin(const Uint module, const Uint channel)
	{
		return static_cast<Adc::Name>(module * Adc::kModuleChannelCount
				+ channel);
	}
};

}
}
