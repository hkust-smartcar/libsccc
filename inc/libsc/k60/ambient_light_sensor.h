/*
 * ambient_light_sensor.h
 * LS1970
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
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
