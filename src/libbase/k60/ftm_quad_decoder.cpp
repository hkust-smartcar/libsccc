/*
 * ftm_quad_decoder.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstddef>
#include <cstdint>

#include <functional>

#include "libbase/log.h"
#include "libbase/k60/ftm.h"
#include "libbase/k60/ftm_quad_decoder.h"
#include "libbase/k60/ftm_utils.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/pinout.h"
#include "libbase/k60/quad_decoder_interface.h"
#include "libbase/k60/sim.h"
#include "libbase/k60/vectors.h"

#include "libutil/misc.h"

using namespace libutil;

namespace libbase
{
namespace k60
{

namespace
{

constexpr FTM_Type* MEM_MAPS[PINOUT::GetFtmCount()] = {FTM0, FTM1, FTM2};

FtmQuadDecoder* g_instances[PINOUT::GetFtmCount()] = {};

inline void ConsumeInterrupt(const Uint module)
{
	CLEAR_BIT(MEM_MAPS[module]->SC, FTM_SC_TOF_SHIFT);
}

}

FtmQuadDecoder::FtmQuadDecoder(const Config &config)
		: m_a(nullptr),
		  m_b(nullptr),
		  m_overflow(0),
		  m_is_init(false)
{
	if (!InitModule(config.a_pin, config.b_pin))
	{
		assert(false);
		return;
	}

	m_is_init = true;

	Sim::SetEnableClockGate(EnumAdvance(Sim::ClockGate::kFtm0, m_module), true);

	g_instances[m_module] = this;
	InitPin(config.a_pin, config.b_pin);
	SetReadOnlyReg(false);
	SET_BIT(MEM_MAPS[m_module]->MODE, FTM_MODE_FTMEN_SHIFT);
	InitCounter(config);
	InitScReg(config);
	InitQdctrlReg(config);
	InitConfReg(config);
	InitCounter(config);
	InitInputFilter(config);
	InitInterrupt(config);
	SetReadOnlyReg(true);
}

FtmQuadDecoder::FtmQuadDecoder(FtmQuadDecoder &&rhs)
		: FtmQuadDecoder(nullptr)
{
	*this = std::move(rhs);
}

FtmQuadDecoder::FtmQuadDecoder(nullptr_t)
		: m_module(0),
		  m_a(nullptr),
		  m_b(nullptr),
		  m_overflow(0),
		  m_mod(UINT16_MAX - 1),
		  m_is_init(false)
{}

FtmQuadDecoder::~FtmQuadDecoder()
{
	Uninit();
}

FtmQuadDecoder& FtmQuadDecoder::operator=(FtmQuadDecoder &&rhs)
{
	if (this != &rhs)
	{
		Uninit();
		if (rhs)
		{
			rhs.m_is_init = false;

			m_module = rhs.m_module;

			m_a = std::move(rhs.m_a);
			m_b = std::move(rhs.m_b);

			m_overflow = rhs.m_overflow;
			m_mod = rhs.m_mod;
			m_overflow_isr = rhs.m_overflow_isr;

			m_is_init = true;
			g_instances[m_module] = this;
		}
	}
	return *this;
}

bool FtmQuadDecoder::InitModule(const Pin::Name a_pin, const Pin::Name b_pin)
{
	const Ftm::QdName a_ftm = PINOUT::GetFtmQd(a_pin);
	const int a_module = FtmUtils::GetFtmModule(a_ftm);

	const Ftm::QdName b_ftm = PINOUT::GetFtmQd(b_pin);
	const int b_module = FtmUtils::GetFtmModule(b_ftm);

	if (a_module == b_module && a_ftm != Ftm::QdName::kDisable)
	{
		m_module = a_module;
		return true;
	}
	else
	{
		return false;
	}
}

void FtmQuadDecoder::InitPin(const Pin::Name a_pin, const Pin::Name b_pin)
{
	Pin::Config a_config, b_config;
	a_config.pin = a_pin;
	a_config.mux = PINOUT::GetFtmQdMux(a_pin);
	b_config.pin = b_pin;
	b_config.mux = PINOUT::GetFtmQdMux(b_pin);

	m_a = Pin(a_config);
	m_b = Pin(b_config);
}

void FtmQuadDecoder::InitScReg(const Config&)
{
	uint32_t reg = 0;
	reg |= FTM_SC_CLKS(0);

	MEM_MAPS[m_module]->SC = reg;
}

void FtmQuadDecoder::InitQdctrlReg(const Config &config)
{
	uint32_t reg = 0;
	SET_BIT(reg, FTM_QDCTRL_PHAFLTREN_SHIFT);
	SET_BIT(reg, FTM_QDCTRL_PHBFLTREN_SHIFT);
	if (config.is_invert_a_polarity)
	{
		SET_BIT(reg, FTM_QDCTRL_PHAPOL_SHIFT);
	}
	if (config.is_invert_b_polarity)
	{
		SET_BIT(reg, FTM_QDCTRL_PHBPOL_SHIFT);
	}
	if (config.encoding_mode == Config::EncodingMode::kCountDirection)
	{
		SET_BIT(reg, FTM_QDCTRL_QUADMODE_SHIFT);
	}
	SET_BIT(reg, FTM_QDCTRL_QUADEN_SHIFT);

	MEM_MAPS[m_module]->QDCTRL = reg;
}

void FtmQuadDecoder::InitConfReg(const Config&)
{
	uint32_t reg = 0;
	reg |= FTM_CONF_BDMMODE(1);

	MEM_MAPS[m_module]->CONF = reg;
}

void FtmQuadDecoder::InitCounter(const Config &config)
{
	MEM_MAPS[m_module]->CNT = 0;

	uint32_t cntin_reg = 0;
	cntin_reg |= FTM_CNTIN_INIT(0);

	MEM_MAPS[m_module]->CNTIN = cntin_reg;

	uint32_t mod_reg = 0;
	// To have IRQ at overflow_count, mod should be subtracted by 1
	m_mod = config.overflow_isr
			? Clamp<Uint>(2, config.overflow_count, UINT16_MAX) - 1
			: (UINT16_MAX - 1);
	mod_reg |= FTM_MOD_MOD(m_mod);

	MEM_MAPS[m_module]->MOD = mod_reg;
}

void FtmQuadDecoder::InitInputFilter(const Config &config)
{
	uint32_t reg = MEM_MAPS[m_module]->FILTER & ~FTM_FILTER_CH0FVAL_MASK
			& ~FTM_FILTER_CH1FVAL_MASK;
	reg |= FTM_FILTER_CH1FVAL(config.b_filter_length);
	reg |= FTM_FILTER_CH0FVAL(config.a_filter_length);

	MEM_MAPS[m_module]->FILTER = reg;
}

void FtmQuadDecoder::InitInterrupt(const Config &config)
{
	m_overflow_isr = config.overflow_isr;
	SetInterrupt(true);
}

void FtmQuadDecoder::Uninit()
{
	if (m_is_init)
	{
		m_is_init = false;

		SetInterrupt(false);
		SetReadOnlyReg(false);
		MEM_MAPS[m_module]->QDCTRL = 0;
		SetReadOnlyReg(true);

		Sim::SetEnableClockGate(EnumAdvance(Sim::ClockGate::kFtm0, m_module),
				false);
		g_instances[m_module] = nullptr;
	}
}

void FtmQuadDecoder::SetReadOnlyReg(const bool flag)
{
	if (flag)
	{
		SET_BIT(MEM_MAPS[m_module]->FMS, FTM_FMS_WPEN_SHIFT);
	}
	else
	{
		SET_BIT(MEM_MAPS[m_module]->MODE, FTM_MODE_WPDIS_SHIFT);
	}
}

void FtmQuadDecoder::SetInterrupt(const bool flag)
{
	if (flag)
	{
		ConsumeInterrupt(m_module);
		SetIsr(EnumAdvance(FTM0_IRQn, m_module), TofIrqHandler);
		SET_BIT(MEM_MAPS[m_module]->SC, FTM_SC_TOIE_SHIFT);
		EnableIrq(EnumAdvance(FTM0_IRQn, m_module));
	}
	else
	{
		DisableIrq(EnumAdvance(FTM0_IRQn, m_module));
		CLEAR_BIT(MEM_MAPS[m_module]->SC, FTM_SC_TOIE_SHIFT);
		SetIsr(EnumAdvance(FTM0_IRQn, m_module), nullptr);
	}
}

int32_t FtmQuadDecoder::GetCount()
{
	const uint16_t curr_count = GET_BITS(MEM_MAPS[m_module]->CNT,
			FTM_CNT_COUNT_SHIFT, FTM_CNT_COUNT_MASK);
	const int curr_overflow = m_overflow;
	if (curr_overflow == 0)
	{
		return curr_count;
	}
	else if (curr_overflow > 0)
	{
		return curr_count + ((m_mod + 1) * curr_overflow);
	}
	else // curr_overflow < 0
	{
		return -(m_mod - curr_count + 1) + ((m_mod + 1) * (curr_overflow + 1));
	}
}

void FtmQuadDecoder::ResetCount()
{
	MEM_MAPS[m_module]->CNT = FTM_CNT_COUNT(0);
	m_overflow = 0;
}

__ISR void FtmQuadDecoder::TofIrqHandler()
{
	const int module = GetActiveIrq() - FTM0_IRQn;
	FtmQuadDecoder *const that = g_instances[module];
	if (!that || !(*that))
	{
		// Something wrong?
		assert(false);
		CLEAR_BIT(MEM_MAPS[module]->SC, FTM_SC_TOIE_SHIFT);
		return;
	}

	that->m_overflow += GET_BIT(MEM_MAPS[module]->QDCTRL,
			FTM_QDCTRL_TOFDIR_SHIFT) ? 1 : -1;
	if (that->m_overflow_isr)
	{
		that->m_overflow_isr(that);
	}
	ConsumeInterrupt(module);
}

}
}
