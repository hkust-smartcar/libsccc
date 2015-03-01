/*

 * adc.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
*/

#include "libbase/kl26/hardware.h"

#include <cassert>
#include <cstddef>
#include <cstdint>

#include <algorithm>
#include <bitset>
#include <functional>

#include <machine/endian.h>

#include "libbase/log.h"
#include "libbase/kl26/adc.h"
#include "libbase/kl26/adc_utils.h"
#include "libbase/kl26/clock_utils.h"
#include "libbase/kl26/misc_utils.h"
#include "libbase/kl26/pinout.h"
#include "libbase/kl26/sim.h"
#include "libbase/kl26/vectors.h"

#include "libutil/misc.h"

using namespace libutil;
using namespace std;

namespace libbase
{
namespace kl26
{

namespace
{

constexpr ADC_Type* MEM_MAPS[PINOUT::GetAdcCount()] =
{
	ADC0,
};

Adc* g_instances[PINOUT::GetAdcCount()] = {};

Uint GetClockDivider(const Uint target_clock_khz)
{
	const Uint bus_clock = ClockUtils::GetBusClockKhz();
	// 2^n
	Uint divider = 0;

	for (Uint i = bus_clock; i > target_clock_khz; i >>= 1)
	{
		++divider;
	}

	if (divider == 0)
	{
		return 0;
	}
	else if (divider >= 4)
	{
		return 4;
	}
	else
	{
		const Uint up_diff = abs((int)target_clock_khz
				- (int)(bus_clock >> (divider - 1)));
		const Uint down_diff = abs((int)target_clock_khz
				- (int)(bus_clock >> divider));
		if (up_diff < down_diff)
		{
			return divider - 1;
		}
		else
		{
			return divider;
		}
	}
}

}

Adc::Adc(const Config &config)
		: m_name(Name::kDisable),
		  m_config(config),
		  m_pin(nullptr)
{
	assert(m_config.pin != Pin::Name::kDisable
			|| m_config.adc != Adc::Name::kDisable);

	if (m_config.pin != Pin::Name::kDisable)
	{
		if (!InitModule(m_config.pin) || !InitPin(m_config.pin))
		{
			assert(false);
			return;
		}
	}
	else
	{
		if (!InitModule(m_config.adc))
		{
			assert(false);
			return;
		}
	}

	Sim::SetEnableClockGate(EnumAdvance(Sim::ClockGate::kAdc0,
			AdcUtils::GetModule(m_name)), true);

/*
	if (m_config.is_continuous_mode && !m_config.conversion_isr)
	{
		m_config.conversion_isr = std::bind(&Adc::OnConversionComplete,
				this, placeholders::_1, placeholders::_2);
	}
*/
}

Adc::Adc(Adc &&rhs)
		: Adc(nullptr)
{
	*this = std::move(rhs);
}

Adc::Adc(nullptr_t)
		: m_name(Name::kDisable),
		  m_pin(nullptr)
{}

Adc::~Adc()
{
	Uninit();
}

Adc& Adc::operator=(Adc &&rhs)
{
	if (this != &rhs)
	{
		Uninit();
		if (rhs)
		{
			const Name name = rhs.m_name;
			rhs.m_name = Name::kDisable;

			m_pin = std::move(rhs.m_pin);
			m_config = std::move(rhs.m_config);
			m_name = name;
		}
	}
	return *this;
}

bool Adc::InitModule(const Pin::Name pin)
{
	m_name = PINOUT::GetAdc(pin);
	return (m_name != Adc::Name::kDisable);
}

bool Adc::InitModule(const Name adc)
{
	m_name = adc;
	return (m_name != Name::kDisable);
}

bool Adc::InitPin(const Pin::Name pin)
{
	Pin::Config config;
	config.pin = pin;
	config.mux = Pin::Config::MuxControl::kAnalog;

	m_pin = Pin(config);
	return m_pin;
}

void Adc::InitSc1Reg()
{
	uint32_t reg = 0;

	if (m_config.is_diff_mode && AdcUtils::GetChannelNumber(m_name) < 4)
	{
		SET_BIT(reg, ADC_SC1_DIFF_SHIFT);
	}
	// Terminate conversion
	reg |= ADC_SC1_ADCH(0x1F);

	MEM_MAPS[AdcUtils::GetModule(m_name)]->SC1[0] = reg;
}

void Adc::InitCfg1Reg()
{
	uint32_t reg = 0;

	switch (m_config.resolution)
	{
	case Config::Resolution::k8Bit:
		reg |= ADC_CFG1_MODE(0);
		break;

	case Config::Resolution::k10Bit:
		reg |= ADC_CFG1_MODE(2);
		break;

	case Config::Resolution::k12Bit:
		reg |= ADC_CFG1_MODE(1);
		break;

	default:
	case Config::Resolution::k16Bit:
		reg |= ADC_CFG1_MODE(3);
		break;
	}

	MEM_MAPS[AdcUtils::GetModule(m_name)]->CFG1 = reg;
}

void Adc::InitCfg2Reg()
{
	uint32_t reg = 0;

	if (AdcUtils::GetChannel(m_name) >= (Uint)Adc::Name::kAdc0Ad4B
			&& AdcUtils::GetChannel(m_name) <= (Uint)Adc::Name::kAdc0Ad7B)
	{
		SET_BIT(reg, ADC_CFG2_MUXSEL_SHIFT);
	}

	MEM_MAPS[AdcUtils::GetModule(m_name)]->CFG2 = reg;
}

void Adc::InitSc3Reg()
{
	uint32_t reg = 0;

	if (m_config.is_continuous_mode)
	{
		SET_BIT(reg, ADC_SC3_ADCO_SHIFT);
	}

	if (m_config.avg_pass != Config::AveragePass::kDisable)
	{
		SET_BIT(reg, ADC_SC3_AVGE_SHIFT);
		reg |= ADC_SC3_AVGS((int)m_config.avg_pass - 1);
	}

	MEM_MAPS[AdcUtils::GetModule(m_name)]->SC3 = reg;
}

void Adc::InitSpeed()
{
	const Uint module = AdcUtils::GetModule(m_name);

	Uint clock_divider = 0;
	// [0 - 4], 4 == longest
	int long_sample_time = -1;
	bool is_high_speed = false;

	switch (m_config.speed)
	{
	case Config::SpeedMode::kExFast:
		clock_divider = GetClockDivider(20000);
		is_high_speed = true;
		break;

	case Config::SpeedMode::kFast:
		clock_divider = GetClockDivider(12000);
		is_high_speed = true;
		break;

	case Config::SpeedMode::kTypical:
		clock_divider = GetClockDivider(8000);
		break;

	case Config::SpeedMode::kSlow:
		clock_divider = GetClockDivider(4000);
		long_sample_time = 2;
		break;

	case Config::SpeedMode::kExSlow:
		clock_divider = GetClockDivider(1000);
		long_sample_time = 4;
		break;
	}

	MEM_MAPS[module]->CFG1 |= ADC_CFG1_ADIV(std::min<Uint>(clock_divider, 3));

	if (long_sample_time != -1)
	{
		SET_BIT(MEM_MAPS[module]->CFG1, ADC_CFG1_ADLSMP_SHIFT);
		MEM_MAPS[module]->CFG2 |= ADC_CFG2_ADLSTS(4 - long_sample_time);
	}

	if (is_high_speed)
	{
		SET_BIT(MEM_MAPS[module]->CFG2, ADC_CFG2_ADHSC_SHIFT);
	}
}

void Adc::InitInterrupt()
{
	const Uint module = AdcUtils::GetModule(m_name);

	DisableInterrupt(module);
	if (m_config.conversion_isr)
	{
		EnableInterrupt();
	}
}

void Adc::Uninit()
{
	if (m_name != Name::kDisable)
	{
		const Name name = m_name;
		m_name = Name::kDisable;

		if (IsActive())
		{
			StopConvert();
		}
		Sim::SetEnableClockGate(EnumAdvance(Sim::ClockGate::kAdc0,
				AdcUtils::GetModule(name)), false);
		PINOUT::UnregPin(name);
	}
}

void Adc::StartConvert()
{
	InitSc1Reg();
	InitCfg1Reg();
	InitCfg2Reg();
	InitSc3Reg();
	InitSpeed();
	InitInterrupt();

	// Start conversion
	uint32_t reg = MEM_MAPS[AdcUtils::GetModule(m_name)]->SC1[0];
	reg &= ~ADC_SC1_ADCH_MASK;
	reg |= ADC_SC1_ADCH(AdcUtils::GetChannelNumber(m_name));
	MEM_MAPS[AdcUtils::GetModule(m_name)]->SC1[0] = reg;
}

void Adc::StopConvert()
{
	const Uint module = AdcUtils::GetModule(m_name);
	DisableInterrupt(module);
	MEM_MAPS[AdcUtils::GetModule(m_name)]->SC1[0] = 0 | ADC_SC1_ADCH(0x1F);
}

uint16_t Adc::GetResult()
{
	STATE_GUARD(Adc, 0);

	if (!IsActive())
	{
		StartConvert();
	}
	uint16_t result;
	while (!PeekResult(&result))
	{}
	return result;
}

float Adc::GetResultF()
{
	// we can't predivide 3.3 and the resolution because the number is too small
	// i.e., precision problem
	const float multiplied = GetResult() * 3.3f;
	switch (m_config.resolution)
	{
	case Config::Resolution::k8Bit:
		return multiplied / 0x00FF;

	case Config::Resolution::k10Bit:
		return multiplied / 0x03FF;

	case Config::Resolution::k12Bit:
		return multiplied / 0x0FFF;

	default:
	case Config::Resolution::k16Bit:
		return multiplied / 0xFFFF;
	}
}

bool Adc::PeekResult(uint16_t *out_val)
{
	STATE_GUARD(Adc, false);

	if (!IsConversionComplete())
	{
		return false;
	}

	const uint16_t result = MEM_MAPS[AdcUtils::GetModule(m_name)]->R[0];
	if (m_config.is_diff_mode)
	{
		switch (m_config.resolution)
		{
		case Config::Resolution::k8Bit:
			*out_val = result & 0x01FF;
			break;

		case Config::Resolution::k10Bit:
			*out_val = result & 0x07FF;
			break;

		case Config::Resolution::k12Bit:
			*out_val = result & 0x1FFF;
			break;

		default:
		case Config::Resolution::k16Bit:
			*out_val = result;
			break;
		}
	}
	else
	{
		*out_val = result;
	}
	return true;
}

bool Adc::IsActive() const
{
	STATE_GUARD(Adc, false);

	const bool is_channel_selected =
			((MEM_MAPS[AdcUtils::GetModule(m_name)]->SC1[0] & ADC_SC1_ADCH_MASK)
			>> ADC_SC1_ADCH_SHIFT) == AdcUtils::GetChannelNumber(m_name);
	return is_channel_selected && IsConversionActive();
}

bool Adc::IsConversionActive() const
{
	STATE_GUARD(Adc, false);
	return GET_BIT(MEM_MAPS[AdcUtils::GetModule(m_name)]->SC2,
			ADC_SC2_ADACT_SHIFT);
}

bool Adc::IsConversionComplete() const
{
	STATE_GUARD(Adc, false);
	return GET_BIT(MEM_MAPS[AdcUtils::GetModule(m_name)]->SC1[0],
			ADC_SC1_COCO_SHIFT);
}

void Adc::EnableInterrupt()
{
	const Uint module = AdcUtils::GetModule(m_name);

	g_instances[module] = this;
	if (module < 2)
	{
		SetIsr(EnumAdvance(ADC0_IRQn, module), IrqHandler);
		EnableIrq(EnumAdvance(ADC0_IRQn, module));
	}
#if PINOUT_ADC_COUNT > 2
	else if (module < 4)
	{
		SetIsr(EnumAdvance(ADC2_IRQn, module - 2), IrqHandler);
		EnableIrq(EnumAdvance(ADC2_IRQn, module - 2));
	}
#endif
	SET_BIT(MEM_MAPS[module]->SC1[0], ADC_SC1_AIEN_SHIFT);
}

void Adc::DisableInterrupt(const Uint module)
{
	CLEAR_BIT(MEM_MAPS[module]->SC1[0], ADC_SC1_AIEN_SHIFT);
	if (module < 2)
	{
		DisableIrq(EnumAdvance(ADC0_IRQn, module));
		SetIsr(EnumAdvance(ADC0_IRQn, module), nullptr);
	}
#if PINOUT_ADC_COUNT > 2
	else if (module < 4)
	{
		DisableIrq(EnumAdvance(ADC2_IRQn, module - 2));
		SetIsr(EnumAdvance(ADC2_IRQn, module - 2), nullptr);
	}
#endif
	g_instances[module] = nullptr;
}

__ISR void Adc::IrqHandler()
{
	Uint module = 0;
	if (module < 2)
	{
		module = GetActiveIrq() - ADC0_IRQn;
	}
#if PINOUT_ADC_COUNT > 2
	else if (module < 4)
	{
		module = GetActiveIrq() - ADC2_IRQn;
	}
#endif

	Adc *const that = g_instances[module];
	if (!that || !(*that))
	{
		// Something wrong?
		assert(false);
		DisableInterrupt(module);
	}
	else
	{
		that->m_config.conversion_isr(that, MEM_MAPS[module]->R[0]);
	}
}

}
}
