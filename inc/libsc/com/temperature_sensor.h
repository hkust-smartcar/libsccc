/*
 * temperature_sensor.h
 * DS18B20 temperature sensor
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_TEMPERATURE_SENSOR_H_
#define LIBSC_TEMPERATURE_SENSOR_H_

#include <cstdint>

namespace libsc
{

class TemperatureSensor
{
public:
	explicit TemperatureSensor(const uint8_t id);

	void UpdateTemperature();

	float GetTemperature() const
	{
		return m_temperature;
	}

private:
	bool Init();
	void SendByte(const uint8_t byte);
	uint8_t ReceiveByte();

	uint8_t m_id;
	float m_temperature;
};

}

#endif /* LIBSC_TEMPERATURE_SENSOR_H_ */
