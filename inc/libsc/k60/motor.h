/*
 * motor.h
 * DC Motor
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

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
	/**
	 * Construct a DC motor
	 *
	 * @param id
	 * @param is_invert Say you've installed the motor the other way round,
	 * you'll need to invert it then
	 * @param multiplier A multiplier in percentage to compensate the power
	 * output, [0, 200]
	 * @see SetClockwise()
	 */
	Motor(const uint8_t id, const bool is_invert, const uint8_t multiplier);
	Motor(const uint8_t id, const bool is_invert)
			: Motor(id, is_invert, 100)
	{}

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

	/**
	 * Clockwise (top view):
	 * |-------|   ^
	 * | Motor |-- ^
	 * |-------|   ^
	 *
	 * @param flag
	 */
	void SetClockwise(const bool flag);
	bool IsClockwise() const
	{
		return m_is_clockwise;
	}

private:
	const bool m_is_clockwise_high;
	const uint8_t m_multiplier;

	libbase::k60::FtmPwm m_pwm;
	libbase::k60::Gpo m_dir;

	uint16_t m_power;
	bool m_is_clockwise;
};

}
}
