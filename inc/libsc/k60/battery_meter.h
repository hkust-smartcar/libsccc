/*
 * battery_meter.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libbase/k60/adc.h"

namespace libsc
{
namespace k60
{

class BatteryMeter
{
public:
	struct Config
	{
		float voltage_ratio;
	};

	explicit BatteryMeter(const Config &config);

	float GetVoltage();

private:
	libbase::k60::Adc m_adc;
	float m_voltage_ratio;
};

}
}
