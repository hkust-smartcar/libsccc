/*
 * ambient_light_sensor.h
 * LS1970
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/k60/adc.h"

namespace libsc
{
namespace k60
{

class AmbientLightSensor
{
public:
	struct Config
	{
		uint8_t id;
	};

	explicit AmbientLightSensor(const Config &config);

	uint16_t GetLux();

private:
	libbase::k60::Adc m_pin;
};

}
}
