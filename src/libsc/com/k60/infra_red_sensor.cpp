/*
 * infra_red_sensor.h
 * Infra-red sensor
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>
#include <hw_common.h>
#include <cstdint>

#include <MK60_gpio.h>

#include "libsc/com/config.h"
#include "libsc/com/infra_red_sensor.h"
#include "libsc/com/k60/infra_red_sensor.h"
#include "libsc/com/k60/port_isr_manager.h"

namespace libsc
{

#ifdef LIBSC_USE_INFRA_RED_SENSOR

namespace
{

#if LIBSC_USE_INFRA_RED_SENSOR == 1
#define GetGpio(x) LIBSC_INFRA_RED0

#else
inline PTXn_e GetGpio(const uint8_t id)
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

}

InfraRedSensor::InfraRedSensor(const uint8_t id)
		: m_id(id)
{
	gpio_init(GetGpio(m_id), GPI, 0);
}

bool InfraRedSensor::IsDetected()
{
	return gpio_get(GetGpio(m_id));
}

namespace k60
{

void InfraRedSensor::SetOnDetectHandler(
		PortIsrManager::OnTriggerPortIsrListener listener)
{
	port_init_NoALT(GetGpio(GetId()), IRQ_RISING | PF);
	PortIsrManager::GetInstance()->SetIsrHandler(PTX(GetGpio(GetId())),
			PTn(GetGpio(GetId())), listener);
}

}

#else
InfraRedSensor::InfraRedSensor(const uint8_t) : m_id(0) {}
bool InfraRedSensor::IsDetected() { return false; }
namespace k60
{
void InfraRedSensor::SetOnDetectHandler(PortIsrManager::OnTriggerPortIsrListener) {}
}

#endif

}
