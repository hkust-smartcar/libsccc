/*
 * ftm_pwm.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBBASE_K60_FTM_PWM_H_
#define LIBBASE_K60_FTM_PWM_H_

#include <cstdint>

#include "libbase/k60/pin.h"
#include "libbase/k60/pwm.h"

namespace libbase
{
namespace k60
{

// TODO Native FTM support
class FtmPwm : public Pwm
{
public:
	struct Config : public Pwm::Config
	{};

	explicit FtmPwm(const Config &config);

	void SetPeriod(const uint32_t period, const uint32_t pos_width) override;
	void SetPosWidth(const uint32_t pos_width) override;

private:
	bool InitModule(const PinConfig::Name pin);

	uint8_t m_module;
	uint8_t m_channel;

	Pwm::Config::Precision m_precision;
	uint32_t m_period;
};

}
}

#endif /* LIBBASE_K60_FTM_PWM_H_ */
