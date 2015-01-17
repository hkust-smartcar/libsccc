/*
 * alternate_motor.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/k60/ftm_pwm.h"

#include "libsc/k60/motor.h"

namespace libsc
{
namespace k60
{

class AlternateMotor : public Motor
{
public:
	struct Config : public Motor::Config
	{
		uint8_t id;
	};

	explicit AlternateMotor(const Config &config);

private:
	void OnSetPower(const uint16_t power) override;
	void OnSetClockwise(const bool flag) override;

	libbase::k60::FtmPwm m_pwms[2];
	libbase::k60::FtmPwm *m_active_pwm;
	uint32_t m_pos_width;
};

}
}
