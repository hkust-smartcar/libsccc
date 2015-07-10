/*
 * tpm_pwm.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "libbase/kl26/pin.h"
#include "libbase/kl26/pwm.h"
#include "libbase/kl26/tpm.h"

namespace libbase
{
namespace kl26
{

class TpmPwm : public Pwm
{
public:
	struct Config : public Pwm::Config
	{
		enum struct Alignment
		{
			/**
			 * The leading edges of all PWM signals are aligned with the
			 * beginning of the period
			 */
			kEdge,
			kCenter,
		};

		Alignment alignment;
		/**
		 * Select the polarity of the output, this flag will also be used to
		 * determine the "safe" state
		 */
		bool is_active_high = true;
	};

	explicit TpmPwm(const Config &config);
	explicit TpmPwm(nullptr_t);
	TpmPwm(const TpmPwm&) = delete;
	TpmPwm(TpmPwm &&rhs);
	~TpmPwm();

	TpmPwm& operator=(const TpmPwm&) = delete;
	TpmPwm& operator=(TpmPwm &&rhs);
	operator bool() const override
	{
		return m_is_init;
	}

	/**
	 * Reset the period of the PWM, only can be done when this is the only
	 * used channel in the module
	 *
	 * @param period
	 * @param pos_width
	 */
	void SetPeriod(const uint32_t period, const uint32_t pos_width) override;
	void SetPosWidth(const uint32_t pos_width) override;

private:
	bool InitModule(const Pin::Name pin);
	void InitPin(const Pin::Name pin);
	void InitCounter(const uint16_t mod);
	void InitChannel(const Config &config, const uint16_t cv);
	void InitScReg(const Config &config, const uint8_t prescaler);
	void InitConfReg();

	void Uninit();

	void SetCv(const uint16_t cv);
	void SetPrescaler(const uint8_t prescaler);
	void SetEnableCounter(const bool flag);

	bool EnsureModuleCompatible() const;
	uint8_t GetPrescaler() const;

	uint8_t m_module;
	uint8_t m_channel;
	uint32_t m_period;
	Config::Precision m_precision;
	Config::Alignment m_alignment;

	Pin m_pin;

	bool m_is_init;
};

}
}
