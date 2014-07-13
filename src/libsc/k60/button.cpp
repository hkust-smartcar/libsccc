/*
 * button.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cassert>
#include <cstdint>

#include <log.h>

#include "libbase/k60/gpio.h"

#include "libsc/com/config.h"
#include "libsc/k60/button.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_BUTTON

namespace
{

#if LIBSC_USE_BUTTON == 1
#define GetPin(x) LIBSC_BUTTON0

#else
inline PinConfig::Name GetPin(const uint8_t id)
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

Gpi::Config GetGpiConfig(const uint8_t id)
{
	Gpi::Config config;
	config.pin = GetPin(id);
	config.config.set(PinConfig::ConfigBit::kPassiveFilter);
	return config;
}

}

Button::Button(const uint8_t id)
		: m_pin(GetGpiConfig(id))
{}

bool Button::IsDown() const
{
	return !m_pin.Get();
}

#else
Button::Button(const uint8_t)
		: m_pin(nullptr)
{
	LOG_D("Configured not to use Button");
}
bool Button::IsDown() const { return false; }

#endif /* LIBSC_USE_BUTTON */

}
}
