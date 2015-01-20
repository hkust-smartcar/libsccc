/*
 * mcg.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/k60/mcg_c.h"

namespace libbase
{
namespace k60
{

class Mcg
{
public:
	struct Config
	{
		Config();

		uint32_t external_oscillator_khz;
		uint32_t core_clock_khz;
		uint32_t bus_clock_khz;
		uint32_t flexbus_clock_khz;
		uint32_t flash_clock_khz;
	};

	static Mcg& Get()
	{
		static Mcg inst;
		return inst;
	}

	void Init();

	uint32_t GetCoreClock()
	{
		return m_core_clock;
	}

private:
	Mcg();

	/**
	 * Get the Config object used during initialization, to be implemented by
	 * user
	 *
	 * @return
	 */
	static Config GetMcgConfig();

	void InitFbe(const Config &config);
	void InitPbe(const Config &config, const uint8_t vdiv);
	void InitPee(const Config &config);
	void InitClocks(const Config &config, const uint32_t core_clock);

	uint32_t m_core_clock;
};

}
}
