/*
 * button.h
 * Button
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_BUTTON_H_
#define LIBSC_BUTTON_H_

#include <cstdint>

namespace libsc
{

class Button
{
public:
	explicit Button(const uint8_t id);

	bool IsDown() const;
	bool IsUp() const
	{
		return !IsDown();
	}

private:
	uint8_t m_id;
};

}

#endif /* LIBSC_BUTTON_H_ */
