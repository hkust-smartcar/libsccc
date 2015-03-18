/*
 * linear_ccd.h
 * Linear CCD (for TSL1401CL)
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>
#include <array>

#include "libbase/helper.h"
#include LIBBASE_H(adc)
#include LIBBASE_H(gpio)

namespace libsc
{

class LinearCcd
{
public:
	static constexpr int kSensorW = 128;

	explicit LinearCcd(const uint8_t id);

	void StartSample();
	bool SampleProcess();
	/**
	 * Return the latest completely captured data, where dark pixel is false,
	 * true otherwise
	 *
	 * @return
	 */
	const std::array<uint16_t, kSensorW>& GetData() const
	{
		return m_front_buffer;
	}

	bool IsImageReady() const
	{
		return (m_index >= kSensorW);
	}

private:
	inline void Delay();

	LIBBASE_MODULE(Adc) m_ad_pin;
	LIBBASE_MODULE(Gpo) m_clk_pin;
	LIBBASE_MODULE(Gpo) m_si_pin;

	std::array<uint16_t, kSensorW> m_front_buffer;
	std::array<uint16_t, kSensorW> m_back_buffer;

	int m_index;
};

}
