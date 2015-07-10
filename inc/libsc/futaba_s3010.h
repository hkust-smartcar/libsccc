/*
 * futaba_s3010.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libsc/servo.h"

namespace libsc
{

/**
 * Futaba S3010 RC servo
 */
class FutabaS3010 : public Servo
{
public:
	struct Config
	{
		uint8_t id;
	};

	explicit FutabaS3010(const Config &config);
};

}
