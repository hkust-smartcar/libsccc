/*
 * soft_pwm.h
 * Software emulated PWM using PIT and GPIO
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
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
class SoftPwm : public Pwm
{
public:
	struct Config : public Pwm::Config
	{
		// [0 - 3]
		uint8_t pit_channel;
	};

	explicit SoftPwm(const Config &config);
	explicit SoftPwm(nullptr_t);
	SoftPwm(const SoftPwm&) = delete;
	SoftPwm(SoftPwm &&rhs);
	~SoftPwm();

	SoftPwm& operator=(const SoftPwm&) = delete;
	SoftPwm& operator=(SoftPwm &&rhs);
	operator bool() const override
	{
		return m_is_init;
	}

	void SetPeriod(const uint32_t period, const uint32_t pos_width) override;
	void SetPosWidth(const uint32_t pos_width) override;

private:
	void Uninit();

	void Setup(const uint32_t period, const uint32_t pos_width);

	void OnTick(Pit*);

	Pwm::Config::Precision m_precision;

	Pit m_pit;
	Gpo m_pin;

	volatile bool m_flag;
	uint32_t m_pos_width;
	uint32_t m_pos_count;
	uint32_t m_neg_width;
	uint32_t m_neg_count;

	bool m_is_init;
};

}
}
