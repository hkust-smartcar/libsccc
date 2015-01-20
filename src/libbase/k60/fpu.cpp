/*
 * fpu.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/hardware.h"

#include <cstdint>

#include "libbase/k60/fpu.h"
#include "libbase/k60/fpu_c.h"
#include "libbase/k60/misc_utils.h"

#define FPU_FPDSCR_RMode(x) (((x) << FPU_FPDSCR_RMode_Pos) & FPU_FPDSCR_RMode_Msk)

namespace libbase
{
namespace k60
{

#if __FPU_PRESENT

Fpu::Fpu()
{}

void Fpu::Init()
{
	const Config &config = GetFpuConfig();

	// Enable full access to the FPU coprocessor
	SCB->CPACR |= 0x00F00000;
	InitFpccr(config);
	InitFpdscr(config);
}

void Fpu::InitFpccr(const Config&)
{
	// ASPEN and LSPEN must not be modified at this stage
	uint32_t reg = FPU->FPCCR & (FPU_FPCCR_ASPEN_Msk | FPU_FPCCR_LSPEN_Msk);
	SET_BIT(reg, FPU_FPCCR_MONRDY_Pos);
	SET_BIT(reg, FPU_FPCCR_BFRDY_Pos);
	SET_BIT(reg, FPU_FPCCR_MMRDY_Pos);
	SET_BIT(reg, FPU_FPCCR_HFRDY_Pos);
	SET_BIT(reg, FPU_FPCCR_THREAD_Pos);
	SET_BIT(reg, FPU_FPCCR_USER_Pos);
	// enable lazy stacking
	SET_BIT(reg, FPU_FPCCR_LSPACT_Pos);

	FPU->FPCCR = reg;
}

void Fpu::InitFpdscr(const Config &config)
{
	uint32_t reg = 0;
	if (config.is_default_nan)
	{
		SET_BIT(reg, FPU_FPDSCR_DN_Pos);
	}
	if (config.is_flush_to_zero)
	{
		SET_BIT(reg, FPU_FPDSCR_FZ_Pos);
	}
	reg |= FPU_FPDSCR_RMode((int)config.rounding_mode);

	FPU->FPDSCR = reg;
}

__attribute__((__weak__))
Fpu::Config Fpu::GetFpuConfig()
{
	// Just use the default config
	return {};
}

#else
void Fpu::Init() {}

#endif

}
}

void LibbaseK60FpuInit()
{
#if __FPU_PRESENT
	libbase::k60::Fpu::Get().Init();
#endif
}
