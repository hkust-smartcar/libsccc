/*
 * spi_master.cpp
 *
 * Author: Harrison Ng
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/kl26/hardware.h"

#include <climits>
#include <cstddef>
#include <cstdint>

#include <functional>

#include "libbase/log.h"
#include "libbase/kl26/clock_utils.h"
#include "libbase/kl26/misc_utils.h"
#include "libbase/kl26/pin.h"
#include "libbase/kl26/pinout.h"
#include "libbase/kl26/sim.h"
#include "libbase/kl26/spi_master.h"

#include "libutil/misc.h"

using namespace libutil;

namespace libbase
{
namespace kl26
{

namespace
{

constexpr SPI_Type * MEM_MAPS[PINOUT::GetSpiCount()] = {SPI0, SPI1};

}

SpiMaster::SpiMaster(const Config &config)
		: m_module(0)
{
//	Enable module clock
	Sim::SetEnableClockGate(EnumAdvance(Sim::ClockGate::kSpi0,m_module), true);

/*
 * Init pins
 */
//	int sin_module = -1;
//	int sout_module = -1;
//	int sck_module = -1;
//	int cs_module = -1;
//	if(config.sin_pin == Pin::Name::kPte0){
//		sin_module = 1;
//	}

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
		sout_config.config.set(Pin::Config::ConfigBit::kPullUp);
		sout_config.mux = Pin::Config::MuxControl::kAlt2;
		m_sout = Pin(sout_config);
	}

	Pin::Config sck_config;
	sck_config.pin = config.sck_pin;
	sck_config.config.set(Pin::Config::ConfigBit::kPullUp);
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

/*
 * C1 register
 */

//	Enable SPI system
//	Set as master mode
//	Enable interrupt
	MEM_MAPS[m_module]->C1 |= SPI_C1_SPE_MASK | SPI_C1_SPIE_MASK | SPI_C1_MSTR_MASK;

//	CPHA = 1, First edge on SPSCK occurs at the start of the first cycle of a data transfer.
	CLEAR_BIT(MEM_MAPS[m_module]->C1,SPI_C1_CPHA_SHIFT);
//	CPOL = 0, Active-high SPI clock (idles low)
	SET_BIT(MEM_MAPS[m_module]->C1,SPI_C1_CPOL_SHIFT);

/*
 * C2 register
 */
// 	Set SPI mode to 8-bit mode
	CLEAR_BIT(MEM_MAPS[m_module]->C2,SPI_C2_SPIMODE_SHIFT);

//	Enable SPI SS
	SET_BIT(MEM_MAPS[m_module]->C2,SPI_C2_MODFEN_SHIFT);
	SET_BIT(MEM_MAPS[m_module]->C1,SPI_C1_SSOE_SHIFT);

/*
 * BR register - Section 37.4.8
 * BaudRateDivisor = (SPPR + 1) * 2^(SPR  +1)
 * BaudRate = SPI Module Clock / BaudRateDivisor
 */
	uint32_t module_clock = 0;
	if (m_module == 0)
	{
		module_clock = ClockUtils::GetBusClock();
	}
	else if (m_module == 1)
	{
		module_clock = ClockUtils::GetCoreClock();
	}

	int diff = INT_MAX;
	int best_sppr = 0;
	int best_spr = 0;
	for (int spr = 0; spr < 9; spr++)
	{
		if ((module_clock/(2<<(spr))/(config.baud_rate_khz*1000)) <= 8)
		{
			best_spr = spr;
			break;
		}
	}
	for (int sppr = 0; sppr <8; sppr++)
	{
		if (abs((module_clock/((sppr+1) * (2<<best_spr) )) - config.baud_rate_khz*1000) < diff)
		{
			best_sppr = sppr;
			diff = abs((module_clock/((sppr+1) * (2<<best_spr) )) - config.baud_rate_khz*1000);
			if (diff==0)
			{
				break;
			}
		}
	}
	MEM_MAPS[m_module]->BR |= (SPI_BR_SPPR(best_sppr) | SPI_BR_SPR(best_spr));

	m_tx_isr = config.tx_isr;
	m_rx_isr = config.rx_isr;
}

SpiMaster::~SpiMaster()
{}

uint16_t SpiMaster:: ExchangeData(const uint8_t, const uint16_t data)
{
	uint16_t received;

//	Wait untill SPTEF = 1
	while(!GET_BIT(MEM_MAPS[m_module]->S,SPI_S_SPTEF_SHIFT));
//	Remark: in 8 bit mode, DH ignored
	MEM_MAPS[m_module]->DH = (data & 0xFF00) >> 8;
	MEM_MAPS[m_module]->DL = data & 0xFF;

//	Wait till transmitt buffer is empty
	while(!GET_BIT(MEM_MAPS[m_module]->S,SPI_S_SPTEF_SHIFT));
//	Remark: in 8 bit mode, DH ignored
	received = (MEM_MAPS[m_module]->DH << 8) | (MEM_MAPS[m_module]->DL);
	return received;
}

void SpiMaster::KickStart()
{}

size_t SpiMaster::PushData(const uint8_t, const uint16_t*, const size_t)
{
	return 0;
}

size_t SpiMaster::PushData(const uint8_t, const uint8_t*, const size_t)
{
	return 0;
}

void SpiMaster::SetEnableRxIrq(const bool)
{
	STATE_GUARD(SpiMaster, VOID);
}

void SpiMaster::SetEnableTxIrq(const bool)
{
	STATE_GUARD(SpiMaster, VOID);
}

}
}
