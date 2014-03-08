/*
 * encoder.h
 * Encoder
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_ENCODER_H_
#define LIBSC_ENCODER_H_

#include <cstdint>

namespace libsc
{

class Encoder
{
public:
	Encoder(const uint8_t id);

	uint32_t GetCount();

	static uint32_t CountDiff(const uint32_t a, const uint32_t b)
	{
		return (uint32_t)(b - a);
	}

private:
	const uint8_t m_id;
	uint32_t m_count;
};

}

#endif /* LIBSC_ENCODER_H_ */
