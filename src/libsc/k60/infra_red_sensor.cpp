/*
 * infra_red_sensor.h
 * Infra-red sensor
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cassert>
#include <cstdint>

#include <log.h>

#include "libbase/k60/gpio.h"

#include "libsc/com/config.h"
#include "libsc/k60/infra_red_sensor.h"
#include "libsc/k60/pin_isr_manager.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_INFRA_RED_SENSOR

namespace
{

#if LIBSC_USE_INFRA_RED_SENSOR == 1
inline PinConfig::Name GetPin(const uint8_t)
{
	return LIBSC_INFRA_RED0;
}

#else
inline PinConfig::Name GetPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(0);

	case 0:
		return LIBSC_INFRA_RED0;

	case 1:
		return LIBSC_INFRA_RED1;
	}
}

#endif

GpiConfig GetGpiConfig(const uint8_t id)
{
	GpiConfig config;
	config.pin = GetPin(id);
	config.config.set(PinConfig::ConfigBit::PASSIVE_FILTER);
	return config;
}

}

InfraRedSensor::InfraRedSensor(const uint8_t id)
		: m_pin(GetGpiConfig(id))
{}

bool InfraRedSensor::IsDetected() const
{
	return m_pin.Get();
}

void InfraRedSensor::SetOnDetectListener(
		const PinIsrManager::OnPinIrqListener &listener)
{
	m_pin.GetPin()->SetInterrupt(PinConfig::Interrupt::RISING);
	PinIsrManager::GetInstance()->SetPinIsr(m_pin.GetPin(), listener);
}

#else
InfraRedSensor::InfraRedSensor(const uint8_t)
		: m_pin(nullptr)
{
	LOG_D("Configured not to use InfraRedSensor");
}
bool InfraRedSensor::IsDetected() const { return false; }
void InfraRedSensor::SetOnDetectListener(const PinIsrManager::OnPinIrqListener&) {}

#endif

}
}
