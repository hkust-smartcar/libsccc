/*
 * pit_delay.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <cstdint>

#include "libbase/k60/pit.h"

#include "libsc/k60/delay.h"

namespace libsc
{
namespace k60
{

class PitDelay : public Delay
{
public:
	explicit PitDelay(const uint8_t channel);

	void DelayUs(const uint16_t us) override;
	void DelayMs(const uint16_t ms) override;

private:
	inline void DelayCount(const uint32_t count);

	libbase::k60::Pit m_pit;
};

}
}
