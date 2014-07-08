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

/**
 * Generate PWM signal using PIT and GPIO, only guarantee precision down to us
 * level
 */
class PitPwm : public Pwm
{
public:
	struct Config : public Pwm::Config
	{
		// [0 - 3]
		uint8_t pit_channel;
	};

	explicit PitPwm(const Config &config);

	void SetPeriod(const uint32_t period, const uint32_t pos_width) override;
	void SetPosWidth(const uint32_t pos_width) override;

private:
	void Setup(const uint32_t period, const uint32_t pos_width);

	void OnTick(Pit*);

	Pit m_pit;
	Gpo m_pin;

	volatile bool m_flag;
	Pwm::Config::Precision m_precision;
	uint32_t m_pos_width;
	uint32_t m_pos_count;
	uint32_t m_neg_width;
	uint32_t m_neg_count;
};

}
}

#endif /* LIBBASE_K60_PIT_PWM_H_ */
