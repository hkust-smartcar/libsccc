/*
 * lcd_console.h
 * Use the LCD screen as console
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "libsc/k60/st7735r.h"
#include "libutil/misc.h"

namespace libsc
{
namespace k60
{

namespace internal
{

inline constexpr int LcdConsoleGetMaxTextW()
{
	return St7735r::kW / St7735r::kFontW;
}

inline constexpr int LcdConsoleGetMaxTextH()
{
	return St7735r::kH / St7735r::kFontH;
}

}

class LcdConsole
{
public:
	explicit LcdConsole(St7735r *const lcd);

	void PrintChar(const char ch, const uint16_t color, const uint16_t bg_color);
	void PrintString(const char *str, const uint16_t color,
			const uint16_t bg_color);
	void PrintRawString(const char *str, const size_t len,
			const uint16_t color, const uint16_t bg_color);

	void PrintChar(const char ch, const uint16_t color)
	{
		PrintChar(ch, color, 0);
	}

	void PrintString(const char *str, const uint16_t color)
	{
		PrintString(str, color, 0);
	}

	void PrintRawString(const char *str, const size_t len, const uint16_t color)
	{
		PrintRawString(str, len, color, 0);
	}

	void PrintChar(const char ch)
	{
		PrintChar(ch, 0xFFFF, 0);
	}

	void PrintString(const char *str)
	{
		PrintString(str, 0xFFFF, 0);
	}

	void PrintRawString(const char *str, const size_t len)
	{
		PrintRawString(str, len, 0xFFFF, 0);
	}

	void Clear(const bool is_clear_screen);

	void SetCursorRow(const uint8_t row)
	{
		m_cursor_x = 0;
		m_cursor_y = libutil::ClampVal<uint8_t>(0, row, GetMaxTextH());
	}

	static constexpr int GetMaxTextW()
	{
		return internal::LcdConsoleGetMaxTextW();
	}

	static constexpr int GetMaxTextH()
	{
		return internal::LcdConsoleGetMaxTextH();
	}

private:
	struct CellData
	{
		char ch;
		uint16_t color;
		uint16_t bg_color;
	};

	void NewChar()
	{
		if (++m_cursor_x >= GetMaxTextW())
		{
			NewLine();
			m_cursor_x = 0;
		}
	}

	void NewLine()
	{
		if (++m_cursor_y >= GetMaxTextH())
		{
			m_cursor_y = 0;
		}
		m_cursor_x = 0;
	}

	St7735r *const m_lcd;
	uint8_t m_cursor_x;
	uint8_t m_cursor_y;

	CellData m_buffer[internal::LcdConsoleGetMaxTextW()
			* internal::LcdConsoleGetMaxTextH()];
};

}
}
