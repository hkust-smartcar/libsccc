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
	static Uint GetTpmModule(const Tpm::Name tpm)
	{
		return static_cast<Uint>(tpm) / Tpm::GetMaxTpmChannelCount();
	}

	static Uint GetTpmChannel(const Tpm::Name tpm)
	{
		return static_cast<Uint>(tpm) % Tpm::GetMaxTpmChannelCount();
	}

	static Tpm::Name GetTpm(const Uint module, const Uint channel)
	{
		return static_cast<Tpm::Name>(module * Tpm::GetMaxTpmChannelCount()
				+ channel);
	}
};

}
}
