/*
 * switch.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstdint>

#include "libbase/log.h"
#include "libbase/helper.h"
#include LIBBASE_H(gpio)

#include "libsc/config.h"
#include "libsc/switch.h"

using namespace LIBBASE_NS;

namespace libsc
{

#ifdef LIBSC_USE_SWITCH

namespace
{

#if LIBSC_USE_SWITCH == 1
inline Pin::Name GetPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_SWITCH0;
}

#else
inline Pin::Name GetPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break

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
	config.config.set(Pin::Config::ConfigBit::kPassiveFilter);
	return config;
}

}

Switch::Switch(const Config &config)
		: m_pin(GetGpiConfig(config.id)),
		  m_is_active_low(config.is_active_low)
{}

bool Switch::IsOn() const
{
	return (m_pin.Get() ^ m_is_active_low);
}

#else
Switch::Switch(const Config&)
		: m_pin(nullptr), m_is_active_low(false)
{
	LOG_DL("Configured not to use Switch");
}
bool Switch::IsOn() const { return false; }

#endif /* LIBSC_USE_SWITCH */

}
