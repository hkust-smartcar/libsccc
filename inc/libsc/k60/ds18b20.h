/*
 * ds18b20.h
 * DS18B20 digital thermometer
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/k60/gpio.h"

namespace libsc
{
namespace k60
{

class Ds18b20
{
public:
	struct Config
	{
		uint8_t id;
	};

	explicit Ds18b20(const Config &config);

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
