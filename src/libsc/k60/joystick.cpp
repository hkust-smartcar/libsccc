/*
 * joystick.cpp
 * Joystick
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cassert>
#include <cstdint>

#include "libbase/log.h"
#include "libbase/k60/gpio.h"

#include "libsc/config.h"
#include "libsc/k60/joystick.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_JOYSTICK

namespace
{

#if LIBSC_USE_JOYSTICK == 1
#define GetUpPin(x) LIBSC_JOYSTICK0_UP
#define GetDownPin(x) LIBSC_JOYSTICK0_DOWN
#define GetLeftPin(x) LIBSC_JOYSTICK0_LEFT
#define GetRightPin(x) LIBSC_JOYSTICK0_RIGHT
#define GetSelectPin(x) LIBSC_JOYSTICK0_SELECT

#endif

#if LIBSC_USE_JOYSTICK == 1 && defined(__GNUC__)
#define UNUSED __attribute__((__unused__))
#else
#define UNUSED
#endif

Gpi::Config GetUpGpiConfig(const uint8_t id UNUSED)
{
	Gpi::Config config;
	config.pin = GetUpPin(id);
	config.config.set(Pin::Config::ConfigBit::kPassiveFilter);
	return config;
}

Gpi::Config GetDownGpiConfig(const uint8_t id UNUSED)
{
	Gpi::Config config;
	config.pin = GetDownPin(id);
	config.config.set(Pin::Config::ConfigBit::kPassiveFilter);
	return config;
}

Gpi::Config GetLeftGpiConfig(const uint8_t id UNUSED)
{
	Gpi::Config config;
	config.pin = GetLeftPin(id);
	config.config.set(Pin::Config::ConfigBit::kPassiveFilter);
	return config;
}

Gpi::Config GetRightGpiConfig(const uint8_t id UNUSED)
{
	Gpi::Config config;
	config.pin = GetRightPin(id);
	config.config.set(Pin::Config::ConfigBit::kPassiveFilter);
	return config;
}

Gpi::Config GetSelectGpiConfig(const uint8_t id UNUSED)
{
	Gpi::Config config;
	config.pin = GetSelectPin(id);
	config.config.set(Pin::Config::ConfigBit::kPassiveFilter);
	return config;
}

#if LIBSC_USE_JOYSTICK == 1
#pragma GCC diagnostic pop
#endif

}

Joystick::Joystick(const uint8_t id)
		: m_pins{Gpi(GetUpGpiConfig(id)), Gpi(GetDownGpiConfig(id)),
				  Gpi(GetLeftGpiConfig(id)), Gpi(GetRightGpiConfig(id)),
				  Gpi(GetSelectGpiConfig(id))}
{}

Joystick::State Joystick::GetState() const
{
	if (!m_pins[0].Get())
	{
		return State::UP;
	}
	else if (!m_pins[1].Get())
	{
		return State::DOWN;
	}
	else if (!m_pins[2].Get())
	{
		return State::LEFT;
	}
	else if (!m_pins[3].Get())
	{
		return State::RIGHT;
	}
	else if (!m_pins[4].Get())
	{
		return State::SELECT;
	}
	else
	{
		return State::IDLE;
	}
}

#else
Joystick::Joystick(const uint8_t)
		: m_pins{Gpi(nullptr), Gpi(nullptr), Gpi(nullptr), Gpi(nullptr),
				  Gpi(nullptr)}
{
	LOG_DL("Configured not to use Joystick");
}
Joystick::State Joystick::GetState() const { return State::IDLE; }

#endif /* LIBSC_USE_BUTTON */

}
}
