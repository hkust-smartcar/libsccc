/*
 * lcd_console.h
 * Use the LCD screen as console
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_LCD_CONSOLE_H_
#define LIBSC_LCD_CONSOLE_H_

#include <cstddef>
#include <cstdint>

#include "libsc/com/lcd.h"

namespace libsc
{

namespace internal
{

inline constexpr int LcdConsoleGetMaxTextW()
{
	return Lcd::W / Lcd::FONT_W;
}

inline constexpr int LcdConsoleGetMaxTextH()
{
	return Lcd::H / Lcd::FONT_H;
}

}

class LcdConsole
{
public:
	explicit LcdConsole(Lcd *const lcd);

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

private:
	struct CellData
	{
		char ch;
		uint16_t color;
		uint16_t bg_color;
	};

	static constexpr int GetMaxTextW()
	{
		return internal::LcdConsoleGetMaxTextW();
	}

	static constexpr int GetMaxTextH()
	{
		return internal::LcdConsoleGetMaxTextH();
	}

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

	Lcd *const m_lcd;
	uint8_t m_cursor_x;
	uint8_t m_cursor_y;

	CellData m_buffer[internal::LcdConsoleGetMaxTextW()
			* internal::LcdConsoleGetMaxTextH()];
};

}

#endif /* LIBSC_LCD_CONSOLE_H_ */
