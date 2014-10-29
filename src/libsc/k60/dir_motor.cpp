/*
 * dir_motor.cpp
 * DC Motor
 *
 * Author: Harrison Ng, Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
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
#include "libsc/k60/dir_motor.h"
#include "libutil/misc.h"

// 10 kHz
#define PERIOD 100000

#ifndef LIBSC_DIR_MOTOR_CW_LEVEL
	#define LIBSC_DIR_MOTOR_CW_LEVEL 1
#endif

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
	// CW by default
	config.is_high = LIBSC_DIR_MOTOR_CW_LEVEL;
	return config;
}

}

DirMotor::DirMotor(const Config &config)
		: Motor(config),
		  m_pwm(GetFtmPwmConfig(config.id)),
		  m_dir(GetDirConfig(config.id)),
		  m_power(0),
		  m_is_clockwise(true)
{}

void DirMotor::SetPower(const uint16_t power)
{
	const uint16_t real_power = libutil::Clamp<Uint>(0,
			power * GetMultiplier() / 100, 1000);
	if (m_power == real_power)
	{
		return;
	}

	m_pwm.SetPosWidth(PwmUtils::GetPosWidth(PERIOD, real_power));
	m_power = real_power;
	return;
}

void DirMotor::AddPower(const int16_t power)
{
	SetPower(libutil::Clamp<int>(0, m_power + power, 1000));
}

void DirMotor::SetClockwise(const bool flag)
{
	if (m_is_clockwise == flag)
	{
		return;
	}

	m_dir.Set(!(flag ^ LIBSC_DIR_MOTOR_CW_LEVEL));
	m_is_clockwise = flag;
}

#else
DirMotor::DirMotor(const Config &config)
		: Motor(config),
		  m_pwm(nullptr), m_dir(nullptr), m_power(0), m_is_clockwise(false)
{
	LOG_DL("Configured not to use DirMotor");
}
void DirMotor::SetPower(const uint16_t) {}
void DirMotor::AddPower(const int16_t) {}
void DirMotor::SetClockwise(const bool) {}

#endif

}
}
