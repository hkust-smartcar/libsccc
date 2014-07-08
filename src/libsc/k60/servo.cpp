/*
 * servo.cpp
 * RC servo
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cassert>
#include <cstdint>

#include "libbase/k60/ftm_pwm.h"
#include "libbase/k60/misc_utils.h"

#include "libsc/com/config.h"
#include "libsc/k60/servo.h"
#include "libutil/misc.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_SERVO

namespace
{

#if LIBSC_USE_SERVO == 1
inline PinConfig::Name GetPin(const uint8_t)
{
	return LIBSC_SERVO0;
}

#else
inline PinConfig::Name GetPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);

	case 0:
		return LIBSC_SERVO0;

	case 1:
		return LIBSC_SERVO1;
	}
}

#endif

FtmPwm::Config GetFtmPwmConfig(const uint8_t id, const uint16_t period,
		const uint16_t high_time)
{
	FtmPwm::Config config;
	config.pin = GetPin(id);
	config.period = period;
	config.pos_width = high_time;
	return config;
}

}

Servo::Servo(const uint8_t id, const uint16_t period,
		const uint16_t pos_width_min, const uint16_t pos_width_max)
		: m_pwm(GetFtmPwmConfig(id, period,
				  (pos_width_max - pos_width_min) / 2 + pos_width_min)),
		  m_pos_width_min(pos_width_min),
		  m_pos_width_diff(pos_width_max - pos_width_min),
		  m_degree(900)
{}

void Servo::SetDegree(const uint16_t degree)
{
	const uint32_t real_degree = libutil::Clamp<Uint>(0, degree, 1800);
	if (m_degree == real_degree)
	{
		return;
	}

	const uint32_t pos_width = (real_degree * m_pos_width_diff / 1800)
			+ m_pos_width_min;
	m_pwm.SetPosWidth(pos_width);
	m_degree = real_degree;
}

void Servo::AddDegree(const int16_t degree)
{
	SetDegree(libutil::Clamp<int>(0, m_degree + degree, 1800));
}

#else
Servo::Servo(const uint8_t, const uint16_t, const uint16_t, const uint16_t)
		: m_pwm(nullptr), m_pos_width_min(0), m_pos_width_diff(0), m_degree(0)
{}
void Servo::SetDegree(const uint16_t) {}
void Servo::AddDegree(const int16_t) {}

#endif

}
}
