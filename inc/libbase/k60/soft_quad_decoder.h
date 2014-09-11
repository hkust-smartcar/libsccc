/*
 * soft_quad_decoder.h
 * Software emulated quadrature decoding using pin interrupt and GPIO
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "libbase/k60/gpio.h"

namespace libbase
{
namespace k60
{

class SoftQuadDecoder : public QuadDecoder
{
public:
	typedef QuadDecoder::Config Config;

	explicit SoftQuadDecoder(const Config &config);
	explicit SoftQuadDecoder(nullptr_t);
	SoftQuadDecoder(const SoftQuadDecoder&) = delete;
	SoftQuadDecoder(SoftQuadDecoder &&rhs);
	~SoftQuadDecoder();

	SoftQuadDecoder& operator=(const SoftQuadDecoder&) = delete;
	SoftQuadDecoder& operator=(SoftQuadDecoder &&rhs);
	operator bool() const override
	{
		return m_is_init;
	}

	int32_t GetCount() override;
	void ResetCount() override;

private:
	void Uninit();

	void OnTick(Gpi*);

	bool m_is_invert_a_polarity;
	bool m_is_invert_b_polarity;

	Gpi m_qda;
	Gpi m_qdb;

	volatile int32_t m_count;

	bool m_is_init;
};

}
}
