/*
 * mcg.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
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

	static void Init();

	static uint32_t GetCoreClock()
	{
		return m_core_clock;
	}

private:
	__attribute__((__weak__))
	static Config GetMcgConfig();

	static void InitFbe(const Config &config);
	static void InitPbe(const Config &config, const uint8_t vdiv);
	static void InitPee(const Config &config);
	static void InitClocks(const Config &config, const uint32_t core_clock);

	static uint32_t m_core_clock;
};

}
}
