/*
 * linear_ccd.h
 * Linear CCD (for TSL1401CL)
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>
#include <hw_common.h>
#include <cstdint>
#include <bitset>

#include <MK60_gpio.h>

#include "libsc/com/config.h"
#include "libsc/com/linear_ccd.h"
#include "libsc/com/k60/macro.h"

namespace libsc
{

#ifdef LIBSC_USE_LINEAR_CCD

namespace
{

#if LIBSC_USE_LINEAR_CCD == 1
#define GetAoPin(x) LIBSC_LINEAR_CCD0_AO
#define GetClkPin(x) LIBSC_LINEAR_CCD0_CLK
#define GetSiPin(x) LIBSC_LINEAR_CCD0_SI

#else
inline PTXn_e GetAoPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(0);

	case 0:
		return LIBSC_LINEAR_CCD0_AO;

	case 1:
		return LIBSC_LINEAR_CCD1_AO;
	}
}

inline PTXn_e GetClkPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(0);

	case 0:
		return LIBSC_LINEAR_CCD0_CLK;

	case 1:
		return LIBSC_LINEAR_CCD1_CLK;
	}
}

inline PTXn_e GetSiPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(0);

	case 0:
		return LIBSC_LINEAR_CCD0_SI;

	case 1:
		return LIBSC_LINEAR_CCD1_SI;
	}
}

#endif

}

LinearCcd::LinearCcd(const uint8_t id)
		: m_id(id), m_index(0)
{
	// PTB10, AO(D1)
	gpio_init(GetAoPin(id), GPI, 1);
	// PTB9 , Clock / CLK
	gpio_init(GetClkPin(id), GPO, 0);
	// PTB8 , SI
	gpio_init(GetSiPin(id), GPO, 1);
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
		gpio_set(GetSiPin(m_id), 1);
	}

	gpio_set(GetClkPin(m_id), 1);
	DELAY_US(50);
	gpio_set(GetClkPin(m_id), 0);
	DELAY_US(50);

	// Black == false
	m_back_buffer[m_index] = (gpio_get(GetAoPin(m_id)) == 0);

	if (m_index == 0)
	{
		gpio_set(GetSiPin(m_id), 0);
	}

	if (++m_index >= SENSOR_W)
	{
		m_front_buffer = m_back_buffer;
		return true;
	}
	else
	{
		return false;
	}
}

#else
LinearCcd::LinearCcd() : m_index(0) {}
void LinearCcd::StartSample() {}
bool LinearCcd::SampleProcess() { return false; }

#endif

}
