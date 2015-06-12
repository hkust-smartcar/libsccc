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

#include <memory>

#include "libbase/helper.h"
#include "libbase/misc_types.h"
#include LIBBASE_H(gpio)
#include LIBBASE_H(spi_master)

#include "libsc/config.h"
#include "libsc/lcd.h"
#include "libsc/next/st7735r_cmd.h"
#include "libutil/fixed_circular_buffer.h"

namespace libsc
{
namespace next
{

class St7735r : public Lcd
{
public:
	typedef LIBBASE_MODULE(SpiMaster) SpiMaster;

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
	static constexpr Uint READ_BUFFER_SIZE = 32;

	void InitMadctl(const Config &config);
	void InitFrmctr(const Config &config);
	void InitPwctr();
	void InitGamma();

	inline void EnableTx();
	inline void DisableTx();

	void OnTxComplete(SpiMaster *spi);

	void SetActiveRect();
	void SetSendCmd(const bool flag);
	inline void Send(const bool is_cmd, const uint8_t data);

	libutil::FixedCircularBuffer<std::unique_ptr<St7735rCmd>> m_tx_buf;
	volatile bool m_is_tx_idle;
	Uint m_buf_start;
	Uint m_data_it;
	Uint m_data_size;
	Byte m_data[READ_BUFFER_SIZE];

	Rect m_region;

	SpiMaster m_spi;
	LIBBASE_MODULE(Gpo) m_rst;
	LIBBASE_MODULE(Gpo) m_dc;
};

}
}
