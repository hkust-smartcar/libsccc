/*
 * st7735r_cmd.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <algorithm>

#include "libbase/misc_types.h"

#include "libsc/device_h/st7735r.h"
#include "libsc/next/st7735r.h"
#include "libsc/next/st7735r_cmd.h"
#include "libutil/misc.h"
#include "libutil/ownership_ptr.h"

using namespace libutil;
using namespace std;

namespace libsc
{
namespace next
{

St7735rDrawCmd::St7735rDrawCmd(const Lcd::Rect &region)
		: St7735rDrawCmd(region, static_cast<size_t>(-1))
{}

St7735rDrawCmd::St7735rDrawCmd(const Lcd::Rect &region, const size_t pixel_count)
		: m_cmd_it(-1),
		  m_region(region),
		  m_pixel_count(pixel_count)
{
	m_w = Clamp<size_t>(0, m_region.w, St7735r::GetW() - m_region.x);
	m_h = Clamp<size_t>(0, m_region.h, St7735r::GetH() - m_region.y);
}

Byte St7735rDrawCmd::NextCmd()
{
	switch (++m_cmd_it)
	{
	case 0:
		return ST7735R_CASET;

	case 1:
		return ST7735R_RASET;

	case 2:
		return ST7735R_RAMWR;

	default:
		return ST7735R_NOP;
	}
}

size_t St7735rDrawCmd::GetBytes(const size_t start, const size_t max_size,
		Byte *out_data)
{
	switch (m_cmd_it)
	{
	case 0:
		return GetCasetBytes(start, max_size, out_data);

	case 1:
		return GetRasetBytes(start, max_size, out_data);

	case 2:
		return GetDrawCmdBytes(start, max_size, out_data);

	default:
		return 0;
	}
}

size_t St7735rDrawCmd::GetCasetBytes(const size_t start, const size_t max_size,
		Byte *out_data)
{
	static constexpr size_t DATA_SIZE = 4;
	if (start >= DATA_SIZE)
	{
		return 0;
	}
	const Byte data[DATA_SIZE] = {0x00, static_cast<Byte>(GetRegion().x), 0x00,
			static_cast<Byte>(GetRegion().x + GetRegion().w - 1)};
	const size_t size = std::min(DATA_SIZE - start, max_size);
	memcpy(out_data, data + start, size);
	return size;
}

size_t St7735rDrawCmd::GetRasetBytes(const size_t start, const size_t max_size,
		Byte *out_data)
{
	static constexpr size_t DATA_SIZE = 4;
	if (start >= DATA_SIZE)
	{
		return 0;
	}
	const Byte data[DATA_SIZE] = {0x00, static_cast<Byte>(GetRegion().y), 0x00,
			static_cast<Byte>(GetRegion().y + GetRegion().h - 1)};
	const size_t size = std::min(DATA_SIZE - start, max_size);
	memcpy(out_data, data + start, size);
	return size;
}

size_t St7735rDrawCmd::GetDrawCmdBytes(const size_t start, const size_t max_size,
		Byte *out_data)
{
	// Remember: 2 bytes/pixel
	const size_t half_start = start / 2;
	if (half_start >= m_w * m_h)
	{
		return 0;
	}

	Uint out_it = 0;
	Uint y = half_start / m_w;
	Uint x = half_start % m_w;
	Uint pixel_pos = y * GetRegion().w;
	for (; y < m_h; ++y)
	{
		if (pixel_pos >= m_pixel_count || out_it >= max_size)
		{
			return out_it;
		}

		for (; x < m_w; ++x)
		{
			if (pixel_pos + x >= m_pixel_count || out_it >= max_size)
			{
				return out_it;
			}

			const uint16_t color = GetColor(y, x, pixel_pos + x);
			for (int i = 0; i < 2 && out_it < max_size; ++i)
			{
				out_data[out_it] = ((start + out_it) % 2 == 0) ? (color >> 8)
						: color;
				++out_it;
			}
		}
		x = 0;
		pixel_pos += GetRegion().w;
	}
	return out_it;
}

St7735rFillColor::St7735rFillColor(const Lcd::Rect &region, const uint16_t color)
		: St7735rDrawCmd(region),
		  m_color(color)
{}

St7735rFillGrayscalePixel::St7735rFillGrayscalePixel(const Lcd::Rect &region,
		OwnershipPtr<const uint8_t[]> &&pixel, const size_t length)
		: St7735rDrawCmd(region, length),
		  m_pixel(std::move(pixel))
{}

uint16_t St7735rFillGrayscalePixel::GetColor(const size_t, const size_t,
		const size_t pixel_position)
{
	const uint8_t gs_color = m_pixel[pixel_position];
	return GetRgb565(gs_color, gs_color, gs_color);
}

St7735rFillPixel::St7735rFillPixel(const Lcd::Rect &region,
		OwnershipPtr<const uint16_t[]> &&pixel, const size_t length)
		: St7735rDrawCmd(region, length),
		  m_pixel(std::move(pixel))
{}

uint16_t St7735rFillPixel::GetColor(const size_t, const size_t,
		const size_t pixel_pos)
{
	return m_pixel[pixel_pos];
}

St7735rFillBools::St7735rFillBools(const Lcd::Rect &region,
		const uint16_t color_t, const uint16_t color_f,
		OwnershipPtr<const bool[]> &&data, const size_t length)
		: St7735rDrawCmd(region, length),
		  m_color_t(color_t),
		  m_color_f(color_f),
		  m_data(std::move(data))
{}

uint16_t St7735rFillBools::GetColor(const size_t, const size_t,
		const size_t pixel_pos)
{
	return m_data[pixel_pos] ? m_color_t : m_color_f;
}

St7735rFillBits::St7735rFillBits(const Lcd::Rect &region,
		const uint16_t color_t, const uint16_t color_f,
		OwnershipPtr<const Byte[]> &&data, const size_t bit_length)
		: St7735rDrawCmd(region, bit_length),
		  m_color_t(color_t),
		  m_color_f(color_f),
		  m_data(std::move(data))
{}

uint16_t St7735rFillBits::GetColor(const size_t, const size_t,
		const size_t pixel_pos)
{
	const size_t byte_pos = pixel_pos / 8;
	// pixel_pos % 8
	const size_t bit_pos = 7 - (pixel_pos - (byte_pos * 8));
	return GET_BIT(m_data[byte_pos], bit_pos) ? m_color_t : m_color_f;
}

}
}
