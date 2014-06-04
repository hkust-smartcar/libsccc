/*
 * joystick.cpp
 * Joystick
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
#include "libsc/com/joystick.h"

namespace libsc
{

#ifdef LIBSC_USE_JOYSTICK

namespace
{

#if LIBSC_USE_JOYSTICK == 1
#define GetUpGpio(x) LIBSC_JOYSTICK0_UP
#define GetDownGpio(x) LIBSC_JOYSTICK0_DOWN
#define GetLeftGpio(x) LIBSC_JOYSTICK0_LEFT
#define GetRightGpio(x) LIBSC_JOYSTICK0_RIGHT
#define GetSelectGpio(x) LIBSC_JOYSTICK0_SELECT

#endif

}

Joystick::Joystick(const uint8_t id)
		: m_id(id)
{
	gpio_init(GetUpGpio(m_id), GPI, 0);
	gpio_init(GetDownGpio(m_id), GPI, 0);
	gpio_init(GetLeftGpio(m_id), GPI, 0);
	gpio_init(GetRightGpio(m_id), GPI, 0);
	gpio_init(GetSelectGpio(m_id), GPI, 0);
}

Joystick::State Joystick::GetState() const
{
	if (!gpio_get(GetUpGpio(m_id)))
	{
		return UP;
	}
	else if (!gpio_get(GetDownGpio(m_id)))
	{
		return DOWN;
	}
	else if (!gpio_get(GetLeftGpio(m_id)))
	{
		return LEFT;
	}
	else if (!gpio_get(GetRightGpio(m_id)))
	{
		return RIGHT;
	}
	else if (!gpio_get(GetSelectGpio(m_id)))
	{
		return SELECT;
	}
	else
	{
		return IDLE;
	}
}

#else
Joystick::Joystick(const uint8_t) : m_id(0) {}
State Joystick::GetState() const { return IDLE; }

#endif /* LIBSC_USE_BUTTON */

}
