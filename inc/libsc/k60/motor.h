/*
 * motor.h
 * DC Motor
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_K60_MOTOR_H_
#define LIBSC_K60_MOTOR_H_

#include <cstdint>

#include "libbase/k60/ftm_pwm.h"
#include "libbase/k60/gpio.h"

namespace libsc
{
namespace k60
{

class Motor
{
public:
	explicit Motor(const uint8_t id);
	/**
	 * Construct a DC motor and set a multiplier to compensate the power output
	 *
	 * @param id
	 * @param multiplier [0, 200], in percentage
	 */
	Motor(const uint8_t id, const uint8_t multiplier);

	/**
	 * Set the motor power percentage, [0, 100] * 10 (i.e., 84 => 8.4%)
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
	libbase::k60::FtmPwm m_pwm;
	libbase::k60::Gpo m_dir;

	const uint8_t m_multiplier;
	uint16_t m_power;
	bool m_is_clockwise;
};

}
}

#endif /* LIBSC_K60_MOTOR_H_ */
