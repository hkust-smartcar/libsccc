/*
 * servo.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstdint>

#include "libbase/log.h"
#include "libbase/helper.h"
#include "libbase/pinout_macros.h"
#include LIBBASE_H(soft_pwm)

#if PINOUT_FTM_COUNT
#include LIBBASE_H(ftm_pwm)

#elif PINOUT_TPM_COUNT
#include LIBBASE_H(tpm_pwm)

#endif // PINOUT_FTM_COUNT

#include "libsc/config.h"
#include "libsc/servo.h"
#include "libutil/misc.h"

using namespace LIBBASE_NS;

namespace libsc
{

#ifdef LIBSC_USE_SERVO

namespace
{

#if LIBSC_USE_SERVO == 1
inline Pin::Name GetPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_SERVO0;
}

#else
inline Pin::Name GetPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break

	case 0:
		return LIBSC_SERVO0;

	case 1:
		return LIBSC_SERVO1;
	}
}

#endif // LIBSC_USE_SERVO

#if LIBSC_USE_SOFT_SERVO_PWM
#if LIBSC_USE_SERVO == 1
inline uint8_t GetSoftPwmPitChannel(const uint8_t)
{
	return LIBSC_SERVO0_SOFT_PWM_PIT_CHANNEL;
}

#else
inline uint8_t GetSoftPwmPitChannel(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);

	case 0:
		return LIBSC_SERVO0_SOFT_PWM_PIT_CHANNEL;

	case 1:
		return LIBSC_SERVO1_SOFT_PWM_PIT_CHANNEL;
	}
}

#endif // LIBSC_USE_SERVO
#endif // LIBSC_USE_SOFT_SERVO_PWM

Servo::Pwm::Config GetPwmConfig(const uint8_t id, const uint16_t period,
		const uint16_t high_time)
{
	Servo::Pwm::Config config;
	config.pin = GetPin(id);
	config.period = period;
	config.pos_width = high_time;
#if LIBSC_USE_SOFT_SERVO_PWM
	config.pit_channel = GetSoftPwmPitChannel(id);
#else
	config.alignment = Servo::Pwm::Config::Alignment::kEdge;
#endif
	return config;
}

}

Servo::Servo(const Config &config)
		: m_pos_width_min(config.min_pos_width),
		  m_pos_width_diff(config.max_pos_width - config.min_pos_width),
		  m_pwm(GetPwmConfig(config.id, config.period,
				  m_pos_width_diff / 2 + m_pos_width_min)),
		  m_degree(900)
{}

void Servo::SetDegree(const uint16_t degree)
{
	const uint32_t degree_ = libutil::Clamp<Uint>(0, degree, 1800);
	if (m_degree == degree_)
	{
		return;
	}

	const uint32_t pos_width = (degree_ * m_pos_width_diff / 1800)
			+ m_pos_width_min;
	m_pwm.SetPosWidth(pos_width);
	m_degree = degree_;
}

#else
Servo::Servo(const Config&)
		: m_pos_width_min(0), m_pos_width_diff(0), m_degree(0)
{
	LOG_DL("Configured not to use Servo");
}
void Servo::SetDegree(const uint16_t) {}

#endif

}
