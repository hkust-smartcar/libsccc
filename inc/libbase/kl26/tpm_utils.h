/*
 * tpm_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libbase/kl26/tpm.h"
#include "libbase/misc_types.h"

namespace libbase
{
namespace kl26
{

class TpmUtils
{
public:
	static Uint GetTpmModule(const Tpm::Name pin)
	{
		return static_cast<Uint>(pin) / Tpm::GetMaxTpmChannelCount();
	}

	static Uint GetTpmChannel(const Tpm::Name pin)
	{
		return static_cast<Uint>(pin) % Tpm::GetMaxTpmChannelCount();
	}
};

}
}
