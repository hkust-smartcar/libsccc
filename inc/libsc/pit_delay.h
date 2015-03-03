/*
 * pit_delay.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/helper.h"
#include LIBBASE_H(pit)

#include "libsc/delay.h"

namespace libsc
{

class PitDelay : public Delay
{
public:
	explicit PitDelay(const uint8_t channel);

	void DelayUs(const uint16_t us) override;
	void DelayMs(const uint16_t ms) override;

private:
	inline void DelayCount(const uint32_t count);

	LIBBASE_MODULE(Pit) m_pit;
};

}
