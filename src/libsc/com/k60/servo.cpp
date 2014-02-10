/*
 * servo.cpp
 * Servo abstraction (for Futaba S3010)
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>
#include <hw_common.h>
#include <cstdint>

#include <MK60_FTM.h>

#include "ftm_utils.h"
#include "libutil/misc.h"
#include "libsc/com/config.h"
#include "libsc/com/servo.h"

#define PWM_MAX 1000
#define PWM_MIN 500

namespace libsc
{

#ifdef LIBSC_USE_SERVO

Servo::Servo(const uint8_t id)
		: m_id(id)
{
	m_degree = 90;
	FTM_PWM_init(FtmUtils::GetFtmModule<LIBSC_SERVO0>(),
			FtmUtils::GetFtmChannel<LIBSC_SERVO0>(), 50, 750);
}

void Servo::SetDegree(const uint8_t degree)
{
	const uint8_t real_degree = libutil::Clamp<uint8_t>(0, degree, 180);
	if (m_degree == real_degree)
	{
		return;
	}

	const uint32_t duty = static_cast<uint32_t>((real_degree / 180.0f)
			* (PWM_MAX - PWM_MIN)) + PWM_MIN;
	FTM_PWM_Duty(FtmUtils::GetFtmModule<LIBSC_SERVO0>(),
			FtmUtils::GetFtmChannel<LIBSC_SERVO0>(), duty);
	m_degree = real_degree;
}

void Servo::AddDegree(const int16_t degree)
{
	SetDegree(libutil::Clamp<int>(0, m_degree + degree, 180));
}

#else
Servo::Servo(const uint8_t id) {}
void Servo::SetDegree(const uint8_t degree) {}
void Servo::AddDegree(const int16_t degree) {}

#endif

}
