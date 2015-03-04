/*
 * encoder.cpp
 * Phase A/B encoder
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdint>

#include "libbase/log.h"
#include "libbase/helper.h"
#include "libbase/pinout_macros.h"
#include LIBBASE_H(gpio)
#include LIBBASE_H(soft_quad_decoder)
#include LIBBASE_H(vectors)

#if PINOUT_FTM_COUNT
#include LIBBASE_H(ftm_quad_decoder)

#elif PINOUT_TPM_COUNT
#include LIBBASE_H(tpm_quad_decoder)

#endif // PINOUT_FTM_COUNT

#include "libsc/config.h"
#include "libsc/encoder.h"

using namespace LIBBASE_NS;

namespace libsc
{

#if LIBSC_USE_ENCODER

namespace
{

#if LIBSC_USE_ENCODER == 1
inline Pin::Name GetQda(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_ENCODER0_QDA;
}

inline Pin::Name GetQdb(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_ENCODER0_QDB;
}

#else
inline Pin::Name GetQda(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break

	case 0:
		return LIBSC_ENCODER0_QDA;

	case 1:
		return LIBSC_ENCODER1_QDA;
	}
}

inline Pin::Name GetQdb(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break

	case 0:
		return LIBSC_ENCODER0_QDB;

	case 1:
		return LIBSC_ENCODER1_QDB;
	}
}

#endif // LIBSC_USE_ENCODER

#if PINOUT_TPM_COUNT
#if LIBSC_USE_ENCODER == 1
inline uint8_t GetEncoderTpmModule(const uint8_t)
{
	return LIBSC_ENCODER0_TPM_MODULE;
}

#else
inline uint8_t GetEncoderTpmModule(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break

	case 0:
		return LIBSC_ENCODER0_TPM_MODULE;

	case 1:
		return LIBSC_ENCODER1_TPM_MODULE;
	}
}

#endif // LIBSC_USE_ENCODER
#endif // PINOUT_TPM_COUNT

inline Encoder::QuadDecoder::Config GetQuadDecoderConfig_(const uint8_t id)
{
	Encoder::QuadDecoder::Config config;
	config.a_pin = GetQda(id);
	config.b_pin = GetQdb(id);
#if PINOUT_FTM_COUNT
	config.a_filter_length = 1;
	config.b_filter_length = 1;
#elif PINOUT_TPM_COUNT
	config.tpm_module = GetEncoderTpmModule(id);
#endif
	return config;
}

}

Encoder::QuadDecoder::Config Encoder::Initializer::GetQuadDecoderConfig() const
{
	return GetQuadDecoderConfig_(config.id);
}

Encoder::Encoder(const Initializer &initializer)
		: m_count(0),
		  m_quad_decoder(initializer.GetQuadDecoderConfig())
{}

Encoder::Encoder(Encoder &&rhs)
		: m_count(rhs.m_count),
		  m_quad_decoder(std::move(rhs.m_quad_decoder))
{}

Encoder::~Encoder()
{}

void Encoder::Update()
{
	m_count = m_quad_decoder.GetCount();
	m_quad_decoder.ResetCount();
}

#else
Encoder::Encoder(const Initializer&)
		: Encoder(nullptr)
{}
Encoder::Encoder(Encoder&&)
		: Encoder(nullptr)
{}
Encoder::Encoder(nullptr_t)
		: m_count(0), m_quad_decoder(nullptr)
{
	LOG_DL("Configured not to use Encoder");
}
Encoder::~Encoder() {}
void Encoder::Update() {}

#endif /* LIBSC_USE_ENCODER */

}
