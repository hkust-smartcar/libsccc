/*
 * switch.h
 * Switch
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_SWITCH_H_
#define LIBSC_SWITCH_H_

#include <cstdint>

namespace libsc
{

class Switch
{
public:
	explicit Switch(const uint8_t id);

	bool IsOn() const;
	bool IsOff() const
	{
		return !IsOn();
	}

private:
	uint8_t m_id;
};

}

#endif /* LIBSC_SWITCH_H_ */
