/*
 * pit_pwm.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBBASE_K60_PIT_PWM_H_
#define LIBBASE_K60_PIT_PWM_H_

#include <cstdint>

#include "libbase/k60/gpio.h"
#include "libbase/k60/pit.h"
#include "libbase/k60/pwm.h"

namespace libbase
{
namespace k60
{

class PitPwm : public Pwm
{
public:
	struct Config : public Pwm::Config
	{
		// [0 - 3]
		uint8_t pit_channel;
	};

	explicit PitPwm(const Config &config);

	void SetPeriodUs(const uint32_t period_us, const uint32_t high_time_us) override;
	void SetHighTimeUs(const uint32_t high_time_us) override;

private:
	void Setup(const uint32_t period_us, const uint32_t high_time_us);

	void OnTick(Pit*);

	Pit m_pit;
	Gpo m_pin;

	volatile bool m_flag;
	uint32_t m_high_time;
	uint32_t m_high_count;
	uint32_t m_low_time;
	uint32_t m_low_count;
};

}
}

#endif /* LIBBASE_K60_PIT_PWM_H_ */
