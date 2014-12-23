/*
 * joystick.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
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
inline Pin::Name GetUpPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_JOYSTICK0_UP;
}

inline Pin::Name GetDownPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_JOYSTICK0_DOWN;
}

inline Pin::Name GetLeftPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_JOYSTICK0_LEFT;
}

inline Pin::Name GetRightPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_JOYSTICK0_RIGHT;
}

inline Pin::Name GetSelectPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_JOYSTICK0_SELECT;
}

#endif

Gpi::Config GetUpGpiConfig(const uint8_t id)
{
	Gpi::Config config;
	config.pin = GetUpPin(id);
	config.config.set(Pin::Config::ConfigBit::kPassiveFilter);
	return config;
}

Gpi::Config GetDownGpiConfig(const uint8_t id)
{
	Gpi::Config config;
	config.pin = GetDownPin(id);
	config.config.set(Pin::Config::ConfigBit::kPassiveFilter);
	return config;
}

Gpi::Config GetLeftGpiConfig(const uint8_t id)
{
	Gpi::Config config;
	config.pin = GetLeftPin(id);
	config.config.set(Pin::Config::ConfigBit::kPassiveFilter);
	return config;
}

Gpi::Config GetRightGpiConfig(const uint8_t id)
{
	Gpi::Config config;
	config.pin = GetRightPin(id);
	config.config.set(Pin::Config::ConfigBit::kPassiveFilter);
	return config;
}

Gpi::Config GetSelectGpiConfig(const uint8_t id)
{
	Gpi::Config config;
	config.pin = GetSelectPin(id);
	config.config.set(Pin::Config::ConfigBit::kPassiveFilter);
	return config;
}

}

Joystick::Joystick(const Config &config)
		: m_pins{Gpi(GetUpGpiConfig(config.id)),
				Gpi(GetDownGpiConfig(config.id)),
				Gpi(GetLeftGpiConfig(config.id)),
				Gpi(GetRightGpiConfig(config.id)),
				Gpi(GetSelectGpiConfig(config.id))},
		  m_is_active_low(config.is_active_low)
{}

Joystick::State Joystick::GetState() const
{
	if (m_pins[0].Get() ^ m_is_active_low)
	{
		return State::UP;
	}
	else if (m_pins[1].Get() ^ m_is_active_low)
	{
		return State::DOWN;
	}
	else if (m_pins[2].Get() ^ m_is_active_low)
	{
		return State::LEFT;
	}
	else if (m_pins[3].Get() ^ m_is_active_low)
	{
		return State::RIGHT;
	}
	else if (m_pins[4].Get() ^ m_is_active_low)
	{
		return State::SELECT;
	}
	else
	{
		return State::IDLE;
	}
}

#else
Joystick::Joystick(const Config&)
		: m_pins{Gpi(nullptr), Gpi(nullptr), Gpi(nullptr), Gpi(nullptr),
				  Gpi(nullptr)}
{
	LOG_DL("Configured not to use Joystick");
}
Joystick::State Joystick::GetState() const { return State::IDLE; }

#endif /* LIBSC_USE_BUTTON */

}
}
