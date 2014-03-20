/*
 * trs_d05.h
 * TRS-D05 servo abstraction
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_TRS_D05_H_
#define LIBSC_TRS_D05_H_

#include "libsc/com/servo.h"

namespace libsc
{

class TrsD05 : public Servo
{
public:
	explicit TrsD05(const uint8_t id);
};

}

#endif /* LIBSC_TRS_D05_H_ */
