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

#if LIBSC_USE_BUTTON > 1
	case 1:
		return LIBSC_BUTTON1;
#endif

#if LIBSC_USE_BUTTON > 2
	case 2:
		return LIBSC_BUTTON2;
#endif

#if LIBSC_USE_BUTTON > 3
	case 3:
		return LIBSC_BUTTON3;
#endif

#if LIBSC_USE_BUTTON > 4
	case 4:
		return LIBSC_BUTTON4;
#endif

#if LIBSC_USE_BUTTON > 5
	case 5:
		return LIBSC_BUTTON5;
#endif
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
Button::Button(const uint8_t) : m_id(0) {}
bool Button::IsDown() const { return false; }

#endif /* LIBSC_USE_BUTTON */

}
