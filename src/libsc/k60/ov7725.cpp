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
	assert(config.id < LIBSC_USE_OV7725);
	Ov7725Configurator::Config product;

	switch (config.id)
	{
	case 0:
		product.scl = LIBSC_OV77250_SCL;
		product.sda = LIBSC_OV77250_SDA;
		break;

#if LIBSC_USE_OV7725 > 1
	case 1:
		product.scl = LIBSC_OV77251_SCL;
		product.sda = LIBSC_OV77251_SDA;
		break;
#endif
	}

	product.w = config.w;
	product.h = config.h;
	product.fps = config.fps;
	product.brightness = config.brightness;
	product.contrast = config.contrast;
	return product;
}

GpiArray::Config GetGpiArrayConfig(const uint8_t id)
{
	GpiArray::Config product;

	switch (id)
	{
	case 0:
		product.start_pin = LIBSC_OV77250_DATA0;
		break;

#if LIBSC_USE_OV7725 > 1
	case 1:
		product.start_pin = LIBSC_OV77251_DATA0;
		break;
#endif
	}

	product.count = 8;
	product.config.set(Pin::Config::ConfigBit::kPullEnable);
	return product;
}

Gpi::Config GetClockConfig(const uint8_t id)
{
	Gpi::Config product;

	switch (id)
	{
	case 0:
		product.pin = LIBSC_OV77250_PCLK;
		break;

#if LIBSC_USE_OV7725 > 1
	case 1:
		product.pin = LIBSC_OV77251_PCLK;
		break;
#endif
	}

	product.config.set(Pin::Config::ConfigBit::kPullEnable);
	product.interrupt = Pin::Config::Interrupt::kDmaFalling;
	return product;
}

Gpi::Config GetVsyncConfig(const uint8_t id, Gpi::OnGpiEventListener isr)
{
	Gpi::Config product;

	switch (id)
	{
	case 0:
		product.pin = LIBSC_OV77250_VSYNC;
		break;

#if LIBSC_USE_OV7725 > 1
	case 1:
		product.pin = LIBSC_OV77251_VSYNC;
		break;
#endif
	}

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
		  m_data_array(GetGpiArrayConfig(config.id)),
		  m_clock(GetClockConfig(config.id)),
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

	InitDma(config.id);

	m_vsync = Gpi(GetVsyncConfig(config.id, std::bind(&Ov7725::OnVsync, this,
			placeholders::_1)));
	// Set DMA to a higher priority to prevent VSYNC being processed earlier
	NVIC_SetPriority(DMA1_DMA17_IRQn, __BASE_IRQ_PRIORITY - 1);
}

Ov7725::~Ov7725()
{
	DmaManager::Delete(m_dma);
	NVIC_SetPriority(DMA1_DMA17_IRQn, __BASE_IRQ_PRIORITY);
}

void Ov7725::InitDma(const uint8_t id)
{
	Dma::Config config;
	m_data_array.ConfigValueAsDmaSrc(&config);
	config.dst.addr = m_back_buf.get();
	config.dst.offset = 1;
	config.dst.size = Dma::Config::TransferSize::k1Byte;
	config.dst.major_offset = -m_buf_size;
	config.major_count = m_buf_size;
	config.complete_isr = std::bind(&Ov7725::OnDmaComplete, this,
			placeholders::_1);

	switch (id)
	{
	case 0:
		config.mux_src = EnumAdvance(DmaMux::Source::kPortA,
				PinUtils::GetPort(LIBSC_OV77250_PCLK));
		m_dma = DmaManager::New(config, LIBSC_OV77250_DMA_CH);
		break;

#if LIBSC_USE_OV7725 > 1
	case 1:
		config.mux_src = EnumAdvance(DmaMux::Source::kPortA,
				PinUtils::GetPort(LIBSC_OV77251_PCLK));
		m_dma = DmaManager::New(config, LIBSC_OV77251_DMA_CH);
		break;
#endif
	}
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
