/*
 * tpm.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libbase/kl26/pinout_macros.h"

namespace libbase
{
namespace kl26
{

class Tpm
{
public:
	enum struct Name
	{
		kTpm0Ch0 = 0,
		kTpm0Ch1,
		kTpm0Ch2,
		kTpm0Ch3,
		kTpm0Ch4,
		kTpm0Ch5,
		kTpm0Ch6,
		kTpm0Ch7,

		kTpm1Ch0 = 8,
		kTpm1Ch1,
		kTpm1Ch2,
		kTpm1Ch3,
		kTpm1Ch4,
		kTpm1Ch5,
		kTpm1Ch6,
		kTpm1Ch7,

		kTpm2Ch0 = 16,
		kTpm2Ch1,
		kTpm2Ch2,
		kTpm2Ch3,
		kTpm2Ch4,
		kTpm2Ch5,
		kTpm2Ch6,
		kTpm2Ch7,

		kDisable = 24
	};

	enum struct ClkinName
	{
		kTpmClkin0 = 0,
		kTpmClkin1,
		kDisable
	};

	static Tpm& Get()
	{
		static Tpm tpm;
		return tpm;
	}

	static constexpr int GetMaxTpmChannelCount()
	{
		return static_cast<int>(Name::kTpm1Ch0);
	}

	bool RegTpm(const Tpm::Name tpm, const bool is_exclusive_module);
	/**
	 * Unregister a TPM channel and return whether the module is free or not
	 *
	 * @param tpm
	 * @return
	 */
	bool UnregTpm(const Tpm::Name tpm);

private:
	enum struct ModuleLockMode
	{
		kFree,
		kExclusive,
		kShared,
	};

	Tpm();

	ModuleLockMode m_module_lock[PINOUT_TPM_COUNT];
	bool m_channel_lock[PINOUT_TPM_COUNT][PINOUT_TPM_CHANNEL_COUNT];
};

}
}
