/*
 * dir_motor.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/helper.h"
#include "libbase/pinout_macros.h"
#include LIBBASE_H(gpio)

#if PINOUT_FTM_COUNT
#include LIBBASE_H(ftm_pwm)

#elif PINOUT_TPM_COUNT
#include LIBBASE_H(tpm_pwm)

#endif // PINOUT_FTM_COUNT

#include "libsc/motor.h"

namespace libsc
{

class DirMotor : public Motor
{
public:
#if PINOUT_FTM_COUNT
	typedef LIBBASE_MODULE(FtmPwm) Pwm;

#elif PINOUT_TPM_COUNT
	typedef LIBBASE_MODULE(TpmPwm) Pwm;

#endif // PINOUT_FTM_COUNT

	struct Config : public Motor::Config
	{
		uint8_t id;
	};

	explicit DirMotor(const Config &config);

private:
	void OnSetPower(const uint16_t power) override;
	void OnSetClockwise(const bool flag) override;

	Pwm m_pwm;
	LIBBASE_MODULE(Gpo) m_dir;
};

}
