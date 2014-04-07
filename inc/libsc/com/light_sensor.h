/*
 * light_sensor.h
 * Light sensor
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_LIGHT_SENSOR_H_
#define LIBSC_LIGHT_SENSOR_H_

#include <cstdint>

namespace libsc
{

class LightSensor
{
public:
	explicit LightSensor(const uint8_t id);

	bool IsDetected();

private:
	uint8_t m_id;
};

}

#endif /* LIBSC_LIGHT_SENSOR_H_ */
