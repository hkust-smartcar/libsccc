/*
 * infra_red_sensor.h
 * Infra-red sensor
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cassert>
#include <cstdint>

#include <functional>

#include "libbase/log.h"
#include "libbase/k60/gpio.h"

#include "libsc/config.h"
#include "libsc/k60/infra_red_sensor.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_INFRA_RED_SENSOR

namespace
{

#if LIBSC_USE_INFRA_RED_SENSOR == 1
inline Pin::Name GetPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_INFRA_RED0;
}

#else
inline Pin::Name GetPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break

	case 0:
		return LIBSC_INFRA_RED0;

	case 1:
		return LIBSC_INFRA_RED1;
	}
}

#endif

Gpi::Config GetGpiConfig(const uint8_t id,
		const InfraRedSensor::OnDetectListener &listener)
{
	Gpi::Config config;
	config.pin = GetPin(id);
	config.config.set(Pin::Config::ConfigBit::kPassiveFilter);
	if (listener)
	{
		config.interrupt = Pin::Config::Interrupt::kRising;
		config.isr = [&listener](Gpi*)
				{
					listener(id);
				};
	}
	return config;
}

}

InfraRedSensor::InfraRedSensor(const uint8_t id,
		const OnDetectListener &listener)
		: m_pin(nullptr), m_isr(listener)
{
	m_pin = Gpi(GetGpiConfig(id, m_isr));
}

InfraRedSensor::InfraRedSensor(const uint8_t id)
		: InfraRedSensor(id, nullptr)
{}

bool InfraRedSensor::IsDetected() const
{
	return m_pin.Get();
}

#else
InfraRedSensor::InfraRedSensor(const uint8_t, const OnDetectListener&)
		: InfraRedSensor(0)
{}
InfraRedSensor::InfraRedSensor(const uint8_t)
		: m_pin(nullptr)
{
	LOG_DL("Configured not to use InfraRedSensor");
}
bool InfraRedSensor::IsDetected() const { return false; }

#endif

}
}
