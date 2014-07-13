/*
 * sys_tick.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstdint>

#include "libbase/k60/clock_utils.h"
#include "libbase/k60/sys_tick.h"

#define SysTickPtr ((SysTick_Type*)SysTick_BASE)
#define SYST_CSR (SysTickPtr->CTRL)
#define SYST_RVR (SysTickPtr->LOAD)
#define SYST_CVR (SysTickPtr->VAL)
#define SYST_CALIB (SysTickPtr->CALIB)

#define SysTick_CSR_COUNTFLAG_SHIFT SysTick_CTRL_COUNTFLAG_Pos
#define SysTick_CSR_CLKSOURCE_MASK SysTick_CTRL_CLKSOURCE_Msk
#define SysTick_CSR_ENABLE_MASK SysTick_CTRL_ENABLE_Msk
#define SysTick_CSR_ENABLE_SHIFT SysTick_CTRL_ENABLE_Pos

namespace libbase
{
namespace k60
{

void SysTick::DelayUs(const uint16_t us)
{
	const uint32_t count = ClockUtils::GetCoreTickPerUs(us) - 1;
	SYST_RVR = count & 0x00FFFFFFu;
	SYST_CVR = 0;
	SYST_CSR = 0 | SysTick_CSR_CLKSOURCE_MASK | SysTick_CSR_ENABLE_MASK;
	while (!GET_BIT(SYST_CSR, SysTick_CSR_COUNTFLAG_SHIFT))
	{}

	if (count > 0x00FFFFFFu)
	{
		SYST_RVR = count >> 24;
		SYST_CVR = 0;
		while (!GET_BIT(SYST_CSR, SysTick_CSR_COUNTFLAG_SHIFT))
		{}
	}

	CLEAR_BIT(SYST_CSR, SysTick_CSR_ENABLE_SHIFT);
}

}
}
