/*
 * lcd_typewriter.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "libbase/misc_types.h"

#include "libsc/k60/st7735r.h"

namespace libsc
{
namespace k60
{

/**
 * Draw text on Lcd. Working only with the Lcd interface, this class is rather
 * hardware independent. Since we have no plan (and largely unnecessary based on
 * the goal of this library) to support more than 1 font, the font data is also
 * being stored in this class
 */
class LcdTypewriter
{
public:
	typedef St7735r Lcd;

	struct Config
	{
		Lcd *lcd;
		uint16_t text_color = 0xFFFF;
		uint16_t bg_color = 0;
	};

	explicit LcdTypewriter(const Config &config);

	void WriteChar(const char ch);
	void WriteString(const char *str);
	void WriteBuffer(const char *buf, const size_t length);

	void SetTextColor(const uint16_t color)
	{
		m_fg_color = color;
	}

	void SetBgColor(const uint16_t color)
	{
		m_bg_color = color;
	}

	static constexpr Uint GetFontW()
	{
		return kFontW;
	}

	static constexpr Uint GetFontH()
	{
		return kFontH;
	}

private:
	Lcd *m_lcd;
	uint16_t m_fg_color;
	uint16_t m_bg_color;

	static constexpr uint8_t kFontW = 8;
	static constexpr uint8_t kFontH = 16;
	static const Byte kFontData[];
};

}
}
