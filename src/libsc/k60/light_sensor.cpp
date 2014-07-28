/*
 * light_sensor.cpp
 * Light sensor
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cassert>
#include <cstdint>

#include <functional>

#include <log.h>

#include "libbase/k60/gpio.h"

#include "libsc/com/config.h"
#include "libsc/k60/light_sensor.h"
#include "libsc/k60/pin_isr_manager.h"

using namespace libbase::k60;
using namespace std;

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_LIGHT_SENSOR

namespace
{

#if LIBSC_USE_LIGHT_SENSOR == 1
inline PinConfig::Name GetPin(const uint8_t)
{
	return LIBSC_LIGHT_SENSOR0;
}

#else
inline PinConfig::Name GetPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(0);

	case 0:
		return LIBSC_LIGHT_SENSOR0;

	case 1:
		return LIBSC_LIGHT_SENSOR1;
	}
}

#endif

void OnPinIrqListener(const PinConfig::Name pin,
		const LightSensor::OnDetectListener &listener)
{
	switch (pin)
	{
	default:
		assert(false);

	case LIBSC_LIGHT_SENSOR0:
		listener(0);
		return;

#if LIBSC_USE_LIGHT_SENSOR > 1
	case LIBSC_LIGHT_SENSOR1:
		listener(1);
		return;
#endif
	}
}

Gpi::Config GetGpiConfig(const uint8_t id)
{
	Gpi::Config config;
	config.pin = GetPin(id);
	config.config.set(PinConfig::ConfigBit::kPassiveFilter);
	return config;
}

}

LightSensor::LightSensor(const uint8_t id)
		: m_pin(GetGpiConfig(id))
{}

bool LightSensor::IsDetected() const
{
	return m_pin.Get();
}

void LightSensor::SetOnDetectListener(const OnDetectListener &listener)
{
	m_pin.GetPin()->SetInterrupt(PinConfig::Interrupt::kRising);
	if (listener)
	{
		PinIsrManager::GetInstance()->SetPinIsr(m_pin.GetPin(),
				std::bind(OnPinIrqListener, placeholders::_1, listener));
	}
	else
	{
		PinIsrManager::GetInstance()->SetPinIsr(m_pin.GetPin(), nullptr);
	}
}

#else
LightSensor::LightSensor(const uint8_t)
		: m_pin(nullptr)
{
	LOG_D("Configured not to use LightSensor");
}
bool LightSensor::IsDetected() const { return false; }
void LightSensor::SetOnDetectListener(const OnDetectListener&) {}

#endif

}
}
