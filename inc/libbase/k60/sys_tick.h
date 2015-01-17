/*
 * sys_tick.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include <functional>

#include "libbase/k60/misc_utils.h"

#ifdef SysTick
#undef SysTick
#endif

namespace libbase
{
namespace k60
{

class SysTick
{
public:
	typedef std::function<void(SysTick *sys_tick)> OnSysTickTriggerListener;

	struct Config
	{
		uint32_t count;
		OnSysTickTriggerListener isr;
		bool is_enable = true;
	};

	explicit SysTick(const Config &config);
	explicit SysTick(nullptr_t);
	SysTick(const SysTick&) = delete;
	SysTick(SysTick &&rhs);
	~SysTick();

	SysTick& operator=(const SysTick&) = delete;
	SysTick& operator=(SysTick &&rhs);
	operator bool() const
	{
		return m_is_init;
	}

	void SetEnable(const bool flag);

	void SetCount(const uint32_t count);
	uint32_t GetCountLeft() const;

	void ConsumeInterrupt();
	bool IsInterruptRequested() const;

private:
	void InitCsrReg(const Config &config);
	void SetIsr(const OnSysTickTriggerListener &isr);
	void Uninit();

	static __ISR void IrqHandler();

	OnSysTickTriggerListener m_isr;

	bool m_is_init;
};

}
}
