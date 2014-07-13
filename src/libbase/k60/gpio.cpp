/*
 * gpio.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include "libbase/k60/hardware.h"

#include <cassert>

#include "libbase/k60/gpio.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/pin_utils.h"

namespace libbase
{
namespace k60
{

namespace
{

constexpr GPIO_Type* MEM_MAPS[5] = {PTA, PTB, PTC, PTD, PTE};

PinConfig GetPinConfig(const Gpi::Config &config)
{
	PinConfig pin_config;
	pin_config.pin = config.pin;
	pin_config.mux = PinConfig::MuxControl::kGpio;
	pin_config.config = config.config;
	return pin_config;
}

PinConfig GetPinConfig(const Gpo::Config &config)
{
	PinConfig pin_config;
	pin_config.pin = config.pin;
	pin_config.mux = PinConfig::MuxControl::kGpio;
	pin_config.config = config.config;
	return pin_config;
}

}

Gpi::Gpi(const Gpi::Config &config)
		: m_pin(GetPinConfig(config))
{
	RESET_BIT(MEM_MAPS[PinUtils::GetPort(m_pin.GetName())]->PDDR,
			PinUtils::GetPinNumber(m_pin.GetName()));
}

Gpi::Gpi(Gpi &&rhs)
		: m_pin(std::move(rhs.m_pin))
{}

Gpi::Gpi(Pin &&rhs)
		: m_pin(std::move(rhs))
{
	RESET_BIT(MEM_MAPS[PinUtils::GetPort(m_pin.GetName())]->PDDR,
			PinUtils::GetPinNumber(m_pin.GetName()));
}

Gpi::Gpi(nullptr_t)
		: m_pin(nullptr)
{}

Gpi& Gpi::operator=(Gpi &&rhs)
{
	if (this != &rhs)
	{
		m_pin = std::move(rhs.m_pin);
	}
	return *this;
}

bool Gpi::Get() const
{
	assert(!GET_BIT(MEM_MAPS[PinUtils::GetPort(m_pin.GetName())]->PDDR,
			PinUtils::GetPinNumber(m_pin.GetName())));
	return GET_BIT(MEM_MAPS[PinUtils::GetPort(m_pin.GetName())]->PDIR,
			PinUtils::GetPinNumber(m_pin.GetName()));
}

Gpo Gpi::ToGpo()
{
	return Gpo(std::move(m_pin));
}

Gpo::Gpo(const Gpo::Config &config)
		: m_pin(GetPinConfig(config))
{
	SET_BIT(MEM_MAPS[PinUtils::GetPort(m_pin.GetName())]->PDDR,
			PinUtils::GetPinNumber(m_pin.GetName()));
	Set(config.is_high);
}

Gpo::Gpo(Gpo &&rhs)
		: m_pin(std::move(rhs.m_pin))
{}

Gpo::Gpo(Pin &&rhs)
		: m_pin(std::move(rhs))
{
	SET_BIT(MEM_MAPS[PinUtils::GetPort(m_pin.GetName())]->PDDR,
			PinUtils::GetPinNumber(m_pin.GetName()));
}

Gpo::Gpo(nullptr_t)
		: m_pin(nullptr)
{}

Gpo& Gpo::operator=(Gpo &&rhs)
{
	if (this != &rhs)
	{
		m_pin = std::move(rhs.m_pin);
	}
	return *this;
}

void Gpo::Set(bool is_high)
{
	assert(GET_BIT(MEM_MAPS[PinUtils::GetPort(m_pin.GetName())]->PDDR,
			PinUtils::GetPinNumber(m_pin.GetName())));
	if (is_high)
	{
		SET_BIT(MEM_MAPS[PinUtils::GetPort(m_pin.GetName())]->PSOR,
				PinUtils::GetPinNumber(m_pin.GetName()));
	}
	else
	{
		SET_BIT(MEM_MAPS[PinUtils::GetPort(m_pin.GetName())]->PCOR,
				PinUtils::GetPinNumber(m_pin.GetName()));
	}
}

void Gpo::Turn()
{
	assert(GET_BIT(MEM_MAPS[PinUtils::GetPort(m_pin.GetName())]->PDDR,
			PinUtils::GetPinNumber(m_pin.GetName())));
	SET_BIT(MEM_MAPS[PinUtils::GetPort(m_pin.GetName())]->PTOR,
			PinUtils::GetPinNumber(m_pin.GetName()));
}

Gpi Gpo::ToGpi()
{
	return Gpi(std::move(m_pin));
}

Gpio::Gpio(const Gpi::Config &config)
		: m_gpi(config), m_gpo(nullptr), m_is_gpo(false)
{}

Gpio::Gpio(const Gpo::Config &config)
		: m_gpi(nullptr), m_gpo(config), m_is_gpo(true)
{}

Gpio::Gpio(nullptr_t)
		: m_gpi(nullptr), m_gpo(nullptr), m_is_gpo(true)
{}

void Gpio::EnsureGpi()
{
	if (m_is_gpo)
	{
		m_gpi = m_gpo.ToGpi();
	}
}

void Gpio::EnsureGpo()
{
	if (!m_is_gpo)
	{
		m_gpo = m_gpi.ToGpo();
	}
}

}
}
