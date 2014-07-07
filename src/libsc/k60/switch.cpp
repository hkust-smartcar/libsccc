/*
 * switch.cpp
 * Switch
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cassert>
#include <cstdint>

#include <log.h>

#include "libbase/k60/gpio.h"

#include "libsc/com/config.h"
#include "libsc/k60/switch.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_SWITCH

namespace
{

#if LIBSC_USE_SWITCH == 1
#define GetPin(x) LIBSC_BUTTON0

#else
inline PinConfig::Name GetPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);

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

Gpi::Config GetGpiConfig(const uint8_t id)
{
	Gpi::Config config;
	config.pin = GetPin(id);
	config.config.set(PinConfig::ConfigBit::PASSIVE_FILTER);
	return config;
}

}

Switch::Switch(const uint8_t id)
		: m_pin(GetGpiConfig(id))
{}

bool Switch::IsOn() const
{
	return m_pin.Get();
}

#else
Switch::Switch(const uint8_t)
		: m_pin(nullptr)
{
	LOG_D("Configured not to use Switch");
}
bool Switch::IsOn() const { return false; }

#endif /* LIBSC_USE_SWITCH */

}
}
