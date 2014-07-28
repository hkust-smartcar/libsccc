/*
 * infra_red_sensor.h
 * Infra-red sensor
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_K60_INFRA_RED_SENSOR_H_
#define LIBSC_K60_INFRA_RED_SENSOR_H_

#include <cstdint>

#include "libbase/k60/gpio.h"

#include "libsc/k60/pin_isr_manager.h"

namespace libsc
{
namespace k60
{

class InfraRedSensor
{
public:
	explicit InfraRedSensor(const uint8_t id);

	bool IsDetected() const;
	void SetOnDetectListener(const PinIsrManager::OnPinIrqListener &listener);

private:
	libbase::k60::Gpi m_pin;
};

}
}

#endif /* LIBSC_K60_INFRA_RED_SENSOR_H_ */
