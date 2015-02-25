/*
 * spi_utils.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/spi_utils.h"

namespace libbase
{
namespace k60
{

Uint SpiUtils::GetCsNumber(const Pin::Name pin)
{
	switch (pin)
	{
	case Pin::Name::kPta14:
		return 0;

	case Pin::Name::kPtb9:
		return 1;

	case Pin::Name::kPtb10:
		return 0;

	case Pin::Name::kPtb20:
		return 0;

	case Pin::Name::kPtc0:
		return 4;

	case Pin::Name::kPtc1:
		return 3;

	case Pin::Name::kPtc2:
		return 2;

	case Pin::Name::kPtc3:
		return 1;

	case Pin::Name::kPtc4:
		return 0;

	case Pin::Name::kPtd0:
		return 0;

	case Pin::Name::kPtd4:
		return 1;

	case Pin::Name::kPtd5:
		return 2;

	case Pin::Name::kPtd6:
		return 3;

	case Pin::Name::kPtd11:
		return 0;

	case Pin::Name::kPtd15:
		return 1;

	case Pin::Name::kPte0:
		return 1;

	case Pin::Name::kPte4:
		return 0;

	case Pin::Name::kPte5:
		return 2;

	case Pin::Name::kPte6:
		return 3;

	default:
		assert(false);
		return 0;
	}
}

}
}
