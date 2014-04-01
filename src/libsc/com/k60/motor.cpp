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

#include <assert.h>
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

namespace
{

#if LIBSC_USE_MOTOR == 1
#define GetFtmModule(x) FtmUtils::GetFtmModule<LIBSC_MOTOR0_PWM>()

#else
inline FTMn_e GetFtmModule(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(0);
	case 0:
		return FtmUtils::GetFtmModule<LIBSC_MOTOR0_PWM>();

	case 1:
		return FtmUtils::GetFtmModule<LIBSC_MOTOR1_PWM>();
	}
}

#endif

#if LIBSC_USE_MOTOR == 1
#define GetFtmChannel(x) FtmUtils::GetFtmChannel<LIBSC_MOTOR0_PWM>()

#else
inline FTM_CHn_e GetFtmChannel(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(0);
	case 0:
		return FtmUtils::GetFtmChannel<LIBSC_MOTOR0_PWM>();

	case 1:
		return FtmUtils::GetFtmChannel<LIBSC_MOTOR1_PWM>();
	}
}

#endif

#if LIBSC_USE_MOTOR == 1
#define GetDirGpio(x) LIBSC_MOTOR0_DIR

#else
inline PTXn_e GetDirGpio(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(0);
	case 0:
		return LIBSC_MOTOR0_DIR;

	case 1:
		return LIBSC_MOTOR1_DIR;
	}
}

#endif

}

Motor::Motor(const uint8_t id)
		: m_id(id), multiplier(1)
{
	m_power = 0;
	m_is_clockwise = true;

	FTM_PWM_init(GetFtmModule(id), GetFtmChannel(id), 10000, 0);
	gpio_init(GetDirGpio(id), GPO, 1);
}

Motor::Motor(const uint8_t id, const float m)
	: m_id(id), m_power(0), m_is_clockwise(true), multiplier(m)
{
	FTM_PWM_init(GetFtmModule(id), GetFtmChannel(id), 10000, 0);
	gpio_init(GetDirGpio(id), GPO, 1);
}

void Motor::SetPower(const uint16_t power)
{
	const uint16_t real_power = libutil::Clamp<uint16_t>(0, power * multiplier, 10000);
	if (m_power == real_power)
	{
		return;
	}

	FTM_PWM_Duty(GetFtmModule(m_id), GetFtmChannel(m_id), real_power);
	m_power = real_power;
	return;
}

void Motor::AddPower(const int16_t power)
{
	SetPower(libutil::Clamp<int>(0, m_power + power, 10000));
}

void Motor::SetClockwise(const bool flag)
{
	if (m_is_clockwise == flag)
	{
		return;
	}

	gpio_set(GetDirGpio(m_id), flag ? 1 : 0);
	m_is_clockwise = flag;
}

#else
Motor::Motor(const uint8_t id) {}
Motor::Motor(const uint8_t id, const float m) {}
void Motor::SetPower(const uint16_t power) {}
void Motor::AddPower(const int16_t power) {}
void Motor::SetClockwise(const bool flag) {}

#endif

}
