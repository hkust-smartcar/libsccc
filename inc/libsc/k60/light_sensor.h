/*
 * light_sensor.h
 * Light sensor
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

class LightSensor
{
public:
	typedef std::function<void(const uint8_t id)> OnDetectListener;

	LightSensor(const uint8_t id, const OnDetectListener &listener);
	explicit LightSensor(const uint8_t id);

	bool IsDetected() const;

private:
	libbase::k60::Gpi m_pin;
	OnDetectListener m_isr;
};

}
}
