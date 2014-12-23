/*
 * encoder.h
 * Phase A/B encoder. Provides two mechanisms to read the feedback value,
 * interrupt based software emulation and hardware quadrature decoding. Software
 * emulation will pose a large overhead to the system
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
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
	struct Config : public Motor::Config
	{
		uint8_t id;
	};

	explicit Encoder(const Config &config);

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

private:
	int32_t m_count;

#ifdef LIBSC_USE_ENCODER
#ifdef LIBSC_USE_SOFT_ENCODER
	libbase::k60::SoftQuadDecoder m_quad_decoder;

#else
	libbase::k60::FtmQuadDecoder m_quad_decoder;

#endif // LIBSC_USE_SOFT_ENCODER
#endif // LIBSC_USE_ENCODER
};

}
}
