/*
 * servo.h
 * Servo abstraction (for Futaba S3010)
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
	explicit Servo(const uint8_t id);

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

private:
	const uint8_t m_id;
	uint8_t m_degree;
};

}

#endif /* LIBSC_SERVO_H_ */
