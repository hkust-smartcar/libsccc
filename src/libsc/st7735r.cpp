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

#include "libbase/helper.h"

#include "libbase/log.h"
#include LIBBASE_H(soft_spi_master)
#if MK60DZ10 || MK60D10 || MK60F15
#include LIBBASE_H(spi_master)
#endif

#include "libsc/config.h"
#include "libsc/device_h/st7735r.h"
#include "libsc/st7735r.h"
#include "libsc/system.h"
#include "libsc/lcd_font.h"
#include "libutil/misc.h"

#define SEND_COMMAND(dat) Send(true, dat)
#define SEND_DATA(dat) Send(false, dat)

using namespace LIBBASE_NS;
using namespace libutil;
using namespace std;

namespace libsc
{

#ifdef LIBSC_USE_LCD

namespace
{

St7735r::SpiMaster::Config GetSpiConfig()
{
	St7735r::SpiMaster::Config config;
	config.sout_pin = LIBSC_ST7735R_SDAT;
	config.sck_pin = LIBSC_ST7735R_SCLK;
	// Max freq of ST7735R == 15MHz
//	config.baud_rate_khz = 15000;
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

	InitMadctl(config);

	// 16-bit
	SEND_COMMAND(ST7735R_COLMOD);
	SEND_DATA(0x05);

	InitFrmctr(config);
	InitPwctr();
	InitGamma();

	SEND_COMMAND(ST7735R_VMCTR1);
	SEND_DATA(0x0E);

	SetActiveRect();

	SEND_COMMAND(ST7735R_DISPON);
	System::DelayMs(10);
}

void St7735r::InitMadctl(const Config &config)
{
	uint8_t param = 0;
	if (config.is_revert)
	{
		SET_BIT(param, 7);
		SET_BIT(param, 6);
	}
	if (config.is_bgr)
	{
		SET_BIT(param, 4);
	}

	SEND_COMMAND(ST7735R_MADCTL);
	SEND_DATA(param);
}

void St7735r::InitFrmctr(const Config &config)
{
	const Uint line = 160;
	const uint32_t fosc = 850000;
	Uint best_rtna = 0;
	Uint best_fpa = 0;
	Uint best_bpa = 0;
	Uint min_diff = static_cast<Uint>(-1);
	for (uint32_t rtna = 0; rtna <= 0x0F; ++rtna)
	{
		const uint32_t this_rtna = rtna * 2 + 40;
		for (Uint fpa = 1; fpa <= 0x3F; ++fpa)
		{
			for (Uint bpa = 1; bpa <= 0x3F; ++bpa)
			{
				const Uint this_rate = fosc / (this_rtna * (line + fpa + bpa + 2));
				const Uint this_diff = abs((int32_t)(this_rate - config.fps));
				if (this_diff < min_diff)
				{
					min_diff = this_diff;
					best_rtna = rtna;
					best_fpa = fpa;
					best_bpa = bpa;
				}

				if (min_diff == 0)
				{
					break;
				}
			}
		}
	}

	SEND_COMMAND(ST7735R_FRMCTR1);
	SEND_DATA(best_rtna);
	SEND_DATA(best_fpa);
	SEND_DATA(best_bpa);
}

void St7735r::InitPwctr()
{
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
}

void St7735r::InitGamma()
{
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
}

void St7735r::FillColor(const uint16_t color)
{
	if (m_region.x >= kW || m_region.y >= kH)
	{
		return;
	}

	SetActiveRect();
	SEND_COMMAND(ST7735R_RAMWR);
	const Uint w = Clamp<Uint>(0, m_region.w, kW - m_region.x);
	const Uint h = Clamp<Uint>(0, m_region.h, kH - m_region.y);
	const Uint length = w * h;
	for (Uint i = 0; i < length; ++i)
	{
		SEND_DATA(color >> 8);
		SEND_DATA(color);
	}
}

void St7735r::FillGrayscalePixel(const uint8_t *pixel, const size_t length)
{
	if (m_region.x >= kW || m_region.y >= kH)
	{
		return;
	}

	SetActiveRect();
	SEND_COMMAND(ST7735R_RAMWR);
	const Uint w = Clamp<Uint>(0, m_region.w, kW - m_region.x);
	//const Uint h = Clamp<Uint>(0, m_region.h, kH - m_region.y);
	// We add the original region w to row_beg, so length_ here also should be
	// the original
	const Uint length_ = std::min<Uint>(m_region.w * m_region.h, length);
	for (Uint row_beg = 0; row_beg < length_; row_beg += m_region.w)
	{
		for (Uint x = 0; x < w; ++x)
		{
			const uint8_t gs_color = pixel[row_beg + x];
			const uint16_t color = GetRgb565(gs_color, gs_color, gs_color);
			SEND_DATA(color >> 8);
			SEND_DATA(color);
		}
	}
}

void St7735r::FillPixel(const uint16_t *pixel, const size_t length)
{
	if (m_region.x >= kW || m_region.y >= kH)
	{
		return;
	}

	SetActiveRect();
	SEND_COMMAND(ST7735R_RAMWR);
	const Uint w = Clamp<Uint>(0, m_region.w, kW - m_region.x);
	//const Uint h = Clamp<Uint>(0, m_region.h, kH - m_region.y);
	// We add the original region w to row_beg, so length_ here also should be
	// the original
	const Uint length_ = std::min<Uint>(m_region.w * m_region.h, length);
	for (Uint row_beg = 0; row_beg < length_; row_beg += m_region.w)
	{
		for (Uint x = 0; x < w; ++x)
		{
			SEND_DATA(pixel[row_beg + x] >> 8);
			SEND_DATA(pixel[row_beg + x]);
		}
	}
}

void St7735r::FillBits(const uint16_t color_t, const uint16_t color_f,
		const bool *data, const size_t length)
{
	if (m_region.x >= kW || m_region.y >= kH)
	{
		return;
	}

	SetActiveRect();
	SEND_COMMAND(ST7735R_RAMWR);
	const Uint w = Clamp<Uint>(0, m_region.w, kW - m_region.x);
	//const Uint h = Clamp<Uint>(0, m_region.h, kH - m_region.y);
	// We add the original region w to row_beg, so length_ here also should be
	// the original
	const Uint length_ = std::min<Uint>(m_region.w * m_region.h, length);
	for (Uint row_beg = 0; row_beg < length_; row_beg += m_region.w)
	{
		for (Uint x = 0; x < w; ++x)
		{
			if (data[row_beg + x])
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
}

void St7735r::FillBits(const uint16_t color_t, const uint16_t color_f,
		const Byte *data, const size_t bit_length)
{
	if (m_region.x >= kW || m_region.y >= kH)
	{
		return;
	}

	SetActiveRect();
	SEND_COMMAND(ST7735R_RAMWR);
	const Uint w = Clamp<Uint>(0, m_region.w, kW - m_region.x);
	//const Uint h = Clamp<Uint>(0, m_region.h, kH - m_region.y);
	// We add the original region w to row_beg, so length_ here also should be
	// the original
	const Uint length_ = std::min<Uint>(m_region.w * m_region.h, bit_length);
	Uint pos = 0;
	int bit_pos = 8;
	for (Uint row_beg = 0; row_beg < length_; row_beg += m_region.w)
	{
		for (Uint x = 0; x < w; ++x)
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

		bit_pos -= (m_region.w - w) % 8;
		pos += (m_region.w - w) >> 3; // /8
	}
}

void St7735r::Clear()
{
	ClearRegion();
	FillColor(0);
}

void St7735r::Clear(const uint16_t color)
{
	ClearRegion();
	FillColor(color);
}

void St7735r::SetInvertColor(const bool flag)
{
	if (flag)
	{
		SEND_COMMAND(ST7735R_INVON);
	}
	else
	{
		SEND_COMMAND(ST7735R_INVOFF);
	}
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

inline void St7735r::Send(const bool is_cmd, const uint8_t data)
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
void St7735r::FillColor(const uint16_t) {}
void St7735r::FillGrayscalePixel(const uint8_t*, const size_t) {}
void St7735r::FillPixel(const uint16_t*, const size_t) {}
void St7735r::FillBits(const uint16_t, const uint16_t, const bool*, const size_t) {}
void St7735r::FillBits(const uint16_t, const uint16_t, const Byte*, const size_t) {}
void St7735r::Clear() {}
void St7735r::Clear(const uint16_t) {}

#endif /* LIBSC_USE_LCD */


}
