/*
 * st7735r.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
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
#include "libsc/device_h/st7735r.h"
#include "libsc/k60/st7735r.h"
#include "libsc/k60/system.h"
#include "libsc/lcd_font.h"
#include "libutil/misc.h"

#define SEND_COMMAND(dat) Send(true, dat)
#define SEND_DATA(dat) Send(false, dat)

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
	config.sout_pin = LIBSC_ST7735R_SDAT;
	config.sck_pin = LIBSC_ST7735R_SCLK;
	// Max freq of ST7735R == 15MHz
	config.baud_rate_khz = 15000;
	config.frame_size = 8;
	config.is_sck_idle_low = true;
	config.is_sck_capture_first = true;
	config.is_msb_firt = true;

	config.slaves[0].cs_pin = LIBSC_ST7735R_CS;
	return config;
}

Gpo::Config GetRstConfig()
{
	Gpo::Config config;
	config.pin = LIBSC_ST7735R_RST;
	config.is_high = true;
	return config;
}

Gpo::Config GetDcConfig()
{
	Gpo::Config config;
	config.pin = LIBSC_ST7735R_DC;
	config.is_high = true;
	return config;
}

}

St7735r::St7735r(const Config &config)
		: m_spi(GetSpiConfig()),
		  m_rst(GetRstConfig()),
		  m_dc(GetDcConfig()),

		  m_region{0, 0, GetW(), GetH()}
{
	Clear();
	SEND_COMMAND(ST7735R_SWRESET);
	System::DelayMs(10);

	SEND_COMMAND(ST7735R_SLPOUT);
	System::DelayMs(120);

	uint8_t madctl_reg = 0;
	if (!config.is_revert)
	{
		madctl_reg |= 0xC0;
	}
#ifdef LIBSC_ST7735R_BGR_PANEL
	madctl_reg |= 0x08;
#endif
	SEND_COMMAND(ST7735R_MADCTL);
	SEND_DATA(madctl_reg);

	// 16-bit
	SEND_COMMAND(ST7735R_COLMOD);
	SEND_DATA(0x55);

	// ~60Hz
	SEND_COMMAND(ST7735R_FRMCTRL1);
	SEND_DATA(0x05);
	SEND_DATA(0x3C);
	SEND_DATA(0x3C);

	SEND_COMMAND(ST7735R_PWCTR1);
	SEND_DATA(0xA2);
	SEND_DATA(0x02);
	SEND_DATA(0x84);
	SEND_COMMAND(ST7735R_PWCTR2);
	SEND_DATA(0xC5);
	SEND_COMMAND(ST7735R_PWCTR3);
	SEND_DATA(0x0A);
	SEND_DATA(0x00);
	SEND_COMMAND(ST7735R_PWCTR4);
	SEND_DATA(0x8A);
	SEND_DATA(0x2A);
	SEND_COMMAND(ST7735R_PWCTR5);
	SEND_DATA(0x8A);
	SEND_DATA(0xEE);

	SEND_COMMAND(ST7735R_VMCTR1);
	SEND_DATA(0x0E);

	SEND_COMMAND(ST7735R_GMCTRP1);
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

	SEND_COMMAND(ST7735R_GMCTRN1);
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

	SetActiveRect();

	SEND_COMMAND(ST7735R_DISPON);
	System::DelayMs(10);
}

void St7735r::SetRegion(const Rect &rect)
{
	m_region.x = rect.x;
	m_region.y = rect.y;
	m_region.w = libutil::Clamp<Uint>(0, rect.w, (Uint)kW);
	m_region.h = libutil::Clamp<Uint>(0, rect.h, (Uint)kH);
}

void St7735r::FillColor(const uint16_t color)
{
	SetActiveRect();
	SEND_COMMAND(ST7735R_RAMWR);
	const Uint length = m_region.w * m_region.h;
	for (Uint i = 0; i < length; ++i)
	{
		SEND_DATA(color >> 8);
		SEND_DATA(color);
	}
}

void St7735r::FillGrayscalePixel(const uint8_t *pixel, const size_t length)
{
	SetActiveRect();
	SEND_COMMAND(ST7735R_RAMWR);
	const Uint length_ = std::min<Uint>(m_region.w * m_region.h, length);
	for (Uint i = 0; i < length_; ++i)
	{
		const uint16_t color = libutil::GetRgb565(pixel[i], pixel[i], pixel[i]);
		SEND_DATA(color >> 8);
		SEND_DATA(color);
	}
}

void St7735r::FillPixel(const uint16_t *pixel, const size_t length)
{
	SetActiveRect();
	SEND_COMMAND(ST7735R_RAMWR);
	const Uint length_ = std::min<Uint>(m_region.w * m_region.h, length);
	for (Uint i = 0; i < length_; ++i)
	{
		SEND_DATA(pixel[i] >> 8);
		SEND_DATA(pixel[i]);
	}
}

void St7735r::FillBits(const uint16_t color_t, const uint16_t color_f,
		const bool *data, const size_t length)
{
	SetActiveRect();
	SEND_COMMAND(ST7735R_RAMWR);
	const Uint length_ = std::min<Uint>(m_region.w * m_region.h, length);
	for (Uint i = 0; i < length_; ++i)
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

void St7735r::FillBits(const uint16_t color_t, const uint16_t color_f,
		const Byte *data, const size_t bit_length)
{
	SetActiveRect();
	SEND_COMMAND(ST7735R_RAMWR);
	const Uint length_ = std::min<Uint>(m_region.w * m_region.h, bit_length);
	Uint pos = 0;
	int bit_pos = 8;
	for (Uint i = 0; i < length_; ++i)
	{
		if (--bit_pos < 0)
		{
			bit_pos = 7;
			++pos;
		}
		if (GET_BIT(data[pos], bit_pos))
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

void St7735r::Clear()
{
	ClearRegion();
	m_rst.Clear();
	System::DelayMs(100);
	m_rst.Set();
	System::DelayMs(100);
}

void St7735r::Clear(const uint16_t color)
{
	ClearRegion();
	FillColor(color);
}

void St7735r::SetActiveRect()
{
	SEND_COMMAND(ST7735R_CASET);
	// start
	SEND_DATA(0x00);
	SEND_DATA(m_region.x);
	// end
	SEND_DATA(0x00);
	SEND_DATA(m_region.x + m_region.w - 1);

	SEND_COMMAND(ST7735R_RASET);
	SEND_DATA(0x00);
	SEND_DATA(m_region.y);
	SEND_DATA(0x00);
	SEND_DATA(m_region.y + m_region.h - 1);
}

void St7735r::Send(const bool is_cmd, const uint8_t data)
{
	m_dc.Set(!is_cmd);
	m_spi.ExchangeData(0, data);
}

#else
St7735r::St7735r(const Config&)
		: m_spi(nullptr), m_rst(nullptr), m_dc(nullptr)
{
	LOG_DL("Configured not to use St7735r(LCD)");
}
void St7735r::SetRegion(const Rect&) {}
void St7735r::FillColor(const uint16_t) {}
void St7735r::FillGrayscalePixel(const uint8_t*, const size_t) {}
void St7735r::FillPixel(const uint16_t*, const size_t) {}
void St7735r::FillBits(const uint16_t, const uint16_t, const bool*, const size_t) {}
void St7735r::FillBits(const uint16_t, const uint16_t, const Byte*, const size_t) {}
void St7735r::Clear() {}
void St7735r::Clear(const uint16_t) {}

#endif /* LIBSC_USE_LCD */

}
}
