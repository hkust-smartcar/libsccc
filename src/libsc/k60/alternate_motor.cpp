/*
 * alternate_motor.cpp
 *
 * Author: Ming Tsang
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
#include "libsc/k60/alternate_motor.h"

// 10 kHz
#define PERIOD 100000

// Default PWM A -> CW
#ifndef LIBSC_ALTERNATE_MOTOR_CW_PWM
	#define LIBSC_ALTERNATE_MOTOR_CW_PWM 0
#endif

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

#if defined(LIBSC_USE_MOTOR) && defined(LIBSC_MOTOR0_PWMA) \
		&& defined(LIBSC_MOTOR0_PWMB)

namespace
{

#if LIBSC_USE_MOTOR == 1
inline Pin::Name GetPwmAPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_MOTOR0_PWMA;
}

inline Pin::Name GetPwmBPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_MOTOR0_PWMB;
}

inline uint32_t GetDeadtime(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_MOTOR0_DEADTIME;
}

#else
inline Pin::Name GetPwmAPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);

	case 0:
		return LIBSC_MOTOR0_PWMA;

	case 1:
		return LIBSC_MOTOR1_PWMA;
	}
}

inline Pin::Name GetPwmBPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);

	case 0:
		return LIBSC_MOTOR0_PWMB;

	case 1:
		return LIBSC_MOTOR1_PWMB;
	}
}

inline uint32_t GetDeadtime(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);

	case 0:
		return LIBSC_MOTOR0_DEADTIME;

	case 1:
		return LIBSC_MOTOR1_DEADTIME;
	}
}

#endif

FtmPwm::Config GetFtmPwmAConfig(const uint8_t id)
{
	FtmPwm::Config config;
	config.pin = GetPwmAPin(id);
	config.period = PERIOD;
	config.pos_width = 0;
	config.precision = Pwm::Config::Precision::kNs;
	config.alignment = FtmPwm::Config::Alignment::kCenter;
	config.is_insert_deadtime = true;
	config.deadtime_ns = GetDeadtime(id);
	return config;
}

FtmPwm::Config GetFtmPwmBConfig(const uint8_t id)
{
	FtmPwm::Config config;
	config.pin = GetPwmBPin(id);
	config.period = PERIOD;
	config.pos_width = 0;
	config.precision = Pwm::Config::Precision::kNs;
	config.alignment = FtmPwm::Config::Alignment::kCenter;
	config.is_insert_deadtime = true;
	config.deadtime_ns = GetDeadtime(id);
	return config;
}

}

AlternateMotor::AlternateMotor(const Config &config)
		: Motor(config),
		  m_pwms{FtmPwm(GetFtmPwmAConfig(config.id)),
				  FtmPwm(GetFtmPwmBConfig(config.id))},
		  m_active_pwm(&m_pwms[LIBSC_ALTERNATE_MOTOR_CW_PWM]),
		  m_pos_width(0)
{}

void AlternateMotor::OnSetPower(const uint16_t power)
{
	const uint32_t pos_width = PwmUtils::GetPosWidth(PERIOD, power);
	m_active_pwm->SetPosWidth(pos_width);
	m_pos_width = pos_width;
}

void AlternateMotor::OnSetClockwise(const bool flag)
{
	if (flag && m_active_pwm != &m_pwms[LIBSC_ALTERNATE_MOTOR_CW_PWM])
	{
		m_pwms[!LIBSC_ALTERNATE_MOTOR_CW_PWM].SetPosWidth(0);
		m_pwms[LIBSC_ALTERNATE_MOTOR_CW_PWM].SetPosWidth(m_pos_width);
		m_active_pwm = &m_pwms[LIBSC_ALTERNATE_MOTOR_CW_PWM];
	}
	else if (!flag && m_active_pwm != &m_pwms[!LIBSC_ALTERNATE_MOTOR_CW_PWM])
	{
		m_pwms[LIBSC_ALTERNATE_MOTOR_CW_PWM].SetPosWidth(0);
		m_pwms[!LIBSC_ALTERNATE_MOTOR_CW_PWM].SetPosWidth(m_pos_width);
		m_active_pwm = &m_pwms[!LIBSC_ALTERNATE_MOTOR_CW_PWM];
	}
}

#else
AlternateMotor::AlternateMotor(const Config &config)
		: Motor(config),
		  m_pwms({FtmPwm(nullptr), FtmPwm(nullptr)}),
		  m_active_pwm(nullptr),
		  m_pos_width(0)
{
	LOG_DL("Configured not to use AlternateMotor");
}
void AlternateMotor::OnSetPower(const uint16_t) {}
void AlternateMotor::OnSetClockwise(const bool) {}

#endif

}
}
