/*
 * ov7725_configurator.h
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
#include LIBBASE_H(gpio)
#include LIBBASE_H(soft_sccb_master)

namespace libsc
{
namespace k60
{

class Ov7725Configurator
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

		enum struct Format
		{
			kYuv = 0,
			kProcessedBayerRaw,
			kRgb,
			kBayerRaw,
		};

		enum struct RgbFormat
		{
			kGbr422 = 0,
			kRgb565,
			kRgb555,
			kRgb444,
		};

		LIBBASE_MODULE(Pin)::Name scl;
		LIBBASE_MODULE(Pin)::Name sda;

		/// Width of the image, [1, 640]
		Uint w = 320;
		/// Height of the image, [1, 480]
		Uint h = 240;
		Fps fps = Fps::kLow;
		Format format = Format::kYuv;
		RgbFormat rgb_format = RgbFormat::kGbr422;

		uint8_t brightness = 0x00;
		uint8_t contrast = 0x40;
	};

	explicit Ov7725Configurator(const Config &config);
	explicit Ov7725Configurator(nullptr_t);
	~Ov7725Configurator();

	bool Verify();

private:
	void InitCom2Reg();
	void InitCom3Reg();
	void InitClock(const Config &config);
	void InitResolution(const Config &config);
	void InitBandingFilter();
	void InitLensCorrection();
	void InitDsp();
	void InitGamma();
	void InitSecialDigitalEffect(const Config &config);
	void InitAutoUv();

	LIBBASE_MODULE(SoftSccbMaster) m_sccb;
};

}
}
