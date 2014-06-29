/*
 * led.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cassert>
#include <cstdint>

#include <log.h>

#include "libbase/k60/gpio.h"

#include "libsc/com/config.h"
#include "libsc/k60/led.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_LED

namespace
{

#if LIBSC_USE_LED == 1
#define GetPin(x) LIBSC_LED0

#else
inline PinConfig::Name GetPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(0);

	case 0:
		return LIBSC_LED0;

#if LIBSC_USE_LED > 1
	case 1:
		return LIBSC_LED1;
#endif

#if LIBSC_USE_LED > 2
	case 2:
		return LIBSC_LED2;
#endif

#if LIBSC_USE_LED > 3
	case 3:
		return LIBSC_LED3;
#endif
	}
}

#endif

GpoConfig GetGpoConfig(const uint8_t id)
{
	GpoConfig config;
	config.pin = GetPin(id);
	config.is_high = true;
	return config;
}

}

Led::Led(const uint8_t id)
		: m_pin(GetGpoConfig(id))
{}

void Led::SetEnable(const bool flag)
{
	m_pin.Set(!flag);
}

void Led::Switch()
{
	m_pin.Turn();
}

#else
Led::Led(const uint8_t id)
		: m_pin(nullptr)
{
	LOG_D("Configured not to use Led");
}
void Led::SetEnable(const bool flag) {}
void Led::Switch() {}

#endif

}
}
