/*
 * motor.cpp
 * Motor abstraction
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>
#include <hw_common.h>
#include <cstdint>

#include <MK60_FTM.h>
#include <MK60_gpio.h>

#include "macro.h"
#include "ftm_utils.h"
#include "libutil/misc.h"
#include "libsc/com/config.h"
#include "libsc/com/motor.h"

namespace libsc
{

#ifdef LIBSC_USE_MOTOR

Motor::Motor(const uint8_t id)
		: m_id(id)
{
	m_power = 0;
	m_is_clockwise = true;

	FTM_PWM_init(FtmUtils::GetFtmModule<LIBSC_MOTOR0_PWM>(),
			FtmUtils::GetFtmChannel<LIBSC_MOTOR0_PWM>(), 10000, 0);
	gpio_init(LIBSC_MOTOR0_DIR, GPO, 1);
}

void Motor::SetPower(const uint16_t power)
{
	const uint16_t real_power = libutil::Clamp<uint16_t>(0, power, 1000);
	if (m_power == real_power)
	{
		return;
	}

	FTM_PWM_Duty(FtmUtils::GetFtmModule<LIBSC_MOTOR0_PWM>(),
			FtmUtils::GetFtmChannel<LIBSC_MOTOR0_PWM>(), real_power * 10);
	m_power = real_power;
	return;
}

void Motor::AddPower(const int16_t power)
{
	SetPower(libutil::Clamp<int>(0, m_power + power, 1000));
}

void Motor::SetClockwise(const bool flag)
{
	if (m_is_clockwise == flag)
	{
		return;
	}

	LIBSC_PIN_OUT(LIBSC_MOTOR0_DIR) = flag ? 1 : 0;
	m_is_clockwise = flag;
}

#else
Motor::Motor(const uint8_t id) {}
void Motor::SetPower(const uint16_t power) {}
void Motor::AddPower(const int16_t power) {}
void Motor::SetClockwise(const bool flag) {}

#endif

}
