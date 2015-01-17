/*
 * dwt.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstdint>

#include "libbase/k60/clock_utils.h"
#include "libbase/k60/dwt.h"
#include "libbase/k60/misc_utils.h"

namespace libbase
{
namespace k60
{

void Dwt::DelayUs(const uint16_t us)
{
	if (GET_BIT(DWT->CTRL, DWT_CTRL_NOCYCCNT_Pos))
	{
		// CYCCNT not implemented
		assert(false);
		return;
	}

	SET_BIT(CoreDebug->DEMCR, CoreDebug_DEMCR_TRCENA_Pos);
	CLEAR_BIT(DWT->CTRL, DWT_CTRL_CYCCNTENA_Pos);
	const uint32_t count = ClockUtils::GetCoreTickPerUs(us);
	DWT->CYCCNT = 0;
	SET_BIT(DWT->CTRL, DWT_CTRL_CYCCNTENA_Pos);
	uint32_t store = 0;
	uint32_t curr = 0;
	while (store + curr < count)
	{
		if (curr > DWT->CYCCNT)
		{
			store += curr;
		}
		curr = DWT->CYCCNT;
	}
}

}
}
