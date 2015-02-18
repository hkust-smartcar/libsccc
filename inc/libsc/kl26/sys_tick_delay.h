/*
 * sys_tick_delay.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/kl26/sys_tick.h"

#include "libsc/kl26/delay.h"

namespace libsc
{
namespace kl26
{

class SysTickDelay : public Delay
{
public:
	SysTickDelay();

	void DelayUs(const uint16_t us) override;

private:
	inline void DelayCount(const uint32_t count);

	libbase::kl26::SysTick m_sys_tick;
};

}
}
