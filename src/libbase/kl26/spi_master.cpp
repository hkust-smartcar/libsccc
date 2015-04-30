#include "libbase/kl26/hardware.h"
#include "libbase/kl26/spi_master.h"
#include "libbase/misc_utils_c.h"
#include "libutil/misc.h"
#include "libbase/kl26/sim.h"
#include "libbase/kl26/pin.h"
#include "libbase/kl26/clock_utils.h"
#include <climits>

using namespace libutil;

namespace libbase {
namespace kl26 {

constexpr SPI_Type * MEM_MAPS[2] = { SPI0, SPI1 };

SpiMaster::SpiMaster(const Config &config):
	m_module(0){
//	Enable module clock
	Sim::SetEnableClockGate(EnumAdvance(Sim::ClockGate::kSpi0,m_module), true);

/*
 * Init pins
 */
//	int sin_module = -1;
//	int sout_module = -1;
//	int sck_module = -1;
//	int cs_module = -1;

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
	MEM_MAPS[0]->C1 |= SPI_C1_SPE_MASK | SPI_C1_SPIE_MASK | SPI_C1_MSTR_MASK;

/*
 * C2 register
 */
// 	Set SPI mode to 16-bit mode
	SET_BIT(MEM_MAPS[0]->C2,SPI_C2_SPIMODE_SHIFT);
	SET_BIT(MEM_MAPS[0]->C2,SPI_C2_MODFEN_SHIFT);

	SET_BIT(MEM_MAPS[0]->C1,SPI_C1_SSOE_SHIFT);

/*
 * BR register - Section 37.4.8
 * BaudRateDivisor = (SPPR + 1) * 2^(SPR  +1)
 * BaudRate = SPI Module Clock / BaudRateDivisor
 */
	uint32_t module_clock;
	if(m_module == 0){
		module_clock = ClockUtils::GetBusClock();
	}else if(m_module == 1){
		module_clock = ClockUtils::GetCoreClock();
	}

	int diff = INT_MAX;
	int best_sppr = 0;
	int best_spr = 0;
//	for(int spr = 0; spr < 9; spr++ ){
//		if((abs(module_clock/pow(2.0,(double)(spr+1))) /config.baud_rate_khz*1000) <= 8){
//			best_spr = spr;
//			break;
//		}
//	}
//	for(int sppr = 0; sppr <8; sppr++){
//
//			if(abs((module_clock/((sppr+1) * pow(2.0,(double)(best_spr+1)) )) - config.baud_rate_khz*1000) < diff){
//				best_sppr = sppr;
//				diff = abs((module_clock/((sppr+1) * pow(2.0,(double)(best_spr+1)) )) - config.baud_rate_khz*1000);
//			}
//			if((module_clock/((sppr+1) * pow(2.0,(double)(best_spr)) )) == config.baud_rate_khz*1000){
//				best_sppr = sppr;
//				break;
//			}
//
//	}
	MEM_MAPS[0]->BR |= (SPI_BR_SPPR(8) | SPI_BR_SPR(0));


}

SpiMaster::~SpiMaster() {
	// TODO Auto-generated destructor stub
}

uint16_t SpiMaster:: ExchangeData(const uint8_t slave_id, const uint16_t data){
	uint16_t received;

//	Wait untill SPTEF = 1
	while(!GET_BIT(MEM_MAPS[0]->S,SPI_S_SPTEF_SHIFT));
	MEM_MAPS[0]->DH = (data & 0xFF00) >> 8;
	MEM_MAPS[0]->DL = data & 0xFF;

//	Wait till transmitt buffer is empty
	while(!GET_BIT(MEM_MAPS[0]->S,SPI_S_SPTEF_SHIFT));

	received = (MEM_MAPS[0]->DH << 8) | (MEM_MAPS[0]->DL);
	return received;
}

} /* namespace kl26 */
} /* namespace libbase */
