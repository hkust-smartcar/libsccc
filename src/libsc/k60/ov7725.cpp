/*
 * ov7725.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstdint>
#include <cstring>

#include <algorithm>
#include <functional>
#include <memory>

#include "libbase/log.h"
#include "libbase/k60/dma.h"
#include "libbase/k60/dma_manager.h"
#include "libbase/k60/dma_mux.h"
#include "libbase/k60/gpio.h"
#include "libbase/k60/gpio_array.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/pin_utils.h"
#include "libbase/k60/soft_sccb_master.h"
#include "libbase/k60/vectors.h"

#include "libsc/config.h"
#include "libsc/device_h/ov7725.h"
#include "libsc/k60/ov7725.h"
#include "libsc/k60/system.h"
#include "libutil/misc.h"

using namespace libbase::k60;
using namespace libutil;
using namespace std;

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

#ifdef LIBSC_USE_OV7725

namespace
{

SoftSccbMaster::Config GetSccbConfig()
{
	SoftSccbMaster::Config product;
	product.sio_c_pin = LIBSC_OV77250_SCL;
	product.sio_d_pin = LIBSC_OV77250_SDA;
	return product;
}

GpiArray::Config GetGpiArrayConfig()
{
	GpiArray::Config product;
	product.start_pin = LIBSC_OV77250_DATA0;
	product.count = 8;
	product.config.set(Pin::Config::ConfigBit::kPullEnable);
	return product;
}

Gpi::Config GetClockConfig()
{
	Gpi::Config product;
	product.pin = LIBSC_OV77250_PCLK;
	product.config.set(Pin::Config::ConfigBit::kPullEnable);
	product.interrupt = Pin::Config::Interrupt::kDmaFalling;
	return product;
}

Gpi::Config GetVsyncConfig(Gpi::OnGpiEventListener isr)
{
	Gpi::Config product;
	product.pin = LIBSC_OV77250_VSYNC;
	product.config.set(Pin::Config::ConfigBit::kPassiveFilter);
	product.config.set(Pin::Config::ConfigBit::kPullEnable);
	product.config.set(Pin::Config::ConfigBit::kPullUp);
	product.interrupt = Pin::Config::Interrupt::kRising;
	product.isr = isr;
	return product;
}

}

Ov7725::Ov7725(const Config &config)
		: m_sccb(GetSccbConfig()),
		  m_data_array(GetGpiArrayConfig()),
		  m_clock(GetClockConfig()),
		  m_vsync(nullptr),
		  m_dma(nullptr),
		  m_w(libutil::Clamp<Uint>(1, config.w, 640)),
		  m_h(libutil::Clamp<Uint>(1, config.h, 480)),
		  m_buf_size(m_w * m_h / 8),
		  m_is_shoot(false),
		  m_is_lock_buffer(false),
		  m_is_available(false),
		  m_is_dma_start(false)
{
	if (!Verify())
	{
		LOG_EL("Failed verifying OV7725");
		assert(false);
	}

	InitCom2Reg();
	InitCom3Reg();
	InitClock(config);
	InitResolution();
	InitBandingFilter();
	InitLensCorrection();
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_AWB_CTRL0, 0xE0);
	InitDsp();
	InitGamma();
	InitSecialDigitalEffect(config);
	InitAutoUv();
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_SIGN, 0x06);

	m_front_buf.reset(new Byte[m_buf_size]);
	memset(m_front_buf.get(), 0, m_buf_size);
	m_back_buf.reset(new Byte[m_buf_size]);

	InitDma();

	m_vsync = Gpi(GetVsyncConfig(std::bind(&Ov7725::OnVsync, this,
			placeholders::_1)));
	// Set DMA to a higher priority to prevent VSYNC being processed earlier
	NVIC_SetPriority(DMA1_DMA17_IRQn, __BASE_IRQ_PRIORITY - 1);
}

Ov7725::~Ov7725()
{
	NVIC_SetPriority(DMA1_DMA17_IRQn, __BASE_IRQ_PRIORITY);
}

bool Ov7725::Verify()
{
	Byte pid, ver;
	CHECKED_SCCB_GET_BYTE(OV7725_SLAVE_ADDR, OV7725_PID, &pid, false);
	CHECKED_SCCB_GET_BYTE(OV7725_SLAVE_ADDR, OV7725_VER, &ver, false);
	return (pid == 0x77 && ver == 0x21);
}

void Ov7725::InitCom2Reg()
{
	uint8_t reg = 0;
	// 4x output drive capability
	reg |= 0x3;

	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_COM2, reg);
}

void Ov7725::InitCom3Reg()
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

void Ov7725::InitClock(const Config &config)
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

void Ov7725::InitResolution()
{
	// Use QVGA whenever possible
	const bool is_qvga = (m_w <= 320 && m_h <= 240);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_COM7, is_qvga ? 0x40 : 0x00);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_HSTART, is_qvga ? 0x3F : 0x23);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_HSIZE, is_qvga ? 0x50 : 0xA0);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_HOUTSIZE, m_w >> 2);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_VSTRT, is_qvga ? 0x03 : 0x07);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_VSIZE, is_qvga ? 0x78 : 0xF0);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_VOUTSIZE, m_h >> 1);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_HREF, 0x00);

	uint8_t exhch_reg = 0;
	exhch_reg |= m_w & 0x3;
	exhch_reg |= (m_h & 0x1) << 2;
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_EXHCH, exhch_reg);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_EXHCL, 0x00);
}

void Ov7725::InitBandingFilter()
{
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_BDBASE, 0x99);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_BDMSTEP, 0x03);
}

void Ov7725::InitLensCorrection()
{
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_LC_CTR, 0x05);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_LC_XC, 0x08);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_LC_YC, 0x00);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_LC_COEF, 0x13);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_LC_RADI, 0x00);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_LC_COEFB, 0x14);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_LC_COEFR, 0x17);
}

void Ov7725::InitDsp()
{
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_DSP_CTRL1, 0xBF);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_DSP_CTRL2, 0x0C);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_DSP_CTRL3, 0x00);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_DSP_CTRL4, 0x00);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_DSPAUTO, 0xFF);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_SCAL0, 0x0A);
}

void Ov7725::InitGamma()
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

void Ov7725::InitSecialDigitalEffect(const Config &config)
{
	// enable brightness and contrast control
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_SDE, 0x04);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_BRIGHT, config.brightness);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_CNST, config.contrast);
}

void Ov7725::InitAutoUv()
{
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_UVADJ0, 0x11);
	m_sccb.SendByte(OV7725_SLAVE_ADDR, OV7725_UVADJ1, 0x02);
}

void Ov7725::InitDma()
{
	Dma::Config config;
	m_data_array.ConfigValueAsDmaSrc(&config);
	config.mux_src = EnumAdvance(DmaMux::Source::kPortA,
			PinUtils::GetPort(LIBSC_OV77250_PCLK));
	config.dst.addr = m_back_buf.get();
	config.dst.offset = 1;
	config.dst.size = Dma::Config::TransferSize::k1Byte;
	config.dst.major_offset = -m_buf_size;
	config.major_count = m_buf_size;
	config.complete_isr = std::bind(&Ov7725::OnDmaComplete, this,
			placeholders::_1);

	m_dma = DmaManager::New(config, LIBSC_OV77250_DMA_CH);
}

void Ov7725::Start()
{
	m_is_available = false;
	m_is_dma_start = false;
	m_is_shoot = true;
}

void Ov7725::Stop()
{
	m_is_available = false;
	m_is_shoot = false;
}

const Byte* Ov7725::LockBuffer()
{
	m_is_lock_buffer = true;
	return m_front_buf.get();
}

void Ov7725::UnlockBuffer()
{
	m_is_lock_buffer = false;
}

void Ov7725::OnVsync(Gpi*)
{
	LOG_VL("Ov7725 vsync");
	if (!m_is_shoot)
	{
		return;
	}

	if (m_dma->IsDone() || !m_is_dma_start)
	{
		m_is_dma_start = true;
		Pin::ConsumeInterrupt(m_clock.GetPin()->GetName());
		m_dma->Start();
	}
}

void Ov7725::OnDmaComplete(Dma*)
{
	LOG_VL("Ov7725 complete");
	if (!m_is_lock_buffer)
	{
		memcpy(m_front_buf.get(), m_back_buf.get(), m_buf_size);
		m_is_available = true;
	}
}

#else
Ov7725::Ov7725(const Config&)
		: m_sccb(nullptr), m_data_array(nullptr), m_clock(nullptr),
		  m_vsync(nullptr), m_dma(nullptr), m_w(0), m_h(0), m_buf_size(0),
		  m_is_shoot(false), m_is_lock_buffer(false), m_is_available(false),
		  m_is_dma_start(false)
{
	LOG_DL("Configured not to use Ov7725");
}
Ov7725::~Ov7725() {}
void Ov7725::Start() {}
void Ov7725::Stop() {}
const Byte* Ov7725::LockBuffer() { return nullptr; }
void Ov7725::UnlockBuffer() {}

#endif /* LIBSC_USE_OV7725 */

}
}
