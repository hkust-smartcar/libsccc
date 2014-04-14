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
	explicit Encoder(const uint8_t id);

	void Update();

	int16_t GetCount() const
	{
		return m_count;
	}

private:
	const uint8_t m_id;
	int16_t m_count;
};

}

#endif /* LIBSC_ENCODER_H_ */
