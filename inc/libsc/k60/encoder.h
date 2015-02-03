/*
 * encoder.h
 * Phase A/B encoder. Provides two mechanisms to read the feedback value,
 * interrupt based software emulation and hardware quadrature decoding. Software
 * emulation will pose a large overhead to the system
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/k60/ftm_quad_decoder.h"
#include "libbase/k60/gpio.h"
#include "libbase/k60/soft_quad_decoder.h"

#include "libsc/config.h"

namespace libsc
{
namespace k60
{

class Encoder
{
public:
#if LIBSC_USE_SOFT_ENCODER
	typedef libbase::k60::SoftQuadDecoder QuadDecoder;

#else
	typedef libbase::k60::FtmQuadDecoder QuadDecoder;

#endif // LIBSC_USE_SOFT_ENCODER

	struct Config
	{
		uint8_t id;
	};

	virtual ~Encoder();

	void Update();

	/**
	 * Return the decoded count
	 *
	 * @return
	 */
	int32_t GetCount() const
	{
		return m_count;
	}

protected:
	/**
	 * Use to initialize the encoder in possibly a polymorphic way, notice that
	 * Initializer::config is stored as a reference only
	 */
	struct Initializer
	{
		explicit Initializer(const Config &config)
				: config(config)
		{}

		virtual QuadDecoder::Config GetQuadDecoderConfig() const;

		const Config &config;
	};

	explicit Encoder(const Initializer &initializer);

private:
	int32_t m_count;

	QuadDecoder m_quad_decoder;
};

}
}
