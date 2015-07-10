/*
 * tpm.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/kl26/hardware.h"

#include <cassert>

#include <algorithm>

#include "libbase/kl26/tpm_utils.h"
#include "libbase/kl26/pinout.h"
#include "libbase/misc_types.h"

namespace libbase
{
namespace kl26
{

Tpm::Tpm()
{
	for (Uint i = 0; i < PINOUT_TPM_COUNT; ++i)
	{
		m_module_lock[i] = ModuleLockMode::kFree;
		for (Uint j = 0; j < PINOUT_TPM_CHANNEL_COUNT; ++j)
		{
			m_channel_lock[i][j] = false;
		}
	}
}

bool Tpm::RegTpm(const Tpm::Name tpm, const bool is_exclusive_module)
{
	const Uint module = TpmUtils::GetTpmModule(tpm);
	const Uint channel = TpmUtils::GetTpmChannel(tpm);
	assert(module < PINOUT_TPM_COUNT);
	assert(channel < PINOUT_TPM_CHANNEL_COUNT);
	if (m_module_lock[module] == ModuleLockMode::kExclusive)
	{
		return false;
	}
	else if (m_module_lock[module] == ModuleLockMode::kShared
			&& is_exclusive_module)
	{
		return false;
	}
	else if (m_channel_lock[module][channel])
	{
		return false;
	}

	m_module_lock[module] = is_exclusive_module ? ModuleLockMode::kExclusive
			: ModuleLockMode::kShared;
	m_channel_lock[module][channel] = true;
	return true;
}

bool Tpm::UnregTpm(const Tpm::Name tpm)
{
	const Uint module = TpmUtils::GetTpmModule(tpm);
	const Uint channel = TpmUtils::GetTpmChannel(tpm);
	assert(module < PINOUT_TPM_COUNT);
	assert(channel < PINOUT_TPM_CHANNEL_COUNT);
	m_channel_lock[module][channel] = false;

	for (bool l : m_channel_lock[PINOUT_TPM_COUNT])
	{
		if (l)
		{
			return false;
		}
	}
	m_module_lock[module] = ModuleLockMode::kFree;
	return true;
}

}
}
