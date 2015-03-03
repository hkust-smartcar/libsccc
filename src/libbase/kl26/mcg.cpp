/*
 * mcg.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/kl26/hardware.h"

#include <cassert>
#include <cstdint>

#include <algorithm>

#include "libbase/kl26/mcg.h"
#include "libbase/kl26/mcg_c.h"
#include "libbase/kl26/misc_utils.h"

#define FLL_MAX_FREQ 39062
#define FLL_MIN_FREQ 31250

#if MKL26Z4
	#define LOW_FREQ_MIN_KHZ 32
	#define LOW_FREQ_MAX_KHZ 40
	#define HIGH_FREQ_MIN_KHZ 3000
	#define HIGH_FREQ_MAX_KHZ 8000
	#define VERY_HIGH_FREQ_MIN_KHZ 8000
	#define VERY_HIGH_FREQ_MAX_KHZ 32000

	#define PLL_REF_MIN_KHZ 2000
	#define PLL_REF_MAX_KHZ 4000

	#define PRDIV_MIN 0
	#define PRDIV_MAX 25
	#define VDIV_MIN 24
	#define VDIV_MAX 55

	#define CORE_CLOCK 48000000
	#define BUS_CLOCK 24000000
	#define MAX_CORE_CLOCK 100000000
	#define MAX_BUS_CLOCK 28800000
#endif

namespace libbase
{
namespace kl26
{

namespace
{

Uint GetFreqRange(const uint32_t external_osc_khz)
{
	if (external_osc_khz >= LOW_FREQ_MIN_KHZ
			&& external_osc_khz <= LOW_FREQ_MAX_KHZ)
	{
		return 0;
	}
	// Let the border case go upwards
	else if (external_osc_khz >= HIGH_FREQ_MIN_KHZ
			&& external_osc_khz < HIGH_FREQ_MAX_KHZ)
	{
		return 1;
	}
	else if (external_osc_khz >= VERY_HIGH_FREQ_MIN_KHZ
			&& external_osc_khz <= VERY_HIGH_FREQ_MAX_KHZ)
	{
		return 2;
	}
	else
	{
		assert(false);
		return 0;
	}
}

class FllDividerCalc
{
public:
	void Calc(const uint32_t external_osc_khz);

	Uint GetFrdiv() const
	{
		return m_frdiv;
	}

private:
	Uint m_frdiv;
};

void FllDividerCalc::Calc(const uint32_t external_osc_khz)
{
	Uint best_div = 0;
	Uint min_diff = static_cast<Uint>(-1);
	const Uint freq_range = GetFreqRange(external_osc_khz);
	for (int i = 0; i < 8; ++i)
	{
		uint32_t freq;
		if (freq_range == 0 || freq_range == 1)
		{
			freq = (external_osc_khz * 1000) >> i;
		}
		else
		{
			if (i < 6)
			{
				freq = (external_osc_khz * 1000) >> (5 + i);
			}
			else
			{
				freq = (external_osc_khz * 1000) / ((i == 6) ? 1280 : 1536);
			}
		}

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

	m_frdiv = best_div;
}

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
	Uint best_prdiv = 0, best_vdiv = 0;
	Uint min_diff = static_cast<Uint>(-1);
	for (Uint i = PRDIV_MIN; i <= PRDIV_MAX; ++i)
	{
		const uint32_t pll_ref_khz = external_osc_khz / i;
		if (pll_ref_khz < PLL_REF_MIN_KHZ || pll_ref_khz > PLL_REF_MAX_KHZ)
		{
			// PLL reference freq not valid
			continue;
		}

		for (Uint j = VDIV_MIN; j <= VDIV_MAX; ++j)
		{
			uint32_t this_clock = external_osc_khz * j / i;
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

	m_prdiv = best_prdiv - PRDIV_MIN;
	m_vdiv = best_vdiv - VDIV_MIN;

	m_core_clock = (uint64_t)(external_osc_khz * 1000) * best_vdiv / best_prdiv;
}

}

Mcg::Config::Config()
		: external_oscillator_khz(0),
		  core_clock_khz(CORE_CLOCK / 1000),
		  bus_clock_khz(BUS_CLOCK / 1000)
{}

Mcg::Mcg()
		: m_core_clock(0)
{}

void Mcg::Init()
{
	const Config &config = GetMcgConfig();

	// First, FEI must transition to FBE mode
	InitFbe(config);

	PllDividerCalc calc;
	calc.Calc(config.external_oscillator_khz, config.core_clock_khz);
	InitClocks(config, calc.GetCoreClock());

	// Then configure C5[PRDIV] to generate correct PLL reference frequency
	MCG->C5 |= MCG_C5_PRDIV0(calc.GetPrdiv());

	// Then, FBE must transition either directly to PBE mode or first through
	// BLPE mode and then to PBE mode
	InitPbe(config, calc.GetVdiv());

	// Lastly, PBE mode transitions into PEE mode
	InitPee(config);

	m_core_clock = calc.GetCoreClock();
}

void Mcg::InitFbe(const Config &config)
{
	uint8_t c2_reg = 0;
	// Reset with a loss of OSC clock
	SET_BIT(c2_reg, MCG_C2_LOCRE0_SHIFT);
	c2_reg |= MCG_C2_RANGE0(GetFreqRange(config.external_oscillator_khz));
	SET_BIT(c2_reg, MCG_C2_HGO0_SHIFT);
	SET_BIT(c2_reg, MCG_C2_EREFS0_SHIFT);
	MCG->C2 = c2_reg;

	FllDividerCalc calc;
	calc.Calc(config.external_oscillator_khz);

	uint8_t c1_reg = 0;
	// C1[CLKS] set to 2'b10 in order to select external reference clock as
	// system clock source
	c1_reg |= MCG_C1_CLKS(2);
	c1_reg |= MCG_C1_FRDIV(calc.GetFrdiv());
	MCG->C1 = c1_reg;

	// Loop until S[OSCINIT0] is 1, indicating the crystal selected by
	// C2[EREFS0] has been initialized
	while (!GET_BIT(MCG->S, MCG_S_OSCINIT0_SHIFT))
	{}

	// Loop until S[IREFST] is 0, indicating the external reference is the
	// current source for the reference clock
	while (GET_BIT(MCG->S, MCG_S_IREFST_SHIFT))
	{}

	// Loop until S[CLKST] is 2'b10, indicating that the external reference
	// clock is selected to feed MCGOUTCLK
	while (GET_BITS(MCG->S, MCG_S_CLKST_SHIFT, MCG_S_CLKST_MASK) != 0x2)
	{}
}

void Mcg::InitPbe(const Config&, const uint8_t vdiv)
{
	// If a transition through BLPE mode is desired, first set C2[LP] to 1
	SET_BIT(MCG->C2, MCG_C2_LP_SHIFT);

	uint8_t c6_reg = 0;
	SET_BIT(c6_reg, MCG_C6_PLLS_SHIFT);
	c6_reg |= MCG_C6_VDIV0(vdiv);
	MCG->C6 = c6_reg;

	// Clear C2[LP] to 0 here to switch to PBE mode
	CLEAR_BIT(MCG->C2, MCG_C2_LP_SHIFT);

	// Loop until S[PLLST] is set, indicating that the current source for the
	// PLLS clock is the PLL
	while (!GET_BIT(MCG->S, MCG_S_PLLST_SHIFT))
	{}

	// Then loop until S[LOCK0] is set, indicating that the PLL has acquired
	// lock
	while (!GET_BIT(MCG->S, MCG_S_LOCK0_SHIFT))
	{}
}

void Mcg::InitPee(const Config&)
{
	// C1[CLKS] set to 2'b00 in order to select the output of the PLL as the
	// system clock source
	MCG->C1 &= ~MCG_C1_CLKS_MASK;

	// Loop until S[CLKST] are 2'b11, indicating that the PLL output is selected
	// to feed MCGOUTCLK in the current clock mode
	while (GET_BITS(MCG->S, MCG_S_CLKST_SHIFT, MCG_S_CLKST_MASK) != 0x3)
	{}
}

void Mcg::InitClocks(const Config &config, const uint32_t core_clock)
{
	Uint bus_div = 0;
	Uint bus_clk = config.bus_clock_khz * 1000;
	bus_div = core_clock / std::min<uint32_t>(bus_clk, MAX_BUS_CLOCK);
	const Uint max_bus_clk = core_clock / bus_div;
	const Uint min_bus_clk = core_clock / (bus_div + 1);
	if (max_bus_clk > MAX_BUS_CLOCK
			|| max_bus_clk - bus_clk > bus_clk - min_bus_clk)
	{
		++bus_div;
	}
	assert(bus_div >= 1 && bus_div <= 8);

	SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV4(bus_div - 1);
}

}
}

void LibbaseKl26McgInit()
{
	libbase::kl26::Mcg::Get().Init();
}

uint32_t LibbaseKl26McgGetCoreClock()
{
	return libbase::kl26::Mcg::Get().GetCoreClock();
}
