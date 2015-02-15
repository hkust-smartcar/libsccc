/*
 * pin.cpp
 *
 *  Created on: Feb 10, 2015
 *      Author: harrison
 */
#include <cassert>
#include <cstdint>

#include "libbase/kl26/hardware.h"
#include "libbase/kl26/pin.h"
#include "libbase/kl26/pinout.h"
#include "libbase/kl26/sim.h"

#include "libbase/kl26/pin_utils.h"
#include "libutil/misc.h"

#define PORT_COUNT 5

using namespace libutil;

namespace libbase{
namespace kl26{


namespace
{

constexpr PORT_Type* MEM_MAPS[PORT_COUNT] = {PORTA, PORTB, PORTC, PORTD, PORTE};

void SetInterruptBit(const Pin::Config::Interrupt config, uint32_t *reg)
{
	switch (config)
	{
	case Pin::Config::Interrupt::kDisable:
		break;

	case Pin::Config::Interrupt::kDmaRising:
		*reg |= PORT_PCR_IRQC(0x1);
		break;

	case Pin::Config::Interrupt::kDmaFalling:
		*reg |= PORT_PCR_IRQC(0x2);
		break;

	case Pin::Config::Interrupt::kDmaBoth:
		*reg |= PORT_PCR_IRQC(0x3);
		break;

	case Pin::Config::Interrupt::kLow:
		*reg |= PORT_PCR_IRQC(0x8);
		break;

	case Pin::Config::Interrupt::kRising:
		*reg |= PORT_PCR_IRQC(0x9);
		break;

	case Pin::Config::Interrupt::kFalling:
		*reg |= PORT_PCR_IRQC(0xA);
		break;

	case Pin::Config::Interrupt::kBoth:
		*reg |= PORT_PCR_IRQC(0xB);
		break;

	case Pin::Config::Interrupt::kHigh:
		*reg |= PORT_PCR_IRQC(0xC);
		break;
	}
}

}

Pin::Pin(const Config &config)
		: m_name(Name::kDisable)
{
	if (config.pin == Name::kDisable || !PINOUT::RegPin(config.pin))
	{
		assert(false);
		return;
	}

	Sim::SetEnableClockGate(EnumAdvance(Sim::ClockGate::kPortA, PinUtils::GetPort(config.pin)), true);
	uint32_t reg = 0;

	switch (config.mux)
	{
	case Config::MuxControl::kAnalog:
		reg |= PORT_PCR_MUX(0);
		break;

	case Config::MuxControl::kGpio:
		reg |= PORT_PCR_MUX(1);
		break;

	case Config::MuxControl::kAlt2:
		reg |= PORT_PCR_MUX(2);
		break;

	case Config::MuxControl::kAlt3:
		reg |= PORT_PCR_MUX(3);
		break;

	case Config::MuxControl::kAlt4:
		reg |= PORT_PCR_MUX(4);
		break;

	case Config::MuxControl::kAlt5:
		reg |= PORT_PCR_MUX(5);
		break;

	case Config::MuxControl::kAlt6:
		reg |= PORT_PCR_MUX(6);
		break;

	case Config::MuxControl::kAlt7:
		reg |= PORT_PCR_MUX(7);
		break;
	}

	SetInterruptBit(config.interrupt, &reg);

	if (config.config[Config::ConfigBit::kHighDriveStrength])
	{
		reg |= PORT_PCR_DSE_MASK;
	}
/*	if (config.config[Config::ConfigBit::kOpenDrain])
	{
		reg |= PORT_PCR_ODE_MASK;
	}*/
	if (config.config[Config::ConfigBit::kPassiveFilter])
	{
		reg |= PORT_PCR_PFE_MASK;
	}
	if (config.config[Config::ConfigBit::kSlowSlewRate])
	{
		reg |= PORT_PCR_SRE_MASK;
	}
	if (config.config[Config::ConfigBit::kPullEnable])
	{
		reg |= PORT_PCR_PE_MASK;
		if (config.config[Config::ConfigBit::kPullUp])
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
		: m_name(Name::kDisable)
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
		if (rhs)
		{
			const Name name = rhs.m_name;
			rhs.m_name = Name::kDisable;

			m_name = name;
		}
	}
	return *this;
}

void Pin::Uninit()
{
	if (m_name != Name::kDisable)
	{
		const Name name = m_name;
		m_name = Name::kDisable;

		MEM_MAPS[PinUtils::GetPort(name)]
				->PCR[PinUtils::GetPinNumber(name)] = 0 | PORT_PCR_MUX(0);
		PINOUT::UnregPin(name);
	}
}

void Pin::SetInterrupt(const Config::Interrupt config)
{
	STATE_GUARD(Pin, VOID);

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
	STATE_GUARD(Pin, false);

	return IsInterruptRequested(m_name);
}

void Pin::ConsumeInterrupt()
{
	STATE_GUARD(Pin, VOID);

	ConsumeInterrupt(m_name);
}

bool Pin::IsInterruptRequested(const Pin::Name pin)
{
	return GET_BIT(MEM_MAPS[PinUtils::GetPort(pin)]->ISFR,
			PinUtils::GetPinNumber(pin));
}

void Pin::ConsumeInterrupt(const Pin::Name pin)
{
	SET_BIT(MEM_MAPS[PinUtils::GetPort(pin)]->ISFR,
			PinUtils::GetPinNumber(pin));
}

}
}


