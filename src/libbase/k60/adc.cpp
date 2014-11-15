/*
 * adc.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstddef>
#include <cstdint>

#include <bitset>
#include <functional>

#include <machine/endian.h>

#include "libbase/log.h"
#include "libbase/k60/adc.h"
#include "libbase/k60/adc_utils.h"
#include "libbase/k60/clock_utils.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pinout.h"
#include "libbase/k60/sim.h"
#include "libbase/k60/vectors.h"

#include "libutil/misc.h"

using namespace libutil;
using namespace std;

namespace libbase
{
namespace k60
{

namespace
{

constexpr ADC_Type* MEM_MAPS[PINOUT::GetAdcCount()] =
{
	ADC0,
	ADC1,
#if PINOUT_ADC_COUNT > 2
	ADC2,
#endif
#if PINOUT_ADC_COUNT > 3
	ADC3,
#endif
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
	if (m_config.is_continuous_mode && !m_config.conversion_listener)
	{
		m_config.conversion_listener = std::bind(&Adc::OnConversionComplete,
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

	if (clock_divider >= 4)
	{
		MEM_MAPS[module]->CFG1 |= ADC_CFG1_ADIV(3) | ADC_CFG1_ADICLK(1);
	}
	else
	{
		MEM_MAPS[module]->CFG1 |= ADC_CFG1_ADIV(clock_divider);
	}

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
	if (m_config.conversion_listener)
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
	return GET_BIT(MEM_MAPS[AdcUtils::GetModule(m_name)]->SC2,
			ADC_SC2_ADACT_SHIFT);
}

bool Adc::IsConversionComplete() const
{
	return GET_BIT(MEM_MAPS[AdcUtils::GetModule(m_name)]->SC1[0],
			ADC_SC1_COCO_SHIFT);
}

void Adc::EnableInterrupt()
{
	const Uint module = AdcUtils::GetModule(m_name);

	g_instances[module] = this;
	SetIsr(EnumAdvance(ADC0_IRQn, module), IrqHandler);
	EnableIrq(EnumAdvance(ADC0_IRQn, module));
	SET_BIT(MEM_MAPS[module]->SC1[0], ADC_SC1_AIEN_SHIFT);
}

void Adc::DisableInterrupt(const Uint module)
{
	CLEAR_BIT(MEM_MAPS[module]->SC1[0], ADC_SC1_AIEN_SHIFT);
	DisableIrq(EnumAdvance(ADC0_IRQn, module));
	SetIsr(EnumAdvance(ADC0_IRQn, module), nullptr);
	g_instances[module] = nullptr;
}

__ISR void Adc::IrqHandler()
{
	const Uint module = GetActiveIrq() - ADC0_IRQn;
	Adc *const that = g_instances[module];
	if (!that || !(*that))
	{
		// Something wrong?
		assert(false);
		DisableInterrupt(module);
	}
	else
	{
		that->m_config.conversion_listener(that, MEM_MAPS[module]->R[0]);
	}
}

}
}
