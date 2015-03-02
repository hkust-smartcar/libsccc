/*
 * sys_tick_delay.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/helper.h"
#include LIBBASE_H(sys_tick)

#include "libsc/delay.h"

namespace libsc
{

class SysTickDelay : public Delay
{
public:
	SysTickDelay();

	void DelayUs(const uint16_t us) override;

private:
	inline void DelayCount(const uint32_t count);

	LIBBASE_MODULE(SysTick) m_sys_tick;
};

}
