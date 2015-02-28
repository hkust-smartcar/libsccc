/*
 * infra_red_sensor.h
 * Infra-red sensor
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
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
	libbase::k60::Gpi m_pin;
};

}
}
