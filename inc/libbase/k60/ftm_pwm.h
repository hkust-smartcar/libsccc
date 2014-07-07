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

	void SetPeriodUs(const uint32_t period_us, const uint32_t high_time_us) override;
	void SetHighTimeUs(const uint32_t high_time_us) override;

private:
	bool InitModule(const PinConfig::Name pin);

	uint8_t m_module;
	uint8_t m_channel;

	uint32_t m_period_us;
};

}
}

#endif /* LIBBASE_K60_FTM_PWM_H_ */
