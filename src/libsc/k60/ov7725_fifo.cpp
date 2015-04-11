/*
 * ov7725_fifo.cpp
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
#include <vector>

#include "libbase/log.h"
#include "libbase/k60/gpio.h"
#include "libbase/k60/gpio_array.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/pin_utils.h"
#include "libbase/k60/vectors.h"

#include "libsc/config.h"
#include "libsc/k60/al422b.h"
#include "libsc/k60/ov7725_configurator.h"
#include "libsc/k60/ov7725_fifo.h"
#include "libsc/system.h"
#include "libutil/misc.h"

using namespace libbase::k60;
using namespace libutil;
using namespace std;

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_OV7725_FIFO

namespace
{

#if LIBSC_USE_OV7725_FIFO == 1
inline Pin::Name GetSclPin(const uint8_t)
{
	return LIBSC_OV7725_FIFO0_SCL;
}

inline Pin::Name GetSdaPin(const uint8_t)
{
	return LIBSC_OV7725_FIFO0_SDA;
}

inline Pin::Name GetWenPin(const uint8_t)
{
	return LIBSC_OV7725_FIFO0_WEN;
}

inline Pin::Name GetWrstPin(const uint8_t)
{
	return LIBSC_OV7725_FIFO0_WRST;
}

inline Pin::Name GetData0Pin(const uint8_t)
{
	return LIBSC_OV7725_FIFO0_DATA0;
}

inline Pin::Name GetRclkPin(const uint8_t)
{
	return LIBSC_OV7725_FIFO0_RCLK;
}

inline Pin::Name GetRrstPin(const uint8_t)
{
	return LIBSC_OV7725_FIFO0_RRST;
}

inline Pin::Name GetVsyncPin(const uint8_t)
{
	return LIBSC_OV7725_FIFO0_VSYNC;
}

#else
inline Pin::Name GetSclPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break
	case 0:
		return LIBSC_OV7725_FIFO0_SCL;

#if LIBSC_USE_OV7725 > 1
	case 1:
		return LIBSC_OV7725_FIFO1_SCL;
#endif
	}
}

inline Pin::Name GetSdaPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break
	case 0:
		return LIBSC_OV7725_FIFO0_SDA;

#if LIBSC_USE_OV7725 > 1
	case 1:
		return LIBSC_OV7725_FIFO1_SDA;
#endif
	}
}

inline Pin::Name GetWenPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break
	case 0:
		return LIBSC_OV7725_FIFO0_WEN;

#if LIBSC_USE_OV7725 > 1
	case 1:
		return LIBSC_OV7725_FIFO1_WEN;
#endif
	}
}

inline Pin::Name GetWrstPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break
	case 0:
		return LIBSC_OV7725_FIFO0_WRST;

#if LIBSC_USE_OV7725 > 1
	case 1:
		return LIBSC_OV7725_FIFO1_WRST;
#endif
	}
}

inline Pin::Name GetData0Pin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break
	case 0:
		return LIBSC_OV7725_FIFO0_DATA0;

#if LIBSC_USE_OV7725 > 1
	case 1:
		return LIBSC_OV7725_FIFO1_DATA0;
#endif
	}
}

inline Pin::Name GetRclkPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break
	case 0:
		return LIBSC_OV7725_FIFO0_RCLK;

#if LIBSC_USE_OV7725 > 1
	case 1:
		return LIBSC_OV7725_FIFO1_RCLK;
#endif
	}
}

inline Pin::Name GetRrstPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break
	case 0:
		return LIBSC_OV7725_FIFO0_RRST;

#if LIBSC_USE_OV7725 > 1
	case 1:
		return LIBSC_OV7725_FIFO1_RRST;
#endif
	}
}

inline Pin::Name GetVsyncPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break
	case 0:
		return LIBSC_OV7725_FIFO0_VSYNC;

#if LIBSC_USE_OV7725 > 1
	case 1:
		return LIBSC_OV7725_FIFO1_VSYNC;
#endif
	}
}

#endif /* LIBSC_USE_OV7725_FIFO == 1 */

Ov7725Configurator::Config GetConfiguratorConfig(
		const Ov7725Fifo::Config &config)
{
	Ov7725Configurator::Config product;
	product.scl = GetSclPin(config.id);
	product.sda = GetSdaPin(config.id);
	product.w = config.w;
	product.h = config.h;
	product.fps = config.fps;
	product.format = Ov7725Configurator::Config::Format::kRgb;
	product.rgb_format = Ov7725Configurator::Config::RgbFormat::kRgb565;
	product.brightness = config.brightness;
	product.contrast = config.contrast;
	return product;
}

Al422b::Config GetFifoConfig(const Ov7725Fifo::Config &config)
{
	Al422b::Config product;
	product.wrst = GetWrstPin(config.id);
	product.do0 = GetData0Pin(config.id);
	product.rck = GetRclkPin(config.id);
	product.rrst = GetRrstPin(config.id);
	return product;
}

Gpi::Config GetClockConfig(const Ov7725Fifo::Config &config)
{
	Gpi::Config product;
	product.pin = GetRclkPin(config.id);
	product.config.set(Pin::Config::ConfigBit::kPullEnable);
	product.interrupt = Pin::Config::Interrupt::kDmaFalling;
	return product;
}

Gpo::Config GetWenConfig(const Ov7725Fifo::Config &config)
{
	Gpo::Config product;
	product.pin = GetWenPin(config.id);
	product.is_high = false;
	return product;
}

Gpi::Config GetVsyncConfig(const Ov7725Fifo::Config &config,
		Gpi::OnGpiEventListener isr)
{
	Gpi::Config product;
	product.pin = GetVsyncPin(config.id);
	product.config.set(Pin::Config::ConfigBit::kPassiveFilter);
	product.config.set(Pin::Config::ConfigBit::kPullEnable);
	product.config.set(Pin::Config::ConfigBit::kPullUp);
	product.interrupt = Pin::Config::Interrupt::kRising;
	product.isr = isr;
	return product;
}

}

Ov7725Fifo::Ov7725Fifo(const Config &config)
		: m_config(GetConfiguratorConfig(config)),
		  m_fifo(GetFifoConfig(config)),
		  m_wen(GetWenConfig(config)),
		  m_vsync(nullptr),
		  m_w(libutil::Clamp<Uint>(1, config.w, 640)),
		  m_h(libutil::Clamp<Uint>(1, config.h, 480)),
		  m_write_state(State::kIdle)
{
	m_vsync = Gpi(GetVsyncConfig(config, std::bind(&Ov7725Fifo::OnVsync, this,
			placeholders::_1)));
}

Ov7725Fifo::~Ov7725Fifo()
{}

void Ov7725Fifo::Start()
{
	if (m_fifo.IsReadEnd())
	{
		m_write_state = State::kReqStart;
	}
}

bool Ov7725Fifo::ReadStep()
{
	// Prevent returning true while still waiting for the initial vsync
	if (m_write_state != State::kIdle)
	{
		return false;
	}
	else
	{
		return m_fifo.ReadStep();
	}
}

void Ov7725Fifo::OnVsync(Gpi*)
{
	LOG_VL("Ov7725Fifo vsync");
	switch (m_write_state)
	{
	default:
		assert(false);
		// no break
	case State::kIdle:
		break;

	case State::kReqStart:
		m_fifo.ResetWrite();
		m_wen.Set();
		m_write_state = State::kStart;
		break;

	case State::kStart:
		m_wen.Reset();
		m_fifo.ResetRead();
		m_fifo.Start(m_w * m_h * 2);
		m_write_state = State::kIdle;
		break;
	}
}

vector<uint16_t> Ov7725Fifo::GetRgb565Data() const
{
	vector<uint16_t> product(m_w * m_h);
	const vector<Byte> &raw = GetData();
	for (size_t i = 0; i < m_fifo.GetDataSize(); ++++i)
	{
		product[i >> 1] = (raw[i] << 8) | raw[i + 1];
	}
	return product;
}

#else
Ov7725Fifo::Ov7725Fifo(const Config&)
		: m_config(nullptr), m_fifo(nullptr), m_w(0), m_h(0),
		  m_write_state(State::kIdle)
{
	LOG_DL("Configured not to use Ov7725");
}
Ov7725Fifo::~Ov7725Fifo() {}
void Ov7725Fifo::Start() {}
bool Ov7725Fifo::ReadStep() { return false; }
vector<uint16_t> Ov7725Fifo::GetRgb565Data() const { return {}; }

#endif /* LIBSC_USE_OV7725_FIFO */

}
}
