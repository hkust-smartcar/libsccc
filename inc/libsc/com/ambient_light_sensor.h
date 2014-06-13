/*
 * ambient_light_sensor.h
 * LS1970
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_AMBIENT_LIGHT_SENSOR_H_
#define LIBSC_AMBIENT_LIGHT_SENSOR_H_

#include <cstdint>

namespace libsc
{

class AmbientLightSensor
{
public:
	explicit AmbientLightSensor(const uint8_t id);

	uint16_t GetLux();

private:
	const uint8_t m_id;
};

}

#endif /* LIBSC_AMBIENT_LIGHT_SENSOR_H_ */
