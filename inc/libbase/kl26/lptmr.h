/*
 * lptmr.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include <functional>

#include "libbase/kl26/misc_utils.h"

namespace libbase
{
namespace kl26
{

/***
 * Low power timer clocked by the 1kHz LPO
 */
class Lptmr
{
public:
	typedef std::function<void(Lptmr *lptmr)> OnLptmrTriggerListener;

	struct Config
	{
		uint16_t count;
		OnLptmrTriggerListener isr;
		bool is_reset_counter_on_irq = false;
		bool is_enable = true;
	};

	explicit Lptmr(const Config &config);
	explicit Lptmr(nullptr_t);
	Lptmr(const Lptmr&) = delete;
	Lptmr(Lptmr &&rhs);
	~Lptmr();

	Lptmr& operator=(const Lptmr&) = delete;
	Lptmr& operator=(Lptmr &&rhs);
	operator bool() const
	{
		return m_is_init;
	}

	void SetEnable(const bool flag);
	/**
	 * Alter the count value, could only be done when the module is disabled or
	 * inside ISR
	 *
	 * @param count
	 */
	void SetCount(const uint16_t count);
	/**
	 * Get the current value of the counter
	 *
	 * @return
	 */
	uint16_t GetCounter() const;

	void ConsumeInterrupt();
	bool IsInterruptRequested() const;

private:
	void InitCsrReg(const Config &config);
	void InitPsrReg();
	void SetIsr(const OnLptmrTriggerListener &isr);
	void Uninit();

	static __ISR void IrqHandler();

	OnLptmrTriggerListener m_isr;

	bool m_is_init;
};

}
}
