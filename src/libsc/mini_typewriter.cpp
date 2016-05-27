/*
 * MiniTypeWriter.cpp
 *
 * Author: petel__
 * Copyright (c) 2014-2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include <algorithm>
#include <memory>
#include <vector>

#include <libsc/config.h>
#include <libsc/lcd.h>
#include <libsc/mini_typewriter.h>

using namespace std;

namespace libsc
{

MiniTypeWriter::MiniTypeWriter(const Config &config)
		: m_lcd(config.lcd),
		  m_fg_color(config.text_color),
		  m_bg_color(config.bg_color),
		  m_is_text_wrap(config.is_text_wrap),
		  m_is_clear_line(config.is_clear_line),
		  kCursorX(0),
		  kCursorY(0),
		  cursorsPos{ 0 }
{
	assert(config.lcd);
	m_lcd->Clear(m_bg_color);
}

bool MiniTypeWriter::setActiveRegion(const uint8_t cursorX, const uint8_t cursorY)
{
	if (checkInRange(0, cursorX, kMaxColumn) && checkInRange(0, cursorY, kMaxRow))
	{
		m_lcd->SetRegion(Lcd::Rect(cursorX * 6, cursorY * 10, 6, 10));
		return true;
	}
	return false;
}

void MiniTypeWriter::WriteChar(const char c)
{
	if (c == '\n' || (kCursorX == kMaxColumn && m_is_text_wrap))
	{
		kCursorY++;
		kCursorX = 0;
		return ;
	}
	else if (c == '\t')
	{
		if (kCursorX == kMaxColumn && m_is_text_wrap)
		{
			kCursorY++;
			kCursorX = 0;
		}
		else
			for (uint8_t i = ((kCursorX >> 1) + 1) << 1; kCursorX <= i;)
				WriteChar(CharTable_Size5[0]);

		return ;
	}
	WriteChar(CharTable_Size5[c - 32]);
}

void MiniTypeWriter::WriteChar(const uint8_t dataArray[10])
{
	if (!setActiveRegion(kCursorX++, kCursorY))
		return ;

	bool bitArray[6 * 10] = { false };

	for (int8_t y = 0; y < 10; y++)
		for (int8_t x = 0; x < 6; x++)
			bitArray[6 * y + x] = (bool)((dataArray[y] >> (5 - x)) & 1);

	m_lcd->FillBits(m_fg_color, m_bg_color, bitArray, 6 * 10);

	cursorsPos[kCursorY]++;

	m_lcd->ClearRegion();
}

void MiniTypeWriter::WriteString(const char *str)
{
	WriteBuffer(str, strlen(str));
}

void MiniTypeWriter::WriteBuffer(const char *buf, const size_t length)
{
	if (length == 0)
	{
		return;
	}

	uint currentIndex = 0;
	while (currentIndex < length)
	{
		for (uint8_t &x = kCursorX; currentIndex < length; currentIndex++)
		{
			if (buf[currentIndex] == '\n')
			{
				if (m_is_clear_line)
					for (int8_t i = 0; i < cursorsPos[kCursorY] - kCursorX; i++)
						WriteChar(CharTable_Size5[0]);

				cursorsPos[kCursorY] = kCursorX;
				kCursorX = 0;
				kCursorY++; currentIndex++;
				break;
			}
			else if (kCursorX == kMaxColumn)
			{
				if (m_is_text_wrap)
				{
					if (m_is_clear_line)
						for (int8_t i = 0; i < cursorsPos[kCursorY] - kCursorX; i++)
							WriteChar(CharTable_Size5[0]);

					cursorsPos[kCursorY] = kCursorX;
					kCursorX = 0;
					kCursorY++;
				}
				else
					return ;
			}
			else if (buf[currentIndex] == '\t')
			{
				for (uint8_t i = ((kCursorX >> 1) + 1) << 1; kCursorX <= i;)
					WriteChar(CharTable_Size5[0]);
				currentIndex++;
				continue;
			}

			if (checkInRange(31, buf[currentIndex], 126))
				WriteChar(CharTable_Size5[buf[currentIndex] - 32]);
			else
				WriteChar(CharTable_Size5[0]);
		}
	}
}

const uint8_t MiniTypeWriter::CharTable_Size5[][10] =
{
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 4, 4, 4, 4, 4, 0, 0, 4, 0 },
	{ 0, 27, 18, 18, 0, 0, 0, 0, 0, 0 },
	{ 0, 5, 5, 10, 31, 10, 31, 10, 20, 0 },
	{ 0, 2, 7, 8, 8, 7, 1, 14, 2, 0 },
	{ 0, 8, 20, 8, 3, 28, 2, 5, 2, 0 },
	{ 0, 12, 18, 22, 12, 8, 21, 18, 13, 0 }, // &
	{ 0, 2, 2, 2, 2, 0, 0, 0, 0, 0 },
	{ 0, 2, 2, 4, 4, 4, 4, 2, 2, 0 }, // '('
	{ 0, 8, 8, 4, 4, 4, 4, 8, 8, 0 },
	{ 0, 4, 31, 4, 10, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 4, 4, 31, 4, 4, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 6, 4, 12, 8 },
	{ 0, 0, 0, 0, 0, 31, 0, 0, 0, 0 }, // '-'
	{ 0, 0, 0, 0, 0, 0, 0, 12, 12, 0 },
	{ 0, 1, 1, 2, 2, 4, 4, 8, 8, 0 },
	{ 0, 14, 17, 19, 21, 21, 25, 17, 14, 0 }, // '0'
	{ 0, 12, 20, 4, 4, 4, 4, 4, 31, 0 },
	{ 0, 14, 17, 1, 2, 4, 8, 17, 31, 0 },
	{ 0, 14, 17, 1, 6, 1, 1, 17, 14, 0 },
	{ 0, 6, 10, 10, 18, 18, 31, 2, 7, 0 },
	{ 0, 31, 16, 16, 30, 1, 1, 17, 14, 0 }, // '5'
	{ 0, 7, 8, 16, 30, 17, 17, 17, 14, 0 },
	{ 0, 31, 17, 1, 2, 2, 2, 4, 4, 0 },
	{ 0, 14, 17, 17, 14, 17, 17, 17, 14, 0 },
	{ 0, 14, 17, 17, 17, 15, 1, 2, 28, 0 }, // ':'
	{ 0, 0, 0, 12, 12, 0, 0, 12, 12, 0 },
	{ 0, 0, 0, 12, 12, 0, 0, 12, 24, 16 },
	{ 0, 0, 1, 6, 8, 16, 8, 6, 1, 0 },
	{ 0, 0, 0, 0, 31, 0, 31, 0, 0, 0 },
	{ 0, 0, 16, 12, 2, 1, 2, 12, 16, 0 },
	{ 0, 14, 17, 17, 2, 4, 4, 0, 4, 0 },
	{ 0, 14, 17, 19, 21, 19, 16, 17, 14, 0 },
	{ 0, 4, 4, 10, 10, 10, 31, 17, 17, 0 }, // 'A'
	{ 0, 30, 17, 17, 30, 17, 17, 17, 30, 0 },
	{ 0, 14, 17, 16, 16, 16, 16, 17, 14, 0 },
	{ 0, 28, 18, 17, 17, 17, 17, 18, 28, 0 },
	{ 0, 31, 16, 16, 28, 16, 16, 16, 31, 0 },
	{ 0, 31, 16, 16, 28, 16, 16, 16, 16, 0 }, // 'F'
	{ 0, 14, 17, 16, 16, 19, 17, 17, 15, 0 },
	{ 0, 17, 17, 17, 31, 17, 17, 17, 17, 0 },
	{ 0, 14, 4, 4, 4, 4, 4, 4, 14, 0 },
	{ 0, 2, 2, 2, 2, 18, 18, 18, 12, 0 },
	{ 0, 17, 17, 18, 20, 28, 18, 17, 17, 0 }, // 'K'
	{ 0, 16, 16, 16, 16, 16, 16, 16, 31, 0 },
	{ 0, 17, 27, 27, 21, 21, 17, 17, 17, 0 },
	{ 0, 17, 25, 25, 21, 21, 21, 19, 17, 0 },
	{ 0, 14, 17, 17, 17, 17, 17, 17, 14, 0 },
	{ 0, 30, 17, 17, 17, 30, 16, 16, 16, 0 }, // 'P'
	{ 0, 14, 17, 17, 17, 17, 17, 14, 7, 0 },
	{ 0, 30, 17, 17, 17, 30, 18, 17, 17, 0 },
	{ 0, 14, 17, 16, 14, 1, 1, 17, 14, 0 },
	{ 0, 31, 4, 4, 4, 4, 4, 4, 4, 0 },
	{ 0, 17, 17, 17, 17, 17, 17, 17, 14, 0 }, // 'U'
	{ 0, 17, 17, 17, 10, 10, 10, 4, 4, 0 },
	{ 0, 17, 17, 17, 21, 21, 21, 21, 10, 0 },
	{ 0, 17, 17, 10, 4, 4, 10, 17, 17, 0 },
	{ 0, 17, 17, 10, 10, 4, 4, 4, 4, 0 },
	{ 0, 31, 17, 2, 4, 4, 8, 17, 31, 0 }, // 'Z'
	{ 0, 14, 8, 8, 8, 8, 8, 8, 14, 0 },
	{ 0, 16, 16, 8, 8, 4, 4, 2, 2, 0 },
	{ 0, 14, 2, 2, 2, 2, 2, 2, 14, 0 },
	{ 0, 4, 4, 10, 17, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 31, 0 }, // '_'
	{ 0, 4, 2, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 14, 17, 15, 17, 17, 15, 0 },
	{ 0, 16, 16, 22, 25, 17, 17, 17, 30, 0 },
	{ 0, 0, 0, 14, 17, 16, 16, 17, 14, 0 },
	{ 0, 1, 1, 13, 19, 17, 17, 17, 15, 0 }, // 'd'
	{ 0, 0, 0, 14, 17, 31, 16, 16, 15, 0 },
	{ 0, 7, 8, 31, 8, 8, 8, 8, 8, 0 },
	{ 0, 0, 13, 19, 17, 17, 17, 15, 1, 14 },
	{ 0, 16, 16, 22, 25, 17, 17, 17, 17, 0 },
	{ 0, 4, 0, 4, 4, 4, 4, 4, 4, 0 }, // 'i'
	{ 0, 4, 0, 4, 4, 4, 4, 4, 20, 8 },
	{ 0, 16, 16, 17, 18, 28, 20, 18, 17, 0 },
	{ 0, 12, 4, 4, 4, 4, 4, 4, 4, 0 },
	{ 0, 0, 0, 26, 21, 21, 21, 21, 21, 0 },
	{ 0, 0, 0, 22, 25, 17, 17, 17, 17, 0 }, // 'n'
	{ 0, 0, 0, 14, 17, 17, 17, 17, 14, 0 },
	{ 0, 0, 0, 22, 25, 17, 17, 30, 16, 16 },
	{ 0, 0, 0, 13, 19, 17, 17, 15, 1, 1 },
	{ 0, 0, 0, 22, 25, 16, 16, 16, 16, 0 },
	{ 0, 0, 0, 15, 16, 14, 1, 17, 14, 0 }, // 's'
	{ 0, 0, 8, 31, 8, 8, 8, 8, 7, 0 },
	{ 0, 0, 0, 17, 17, 17, 17, 19, 13, 0 },
	{ 0, 0, 0, 17, 17, 17, 10, 10, 4, 0 },
	{ 0, 0, 0, 17, 17, 21, 21, 21, 10, 0 },
	{ 0, 0, 0, 17, 17, 14, 14, 17, 17, 0 }, // 'x'
	{ 0, 0, 0, 17, 17, 9, 10, 6, 4, 8 },
	{ 0, 0, 0, 31, 2, 4, 8, 17, 31, 0 },
	{ 0, 2, 4, 4, 4, 8, 4, 4, 2, 0 },
	{ 0, 4, 4, 4, 4, 4, 4, 4, 4, 4 },
	{ 0, 8, 4, 4, 4, 2, 4, 4, 8, 0 }, // '}'
	{ 0, 0, 0, 0, 9, 22, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

}
