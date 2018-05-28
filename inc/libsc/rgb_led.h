/*
 * rgb_led.h
 *
 * Author: LeeChunHei
 * Copyright (c) 2017-2018 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/helper.h"
#include LIBBASE_H(gpio)

namespace libsc
{

class RGBLed
{
public:
	struct Config
	{
		uint8_t id;
		bool is_active_low;
	};

	enum Color {
		kWhite, kRed, kGreen, kBlue, kYellow, kMagenta, kCyan
	};

	explicit RGBLed(const Config &config);

	void SetEnable(const bool flag);
	void SetColor(const Color color);

private:
	LIBBASE_MODULE(Gpo) m_r_pin;LIBBASE_MODULE(Gpo) m_g_pin;LIBBASE_MODULE(Gpo) m_b_pin;
	bool m_is_active_low;
};

}

