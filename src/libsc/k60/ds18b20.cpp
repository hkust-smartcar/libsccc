/*
 * ds18b20.h
 * DS18B20 digital thermometer
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstdint>
#include <cmath>

#include "libbase/log.h"
#include "libbase/k60/gpio.h"

#include "libsc/config.h"
#include "libsc/device_h/ds18b20.h"
#include "libsc/k60/ds18b20.h"
#include "libsc/k60/system.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_TEMPERATURE_SENSOR

namespace
{

#if LIBSC_USE_TEMPERATURE_SENSOR == 1
inline PinConfig::Name GetPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
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

Ds18b20::Ds18b20(const Config &config)
		: m_pin(GetGpoConfig(config.id)),
		  m_temperature(0.0f)
{}

void Ds18b20::UpdateTemperature()
{
	if (Init())
	{
		SendByte(DS18B20_SKIP);
		SendByte(DS18B20_CONVERT_T);
		if (Init())
		{
			SendByte(DS18B20_SKIP);
			SendByte(DS18B20_READ_SP);
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

bool Ds18b20::Init()
{
	m_pin.Set(false);
	System::DelayUs(480);
	//m_pin.Set(true);
	m_pin.EnsureGpi();
	System::DelayUs(60);
	if (m_pin.Get())
	{
		LOG_E("Failed initializing temperature sensor");
		return false;
	}
	System::DelayUs(240);
	return true;
}

void Ds18b20::SendByte(const uint8_t byte)
{
	for (int i = 0; i < 8; ++i)
	{
		m_pin.Set(false);
		if ((byte >> i) & 0x1)
		{
			m_pin.EnsureGpi();
			System::DelayUs(60);
			m_pin.EnsureGpo();
		}
		else
		{
			System::DelayUs(60);
			m_pin.EnsureGpi();
		}
		System::DelayUs(1);
	}
}

uint8_t Ds18b20::ReceiveByte()
{
	uint8_t product = 0;
	for (int i = 0; i < 8; ++i)
	{
		m_pin.Set(false);
		System::DelayUs(1);
		product |= (m_pin.Get() ? 1 : 0) << i;
		System::DelayUs(61);
	}
	return product;
}

#else

Ds18b20::Ds18b20(const Config&)
		: m_pin(nullptr), m_temperature(0.0f)
{
	LOG_DL("Configured not to use Ds18b20");
}
void Ds18b20::UpdateTemperature() {}

#endif /* LIBSC_USE_TEMPERATURE_SENSOR */

}
}
