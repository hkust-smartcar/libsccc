/*
 * uart.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
*/

#include "libbase/kl26/hardware.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cmath>

#include <algorithm>
#include <bitset>
#include <functional>
#include <vector>

#include "libbase/log.h"
#include "libbase/kl26/clock_utils.h"
#include "libbase/kl26/pin.h"
#include "libbase/kl26/pinout.h"
#include "libbase/kl26/sim.h"
#include "libbase/kl26/uart.h"
#include "libbase/kl26/uart_utils.h"
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

/*
 * Beware! UART0 does NOT share the same register layout as UART1/2, and only
 * registers before D (inclusive) could be accessed this way
 */
constexpr UART_Type* MEM_MAPS[PINOUT::GetUartCount()] = {(UART_Type*)UART0,
		UART1, UART2};

Uart* g_instances[PINOUT::GetUartCount()] = {};

uint32_t GetBaudRate(Uart::Config::BaudRate br)
{
	switch (br)
	{
	case Uart::Config::BaudRate::k2400:
		return 2400;

	case Uart::Config::BaudRate::k4800:
		return 4800;

	case Uart::Config::BaudRate::k9600:
		return 9600;

	case Uart::Config::BaudRate::k14400:
		return 14400;

	case Uart::Config::BaudRate::k19200:
		return 19200;

	case Uart::Config::BaudRate::k28800:
		return 28800;

	case Uart::Config::BaudRate::k38400:
		return 38400;

	case Uart::Config::BaudRate::k57600:
		return 57600;

	case Uart::Config::BaudRate::k76800:
		return 76800;

	default:
		assert(false);
		// no break

	case Uart::Config::BaudRate::k115200:
		return 115200;

	case Uart::Config::BaudRate::k230400:
		return 230400;

	case Uart::Config::BaudRate::k460800:
		return 460800;
	}
}

}

Uart::Uart(const Config &config)
		: m_rx(nullptr),
		  m_tx(nullptr),
		  m_is_init(false)
{
	if (!InitModule(config.rx_pin, config.tx_pin) || g_instances[m_module])
	{
		assert(false);
		return;
	}

	m_is_init = true;

	Sim::SetUart0ClockSource(Sim::UartClock::kPll);
	Sim::SetEnableClockGate(EnumAdvance(Sim::ClockGate::kUart0, m_module), true);
	// Disable UART
	MEM_MAPS[m_module]->C2 = 0;

	g_instances[m_module] = this;
	InitPin(config);
	InitBdhReg(config);
	InitBaudRate(config.baud_rate);
	InitC1Reg(config);
	if (m_module == 0)
	{
		InitUart0C4Reg();
	}
	InitInterrupt(config);

	if (m_module == 0)
	{
		// Write one clear all flags
		MEM_MAPS[m_module]->S1 = 0xFF;
	}
	else
	{
		if (MEM_MAPS[m_module]->S1)
		{
			if (MEM_MAPS[m_module]->D)
			{
				asm("nop");
			}
		}
	}
	// Enable UART
	MEM_MAPS[m_module]->C2 |= UART_C2_TE_MASK | UART_C2_RE_MASK;
}

Uart::Uart(Uart &&rhs)
		: Uart(nullptr)
{
	*this = std::move(rhs);
}

Uart::Uart(nullptr_t)
		: m_module(0),
		  m_rx(nullptr),
		  m_tx(nullptr),
		  m_is_init(false)
{}

Uart::~Uart()
{
	Uninit();
}

Uart& Uart::operator=(Uart &&rhs)
{
	if (this != &rhs)
	{
		Uninit();
		if (rhs)
		{
			rhs.m_is_init = false;

			m_module = rhs.m_module;
			m_rx = std::move(rhs.m_rx);
			// Copy instead of move to prevent race condition
			m_rx_isr = rhs.m_rx_isr;
			m_tx = std::move(rhs.m_tx);
			// Copy instead of move to prevent race condition
			m_tx_isr = rhs.m_tx_isr;

			m_is_init = true;
			g_instances[m_module] = this;
		}
	}
	return *this;
}

bool Uart::InitModule(const Pin::Name rx_pin, const Pin::Name tx_pin)
{
	const Name rx = PINOUT::GetUart(rx_pin);
	const int rx_module = UartUtils::GetUartModule(rx);

	const Name tx = PINOUT::GetUart(tx_pin);
	const int tx_module = UartUtils::GetUartModule(tx);

	if (rx_module == tx_module && rx != Name::kDisable)
	{
		m_module = rx_module;
		return true;
	}
	else
	{
		return false;
	}
}

void Uart::InitPin(const Config &config)
{
	Pin::Config rx_config, tx_config;
	rx_config.pin = config.rx_pin;
	rx_config.mux = PINOUT::GetUartMux(config.rx_pin);
	rx_config.config = config.rx_config;

	tx_config.pin = config.tx_pin;
	tx_config.mux = PINOUT::GetUartMux(config.tx_pin);
	tx_config.config = config.tx_config;

	m_rx = Pin(rx_config);
	m_tx = Pin(tx_config);
}

void Uart::InitBdhReg(const Config &config)
{
	uint8_t reg = 0;
	if (config.config[Config::ConfigBit::kTwoStopBit])
	{
		SET_BIT(reg, UART_BDH_SBNS_SHIFT);
	}

	MEM_MAPS[m_module]->BDH = reg;
}

void Uart::InitBaudRate(const Config::BaudRate br)
{
	const uint8_t multiplier = Sim::GetCoreClockDivider();
	// PLL clock is divided by 2 before feeding us
	const uint32_t clock = ((m_module == 0)
			? ClockUtils::GetCoreClock() * multiplier / 2
			: ClockUtils::GetBusClock()) / 16;
	const Uint target_baud = GetBaudRate(br);

	int sbr = clock / target_baud;
	const Uint max_baud = clock / sbr;
	const Uint min_baud = clock / (sbr + 1);
	if (max_baud - target_baud > target_baud - min_baud)
	{
		++sbr;
	}
	assert(sbr >= 1 && sbr <= 8191);

	MEM_MAPS[m_module]->BDH |= UART_BDH_SBR(sbr >> 8);
	MEM_MAPS[m_module]->BDL = UART_BDL_SBR(sbr);
}

void Uart::InitC1Reg(const Config &config)
{
	uint8_t reg = 0;
	if (config.config[Config::ConfigBit::kEnableEvenParity])
	{
		SET_BIT(reg, UART_C1_PE_MASK);
	}
	SetLoopMode(config.config[Config::ConfigBit::kLoopMode]);

	MEM_MAPS[m_module]->C1 = reg;
}

void Uart::InitUart0C4Reg()
{
	// Set to 15 to match UART1/2 when calculating the baudrate
	UART0->C4 = UART0_C4_OSR(15);
}

void Uart::InitInterrupt(const Config &config)
{
	m_rx_isr = config.rx_isr;
	m_tx_isr = config.tx_isr;

	SetInterrupt((bool)m_rx_isr, (bool)m_tx_isr);
}

void Uart::Uninit()
{
	if (m_is_init)
	{
		m_is_init = false;

		// Disable Rx, Txand IRQ
		MEM_MAPS[m_module]->C2 = 0;
		SetInterrupt(false, false);

		Sim::SetEnableClockGate(EnumAdvance(Sim::ClockGate::kUart0, m_module),
				false);
		g_instances[m_module] = nullptr;
	}
}

void Uart::SetLoopMode(const bool flag)
{
	STATE_GUARD(Uart, VOID);

	if (flag)
	{
		SET_BIT(MEM_MAPS[m_module]->C1, UART_C1_LOOPS_SHIFT);
	}
	else
	{
		CLEAR_BIT(MEM_MAPS[m_module]->C1, UART_C1_LOOPS_SHIFT);
	}
}

void Uart::SetInterrupt(const bool rx_flag, const bool tx_flag)
{
	// If we init the interrupt here, Tx isr will be called immediately which
	// may not be intended
	SetEnableRxIrq(false);
	SetEnableTxIrq(false);

	if (rx_flag || tx_flag)
	{
		SetIsr(EnumAdvance(UART0_IRQn, m_module), IrqHandler);
		EnableIrq(EnumAdvance(UART0_IRQn, m_module));
	}
	else
	{
		DisableIrq(EnumAdvance(UART0_IRQn, m_module));
		SetIsr(EnumAdvance(UART0_IRQn, m_module), nullptr);
	}
}

void Uart::SetEnableRxIrq(const bool flag)
{
	STATE_GUARD(Uart, VOID);

	if (flag)
	{
		SET_BIT(MEM_MAPS[m_module]->C2, UART_C2_RIE_SHIFT);
	}
	else
	{
		CLEAR_BIT(MEM_MAPS[m_module]->C2, UART_C2_RIE_SHIFT);
	}
}

void Uart::SetEnableTxIrq(const bool flag)
{
	STATE_GUARD(Uart, VOID);

	if (flag)
	{
		SET_BIT(MEM_MAPS[m_module]->C2, UART_C2_TIE_SHIFT);
	}
	else
	{
		CLEAR_BIT(MEM_MAPS[m_module]->C2, UART_C2_TIE_SHIFT);
	}
}

Byte Uart::GetByte() const
{
	STATE_GUARD(Uart, 0);

	while (!GET_BIT(MEM_MAPS[m_module]->S1, UART_S1_RDRF_SHIFT))
	{}
	SET_BIT(MEM_MAPS[m_module]->S1, UART_S1_OR_SHIFT);
	return MEM_MAPS[m_module]->D;
}

bool Uart::PeekByte(Byte *out_byte) const
{
	STATE_GUARD(Uart, false);

	if (GET_BIT(MEM_MAPS[m_module]->S1, UART_S1_RDRF_SHIFT))
	{
		SET_BIT(MEM_MAPS[m_module]->S1, UART_S1_OR_SHIFT);
		*out_byte = MEM_MAPS[m_module]->D;
		return true;
	}
	else
	{
		return false;
	}
}

void Uart::SendByte(const Byte byte)
{
	STATE_GUARD(Uart, VOID);

	while (!GET_BIT(MEM_MAPS[m_module]->S1, UART_S1_TDRE_SHIFT))
	{}
	MEM_MAPS[m_module]->D = byte;
}

bool Uart::PutByte(const Byte byte)
{
	STATE_GUARD(Uart, false);

	if (GET_BIT(MEM_MAPS[m_module]->S1, UART_S1_TDRE_SHIFT))
	{
		MEM_MAPS[m_module]->D = byte;
		return true;
	}
	else
	{
		return false;
	}
}

__ISR void Uart::IrqHandler()
{
	const int module = GetActiveIrq() - UART0_IRQn;
	Uart *const that = g_instances[module];
	if (!that || !(*that))
	{
		// Something wrong?
		assert(false);
		that->SetEnableRxIrq(false);
		that->SetEnableTxIrq(false);
		return;
	}

	if (GET_BIT(MEM_MAPS[module]->C2, UART_C2_RIE_SHIFT)
			&& GET_BIT(MEM_MAPS[module]->S1, UART_S1_RDRF_SHIFT))
	{
		if (that->m_rx_isr)
		{
			that->m_rx_isr(that);
		}
		else
		{
			// what?
			that->SetEnableRxIrq(false);
		}
	}

	if (GET_BIT(MEM_MAPS[module]->C2, UART_C2_TIE_SHIFT)
			&& GET_BIT(MEM_MAPS[module]->S1, UART_S1_TDRE_SHIFT))
	{
		if (that->m_tx_isr)
		{
			that->m_tx_isr(that);
		}
		else
		{
			// what?
			that->SetEnableTxIrq(false);
		}
	}
}

}
}

