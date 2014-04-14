/*
 * gyroscope.h
 * Gyroscope
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_GYROSCOPE_H_
#define LIBSC_GYROSCOPE_H_

#include <mini_common.h>
#include <cstdint>

namespace libsc
{

class Gyroscope
{
public:
	Gyroscope();
	explicit Gyroscope(const Uint avg_size);
	~Gyroscope();

	void Update();

	uint16_t GetAngle() const
	{
		return m_current;
	}

	float GetAverageAngle() const
	{
		return m_avg;
	}

private:
	uint16_t m_current;
	float m_avg;
	uint16_t *m_values;
	Uint m_size;
	Uint m_index;
};

}

#endif /* LIBSC_GYROSCOPE_H_ */
