/*
 * switch.cpp
 * Switch
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>
#include <hw_common.h>
#include <cstdint>

#include <MK60_gpio.h>

#include "libsc/com/config.h"
#include "libsc/com/switch.h"

namespace libsc
{

#ifdef LIBSC_USE_SWITCH

namespace
{

#if LIBSC_USE_SWITCH == 1
#define GetGpio(x) LIBSC_BUTTON0

#else
inline PTXn_e GetGpio(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(0);

	case 0:
		return LIBSC_SWITCH0;

	case 1:
		return LIBSC_SWITCH1;

#if LIBSC_USE_SWITCH > 2
	case 2:
		return LIBSC_SWITCH2;
#endif

#if LIBSC_USE_SWITCH > 3
	case 3:
		return LIBSC_SWITCH3;
#endif

#if LIBSC_USE_SWITCH > 4
	case 4:
		return LIBSC_SWITCH4;
#endif

#if LIBSC_USE_SWITCH > 5
	case 5:
		return LIBSC_SWITCH5;
#endif
	}
}

#endif

}

Switch::Switch(const uint8_t id)
		: m_id(id)
{
	gpio_init(GetGpio(m_id), GPI, 0);
}

bool Switch::IsOn() const
{
	return gpio_get(GetGpio(m_id));
}

#else
Switch::Switch(const uint8_t) : m_id(0) {}
bool Switch::IsOn() const { return false; }

#endif /* LIBSC_USE_SWITCH */

}
