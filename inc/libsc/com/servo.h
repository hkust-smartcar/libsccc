/*
 * servo.h
 * Servo abstraction
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_SERVO_H_
#define LIBSC_SERVO_H_

#include <cstdint>

namespace libsc
{

class Servo
{
public:
	/**
	 * Set the degree(the servo turn in CCW), [0, 180]
	 *
	 * @param degree
	 */
	void SetDegree(const uint8_t degree);
	void AddDegree(const int16_t degree);
	uint8_t GetDegree() const
	{
		return m_degree;
	}

protected:
	Servo(const uint8_t id, const uint16_t pwm_min, const uint16_t pwm_max);

private:
	const uint8_t m_id;
	const uint16_t m_pwm_min, m_pwm_max;
	uint8_t m_degree;
};

}

#endif /* LIBSC_SERVO_H_ */
