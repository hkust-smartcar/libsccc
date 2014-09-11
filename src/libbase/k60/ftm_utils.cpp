/*
 * ftm_utils.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include "libbase/k60/ftm.h"
#include "libbase/k60/ftm_utils.h"
#include "libbase/k60/pin.h"

namespace libbase
{
namespace k60
{

Ftm::Name FtmUtils::GetFtmName(const Pin::Name pin)
{
	switch (pin)
	{
	case Pin::Name::kPta3:
	case Pin::Name::kPtc1:
		return Ftm::Name::kFtm0Ch0;

	case Pin::Name::kPta4:
	case Pin::Name::kPtc2:
		return Ftm::Name::kFtm0Ch1;

	case Pin::Name::kPta5:
	case Pin::Name::kPtc3:
		return Ftm::Name::kFtm0Ch2;

	case Pin::Name::kPta6:
	case Pin::Name::kPtc4:
		return Ftm::Name::kFtm0Ch3;

	case Pin::Name::kPta7:
	case Pin::Name::kPtd4:
		return Ftm::Name::kFtm0Ch4;

	case Pin::Name::kPta0:
	case Pin::Name::kPtd5:
		return Ftm::Name::kFtm0Ch5;

	case Pin::Name::kPta1:
	case Pin::Name::kPtd6:
		return Ftm::Name::kFtm0Ch6;

	case Pin::Name::kPta2:
	case Pin::Name::kPtd7:
		return Ftm::Name::kFtm0Ch7;

	case Pin::Name::kPta8:
	case Pin::Name::kPta12:
	case Pin::Name::kPtb0:
		return Ftm::Name::kFtm1Ch0;

	case Pin::Name::kPta9:
	case Pin::Name::kPta13:
	case Pin::Name::kPtb1:
		return Ftm::Name::kFtm1Ch1;

	case Pin::Name::kPta10:
	case Pin::Name::kPtb18:
		return Ftm::Name::kFtm2Ch0;

	case Pin::Name::kPta11:
	case Pin::Name::kPtb19:
		return Ftm::Name::kFtm2Ch1;

	default:
		return Ftm::Name::kDisable;
	}
}

}
}
