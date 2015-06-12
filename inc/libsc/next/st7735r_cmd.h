/*
 * st7735r_cmd.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "libbase/misc_types.h"

#include "libsc/next/st7735r.h"
#include "libutil/ownership_ptr.h"

namespace libsc
{
namespace next
{

class St7735rCmd
{
public:
	virtual ~St7735rCmd()
	{}

	/**
	 * Proceed and return the next command, if none is available, ST7735R_NOP
	 * will be returned instead
	 *
	 * @return
	 */
	virtual Byte NextCmd() = 0;
	/**
	 * Return the data bytes beginning from @a start and with a maximum size of
	 * @a max_size under the current command
	 *
	 * @param start
	 * @param max_size
	 * @param out_data
	 * @return Number of bytes written
	 * @see NextCmd()
	 */
	virtual size_t GetBytes(const size_t start,
			const size_t max_size, Byte *out_data) = 0;
};

class St7735rDrawCmd : public St7735rCmd
{
public:
	explicit St7735rDrawCmd(const Lcd::Rect &region);
	St7735rDrawCmd(const Lcd::Rect &region, const size_t pixel_count);

	Byte NextCmd() override;
	size_t GetBytes(const size_t start, const size_t max_size, Byte *out_data) override;

protected:
	const Lcd::Rect& GetRegion() const
	{
		return m_region;
	}

private:
	size_t GetCasetBytes(const size_t start, const size_t max_size,
			Byte *out_data);
	size_t GetRasetBytes(const size_t start, const size_t max_size,
			Byte *out_data);
	size_t GetDrawCmdBytes(const size_t start, const size_t max_size,
			Byte *out_data);

	virtual uint16_t GetColor(const size_t y, const size_t x,
			const size_t pixel_pos) = 0;

	Uint m_cmd_it;
	Lcd::Rect m_region;
	size_t m_pixel_count;
	size_t m_w;
	size_t m_h;
};

class St7735rFillColor : public St7735rDrawCmd
{
public:
	St7735rFillColor(const Lcd::Rect &region, const uint16_t color);

	uint16_t GetColor(const size_t, const size_t, const size_t) override
	{
		return m_color;
	}

private:
	uint16_t m_color;
};

class St7735rFillGrayscalePixel : public St7735rDrawCmd
{
public:
	St7735rFillGrayscalePixel(const Lcd::Rect &region,
			libutil::OwnershipPtr<const uint8_t[]> &&pixel, const size_t length);

	uint16_t GetColor(const size_t y, const size_t x, const size_t pixel_pos) override;

private:
	libutil::OwnershipPtr<const uint8_t[]> m_pixel;
};

class St7735rFillPixel : public St7735rDrawCmd
{
public:
	St7735rFillPixel(const Lcd::Rect &region,
			libutil::OwnershipPtr<const uint16_t[]> &&pixel, const size_t length);

	uint16_t GetColor(const size_t y, const size_t x, const size_t pixel_pos) override;

private:
	libutil::OwnershipPtr<const uint16_t[]> m_pixel;
};

class St7735rFillBools : public St7735rDrawCmd
{
public:
	St7735rFillBools(const Lcd::Rect &region, const uint16_t color_t,
			const uint16_t color_f, libutil::OwnershipPtr<const bool[]> &&data,
			const size_t length);

	uint16_t GetColor(const size_t y, const size_t x, const size_t pixel_pos) override;

private:
	uint16_t m_color_t;
	uint16_t m_color_f;
	libutil::OwnershipPtr<const bool[]> m_data;
};

class St7735rFillBits : public St7735rDrawCmd
{
public:
	St7735rFillBits(const Lcd::Rect &region, const uint16_t color_t,
			const uint16_t color_f, libutil::OwnershipPtr<const Byte[]> &&data,
			const size_t bit_length);

	uint16_t GetColor(const size_t y, const size_t x, const size_t pixel_pos) override;

private:
	uint16_t m_color_t;
	uint16_t m_color_f;
	libutil::OwnershipPtr<const Byte[]> m_data;
};

}
}
