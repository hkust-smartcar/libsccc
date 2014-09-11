/*
 * gyroscope.cpp
 * Gyroscope
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cstdint>

#include <algorithm>

#include "libbase/log.h"
#include "libbase/k60/adc.h"
#include "libbase/k60/pin.h"

#include "libsc/config.h"
#include "libsc/k60/gyroscope.h"

using namespace libbase::k60;

namespace libsc
{
namespace k60
{

#ifdef LIBSC_USE_GYRO

namespace
{

#if LIBSC_USE_GYRO == 1
inline Pin::Name GetPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_GYRO0_ANGULAR;
}

#endif

Adc::Config GetAdcConfig(const uint8_t id)
{
	Adc::Config config;
	config.pin = GetPin(id);
	config.resolution = Adc::Config::Resolution::k16Bit;
	return config;
}

}

Gyroscope::Gyroscope()
		: Gyroscope(3)
{}

Gyroscope::Gyroscope(const Uint avg_size)
		: m_pin(GetAdcConfig(0)),
		  m_current(0),
		  m_avg(0),
		  m_values(nullptr),
		  m_size(avg_size),
		  m_index(0)
{
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
	m_current = m_pin.GetResult();
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
Gyroscope::Gyroscope()
		: m_pin(nullptr), m_current(0), m_avg(0), m_values(nullptr), m_size(0),
		  m_index(0)
{
	LOG_DL("Configured not to use Gyroscope");
}
Gyroscope::Gyroscope(const Uint) : Gyroscope() {}
Gyroscope::~Gyroscope() {}
void Gyroscope::Update() {}

#endif

}
}
