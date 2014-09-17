/*
 * sys_tick_delay.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <cstdint>

#include "libbase/k60/sys_tick.h"

#include "libsc/k60/delay.h"

namespace libsc
{
namespace k60
{

class SysTickDelay : public Delay
{
public:
	SysTickDelay();

	void DelayUs(const uint16_t us) override;

private:
	inline void DelayCount(const uint32_t count);

	libbase::k60::SysTick m_sys_tick;
};

}
}
