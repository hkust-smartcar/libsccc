/*
 * dir_motor.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/k60/ftm_pwm.h"
#include "libbase/k60/gpio.h"

#include "libsc/k60/motor.h"

namespace libsc
{
namespace k60
{

class DirMotor : public Motor
{
public:
	struct Config : public Motor::Config
	{
		uint8_t id;
	};

	DirMotor(const Config &config);

	void SetPower(const uint16_t power) override;
	void AddPower(const int16_t power) override;
	uint16_t GetPower() const override
	{
		return m_power;
	}

	void SetClockwise(const bool flag) override;
	bool IsClockwise() const override
	{
		return m_is_clockwise;
	}

private:
	libbase::k60::FtmPwm m_pwm;
	libbase::k60::Gpo m_dir;

	uint16_t m_power;
	bool m_is_clockwise;
};

}
}
