/*
 * st7735r.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */
/*
 * Author: Henry
 * Copyright (c) 2011-2014 HKUST Robotics Team
 */

#include <cstdint>

#include <algorithm>

#include "libbase/log.h"
#include "libbase/k60/soft_spi_master.h"

#include "libsc/config.h"
#include "libsc/k60/st7735r.h"
#include "libsc/k60/system.h"
#include "libsc/lcd_font.h"
#include "libutil/misc.h"

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

using namespace libbase::k60;
using namespace std;

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_LCD

namespace
{

SpiMaster::Config GetSpiConfig()
{
	SpiMaster::Config config;
	config.sout_pin = LIBSC_LCD_SDAT;
	config.sck_pin = LIBSC_LCD_SCLK;
	// Max freq of ST7735R == 15MHz
	config.baud_rate_khz = 15000;
	config.frame_size = 8;
	config.is_sck_idle_low = true;
	config.is_sck_capture_first = true;
	config.is_msb_firt = true;

	config.slaves[0].cs_pin = LIBSC_LCD_CS;
	return config;
}

Gpo::Config GetRstConfig()
{
	Gpo::Config config;
	config.pin = LIBSC_LCD_RST;
	config.is_high = true;
	return config;
}

Gpo::Config GetDcConfig()
{
	Gpo::Config config;
	config.pin = LIBSC_LCD_DC;
	config.is_high = true;
	return config;
}

}

St7735r::St7735r()
		: St7735r(false)
{}

St7735r::St7735r(const bool is_revert)
		: m_spi(GetSpiConfig()),
		  m_rst(GetRstConfig()),
		  m_dc(GetDcConfig()),

		  m_bg_color(0)
{
#ifdef LIBSC_USE_LCD_HW_SPI
	spi_init(SpiUtils::GetSpiModule<LIBSC_LCD_SCLK, LIBSC_LCD_MOSI, LIBSC_LCD_MISO, LIBSC_LCD_CS>(),
			SpiUtils::GetSpiPcs<LIBSC_LCD_SCLK, LIBSC_LCD_MOSI, LIBSC_LCD_MISO, LIBSC_LCD_CS>(),
			MASTER, SPI_MOSI_FLAG, 12000000u);
#endif

	Clear();
	SEND_COMMAND(CMD_SW_RESET);
	System::DelayMs(10);

	SEND_COMMAND(CMD_SLEEP_OUT);
	System::DelayMs(120);

	SEND_COMMAND(CMD_MEM_DATA_ACCESS_CTRL);
	SEND_DATA(is_revert ? 0x08 : 0xC8);

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

	SetActiveRect(0, 0, kW, kH);

	SEND_COMMAND(CMD_DISPLAY_ON);
	System::DelayMs(10);
}

void St7735r::Clear()
{
	m_rst.Clear();
	System::DelayMs(100);
	m_rst.Set();
	System::DelayMs(100);

	m_bg_color = 0;
}

void St7735r::DrawGrayscalePixelBuffer(const uint8_t x, const uint8_t y,
		const uint8_t w, const uint8_t h, const uint8_t *pixel)
{
	SetActiveRect(x, y, w, h);
	SEND_COMMAND(CMD_MEMORY_WRITE);
	const Uint area = w * h;
	for (Uint i = 0; i < area; ++i)
	{
		const uint16_t col = libutil::GetRgb565(pixel[i], pixel[i], pixel[i]);
		SEND_DATA(col >> 8);
		SEND_DATA(col);
	}
}

void St7735r::DrawPixelBuffer(const uint8_t x, const uint8_t y, const uint8_t w,
		const uint8_t h, const uint16_t *pixel)
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

void St7735r::DrawPixelBuffer(const uint8_t x, const uint8_t y, const uint8_t w,
		const uint8_t h, const uint16_t color_t, const uint16_t color_f,
		const bool *data)
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

void St7735r::DrawPixel(const uint8_t x, const uint8_t y, const uint8_t w,
		const uint8_t h, const uint16_t color)
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

void St7735r::DrawChar(const uint8_t x, const uint8_t y, const char ch,
		const uint16_t color, const uint16_t bg_color)
{
	if (ch < 32 || ch > 126)
	{
		LOG_EL("Unsupported character");
		return;
	}

	const uint8_t *font_data = &LcdFont::DATA_8x16[(ch - 32) << 4];
	SetActiveRect(x, y, kFontW, kFontH);
	SEND_COMMAND(0x2C);
	for (Uint y = 0; y < kFontH; ++y)
	{
		for (Uint x = 0; x < kFontW; ++x)
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

void St7735r::SetActiveRect(const uint8_t x, const uint8_t y, const uint8_t w,
		const uint8_t h)
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

void St7735r::Send(const bool is_cmd, const uint8_t data)
{
	m_dc.Set(!is_cmd);
	m_spi.ExchangeData(0, data);
}

#else
St7735r::St7735r()
		: m_spi(nullptr), m_rst(nullptr), m_dc(nullptr), m_bg_color(0)
{
	LOG_DL("Configured not to use St7735r(LCD)");
}
St7735r::St7735r(const bool) : St7735r() {}
void St7735r::Clear() {}
void St7735r::DrawPixel(const uint8_t, const uint8_t, const uint8_t,
		const uint8_t, const uint16_t) {}
void St7735r::DrawGrayscalePixelBuffer(const uint8_t, const uint8_t,
		const uint8_t, const uint8_t, const uint8_t*) {}
void St7735r::DrawPixelBuffer(const uint8_t, const uint8_t, const uint8_t,
		const uint8_t, const uint16_t*) {}
void St7735r::DrawPixelBuffer(const uint8_t, const uint8_t, const uint8_t,
		const uint8_t, const uint16_t, const uint16_t,
		const bool*) {}
void St7735r::DrawChar(const uint8_t, const uint8_t, const char,
		const uint16_t, const uint16_t) {}
void St7735r::SetActiveRect(const uint8_t, const uint8_t, const uint8_t,
		const uint8_t) {}
void St7735r::Send(const bool, const uint8_t) {}

#endif /* LIBSC_USE_LCD */

}
}
