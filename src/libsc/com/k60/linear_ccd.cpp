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

#include <MK60_gpio.h>

#include "libsc/com/config.h"
#include "libsc/com/linear_ccd.h"

namespace libsc
{

#ifdef LIBSC_USE_LINEAR_CCD

LinearCcd::LinearCcd()
		: m_clk_state(true)
{
	// PTB10, AO(D1)
	gpio_init(LIBSC_LINEAR_CCD0_AO, GPI, 1);
	// PTB9 , Clock / CLK
	gpio_init(LIBSC_LINEAR_CCD0_CLK, GPO, 1);
	// PTB8 , SI
	gpio_init(LIBSC_LINEAR_CCD0_SI, GPO, 1);
}

const bool* LinearCcd::SampleData()
{
	gpio_set(LIBSC_LINEAR_CCD0_SI, 1);
	bool is_si_triggered = true;

	for (int i = 0; i < SENSOR_W; ++i)
	{
		// gpio_turn is not working!
		//gpio_turn(LIBSC_LINEAR_CCD0_CLK);
		gpio_set(LIBSC_LINEAR_CCD0_CLK, m_clk_state);
		m_clk_state ^= true;

		// Black == true
		m_buffer[i] = (gpio_get(LIBSC_LINEAR_CCD0_AO) == 1);

		if (is_si_triggered)
		{
			gpio_set(LIBSC_LINEAR_CCD0_SI, 0);
			is_si_triggered = false;
		}
	}
	return m_buffer;
}

#else
LinearCcd::LinearCcd() : m_clk_state(false) {}
const bool* LinearCcd::SampleData() { return nullptr; }

#endif

}
