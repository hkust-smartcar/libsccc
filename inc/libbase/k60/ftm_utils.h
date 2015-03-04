/*
 * ftm_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libbase/k60/ftm.h"
#include "libbase/misc_types.h"

namespace libbase
{
namespace k60
{

class FtmUtils
{
public:
	static Uint GetFtmModule(const Ftm::Name pin)
	{
		return static_cast<Uint>(pin) / Ftm::GetMaxFtmChannelCount();
	}

	static Uint GetFtmChannel(const Ftm::Name pin)
	{
		return static_cast<Uint>(pin) % Ftm::GetMaxFtmChannelCount();
	}

	static Ftm::Name GetFtm(const Uint module, const Uint channel)
	{
		return static_cast<Ftm::Name>(module * Ftm::GetMaxFtmChannelCount()
				+ channel);
	}

	static Uint GetFtmModule(const Ftm::QdName pin)
	{
		return static_cast<Uint>(pin) >> 1;
	}

	static Uint GetFtmPhase(const Ftm::QdName pin)
	{
		return static_cast<Uint>(pin) % 2;
	}
};

}
}
