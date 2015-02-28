/*
 * spi_master.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstddef>
#include <cstdint>

#include <algorithm>

#include "libbase/log.h"
#include "libbase/k60/clock_utils.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/pinout.h"
#include "libbase/k60/sim.h"
#include "libbase/k60/spi_master.h"
#include "libbase/k60/spi_utils.h"
#include "libbase/k60/vectors.h"

#include "libutil/misc.h"

using namespace libutil;
using namespace std;

#define TX_FIFO_SIZE 4

namespace libbase
{
namespace k60
{

namespace
{

constexpr SPI_Type* MEM_MAPS[PINOUT::GetSpiCount()] = {SPI0, SPI1, SPI2};

SpiMaster* g_instances[PINOUT::GetSpiCount()] = {};

class DelayScalerCalculator
{
public:
	void Calc(const uint32_t delay_ns);

	Uint GetBestPrescaler() const
	{
		return m_best_prescaler;
	}

	Uint GetBestScaler() const
	{
		return m_best_scaler;
	}

private:
	Uint m_best_prescaler;
	Uint m_best_scaler;
};

void DelayScalerCalculator::Calc(const uint32_t delay_ns)
{
	const Uint prescalers[] = {1, 3, 5, 7};
	const uint32_t bus_mhz = ClockUtils::GetBusClockMhz();

	m_best_prescaler = m_best_scaler = 0;
	Uint min_diff = static_cast<Uint>(-1);
	for (Uint i = 0; i < sizeof(prescalers) / sizeof(Uint); ++i)
	{
		for (Uint j = 0; j < 0x10; ++j)
		{
			const uint32_t this_delay = (prescalers[i] * (0x10 << j)) * 1000
					/ bus_mhz;
			const Uint this_diff = abs((int32_t)(this_delay - delay_ns));
			if (this_diff < min_diff)
			{
				min_diff = this_diff;
				m_best_prescaler = i;
				m_best_scaler = j;
			}

			if (min_diff == 0)
			{
				break;
			}
		}
	}
}

inline void SetHalt_(const uint8_t module, const bool flag)
{
	if (flag)
	{
		SET_BIT(MEM_MAPS[module]->MCR, SPI_MCR_HALT_SHIFT);
	}
	else
	{
		CLEAR_BIT(MEM_MAPS[module]->MCR, SPI_MCR_HALT_SHIFT);
	}
}

}

SpiMaster::SpiMaster(const Config &config)
		: m_sin(nullptr),
		  m_sout(nullptr),
		  m_sck(nullptr),

		  m_is_init(false)
{
	if (!InitModule(config) || g_instances[m_module])
	{
		assert(false);
		return;
	}

	m_is_init = true;
	g_instances[m_module] = this;

#if MK60DZ10 || MK60D10
	Sim::SetEnableClockGate(EnumAdvance(Sim::ClockGate::kSpi0, m_module), true);
#elif MK60F15
	Sim::SetEnableClockGate(EnumAdvance(Sim::ClockGate::kDspi0, m_module), true);
#endif
	InitPin(config);
	InitMcrReg(config);
	InitCtarReg(config);

	CLEAR_BIT(MEM_MAPS[m_module]->MCR, SPI_MCR_HALT_SHIFT);
}

SpiMaster::SpiMaster(SpiMaster &&rhs)
		: SpiMaster(nullptr)
{
	*this = std::move(rhs);
}

SpiMaster::SpiMaster(nullptr_t)
		: m_module(0),
		  m_frame_size(0),

		  m_sin(nullptr),
		  m_sout(nullptr),
		  m_sck(nullptr),

		  m_is_init(false)
{}

SpiMaster::~SpiMaster()
{
	Uninit();
}

SpiMaster& SpiMaster::operator=(SpiMaster &&rhs)
{
	if (this != &rhs)
	{
		Uninit();
		if (rhs)
		{
			rhs.m_is_init = false;

			m_module = rhs.m_module;

			m_sin = std::move(rhs.m_sin);
			m_sout = std::move(rhs.m_sout);
			m_sck = std::move(rhs.m_sck);
			for (Uint i = 0; i < kSlaveCount; ++i)
			{
				m_cs[i] = std::move(rhs.m_cs[i]);
			}

			m_frame_size = rhs.m_frame_size;

			m_is_init = true;
			g_instances[m_module] = this;
		}
	}
	return *this;
}

bool SpiMaster::InitModule(const Config &config)
{
	int sin_module = -1;
	switch (config.sin_pin)
	{
	case Pin::Name::kPta17:
	case Pin::Name::kPtc7:
	case Pin::Name::kPtd3:
		sin_module = 0;
		break;

	case Pin::Name::kPtb17:
	case Pin::Name::kPte3:
		sin_module = 1;
		break;

	case Pin::Name::kPtb23:
	case Pin::Name::kPtd14:
		sin_module = 2;
		break;

	default:
		break;
	}

	int sout_module = -1;
	switch (config.sout_pin)
	{
	case Pin::Name::kPta16:
	case Pin::Name::kPtc6:
	case Pin::Name::kPtd2:
		sout_module = 0;
		break;

	case Pin::Name::kPtb16:
	case Pin::Name::kPte1:
		sout_module = 1;
		break;

	case Pin::Name::kPtb22:
	case Pin::Name::kPtd13:
		sout_module = 2;
		break;

	default:
		break;
	}

	int sck_module = -1;
	switch (config.sck_pin)
	{
	case Pin::Name::kPta15:
	case Pin::Name::kPtc5:
	case Pin::Name::kPtd1:
		sck_module = 0;
		break;

	case Pin::Name::kPtb11:
	case Pin::Name::kPte2:
		sck_module = 1;
		break;

	case Pin::Name::kPtb21:
	case Pin::Name::kPtd12:
		sck_module = 2;
		break;

	default:
		break;
	}

	int cs_module = -1;
	for (Uint i = 0; i < kSlaveCount; ++i)
	{
		int this_cs_module = -1;
		switch (config.slaves[i].cs_pin)
		{
		case Pin::Name::kPta14:
		case Pin::Name::kPtc0:
		case Pin::Name::kPtc1:
		case Pin::Name::kPtc2:
		case Pin::Name::kPtc3:
		case Pin::Name::kPtc4:
		case Pin::Name::kPtd0:
		case Pin::Name::kPtd4:
		case Pin::Name::kPtd5:
		case Pin::Name::kPtd6:
			this_cs_module = 0;
			break;

		case Pin::Name::kPtb9:
		case Pin::Name::kPtb10:
		case Pin::Name::kPte0:
		case Pin::Name::kPte4:
		case Pin::Name::kPte5:
		case Pin::Name::kPte6:
			this_cs_module = 1;
			break;

		case Pin::Name::kPtb20:
		case Pin::Name::kPtd11:
		case Pin::Name::kPtd15:
			this_cs_module = 2;
			break;

		case Pin::Name::kDisable:
			continue;

		default:
			return false;
		}
		if (cs_module != -1 && cs_module != this_cs_module)
		{
			return false;
		}
		else
		{
			cs_module = this_cs_module;
		}
	}

	// 3-wire setup
	if (config.sin_pin == Pin::Name::kDisable)
	{
		sin_module = sout_module;
	}
	else if (config.sout_pin == Pin::Name::kDisable)
	{
		sout_module = sin_module;
	}

	if (sin_module == sout_module && sout_module == sck_module
			&& sck_module == cs_module && sin_module != -1)
	{
		m_module = sin_module;
		return true;
	}
	else
	{
		return false;
	}
}

void SpiMaster::InitPin(const Config &config)
{
	if (config.sin_pin != Pin::Name::kDisable)
	{
		Pin::Config sin_config;
		sin_config.pin = config.sin_pin;
		sin_config.mux = Pin::Config::MuxControl::kAlt2;
		m_sin = Pin(sin_config);
	}

	if (config.sout_pin != Pin::Name::kDisable)
	{
		Pin::Config sout_config;
		sout_config.pin = config.sout_pin;
		sout_config.mux = Pin::Config::MuxControl::kAlt2;
		m_sout = Pin(sout_config);
	}

	Pin::Config sck_config;
	sck_config.pin = config.sck_pin;
	sck_config.mux = Pin::Config::MuxControl::kAlt2;
	m_sck = Pin(sck_config);

	for (Uint i = 0; i < kSlaveCount; ++i)
	{
		if (config.slaves[i].cs_pin == Pin::Name::kDisable)
		{
			break;
		}
		Pin::Config cs_config;
		cs_config.pin = config.slaves[i].cs_pin;
		cs_config.mux = Pin::Config::MuxControl::kAlt2;
		m_cs[i] = Pin(cs_config);
	}
}

void SpiMaster::InitMcrReg(const Config &config)
{
	uint32_t reg = 0;

	SET_BIT(reg, SPI_MCR_MSTR_SHIFT);
	if (config.is_modified_timing)
	{
		SET_BIT(reg, SPI_MCR_MTFE_SHIFT);
	}
	SET_BIT(reg, SPI_MCR_ROOE_SHIFT);

	for (Uint i = 0; i < kSlaveCount; ++i)
	{
		if (config.slaves[i].cs_pin == Pin::Name::kDisable)
		{
			break;
		}
		reg |= (!config.slaves[i].is_active_high) << (SPI_MCR_PCSIS_SHIFT
				+ SpiUtils::GetCsNumber(config.slaves[i].cs_pin));
	}

	SET_BIT(reg, SPI_MCR_CLR_TXF_SHIFT);
	SET_BIT(reg, SPI_MCR_CLR_RXF_SHIFT);
	// Don't start yet
	SET_BIT(reg, SPI_MCR_HALT_SHIFT);

	MEM_MAPS[m_module]->MCR = reg;
}

void SpiMaster::InitCtarReg(const Config &config)
{
	assert(config.frame_size >= 4 && config.frame_size <= 16);

	uint32_t reg = 0;

	SetBaudRate(config.baud_rate_khz, &reg);
	SetPcsSckDelay(config.pcs_to_sck_delay_ns, &reg);
	SetAfterSckDelay(config.after_sck_delay_ns, &reg);
	SetAfterTransferDelay(config.after_transfer_delay_ns, &reg);

	m_frame_size = Clamp<uint8_t>(4, config.frame_size, 16);
	reg |= SPI_CTAR_FMSZ(m_frame_size - 1);
	if (!config.is_sck_idle_low)
	{
		SET_BIT(reg, SPI_CTAR_CPOL_SHIFT);
	}
	if (!config.is_sck_capture_first)
	{
		SET_BIT(reg, SPI_CTAR_CPHA_SHIFT);
	}

	MEM_MAPS[m_module]->CTAR[0] = reg;
}

void SpiMaster::SetBaudRate(const uint32_t baud_rate_khz, uint32_t *reg)
{
	const Uint pbrs[] = {1, 2, 3, 5, 7};
	const Uint brs[] = {2, 4, 6, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096,
			8192, 16384, 32768};
	const uint32_t bus_khz = ClockUtils::GetBusClockKhz();

	Uint best_pbr_pos = 0, best_br_pos = 0;
	Uint min_diff = static_cast<Uint>(-1);
	for (Uint i = 0; i < sizeof(pbrs) / sizeof(Uint); ++i)
	{
		for (Uint j = 0; j < sizeof(brs) / sizeof(Uint); ++j)
		{
			const uint32_t this_baud_rate = bus_khz / (pbrs[i] * brs[j]);
			const Uint this_diff = abs((int32_t)(this_baud_rate - baud_rate_khz));
			if (this_diff < min_diff)
			{
				min_diff = this_diff;
				best_pbr_pos = i;
				best_br_pos = j;
			}

			if (min_diff == 0)
			{
				break;
			}
		}
	}

	if (best_pbr_pos == 0)
	{
		SET_BIT(*reg, SPI_CTAR_DBR_SHIFT);
	}
	else
	{
		*reg |= SPI_CTAR_PBR(best_pbr_pos - 1);
	}
	*reg |= SPI_CTAR_BR(best_br_pos);
}

void SpiMaster::SetPcsSckDelay(const uint32_t pcs_sck_delay_ns, uint32_t *reg)
{
	DelayScalerCalculator calc;
	calc.Calc(pcs_sck_delay_ns);
	*reg |= SPI_CTAR_PCSSCK(calc.GetBestPrescaler());
	*reg |= SPI_CTAR_CSSCK(calc.GetBestScaler());
}

void SpiMaster::SetAfterSckDelay(const uint32_t after_sck_delay_ns,
		uint32_t *reg)
{
	DelayScalerCalculator calc;
	calc.Calc(after_sck_delay_ns);
	*reg |= SPI_CTAR_PASC(calc.GetBestPrescaler());
	*reg |= SPI_CTAR_ASC(calc.GetBestScaler());
}

void SpiMaster::SetAfterTransferDelay(const uint32_t after_transfer_delay_ns,
		uint32_t *reg)
{
	DelayScalerCalculator calc;
	calc.Calc(after_transfer_delay_ns);
	*reg |= SPI_CTAR_PDT(calc.GetBestPrescaler());
	*reg |= SPI_CTAR_DT(calc.GetBestScaler());
}

void SpiMaster::Uninit()
{
	if (m_is_init)
	{
		m_is_init = false;

		SetHalt(true);
#if MK60DZ10 || MK60D10
		Sim::SetEnableClockGate(EnumAdvance(Sim::ClockGate::kSpi0, m_module),
				false);
#elif MK60F15
		Sim::SetEnableClockGate(EnumAdvance(Sim::ClockGate::kDspi0, m_module),
				false);
#endif
		g_instances[m_module] = nullptr;
	}
}

void SpiMaster::SetHalt(const bool flag)
{
	SetHalt_(m_module, flag);
}

uint16_t SpiMaster::ExchangeData(const uint8_t slave_id, const uint16_t data)
{
	STATE_GUARD(SpiMaster, 0);
	if (slave_id >= kSlaveCount)
	{
		assert(false);
		return 0;
	}

	uint32_t reg = 0;
	reg |= SPI_PUSHR_CTAS(0);
	reg |= SPI_PUSHR_PCS(1 << SpiUtils::GetCsNumber(m_cs[slave_id].GetName()));
	reg |= SPI_PUSHR_TXDATA(data & ((1 << m_frame_size) - 1));

	// Wait until old data is gone
	while (MEM_MAPS[m_module]->SR & SPI_SR_TXCTR_MASK)
	{}
	MEM_MAPS[m_module]->PUSHR = reg;
	// Wait until new data is sent
	while (MEM_MAPS[m_module]->SR & SPI_SR_TXCTR_MASK)
	{}
	while (!(MEM_MAPS[m_module]->SR & SPI_SR_RXCTR_MASK))
	{}

	uint16_t receive = 0;
	// Dismiss old data
	while (GET_BITS(MEM_MAPS[m_module]->SR, SPI_SR_RXCTR_SHIFT,
			SPI_SR_RXCTR_MASK) > 1)
	{
		receive = MEM_MAPS[m_module]->POPR;
	}
	receive = MEM_MAPS[m_module]->POPR;
	return receive;
}

void SpiMaster::KickStart()
{
	STATE_GUARD(SpiMaster, VOID);
	SetHalt(false);
}

size_t SpiMaster::PushData(const uint8_t slave_id, const uint16_t *data,
		const size_t size)
{
	return PushData(slave_id, reinterpret_cast<const uint8_t*>(data), size * 2);
}

size_t SpiMaster::PushData(const uint8_t slave_id, const uint8_t *data,
		const size_t size)
{
	STATE_GUARD(SpiMaster, 0);

	const size_t send = std::min<Uint>(TX_FIFO_SIZE - GET_BITS(
			MEM_MAPS[m_module]->SR, SPI_SR_TXCTR_SHIFT, SPI_SR_TXCTR_MASK),
			((m_frame_size > 8) ? (size / 2) : size));
	if (send == 0)
	{
		return 0;
	}

	const uint8_t csn = SpiUtils::GetCsNumber(m_cs[slave_id].GetName());
	for (size_t i = 0; i < send; ++i)
	{
		uint32_t reg = 0;
		reg |= SPI_PUSHR_CTAS(0);
		reg |= SPI_PUSHR_PCS(1 << csn);
		if (m_frame_size > 8)
		{
			reg |= SPI_PUSHR_TXDATA(((data[i * 2] << 8) | data[i * 2 + 1])
					& ((1 << m_frame_size) - 1));
		}
		else
		{
			reg |= SPI_PUSHR_TXDATA(data[i] & ((1 << m_frame_size) - 1));
		}
		MEM_MAPS[m_module]->PUSHR = reg;
	}
	SET_BIT(MEM_MAPS[m_module]->SR, SPI_SR_TFFF_SHIFT);
	return send;
}

__ISR void SpiMaster::IrqHandler()
{
	const int module = GetActiveIrq() - SPI0_IRQn;
	SpiMaster *const that = g_instances[module];
	if (!that || !(*that))
	{
		// Something wrong?
		assert(false);
		SetHalt_(module, true);
		MEM_MAPS[module]->RSER = 0;
		return;
	}

	if (GET_BIT(MEM_MAPS[module]->RSER, SPI_RSER_TFFF_RE_SHIFT)
			&& GET_BIT(MEM_MAPS[module]->SR, SPI_SR_TFFF_SHIFT))
	{
		SET_BIT(MEM_MAPS[module]->SR, SPI_SR_TFFF_SHIFT);
		if (that->m_tx_isr)
		{
			that->m_tx_isr(that);
		}
		else
		{
			SetHalt_(module, true);
			CLEAR_BIT(MEM_MAPS[module]->RSER, SPI_RSER_TFFF_RE_SHIFT);
			SetHalt_(module, false);
		}
	}

	if (GET_BIT(MEM_MAPS[module]->RSER, SPI_RSER_RFDF_RE_SHIFT)
			&& GET_BIT(MEM_MAPS[module]->SR, SPI_SR_RFDF_SHIFT))
	{
		SET_BIT(MEM_MAPS[module]->SR, SPI_SR_RFDF_SHIFT);
		if (that->m_rx_isr)
		{
			that->m_rx_isr(that);
		}
		else
		{
			SetHalt_(module, true);
			CLEAR_BIT(MEM_MAPS[module]->RSER, SPI_RSER_RFDF_RE_SHIFT);
			SetHalt_(module, false);
		}
	}

	if (GET_BIT(MEM_MAPS[module]->RSER, SPI_RSER_TCF_RE_SHIFT)
			&& GET_BIT(MEM_MAPS[module]->SR, SPI_SR_TCF_SHIFT))
	{
		SET_BIT(MEM_MAPS[module]->SR, SPI_SR_TCF_SHIFT);
		SetHalt_(module, true);
	}
}

}
}
