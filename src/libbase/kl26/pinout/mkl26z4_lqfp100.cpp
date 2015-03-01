/*
 * mkl26z4_lqfp100.cpp
 *
 * Author: Harrison Ng
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstdint>

#include <bitset>

#include "libbase/kl26/pinout/mkl26z4_lqfp100.h"

#include "libbase/kl26/adc.h"
#include "libbase/kl26/pin.h"
#include "libbase/kl26/pin_utils.h"
#include "libbase/misc_types.h"

using namespace std;

namespace libbase
{
namespace kl26
{

namespace
{

int GetPinId(const Pin::Name pin)
{
	if ((Uint)pin <= (Uint)Pin::Name::kPta5)
	{
		return (Uint)pin - (Uint)Pin::Name::kPta0 + 33;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPta6
			&& (Uint)pin <= (Uint)Pin::Name::kPta7)
	{
		return (Uint)pin - (Uint)Pin::Name::kPta6 + 39;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPta12
			&& (Uint)pin <= (Uint)Pin::Name::kPta17)
	{
		return (Uint)pin - (Uint)Pin::Name::kPta12 + 41;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPta18
			&& (Uint)pin <= (Uint)Pin::Name::kPta20)
	{
		return (Uint)pin - (Uint)Pin::Name::kPta18 + 49;
	}

	else if ((Uint)pin >= (Uint)Pin::Name::kPtb0
			&& (Uint)pin <= (Uint)Pin::Name::kPtb3)
	{
		return (Uint)pin - (Uint)Pin::Name::kPtb0 + 52;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPtb7
			&& (Uint)pin <= (Uint)Pin::Name::kPtb11)
	{
		return (Uint)pin - (Uint)Pin::Name::kPtb7 + 56;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPtb16
			&& (Uint)pin <= (Uint)Pin::Name::kPtb23)
	{
		return (Uint)pin - (Uint)Pin::Name::kPtb16 + 61;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPtc0
			&& (Uint)pin <= (Uint)Pin::Name::kPtc3)
	{
		return (Uint)pin - (Uint)Pin::Name::kPtc0 + 69;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPtc4
			&& (Uint)pin <= (Uint)Pin::Name::kPtc18)
	{
		return (Uint)pin - (Uint)Pin::Name::kPtc4 + 75;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPtd0
			&& (Uint)pin <= (Uint)Pin::Name::kPtd7)
	{
		return (Uint)pin - (Uint)Pin::Name::kPtd0 + 92;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPte0
			&& (Uint)pin <= (Uint)Pin::Name::kPte6)
	{
		return (Uint)pin - (Uint)Pin::Name::kPte0;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPte16
			&& (Uint)pin <= (Uint)Pin::Name::kPte23)
	{
		return (Uint)pin - (Uint)Pin::Name::kPte16 + 13;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPte29
			&& (Uint)pin <= (Uint)Pin::Name::kPte31)
	{
		return (Uint)pin - (Uint)Pin::Name::kPte29 + 25;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPte24
			&& (Uint)pin <= (Uint)Pin::Name::kPte26)
	{
		return (Uint)pin - (Uint)Pin::Name::kPte24 + 30;
	}
	else
	{
		assert(false);
		return -1;
	}
}

int GetPinId(const Adc::Name pin)
{
	if (pin == Adc::Name::kAdc0DAd1 || pin == Adc::Name::kAdc0Ad1)
	{
		return 13;
	}
	else if (pin == Adc::Name::kAdc0Ad5A)
	{
		return 14;
	}
	else if (pin == Adc::Name::kAdc0DAd2 || pin == Adc::Name::kAdc0Ad2)
	{
		return 15;
	}
	else if (pin == Adc::Name::kAdc0Ad6A)
	{
		return 16;
	}
	else if (pin == Adc::Name::kAdc0DAd0 || pin == Adc::Name::kAdc0Ad0)
	{
		return 17;
	}
	else if (pin == Adc::Name::kAdc0Ad4A)
	{
		return 18;
	}
	else if (pin == Adc::Name::kAdc0DAd3 || pin == Adc::Name::kAdc0Ad3)
	{
		return 19;
	}
	else if (pin == Adc::Name::kAdc0Ad7A)
	{
		return 20;
	}
	else if (pin == Adc::Name::kAdc0Ad4B)
	{
		return 25;
	}
	else if (pin == Adc::Name::kAdc0Ad23)
	{
		return 27;
	}
	else if (pin == Adc::Name::kAdc0Ad8)
	{
		return 52;
	}
	else if (pin == Adc::Name::kAdc0Ad9)
	{
		return 53;
	}
	else if (pin == Adc::Name::kAdc0Ad12)
	{
		return 54;
	}
	else if (pin == Adc::Name::kAdc0Ad13)
	{
		return 55;
	}
	else if (pin == Adc::Name::kAdc0Ad14)
	{
		return 69;
	}
	else if (pin == Adc::Name::kAdc0Ad15)
	{
		return 70;
	}
	else if (pin == Adc::Name::kAdc0Ad11)
	{
		return 71;
	}
	else if (pin == Adc::Name::kAdc0Ad6B)
	{
		return 97;
	}
	else if (pin == Adc::Name::kAdc0Ad7B)
	{
		return 98;
	}
	else
	{
		assert(false);
		return -1;
	}
}

}

Mkl26z4Lqfp100::Mkl26z4Lqfp100()
{}

bool Mkl26z4Lqfp100::RegPin_(const Uint pin)
{
	if (pin >= GetPinCount())
	{
		assert(false);
		return false;
	}

	if (m_is_pins_active[pin])
	{
		return false;
	}
	else
	{
		m_is_pins_active[pin] = true;
		return true;
	}
}

bool Mkl26z4Lqfp100::RegPin_(const Pin::Name pin)
{
	return RegPin_((Uint)GetPinId(pin));
}

bool Mkl26z4Lqfp100::RegPin_(const Adc::Name pin)
{
	return RegPin_((Uint)GetPinId(pin));
}

void Mkl26z4Lqfp100::UnregPin_(const Uint pin)
{
	if (pin >= GetPinCount())
	{
		assert(false);
		return;
	}

	m_is_pins_active[pin] = false;
}

void Mkl26z4Lqfp100::UnregPin_(const Pin::Name pin)
{
	UnregPin_((Uint)GetPinId(pin));
}

void Mkl26z4Lqfp100::UnregPin_(const Adc::Name pin)
{
	UnregPin_((Uint)GetPinId(pin));
}

Adc::Name Mkl26z4Lqfp100::GetAdc(const Pin::Name pin)
{
	switch (pin)
	{
	case Pin::Name::kPtb0:
		return Adc::Name::kAdc0Ad8;

	case Pin::Name::kPtb1:
		return Adc::Name::kAdc0Ad9;

	case Pin::Name::kPtb2:
		return Adc::Name::kAdc0Ad12;

	case Pin::Name::kPtb3:
		return Adc::Name::kAdc0Ad13;

	case Pin::Name::kPtc0:
		return Adc::Name::kAdc0Ad14;

	case Pin::Name::kPtc1:
		return Adc::Name::kAdc0Ad15;

	case Pin::Name::kPtc2:
		return Adc::Name::kAdc0Ad11;

	case Pin::Name::kPtd1:
		return Adc::Name::kAdc0Ad5B;

	case Pin::Name::kPtd5:
		return Adc::Name::kAdc0Ad6B;

	case Pin::Name::kPtd6:
		return Adc::Name::kAdc0Ad7B;

	case Pin::Name::kPte16:
		return Adc::Name::kAdc0Ad1;

	case Pin::Name::kPte17:
		return Adc::Name::kAdc0Ad5A;

	case Pin::Name::kPte18:
		return Adc::Name::kAdc0Ad2;

	case Pin::Name::kPte19:
		return Adc::Name::kAdc0Ad6A;

	case Pin::Name::kPte20:
		return Adc::Name::kAdc0Ad0;

	case Pin::Name::kPte21:
		return Adc::Name::kAdc0Ad4A;

	case Pin::Name::kPte22:
		return Adc::Name::kAdc0Ad3;

	case Pin::Name::kPte23:
		return Adc::Name::kAdc0Ad7A;

	case Pin::Name::kPte29:
		return Adc::Name::kAdc0Ad4B;

	case Pin::Name::kPte30:
		return Adc::Name::kAdc0Ad23;

	default:
		return Adc::Name::kDisable;
	}
}

}
}
