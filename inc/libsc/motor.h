/*
 * motor.h
 *
 * Author: Harrison Ng, Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>
#define MOTORPERIOD 100000
#define RESOLUTION 1000

namespace libsc
{

class Motor
{
public:
	struct Config
	{
		/**
		 * A multiplier in percentage to compensate the power output, [0, 200]
		 */
		uint8_t multiplier = 100;
	};

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
	 * <pre>
	 * |-------|   ^
	 * | Motor |-- ^
	 * |-------|   ^
	 * </pre>
	 *
	 * @param flag
	 */
	void SetClockwise(const bool flag);
	bool IsClockwise() const
	{
		return m_is_clockwise;
	}

protected:
	/**
	 * Construct a Motor, deriving class must init in a way that the motor is
	 * stalled and clockwise
	 *
	 * @param config
	 */
	explicit Motor(const Config &config);
	~Motor()
	{}

	uint8_t GetMultiplier() const
	{
		return m_multiplier;
	}

private:
	virtual void OnSetPower(const uint16_t power) = 0;
	virtual void OnSetClockwise(const bool flag) = 0;

	const uint8_t m_multiplier;

	uint16_t m_power;
	bool m_is_clockwise;
};

}
