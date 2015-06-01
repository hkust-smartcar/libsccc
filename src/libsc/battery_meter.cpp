/*
 * battery_meter.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/log.h"
#include "libbase/helper.h"
#include LIBBASE_H(adc)

#include "libsc/config.h"
#include "libsc/battery_meter.h"

using namespace LIBBASE_NS;

namespace libsc
{

#ifdef LIBSC_USE_BATTERY_METER

namespace
{

Adc::Config GetAdcConfig()
{
	Adc::Config config;
	config.adc = LIBSC_BATTERY_METER;
	config.speed = Adc::Config::SpeedMode::kExSlow;
	config.is_continuous_mode = true;
	config.avg_pass = Adc::Config::AveragePass::k32;
	return config;
}

}

BatteryMeter::BatteryMeter(const Config &config)
		: m_adc(GetAdcConfig()),
		  m_voltage_ratio(1.0f / config.voltage_ratio)
{
	m_adc.StartConvert();
}

float BatteryMeter::GetVoltage()
{
	return m_adc.GetResultF() * m_voltage_ratio;
}

#else
BatteryMeter::BatteryMeter(const Config&)
		: m_adc(nullptr), m_voltage_ratio(0.0f)
{
	LOG_DL("Configured not to use BatteryMeter");
}
float BatteryMeter::GetVoltage() { return 0.0f; }

#endif /* LIBSC_USE_BATTERY_METER */

}
