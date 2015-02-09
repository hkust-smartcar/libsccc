/*
 * lcd.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "libbase/misc_types.h"

namespace libsc
{
namespace k60
{

class Lcd
{
public:
	struct Rect
	{
		Uint x;
		Uint y;
		Uint w;
		Uint h;
	};

	static constexpr uint16_t kBlack = 0x0000;
	static constexpr uint16_t kGray = 0x7BEF;
	static constexpr uint16_t kWhite = 0xFFFF;
	static constexpr uint16_t kRed = 0xF800;
	static constexpr uint16_t kYellow = 0xFFE0;
	static constexpr uint16_t kGreen = 0x07E0;
	static constexpr uint16_t kCyan = 0x07FF;
	static constexpr uint16_t kBlue = 0x001F;
	static constexpr uint16_t kPurple = 0xF81F;

	virtual ~Lcd()
	{}

	/**
	 * Set the interesting region that corresponding Fill* methods would affect
	 *
	 * @param rect
	 */
	virtual void SetRegion(const Rect &rect) = 0;
	/**
	 * Return the current region
	 *
	 * @param rect
	 */
	virtual Rect GetRegion() = 0;
	/**
	 * Clear the currect region. Basically what it does is to extend the region
	 * to the whole screen
	 */
	virtual void ClearRegion() = 0;

	/**
	 * Fill a sigle color
	 *
	 * @param color Color in RGB565
	 */
	virtual void FillColor(const uint16_t color) = 0;
	/**
	 * Fill an array of grayscale color pixel
	 *
	 * @param pixel The grayscale level, 0 = black, 0xFF = white
	 * @param length Length of @a pixel
	 */
	virtual void FillGrayscalePixel(const uint8_t *pixel, const size_t length) = 0;
	/**
	 * Fill an array of color pixel
	 *
	 * @param pixel Color pixel in RGB565
	 * @param length Length of @a pixel
	 */
	virtual void FillPixel(const uint16_t *pixel, const size_t length) = 0;
	/**
	 * Fill two colors based on the bool array
	 *
	 * @param color_t Color in RGB565 used when the element is true
	 * @param color_f Color in RGB565 used when the element is false
	 * @param data The bool array indicating which color to fill. Each element
	 * corresponds to a single pixel
	 * @param length Length of @a data
	 */
	virtual void FillBits(const uint16_t color_t, const uint16_t color_f,
			const bool *data, const size_t length) = 0;
	/**
	 * Fill two colors based on the bit set
	 *
	 * @param color_t Color in RGB565 used when the element is true
	 * @param color_f Color in RGB565 used when the element is false
	 * @param data A byte array indicating which color to fill. Each element
	 * corresponds to 8 pixel, MSB first -> LSB last
	 * @param bit_length Length of @a data in @b bits
	 * @warning The @a bit_length argument takes length in bits not bytes, think
	 * of it the pixel count
	 */
	virtual void FillBits(const uint16_t color_t, const uint16_t color_f,
			const Byte *data, const size_t bit_length) = 0;

	/**
	 * Clear the screen. Depending on the implementation, this may or may not
	 * fill the screen with a single expectable color
	 *
	 * @note Active region is cleared
	 */
	virtual void Clear() = 0;
	/**
	 * Clear the screen and fill with a single color
	 *
	 * @note Active region is cleared
	 * @param color
	 */
	virtual void Clear(const uint16_t color) = 0;
};

}
}
