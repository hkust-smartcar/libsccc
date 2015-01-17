/*
 * light_sensor.cpp
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
#include "libsc/k60/light_sensor.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_LIGHT_SENSOR

namespace
{

#if LIBSC_USE_LIGHT_SENSOR == 1
inline Pin::Name GetPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_LIGHT_SENSOR0;
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
		return LIBSC_LIGHT_SENSOR0;

	case 1:
		return LIBSC_LIGHT_SENSOR1;
	}
}

#endif

Gpi::Config GetGpiConfig(const LightSensor::Config &config,
		const Gpi::OnGpiEventListener &listener)
{
	Gpi::Config product;
	product.pin = GetPin(config.id);
	product.config.set(Pin::Config::ConfigBit::kPassiveFilter);
	if (listener)
	{
		switch (config.listener_trigger)
		{
		default:
		case LightSensor::Config::Trigger::kBright:
			product.interrupt = (config.is_active_low
					? Pin::Config::Interrupt::kFalling
					: Pin::Config::Interrupt::kRising);
			break;

		case LightSensor::Config::Trigger::kDark:
			product.interrupt = (config.is_active_low
					? Pin::Config::Interrupt::kRising
					: Pin::Config::Interrupt::kFalling);
			break;

		case LightSensor::Config::Trigger::kBoth:
			product.interrupt = Pin::Config::Interrupt::kBoth;
			break;
		}
		product.isr = listener;
	}
	return product;
}

}

LightSensor::LightSensor(const Config &config)
		: m_pin(nullptr),
		  m_is_active_low(config.is_active_low)
{
	Gpi::OnGpiEventListener listener;
	if (config.listener)
	{
		const uint8_t id = config.id;
		listener = [this, id](Gpi*)
				{
					m_isr(id);
				};
	}
	m_pin = Gpi(GetGpiConfig(config, listener));
}

bool LightSensor::IsBright() const
{
	return (m_pin.Get() ^ m_is_active_low);
}

#else
LightSensor::LightSensor(const Config&)
		: m_pin(nullptr), m_is_active_low(false)
{
	LOG_DL("Configured not to use LightSensor");
}
bool LightSensor::IsBright() const { return false; }

#endif

}
}
