/*
 * ds18b20.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/helper.h"
#include LIBBASE_H(gpio)

namespace libsc
{

/**
 * DS18B20 digital thermometer
 */
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

	LIBBASE_MODULE(Gpio) m_pin;
	float m_temperature;
};

}
