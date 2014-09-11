/*
 * motor.cpp
 * DC Motor
 *
 * Author: Harrison Ng, Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cassert>
#include <cstdint>

#include "libbase/log.h"
#include "libbase/k60/ftm_pwm.h"
#include "libbase/k60/gpio.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/pwm_utils.h"

#include "libsc/config.h"
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
inline Pin::Name GetPwmPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_MOTOR0_PWM;
}

inline Pin::Name GetDirPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_MOTOR0_DIR;
}

#else
inline Pin::Name GetPwmPin(const uint8_t id)
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

inline Pin::Name GetDirPin(const uint8_t id)
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
	config.precision = Pwm::Config::Precision::KNs;
	config.alignment = FtmPwm::Config::Alignment::kCenter;
	return config;
}

Gpo::Config GetDirConfig(const uint8_t id)
{
	Gpo::Config config;
	config.pin = GetDirPin(id);
	return config;
}

}

Motor::Motor(const uint8_t id, const bool is_clockwise_high,
		const uint8_t multiplier)
		: m_is_clockwise_high(is_clockwise_high),
		  m_multiplier(multiplier),
		  m_pwm(GetFtmPwmConfig(id)),
		  m_dir(GetDirConfig(id)),
		  m_power(0),
		  m_is_clockwise(true)
{
	m_dir.Set(is_clockwise_high);
}

Motor::Motor(const uint8_t id, const bool is_clockwise_high)
		: Motor(id, is_clockwise_high, 100)
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

	m_dir.Set(!(flag ^ m_is_clockwise_high));
	m_is_clockwise = flag;
}

#else
Motor::Motor(const uint8_t, const bool)
		: m_is_clockwise_high(false), m_multiplier(0), m_pwm(nullptr),
		  m_dir(nullptr), m_power(0), m_is_clockwise(false)
{
	LOG_DL("Configured not to use Motor");
}
Motor::Motor(const uint8_t, const bool, const uint8_t) : Motor(0, false) {}
void Motor::SetPower(const uint16_t) {}
void Motor::AddPower(const int16_t) {}
void Motor::SetClockwise(const bool) {}

#endif

}
}
