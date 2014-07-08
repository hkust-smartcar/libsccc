/*
 * futaba_s3010.cpp
 * Futaba S3010 RC servo
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <cstdint>

#include "libsc/k60/futaba_s3010.h"

#define PERIOD 20000
#define POS_WIDTH_MIN 1500 - 500
#define POS_WIDTH_MAX 1500 + 500

namespace libsc
{
namespace k60
{

FutabaS3010::FutabaS3010(const uint8_t id)
		: Servo(id, PERIOD, POS_WIDTH_MIN, POS_WIDTH_MAX)
{}

}
}
