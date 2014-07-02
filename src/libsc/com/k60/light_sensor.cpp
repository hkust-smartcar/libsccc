/*
 * light_sensor.cpp
 * Light sensor
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>
#include <hw_common.h>
#include <cstdint>

#include <MK60_gpio.h>

#include "libsc/com/config.h"
#include "libsc/com/light_sensor.h"

namespace libsc
{

#ifdef LIBSC_USE_LIGHT_SENSOR

namespace
{

#if LIBSC_USE_LIGHT_SENSOR == 1
#define GetGpio(x) LIBSC_LIGHT_SENSOR0

#else
inline PTXn_e GetGpio(const uint8_t id)
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

}

LightSensor::LightSensor(const uint8_t id)
		: m_id(id)
{
	gpio_init(GetGpio(m_id), GPI, 0);
}

bool LightSensor::IsDetected()
{
	return gpio_get(GetGpio(m_id));
}

#else
LightSensor::LightSensor(const uint8_t) : m_id(0) {}
bool LightSensor::IsDetected() { return false; }

#endif

}
