/*
 * ov7725_configurator.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstddef>
#include <cstdint>

#include "libbase/helper.h"
#include "libbase/log.h"
#include "libbase/misc_types.h"
#include LIBBASE_H(gpio)
#include LIBBASE_H(soft_sccb_master)

#include "libsc/device_h/ov7725.h"
#include "libsc/k60/ov7725_configurator.h"

using namespace LIBBASE_NS;

#define CHECKED_SCCB_GET_BYTE(slave_addr, reg_addr, out_byte, ret) \
		if (!m_sccb.GetByte(slave_addr, reg_addr, out_byte)) \
		{\
			LOG_EL("Failed reading " #reg_addr);\
			return ret;\
		}

namespace libsc
{
namespace k60
{

namespace
{

SoftSccbMaster::Config GetSccbConfig(const Ov7725Configurator::Config &config)
{
	SoftSccbMaster::Config product;
	product.sio_c_pin = config.scl;
	product.sio_d_pin = config.sda;
	return product;
}

}

Ov7725Configurator::Ov7725Configurator(const Config &config)
		: m_sccb(GetSccbConfig(config))
{
	if (!Verify())
	{
		LOG_EL("Failed verifying OV7725");
		assert(false);
	}

	InitCom2Reg();
	InitCom3Reg();
	InitClock(config);
	InitResolution(config);
	InitBandingFilter();
	InitLensCorrection();
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_AWB_CTRL0, 0xE0);
	InitDsp();
	InitGamma();
	InitSecialDigitalEffect(config);
	InitAutoUv();
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_SIGN, 0x06);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_REG16, 0x00);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_COM10, 0x00);
}

Ov7725Configurator::Ov7725Configurator(nullptr_t)
		: m_sccb(nullptr)
{}

Ov7725Configurator::~Ov7725Configurator()
{}

bool Ov7725Configurator::Verify()
{
	Byte pid, ver;
	CHECKED_SCCB_GET_BYTE(OV7725_SLAVE_ADDR, OV7725_PID, &pid, false);
	CHECKED_SCCB_GET_BYTE(OV7725_SLAVE_ADDR, OV7725_VER, &ver, false);
	return (pid == 0x77 && ver == 0x21);
}

void Ov7725Configurator::ChangeSecialDigitalEffect(uint8_t brightness, uint8_t contrast) {
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_SDE, 0x04);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_BRIGHT, brightness);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_CNST, contrast);
}

void Ov7725Configurator::InitCom2Reg()
{
	uint8_t reg = 0;
	// 4x output drive capability
	reg |= 0x3;

	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_COM2, reg);
}

void Ov7725Configurator::InitCom3Reg()
{
	uint8_t reg = 0;
	// Swap YUV output seq
	SET_BIT(reg, 4);
	// Horizontal mirror image
	SET_BIT(reg, 6);
	// Vertical flip image
	SET_BIT(reg, 7);

	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_COM3, reg);
}

void Ov7725Configurator::InitClock(const Config &config)
{
	// Internal clock = 12mHz * PLLx / ((CLKRC + 1)) * 2)
	// 12m is the input clock of our part
	const uint8_t pll = std::min<Uint>(static_cast<Uint>(config.fps) + 1, 3);
	// However, CLKRC must be 0
	const uint8_t clkrc = 0;

	uint8_t com4_reg = 0x1;
	com4_reg |= pll << 6;

	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_COM4, com4_reg);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_CLKRC, clkrc);
}

void Ov7725Configurator::InitResolution(const Config &config)
{
	// Use QVGA whenever possible
	const bool is_qvga = (config.w <= 320 && config.h <= 240);

	uint8_t com7_reg = 0;
	if (is_qvga)
	{
		com7_reg |= 0x40;
	}
	com7_reg |= ((int)config.format & 0x3) << 0;
	com7_reg |= ((int)config.rgb_format & 0x3) << 2;
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_COM7, com7_reg);

	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_HSTART, is_qvga ? 0x3F : 0x23);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_HSIZE, is_qvga ? 0x50 : 0xA0);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_HOUTSIZE, config.w >> 2);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_VSTRT, is_qvga ? 0x03 : 0x07);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_VSIZE, is_qvga ? 0x78 : 0xF0);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_VOUTSIZE, config.h >> 1);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_HREF, 0x00);

	uint8_t exhch_reg = 0;
	exhch_reg |= config.w & 0x3;
	exhch_reg |= (config.h & 0x1) << 2;
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_EXHCH, exhch_reg);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_EXHCL, 0x00);
}

void Ov7725Configurator::InitBandingFilter()
{
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_BDBASE, 0x99);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_BDMSTEP, 0x03);
}

void Ov7725Configurator::InitLensCorrection()
{
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_LC_CTR, 0x05);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_LC_XC, 0x08);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_LC_YC, 0x00);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_LC_COEF, 0x13);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_LC_RADI, 0x00);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_LC_COEFB, 0x14);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_LC_COEFR, 0x17);
}

void Ov7725Configurator::InitDsp()
{
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_DSP_CTRL1, 0xBF);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_DSP_CTRL2, 0x0C);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_DSP_CTRL3, 0x00);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_DSP_CTRL4, 0x00);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_DSPAUTO, 0xFF);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_SCAL0, 0x0A);
}

void Ov7725Configurator::InitGamma()
{
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_GAM1, 0x0E);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_GAM2, 0x1A);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_GAM3, 0x31);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_GAM4, 0x5A);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_GAM5, 0x69);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_GAM6, 0x75);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_GAM7, 0x7E);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_GAM8, 0x88);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_GAM9, 0x8F);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_GAM10, 0x96);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_GAM11, 0xA3);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_GAM12, 0xAF);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_GAM13, 0xC4);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_GAM14, 0xD7);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_GAM15, 0xE8);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_SLOP, 0x20);
}

void Ov7725Configurator::InitSecialDigitalEffect(const Config &config)
{
	// enable brightness and contrast control
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_SDE, 0x04);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_BRIGHT, config.brightness);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_CNST, config.contrast);
}

void Ov7725Configurator::InitAutoUv()
{
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_UVADJ0, 0x11);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_UVADJ1, 0x02);
}

}
}
