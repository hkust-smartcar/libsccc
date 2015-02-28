/*
 * ftm.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libbase/k60/pinout_macros.h"

namespace libbase
{
namespace k60
{

class Ftm
{
public:
	enum struct Name
	{
		kFtm0Ch0 = 0,
		kFtm0Ch1,
		kFtm0Ch2,
		kFtm0Ch3,
		kFtm0Ch4,
		kFtm0Ch5,
		kFtm0Ch6,
		kFtm0Ch7,

		kFtm1Ch0 = 8,
		kFtm1Ch1,

		kFtm2Ch0 = 16,
		kFtm2Ch1,

#if PINOUT_FTM_COUNT > 3
		kFtm3Ch0 = 24,
		kFtm3Ch1,
		kFtm3Ch2,
		kFtm3Ch3,
		kFtm3Ch4,
		kFtm3Ch5,
		kFtm3Ch6,
		kFtm3Ch7,
#endif

		kDisable = 32
	};

	enum struct QdName
	{
		kFtm0QdPha = 0,
		kFtm0QdPhb,
		kFtm1QdPha,
		kFtm1QdPhb,
		kFtm2QdPha,
		kFtm2QdPhb,
#if PINOUT_FTM_COUNT > 3
		kFtm3QdPha,
		kFtm3QdPhb,
#endif

		kDisable
	};
};

}
}
