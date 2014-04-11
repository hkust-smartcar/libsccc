/*
 * button.cpp
 * Button
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>
#include <hw_common.h>
#include <cstdint>

#include <MK60_gpio.h>

#include "libsc/com/config.h"
#include "libsc/com/button.h"

namespace libsc
{

#ifdef LIBSC_USE_BUTTON

namespace
{

#if LIBSC_USE_BUTTON == 1
#define GetGpio(x) LIBSC_BUTTON0

#else
inline PTXn_e GetGpio(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(0);

	case 0:
		return LIBSC_BUTTON0;

	case 1:
		return LIBSC_BUTTON1;

	case 2:
		return LIBSC_BUTTON2;

	case 3:
		return LIBSC_BUTTON3;
	}
}

#endif

}

Button::Button(const uint8_t id)
		: m_id(id)
{
	gpio_init(GetGpio(m_id), GPI, 0);
}

bool Button::IsDown() const
{
	return !gpio_get(GetGpio(m_id));
}

#else
LightSensor::LightSensor(const uint8_t) : m_id(0) {}
bool LightSensor::IsDown() { return false; }

#endif /* LIBSC_USE_BUTTON */

}
