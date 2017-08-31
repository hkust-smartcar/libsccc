/*
 * battery_meter.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libbase/helper.h"
#include LIBBASE_H(adc)

namespace libsc
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
	LIBBASE_MODULE(Adc) m_adc;
	float m_voltage_ratio;
};

}
