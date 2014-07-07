/*
 * uart.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <hw_common.h>

#include <cassert>
#include <cstdint>
#include <cmath>

#include <algorithm>
#include <bitset>
#include <vector>

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/uart.h"
#include "libutil/misc.h"

using namespace std;

namespace libbase
{
namespace k60
{

namespace
{

constexpr UART_MemMapPtr MEM_MAP[6] = {UART0_BASE_PTR, UART1_BASE_PTR,
		UART2_BASE_PTR, UART3_BASE_PTR, UART4_BASE_PTR, UART5_BASE_PTR};

Uart* g_instances[6] = {};

uint32_t GetBaudRate(Uart::Config::BaudRate br)
{
	switch (br)
	{
	case Uart::Config::BaudRate::BR_2400:
		return 2400;

	case Uart::Config::BaudRate::BR_4800:
		return 4800;

	case Uart::Config::BaudRate::BR_9600:
		return 9600;

	case Uart::Config::BaudRate::BR_14400:
		return 14400;

	case Uart::Config::BaudRate::BR_19200:
		return 19200;

	case Uart::Config::BaudRate::BR_28800:
		return 28800;

	case Uart::Config::BaudRate::BR_38400:
		return 38400;

	case Uart::Config::BaudRate::BR_57600:
		return 57600;

	case Uart::Config::BaudRate::BR_76800:
		return 76800;

	case Uart::Config::BaudRate::BR_115200:
		return 115200;

	case Uart::Config::BaudRate::BR_230400:
		return 230400;

	case Uart::Config::BaudRate::BR_460800:
		return 460800;

	default:
		return 0;
	}
}

uint8_t GetFifoSize(const uint8_t buf_depth_reg)
{
	switch (buf_depth_reg)
	{
	default:
	case 0x0:
		return 1;
	case 0x1:
		return 4;
	case 0x2:
		return 8;
	case 0x3:
		return 16;
	case 0x4:
		return 32;
	case 0x5:
		return 64;
	case 0x6:
		return 128;
	}
}

}

Uart::Uart(const Config &config)
		: m_is_fifo(false), m_tx(nullptr), m_tx_fifo_size(1), m_tx_isr(nullptr),
		  m_rx(nullptr), m_rx_fifo_size(1), m_rx_isr(nullptr)
{
	if (!InitModule(config.tx_pin, config.rx_pin))
	{
		assert(false);
	}

	SetEnableClockGate(true);
	// Disable UART
	MEM_MAP[m_module]->C2 = 0;

	g_instances[m_module] = this;
	InitBaudRate(config.baud_rate);
	InitPin(config.tx_pin, config.rx_pin);

	InitC1Reg(config);
	InitFifo(config);
	InitInterrupt(config);
	MEM_MAP[m_module]->C2 |= UART_C2_TE_MASK | UART_C2_RE_MASK;
}

Uart::~Uart()
{
	// Disable UART
	MEM_MAP[m_module]->C2 = 0;
	SetEnableClockGate(false);
}

bool Uart::InitModule(const PinConfig::Name tx_pin, const PinConfig::Name rx_pin)
{
	int tx_module = -1;
	switch (tx_pin)
	{
	case PinConfig::Name::PTA2:
	case PinConfig::Name::PTA14:
	case PinConfig::Name::PTB17:
	case PinConfig::Name::PTD7:
		tx_module = 0;
		break;

	case PinConfig::Name::PTC4:
	case PinConfig::Name::PTE0:
		tx_module = 1;
		break;

	case PinConfig::Name::PTD3:
		tx_module = 2;
		break;

	case PinConfig::Name::PTB11:
	case PinConfig::Name::PTC17:
	case PinConfig::Name::PTE4:
		tx_module = 3;
		break;

	case PinConfig::Name::PTC15:
	case PinConfig::Name::PTE24:
		tx_module = 4;
		break;

	case PinConfig::Name::PTD9:
	case PinConfig::Name::PTE8:
		tx_module = 5;
		break;

	default:
		break;
	}

	int rx_module = -1;
	switch (rx_pin)
	{
	case PinConfig::Name::PTA1:
	case PinConfig::Name::PTA15:
	case PinConfig::Name::PTB16:
	case PinConfig::Name::PTD6:
		rx_module = 0;
		break;

	case PinConfig::Name::PTC3:
	case PinConfig::Name::PTE1:
		rx_module = 1;
		break;

	case PinConfig::Name::PTD2:
		rx_module = 2;
		break;

	case PinConfig::Name::PTB10:
	case PinConfig::Name::PTC16:
	case PinConfig::Name::PTE5:
		rx_module = 3;
		break;

	case PinConfig::Name::PTC14:
	case PinConfig::Name::PTE25:
		rx_module = 4;
		break;

	case PinConfig::Name::PTD8:
	case PinConfig::Name::PTE9:
		rx_module = 5;
		break;

	default:
		break;
	}

	if (tx_module == rx_module && tx_module != -1)
	{
		m_module = (Module)tx_module;
		return true;
	}
	else
	{
		return false;
	}
}

void Uart::SetEnableClockGate(const bool flag)
{
	volatile uint32_t *sim = nullptr;
	int shift = 0;
	switch (m_module)
	{
	case Uart::Module::UART0:
		sim = &SIM_SCGC4;
		shift = SIM_SCGC4_UART0_SHIFT;
		break;

	case Uart::Module::UART1:
		sim = &SIM_SCGC4;
		shift = SIM_SCGC4_UART1_SHIFT;
		break;

	case Uart::Module::UART2:
		sim = &SIM_SCGC4;
		shift = SIM_SCGC4_UART2_SHIFT;
		break;

	case Uart::Module::UART3:
		sim = &SIM_SCGC4;
		shift = SIM_SCGC4_UART3_SHIFT;
		break;

	case Uart::Module::UART4:
		sim = &SIM_SCGC1;
		shift = SIM_SCGC1_UART4_SHIFT;
		break;

	case Uart::Module::UART5:
		sim = &SIM_SCGC1;
		shift = SIM_SCGC1_UART5_SHIFT;
		break;
	}

	if (flag)
	{
		SET_BIT(*sim, shift);
	}
	else
	{
		CLEAR_BIT(*sim, shift);
	}
}

void Uart::InitBaudRate(const Config::BaudRate br)
{
	const uint32_t clock = ((m_module < 2) ? core_clk_khz : bus_clk_khz) * 1000;
	const float target = clock / (float)GetBaudRate(br) / 16.0f;
	const int sbr = static_cast<int>(target);
	assert(sbr <= 0x1FFF);

	UART_MemMapPtr uart_ptr = MEM_MAP[m_module];
	uart_ptr->BDH = (sbr >> 8) & 0x1F;
	uart_ptr->BDL = sbr & 0xFF;

	float null;
	const int brfa = roundf(modf(target, &null) * 32);
	uart_ptr->C4 &= ~UART_C4_BRFA_MASK;
	uart_ptr->C4 |= brfa;
}

void Uart::InitPin(const PinConfig::Name tx_pin, const PinConfig::Name rx_pin)
{
	PinConfig tx_config, rx_config;
	tx_config.pin = tx_pin;
	rx_config.pin = rx_pin;
	if (m_module == 0)
	{
		tx_config.mux = (tx_pin == PinConfig::Name::PTA2)
				? PinConfig::MuxControl::ALT2 : PinConfig::MuxControl::ALT3;
		rx_config.mux = (rx_pin == PinConfig::Name::PTA1)
				? PinConfig::MuxControl::ALT2 : PinConfig::MuxControl::ALT3;
	}
	else
	{
		tx_config.mux = PinConfig::MuxControl::ALT3;
		rx_config.mux = PinConfig::MuxControl::ALT3;
	}

	m_tx = Pin(tx_config);
	m_rx = Pin(rx_config);
}

void Uart::InitC1Reg(const Config &config)
{
	UART_MemMapPtr uart_ptr = MEM_MAP[m_module];
	uart_ptr->C1 = 0;
	SetLoopMode(config.config.test(Config::ConfigBit::LOOP_MODE));
	if (config.config.test(Config::ConfigBit::ENABLE_EVEN_PARITY))
	{
		SET_BIT(uart_ptr->C1, UART_C1_PE_MASK);
	}
}

void Uart::InitFifo(const Config &config)
{
	UART_MemMapPtr uart_ptr = MEM_MAP[m_module];
	uart_ptr->CFIFO = 0;
	m_is_fifo = config.config.test(Config::ConfigBit::FIFO);
	if (m_is_fifo)
	{
		SET_BIT(uart_ptr->PFIFO, UART_PFIFO_TXFE_SHIFT);
		m_tx_fifo_size = GetFifoSize((uart_ptr->PFIFO
				& UART_PFIFO_TXFIFOSIZE_MASK) >> UART_PFIFO_TXFIFOSIZE_SHIFT);
		uint8_t twfifo = config.tx_irq_threshold;
		if (config.is_tx_irq_threshold_percentage)
		{
			twfifo = std::min<uint8_t>(twfifo, 100);
			twfifo = m_tx_fifo_size * (twfifo / 100.0f);
		}
		uart_ptr->TWFIFO = libutil::Clamp<uint8_t>(0, twfifo, m_tx_fifo_size - 1);

		SET_BIT(uart_ptr->PFIFO, UART_PFIFO_RXFE_SHIFT);
		m_rx_fifo_size = GetFifoSize((uart_ptr->PFIFO
				& UART_PFIFO_RXFIFOSIZE_MASK) >> UART_PFIFO_RXFIFOSIZE_SHIFT);
		uint8_t rwfifo = config.rx_irq_threshold;
		if (config.is_rx_irq_threshold_percentage)
		{
			rwfifo = std::min<uint8_t>(rwfifo, 100);
			rwfifo = m_tx_fifo_size * (rwfifo / 100.0f);
		}
		uart_ptr->RWFIFO = libutil::Clamp<uint8_t>(1, rwfifo, m_rx_fifo_size);
	}
	else
	{
		CLEAR_BIT(uart_ptr->PFIFO, UART_PFIFO_TXFE_SHIFT);
		CLEAR_BIT(uart_ptr->PFIFO, UART_PFIFO_RXFE_SHIFT);
	}
}

void Uart::InitInterrupt(const Config &config)
{
	m_tx_isr = config.tx_isr;
	m_rx_isr = config.rx_isr;

	if (m_tx_isr || m_rx_isr)
	{
		SetIsr(static_cast<VECTORn_t>(UART0_RX_TX_VECTORn + (m_module << 1)),
				IrqHandler);
		if (m_tx_isr)
		{
			SET_BIT(MEM_MAP[m_module]->C2, UART_C2_TIE_SHIFT);
		}
		if (m_rx_isr)
		{
			SET_BIT(MEM_MAP[m_module]->C2, UART_C2_RIE_SHIFT);
		}
		EnableIsr(static_cast<VECTORn_t>(UART0_RX_TX_VECTORn + (m_module << 1)));
	}
}

void Uart::SetLoopMode(const bool flag)
{
	UART_MemMapPtr uart_ptr = MEM_MAP[m_module];
	if (flag)
	{
		SET_BIT(uart_ptr->C1, UART_C1_LOOPS_SHIFT);
	}
	else
	{
		CLEAR_BIT(uart_ptr->C1, UART_C1_LOOPS_SHIFT);
	}
}

uint8_t Uart::GetAvailableBytes() const
{
	return MEM_MAP[m_module]->RCFIFO;
}

Byte Uart::GetByte() const
{
	// Read S1 to clear RDRF
	while (!GET_BIT(MEM_MAP[m_module]->S1, UART_S1_RDRF_SHIFT)
			&& !GetAvailableBytes())
	{}
	return MEM_MAP[m_module]->D;
}

bool Uart::PeekByte(Byte *out_byte) const
{
	// Read S1 to clear RDRF
	if (GET_BIT(MEM_MAP[m_module]->S1, UART_S1_RDRF_SHIFT) || GetAvailableBytes())
	{
		*out_byte = MEM_MAP[m_module]->D;
		return true;
	}
	else
	{
		return false;
	}
}

vector<Byte> Uart::GetBytes() const
{
	vector<Byte> data;
	while (!PeekBytes(&data))
	{}
	return data;
}

bool Uart::PeekBytes(vector<Byte> *out_bytes) const
{
	const int size = MEM_MAP[m_module]->RCFIFO;
	if (size == 0)
	{
		return false;
	}

	out_bytes->resize(size);
	for (int i = 0; i < size - 1; ++i)
	{
		(*out_bytes)[i] = MEM_MAP[m_module]->D;
	}
	// Read S1 to clear RDRF
	if (GET_BIT(MEM_MAP[m_module]->S1, UART_S1_RDRF_SHIFT)
			|| MEM_MAP[m_module]->RCFIFO > 0)
	{
		(*out_bytes)[size - 1] = MEM_MAP[m_module]->D;
	}
	return true;
}

void Uart::SendByte(const Byte byte)
{
	// Read S1 to clear TDRE
	while (!GET_BIT(MEM_MAP[m_module]->S1, UART_S1_TDRE_SHIFT)
			&& MEM_MAP[m_module]->TCFIFO >= m_tx_fifo_size)
	{}
	MEM_MAP[m_module]->D = byte;
}

bool Uart::PutByte(const Byte byte)
{
	if (GET_BIT(MEM_MAP[m_module]->S1, UART_S1_TDRE_SHIFT))
	{
		MEM_MAP[m_module]->D = byte;
		return true;
	}
	else
	{
		return false;
	}
}

size_t Uart::PutBytes(const Byte *bytes, const size_t size)
{
	const size_t send = std::min<Uint>(
			m_tx_fifo_size - MEM_MAP[m_module]->TCFIFO, size);
	if (send == 0)
	{
		return 0;
	}

	for (size_t i = 0; i < send - 1; ++i)
	{
		MEM_MAP[m_module]->D = bytes[i];
	}
	// Read S1 to clear TDRE
	if (GET_BIT(MEM_MAP[m_module]->S1, UART_S1_TDRE_SHIFT)
			|| MEM_MAP[m_module]->TCFIFO < m_tx_fifo_size)
	{
		MEM_MAP[m_module]->D = bytes[send - 1];
	}
	return send;
}

void Uart::SetEnableTxIrq(const bool flag)
{
	if (flag)
	{
		SET_BIT(MEM_MAP[m_module]->C2, UART_C2_TIE_SHIFT);
	}
	else
	{
		CLEAR_BIT(MEM_MAP[m_module]->C2, UART_C2_TIE_SHIFT);
	}
}

void Uart::SetEnableRxIrq(const bool flag)
{
	if (flag)
	{
		SET_BIT(MEM_MAP[m_module]->C2, UART_C2_RIE_SHIFT);
	}
	else
	{
		CLEAR_BIT(MEM_MAP[m_module]->C2, UART_C2_RIE_SHIFT);
	}
}

__ISR void Uart::IrqHandler()
{
	const VECTORn_t v = GetVectorX();
	const int module = (v - UART0_RX_TX_VECTORn) >> 1;
	if (!g_instances[module])
	{
		// Something wrong?
		assert(false);
		CLEAR_BIT(MEM_MAP[module]->C2, UART_C2_TIE_SHIFT);
		CLEAR_BIT(MEM_MAP[module]->C2, UART_C2_RIE_SHIFT);
		return;
	}

	if (GET_BIT(MEM_MAP[module]->C2, UART_C2_TIE_SHIFT)
			&& GET_BIT(MEM_MAP[module]->S1, UART_S1_TDRE_SHIFT))
	{
		if (g_instances[module]->m_tx_isr)
		{
			g_instances[module]->m_tx_isr(g_instances[module]);
		}
		else
		{
			CLEAR_BIT(MEM_MAP[module]->C2, UART_C2_TIE_SHIFT);
		}
	}

	if (GET_BIT(MEM_MAP[module]->C2, UART_C2_RIE_SHIFT)
			&& GET_BIT(MEM_MAP[module]->S1, UART_S1_RDRF_SHIFT))
	{
		if (g_instances[module]->m_rx_isr)
		{
			g_instances[module]->m_rx_isr(g_instances[module]);
		}
		else
		{
			CLEAR_BIT(MEM_MAP[module]->C2, UART_C2_RIE_SHIFT);
		}
	}
}

}
}
