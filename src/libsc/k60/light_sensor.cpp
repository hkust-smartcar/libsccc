/*
 * light_sensor.cpp
 * Light sensor
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cassert>
#include <cstdint>

#include <log.h>

#include "libbase/k60/gpio.h"

#include "libsc/com/config.h"
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
inline PinConfig::Name GetPin(const uint8_t)
{
	return LIBSC_LIGHT_SENSOR0;
}

#else
inline PinConfig::Name GetPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(0);

	case 0:
		return LIBSC_LIGHT_SENSOR0;

	case 1:
		return LIBSC_LIGHT_SENSOR1;
	}
}

#endif

GpiConfig GetGpiConfig(const uint8_t id)
{
	GpiConfig config;
	config.pin = GetPin(id);
	config.config.set(PinConfig::ConfigBit::PASSIVE_FILTER);
	return config;
}

}

LightSensor::LightSensor(const uint8_t id)
		: m_pin(GetGpiConfig(id))
{}

bool LightSensor::IsDetected()
{
	return m_pin.Get();
}

#else
LightSensor::LightSensor(const uint8_t)
		: m_pin(nullptr)
{
	LOG_D("Configured not to use LightSensor");
}
bool LightSensor::IsDetected() const { return false; }

#endif

}
}
