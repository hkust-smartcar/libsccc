/*
 * servo.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/helper.h"
#include "libbase/pinout_macros.h"
#include LIBBASE_H(soft_pwm)

#if PINOUT_FTM_COUNT
#include LIBBASE_H(ftm_pwm)

#elif PINOUT_TPM_COUNT
#include LIBBASE_H(tpm_pwm)

#else
#define LIBSC_USE_SOFT_SERVO_PWM 1

#endif // PINOUT_FTM_COUNT

#include "libsc/config.h"

namespace libsc
{

/**
 * RC servo
 */
class Servo
{
public:
#if LIBSC_USE_SOFT_SERVO_PWM
	typedef LIBBASE_MODULE(SoftPwm) Pwm;

#elif PINOUT_FTM_COUNT
	typedef LIBBASE_MODULE(FtmPwm) Pwm;

#elif PINOUT_TPM_COUNT
	typedef LIBBASE_MODULE(TpmPwm) Pwm;

#endif // LIBSC_USE_SOFT_SERVO_PWM

	struct Config
	{
		uint8_t id;
		/// The period of the PWM signal, in us
		uint16_t period;
		/// The min pos width of the PWM signal, in us
		uint16_t min_pos_width;
		/// The max pos width of the PWM signal, in us
		uint16_t max_pos_width;
	};

	explicit Servo(const Config &config);

	/**
	 * Set the degree(the servo turn in CCW), [0, 180] * 10 (i.e., 84 => 8.4°)
	 *
	 * @param degree
	 */
	void SetDegree(const uint16_t degree);
	uint16_t GetDegree() const
	{
		return m_degree;
	}

private:
	const uint16_t m_pos_width_min;
	const uint16_t m_pos_width_diff;

	Pwm m_pwm;

	uint16_t m_degree;
};

}
