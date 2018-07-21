/*
 * dir_motor.cpp
 * DC Motor
 *
 * Author: Harrison Ng, Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstdint>

#include "libbase/log.h"
#include "libbase/helper.h"
#include "libbase/pinout_macros.h"
#include LIBBASE_H(gpio)
#include LIBBASE_H(pin)
#include LIBBASE_H(pwm_utils)

#if PINOUT_FTM_COUNT
#include LIBBASE_H(ftm_pwm)

#elif PINOUT_TPM_COUNT
#include LIBBASE_H(tpm_pwm)

#endif // PINOUT_FTM_COUNT

#include "libsc/config.h"
#include "libsc/dir_motor.h"

// 10 kHz
#define PERIOD 100000

#ifndef LIBSC_DIR_MOTOR_CW_LEVEL
	#define LIBSC_DIR_MOTOR_CW_LEVEL 1
#endif

using namespace LIBBASE_NS;

namespace libsc
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

#elif LIBSC_USE_MOTOR == 2
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

#elif LIBSC_USE_MOTOR == 8
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

	case 2:
		return LIBSC_MOTOR2_PWM;

	case 3:
		return LIBSC_MOTOR3_PWM;

	case 4:
		return LIBSC_MOTOR4_PWM;

	case 5:
		return LIBSC_MOTOR5_PWM;

	case 6:
		return LIBSC_MOTOR6_PWM;

	case 7:
		return LIBSC_MOTOR7_PWM;
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

	case 2:
		return LIBSC_MOTOR2_DIR;

	case 3:
		return LIBSC_MOTOR3_DIR;

	case 4:
		return LIBSC_MOTOR4_DIR;

	case 5:
		return LIBSC_MOTOR5_DIR;

	case 6:
		return LIBSC_MOTOR6_DIR;

	case 7:
		return LIBSC_MOTOR7_DIR;
	}
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

	case 2:
		return LIBSC_MOTOR2_PWM;
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

	case 2:
		return LIBSC_MOTOR2_DIR;
	}
}

#endif

DirMotor::Pwm::Config GetPwmConfig(const uint8_t id)
{
	DirMotor::Pwm::Config config;
	config.pin = GetPwmPin(id);
	config.period = PERIOD;
	config.pos_width = 0;
	config.precision = Pwm::Config::Precision::kNs;
	config.alignment = DirMotor::Pwm::Config::Alignment::kCenter;
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
		  m_pwm(GetPwmConfig(config.id)),
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
