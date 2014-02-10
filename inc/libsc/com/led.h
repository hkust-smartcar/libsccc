/*
 * led.h
 * LED abstraction
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_LED_H_
#define LIBSC_LED_H_

#include <cstdint>

namespace libsc
{

class Led
{
public:
	explicit Led(const uint8_t id);

	void SetEnable(const bool flag);

private:
	uint8_t m_id;
};

}

#endif /* LIBSC_LED_H_ */
