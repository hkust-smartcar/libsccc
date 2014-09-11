/*
 * gyroscope.h
 * Gyroscope
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <cstdint>

#include "libbase/k60/adc.h"

namespace libsc
{
namespace k60
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
	libbase::k60::Adc m_pin;

	uint16_t m_current;
	float m_avg;
	uint16_t *m_values;
	Uint m_size;
	Uint m_index;
};

}
}
