/*
 * servo.h
 * RC servo
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_K60_SERVO_H_
#define LIBSC_K60_SERVO_H_

#include <cstdint>

#include "libbase/k60/ftm_pwm.h"

namespace libsc
{
namespace k60
{

class Servo
{
public:
	/**
	 * Set the degree(the servo turn in CCW), [0, 180] * 10 (i.e., 84 => 8.4°)
	 *
	 * @param degree
	 */
	void SetDegree(const uint16_t degree);
	void AddDegree(const int16_t degree);
	uint16_t GetDegree() const
	{
		return m_degree;
	}

protected:
	/**
	 * Construct a RC Servo
	 *
	 * @param id
	 * @param period The period of the PWM signal, in us
	 * @param pos_width_min The min pos width of the PWM signal, in us
	 * @param pos_width_max The max pos width of the PWM signal, in us
	 */
	Servo(const uint8_t id, const uint16_t period, const uint16_t pos_width_min,
			const uint16_t pos_width_max);

private:
	libbase::k60::FtmPwm m_pwm;

	const uint16_t m_pos_width_min;
	const uint16_t m_pos_width_diff;
	uint16_t m_degree;
};

}
}

#endif /* LIBSC_K60_SERVO_H_ */
