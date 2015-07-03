/*
 * dma.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstddef>
#include <cstdint>

#include <functional>

#include "libbase/log.h"
#include "libbase/k60/dma.h"
#include "libbase/k60/dma_manager.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pinout.h"
#include "libbase/k60/vectors.h"

#include "libutil/misc.h"

using namespace libutil;

#if MK60D10 || MK60DZ10
	#define DMA_IRQ_BEG DMA0_IRQn
#elif MK60F15
	#define DMA_IRQ_BEG DMA0_DMA16_IRQn
#endif

namespace libbase
{
namespace k60
{

Dma::Dma(const Config &config, const Uint channel)
		: m_mux_src(config.mux_src),
		  m_complete_isr(config.complete_isr),
		  m_error_isr(config.error_isr),
		  m_is_init(false)
{
	assert(channel < 32);
	m_channel = channel;

	Stop_();
	m_is_init = true;
	Init(config);
}

Dma::Dma(Dma &&rhs)
		: Dma(nullptr)
{
	*this = std::move(rhs);
}

Dma::Dma(nullptr_t)
		: m_channel(0),
		  m_mux_src(DmaMux::Source::kNull),
		  m_is_init(false)
{}

Dma::~Dma()
{
	if (m_is_init)
	{
		DmaManager::Delete(this);
	}
}

Dma& Dma::operator=(Dma &&rhs)
{
	if (this != &rhs)
	{
		Uninit();
		if (rhs)
		{
			rhs.m_is_init = false;

			m_channel = rhs.m_channel;
			m_mux_src = rhs.m_mux_src;
			m_complete_isr = rhs.m_complete_isr;
			m_error_isr = rhs.m_error_isr;

			m_is_init = true;
		}
	}
	return *this;
}

void Dma::Init(const Config &config)
{
	DMA0->TCD[m_channel].SADDR = DMA_SADDR_SADDR(config.src.addr);
	DMA0->TCD[m_channel].SOFF = DMA_SOFF_SOFF(config.src.offset);
	InitTcdAttrReg(config);
	InitTcdNbytesReg(config);
	DMA0->TCD[m_channel].SLAST = DMA_SLAST_SLAST(config.src.major_offset);
	DMA0->TCD[m_channel].DADDR = DMA_DADDR_DADDR(config.dst.addr);
	DMA0->TCD[m_channel].DOFF = DMA_DOFF_DOFF(config.dst.offset);
	DMA0->TCD[m_channel].DLAST_SGA = DMA_DLAST_SGA_DLASTSGA(
			config.dst.major_offset);
	InitTcdCsrReg(config);
	InitTcdIterReg(config);

	InitEeiReg(config);

	ResetDone();
}

void Dma::InitTcdAttrReg(const Config &config)
{
	uint16_t reg = 0;
	reg |= DMA_ATTR_SSIZE(config.src.size);
	reg |= DMA_ATTR_DSIZE(config.dst.size);

	DMA0->TCD[m_channel].ATTR = reg;
}

void Dma::InitTcdNbytesReg(const Config &config)
{
	uint32_t reg = 0;
	if (!DmaManager::IsMinorLoopMapping())
	{
		reg |= DMA_NBYTES_MLNO_NBYTES(config.minor_bytes);

		DMA0->TCD[m_channel].NBYTES_MLNO = reg;
	}
	else if (!config.minor_loop.is_enable_src_offset
				&& !config.minor_loop.is_enable_dst_offset)
	{
		assert(config.minor_bytes < (1 << 30));

		reg |= DMA_NBYTES_MLOFFNO_NBYTES(config.minor_bytes);

		DMA0->TCD[m_channel].NBYTES_MLOFFNO = reg;
	}
	else
	{
		assert(config.minor_bytes < (1 << 10));

		if (config.minor_loop.is_enable_src_offset)
		{
			SET_BIT(reg, DMA_NBYTES_MLOFFYES_SMLOE_SHIFT);
		}
		if (config.minor_loop.is_enable_dst_offset)
		{
			SET_BIT(reg, DMA_NBYTES_MLOFFYES_DMLOE_SHIFT);
		}
		reg |= DMA_NBYTES_MLOFFYES_MLOFF(config.minor_loop.offset);
		reg |= DMA_NBYTES_MLOFFYES_NBYTES(config.minor_bytes);

		DMA0->TCD[m_channel].NBYTES_MLOFFYES = reg;
	}
}

void Dma::InitTcdIterReg(const Config &config)
{
	uint16_t reg = 0;
	reg |= DMA_CITER_ELINKNO_CITER(config.major_count);
	DMA0->TCD[m_channel].CITER_ELINKNO = reg;
	DMA0->TCD[m_channel].BITER_ELINKNO = reg;
}

void Dma::InitTcdCsrReg(const Config &config)
{
	uint16_t reg = 0;
	reg |= DMA_CSR_BWC(static_cast<int>(config.stall_duration));
	if (config.is_disable_request)
	{
		SET_BIT(reg, DMA_CSR_DREQ_SHIFT);
	}
	if (config.complete_isr)
	{
		if (config.is_listen_half_complete)
		{
			SET_BIT(reg, DMA_CSR_INTHALF_SHIFT);
		}
		SET_BIT(reg, DMA_CSR_INTMAJOR_SHIFT);
	}

	DMA0->TCD[m_channel].CSR = reg;
}

void Dma::InitEeiReg(const Config &config)
{
	if (config.is_eror_irq)
	{
		DMA0->SEEI = DMA_SEEI_SEEI(m_channel);
	}
	else
	{
		DMA0->CEEI = DMA_CEEI_CEEI(m_channel);
	}
}

bool Dma::Reinit(const Config &config)
{
	if (!IsActive())
	{
		Init(config);
		return true;
	}
	else
	{
		return false;
	}
}

void Dma::Uninit()
{
	if (m_is_init)
	{
		m_is_init = false;

		Stop_();
		DMA0->TCD[m_channel].CSR = 0;
	}
}

void Dma::Start()
{
	STATE_GUARD(Dma, VOID);

	ResetDone();

	// Set current major count to beg
	const uint16_t major_count = DMA_BITER_ELINKNO_BITER(
			DMA0->TCD[m_channel].BITER_ELINKNO);
	DMA0->TCD[m_channel].CITER_ELINKNO |= DMA_CITER_ELINKNO_CITER(major_count);

	// Enable request
	DMA0->SERQ = DMA_SERQ_SERQ(m_channel);

	ConsumeInterrupt();
	EnableInterrupt();

	if (m_mux_src == DmaMux::Source::kNull)
	{
		SET_BIT(DMA0->TCD[m_channel].CSR, DMA_CSR_START_SHIFT);
	}
	else
	{
		DmaMux::SetEnableSource(m_mux_src, m_channel);
	}
}

void Dma::Stop()
{
	// The public version has to be guarded, while the internal one should not
	STATE_GUARD(Dma, VOID);

	Stop_();
}

void Dma::Stop_()
{
	DisableInterrupt();

	// Disable request
	DMA0->CERQ = DMA_CERQ_CERQ(m_channel);

	// Set major count to 0
	DMA0->TCD[m_channel].CITER_ELINKNO &= ~DMA_CITER_ELINKNO_CITER_MASK;

	// Disable DMA MUX
	if (m_mux_src != DmaMux::Source::kNull)
	{
		DmaMux::SetEnableSource(DmaMux::Source::kNull, m_channel);
	}
}

bool Dma::IsDone() const
{
	STATE_GUARD(Dma, false);

	return GET_BIT(DMA0->TCD[m_channel].CSR, DMA_CSR_DONE_SHIFT);
}

void Dma::ResetDone()
{
	DMA0->CDNE = DMA_CDNE_CDNE(m_channel);
}

bool Dma::IsActive() const
{
	STATE_GUARD(Dma, false);

	return GET_BIT(DMA0->TCD[m_channel].CSR, DMA_CSR_ACTIVE_SHIFT);
}

void Dma::EnableInterrupt()
{
#if MK60F15
	const Uint irq_num = (m_channel >= 16) ? (m_channel - 16) : m_channel;
#elif MK60D10 || MK60DZ10
	const Uint irq_num = m_channel;
#endif
	SetIsr(EnumAdvance(DMA_IRQ_BEG, irq_num), IrqHandler);
	EnableIrq(EnumAdvance(DMA_IRQ_BEG, irq_num));
}

void Dma::DisableInterrupt()
{
#if MK60F15
	const Uint irq_num = (m_channel >= 16) ? (m_channel - 16) : m_channel;
#elif MK60D10 || MK60DZ10
	const Uint irq_num = m_channel;
#endif
	DisableIrq(EnumAdvance(DMA_IRQ_BEG, irq_num));
	SetIsr(EnumAdvance(DMA_IRQ_BEG, irq_num), nullptr);
}

void Dma::ConsumeInterrupt()
{
	uint8_t reg = 0;
	reg |= DMA_CINT_CINT(m_channel);

	DMA0->CINT = reg;
}

__ISR void Dma::IrqHandler()
{
	const Uint irq_num = GetActiveIrq() - DMA_IRQ_BEG;
#if MK60F15
	const Uint channel = GET_BIT(DMA0->INT, irq_num) ? irq_num : irq_num + 16;
#elif MK60D10 || MK60DZ10
	const Uint channel = irq_num;
#endif
	Dma *const that = DmaManager::GetChannel(channel);
	if (!that || !(*that))
	{
		// Something wrong?
		assert(false);
		that->DisableInterrupt();
	}
	else
	{
		that->m_complete_isr(that);
	}
	that->ConsumeInterrupt();
}

}
}
