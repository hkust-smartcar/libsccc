/*
 * temperature_sensor.h
 * DS18B20 temperature sensor
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <cstdint>

#include "libbase/k60/gpio.h"

namespace libsc
{
namespace k60
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

	libbase::k60::Gpio m_pin;
	float m_temperature;
};

}
}
