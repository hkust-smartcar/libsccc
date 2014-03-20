/*
 * futaba_s3010.cpp
 * Futaba S3010 servo abstraction
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cstdint>

#include "libsc/com/futaba_s3010.h"

#define PWM_MIN 500
#define PWM_MAX 1000

namespace libsc
{

FutabaS3010::FutabaS3010(const uint8_t id)
		: Servo(id, PWM_MIN, PWM_MAX)
{}

}
