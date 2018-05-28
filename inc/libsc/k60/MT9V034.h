/*
 * MT9V034.h
 *
 * Author: Leslie Lee (LeeChunHei)
 * Copyright (c) 2014-2018 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include <memory>

#include "libbase/misc_types.h"
#include "libbase/k60/dma.h"
#include "libbase/k60/gpio.h"
#include "libbase/k60/gpio_array.h"
#include "libbase/helper.h"
#include LIBBASE_H(soft_sccb_master)

namespace libsc {
namespace k60 {

class MT9V034 {
public:
	struct Config {
		enum binning {
			k1, k2, k4
		};
		//Width binning of the image, [1, 2, 4]
		binning w_binning;
		//Height binning of the image, [1, 2, 4]
		binning h_binning;
//		uint8_t brightness = 0x00;
//		uint8_t contrast = 0x40;
		//Coarse Shutter Width 1, [0 - 32765]
		uint32_t shutter_width_1 = 443;
		//Coarse Shutter Width 2, [0 - 32765]
		uint32_t shutter_width_2 = 473;
		//T2 Ratio, [0 - 15]
		uint8_t t2 = 4;
		//T3 Ratio, [0 - 15]
		uint8_t t3 = 6;
		//Exposure Knee Point Auto Adjust Enable
		bool auto_knee_point = true;
		//Single Knee Enable
		bool single_knee = false;
		//Coarse Shutter Width Total, [0 - 32765]
		uint32_t shutter_width_total = 480;
		//HDR enable
		bool HDR = false;
		//V1 Voltage Level, [0 - 63]
		uint8_t v1_voltage = 39;
		//V2 Voltage Level, [0 - 63]
		uint8_t v2_voltage = 26;
		//V3 Voltage Level, [0 - 63]
		uint8_t v3_voltage = 5;
		//V4 Voltage Level, [0 - 63]
		uint8_t v4_voltage = 3;
		//Dark Average Thresholds, [0 - 255]
		uint8_t lower_threshold = 29;
		uint8_t upper_threshold = 35;
		//Auto Exposure Control Enable
		bool AEC = true;
		//Auto Gain Control Enable
		bool AGC = true;
	};

	explicit MT9V034(const Config &config);
	~MT9V034();

	/**
	 * Start shooting frames
	 */
	void Start();
	/**
	 * Stop shooting new frames after finishing the current one
	 */
	void Stop();

	/**
	 * Return whether an image is ready. This is only useful to check whether
	 * the first frame arrived or not as this method will always return true
	 * afterwards until the next Stop()/Start()
	 *
	 * @return
	 */
	bool IsAvailable() const {
		return m_is_available;
	}
	/**
	 * Lock and return the buffer. When the buffer is locked, new incoming frame
	 * will be dropped until UnlockBuffer() is called (i.e., the buffer is
	 * guaranteed to stay unchanged)
	 *
	 * @return Reference to the image buffer, 1 pixel/byte
	 */
	const Byte* LockBuffer();
	void UnlockBuffer();

	Uint GetW() const {
		return m_w;
	}

	Uint GetH() const {
		return m_h;
	}

private:
	void RegSet(uint8_t reg_addr, uint16_t value);

	void InitDma();

	void OnLine(libbase::k60::Gpi *gpi);
	void OnVsync(libbase::k60::Gpi *gpi);
	void OnDmaComplete(libbase::k60::Dma *dma);

	LIBBASE_MODULE(SoftSccbMaster) m_sccb;
	libbase::k60::GpiArray m_data_array;
	libbase::k60::Gpi m_clock;
	libbase::k60::Gpi m_vsync;
	libbase::k60::Dma *m_dma;
	libbase::k60::Dma::Config m_dma_config;

	Uint m_w;
	Uint m_h;
	Uint m_buf_size;
	std::unique_ptr<Byte[]> m_front_buf;
	std::unique_ptr<Byte[]> m_back_buf;

	bool m_is_shoot;
	bool m_is_lock_buffer;
	bool m_front_buffer_locked;
	bool m_front_buffer_writing;
	volatile bool m_is_available;
	volatile bool m_is_dma_start;
};

}
}
