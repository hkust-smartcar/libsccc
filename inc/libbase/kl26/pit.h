/*
 * pit.h
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

class Pit
{
public:
	typedef std::function<void(Pit *pit)> OnPitTriggerListener;

	struct Config
	{
		// [0 - 3]
		uint8_t channel;
		uint32_t count;
		OnPitTriggerListener isr;
		bool is_enable = true;
	};

	explicit Pit(const Config &config);
	explicit Pit(nullptr_t);
	Pit(const Pit&) = delete;
	Pit(Pit &&rhs);
	~Pit();

	Pit& operator=(const Pit&) = delete;
	Pit& operator=(Pit &&rhs);
	operator bool() const
	{
		return m_is_init;
	}

	void SetEnable(const bool flag);

	void SetCount(const uint32_t count);
	uint32_t GetCountLeft() const;

	/**
	 * Consume an interrupt, this basically clear the IRQ flag for this
	 * particular PIT channel. Notice that you don't need to call it in a
	 * OnPitTriggerListener as it has been called for you
	 */
	void ConsumeInterrupt();
	bool IsInterruptRequested() const;

	uint8_t GetChannel() const
	{
		return m_channel;
	}

private:
	void SetIsr(const OnPitTriggerListener &isr);
	void Uninit();

	static __ISR void IrqHandler();

	uint8_t m_channel;
	OnPitTriggerListener m_isr;

	bool m_is_init;
};

}
}
