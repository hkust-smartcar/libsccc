/*
 * lcd.cpp
 * LCD abstraction
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */
/*
 * Author: Henry
 * Copyright (c) 2011-2014 HKUST Robotics Team
 */

#include <mini_common.h>
#include <hw_common.h>
#include <algorithm>
#include <cstdint>

#include <MK60_gpio.h>
#include <MK60_spi.h>

#include "macro.h"
#include "lcd_font.h"
#include "spi_utils.h"
#include "libsc/com/config.h"
#include "libsc/com/lcd.h"

#define SEND_COMMAND(dat) Send(true, dat)
#define SEND_DATA(dat) Send(false, dat)

#define CMD_SW_RESET 0x01
#define CMD_SLEEP_OUT 0x11
#define CMD_DISPLAY_ON 0x29
#define CMD_COLUMN_ADDRESS_SET 0x2A
#define CMD_ROW_ADDRESS_SET 0x2B
#define CMD_MEMORY_WRITE 0x2C
#define CMD_MEM_DATA_ACCESS_CTRL 0x36
#define CMD_PIXEL_FORMAT 0x3A
#define CMD_FRAME_RATE_CTRL_NORMAL 0xB1
#define CMD_POWER_CTRL1 0xC0
#define CMD_POWER_CTRL2 0xC1
#define CMD_POWER_CTRL3 0xC2
#define CMD_POWER_CTRL4 0xC3
#define CMD_POWER_CTRL5 0xC4
#define CMD_VCOM_CTRL 0xC5
#define CMD_GAMMA_CORRECTION_POS_POLARITY 0xE0
#define CMD_GAMMA_CORRECTION_NEG_POLARITY 0xE1

namespace libsc
{

#ifdef LIBSC_USE_LCD

Lcd::Lcd(const bool is_revert)
		: m_bg_color(0)
{
	gpio_init(LIBSC_LCD_RST, GPO, 1);
	gpio_init(LIBSC_LCD_DC, GPO, 1);
#ifdef LIBSC_USE_LCD_HW_SPI
	spi_init(SpiUtils::GetSpiModule<LIBSC_LCD_SCLK, LIBSC_LCD_MOSI, LIBSC_LCD_MISO, LIBSC_LCD_CS>(),
			SpiUtils::GetSpiPcs<LIBSC_LCD_SCLK, LIBSC_LCD_MOSI, LIBSC_LCD_MISO, LIBSC_LCD_CS>(),
			MASTER, SPI_MOSI_FLAG, 12000000u);
#else
	gpio_init(LIBSC_LCD_CS, GPO, 1);
	gpio_init(LIBSC_LCD_SDAT, GPO, 1);
	gpio_init(LIBSC_LCD_SCLK, GPO, 1);
#endif

	Clear();
	SEND_COMMAND(CMD_SW_RESET);
	DELAY_MS(10);

	SEND_COMMAND(CMD_SLEEP_OUT);
	DELAY_MS(120);

	SEND_COMMAND(CMD_MEM_DATA_ACCESS_CTRL);
	SEND_DATA(is_revert ? 0xC8 : 0x08);

	// 16-bit
	SEND_COMMAND(CMD_PIXEL_FORMAT);
	SEND_DATA(0x55);

	// ~60Hz
	SEND_COMMAND(CMD_FRAME_RATE_CTRL_NORMAL);
	SEND_DATA(0x05);
	SEND_DATA(0x3C);
	SEND_DATA(0x3C);

	SEND_COMMAND(CMD_POWER_CTRL1);
	SEND_DATA(0xA2);
	SEND_DATA(0x02);
	SEND_DATA(0x84);
	SEND_COMMAND(CMD_POWER_CTRL2);
	SEND_DATA(0xC5);
	SEND_COMMAND(CMD_POWER_CTRL3);
	SEND_DATA(0x0A);
	SEND_DATA(0x00);
	SEND_COMMAND(CMD_POWER_CTRL4);
	SEND_DATA(0x8A);
	SEND_DATA(0x2A);
	SEND_COMMAND(CMD_POWER_CTRL5);
	SEND_DATA(0x8A);
	SEND_DATA(0xEE);

	SEND_COMMAND(CMD_VCOM_CTRL);
	SEND_DATA(0x0E);

	SEND_COMMAND(CMD_GAMMA_CORRECTION_POS_POLARITY);
	SEND_DATA(0x02);
	SEND_DATA(0x1C);
	SEND_DATA(0x07);
	SEND_DATA(0x12);
	SEND_DATA(0x37);
	SEND_DATA(0x32);
	SEND_DATA(0x29);
	SEND_DATA(0x2D);
	SEND_DATA(0x29);
	SEND_DATA(0x25);
	SEND_DATA(0x2B);
	SEND_DATA(0x39);
	SEND_DATA(0x00);
	SEND_DATA(0x01);
	SEND_DATA(0x03);
	SEND_DATA(0x10);

	SEND_COMMAND(CMD_GAMMA_CORRECTION_NEG_POLARITY);
	SEND_DATA(0x03);
	SEND_DATA(0x1D);
	SEND_DATA(0x07);
	SEND_DATA(0x06);
	SEND_DATA(0x2E);
	SEND_DATA(0x2C);
	SEND_DATA(0x29);
	SEND_DATA(0x2D);
	SEND_DATA(0x2E);
	SEND_DATA(0x2E);
	SEND_DATA(0x37);
	SEND_DATA(0x3F);
	SEND_DATA(0x00);
	SEND_DATA(0x00);
	SEND_DATA(0x02);
	SEND_DATA(0x10);

	SetActiveRect(0, 0, W, H);

	SEND_COMMAND(CMD_DISPLAY_ON);
	DELAY_MS(10);
}

void Lcd::Clear()
{
	LIBSC_PIN_OUT(LIBSC_LCD_RST) = 0;
	DELAY_MS(100);
	LIBSC_PIN_OUT(LIBSC_LCD_RST) = 1;
	DELAY_MS(100);

	m_bg_color = 0;
}

void Lcd::DrawPixelBuffer(const uint8_t x, const uint8_t y, const uint8_t w,
		const uint8_t h, const uint16_t *pixel) const
{
	SetActiveRect(x, y, w, h);
	SEND_COMMAND(CMD_MEMORY_WRITE);
	const Uint area = w * h;
	for (Uint i = 0; i < area; ++i)
	{
		SEND_DATA(pixel[i] >> 8);
		SEND_DATA(pixel[i]);
	}
}

void Lcd::DrawPixelBuffer(const uint8_t x, const uint8_t y, const uint8_t w,
		const uint8_t h, const uint16_t color_t, const uint16_t color_f,
		const bool *data) const
{
	SetActiveRect(x, y, w, h);
	SEND_COMMAND(CMD_MEMORY_WRITE);
	const Uint area = w * h;
	for (Uint i = 0; i < area; ++i)
	{
		if (data[i])
		{
			SEND_DATA(color_t >> 8);
			SEND_DATA(color_t);
		}
		else
		{
			SEND_DATA(color_f >> 8);
			SEND_DATA(color_f);
		}
	}
}

void Lcd::DrawPixel(const uint8_t x, const uint8_t y, const uint8_t w,
		const uint8_t h, const uint16_t color) const
{
	SetActiveRect(x, y, w, h);
	SEND_COMMAND(CMD_MEMORY_WRITE);
	const Uint area = w * h;
	for (Uint i = 0; i < area; ++i)
	{
		SEND_DATA(color >> 8);
		SEND_DATA(color);
	}
}

void Lcd::DrawChar(const uint8_t x, const uint8_t y, const char ch,
		const uint16_t color, const uint16_t bg_color) const
{
	if (ch < 32 || ch > 126)
	{
		LOG_E("Lcd::DrawChar Unsupported character");
		return;
	}

	const uint8_t *font_data = &LcdFont::DATA_8x16[(ch - 32) << 4];
	SetActiveRect(x, y, FONT_W, FONT_H);
	SEND_COMMAND(0x2C);
	for (Uint y = 0; y < FONT_H; ++y)
	{
		for (Uint x = 0; x < FONT_W; ++x)
		{
			if (*font_data & (0x80 >> x))
			{
				SEND_DATA(color >> 8);
				SEND_DATA(color);
			}
			else
			{
				SEND_DATA(bg_color >> 8);
				SEND_DATA(bg_color);
			}
		}
		++font_data;
	}
}

void Lcd::SetActiveRect(const uint8_t x, const uint8_t y, const uint8_t w,
		const uint8_t h) const
{
	SEND_COMMAND(CMD_COLUMN_ADDRESS_SET);
	// start
	SEND_DATA(0x00);
	SEND_DATA(x);
	// end
	SEND_DATA(0x00);
	SEND_DATA(x + w - 1);

	SEND_COMMAND(CMD_ROW_ADDRESS_SET);
	SEND_DATA(0x00);
	SEND_DATA(y);
	SEND_DATA(0x00);
	SEND_DATA(y + h - 1);
}

#ifdef LIBSC_USE_LCD_HW_SPI
void Lcd::Send(const bool is_cmd, const uint8_t data) const
{
	LIBSC_PIN_OUT(LIBSC_LCD_DC) = is_cmd ? 0 : 1;
	spi_mosi(SpiUtils::GetSpiModule<LIBSC_LCD_SCLK, LIBSC_LCD_MOSI, LIBSC_LCD_MISO, LIBSC_LCD_CS>(),
			SpiUtils::GetSpiPcs<LIBSC_LCD_SCLK, LIBSC_LCD_MOSI, LIBSC_LCD_MISO, LIBSC_LCD_CS>(),
			&data, NULL, 1);
}

#else
void Lcd::Send(const bool is_cmd, const uint8_t data) const
{
	LIBSC_PIN_OUT(LIBSC_LCD_DC) = is_cmd ? 0 : 1;
	LIBSC_PIN_OUT(LIBSC_LCD_CS) = 0;

	LIBSC_PIN_OUT(LIBSC_LCD_SDAT) = (data >> 7) & 0x1;
	LIBSC_PIN_OUT(LIBSC_LCD_SCLK) = 0;
	LIBSC_PIN_OUT(LIBSC_LCD_SCLK) = 1;
	LIBSC_PIN_OUT(LIBSC_LCD_SDAT) = (data >> 6) & 0x1;
	LIBSC_PIN_OUT(LIBSC_LCD_SCLK) = 0;
	LIBSC_PIN_OUT(LIBSC_LCD_SCLK) = 1;
	LIBSC_PIN_OUT(LIBSC_LCD_SDAT) = (data >> 5) & 0x1;
	LIBSC_PIN_OUT(LIBSC_LCD_SCLK) = 0;
	LIBSC_PIN_OUT(LIBSC_LCD_SCLK) = 1;
	LIBSC_PIN_OUT(LIBSC_LCD_SDAT) = (data >> 4) & 0x1;
	LIBSC_PIN_OUT(LIBSC_LCD_SCLK) = 0;
	LIBSC_PIN_OUT(LIBSC_LCD_SCLK) = 1;
	LIBSC_PIN_OUT(LIBSC_LCD_SDAT) = (data >> 3) & 0x1;
	LIBSC_PIN_OUT(LIBSC_LCD_SCLK) = 0;
	LIBSC_PIN_OUT(LIBSC_LCD_SCLK) = 1;
	LIBSC_PIN_OUT(LIBSC_LCD_SDAT) = (data >> 2) & 0x1;
	LIBSC_PIN_OUT(LIBSC_LCD_SCLK) = 0;
	LIBSC_PIN_OUT(LIBSC_LCD_SCLK) = 1;
	LIBSC_PIN_OUT(LIBSC_LCD_SDAT) = (data >> 1) & 0x1;
	LIBSC_PIN_OUT(LIBSC_LCD_SCLK) = 0;
	LIBSC_PIN_OUT(LIBSC_LCD_SCLK) = 1;
	LIBSC_PIN_OUT(LIBSC_LCD_SDAT) = (data >> 0) & 0x1;
	LIBSC_PIN_OUT(LIBSC_LCD_SCLK) = 0;
	LIBSC_PIN_OUT(LIBSC_LCD_SCLK) = 1;

	LIBSC_PIN_OUT(LIBSC_LCD_CS) = 1;
}

#endif /* LIBSC_USE_LCD_HW_SPI */

#else
Lcd::Lcd(const bool) {}
void Lcd::Clear() {}
void Lcd::Clear(const uint16_t) {}
void Lcd::DrawPixel(const uint8_t, const uint8_t, const uint8_t, const uint8_t,
		const uint16_t) const {}
void Lcd::DrawPixelBuffer(const uint8_t, const uint8_t, const uint8_t,
		const uint8_t, const uint16_t*) const {}
void Lcd::DrawPixelBuffer(const uint8_t, const uint8_t, const uint8_t,
		const uint8_t, const uint16_t, const uint16_t,
		const bool*) const {}
void Lcd::DrawChar(const uint8_t, const uint8_t, const char,
		const uint16_t, const uint16_t) const {}
void Lcd::SetActiveRect(const uint8_t, const uint8_t, const uint8_t,
		const uint8_t) const {}
void Lcd::Send(const bool, const uint8_t) const {}

#endif /* LIBSC_USE_LCD */

}
