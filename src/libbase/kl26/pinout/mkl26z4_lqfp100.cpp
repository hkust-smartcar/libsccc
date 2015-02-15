#include <cassert>

#include <bitset>

#include "libbase/kl26/pinout/mkl26z4_lqfp100.h"

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
	else if ((Uint)pin >= (Uint)Pin::Name::kPta12 &&  (Uint)pin <= (Uint)Pin::Name::kPta17)
	{
		return (Uint)pin - (Uint)Pin::Name::kPta12 + 41;
	}
	else if ((Uint)pin >= (Uint)Pin::Name::kPta18 && (Uint)pin <= (Uint)Pin::Name::kPta20)
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

}

Mkl26z4Lqfp100::Mkl26z4Lqfp100(){
}

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

}
}
