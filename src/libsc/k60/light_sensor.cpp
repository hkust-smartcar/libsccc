/*
 * light_sensor.cpp
 * Light sensor
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
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

Gpi::Config GetGpiConfig(const uint8_t id,
		const LightSensor::OnDetectListener &listener)
{
	Gpi::Config config;
	config.pin = GetPin(id);
	config.config.set(Pin::Config::ConfigBit::kPassiveFilter);
	if (listener)
	{
		config.interrupt = Pin::Config::Interrupt::kRising;
		config.isr = [&listener, id](Gpi*)
				{
					listener(id);
				};
	}
	return config;
}

}

LightSensor::LightSensor(const uint8_t id, const OnDetectListener &listener)
		: m_pin(nullptr), m_isr(listener)
{
	m_pin = Gpi(GetGpiConfig(id, m_isr));
}

LightSensor::LightSensor(const uint8_t id)
		: LightSensor(id, nullptr)
{}

bool LightSensor::IsDetected() const
{
	return m_pin.Get();
}

#else
LightSensor::LightSensor(const uint8_t, const OnDetectListener&)
		: LightSensor(0)
{}
LightSensor::LightSensor(const uint8_t)
		: m_pin(nullptr)
{
	LOG_DL("Configured not to use LightSensor");
}
bool LightSensor::IsDetected() const { return false; }

#endif

}
}
