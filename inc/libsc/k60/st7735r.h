/*
 * st7735r.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "libbase/k60/soft_spi_master.h"
#include "libbase/k60/spi_master.h"

namespace libsc
{
namespace k60
{

class St7735r
{
public:
	struct Config
	{
		bool is_revert = false;
	};

	static constexpr uint8_t kW = 128;
	static constexpr uint8_t kH = 160;
	static constexpr uint8_t kFontW = 8;
	static constexpr uint8_t kFontH = 16;

	explicit St7735r(const Config &config);

	void Clear();
	void Clear(const uint16_t color)
	{
		DrawPixel(0, 0, kW, kH, color);
		m_bg_color = color;
	}

	void DrawPixel(const uint8_t x, const uint8_t y, const uint8_t w,
			const uint8_t h, const uint16_t color);
	void DrawPixel(const uint8_t x, const uint8_t y, const uint16_t color)
	{
		DrawPixel(x, y, 1, 1, color);
	}

	void DrawGrayscalePixelBuffer(const uint8_t x, const uint8_t y,
			const uint8_t w, const uint8_t h, const uint8_t *pixel);
	void DrawPixelBuffer(const uint8_t x, const uint8_t y, const uint8_t w,
			const uint8_t h, const uint16_t *pixel);
	void DrawPixelBuffer(const uint8_t x, const uint8_t y, const uint8_t w,
			const uint8_t h, const uint16_t color_t, const uint16_t color_f,
			const bool *data);

	void DrawChar(const uint8_t x, const uint8_t y, const char ch,
			const uint16_t color, const uint16_t bg_color);
	void DrawChar(const uint8_t x, const uint8_t y, const char ch,
			const uint16_t color)
	{
		DrawChar(x, y, ch, color, m_bg_color);
	}

	void DrawString(const uint8_t x, const uint8_t y, const char *str,
			const uint16_t color)
	{
		DrawString(x, y, str, color, m_bg_color);
	}

	void DrawString(const uint8_t x, const uint8_t y, const char *str,
			const uint16_t color, const uint16_t bg_color)
	{
		uint8_t x_ = x;
		while (*str && x_ < kW)
		{
			DrawChar(x_, y, *str++, color, bg_color);
			x_ += 8;
		}
	}

	void DrawBuffer(const uint8_t x, const uint8_t y, const uint8_t *buf,
			const uint32_t len, const uint16_t color)
	{
		DrawBuffer(x, y, buf, len, color, m_bg_color);
	}

	void DrawBuffer(const uint8_t x, const uint8_t y, const uint8_t *buf,
			const uint32_t len, const uint16_t color, const uint16_t bg_color)
	{
		uint8_t x_ = x;
		for (uint32_t i = 0; i < len && x_ < kW; ++i)
		{
			DrawChar(x_, y, *buf++, color, bg_color);
			x_ += 8;
		}
	}

private:
	void SetActiveRect(const uint8_t x, const uint8_t y, const uint8_t w,
			const uint8_t h);
	void Send(const bool is_cmd, const uint8_t data);

	libbase::k60::SpiMaster m_spi;
	libbase::k60::Gpo m_rst;
	libbase::k60::Gpo m_dc;

	uint16_t m_bg_color;
};

}
}
