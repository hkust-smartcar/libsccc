/*
 * dma_manager.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstdint>

#include <bitset>

#include "libbase/log.h"
#include "libbase/k60/dma.h"
#include "libbase/k60/dma_manager.h"
#include "libbase/k60/pinout.h"
#include "libbase/k60/sim.h"

using namespace libbase::k60;

namespace libbase
{
namespace k60
{

DmaManager* DmaManager::m_instance = nullptr;

DmaManager::Config::Config()
{
	for (Uint i = 0; i < PINOUT::GetDmaChCount(); ++i)
	{
		ch_configs[i].priority = i;
	}
}

DmaManager* DmaManager::GetInstance()
{
	if (!m_instance)
	{
		m_instance = new DmaManager;
	}
	return m_instance;
}

DmaManager::DmaManager()
		: m_is_init(false),
		  m_is_minor_loop_mapping(false)
{}

void DmaManager::Init_(const Config &config)
{
	Sim::SetEnableClockGate(Sim::ClockGate::kDma, true);

	InitCrReg(config);
	InitDchPriReg(config);

	m_is_init = true;
}

void DmaManager::InitCrReg(const Config &config)
{
	uint32_t reg = 0;

	if (config.config[Config::ConfigBit::kMinorLoopMapping])
	{
		SET_BIT(reg, DMA_CR_EMLM_SHIFT);
		m_is_minor_loop_mapping = true;
	}
	if (config.config[Config::ConfigBit::kContinuousLinkMode])
	{
		SET_BIT(reg, DMA_CR_CLM_SHIFT);
	}
	if (config.config[Config::ConfigBit::kHaltOnError])
	{
		SET_BIT(reg, DMA_CR_HOE_SHIFT);
	}
	if (config.config[Config::ConfigBit::kRoundRobinChannelArbitration])
	{
		SET_BIT(reg, DMA_CR_ERCA_SHIFT);
	}
#if MK60F15
	// TODO Allow more flexible control on group priority
	reg |= DMA_CR_GRP0PRI(0);
	reg |= DMA_CR_GRP0PRI(1);
#endif

	DMA0->CR = reg;
}

void DmaManager::InitDchPriReg(const Config &config)
{
	static constexpr volatile uint8_t *DCHPRI_MEM_MAPS[PINOUT::GetDmaChCount()] =
	{
			&DMA0->DCHPRI0,
			&DMA0->DCHPRI1,
			&DMA0->DCHPRI2,
			&DMA0->DCHPRI3,
			&DMA0->DCHPRI4,
			&DMA0->DCHPRI5,
			&DMA0->DCHPRI6,
			&DMA0->DCHPRI7,
			&DMA0->DCHPRI8,
			&DMA0->DCHPRI9,
			&DMA0->DCHPRI10,
			&DMA0->DCHPRI11,
			&DMA0->DCHPRI12,
			&DMA0->DCHPRI13,
			&DMA0->DCHPRI14,
			&DMA0->DCHPRI15,
#if PINOUT_DMA_CH_COUNT > 16
			&DMA0->DCHPRI16,
			&DMA0->DCHPRI17,
			&DMA0->DCHPRI18,
			&DMA0->DCHPRI19,
			&DMA0->DCHPRI20,
			&DMA0->DCHPRI21,
			&DMA0->DCHPRI22,
			&DMA0->DCHPRI23,
			&DMA0->DCHPRI24,
			&DMA0->DCHPRI25,
			&DMA0->DCHPRI26,
			&DMA0->DCHPRI27,
			&DMA0->DCHPRI28,
			&DMA0->DCHPRI29,
			&DMA0->DCHPRI30,
			&DMA0->DCHPRI31,
#endif
	};

	for (Uint i = 0; i < PINOUT::GetDmaChCount(); ++i)
	{
		uint32_t reg = 0;

		if (config.ch_configs[i].is_channel_preemption)
		{
			SET_BIT(reg, DMA_DCHPRI0_ECP_SHIFT);
		}
		if (config.ch_configs[i].is_preempt_low_channel)
		{
			SET_BIT(reg, DMA_DCHPRI0_DPA_SHIFT);
		}
		reg |= DMA_DCHPRI0_CHPRI(config.ch_configs[i].priority);

		*DCHPRI_MEM_MAPS[i] = reg;
	}
}

Dma* DmaManager::New_(const Dma::Config &dma_config, const Uint channel)
{
	EnsureInit();
	if (m_channels[channel].dma)
	{
		LOG_E("DMA channel %u already set", channel);
		assert(false);
		return nullptr;
	}

	m_channels[channel].dma = Dma(dma_config, channel);
	return &m_channels[channel].dma;
}

void DmaManager::Delete_(Dma *dma)
{
	EnsureInit();
	const Uint channel = dma->GetChannel();
	if (!m_channels[channel].dma)
	{
		return;
	}

	m_channels[channel].dma = Dma(nullptr);
}

Dma* DmaManager::GetChannel_(const Uint channel)
{
	return (m_channels[channel].dma ? &m_channels[channel].dma : nullptr);
}

}
}
