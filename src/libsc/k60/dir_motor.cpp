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
#include "libbase/k60/pin.h"
#include "libbase/k60/pwm_utils.h"

#include "libsc/config.h"
#include "libsc/k60/dir_motor.h"

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

#if defined(LIBSC_USE_MOTOR) && defined(LIBSC_MOTOR0_PWM) \
		&& defined(LIBSC_MOTOR0_DIR)

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
		  m_dir(GetDirConfig(config.id))
{}

void DirMotor::OnSetPower(const uint16_t power)
{
	m_pwm.SetPosWidth(PwmUtils::GetPosWidth(PERIOD, power));
}

void DirMotor::OnSetClockwise(const bool flag)
{
	m_dir.Set(!(flag ^ LIBSC_DIR_MOTOR_CW_LEVEL));
}

#else
DirMotor::DirMotor(const Config &config)
		: Motor(config),
		  m_pwm(nullptr), m_dir(nullptr)
{
	LOG_DL("Configured not to use DirMotor");
}
void DirMotor::OnSetPower(const uint16_t) {}
void DirMotor::OnSetClockwise(const bool) {}

#endif

}
}
