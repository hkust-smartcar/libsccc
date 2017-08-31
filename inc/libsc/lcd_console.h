/*
 * lcd_console.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include <memory>

#include "libsc/lcd_typewriter.h"
#include "libsc/st7735r.h"
#include "libutil/misc.h"

namespace libsc
{

/**
 * Print text on screen with a managed buffer. Only changed contents are updated
 */
class LcdConsole
{
public:
	// Conditionally select Lcd implementation here. Should prevent working with
	// the interface directly (performance)
	typedef St7735r Lcd;

	struct Config
	{
		Lcd *lcd = nullptr;
		/// The screen region for this console
		Lcd::Rect region = Lcd::Rect{0, 0, Lcd::GetW(), Lcd::GetH()};
		uint16_t text_color = 0xFFFF;
		uint16_t bg_color = 0;
	};

	explicit LcdConsole(const Config &config);

	void WriteChar(const char ch);
	void WriteString(const char *str);
	void WriteBuffer(const char *buf, const size_t length);

	void Clear(const bool is_clear_screen);

	void SetCursorRow(const uint8_t row)
	{
		m_cursor_x = 0;
		m_cursor_y = libutil::ClampVal<Uint>(0, row, m_max_text_y);
	}

	void SetTextColor(const uint16_t color)
	{
		m_typewriter.SetTextColor(color);
	}

	void SetBgColor(const uint16_t color)
	{
		m_typewriter.SetBgColor(color);
	}

private:
	struct CellData
	{
		char ch;
		uint16_t color;
		uint16_t bg_color;
	};

	inline void NewChar();
	inline void NewLine();

	Lcd *const m_lcd;
	LcdTypewriter m_typewriter;
	Lcd::Rect m_region;
	Uint m_cursor_x;
	Uint m_cursor_y;

	Uint m_max_text_x;
	Uint m_max_text_y;

	std::unique_ptr<CellData[]> m_buffer;
};

}
