/*
 * infra_red_sensor.h
 * Infra-red sensor
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstdint>

#include <functional>

#include "libbase/log.h"
#include "libbase/helper.h"
#include LIBBASE_H(gpio)

#include "libsc/config.h"
#include "libsc/infra_red_sensor.h"

using namespace LIBBASE_NS;

namespace libsc
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

Gpi::Config GetGpiConfig(const InfraRedSensor::Config &config,
		const Gpi::OnGpiEventListener &listener)
{
	Gpi::Config product;
	product.pin = GetPin(config.id);
	product.config.set(Pin::Config::ConfigBit::kPassiveFilter);
	if (listener)
	{
		switch (config.listener_trigger)
		{
		default:
		case InfraRedSensor::Config::Trigger::kEnter:
			product.interrupt = (config.is_active_low
					? Pin::Config::Interrupt::kFalling
					: Pin::Config::Interrupt::kRising);
			break;

		case InfraRedSensor::Config::Trigger::kLeave:
			product.interrupt = (config.is_active_low
					? Pin::Config::Interrupt::kRising
					: Pin::Config::Interrupt::kFalling);
			break;

		case InfraRedSensor::Config::Trigger::kBoth:
			product.interrupt = Pin::Config::Interrupt::kBoth;
			break;
		}
		product.isr = listener;
	}
	return product;
}

}

InfraRedSensor::InfraRedSensor(const Config &config)
		: m_pin(nullptr),
		  m_is_active_low(config.is_active_low)
{
	Gpi::OnGpiEventListener listener;
	if (config.listener)
	{
		const uint8_t id = config.id;
		InfraRedSensor::Listener ir_listener = config.listener;
		listener = [ir_listener, id](Gpi*)
				{
					ir_listener(id);
				};
	}
	m_pin = Gpi(GetGpiConfig(config, listener));
}

bool InfraRedSensor::IsDetected() const
{
	return (m_pin.Get() ^ m_is_active_low);
}

#else
InfraRedSensor::InfraRedSensor(const Config&)
		: m_pin(nullptr)
{
	LOG_DL("Configured not to use InfraRedSensor");
}
bool InfraRedSensor::IsDetected() const { return false; }

#endif

}
