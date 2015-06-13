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
#include <cstring>

#include <algorithm>

#include "libbase/helper.h"
#include "libbase/log.h"
#include LIBBASE_H(gpio)
#include LIBBASE_H(spi_master)

#include "libsc/config.h"
#include "libsc/device_h/st7735r.h"
#include "libsc/next/st7735r.h"
#include "libsc/system.h"
#include "libutil/misc.h"
#include "libutil/ownership_ptr.h"

#define SEND_COMMAND(dat) Send(true, dat)
#define SEND_DATA(dat) Send(false, dat)

using namespace LIBBASE_NS;
using namespace libutil;
using namespace std;

namespace libsc
{
namespace next
{

#ifdef LIBSC_USE_LCD

namespace
{

St7735r::SpiMaster::Config GetSpiConfig(
		const SpiMaster::OnTxFillListener &tx_isr)
{
	St7735r::SpiMaster::Config config;
	config.sout_pin = LIBSC_ST7735R_SDAT;
	config.sck_pin = LIBSC_ST7735R_SCLK;

#if !LIBSC_USE_SOFT_ST7735R
	// Max freq of ST7735R == 15MHz
	config.baud_rate_khz = 15000;
#endif

	config.frame_size = 8;
	config.is_sck_idle_low = true;
	config.is_sck_capture_first = true;
	config.is_msb_firt = true;

#if MK60D10 || MK60DZ10 || MK60F15
	config.slaves[0].cs_pin = LIBSC_ST7735R_CS;
#elif MKL26Z4
	config.pcs_pin = LIBSC_ST7735R_CS;
#endif

	config.tx_isr = tx_isr;
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
		: m_tx_buf(14),
		  m_is_tx_idle(true),
		  m_buf_start(0),
		  m_data_it(0),
		  m_data_size(0),
		  m_region{0, 0, GetW(), GetH()},
		  m_spi(GetSpiConfig(std::bind(&St7735r::OnTxComplete, this,
				  placeholders::_1))),
		  m_rst(GetRstConfig()),
		  m_dc(GetDcConfig())
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

inline void St7735r::EnableTx()
{
	if (m_is_tx_idle)
	{
		m_is_tx_idle = false;
		m_spi.SetEnableTxIrq(true);
	}
}

inline void St7735r::DisableTx()
{
	m_spi.SetEnableTxIrq(false);
	m_is_tx_idle = true;
}

void St7735r::FillColor(const uint16_t color)
{
	if (m_region.x >= kW || m_region.y >= kH)
	{
		return;
	}

	if (m_tx_buf.PushData(unique_ptr<St7735rCmd>(new St7735rFillColor(m_region,
			color))))
	{
		EnableTx();
		return;
	}
	else
	{
		return;
	}
}

void St7735r::FillGrayscalePixel(const uint8_t *pixel, const size_t length)
{
	if (m_region.x >= kW || m_region.y >= kH)
	{
		return;
	}

	uint8_t *pixel_copy = new uint8_t[length];
	memcpy(pixel_copy, pixel, length);
	if (m_tx_buf.PushData(unique_ptr<St7735rCmd>(new St7735rFillGrayscalePixel(
			m_region, {pixel_copy, true}, length))))
	{
		EnableTx();
		return;
	}
	else
	{
		return;
	}
}

void St7735r::FillPixel(const uint16_t *pixel, const size_t length)
{
	if (m_region.x >= kW || m_region.y >= kH)
	{
		return;
	}

	uint16_t *pixel_copy = new uint16_t[length];
	memcpy(pixel_copy, pixel, sizeof(uint16_t) * length);
	if (m_tx_buf.PushData(unique_ptr<St7735rCmd>(new St7735rFillPixel(m_region,
			{pixel_copy, true}, length))))
	{
		EnableTx();
		return;
	}
	else
	{
		return;
	}
}

void St7735r::FillBits(const uint16_t color_t, const uint16_t color_f,
		const bool *data, const size_t length)
{
	if (m_region.x >= kW || m_region.y >= kH)
	{
		return;
	}

	bool *data_copy = new bool[length];
	memcpy(data_copy, data, length);
	if (m_tx_buf.PushData(unique_ptr<St7735rCmd>(new St7735rFillBools(m_region,
			color_t, color_f, {data_copy, true}, length))))
	{
		EnableTx();
		return;
	}
	else
	{
		return;
	}
}

void St7735r::FillBits(const uint16_t color_t, const uint16_t color_f,
		const Byte *data, const size_t bit_length)
{
	if (m_region.x >= kW || m_region.y >= kH)
	{
		return;
	}

	const size_t size = (bit_length + 7) / 8;
	Byte *data_copy = new Byte[size];
	memcpy(data_copy, data, size);
	if (m_tx_buf.PushData(unique_ptr<St7735rCmd>(new St7735rFillBits(m_region,
			color_t, color_f, {data_copy, true}, bit_length))))
	{
		EnableTx();
		return;
	}
	else
	{
		return;
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
	if (m_tx_buf.PushData(unique_ptr<St7735rCmd>(new St7735rInvertColor(flag))))
	{
		EnableTx();
		return;
	}
	else
	{
		return;
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

void St7735r::SetSendCmd(const bool flag)
{
	m_dc.Set(!flag);
}

inline void St7735r::Send(const bool is_cmd, const uint8_t data)
{
	SetSendCmd(is_cmd);
	m_spi.ExchangeData(0, data);
}

void St7735r::OnTxComplete(SpiMaster *spi)
{
	if (m_data_it >= m_data_size)
	{
		// Cache new data
		unique_ptr<St7735rCmd> *cmd = m_tx_buf.GetActiveData();
		size_t data_size = 0;
		while (cmd && (data_size = (*cmd)->GetBytes(m_buf_start, sizeof(m_data),
				m_data)) == 0)
		{
			m_buf_start = 0;
			Byte cmd_code = (*cmd)->NextCmd();
			if (cmd_code == ST7735R_NOP)
			{
				cmd = m_tx_buf.NextData();
			}
			else
			{
				Send(true, cmd_code);
				SetSendCmd(false);
			}
		}
		if (!cmd)
		{
			DisableTx();
			return;
		}

		m_data_it = 0;
		m_buf_start += data_size;
		m_data_size = data_size;
	}

	m_data_it += spi->PushData(0, m_data + m_data_it, m_data_size - m_data_it);
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
}
