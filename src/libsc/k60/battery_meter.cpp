/*
 * battery_meter.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/log.h"
#include "libbase/k60/adc.h"

#include "libsc/config.h"
#include "libsc/k60/battery_meter.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_BATTERY_METER

namespace
{

Adc::Config GetAdcConfig()
{
	Adc::Config config;
	config.adc = LIBSC_BATTERY_METER;
	config.speed = Adc::Config::SpeedMode::kExFast;
	config.is_continuous_mode = true;
	config.avg_pass = Adc::Config::AveragePass::k4;
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
		: m_adc(nullptr)
{
	LOG_DL("Configured not to use BatteryMeter");
}
float BatteryMeter::GetVoltage() { return 0.0f; }

#endif /* LIBSC_USE_BATTERY_METER */

}
}
