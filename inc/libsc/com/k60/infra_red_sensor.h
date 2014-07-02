/*
 * infra_red_sensor_k60.h
 * Infra-red sensor (K60 specific)
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_K60_INFRA_RED_SENSOR_H_
#define LIBSC_K60_INFRA_RED_SENSOR_H_

#include <cstdint>

#include "libsc/com/infra_red_sensor.h"
#include "libsc/com/k60/port_isr_manager.h"

namespace libsc
{
namespace k60
{

class InfraRedSensor : public libsc::InfraRedSensor
{
public:
	explicit InfraRedSensor(const uint8_t id)
			: libsc::InfraRedSensor(id)
	{}

	void SetOnDetectHandler(PortIsrManager::OnTriggerPortIsrListener listener);
};

}
}

#endif /* LIBSC_K60_INFRA_RED_SENSOR_H_ */
