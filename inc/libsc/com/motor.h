/*
 * motor.h
 * Motor abstraction
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_MOTOR_H_
#define LIBSC_MOTOR_H_

#include <cstdint>

namespace libsc
{

class Motor
{
public:
	explicit Motor(const uint8_t id);
	Motor(const uint8_t id, const float multiplier);

	/**
	 * Set the PWM duty rate, [0, 10000]
	 *
	 * @param power
	 */
	void SetPower(const uint16_t power);
	void AddPower(const int16_t power);
	uint16_t GetPower() const
	{
		return m_power;
	}

	void SetClockwise(const bool flag);
	bool IsClockwise() const
	{
		return m_is_clockwise;
	}

private:
	const uint8_t m_id;
	uint16_t m_power; // [0, 10000]
	bool m_is_clockwise;
	float m_multiplier;
};

}

#endif /* LIBSC_MOTOR_H_ */
