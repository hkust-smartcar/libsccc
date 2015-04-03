/*
 * joystick.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstddef>
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

Pin::Name GetStatePin(const Joystick::State state, const uint8_t id)
{
	switch (state)
	{
	default:
		assert(false);
		// no break

	case Joystick::State::kUp:
		return GetUpPin(id);

	case Joystick::State::kDown:
		return GetDownPin(id);

	case Joystick::State::kLeft:
		return GetLeftPin(id);

	case Joystick::State::kRight:
		return GetRightPin(id);

	case Joystick::State::kSelect:
		return GetSelectPin(id);
	}
}

Gpi::Config GetPinGpiConfig(const Joystick::State state,
		const Joystick::Config &config, const Gpi::OnGpiEventListener &listener)
{
	Gpi::Config product;
	product.pin = GetStatePin(state, config.id);
	product.config.set(Pin::Config::ConfigBit::kPassiveFilter);
	if (listener)
	{
		switch (config.listener_triggers[static_cast<int>(state)])
		{
		default:
		case Joystick::Config::Trigger::kDown:
			product.interrupt = (config.is_active_low
					? Pin::Config::Interrupt::kFalling
					: Pin::Config::Interrupt::kRising);
			break;

		case Joystick::Config::Trigger::kUp:
			product.interrupt = (config.is_active_low
					? Pin::Config::Interrupt::kRising
					: Pin::Config::Interrupt::kFalling);
			break;

		case Joystick::Config::Trigger::kBoth:
			product.interrupt = Pin::Config::Interrupt::kBoth;
			break;
		}
		product.isr = listener;
	}
	return product;
}

}

Joystick::Joystick(const Config &config)
		: m_is_active_low(config.is_active_low)
{
	for (int i = 0; i < 5; ++i)
	{
		Gpi::OnGpiEventListener listener;
		if (config.listeners[i])
		{
			const uint8_t id = config.id;
			Joystick::Listener js_listener = config.listeners[i];
			listener = [js_listener, id, i](Gpi*)
					{
						js_listener(id);
					};
		}
		m_pins[i] = Gpi(GetPinGpiConfig(static_cast<Joystick::State>(i), config,
				listener));
	}
}

Joystick::Joystick(nullptr_t)
		: m_is_active_low(false)
{}

Joystick::State Joystick::GetState() const
{
	for (int i = 0; i < 5; ++i)
	{
		if (m_pins[i].Get() ^ m_is_active_low)
		{
			return static_cast<State>(i);
		}
	}
	return State::kIdle;
}

#else
Joystick::Joystick(const Config&)
		: Joystick(nullptr)
{}
Joystick::Joystick(nullptr_t)
		: m_is_active_low(false)
{
	LOG_DL("Configured not to use Joystick");
}
Joystick::State Joystick::GetState() const { return State::kIdle; }

#endif /* LIBSC_USE_BUTTON */

}
}
