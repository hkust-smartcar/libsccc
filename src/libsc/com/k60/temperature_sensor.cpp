/*
 * temperature_sensor.cpp
 * DS18B20 temperature sensor
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>
#include <hw_common.h>
#include <cmath>

#include <MK60_gpio.h>

#include "libsc/com/config.h"
#include "libsc/com/temperature_sensor.h"

#define CMD_SKIP 0xCC
#define CMD_CONVERT_T 0x44
#define CMD_READ_SP 0xBE

namespace libsc
{

#ifdef LIBSC_USE_TEMPERATURE_SENSOR

namespace
{

#define GetGpio(x) LIBSC_TEMPERATURE0

}

TemperatureSensor::TemperatureSensor(const uint8_t id)
		: m_id(id), m_temperature(0.0f)
{
	gpio_init(GetGpio(m_id), GPO, 0);
}

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
	gpio_ddr(GetGpio(m_id), GPO);
	gpio_set(GetGpio(m_id), 0);
	DELAY_US(480);
	//gpio_set(GetGpio(m_id), 1);
	gpio_ddr(GetGpio(m_id), GPI);
	DELAY_US(60);
	if (gpio_get(GetGpio(m_id)))
	{
		LOG_E("Failed initializing temperature sensor");
		return false;
	}
	DELAY_US(240);
	return true;
}

void TemperatureSensor::SendByte(const uint8_t byte)
{
	gpio_ddr(GetGpio(m_id), GPO);
	for (int i = 0; i < 8; ++i)
	{
		gpio_set(GetGpio(m_id), 0);
		if ((byte >> i) & 0x1)
		{
			gpio_ddr(GetGpio(m_id), GPI);
			DELAY_US(60);
			gpio_ddr(GetGpio(m_id), GPO);
		}
		else
		{
			DELAY_US(60);
			gpio_set(GetGpio(m_id), 1);
		}
		DELAY_US(1);
	}
}

uint8_t TemperatureSensor::ReceiveByte()
{
	uint8_t product = 0;
	for (int i = 0; i < 8; ++i)
	{
		gpio_ddr(GetGpio(m_id), GPO);
		gpio_set(GetGpio(m_id), 0);
		DELAY_US(1);
		gpio_ddr(GetGpio(m_id), GPI);
		product |= (gpio_get(GetGpio(m_id)) ? 1 : 0) << i;
		DELAY_US(61);
	}
	return product;
}

#else

TemperatureSensor::TemperatureSensor(const uint8_t)
		: m_id(0), m_temperature(0.0f)
{}
void TemperatureSensor::UpdateTemperature() {}

#endif /* LIBSC_USE_TEMPERATURE_SENSOR */

}
