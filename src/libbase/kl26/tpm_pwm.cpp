/*
 * tpm.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/kl26/hardware.h"

#include <cassert>
#include <cstddef>
#include <cstdint>

#include "libbase/log.h"
#include "libbase/kl26/clock_utils.h"
#include "libbase/kl26/pin.h"
#include "libbase/kl26/pinout.h"
#include "libbase/kl26/sim.h"
#include "libbase/kl26/tpm.h"
#include "libbase/kl26/tpm_pwm.h"
#include "libbase/kl26/tpm_utils.h"
#include "libbase/misc_types.h"

#include "libutil/misc.h"

using namespace libutil;

namespace libbase
{
namespace kl26
{

namespace
{

constexpr TPM_Type* MEM_MAPS[PINOUT::GetTpmCount()] =
{
	TPM0,
	TPM1,
	TPM2,
};

TpmPwm* g_instances[PINOUT::GetTpmCount()][PINOUT::GetTpmChannelCount()] = {};

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
	const uint8_t multiplier = Sim::GetCoreClockDivider() ;
	// PLL clock is divided by 2 before feeding us
	switch (precision)
	{
	default:
		assert(false);
		// no break

	case Pwm::Config::Precision::kUs:
		ticks = ClockUtils::GetCoreTickPerUs(period) * multiplier / 2;
		break;

	case Pwm::Config::Precision::kNs:
		ticks = ClockUtils::GetCoreTickPerNs(period) * multiplier / 2;
		break;
	}

	m_prescaler = 0;
	while (ticks > UINT16_MAX)
	{
		ticks >>= 1;
		++m_prescaler;
	}
	// Check if prescaler is too large or not
	assert(!(m_prescaler & ~TPM_SC_PS_MASK));
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
	const uint8_t multiplier = Sim::GetCoreClockDivider();
	// PLL clock is divided by 2 before feeding us
	switch (precision)
	{
	default:
		assert(false);
		// no break

	case Pwm::Config::Precision::kUs:
		ticks = ClockUtils::GetCoreTickPerUs(pos_width) * multiplier / 2;
		break;

	case Pwm::Config::Precision::kNs:
		ticks = ClockUtils::GetCoreTickPerNs(pos_width) * multiplier / 2;
		break;
	}

	m_cv = ticks >> prescaler;
}

}

TpmPwm::TpmPwm(const Config &config)
		: m_period(config.period),
		  m_precision(config.precision),
		  m_alignment(config.alignment),
		  m_pin(nullptr),
		  m_is_init(false)
{
	assert(config.period > 0);
	assert(config.pos_width <= config.period);
	if (!InitModule(config.pin))
	{
		assert(false);
		return;
	}

	bool is_module_init = false;
	for (Uint i = 0; i < PINOUT::GetTpmChannelCount(); ++i)
	{
		// Channels under the same module must share the same period/frequency
		// and alignment
		if (g_instances[m_module][i])
		{
			is_module_init = true;
			if (g_instances[m_module][i]->m_period != config.period
					|| GET_BIT(MEM_MAPS[m_module]->SC, TPM_SC_CPWMS_SHIFT)
							!= (config.alignment == Config::Alignment::kCenter))
			{
				assert(false);
				return;
			}
		}
	}

	m_is_init = true;

	uint32_t period_ = config.period;
	uint32_t pos_width_ = config.pos_width;
	if (m_alignment == Config::Alignment::kCenter)
	{
		period_ >>= 1;
		pos_width_ >>= 1;
	}
	ModCalc mc;
	mc.Calc(period_, config.precision);
	CvCalc cc;
	cc.Calc(pos_width_, config.precision, mc.GetPrescaler());

	Sim::SetTpmClockSource(Sim::TpmClock::kPll);
	Sim::SetEnableClockGate(EnumAdvance(Sim::ClockGate::kTpm0, m_module), true);

	g_instances[m_module][m_channel] = this;
	InitPin(config.pin);

	if (!is_module_init)
	{
		SetEnableCounter(false);
		InitChannel(config, cc.GetCv());
		InitCounter(mc.GetMod());
		InitScReg(config, mc.GetPrescaler());
		InitConfReg();
		SetEnableCounter(true);
	}
	else
	{
		InitChannel(config, cc.GetCv());
	}
}

TpmPwm::TpmPwm(nullptr_t)
		: m_module(0),
		  m_channel(0),
		  m_period(0),
		  m_precision(Config::Precision::kUs),
		  m_alignment(Config::Alignment::kEdge),
		  m_pin(nullptr),
		  m_is_init(false)
{}

TpmPwm::~TpmPwm()
{
	Uninit();
}

TpmPwm& TpmPwm::operator=(TpmPwm &&rhs)
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

bool TpmPwm::InitModule(const Pin::Name pin)
{
	const Tpm::Name tpm = PINOUT::GetTpm(pin);
	if (tpm == Tpm::Name::kDisable)
	{
		return false;
	}

	m_module = TpmUtils::GetTpmModule(tpm);
	m_channel = TpmUtils::GetTpmChannel(tpm);
	return true;
}

void TpmPwm::InitPin(const Pin::Name pin)
{
	Pin::Config config;
	config.pin = pin;
	config.mux = PINOUT::GetTpmMux(pin);

	m_pin = Pin(config);
}

void TpmPwm::InitCounter(const uint16_t mod)
{
	MEM_MAPS[m_module]->CNT = 0;
	MEM_MAPS[m_module]->MOD = TPM_MOD_MOD(mod);
}

void TpmPwm::InitChannel(const Config &config, const uint16_t cv)
{
	uint32_t csc_reg = 0;
	SET_BIT(csc_reg, TPM_CnSC_MSB_SHIFT);
	if (config.is_active_high)
	{
		SET_BIT(csc_reg, TPM_CnSC_ELSB_SHIFT);
	}
	else
	{
		SET_BIT(csc_reg, TPM_CnSC_ELSA_SHIFT);
	}

	MEM_MAPS[m_module]->CONTROLS[m_channel].CnSC = csc_reg;

	SetCv(cv);
}

void TpmPwm::InitScReg(const Config &config, const uint8_t prescaler)
{
	uint32_t reg = 0;
	if (config.alignment == Config::Alignment::kCenter)
	{
		SET_BIT(reg, TPM_SC_CPWMS_SHIFT);
	}

	MEM_MAPS[m_module]->SC = reg;

	SetPrescaler(prescaler);
}

void TpmPwm::InitConfReg()
{
	uint32_t reg = 0;
	// Keep the counter going in debug mode
	reg |= TPM_CONF_DBGMODE(3);

	MEM_MAPS[m_module]->CONF = reg;
}

void TpmPwm::Uninit()
{
	if (m_is_init)
	{
		m_is_init = false;

		m_pin = Pin(nullptr);
		MEM_MAPS[m_module]->CONTROLS[m_channel].CnSC = 0;

		g_instances[m_module][m_channel] = nullptr;
		if (Tpm::Get().UnregTpm(TpmUtils::GetTpm(m_module, m_channel)))
		{
			Sim::SetEnableClockGate(EnumAdvance(Sim::ClockGate::kTpm0, m_module),
					false);
		}
	}
}

void TpmPwm::SetCv(const uint16_t cv)
{
	MEM_MAPS[m_module]->CONTROLS[m_channel].CnV = TPM_CnV_VAL(cv);
}

void TpmPwm::SetPrescaler(const uint8_t prescaler)
{
	MEM_MAPS[m_module]->SC &= ~TPM_SC_PS_MASK;
	MEM_MAPS[m_module]->SC |= TPM_SC_PS(prescaler);
}

void TpmPwm::SetEnableCounter(const bool flag)
{
	if (flag)
	{
		MEM_MAPS[m_module]->SC |= TPM_SC_CMOD(1);
	}
	else
	{
		MEM_MAPS[m_module]->SC &= ~TPM_SC_CMOD_MASK;
	}
}

void TpmPwm::SetPeriod(const uint32_t period, const uint32_t pos_width)
{
	STATE_GUARD(TpmPwm, VOID);
	assert(pos_width <= period);

	for (Uint i = 0; i < PINOUT::GetTpmChannelCount(); ++i)
	{
		if (g_instances[m_module][i] && g_instances[m_module][i] != this)
		{
			assert(false);
			return;
		}
	}

	uint32_t period_ = period;
	uint32_t pos_width_ = pos_width;
	if (m_alignment == Config::Alignment::kCenter)
	{
		period_ >>= 1;
		pos_width_ >>= 1;
	}
	ModCalc mc;
	mc.Calc(period_, m_precision);
	CvCalc cc;
	cc.Calc(pos_width_, m_precision, mc.GetPrescaler());

	SetEnableCounter(false);
	InitCounter(mc.GetMod());
	SetCv(cc.GetCv());
	SetPrescaler(mc.GetPrescaler());
	SetEnableCounter(true);

	m_period = period;
}

void TpmPwm::SetPosWidth(const uint32_t pos_width)
{
	STATE_GUARD(TpmPwm, VOID);
	assert(pos_width <= m_period);

	uint32_t pos_width_ = pos_width;
	if (m_alignment == Config::Alignment::kCenter)
	{
		pos_width_ >>= 1;
	}
	CvCalc cc;
	cc.Calc(pos_width_, m_precision, GetPrescaler());

	SetCv(cc.GetCv());
}

uint8_t TpmPwm::GetPrescaler() const
{
	return GET_BITS(MEM_MAPS[m_module]->SC, TPM_SC_PS_SHIFT, TPM_SC_PS_MASK);
}

}
}
