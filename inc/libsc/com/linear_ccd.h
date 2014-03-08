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

namespace libsc
{

class LinearCcd
{
public:
	static constexpr int SENSOR_W = 256;

	LinearCcd();

	/**
	 * Return the captured data in a bool array, where light pixel is true,
	 * false otherwise. Must NOT delete the returned array
	 *
	 * @return
	 */
	const bool* SampleData();

private:
	bool m_buffer[SENSOR_W];
	bool m_clk_state;
};

}

#endif /* LIBSC_LINEAR_CCD_H_ */
