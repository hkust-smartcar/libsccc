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

	explicit DirMotor(const Config &config);

private:
	void OnSetPower(const uint16_t power) override;
	void OnSetClockwise(const bool flag) override;

	libbase::k60::FtmPwm m_pwm;
	libbase::k60::Gpo m_dir;
};

}
}
