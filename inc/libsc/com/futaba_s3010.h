/*
 * futaba_s3010.h
 * Futaba S3010 servo abstraction
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_FUTABA_S3010_H_
#define LIBSC_FUTABA_S3010_H_

#include "libsc/com/servo.h"

namespace libsc
{

class FutabaS3010 : public Servo
{
public:
	explicit FutabaS3010(const uint8_t id);
};

}

#endif /* LIBSC_FUTABA_S3010_H_ */
