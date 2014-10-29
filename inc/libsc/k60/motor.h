/*
 * motor.h
 * DC Motor
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

namespace libsc
{
namespace k60
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
	virtual void SetPower(const uint16_t power) = 0;
	virtual void AddPower(const int16_t power) = 0;
	virtual uint16_t GetPower() const = 0;

	/**
	 * Clockwise (top view):
	 * |-------|   ^
	 * | Motor |-- ^
	 * |-------|   ^
	 *
	 * @param flag
	 */
	virtual void SetClockwise(const bool flag) = 0;
	virtual bool IsClockwise() const = 0;

protected:
	Motor(const Config &config)
			: m_multiplier(config.multiplier)
	{}
	~Motor()
	{}

	uint8_t GetMultiplier() const
	{
		return m_multiplier;
	}

private:
	const uint8_t m_multiplier;
};

}
}
