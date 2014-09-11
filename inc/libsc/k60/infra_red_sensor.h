/*
 * infra_red_sensor.h
 * Infra-red sensor
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <cstdint>

#include <functional>

#include "libbase/k60/gpio.h"

namespace libsc
{
namespace k60
{

class InfraRedSensor
{
public:
	typedef std::function<void(const uint8_t id)> OnDetectListener;

	InfraRedSensor(const uint8_t id, const OnDetectListener &listener);
	explicit InfraRedSensor(const uint8_t id);

	bool IsDetected() const;

private:
	libbase::k60::Gpi m_pin;
	OnDetectListener m_isr;
};

}
}
