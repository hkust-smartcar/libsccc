/*
 * pin.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstdint>

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/pin_utils.h"
#include "libbase/k60/sim.h"

namespace libbase
{
namespace k60
{

namespace
{

constexpr PORT_Type* MEM_MAPS[5] = {PORTA, PORTB, PORTC, PORTD, PORTE};

void SetInterruptBit(const PinConfig::Interrupt config, uint32_t *reg)
{
	switch (config)
	{
	case PinConfig::Interrupt::kDisable:
		break;

	case PinConfig::Interrupt::kDmaRising:
		*reg |= PORT_PCR_IRQC(0x1);
		break;

	case PinConfig::Interrupt::kDmaFalling:
		*reg |= PORT_PCR_IRQC(0x2);
		break;

	case PinConfig::Interrupt::kDmaBoth:
		*reg |= PORT_PCR_IRQC(0x3);
		break;

	case PinConfig::Interrupt::kLow:
		*reg |= PORT_PCR_IRQC(0x8);
		break;

	case PinConfig::Interrupt::kRising:
		*reg |= PORT_PCR_IRQC(0x9);
		break;

	case PinConfig::Interrupt::kFalling:
		*reg |= PORT_PCR_IRQC(0xA);
		break;

	case PinConfig::Interrupt::kBoth:
		*reg |= PORT_PCR_IRQC(0xB);
		break;

	case PinConfig::Interrupt::kHigh:
		*reg |= PORT_PCR_IRQC(0xC);
		break;
	}
}

}

Pin::Pin(const PinConfig &config)
{
	if (config.pin == PinConfig::Name::DISABLE)
	{
		assert(false);
		return;
	}

	Sim::SetEnableClockGate((Sim::ClockGate)((int)Sim::ClockGate::kPortA
			+ PinUtils::GetPort(config.pin)), true);
	uint32_t reg = 0;

	switch (config.mux)
	{
	case PinConfig::MuxControl::kDisable:
		reg |= PORT_PCR_MUX(0);
		break;

	case PinConfig::MuxControl::kGpio:
		reg |= PORT_PCR_MUX(1);
		break;

	case PinConfig::MuxControl::kAlt2:
		reg |= PORT_PCR_MUX(2);
		break;

	case PinConfig::MuxControl::kAlt3:
		reg |= PORT_PCR_MUX(3);
		break;

	case PinConfig::MuxControl::kAlt4:
		reg |= PORT_PCR_MUX(4);
		break;

	case PinConfig::MuxControl::kAlt5:
		reg |= PORT_PCR_MUX(5);
		break;

	case PinConfig::MuxControl::kAlt6:
		reg |= PORT_PCR_MUX(6);
		break;

	case PinConfig::MuxControl::kAlt7:
		reg |= PORT_PCR_MUX(7);
		break;
	}

	SetInterruptBit(config.interrupt, &reg);

	if (config.config[PinConfig::ConfigBit::kHighDriveStrength])
	{
		reg |= PORT_PCR_DSE_MASK;
	}
	if (config.config[PinConfig::ConfigBit::kOpenDrain])
	{
		reg |= PORT_PCR_ODE_MASK;
	}
	if (config.config[PinConfig::ConfigBit::kPassiveFilter])
	{
		reg |= PORT_PCR_PFE_MASK;
	}
	if (config.config[PinConfig::ConfigBit::kSlowSlewRate])
	{
		reg |= PORT_PCR_SRE_MASK;
	}
	if (config.config[PinConfig::ConfigBit::kPullEnable])
	{
		reg |= PORT_PCR_PE_MASK;
		if (config.config[PinConfig::ConfigBit::kPullUp])
		{
			reg |= PORT_PCR_PS_MASK;
		}
	}

	MEM_MAPS[PinUtils::GetPort(config.pin)]
			->PCR[PinUtils::GetPinNumber(config.pin)] = reg;

	m_name = config.pin;
}

Pin::Pin(Pin &&rhs)
		: Pin(nullptr)
{
	*this = std::move(rhs);
}

Pin::Pin(nullptr_t)
		: m_name(PinConfig::Name::DISABLE)
{}

Pin::~Pin()
{
	Uninit();
}

Pin& Pin::operator=(Pin &&rhs)
{
	if (this != &rhs)
	{
		Uninit();
		const PinConfig::Name name = rhs.m_name;
		rhs.m_name = PinConfig::Name::DISABLE;
		if (name != PinConfig::Name::DISABLE)
		{
			m_name = rhs.m_name;
		}
	}
	return *this;
}

void Pin::Uninit()
{
	if (m_name != PinConfig::Name::DISABLE)
	{
		MEM_MAPS[PinUtils::GetPort(m_name)]
				->PCR[PinUtils::GetPinNumber(m_name)] = 0 | PORT_PCR_MUX(0);
	}
}

void Pin::SetInterrupt(const PinConfig::Interrupt config)
{
	if (m_name == PinConfig::Name::DISABLE)
	{
		assert(false);
		return;
	}

	uint32_t irqc = 0;
	SetInterruptBit(config, &irqc);
	uint32_t reg = MEM_MAPS[PinUtils::GetPort(m_name)]
			->PCR[PinUtils::GetPinNumber(m_name)];
	const bool curr_irqc = reg & PORT_PCR_IRQC_MASK;
	if (irqc != curr_irqc)
	{
		reg &= ~PORT_PCR_IRQC_MASK;
		reg |= irqc;
		MEM_MAPS[PinUtils::GetPort(m_name)]
				->PCR[PinUtils::GetPinNumber(m_name)] = reg;
	}
}

bool Pin::IsInterruptRequested() const
{
	if (m_name == PinConfig::Name::DISABLE)
	{
		assert(false);
		return false;
	}

	return GET_BIT(MEM_MAPS[PinUtils::GetPort(m_name)]->ISFR,
			PinUtils::GetPinNumber(m_name));
}

void Pin::ConsumeInterrupt()
{
	if (m_name == PinConfig::Name::DISABLE)
	{
		assert(false);
		return;
	}

	SET_BIT(MEM_MAPS[PinUtils::GetPort(m_name)]->ISFR,
			PinUtils::GetPinNumber(m_name));
}

}
}
