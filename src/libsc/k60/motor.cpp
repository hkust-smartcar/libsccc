/*
 * motor.cpp
 * DC Motor
 *
 * Author: Harrison Ng, Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cassert>
#include <cstdint>

#include "libbase/k60/ftm_pwm.h"
#include "libbase/k60/gpio.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pwm_utils.h"

#include "libsc/com/config.h"
#include "libsc/k60/motor.h"
#include "libutil/misc.h"

// 10 kHz
#define PERIOD 100000

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_MOTOR

namespace
{

#if LIBSC_USE_MOTOR == 1
inline PinConfig::Name GetPwmPin(const uint8_t)
{
	return LIBSC_MOTOR0_PWM;
}

inline PinConfig::Name GetDirPin(const uint8_t)
{
	return LIBSC_MOTOR0_DIR;
}

#else
inline PinConfig::Name GetPwmPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);

	case 0:
		return LIBSC_MOTOR0_PWM;

	case 1:
		return LIBSC_MOTOR1_PWM;
	}
}

inline PinConfig::Name GetDirPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);

	case 0:
		return LIBSC_MOTOR0_DIR;

	case 1:
		return LIBSC_MOTOR1_DIR;
	}
}

#endif

FtmPwm::Config GetFtmPwmConfig(const uint8_t id)
{
	FtmPwm::Config config;
	config.pin = GetPwmPin(id);
	config.period = PERIOD;
	config.pos_width = 0;
	config.precision = Pwm::Config::Precision::NS;
	return config;
}

Gpo::Config GetDirConfig(const uint8_t id)
{
	Gpo::Config config;
	config.pin = GetDirPin(id);
	return config;
}

}

Motor::Motor(const uint8_t id)
		: Motor(id, 100)
{}

Motor::Motor(const uint8_t id, const uint8_t multiplier)
		: m_pwm(GetFtmPwmConfig(id)),
		  m_dir(GetDirConfig(id)),
		  m_multiplier(multiplier),
		  m_power(0),
		  m_is_clockwise(true)
{}

void Motor::SetPower(const uint16_t power)
{
	const uint16_t real_power = libutil::Clamp<Uint>(0,
			power * m_multiplier / 100, 1000);
	if (m_power == real_power)
	{
		return;
	}

	m_pwm.SetPosWidth(PwmUtils::GetPosWidth(PERIOD, real_power));
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

#ifndef LIBSC_NEGATE_MOTOR
	m_dir.Set(flag);
#else
	m_dir.Set(!flag);
#endif
	m_is_clockwise = flag;
}

#else
Motor::Motor(const uint8_t) : m_id(0) {}
Motor::Motor(const uint8_t, const float) : m_id(0) {}
void Motor::SetPower(const uint16_t) {}
void Motor::AddPower(const int16_t) {}
void Motor::SetClockwise(const bool) {}

#endif

}
}
