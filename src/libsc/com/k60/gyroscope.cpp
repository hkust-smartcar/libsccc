/*
 * gyroscope.cpp
 * Gyroscope
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>
#include <hw_common.h>

#include <cstdint>

#include <algorithm>

#include <MK60_adc.h>

#include "libsc/com/config.h"
#include "libsc/com/gyroscope.h"
#include "libsc/com/k60/adc_utils.h"

namespace libsc
{

#ifdef LIBSC_USE_GYRO

Gyroscope::Gyroscope()
		: Gyroscope(3)
{}

Gyroscope::Gyroscope(const Uint avg_size)
		: m_current(0), m_avg(0), m_values(nullptr), m_size(avg_size), m_index(0)
{
	adc_init(AdcUtils::GetAdcChannel<LIBSC_GYRO0_ANGULAR>());
	if (m_size > 0)
	{
		m_values = new uint16_t[m_size];
		std::fill(m_values, m_values + m_size, 0);
	}
}

Gyroscope::~Gyroscope()
{
	delete[] m_values;
}

void Gyroscope::Update()
{
	m_current = adc_once(AdcUtils::GetAdcChannel<LIBSC_GYRO0_ANGULAR>(),
			ADC_16bit);
	if (!m_values)
	{
		m_avg = m_current;
	}
	else
	{
		m_avg -= m_values[m_index] / (float)m_size;
		m_avg += m_current / (float)m_size;
		m_values[m_index] = m_current;

		if (++m_index >= m_size)
		{
			m_index = 0;
		}
	}
}

#else
Gyroscope::Gyroscope() : m_current(0), m_avg(0) {}
Gyroscope::Gyroscope(const Uint) : Gyroscope() {}
Gyroscope::~Gyroscope() {}
void Gyroscope::Update() {}

#endif

}
