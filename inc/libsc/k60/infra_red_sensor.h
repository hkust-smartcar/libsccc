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

	struct Config
	{
		uint8_t id;
		OnDetectListener listener;
	};

	explicit InfraRedSensor(const Config &config);

	bool IsDetected() const;

private:
	OnDetectListener m_isr;
	libbase::k60::Gpi m_pin;
};

}
}
