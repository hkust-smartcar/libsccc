/*
 * ambient_light_sensor.cpp
 * LS1970
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>
#include <hw_common.h>
#include <cstdint>

#include <MK60_adc.h>

#include "libsc/com/config.h"
#include "libsc/com/ambient_light_sensor.h"
#include "libsc/com/k60/adc_utils.h"

#define LIBSC_AMBIENT_LIGHT0 PTA8
#define ADC_BEGIN 9930

namespace libsc
{

AmbientLightSensor::AmbientLightSensor(const uint8_t id)
		: m_id(id)
{
	adc_init(AdcUtils::GetAdcChannel<LIBSC_AMBIENT_LIGHT0>());
}

uint16_t AmbientLightSensor::GetLux()
{
	const uint16_t raw = adc_once(AdcUtils::GetAdcChannel<LIBSC_AMBIENT_LIGHT0>(),
			ADC_16bit);
	const float ratio = (raw - ADC_BEGIN) / (float)(UINT16_MAX - ADC_BEGIN);
	return ratio * 500;

}

}
