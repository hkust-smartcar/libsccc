/*
 * futaba_s3010.cpp
 * Futaba S3010 RC servo
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
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

namespace
{

Servo::Config GetServoConfig(const uint8_t id)
{
	Servo::Config config;
	config.id = id;
	config.period = PERIOD;
	config.min_pos_width = POS_WIDTH_MIN;
	config.max_pos_width = POS_WIDTH_MAX;
	return config;
}

}

FutabaS3010::FutabaS3010(const Config &config)
		: Servo(GetServoConfig(config.id))
{}

}
}
