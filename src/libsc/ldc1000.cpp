/*
 * ldc1000.cpp
 *
 * Author: Petel__
 * Copyright (c) 2014-2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstring>
#include <cstdint>
#include "libsc/system.h"
#include "libsc/device_h/ldc1000.h"
#include "libsc/ldc1000.h"
#include "libsc/config.h"

using namespace std;
using namespace libsc;
using namespace LIBBASE_NS;
using namespace libbase::k60;

#define RP_MAX					0x12
#define RP_MIN					0x34
#define SENSOR_FREQ				0xA9

namespace libsc
{

#ifdef LIBSC_USE_LDC1000

Gpo::Config getGpoConfig(const Pin::Name pin, const bool is_high)
{
	Gpo::Config product;
	product.pin = pin;
	product.is_high = is_high;
	return product;
}

void Ldc1000::InitPin(uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break

	case 0:
		m_mosi = Gpo(getGpoConfig(LIBSC_LDC1000_0_DC, true));
		m_cs = Gpo(getGpoConfig(LIBSC_LDC1000_0_CS, true));
		m_sck = Gpo(getGpoConfig(LIBSC_LDC1000_0_SCLK, false));
		m_miso = Gpio(getGpoConfig(LIBSC_LDC1000_0_SDAT,false));
		break;

#if LIBSC_USE_LDC1000 > 1
	case 1:
		m_mosi = Gpo(getGpoConfig(LIBSC_LDC1000_1_DC, true));
		m_cs = Gpo(getGpoConfig(LIBSC_LDC1000_1_CS, true));
		m_sck = Gpo(getGpoConfig(LIBSC_LDC1000_1_SCLK, false));
		m_miso = Gpio(getGpoConfig(LIBSC_LDC1000_1_SDAT,false));
		break;
#endif

#if LIBSC_USE_LDC1000 > 2
	case 2:
		m_mosi = Gpo(getGpoConfig(LIBSC_LDC1000_2_DC, true));
		m_cs = Gpo(getGpoConfig(LIBSC_LDC1000_2_CS, true));
		m_sck = Gpo(getGpoConfig(LIBSC_LDC1000_2_SCLK, false));
		m_miso = Gpio(getGpoConfig(LIBSC_LDC1000_2_SDAT,false));
		break;
#endif
	}
}

Ldc1000::Ldc1000(const Config &config)
:
	m_buf{ 0 },
	m_proxData(0),
	m_freq(0),
	m_mosi(nullptr),
	m_cs(nullptr),
	m_sck(nullptr),
	m_miso(nullptr)

{
	InitPin(config.id);
	System::DelayUs(5);
//		m_mosi =  libbase::k60::Adc::Name::kAdc1Ad10;
	WriteData(REG_RP_MAX, RP_MAX);
	WriteData(REG_RP_MIN, RP_MIN);
	WriteData(REG_SENSOR_FREQ, SENSOR_FREQ);
	WriteData(REG_LDC_CONFIG, 0x13);
	WriteData(REG_CLK_CONFIG, 0x00);
	WriteData(REG_INT_CONFIG, 0x02);
	WriteData(REG_PWR_CONFIG, 0x01);
	WriteData(REG_THRES_HI_LSB, 0x50);
	WriteData(REG_THRES_HI_MSB, 0x14);
	WriteData(REG_THRES_LO_LSB, 0xC0);
	WriteData(REG_THRES_LO_MSB, 0x12);
	ReadBuffer();

	assert(m_buf[0] == 0x80);

}

void Ldc1000::Update(void)
{
	m_proxData = (ReadData(REG_PROX_MSB) << 8) | ReadData(REG_PROX_LSB);
	m_freq = (ReadData(REG_FREQ_MSB) << 16) | (ReadData(REG_FREQ_MID) << 8) | ReadData(REG_FREQ_LSB);
}

uint16_t Ldc1000::GetData(void)
{
	return m_proxData;
}

uint32_t Ldc1000::GetFreq(void)
{
	return m_freq;
}

uint8_t	Ldc1000::DataRW(uint8_t rwData)
{
	uint8_t ret = 0;

	for(uint8_t i = 0; i < 8; i++)
	{
		m_mosi.Set(rwData & 0x80);

		rwData <<= 1;
		ret <<= 1;

		m_sck.Turn();

		if(m_miso.Get())
			ret |= 1;

		m_sck.Turn();
	}

	return(ret);
}

uint8_t Ldc1000::ReadData(const uint8_t reg)
{
	uint8_t ret = 0;
	m_cs.Set(false);
	System::DelayUs(2);
	DataRW(reg | 0x80);

	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	ret = DataRW(0);
	m_cs.Set(true);
	System::DelayUs(875);
	return ret;
}

uint8_t Ldc1000::WriteData(const uint8_t reg, const uint8_t data)
{
	uint8_t ret = 0;
	m_cs.Set(false);
	System::DelayUs(2);
	DataRW(reg & ~0x80);

	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");
	asm("nop");

	ret = DataRW(data);
	m_cs.Set(true);
	System::DelayUs(875);
	return ret;
}

void Ldc1000::ReadBuffer(void)
{
	m_cs.Set(false);
	DataRW(0x80);

	for (uint8_t i = 0; i < 12; i++)
		m_buf[i] = DataRW(0);
	m_cs.Set(true);
}

#else

Ldc1000::Ldc1000(const Config &config)
:
	m_buf{ 0 },
	m_mosi(nullptr),
	m_cs(nullptr),
	m_sck(nullptr),
	m_miso(nullptr),
	m_proxData(0),
	m_freq(0)
{}

void Ldc1000::Update(void)
{}

uint16_t Ldc1000::GetData(void)
{
	return 0;
}

uint32_t Ldc1000::GetFreq(void)
{
	return 0;
}

#endif

}
