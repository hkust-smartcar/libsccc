/*
 * rand_generator.cpp
 * Random Number Generator Accelerator (for RNGA)
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifdef MK60D10

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstddef>
#include <cstdint>

#include <log.h>

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/rand_generator.h"
#include "libbase/k60/sim.h"

namespace libbase
{
namespace k60
{

RandGenerator::RandGenerator(const Config &config)
		: m_is_init(true)
{
	Sim::SetEnableClockGate(Sim::ClockGate::kRnga, true);
	RNG->CR = 0 | RNG_CR_INTM_MASK | RNG_CR_HA_MASK;
	if (config.is_ext_entropy)
	{
		SetEntropy(config.entropy);
	}
	SET_BIT(RNG->CR, RNG_CR_GO_SHIFT);
}

RandGenerator::RandGenerator(RandGenerator &&rhs)
		: RandGenerator(nullptr)
{
	*this = std::move(rhs);
}

RandGenerator::RandGenerator(nullptr_t)
		: m_is_init(false)
{}

RandGenerator::~RandGenerator()
{
	Uninit();
}

RandGenerator& RandGenerator::operator=(RandGenerator &&rhs)
{
	if (this != &rhs)
	{
		Uninit();
		if (rhs)
		{
			rhs.m_is_init = false;

			m_is_init = true;
		}
	}
	return *this;
}

void RandGenerator::Uninit()
{
	if (m_is_init)
	{
		m_is_init = false;

		Sim::SetEnableClockGate(Sim::ClockGate::kRnga, false);
	}
}

void RandGenerator::SetEntropy(const uint32_t entropy)
{
	STATE_GUARD(RandGenerator, VOID);

	RNG->ER = entropy;
}

bool RandGenerator::IsReady() const
{
	STATE_GUARD(RandGenerator, false);

	return ((RNG->SR & RNG_SR_OREG_LVL_MASK) > 0);
}

uint32_t RandGenerator::GetRand() const
{
	STATE_GUARD(RandGenerator, 0);

	while (!IsReady())
	{}
	return RNG->OR;
}

}
}

#endif
