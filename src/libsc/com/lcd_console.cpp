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
#include <cstring>

#include "libsc/com/lcd.h"
#include "libsc/com/lcd_console.h"

namespace libsc
{

LcdConsole::LcdConsole(Lcd *const lcd)
		: m_lcd(lcd), m_cursor_x(0), m_cursor_y(0)
{
	Clear(true);
}

void LcdConsole::PrintChar(const char ch, const uint16_t color,
		const uint16_t bg_color)
{
	if (ch == '\n')
	{
		do
		{
			PrintChar(' ');
		} while (m_cursor_x != 0);
	}
	else
	{
		CellData *cell = &m_buffer[m_cursor_y * GetMaxTextW() + m_cursor_x];
		if (cell->ch != ch || cell->color != color || cell->bg_color != bg_color)
		{
			m_lcd->DrawChar(m_cursor_x * Lcd::FONT_W, m_cursor_y * Lcd::FONT_H,
					ch, color, bg_color);
		}
		cell->ch = ch;
		cell->color = color;
		cell->bg_color = bg_color;
		NewChar();
	}
}

void LcdConsole::PrintString(const char *str, const uint16_t color,
		const uint16_t bg_color)
{
	while (*str)
	{
		PrintChar(*str, color, bg_color);
		++str;
	}
}

void LcdConsole::PrintRawString(const char *str, const size_t len,
		const uint16_t color, const uint16_t bg_color)
{
	for (size_t i = len; i && *str; --i, ++str)
	{
		PrintChar(*str, color, bg_color);
	}
}

void LcdConsole::Clear(const bool is_clear_screen)
{
	if (is_clear_screen)
	{
		m_lcd->Clear(0);
	}
	m_cursor_x = 0;
	m_cursor_y = 0;
	memset(m_buffer, 0, sizeof(m_buffer));
}

}
