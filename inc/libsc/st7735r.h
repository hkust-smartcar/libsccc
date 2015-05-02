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

#include "libbase/helper.h"
#include "libbase/misc_types.h"

#include LIBBASE_H(soft_spi_master)
#if MK60DZ10 || MK60D10 || MK60F15
#include LIBBASE_H(spi_master)

#elif MKL26Z4
#ifndef LIBSC_USE_SOFT_ST7735R
#include LIBBASE_H(spi_master)
#endif

#endif

#include "libsc/config.h"
#include "libsc/lcd.h"

namespace libsc
{


class St7735r : public Lcd
{
public:
#if LIBSC_USE_SOFT_ST7735R
	typedef LIBBASE_MODULE(SoftSpiMaster) SpiMaster;

#else
	typedef LIBBASE_MODULE(SpiMaster) SpiMaster;

#endif // LIBSC_USE_SOFT_ST7735R

	struct Config
	{
		/// Revert the screen upside down
		bool is_revert = false;
		/// Whether using a BGR panel instead of a RGB one
		bool is_bgr = false;
		/// Frame rate of the screen
		uint8_t fps = 60;
	};

	explicit St7735r(const Config &config);

	~St7735r() override{}

	void SetRegion(const Rect &rect) override
	{
		m_region = rect;
	}

	Rect GetRegion() override
	{
		return m_region;
	}

	void ClearRegion() override
	{
		m_region = Rect{0, 0, static_cast<uint8_t>(GetW()),
				static_cast<uint8_t>(GetH())};
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

	void SetInvertColor(const bool flag);

	static constexpr Uint GetW()
	{
		return kW;
	}

	static constexpr Uint GetH()
	{
		return kH;
	}

private:
	static constexpr Uint kW = 128;
	static constexpr Uint kH = 160;

	void InitMadctl(const Config &config);
	void InitFrmctr(const Config &config);
	void InitPwctr();
	void InitGamma();

	void SetActiveRect();
	inline void Send(const bool is_cmd, const uint8_t data);

	SpiMaster m_spi;
	LIBBASE_MODULE(Gpo) m_rst;
	LIBBASE_MODULE(Gpo) m_dc;

	Rect m_region;
};


}
