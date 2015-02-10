/*
 * button.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstdint>

#include <functional>

#include "libbase/log.h"
#include "libbase/k60/gpio.h"

#include "libsc/config.h"
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
inline Pin::Name GetPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_BUTTON0;
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

Gpi::Config GetGpiConfig(const Button::Config &config,
		const Gpi::OnGpiEventListener &listener)
{
	Gpi::Config product;
	product.pin = GetPin(config.id);
	product.config.set(Pin::Config::ConfigBit::kPassiveFilter);
	if (config.is_use_pull_resistor)
	{
		product.config.set(Pin::Config::ConfigBit::kPullEnable);
		product.config.set(Pin::Config::ConfigBit::kPullUp, config.is_active_low);
	}
	if (listener)
	{
		switch (config.listener_trigger)
		{
		default:
		case Button::Config::Trigger::kDown:
			product.interrupt = (config.is_active_low
					? Pin::Config::Interrupt::kFalling
					: Pin::Config::Interrupt::kRising);
			break;

		case Button::Config::Trigger::kUp:
			product.interrupt = (config.is_active_low
					? Pin::Config::Interrupt::kRising
					: Pin::Config::Interrupt::kFalling);
			break;

		case Button::Config::Trigger::kBoth:
			product.interrupt = Pin::Config::Interrupt::kBoth;
			break;
		}
		product.isr = listener;
	}
	return product;
}

}

Button::Button(const Config &config)
		: m_pin(nullptr),
		  m_is_active_low(config.is_active_low)
{
	Gpi::OnGpiEventListener listener;
	if (config.listener)
	{
		const uint8_t id = config.id;
		m_isr = config.listener;
		listener = [this, id](Gpi*)
				{
					m_isr(id);
				};
	}
	m_pin = Gpi(GetGpiConfig(config, listener));
}

bool Button::IsDown() const
{
	return (m_pin.Get() ^ m_is_active_low);
}

#else
Button::Button(const Config&)
		: m_pin(nullptr), m_is_active_low(false)
{
	LOG_DL("Configured not to use Button");
}
bool Button::IsDown() const { return false; }

#endif /* LIBSC_USE_BUTTON */

}
}
