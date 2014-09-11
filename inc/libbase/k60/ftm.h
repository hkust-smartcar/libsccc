/*
 * ftm.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include "libbase/k60/pin.h"

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

		kDisable = 24
	};
};

}
}
