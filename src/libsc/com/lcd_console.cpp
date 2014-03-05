/*
 * lcd_console.cpp
 * Use the LCD screen as console
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>
#include <cstddef>
#include <cstdint>

#include "libsc/com/lcd.h"
#include "libsc/com/lcd_console.h"

namespace libsc
{

LcdConsole::LcdConsole(Lcd *const lcd)
		: m_lcd(lcd), m_cursor_x(0), m_cursor_y(0)
{
	m_lcd->Clear(0);
}

void LcdConsole::PrintChar(const char ch, const uint16_t color,
		const uint16_t bg_color)
{
	m_lcd->DrawChar(m_cursor_x, m_cursor_y, ch, color, bg_color);
	NewChar();
}

void LcdConsole::PrintString(const char *str, const uint16_t color,
		const uint16_t bg_color)
{
	while (*str)
	{
		if (*str == '\n')
		{
			NewLine();
		}
		else
		{
			m_lcd->DrawChar(m_cursor_x, m_cursor_y, *str, color, bg_color);
			NewChar();
		}
		++str;
	}
}

void LcdConsole::PrintRawString(const char *str, const size_t len,
		const uint16_t color, const uint16_t bg_color)
{
	for (size_t i = len; len && *str; --i, ++str)
	{
		if (*str == '\n')
		{
			NewLine();
		}
		else
		{
			m_lcd->DrawChar(m_cursor_x, m_cursor_y, *str, color, bg_color);
			NewChar();
		}
	}
}

void LcdConsole::PrintSplashScreen()
{
	Clear();
	m_cursor_x = 44;
	m_cursor_y = 0;
	PrintString("HKUST");
	m_cursor_x = 12;
	m_cursor_y += 16;
	PrintString("Robotics Team");
	m_cursor_x = 4;
	m_cursor_y += 16;
	PrintString("IntelliCar 2014");
	m_cursor_x = 0;
	m_cursor_y += 32;
	PrintString("Louis Harrison");
	m_cursor_x = 0;
	m_cursor_y += 16;
	PrintString("Ben Cindy David");
	m_cursor_x = 0;
	m_cursor_y += 16;
	PrintString("Fai Jimmy Ming");
	m_cursor_x = 0;
	m_cursor_y += 16;
	PrintString("Ryan Spartey");
	m_cursor_x = 0;
	m_cursor_y += 16;
	PrintString("Vibhor Yumi");

	m_cursor_x = 0;
	m_cursor_y = 0;
}

void LcdConsole::Clear()
{
	m_lcd->Clear(0);
	m_cursor_x = 0;
	m_cursor_y = 0;
}

}
