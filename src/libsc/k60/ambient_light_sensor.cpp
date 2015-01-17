/*
 * ambient_light_sensor.cpp
 * LS1970
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstdint>

#include "libbase/log.h"
#include "libbase/k60/adc.h"
#include "libbase/k60/pin.h"

#include "libsc/config.h"
#include "libsc/k60/ambient_light_sensor.h"

using namespace libbase::k60;

#define ADC_BEGIN 9930

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_AMBIENT_LIGHT

namespace
{

#if LIBSC_USE_AMBIENT_LIGHT == 1
inline Pin::Name GetPin(const uint8_t)
{
	return LIBSC_AMBIENT_LIGHT0;
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
		return LIBSC_AMBIENT_LIGHT0;

	case 1:
		return LIBSC_AMBIENT_LIGHT1;
	}
}

#endif

Adc::Config GetAdcConfig(const uint8_t id)
{
	Adc::Config config;
	config.pin = GetPin(id);
	config.resolution = Adc::Config::Resolution::k16Bit;
	return config;
}

}

AmbientLightSensor::AmbientLightSensor(const Config &config)
		: m_pin(GetAdcConfig(config.id))
{}

uint16_t AmbientLightSensor::GetLux()
{
	const uint16_t raw = m_pin.GetResult();
	const float ratio = (raw - ADC_BEGIN) / (float)(UINT16_MAX - ADC_BEGIN);
	return ratio * 500;

}

#else
AmbientLightSensor::AmbientLightSensor(const Config&)
		: m_pin(nullptr)
{
	LOG_DL("Configured not to use AmbientLightSensor");
}
uint16_t AmbientLightSensor::GetLux() { return 0; }

#endif

}
}
