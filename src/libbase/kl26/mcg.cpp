/*
 * mcg.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
/*
 * The FreeRTOS source code is licensed by a *modified* GNU General Public
 * License (GPL).  The modification is provided in the form of an exception.
 *
 * NOTE: The modification to the GPL is included to allow you to distribute a
 * combined work that includes FreeRTOS without being obliged to provide the source
 * code for proprietary components outside of the FreeRTOS kernel.
 *
 *
 *
 * ----------------------------------------------------------------------------
 *
 * The FreeRTOS GPL Exception Text:
 *
 * Any FreeRTOS source code, whether modified or in it's original release form,
 * or whether in whole or in part, can only be distributed by you under the terms
 * of the GNU General Public License plus this exception. An independent module is
 * a module which is not derived from or based on FreeRTOS.
 *
 * Clause 1:
 *
 * Linking FreeRTOS statically or dynamically with other modules is making a
 * combined work based on FreeRTOS. Thus, the terms and conditions of the GNU
 * General Public License cover the whole combination.
 *
 * As a special exception, the copyright holder of FreeRTOS gives you permission
 * to link FreeRTOS with independent modules that communicate with FreeRTOS
 * solely through the FreeRTOS API interface, regardless of the license terms of
 * these independent modules, and to copy and distribute the resulting combined
 * work under terms of your choice, provided that
 *
 *   + Every copy of the combined work is accompanied by a written statement that
 *   details to the recipient the version of FreeRTOS used and an offer by yourself
 *   to provide the FreeRTOS source code (including any modifications you may have
 *   made) should the recipient request it.
 *
 *   + The combined work is not itself an RTOS, scheduler, kernel or related product.
 *
 *   + The independent modules add significant and primary functionality to FreeRTOS
 *   and do not merely extend the existing functionality already present in FreeRTOS.
 *
 * Clause 2:
 *
 * FreeRTOS may not be used for any competitive or comparative purpose, including the
 * publication of any form of run time or compile time metric, without the express
 * permission of Real Time Engineers Ltd. (this is the norm within the industry and
 * is intended to ensure information accuracy).
 */


#include "libbase/kl26/hardware.h"

#include <cassert>
#include <cstdint>

#include <algorithm>

#include "libbase/kl26/mcg.h"
#include "libbase/kl26/mcg_c.h"
#include "libbase/kl26/misc_utils.h"
#include "libbase/kl26/cmsis/system_MKL26Z4.h"

#define FLL_MAX_FREQ 39062
#define FLL_MIN_FREQ 31250

#if Mkl26DZ10
	#define _MCG_C2_RANGE0 MCG_C2_RANGE
	#define _MCG_C5_PRDIV0 MCG_C5_PRDIV
	#define _MCG_C6_VDIV0 MCG_C6_VDIV
	#define _MCG_S_LOCK0_SHIFT MCG_S_LOCK_SHIFT
#else
	#define _MCG_C2_RANGE0 MCG_C2_RANGE0
	#define _MCG_C5_PRDIV0 MCG_C5_PRDIV0
	#define _MCG_C6_VDIV0 MCG_C6_VDIV0
	#define _MCG_S_LOCK0_SHIFT MCG_S_LOCK0_SHIFT
#endif

namespace libbase
{
namespace kl26
{

namespace
{


 /** This routine must be placed in RAM. It is a workaround for errata e2448.
 * Flash prefetch must be disabled when the flash clock divider is changed.
 * This cannot be performed while executing out of flash.
 * There must be a short delay after the clock dividers are changed before prefetch
 * can be re-enabled*/

__RAMFUNC void SetSysDividers(const uint32_t outdiv1, const uint32_t outdiv2,
		const uint32_t outdiv3, const uint32_t outdiv4)
{
	/*uint32_t temp_reg;
	uint8_t i;

	temp_reg = FMC->PFAPR; // store present value of FMC_PFAPR

	// set M0PFD through M7PFD to 1 to disable prefetch
	FMC->PFAPR |= FMC_PFAPR_M7PFD_MASK | FMC_PFAPR_M6PFD_MASK | FMC_PFAPR_M5PFD_MASK
			| FMC_PFAPR_M4PFD_MASK | FMC_PFAPR_M3PFD_MASK | FMC_PFAPR_M2PFD_MASK
			| FMC_PFAPR_M1PFD_MASK | FMC_PFAPR_M0PFD_MASK;

	// set clock dividers to desired value
	SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(outdiv1) | SIM_CLKDIV1_OUTDIV2(outdiv2)
				| SIM_CLKDIV1_OUTDIV3(outdiv3) | SIM_CLKDIV1_OUTDIV4(outdiv4);

	// wait for dividers to change
	for (i = 0 ; i < outdiv4 ; i++)
	{}

	FMC->PFAPR = temp_reg; // re-store original value of FMC_PFAPR

	return;*/
} // SetSysDividers

class PllDividerCalc
{
public:
	void Calc(const uint32_t external_osc_khz, const uint32_t core_clock_khz);

	Uint GetPrdiv() const
	{
		return m_prdiv;
	}

	Uint GetVdiv() const
	{
		return m_vdiv;
	}

	uint32_t GetCoreClock() const
	{
		return m_core_clock;
	}

private:
	Uint m_prdiv;
	Uint m_vdiv;
	uint32_t m_core_clock;
};

void PllDividerCalc::Calc(const uint32_t external_osc_khz,
		const uint32_t core_clock_khz)
{
	/*Uint best_prdiv = 0, best_vdiv = 0;
	Uint min_diff = static_cast<Uint>(-1);
	for (Uint i = 0; i <= PRDIV_MAX; ++i)
	{
		const uint32_t pll_ref_khz = external_osc_khz / (i + 1);
		if (pll_ref_khz < MIN_PLL_REF_KHZ || pll_ref_khz > MAX_PLL_REF_KHZ)
		{
			// PLL reference freq not valid
			continue;
		}

		for (Uint j = 0; j <= 0x1F; ++j)
		{
			uint32_t this_clock = external_osc_khz * (j + VDIV_BASE) / (i + 1);
#if Mkl26F15
			// kl26 120/150 parts have an additional /2 at the output of VCO
			this_clock >>= 1;
#endif
			const Uint this_diff = abs((int32_t)(this_clock - core_clock_khz));
			if (this_diff < min_diff)
			{
				min_diff = this_diff;
				best_prdiv = i;
				best_vdiv = j;
			}

			if (min_diff == 0)
			{
				break;
			}
		}
	}

	m_prdiv = best_prdiv;
	m_vdiv = best_vdiv;

	m_core_clock = (uint64_t)(external_osc_khz * 1000) * (m_vdiv + VDIV_BASE)
			/ (m_prdiv + 1);
#if Mkl26F15
	m_core_clock >>= 1;
#endif
*/}

}

Mcg::Config::Config()
		: external_oscillator_khz(0),
		  core_clock_khz(SystemCoreClock / 1000),
		  bus_clock_khz(SystemCoreClock / 2 / 1000),
//		  flexbus_clock_khz(MAX_FLEXBUS_CLOCK / 1000),
		  flash_clock_khz(SystemCoreClock / 2 / 1000)
{}

Mcg::Mcg()
		: m_core_clock(0)
{}

void Mcg::Init()
{
	/*const Config &config = GetMcgConfig();

	// First, FEI must transition to FBE mode
	InitFbe(config);

	PllDividerCalc calc;
	calc.Calc(config.external_oscillator_khz, config.core_clock_khz);
	InitClocks(config, calc.GetCoreClock());

	// Then configure C5[PRDIV] to generate correct PLL reference frequency
	MCG->C5 |= _MCG_C5_PRDIV0(calc.GetPrdiv());
#if Mkl26F15
	// Select PLL0 clock source
	CLEAR_BIT(MCG->C11, MCG_C11_PLLCS_SHIFT);
	// Select the external reference clock from OSC0 as the reference clock to
	// the PLL
	CLEAR_BIT(MCG->C5, MCG_C5_PLLREFSEL0_SHIFT);
#endif

	// Then, FBE must transition either directly to PBE mode or first through
	// BLPE mode and then to PBE mode
	InitPbe(config, calc.GetVdiv());

	// Lastly, PBE mode transitions into PEE mode
	InitPee(config);*/

	m_core_clock = SystemCoreClock/*calc.GetCoreClock()*/;
}

void Mcg::InitFbe(const Config &config)
{
	/*uint8_t c2_reg = 0;
#if Mkl26F15
	// Reset with a loss of OSC clock
	SET_BIT(c2_reg, MCG_C2_LOCRE0_SHIFT);
#endif
	c2_reg |= _MCG_C2_RANGE0(2);
	MCG->C2 = c2_reg;

	uint8_t c1_reg = 0;
	// C1[CLKS] set to 2'b10 in order to select external reference clock as
	// system clock source
	c1_reg |= MCG_C1_CLKS(2);
	int best_div = 0;
	Uint min_diff = static_cast<Uint>(-1);
	for (int i = 0; i < 6; ++i)
	{
		const uint32_t freq = config.external_oscillator_khz * 1000 >> (5 + i);
		if (freq >= FLL_MIN_FREQ && freq <= FLL_MAX_FREQ)
		{
			best_div = i;
			break;
		}
		else if (freq > FLL_MAX_FREQ)
		{
			if (freq - FLL_MAX_FREQ < min_diff)
			{
				best_div = i;
				min_diff = freq - FLL_MAX_FREQ;
			}
		}
		else if (freq < FLL_MIN_FREQ)
		{
			if (FLL_MIN_FREQ - freq < min_diff)
			{
				best_div = i;
				min_diff = FLL_MIN_FREQ - freq;
			}
		}
	}
	c1_reg |= MCG_C1_FRDIV(best_div);
	MCG->C1 = c1_reg;

	// Loop until S[IREFST] is 0, indicating the external reference is the
	// current source for the reference clock
	while (GET_BIT(MCG->S, MCG_S_IREFST_SHIFT))
	{}

	// Loop until S[CLKST] is 2'b10, indicating that the external reference
	// clock is selected to feed MCGOUTCLK
	while (GET_BITS(MCG->S, MCG_S_CLKST_SHIFT, MCG_S_CLKST_MASK) != 0x2)
	{}*/
}

void Mcg::InitPbe(const Config&, const uint8_t vdiv)
{
	/*// If a transition through BLPE mode is desired, first set C2[LP] to 1
	SET_BIT(MCG->C2, MCG_C2_LP_SHIFT);

	uint8_t c6_reg = 0;
	SET_BIT(c6_reg, MCG_C6_PLLS_SHIFT);
	c6_reg |= _MCG_C6_VDIV0(vdiv);
	MCG->C6 = c6_reg;

	// Clear C2[LP] to 0 here to switch to PBE mode
	CLEAR_BIT(MCG->C2, MCG_C2_LP_SHIFT);

	// Loop until S[PLLST] is set, indicating that the current source for the
	// PLLS clock is the PLL
	while (!GET_BIT(MCG->S, MCG_S_PLLST_SHIFT))
	{}

	// Then loop until S[LOCK] is set, indicating that the PLL has acquired lock
	while (!GET_BIT(MCG->S, _MCG_S_LOCK0_SHIFT))
	{}*/
}

void Mcg::InitPee(const Config&)
{
	/*// C1[CLKS] set to 2'b00 in order to select the output of the PLL as the
	// system clock source
	MCG->C1 &= ~MCG_C1_CLKS_MASK;

	// Loop until S[CLKST] are 2'b11, indicating that the PLL output is selected
	// to feed MCGOUTCLK in the current clock mode
	while (GET_BITS(MCG->S, MCG_S_CLKST_SHIFT, MCG_S_CLKST_MASK) != 0x3)
	{}*/
}

void Mcg::InitClocks(const Config &config, const uint32_t core_clock)
{
	/*Uint bus_div = 0;
	Uint bus_clk = config.bus_clock_khz * 1000;
	bus_div = core_clock / std::min<uint32_t>(bus_clk, MAX_BUS_CLOCK);
	const Uint max_bus_clk = core_clock / bus_div;
	const Uint min_bus_clk = core_clock / (bus_div + 1);
	if (max_bus_clk > MAX_BUS_CLOCK
			|| max_bus_clk - bus_clk > bus_clk - min_bus_clk)
	{
		++bus_div;
	}
	assert(bus_div > 0);

	Uint flexbus_div = 0;
	Uint flexbus_clk = config.flexbus_clock_khz * 1000;
	flexbus_div = core_clock / std::min<uint32_t>(flexbus_clk, MAX_FLEXBUS_CLOCK);
	const Uint max_flexbus_clk = core_clock / flexbus_div;
	const Uint min_flexbus_clk = core_clock / (flexbus_div + 1);
	if (max_flexbus_clk > MAX_FLEXBUS_CLOCK
			|| max_flexbus_clk - flexbus_clk > flexbus_clk - min_flexbus_clk)
	{
		++flexbus_div;
	}
	assert(flexbus_div > 0);

	Uint flash_div = 0;
	Uint flash_clk = config.flash_clock_khz * 1000;
	flash_div = core_clock / std::min<uint32_t>(flash_clk, MAX_FLASH_CLOCK);
	const Uint max_flash_clk = core_clock / flash_div;
	const Uint min_flash_clk = core_clock / (flash_div + 1);
	if (max_flash_clk > MAX_FLASH_CLOCK
			|| max_flash_clk - flash_clk > flash_clk - min_flash_clk)
	{
		++flash_div;
	}
	assert(flash_div > 0);

//	uint32_t reg = 0;
//	reg |= SIM_CLKDIV1_OUTDIV2(std::min<Uint>(bus_div - 1, 0xF));
//	reg |= SIM_CLKDIV1_OUTDIV3(std::min<Uint>(flexbus_div - 1, 0xF));
//	reg |= SIM_CLKDIV1_OUTDIV4(std::min<Uint>(flash_div - 1, 0xF));
//	SIM->CLKDIV1 = reg;
	SetSysDividers(0, std::min<Uint>(bus_div - 1, 0xF),
			std::min<Uint>(flexbus_div - 1, 0xF),
			std::min<Uint>(flash_div - 1, 0xF));*/
}

}
}

void Libbasekl26McgInit()
{
	libbase::kl26::Mcg::Get().Init();
}

uint32_t Libbasekl26McgGetCoreClock()
{
	return libbase::kl26::Mcg::Get().GetCoreClock();
}
