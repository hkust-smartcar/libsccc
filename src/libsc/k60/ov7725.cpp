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
#include "libbase/k60/vectors.h"

#include "libsc/config.h"
#include "libsc/device_h/ov7725.h"
#include "libsc/k60/ov7725.h"
#include "libsc/k60/ov7725_configurator.h"
#include "libsc/system.h"
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

Ov7725Configurator::Config GetConfiguratorConfig(const Ov7725::Config &config)
{
	Ov7725Configurator::Config product;
	product.scl = LIBSC_OV77250_SCL;
	product.sda = LIBSC_OV77250_SDA;
	product.w = config.w;
	product.h = config.h;
	product.fps = config.fps;
	product.brightness = config.brightness;
	product.contrast = config.contrast;
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
		: m_config(GetConfiguratorConfig(config)),
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
		: m_config(nullptr), m_data_array(nullptr), m_dma(nullptr), m_w(0),
		  m_h(0), m_buf_size(0), m_is_shoot(false), m_is_lock_buffer(false),
		  m_is_available(false), m_is_dma_start(false)
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
