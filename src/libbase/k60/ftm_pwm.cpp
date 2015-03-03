/*
 * ftm_pwm.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstddef>
#include <cstdint>

#include "libbase/log.h"
#include "libbase/k60/clock_utils.h"
#include "libbase/k60/ftm.h"
#include "libbase/k60/ftm_pwm.h"
#include "libbase/k60/ftm_utils.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/pinout.h"
#include "libbase/k60/pwm.h"
#include "libbase/k60/sim.h"

#include "libutil/misc.h"

using namespace libutil;

namespace libbase
{
namespace k60
{

namespace
{

constexpr FTM_Type* MEM_MAPS[PINOUT::GetFtmCount()] =
{
	FTM0,
	FTM1,
	FTM2,
#if PINOUT_FTM_COUNT > 3
	FTM3,
#endif
};

FtmPwm* g_instances[PINOUT::GetFtmCount()][PINOUT::GetFtmChannelCount()] = {};

class ModCalc
{
public:
	void Calc(const uint32_t period, const Pwm::Config::Precision precision);

	uint8_t GetPrescaler() const
	{
		return m_prescaler;
	}

	uint16_t GetMod() const
	{
		return m_mod;
	}

private:
	uint8_t m_prescaler;
	uint16_t m_mod;
};

void ModCalc::Calc(const uint32_t period, const Pwm::Config::Precision precision)
{
	uint32_t ticks = 0;
	switch (precision)
	{
	default:
		assert(false);
		// no break

	case Pwm::Config::Precision::kUs:
		ticks = ClockUtils::GetBusTickPerUs(period);
		break;

	case Pwm::Config::Precision::kNs:
		ticks = ClockUtils::GetBusTickPerNs(period);
		break;
	}

	m_prescaler = 0;
	while (ticks > UINT16_MAX)
	{
		ticks >>= 1;
		++m_prescaler;
	}
	// Check if prescaler is too large or not
	assert(!(m_prescaler & ~FTM_SC_PS_MASK));
	m_mod = ticks - 1;
}

class CvCalc
{
public:
	void Calc(const uint32_t pos_width, const Pwm::Config::Precision precision,
			const uint8_t prescaler);

	uint16_t GetCv() const
	{
		return m_cv;
	}

private:
	uint16_t m_cv;
};

void CvCalc::Calc(const uint32_t pos_width,
		const Pwm::Config::Precision precision, const uint8_t prescaler)
{
	uint32_t ticks = 0;
	switch (precision)
	{
	default:
		assert(false);
		// no break

	case Pwm::Config::Precision::kUs:
		ticks = ClockUtils::GetBusTickPerUs(pos_width);
		break;

	case Pwm::Config::Precision::kNs:
		ticks = ClockUtils::GetBusTickPerNs(pos_width);
		break;
	}

	m_cv = ticks >> prescaler;
}

class DeadtimeCalc
{
public:
	void Calc(const uint32_t ns);

	uint8_t GetPrescaler() const
	{
		return m_prescaler;
	}

	uint8_t GetValue() const
	{
		return m_value;
	}

private:
	uint8_t m_prescaler;
	uint8_t m_value;
};

void DeadtimeCalc::Calc(const uint32_t ns)
{
	const uint32_t ticks = ClockUtils::GetBusTickPerNs(ns);
	if (ticks <= 63)
	{
		m_prescaler = 0;
		m_value = ticks;
	}
	else if (ticks <= 252)
	{
		m_prescaler = 2;
		m_value = (ticks + 3) >> 2;
	}
	else
	{
		m_prescaler = 3;
		m_value = (ticks + 15) >> 4;
	}
}

inline Uint GetDeadtimeEnableShift(const Uint ch)
{
	return (ch >> 1) * 8 + FTM_COMBINE_DTEN0_SHIFT;
}

}

FtmPwm::FtmPwm(const Config &config)
		: m_period(config.period),
		  m_precision(config.precision),
		  m_alignment(config.alignment),
		  m_pin(nullptr),
		  m_is_init(false)
{
	assert(config.pos_width <= config.period);
	if (!InitModule(config.pin))
	{
		assert(false);
		return;
	}

	bool is_module_init = false;
	for (Uint i = 0; i < PINOUT::GetFtmChannelCount(); ++i)
	{
		// Channels under the same module must share the same period/frequency,
		// alignment and polarity
		if (g_instances[m_module][i])
		{
			is_module_init = true;
			if (g_instances[m_module][i]->m_period != config.period
					|| GET_BIT(MEM_MAPS[m_module]->SC, FTM_SC_CPWMS_SHIFT)
							!= (config.alignment == Config::Alignment::kCenter))
			{
				assert(false);
				return;
			}
		}
	}

	m_is_init = true;

	Sim::SetEnableClockGate(EnumAdvance(Sim::ClockGate::kFtm0, m_module), true);

	g_instances[m_module][m_channel] = this;
	InitPin(config.pin);
	SetReadOnlyReg(false);
	InitChannelPolarity(config);

	ModCalc mc;
	uint32_t period = config.period;
	if (config.alignment == Config::Alignment::kCenter)
	{
		period >>= 1;
	}
	mc.Calc(period, config.precision);

	CvCalc cc;
	uint32_t pos_width = config.pos_width;
	if (config.alignment == Config::Alignment::kCenter)
	{
		pos_width >>= 1;
	}
	cc.Calc(pos_width, m_precision, mc.GetPrescaler());

	if (!is_module_init)
	{
		InitChannel(config, cc.GetCv());
		InitCounter(mc.GetMod());
		InitScReg(config, mc.GetPrescaler());
		InitConfReg(config);
		InitDeadtime(config);
	}
	else
	{
		InitChannel(config, cc.GetCv());
	}

	SetReadOnlyReg(true);
}

FtmPwm::FtmPwm(FtmPwm &&rhs)
		: FtmPwm(nullptr)
{
	*this = std::move(rhs);
}

FtmPwm::FtmPwm(nullptr_t)
		: m_module(0),
		  m_channel(0),
		  m_period(0),
		  m_precision(Config::Precision::kUs),
		  m_alignment(Config::Alignment::kEdge),
		  m_pin(nullptr),
		  m_is_init(false)
{}

FtmPwm::~FtmPwm()
{
	Uninit();
}

FtmPwm& FtmPwm::operator=(FtmPwm &&rhs)
{
	if (this != &rhs)
	{
		Uninit();
		if (rhs)
		{
			rhs.m_is_init = false;

			m_module = rhs.m_module;
			m_channel = rhs.m_channel;
			m_period = rhs.m_period;
			m_precision = rhs.m_precision;
			m_alignment = rhs.m_alignment;

			m_pin = std::move(rhs.m_pin);

			m_is_init = true;
			g_instances[m_module][m_channel] = this;
		}
	}
	return *this;
}

bool FtmPwm::InitModule(const Pin::Name pin)
{
	const Ftm::Name ftm = PINOUT::GetFtm(pin);
	if (ftm == Ftm::Name::kDisable)
	{
		return false;
	}

	m_module = FtmUtils::GetFtmModule(ftm);
	m_channel = FtmUtils::GetFtmChannel(ftm);
	return true;
}

void FtmPwm::InitPin(const Pin::Name pin)
{
	Pin::Config config;
	config.pin = pin;
	config.mux = PINOUT::GetFtmMux(pin);

	m_pin = Pin(config);
}

void FtmPwm::InitChannelPolarity(const Config &config)
{
	if (config.is_active_high)
	{
		CLEAR_BIT(MEM_MAPS[m_module]->POL, m_channel);
		CLEAR_BIT(MEM_MAPS[m_module]->OUTINIT, m_channel);
	}
	else
	{
		SET_BIT(MEM_MAPS[m_module]->POL, m_channel);
		SET_BIT(MEM_MAPS[m_module]->OUTINIT, m_channel);
	}
}

void FtmPwm::InitCounter(const uint16_t mod)
{
	MEM_MAPS[m_module]->CNT = 0;

	uint32_t cntin_reg = 0;
	cntin_reg |= FTM_CNTIN_INIT(0);

	MEM_MAPS[m_module]->CNTIN = cntin_reg;

	uint32_t mod_reg = 0;
	mod_reg |= FTM_MOD_MOD(mod);

	MEM_MAPS[m_module]->MOD = mod_reg;
}

void FtmPwm::InitChannel(const Config &config, const uint16_t cv)
{
	uint32_t csc_reg = 0;
	if (config.alignment == Config::Alignment::kEdge)
	{
		SET_BIT(csc_reg, FTM_CnSC_MSB_SHIFT);
	}
	SET_BIT(csc_reg, FTM_CnSC_ELSB_SHIFT);

	MEM_MAPS[m_module]->CONTROLS[m_channel].CnSC = csc_reg;

	SetCv(cv);
}

void FtmPwm::InitScReg(const Config &config, const uint8_t prescaler)
{
	uint32_t reg = 0;
	if (config.alignment == Config::Alignment::kCenter)
	{
		SET_BIT(reg, FTM_SC_CPWMS_SHIFT);
	}
	reg |= FTM_SC_CLKS(1);

	MEM_MAPS[m_module]->SC = reg;

	SetPrescaler(prescaler);
}

void FtmPwm::InitConfReg(const Config&)
{
	uint32_t reg = 0;
	reg |= FTM_CONF_BDMMODE(1);

	MEM_MAPS[m_module]->CONF = reg;
}

void FtmPwm::InitDeadtime(const Config &config)
{
	if (!config.is_insert_deadtime)
	{
		return;
	}

	DeadtimeCalc dc;
	dc.Calc(config.deadtime_ns);

	uint32_t reg = 0;
	reg |= FTM_DEADTIME_DTPS(dc.GetPrescaler());
	reg |= FTM_DEADTIME_DTVAL(dc.GetValue());

	const uint32_t curr_reg = MEM_MAPS[m_module]->DEADTIME
			& (FTM_DEADTIME_DTPS_MASK | FTM_DEADTIME_DTVAL_MASK);
	if (curr_reg != 0 && reg != curr_reg)
	{
		LOG_E("Trying to set deadtime for the same module %d again", m_module);
		assert(false);
	}

	// We use the larger of the two to play safe. If everything's right,
	// curr_reg should either be 0 or == reg
	if (reg >= curr_reg)
	{
		MEM_MAPS[m_module]->DEADTIME = reg;
	}

	SET_BIT(MEM_MAPS[m_module]->COMBINE, GetDeadtimeEnableShift(m_channel));
}

void FtmPwm::Uninit()
{
	if (m_is_init)
	{
		m_is_init = false;

		SetReadOnlyReg(false);
		MEM_MAPS[m_module]->CONTROLS[m_channel].CnSC = 0;
		SetReadOnlyReg(true);

		UninitDeadtime();

		Sim::SetEnableClockGate(EnumAdvance(Sim::ClockGate::kFtm0, m_module),
				false);
		g_instances[m_module][m_channel] = nullptr;
	}
}

void FtmPwm::UninitDeadtime()
{
	bool is_all_free = true;
	bool is_pair_free = true;
	const uint8_t pair_ch = (m_channel & 0x1) ? (m_channel & ~0x1)
			: (m_channel + 1);
	for (Uint i = 0; i < PINOUT::GetFtmChannelCount(); ++i)
	{
		if (g_instances[m_module][i] && i != m_channel)
		{
			is_all_free = false;
			if (i == pair_ch)
			{
				is_pair_free = false;
			}
		}
	}
	if (is_all_free)
	{
		MEM_MAPS[m_module]->DEADTIME &= ~(FTM_DEADTIME_DTPS_MASK
				| FTM_DEADTIME_DTVAL_MASK);
	}
	if (is_pair_free)
	{
		CLEAR_BIT(MEM_MAPS[m_module]->COMBINE, GetDeadtimeEnableShift(m_channel));
	}
}

void FtmPwm::SetReadOnlyReg(const bool flag)
{
	if (flag)
	{
		SET_BIT(MEM_MAPS[m_module]->FMS, FTM_FMS_WPEN_SHIFT);
	}
	else
	{
		if (GET_BIT(MEM_MAPS[m_module]->FMS, FTM_FMS_WPEN_SHIFT))
		{
			SET_BIT(MEM_MAPS[m_module]->MODE, FTM_MODE_WPDIS_SHIFT);
		}
	}
}

void FtmPwm::SetCv(const uint16_t cv)
{
	uint32_t cv_reg = 0;
	cv_reg |= FTM_CnV_VAL(cv);

	MEM_MAPS[m_module]->CONTROLS[m_channel].CnV = cv_reg;
}

void FtmPwm::SetPrescaler(const uint8_t prescaler)
{
	MEM_MAPS[m_module]->SC &= ~FTM_SC_PS_MASK;
	MEM_MAPS[m_module]->SC |= FTM_SC_PS(prescaler);
}

void FtmPwm::SetPeriod(const uint32_t period, const uint32_t pos_width)
{
	STATE_GUARD(FtmPwm, VOID);

	assert(pos_width <= period);
	for (Uint i = 0; i < PINOUT::GetFtmChannelCount(); ++i)
	{
		if (g_instances[m_module][i] && g_instances[m_module][i] != this)
		{
			assert(false);
			return;
		}
	}

	SetReadOnlyReg(false);
	ModCalc mc;
	mc.Calc(period, m_precision);
	InitCounter(mc.GetMod());
	CvCalc cc;
	cc.Calc(pos_width, m_precision, mc.GetPrescaler());
	SetCv(cc.GetCv());
	SetPrescaler(mc.GetPrescaler());
	SetReadOnlyReg(true);

	m_period = period;
}

void FtmPwm::SetPosWidth(const uint32_t pos_width)
{
	STATE_GUARD(FtmPwm, VOID);

	CvCalc cc;
	uint32_t pos_width_ = pos_width;
	if (m_alignment == Config::Alignment::kCenter)
	{
		pos_width_ >>= 1;
	}
	cc.Calc(pos_width_, m_precision, GetPrescaler());

	SetCv(cc.GetCv());
}

uint8_t FtmPwm::GetPrescaler() const
{
	STATE_GUARD(FtmPwm, 0);

	return GET_BITS(MEM_MAPS[m_module]->SC, FTM_SC_PS_SHIFT, FTM_SC_PS_MASK);
}

}
}
