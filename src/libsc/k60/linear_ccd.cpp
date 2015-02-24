/*
 * linear_ccd.h
 * Linear CCD (for TSL1401CL)
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstdint>
#include <bitset>

#include "libbase/log.h"
#include "libbase/k60/adc.h"
#include "libbase/k60/gpio.h"
#include "libbase/k60/pin.h"

#include "libsc/config.h"
#include "libsc/k60/linear_ccd.h"
#include "libsc/k60/system.h"
#include "libutil/misc.h"

using namespace libbase::k60;

// The output is nominally 0V for no light input
#ifndef LIBSC_NEGATE_LINEAR_CCD
#define CCD_DARK false
#else
#define CCD_DARK true
#endif

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_LINEAR_CCD

namespace
{

#if LIBSC_USE_LINEAR_CCD == 1
inline Pin::Name GetAdPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_LINEAR_CCD0_AD;
}

inline Pin::Name GetClkPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_LINEAR_CCD0_CLK;
}

inline Pin::Name GetSiPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_LINEAR_CCD0_SI;
}

#else
inline Pin::Name GetAdPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break

	case 0:
		return LIBSC_LINEAR_CCD0_AD;

	case 1:
		return LIBSC_LINEAR_CCD1_AD;
	}
}

inline Pin::Name GetClkPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break

	case 0:
		return LIBSC_LINEAR_CCD0_CLK;

	case 1:
		return LIBSC_LINEAR_CCD1_CLK;
	}
}

inline Pin::Name GetSiPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break

	case 0:
		return LIBSC_LINEAR_CCD0_SI;

	case 1:
		return LIBSC_LINEAR_CCD1_SI;
	}
}

#endif

Adc::Config GetAdConfig(const uint8_t id)
{
	Adc::Config config;
	config.resolution = Adc::Config::Resolution::k16Bit;
	config.pin = GetAdPin(id);
	return config;
}

Gpo::Config GetClkGpoConfig(const uint8_t id)
{
	Gpo::Config config;
	config.pin = GetClkPin(id);
	return config;
}

Gpo::Config GetSiGpoConfig(const uint8_t id)
{
	Gpo::Config config;
	config.pin = GetSiPin(id);
	return config;
}

}

LinearCcd::LinearCcd(const uint8_t id)
		: m_ad_pin(GetAdConfig(id)),
		  m_clk_pin(GetClkGpoConfig(id)),
		  m_si_pin(GetSiGpoConfig(id)),
		  m_index(0)
{}

void LinearCcd::Delay()
{
	// 50ns under 180MHz
	for (int i = 0; i < 9; ++i)
	{
		asm("nop");
	}
}

void LinearCcd::StartSample()
{
	m_index = 0;
}

bool LinearCcd::SampleProcess()
{
	if (IsImageReady())
	{
		return true;
	}

	if (m_index == 0)
	{
		m_si_pin.Set(true);
	}

	m_clk_pin.Set(true);
	Delay();
	if (m_index == 0)
	{
		m_si_pin.Set(false);
	}
	m_clk_pin.Set(false);
	Delay();

	m_back_buffer[m_index] = m_ad_pin.GetResult();

	if (++m_index >= kSensorW)
	{
		m_front_buffer = m_back_buffer;

		m_clk_pin.Set(true);
		Delay();
		m_clk_pin.Set(false);
		Delay();
		System::DelayUs(20);
		return true;
	}
	else
	{
		return false;
	}
}

#else
LinearCcd::LinearCcd(const uint8_t)
		: m_ad_pin(nullptr), m_clk_pin(nullptr), m_si_pin(nullptr), m_index(0)
{
	LOG_DL("Configured not to use LinearCcd");
}
void LinearCcd::StartSample() {}
bool LinearCcd::SampleProcess() { return false; }

#endif

}
}
