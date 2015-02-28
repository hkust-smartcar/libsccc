/*
 * ov7725.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include <memory>

#include "libbase/misc_types.h"
#include "libbase/k60/dma.h"
#include "libbase/k60/gpio.h"
#include "libbase/k60/gpio_array.h"
#include "libbase/k60/soft_sccb_master.h"

namespace libsc
{
namespace k60
{

class Ov7725
{
public:
	struct Config
	{
		enum struct Fps
		{
			kLow = 0,
			kMid,
			kHigh,
		};
		/// Width of the image, [1, 640]
		Uint w;
		/// Height of the image, [1, 480]
		Uint h;
		Fps fps;
		uint8_t brightness = 0x00;
		uint8_t contrast = 0x40;
	};

	explicit Ov7725(const Config &config);
	~Ov7725();

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
	bool IsAvailable() const
	{
		return m_is_available;
	}
	/**
	 * Lock and return the buffer. When the buffer is locked, new incoming frame
	 * will be dropped until UnlockBuffer() is called (i.e., the buffer is
	 * guaranteed to stay unchanged)
	 *
	 * @return Reference to the image buffer, 8 pixel/byte
	 */
	const Byte* LockBuffer();
	void UnlockBuffer();

private:
	bool Verify();

	void InitCom2Reg();
	void InitCom3Reg();
	void InitClock(const Config &config);
	void InitResolution();
	void InitBandingFilter();
	void InitLensCorrection();
	void InitDsp();
	void InitGamma();
	void InitSecialDigitalEffect(const Config &config);
	void InitAutoUv();
	void InitDma();

	void OnVsync(libbase::k60::Gpi *gpi);
	void OnDmaComplete(libbase::k60::Dma *dma);

	libbase::k60::SoftSccbMaster m_sccb;
	libbase::k60::GpiArray m_data_array;
	libbase::k60::Gpi m_clock;
	libbase::k60::Gpi m_vsync;
	libbase::k60::Dma *m_dma;

	Uint m_w;
	Uint m_h;
	Uint m_buf_size;
	std::unique_ptr<Byte[]> m_front_buf;
	std::unique_ptr<Byte[]> m_back_buf;

	bool m_is_shoot;
	bool m_is_lock_buffer;
	volatile bool m_is_available;
	volatile bool m_is_dma_start;
};

}
}
