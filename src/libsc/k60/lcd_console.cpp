/*
 * lcd_console.cpp
 * Use the LCD screen as console
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "libsc/k60/lcd_console.h"
#include "libsc/k60/st7735r.h"

namespace libsc
{
namespace k60
{

LcdConsole::LcdConsole(St7735r *const lcd)
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
			PrintChar(' ', 0, bg_color);
		} while (m_cursor_x != 0);
	}
	else
	{
		CellData *cell = &m_buffer[m_cursor_y * GetMaxTextW() + m_cursor_x];
		if (cell->ch != ch || cell->color != color || cell->bg_color != bg_color)
		{
			m_lcd->DrawChar(m_cursor_x * St7735r::kFontW,
					m_cursor_y * St7735r::kFontH, ch, color, bg_color);
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
}
