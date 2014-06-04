/*
 * joystick.h
 * Joystick
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_JOYSTICK_H_
#define LIBSC_JOYSTICK_H_

#include <cstdint>

namespace libsc
{

class Joystick
{
public:
	enum State
	{
		IDLE,
		UP,
		DOWN,
		LEFT,
		RIGHT,
		SELECT,
	};

	explicit Joystick(const uint8_t id);

	State GetState() const;

private:
	uint8_t m_id;
};

}

#endif /* LIBSC_JOYSTICK_H_ */
