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
#include "libbase/misc_types.h"

#include "libsc/config.h"
#include "libsc/k60/lcd.h"

namespace libsc
{
namespace k60
{

class St7735r : public Lcd
{
public:
#if LIBSC_USE_SOFT_ST7735R
	typedef libbase::k60::SoftSpiMaster SpiMaster;

#else
	typedef libbase::k60::SpiMaster SpiMaster;

#endif // LIBSC_USE_SOFT_ST7735R

	struct Config
	{
		bool is_revert = false;
	};

	explicit St7735r(const Config &config);

	void SetRegion(const Rect &rect) override;
	Rect GetRegion() override
	{
		return m_region;
	}

	void ClearRegion() override
	{
		m_region = Rect{0, 0, GetW(), GetH()};
	}

	void FillColor(const uint16_t color) override;
	void FillGrayscalePixel(const uint8_t *pixel, const size_t length) override;
	void FillPixel(const uint16_t *pixel, const size_t length) override;
	void FillBits(const uint16_t color_t, const uint16_t color_f,
			const bool *data, const size_t length) override;
	void FillBits(const uint16_t color_t, const uint16_t color_f,
			const Byte *data, const size_t bit_length) override;

	void Clear() override;
	void Clear(const uint16_t color) override;

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

	static constexpr Uint GetW()
	{
		return kW;
	}

	static constexpr Uint GetH()
	{
		return kH;
	}

	static constexpr Uint GetFontW()
	{
		return kFontW;
	}

	static constexpr Uint GetFontH()
	{
		return kFontH;
	}

private:
	static constexpr uint8_t kW = 128;
	static constexpr uint8_t kH = 160;
	static constexpr uint8_t kFontW = 8;
	static constexpr uint8_t kFontH = 16;

	void SetActiveRect();
	void Send(const bool is_cmd, const uint8_t data);

	SpiMaster m_spi;
	libbase::k60::Gpo m_rst;
	libbase::k60::Gpo m_dc;

	Rect m_region;
	uint16_t m_bg_color;
};

}
}
