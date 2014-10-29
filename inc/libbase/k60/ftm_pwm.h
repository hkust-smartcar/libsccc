/*
 * ftm_pwm.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "libbase/k60/pin.h"
#include "libbase/k60/pwm.h"

namespace libbase
{
namespace k60
{

class FtmPwm : public Pwm
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

		/**
		 * Deadtime insertion, notice that this flag is shared by two
		 * neightboring channels (n & n+1)
		 */
		bool is_insert_deadtime = false;
		/**
		 * Length of the deadtime, notice that this value is shared among ALL
		 * channels inside the same FTM module. If @a is_insert_deadtime is true,
		 * this value must be the same as other channels in the same FTM module.
		 * If @a is_insert_deadtime is otherwise, this value will be ignored
		 */
		uint32_t deadtime_ns;
	};

	explicit FtmPwm(const Config &config);
	explicit FtmPwm(nullptr_t);
	FtmPwm(const FtmPwm&) = delete;
	FtmPwm(FtmPwm &&rhs);
	~FtmPwm();

	FtmPwm& operator=(const FtmPwm&) = delete;
	FtmPwm& operator=(FtmPwm &&rhs);
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
	void InitChannelPolarity(const Config &config);
	void InitCounter(const uint16_t mod);
	void InitChannel(const Config &config, const uint16_t cv);
	void InitScReg(const Config &config, const uint8_t prescaler);
	void InitConfReg(const Config &config);
	void InitDeadtime(const Config &config);

	void Uninit();

	void SetReadOnlyReg(const bool flag);
	void SetCv(const uint16_t cv);
	void SetPrescaler(const uint8_t prescaler);

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
