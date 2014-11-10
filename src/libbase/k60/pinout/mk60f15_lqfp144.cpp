/*
 * mk60f15_lqfp144.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>

#include <bitset>

#include "libbase/k60/pinout/mk60f15_lqfp144.h"

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"

namespace libbase
{
namespace k60
{

namespace
{

int GetPinId(const Pin::Name pin)
{
	if ((Uint)pin <= (Uint)Pin::Name::kPta5)
	{
		return (Uint)pin - (Uint)Pin::Name::kPta0 + 49;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPta6
			&& (Uint)pin <= (Uint)Pin::Name::kPta17)
	{
		return (Uint)pin - (Uint)Pin::Name::kPta6 + 57;
	}
	else if (pin == Pin::Name::kPta18)
	{
		return 71;
	}
	else if (pin == Pin::Name::kPta19)
	{
		return 72;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPta24
			&& (Uint)pin <= (Uint)Pin::Name::kPta29)
	{
		return (Uint)pin - (Uint)Pin::Name::kPta24 + 74;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPtb0
			&& (Uint)pin <= (Uint)Pin::Name::kPtb11)
	{
		return (Uint)pin - (Uint)Pin::Name::kPtb0 + 80;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPtb16
			&& (Uint)pin <= (Uint)Pin::Name::kPtb23)
	{
		return (Uint)pin - (Uint)Pin::Name::kPtb16 + 94;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPtc0
			&& (Uint)pin <= (Uint)Pin::Name::kPtc3)
	{
		return (Uint)pin - (Uint)Pin::Name::kPtc0 + 102;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPtc4
			&& (Uint)pin <= (Uint)Pin::Name::kPtc15)
	{
		return (Uint)pin - (Uint)Pin::Name::kPtc4 + 108;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPtc16
			&& (Uint)pin <= (Uint)Pin::Name::kPtc19)
	{
		return (Uint)pin - (Uint)Pin::Name::kPtc16 + 122;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPtd0
			&& (Uint)pin <= (Uint)Pin::Name::kPtd6)
	{
		return (Uint)pin - (Uint)Pin::Name::kPtd0 + 126;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPtd7
			&& (Uint)pin <= (Uint)Pin::Name::kPtd15)
	{
		return (Uint)pin - (Uint)Pin::Name::kPtd7 + 135;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPte0
			&& (Uint)pin <= (Uint)Pin::Name::kPte3)
	{
		return (Uint)pin - (Uint)Pin::Name::kPte0 + 0;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPte4
			&& (Uint)pin <= (Uint)Pin::Name::kPte12)
	{
		return (Uint)pin - (Uint)Pin::Name::kPte4 + 6;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPte24
			&& (Uint)pin <= (Uint)Pin::Name::kPte28)
	{
		return (Uint)pin - (Uint)Pin::Name::kPte24 + 44;
	}
	else
	{
		assert(false);
		return -1;
	}
}

int GetPinId(const Adc::Name pin)
{
	if ((Uint)pin >= (Uint)Adc::Name::kAdc1Ad4A
			&& (Uint)pin <= (Uint)Adc::Name::kAdc1Ad7A)
	{
		return (Uint)pin - (Uint)Adc::Name::kAdc1Ad4A;
	}
	else if (pin == Adc::Name::kAdc0DAd1)
	{
		return 22;
	}
	else if (pin == Adc::Name::kAdc1DAd1)
	{
		return 24;
	}
	else if (pin == Adc::Name::kAdc0DAd2 || pin == Adc::Name::kAdc0DAd0
			|| pin == Adc::Name::kAdc1DAd3)
	{
		return 26;
	}
	else if (pin == Adc::Name::kAdc1DAd2 || pin == Adc::Name::kAdc1DAd0
			|| pin == Adc::Name::kAdc0DAd3)
	{
		return 28;
	}
	else if (pin == Adc::Name::kAdc1Ad16 || pin == Adc::Name::kAdc0Ad22)
	{
		return 34;
	}
	else if (pin == Adc::Name::kAdc0Ad16 || pin == Adc::Name::kAdc0Ad21)
	{
		return 35;
	}
	else if (pin == Adc::Name::kAdc1Ad18)
	{
		return 36;
	}
	else if (pin == Adc::Name::kAdc0Ad23)
	{
		return 37;
	}
	else if (pin == Adc::Name::kAdc1Ad23)
	{
		return 38;
	}
	else if (pin == Adc::Name::kAdc0Ad17)
	{
		return 44;
	}
	else if (pin == Adc::Name::kAdc0Ad18)
	{
		return 45;
	}
	else if (pin == Adc::Name::kAdc0Ad10)
	{
		return 58;
	}
	else if (pin == Adc::Name::kAdc0Ad11)
	{
		return 59;
	}
	else if (pin == Adc::Name::kAdc1Ad17)
	{
		return 68;
	}
	else if (pin == Adc::Name::kAdc0Ad8 || pin == Adc::Name::kAdc1Ad8)
	{
		return 80;
	}
	else if (pin == Adc::Name::kAdc0Ad9 || pin == Adc::Name::kAdc1Ad9)
	{
		return 81;
	}
	else if (pin == Adc::Name::kAdc0Ad12)
	{
		return 82;
	}
	else if (pin == Adc::Name::kAdc0Ad13)
	{
		return 83;
	}
	else if ((Uint)pin >= (Uint)Adc::Name::kAdc1Ad10
			&& (Uint)pin <= (Uint)Adc::Name::kAdc1Ad13)
	{
		return (Uint)pin - (Uint)Adc::Name::kAdc1Ad10 + 84;
	}
	else if (pin == Adc::Name::kAdc1Ad14)
	{
		return 90;
	}
	else if (pin == Adc::Name::kAdc1Ad15)
	{
		return 91;
	}
	else if (pin == Adc::Name::kAdc0Ad14)
	{
		return 102;
	}
	else if (pin == Adc::Name::kAdc0Ad15)
	{
		return 103;
	}
	else if (pin == Adc::Name::kAdc0Ad4B)
	{
		return 104;
	}
	else if ((Uint)pin >= (Uint)Adc::Name::kAdc1Ad4B
			&& (Uint)pin <= (Uint)Adc::Name::kAdc1Ad7B)
	{
		return (Uint)pin - (Uint)Adc::Name::kAdc1Ad4B + 112;
	}
	else if (pin == Adc::Name::kAdc0Ad5B)
	{
		return 127;
	}
	else if (pin == Adc::Name::kAdc0Ad6B)
	{
		return 131;
	}
	else if (pin == Adc::Name::kAdc0Ad7B)
	{
		return 132;
	}
	else
	{
		assert(false);
		return -1;
	}
}

int GetPinId(const Dac::Name pin)
{
	switch (pin)
	{
	case Dac::Name::kDac0:
		return 38;

	case Dac::Name::kDac1:
		return 39;

	default:
		assert(false);
		return -1;
	}
}

}

Mk60f15Lqfp144 *Mk60f15Lqfp144::m_instance = nullptr;

Mk60f15Lqfp144::Mk60f15Lqfp144()
{}

bool Mk60f15Lqfp144::RegPin_(const Uint pin)
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

bool Mk60f15Lqfp144::RegPin_(const Pin::Name pin)
{
	return RegPin_((Uint)GetPinId(pin));
}

bool Mk60f15Lqfp144::RegPin_(const Adc::Name pin)
{
	return RegPin_((Uint)GetPinId(pin));
}

bool Mk60f15Lqfp144::RegPin_(const Dac::Name pin)
{
	return RegPin_((Uint)GetPinId(pin));
}

void Mk60f15Lqfp144::UnregPin_(const Uint pin)
{
	if (pin >= GetPinCount())
	{
		assert(false);
		return;
	}

	m_is_pins_active[pin] = false;
}

void Mk60f15Lqfp144::UnregPin_(const Pin::Name pin)
{
	UnregPin_((Uint)GetPinId(pin));
}

void Mk60f15Lqfp144::UnregPin_(const Adc::Name pin)
{
	UnregPin_((Uint)GetPinId(pin));
}

void Mk60f15Lqfp144::UnregPin_(const Dac::Name pin)
{
	UnregPin_((Uint)GetPinId(pin));
}

}
}
