/*
 * temperature_sensor.cpp
 * DS18B20 temperature sensor
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <hw_common.h>
#include <cstdint>
#include <cmath>

#include <log.h>

#include "libbase/k60/gpio.h"

#include "libsc/com/config.h"
#include "libsc/k60/temperature_sensor.h"

using namespace libbase::k60;

#define CMD_SKIP 0xCC
#define CMD_CONVERT_T 0x44
#define CMD_READ_SP 0xBE

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_TEMPERATURE_SENSOR

namespace
{

#if LIBSC_USE_TEMPERATURE_SENSOR == 1
inline PinConfig::Name GetPin(const uint8_t)
{
	return LIBSC_TEMPERATURE0;
}

#endif

GpoConfig GetGpoConfig(const uint8_t id)
{
	GpoConfig config;
	config.pin = GetPin(id);
	return config;
}

}

TemperatureSensor::TemperatureSensor(const uint8_t id)
		: m_pin(GetGpoConfig(id)), m_temperature(0.0f)
{}

void TemperatureSensor::UpdateTemperature()
{
	if (Init())
	{
		SendByte(CMD_SKIP);
		SendByte(CMD_CONVERT_T);
		if (Init())
		{
			SendByte(CMD_SKIP);
			SendByte(CMD_READ_SP);
			uint16_t data = ReceiveByte();
			data |= ReceiveByte() << 8;

			const uint8_t fractional = data & 0x0F;
			m_temperature = 0.0f;
			for (int i = 0; i < 4; ++i)
			{
				if (fractional & (0x08 >> i))
				{
					m_temperature += 0.5f / powf(2.0f, i);
				}
			}
			data >>= 4;
			if (data & 0x80)
			{
				data = 0xFFFF - data;
				m_temperature += data & 0x7F;
				m_temperature *= -1;
			}
			else
			{
				m_temperature += data & 0x7F;
			}
		}
	}
}

bool TemperatureSensor::Init()
{
	m_pin.Set(false);
	DELAY_US(480);
	//m_pin.Set(true);
	m_pin.EnsureGpi();
	DELAY_US(60);
	if (m_pin.Get())
	{
		LOG_E("Failed initializing temperature sensor");
		return false;
	}
	DELAY_US(240);
	return true;
}

void TemperatureSensor::SendByte(const uint8_t byte)
{
	for (int i = 0; i < 8; ++i)
	{
		m_pin.Set(false);
		if ((byte >> i) & 0x1)
		{
			m_pin.EnsureGpi();
			DELAY_US(60);
			m_pin.EnsureGpo();
		}
		else
		{
			DELAY_US(60);
			m_pin.Set(true);
		}
		DELAY_US(1);
	}
}

uint8_t TemperatureSensor::ReceiveByte()
{
	uint8_t product = 0;
	for (int i = 0; i < 8; ++i)
	{
		m_pin.Set(false);
		DELAY_US(1);
		product |= (m_pin.Get() ? 1 : 0) << i;
		DELAY_US(61);
	}
	return product;
}

#else

TemperatureSensor::TemperatureSensor(const uint8_t)
		: m_pin(nullptr), m_temperature(0.0f)
{
	LOG_D("Configured not to use TemperatureSensor");
}
void TemperatureSensor::UpdateTemperature() {}

#endif /* LIBSC_USE_TEMPERATURE_SENSOR */

}
}
