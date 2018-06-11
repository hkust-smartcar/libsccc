/*
 * rgb_led.cpp
 *
 * Author: LeeChunHei
 * Copyright (c) 2017-2018 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstdint>

#include "libbase/log.h"
#include "libbase/helper.h"
#include LIBBASE_H(gpio)

#include "libsc/config.h"
#include "libsc/rgb_led.h"

using namespace LIBBASE_NS;

namespace libsc {

#ifdef LIBSC_USE_RGB_LED

namespace
{

#if LIBSC_USE_RGB_LED == 1
	inline Pin::Name GetRPin(const uint8_t id)
	{
		if (id != 0)
		{
			assert(false);
		}
		return LIBSC_LED0_R;
	}

#else
	inline Pin::Name GetRPin(const uint8_t id)
	{
		switch (id)
		{
			default:
			assert(false);
			// no break

			case 0:
			return LIBSC_LED0_R;

#if LIBSC_USE_RGB_LED > 1
			case 1:
			return LIBSC_LED1_R;
#endif

#if LIBSC_USE_RGB_LED > 2
			case 2:
			return LIBSC_LED2_R;
#endif

#if LIBSC_USE_RGB_LED > 3
			case 3:
			return LIBSC_LED3_R;
#endif
		}
	}

#endif

#if LIBSC_USE_RGB_LED == 1
	inline Pin::Name GetGPin(const uint8_t id)
	{
		if (id != 0)
		{
			assert(false);
		}
		return LIBSC_LED0_G;
	}

#else
	inline Pin::Name GetGPin(const uint8_t id)
	{
		switch (id)
		{
			default:
			assert(false);
			// no break

			case 0:
			return LIBSC_LED0_G;

#if LIBSC_USE_RGB_LED > 1
			case 1:
			return LIBSC_LED1_G;
#endif

#if LIBSC_USE_RGB_LED > 2
			case 2:
			return LIBSC_LED2_G;
#endif

#if LIBSC_USE_RGB_LED > 3
			case 3:
			return LIBSC_LED3_G;
#endif
		}
	}

#endif

#if LIBSC_USE_RGB_LED == 1
	inline Pin::Name GetBPin(const uint8_t id)
	{
		if (id != 0)
		{
			assert(false);
		}
		return LIBSC_LED0_B;
	}

#else
	inline Pin::Name GetBPin(const uint8_t id)
	{
		switch (id)
		{
			default:
			assert(false);
			// no break

			case 0:
			return LIBSC_LED0_B;

#if LIBSC_USE_RGB_LED > 1
			case 1:
			return LIBSC_LED1_B;
#endif

#if LIBSC_USE_RGB_LED > 2
			case 2:
			return LIBSC_LED2_B;
#endif

#if LIBSC_USE_RGB_LED > 3
			case 3:
			return LIBSC_LED3_B;
#endif
		}
	}

#endif

	Gpo::Config GetRGpoConfig(const RGBLed::Config &config)
	{
		Gpo::Config product;
		product.pin = GetRPin(config.id);
		// Default off
		product.is_high = config.is_active_low;
		product.config.set(Pin::Config::ConfigBit::kHighDriveStrength);
		return product;
	}

	Gpo::Config GetGGpoConfig(const RGBLed::Config &config)
	{
		Gpo::Config product;
		product.pin = GetGPin(config.id);
		// Default off
		product.is_high = config.is_active_low;
		product.config.set(Pin::Config::ConfigBit::kHighDriveStrength);
		return product;
	}

	Gpo::Config GetBGpoConfig(const RGBLed::Config &config)
	{
		Gpo::Config product;
		product.pin = GetBPin(config.id);
		// Default off
		product.is_high = config.is_active_low;
		product.config.set(Pin::Config::ConfigBit::kHighDriveStrength);
		return product;
	}

}

RGBLed::RGBLed(const Config &config)
: m_r_pin(GetRGpoConfig(config)),
m_g_pin(GetGGpoConfig(config)),
m_b_pin(GetBGpoConfig(config)),
m_is_active_low(config.is_active_low)
{}

void RGBLed::SetEnable(const bool flag)
{
	m_r_pin.Set(flag ^ m_is_active_low);
	m_g_pin.Set(flag ^ m_is_active_low);
	m_b_pin.Set(flag ^ m_is_active_low);
}

void RGBLed::SetColor(const RGBLed::Color color) {
	switch(color) {
		case RGBLed::Color::kWhite:
		m_r_pin.Set(!m_is_active_low);
		m_g_pin.Set(!m_is_active_low);
		m_b_pin.Set(!m_is_active_low);
		break;
		case RGBLed::Color::kRed:
		m_r_pin.Set(!m_is_active_low);
		m_g_pin.Set(m_is_active_low);
		m_b_pin.Set(m_is_active_low);
		break;
		case RGBLed::Color::kGreen:
		m_r_pin.Set(m_is_active_low);
		m_g_pin.Set(!m_is_active_low);
		m_b_pin.Set(m_is_active_low);
		break;
		case RGBLed::Color::kBlue:
		m_r_pin.Set(m_is_active_low);
		m_g_pin.Set(m_is_active_low);
		m_b_pin.Set(!m_is_active_low);
		break;
		case RGBLed::Color::kYellow:
		m_r_pin.Set(!m_is_active_low);
		m_g_pin.Set(!m_is_active_low);
		m_b_pin.Set(m_is_active_low);
		break;
		case RGBLed::Color::kMagenta:
		m_r_pin.Set(!m_is_active_low);
		m_g_pin.Set(m_is_active_low);
		m_b_pin.Set(!m_is_active_low);
		break;
		case RGBLed::Color::kCyan:
		m_r_pin.Set(m_is_active_low);
		m_g_pin.Set(!m_is_active_low);
		m_b_pin.Set(!m_is_active_low);
		break;
	}
}

#else
RGBLed::RGBLed(const Config&) :
		m_r_pin(nullptr), m_g_pin(nullptr), m_b_pin(nullptr), m_is_active_low(false) {
	LOG_DL("Configured not to use RGBLed");
}
void RGBLed::SetEnable(const bool) {
}
void RGBLed::SetColor(const RGBLed::Color color) {
}
#endif

}
