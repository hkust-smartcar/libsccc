/*
 * dma_mux.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstdint>

#include "libbase/k60/dma_mux.h"
#include "libbase/k60/dma_mux_utils.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pinout.h"
#include "libbase/k60/sim.h"

#include "libutil/misc.h"

constexpr DMAMUX_Type* MEM_MAPS[PINOUT::GetDmaMuxCount()] =
{
#if MK60DZ10 || MK60DZ10
	DMAMUX
#elif MK60F15
	DMAMUX0,
	DMAMUX1,
#else
#error Unknwon MCU
#endif
};

namespace libbase
{
namespace k60
{

bool DmaMux::SetEnableSource(const Source src, const Uint channel)
{
	if (DmaMuxUtils::GetModule(channel) >= PINOUT::GetDmaMuxCount())
	{
		assert(false);
		return false;
	}

	uint8_t reg = 0;
	if (src != Source::kNull)
	{
		SET_BIT(reg, DMAMUX_CHCFG_ENBL_SHIFT);

		// Each MUX handles 16 DMA channel
		const uint8_t src_num = PINOUT::GetDmaMuxSource(src, channel / 16);
		if (src_num == static_cast<uint8_t>(-1))
		{
			assert(false);
			return false;
		}
		reg |= DMAMUX_CHCFG_SOURCE(src_num);
	}

#if MK60DZ10 || MK60DZ10
	Sim::SetEnableClockGate(Sim::ClockGate::kDmaMux, true);
#elif MK60F15
	Sim::SetEnableClockGate(libutil::EnumAdvance(Sim::ClockGate::kDmaMux0,
			DmaMuxUtils::GetModule(channel)), true);
#else
#error Unknwon MCU
#endif

	MEM_MAPS[DmaMuxUtils::GetModule(channel)]
			->CHCFG[DmaMuxUtils::GetChannel(channel)] = reg;
	return true;
}

}
}
