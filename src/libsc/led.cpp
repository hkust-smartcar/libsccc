/*
 * led.cpp
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
#include "libsc/led.h"

using namespace LIBBASE_NS;

namespace libsc
{

#ifdef LIBSC_USE_LED

namespace
{

#if LIBSC_USE_LED == 1
inline Pin::Name GetPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_LED0;
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

Gpo::Config GetGpoConfig(const Led::Config &config)
{
	Gpo::Config product;
	product.pin = GetPin(config.id);
	// Default off
	product.is_high = config.is_active_low;
	return product;
}

}

Led::Led(const Config &config)
		: m_pin(GetGpoConfig(config)),
		  m_is_active_low(config.is_active_low)
{}

void Led::SetEnable(const bool flag)
{
	m_pin.Set(flag ^ m_is_active_low);
}

void Led::Switch()
{
	m_pin.Turn();
}

#else
Led::Led(const Config&)
		: m_pin(nullptr), m_is_active_low(false)
{
	LOG_DL("Configured not to use Led");
}
void Led::SetEnable(const bool) {}
void Led::Switch() {}

#endif

}
