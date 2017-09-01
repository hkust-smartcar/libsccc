/*
 * cache.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstdint>

#include "libbase/k60/cache.h"
#include "libbase/k60/misc_utils.h"

namespace libbase
{
namespace k60
{

#if MK60F15

Cache::Cache()
		: m_is_init(false)
{}

void Cache::Init(const Config &config)
{
	if (m_is_init)
	{
		assert(false);
		return;
	}

	if (config.is_enable)
	{
		InitPc();
		InitPs();
		LMEM->PSCRMR&=~(LMEM_PSCRMR_R6_MASK);
		LMEM->PCCRMR&=~(LMEM_PCCRMR_R6_MASK);
	}
	m_is_init = true;
}

void Cache::InitPc()
{
	uint32_t ccr_reg = 0;
	SET_BIT(ccr_reg, LMEM_PCCCR_INVW0_SHIFT);
	SET_BIT(ccr_reg, LMEM_PCCCR_INVW1_SHIFT);
	SET_BIT(ccr_reg, LMEM_PCCCR_GO_SHIFT);

	LMEM->PCCCR = ccr_reg;

	// Wait for the LMEM_PCCCR[GO] bit to clear indicating the command has
	// completed
	while (GET_BIT(LMEM->PCCCR, LMEM_PCCCR_GO_SHIFT))
	{}

	SET_BIT(LMEM->PCCCR, LMEM_PCCCR_ENCACHE_SHIFT);
}

void Cache::InitPs()
{
	uint32_t ccr_reg = 0;
	SET_BIT(ccr_reg, LMEM_PSCCR_INVW0_SHIFT);
	SET_BIT(ccr_reg, LMEM_PSCCR_INVW1_SHIFT);
	SET_BIT(ccr_reg, LMEM_PSCCR_GO_SHIFT);

	LMEM->PSCCR = ccr_reg;

	// Wait for the LMEM_PSCCR[GO] bit to clear indicating the command has
	// completed
	while (GET_BIT(LMEM->PSCCR, LMEM_PSCCR_GO_SHIFT))
	{}

	SET_BIT(LMEM->PSCCR, LMEM_PSCCR_ENCACHE_SHIFT);
}

#endif

}
}
