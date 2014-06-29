/*
 * pin.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <hw_common.h>

#include <cstdint>

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/pin_utils.h"

namespace libbase
{
namespace k60
{

namespace
{

constexpr PORT_MemMapPtr MEM_MAP[5] = {PORTA_BASE_PTR, PORTB_BASE_PTR,
		PORTC_BASE_PTR, PORTD_BASE_PTR, PORTE_BASE_PTR};

void SetInterruptBit(const PinConfig::Interrupt config, uint32_t *reg)
{
	switch (config)
	{
	case PinConfig::Interrupt::DISABLE:
		break;

	case PinConfig::Interrupt::DMA_RISING:
		*reg |= PORT_PCR_IRQC(0x1);
		break;

	case PinConfig::Interrupt::DMA_FALLING:
		*reg |= PORT_PCR_IRQC(0x2);
		break;

	case PinConfig::Interrupt::DMA_BOTH:
		*reg |= PORT_PCR_IRQC(0x3);
		break;

	case PinConfig::Interrupt::LOW:
		*reg |= PORT_PCR_IRQC(0x8);
		break;

	case PinConfig::Interrupt::RISING:
		*reg |= PORT_PCR_IRQC(0x9);
		break;

	case PinConfig::Interrupt::FALLING:
		*reg |= PORT_PCR_IRQC(0xA);
		break;

	case PinConfig::Interrupt::BOTH:
		*reg |= PORT_PCR_IRQC(0xB);
		break;

	case PinConfig::Interrupt::HIGH:
		*reg |= PORT_PCR_IRQC(0xC);
		break;
	}
}

}

Pin::Pin(const PinConfig &config)
{
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK << PinUtils::GetPort(config.pin);
	uint32_t reg = 0;

	switch (config.mux)
	{
	case PinConfig::MuxControl::DISABLE:
		reg |= PORT_PCR_MUX(0);
		break;

	case PinConfig::MuxControl::GPIO:
		reg |= PORT_PCR_MUX(1);
		break;

	case PinConfig::MuxControl::ALT2:
		reg |= PORT_PCR_MUX(2);
		break;

	case PinConfig::MuxControl::ALT3:
		reg |= PORT_PCR_MUX(3);
		break;

	case PinConfig::MuxControl::ALT4:
		reg |= PORT_PCR_MUX(4);
		break;

	case PinConfig::MuxControl::ALT5:
		reg |= PORT_PCR_MUX(5);
		break;

	case PinConfig::MuxControl::ALT6:
		reg |= PORT_PCR_MUX(6);
		break;

	case PinConfig::MuxControl::ALT7:
		reg |= PORT_PCR_MUX(7);
		break;
	}

	SetInterruptBit(config.interrupt, &reg);

	if (config.config.test(PinConfig::ConfigBit::HIGH_DRIVE_STRENGTH))
	{
		reg |= PORT_PCR_DSE_MASK;
	}
	if (config.config.test(PinConfig::ConfigBit::OPEN_DRAIN))
	{
		reg |= PORT_PCR_ODE_MASK;
	}
	if (config.config.test(PinConfig::ConfigBit::PASSIVE_FILTER))
	{
		reg |= PORT_PCR_PFE_MASK;
	}
	if (config.config.test(PinConfig::ConfigBit::SLOW_SLEW_RATE))
	{
		reg |= PORT_PCR_SRE_MASK;
	}
	if (config.config.test(PinConfig::ConfigBit::PULL_ENABLE))
	{
		reg |= PORT_PCR_PE_MASK;
		if (config.config.test(PinConfig::ConfigBit::PULL_UP))
		{
			reg |= PORT_PCR_PS_MASK;
		}
	}

	MEM_MAP[PinUtils::GetPort(config.pin)]
			->PCR[PinUtils::GetPinNumber(config.pin)] = reg;

	m_name = config.pin;
}

Pin::Pin(Pin &&rhs)
		: m_name(rhs.m_name)
{}

Pin::Pin(nullptr_t)
{}

Pin::~Pin()
{}

Pin& Pin::operator=(Pin &&rhs)
{
	m_name = rhs.m_name;
	return *this;
}

void Pin::SetInterrupt(const PinConfig::Interrupt config)
{
	uint32_t irqc = 0;
	SetInterruptBit(config, &irqc);
	uint32_t reg = MEM_MAP[PinUtils::GetPort(m_name)]
			->PCR[PinUtils::GetPinNumber(m_name)];
	const bool curr_irqc = reg & PORT_PCR_IRQC_MASK;
	if (irqc != curr_irqc)
	{
		reg &= ~PORT_PCR_IRQC_MASK;
		reg |= irqc;
		MEM_MAP[PinUtils::GetPort(m_name)]
				->PCR[PinUtils::GetPinNumber(m_name)] = reg;
	}
}

bool Pin::IsInterruptRequested() const
{
	return GET_BIT(PORT_ISFR_REG(MEM_MAP[PinUtils::GetPort(m_name)]),
			PinUtils::GetPinNumber(m_name));
}

void Pin::ConsumeInterrupt()
{
	SET_BIT(PORT_ISFR_REG(MEM_MAP[PinUtils::GetPort(m_name)]),
			PinUtils::GetPinNumber(m_name));
}

}
}
