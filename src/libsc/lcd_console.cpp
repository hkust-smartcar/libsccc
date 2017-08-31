/*
 * lcd_console.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <algorithm>

#include "libsc/lcd_console.h"
#include "libsc/lcd_typewriter.h"
#include "libsc/st7735r.h"

namespace libsc
{

namespace
{

LcdTypewriter::Config GetLcdTypewriterConfig(const LcdConsole::Config &config)
{
	LcdTypewriter::Config product;
	product.lcd = config.lcd;
	product.text_color = config.text_color;
	product.bg_color = config.bg_color;
	return product;
}

}

LcdConsole::LcdConsole(const Config &config)
		: m_lcd(config.lcd),
		  m_typewriter(GetLcdTypewriterConfig(config)),
		  m_region(config.region),
		  m_cursor_x(0),
		  m_cursor_y(0),
		  m_max_text_x(std::max<Uint>(m_region.w / LcdTypewriter::GetFontW(), 1)),
		  m_max_text_y(std::max<Uint>(m_region.h / LcdTypewriter::GetFontH(), 1)),
		  m_buffer(new CellData[m_max_text_x * m_max_text_y])
{
	Clear(true);
}

void LcdConsole::WriteChar(const char ch)
{
	if (ch == '\n')
	{
		do
		{
			WriteChar(' ');
		} while (m_cursor_x != 0);
	}
	else
	{
		CellData *cell = &m_buffer[m_cursor_y * m_max_text_x + m_cursor_x];
		if (cell->ch != ch || cell->color != m_typewriter.GetTextColor()
				|| cell->bg_color != m_typewriter.GetBgColor())
		{
			const Lcd::Rect &region = m_lcd->GetRegion();
			m_lcd->SetRegion(Lcd::Rect{
					m_cursor_x * LcdTypewriter::GetFontW() + m_region.x,
					m_cursor_y * LcdTypewriter::GetFontH() + m_region.y,
					LcdTypewriter::GetFontW(), LcdTypewriter::GetFontH()});
			m_typewriter.WriteChar(ch);
			m_lcd->SetRegion(region);

			cell->ch = ch;
			cell->color = m_typewriter.GetTextColor();
			cell->bg_color = m_typewriter.GetBgColor();
		}
		NewChar();
	}
}

void LcdConsole::WriteString(const char *str)
{
	while (*str)
	{
		WriteChar(*str);
		++str;
	}
}

void LcdConsole::WriteBuffer(const char *buf, const size_t length)
{
	for (size_t i = length; i; --i)
	{
		WriteChar(*buf++);
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
	memset(m_buffer.get(), 0, sizeof(CellData) * m_max_text_x * m_max_text_y);
}

inline void LcdConsole::NewChar()
{
	if (++m_cursor_x >= m_max_text_x)
	{
		NewLine();
		m_cursor_x = 0;
	}
}

inline void LcdConsole::NewLine()
{
	if (++m_cursor_y >= m_max_text_y)
	{
		m_cursor_y = 0;
	}
	m_cursor_x = 0;
}

}
