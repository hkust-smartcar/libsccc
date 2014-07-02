/*
 * infra_red_sensor.h
 * Infra-red sensor
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_INFRA_RED_SENSOR_H_
#define LIBSC_INFRA_RED_SENSOR_H_

#include <cstdint>

namespace libsc
{

class InfraRedSensor
{
public:
	explicit InfraRedSensor(const uint8_t id);

	bool IsDetected();

protected:
	uint8_t GetId() const
	{
		return m_id;
	}

private:
	const uint8_t m_id;
};

}

#endif /* LIBSC_INFRA_RED_SENSOR_H_ */
