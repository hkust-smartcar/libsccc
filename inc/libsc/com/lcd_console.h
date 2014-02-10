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

#include "lcd.h"

namespace libsc
{

class LcdConsole
{
public:
	explicit LcdConsole(Lcd *const lcd);

	void PrintChar(const char ch, const uint16_t color, const uint16_t bg_color);
	void PrintString(const char *str, const uint16_t color,
			const uint16_t bg_color);
	void PrintRawString(const char *str, const size_t len,
			const uint16_t color, const uint16_t bg_color);
	void PrintSplashScreen();

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

	void Clear()
	{
		m_lcd->Clear(0);
	}

private:
	void NewChar()
	{
		if ((m_cursor_x += Lcd::FONT_W) >= Lcd::W)
		{
			NewLine();
			m_cursor_x = 0;
		}
	}

	void NewLine()
	{
		if ((m_cursor_y += Lcd::FONT_H) >= Lcd::H)
		{
			m_cursor_y = 0;
		}
	}

	Lcd *const m_lcd;
	uint8_t m_cursor_x;
	uint8_t m_cursor_y;
};

}

#endif /* LIBSC_LCD_CONSOLE_H_ */
