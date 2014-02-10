/*
 * lcd.h
 * LCD abstraction
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_LCD_H_
#define LIBSC_LCD_H_

#include <cstddef>
#include <cstdint>

namespace libsc
{

class Lcd
{
public:
	static constexpr uint8_t W = 128;
	static constexpr uint8_t H = 160;
	static constexpr uint8_t FONT_W = 8;
	static constexpr uint8_t FONT_H = 16;

	explicit Lcd(const bool is_revert);

	void Clear();
	void Clear(const uint16_t color);
	void DrawPixel(const uint8_t x, const uint8_t y, const uint16_t color) const;
	void DrawChar(const uint8_t x, const uint8_t y, const char ch,
			const uint16_t color, const uint16_t bg_color) const;

	void DrawChar(const uint8_t x, const uint8_t y, const char ch,
			const uint16_t color) const
	{
		DrawChar(x, y, ch, color, m_bg_color);
	}

	void DrawString(const uint8_t x, const uint8_t y, const char *str,
			const uint16_t color) const
	{
		DrawString(x, y, str, color, m_bg_color);
	}

	void DrawString(const uint8_t x, const uint8_t y, const char *str,
			const uint16_t color, const uint16_t bg_color) const
	{
		uint8_t x_ = x;
		while (*str && x_ < W)
		{
			DrawChar(x_, y, *str++, color, bg_color);
			x_ += 8;
		}
	}

	void DrawBuffer(const uint8_t x, const uint8_t y, const uint8_t *buf,
			const uint32_t len, const uint16_t color) const
	{
		DrawBuffer(x, y, buf, len, color, m_bg_color);
	}

	void DrawBuffer(const uint8_t x, const uint8_t y, const uint8_t *buf,
			const uint32_t len, const uint16_t color, const uint16_t bg_color) const
	{
		uint8_t x_ = x;
		for (uint32_t i = 0; i < len && x_ < W; ++i)
		{
			DrawChar(x_, y, *buf++, color, bg_color);
			x_ += 8;
		}
	}

	static uint16_t GetRgb565(const uint8_t r, const uint8_t g, const uint8_t b)
	{
		return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
	}

private:
	void SetActiveRect(const uint8_t x, const uint8_t y, const uint8_t w,
			const uint8_t h) const;
	void Send(const bool is_cmd, const uint8_t data) const;

	uint16_t m_bg_color;
};

}

#endif /* LIBSC_LCD_H_ */
