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

namespace
{

#if LIBSC_USE_SERVO == 1
#define GetFtmModule(x) FtmUtils::GetFtmModule<LIBSC_SERVO0>()

#else
inline FTMn_e GetFtmModule(const uint8_t id)
{
	switch (id)
	{
	case 0:
		return FtmUtils::GetFtmModule<LIBSC_SERVO0>();

	case 1:
		return FtmUtils::GetFtmModule<LIBSC_SERVO1>();
	}
}

#endif

#if LIBSC_USE_MOTOR == 1
#define GetFtmChannel(x) FtmUtils::GetFtmChannel<LIBSC_SERVO0>()

#else
inline FTM_CHn_e GetFtmChannel(const uint8_t id)
{
	switch (id)
	{
	case 0:
		return FtmUtils::GetFtmChannel<LIBSC_SERVO0>();

	case 1:
		return FtmUtils::GetFtmChannel<LIBSC_SERVO1>();
	}
}

#endif

}

#ifdef LIBSC_USE_SERVO

Servo::Servo(const uint8_t id)
		: m_id(id)
{
	m_degree = 90;
	FTM_PWM_init(GetFtmModule(id), GetFtmChannel(id), 50, 750);
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
	FTM_PWM_Duty(GetFtmModule(m_id), GetFtmChannel(m_id), duty);
	m_degree = real_degree;
}

void Servo::AddDegree(const int16_t degree)
{
	SetDegree(libutil::Clamp<int>(0, m_degree + degree, 180));
}

#else
Servo::Servo(const uint8_t) : m_id(0) {}
void Servo::SetDegree(const uint8_t) {}
void Servo::AddDegree(const int16_t) {}

#endif

}
