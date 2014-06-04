/*
 * linear_ccd.h
 * Linear CCD (for TSL1401CL)
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_LINEAR_CCD_H_
#define LIBSC_LINEAR_CCD_H_

#include <cstdint>
#include <bitset>

namespace libsc
{

class LinearCcd
{
public:
	static constexpr int SENSOR_W = 128;

	explicit LinearCcd(const uint8_t id);

	void StartSample();
	bool SampleProcess();
	/**
	 * Return the latest completely captured data, where dark pixel is false,
	 * true otherwise
	 *
	 * @return
	 */
	const std::bitset<SENSOR_W>& GetData() const
	{
		return m_front_buffer;
	}

	bool IsImageReady() const
	{
		return (m_index >= SENSOR_W);
	}

private:
	const uint8_t m_id;

	std::bitset<SENSOR_W> m_front_buffer;
	std::bitset<SENSOR_W> m_back_buffer;

	int m_index;
};

}

#endif /* LIBSC_LINEAR_CCD_H_ */
