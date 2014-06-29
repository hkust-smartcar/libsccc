/*
 * light_sensor.h
 * Light sensor
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_K60_LIGHT_SENSOR_H_
#define LIBSC_K60_LIGHT_SENSOR_H_

#include <cstdint>

#include "libbase/k60/gpio.h"

namespace libsc
{
namespace k60
{

class LightSensor
{
public:
	explicit LightSensor(const uint8_t id);

	bool IsDetected() const;

private:
	libbase::k60::Gpi m_pin;
};

}
}

#endif /* LIBSC_K60_LIGHT_SENSOR_H_ */
