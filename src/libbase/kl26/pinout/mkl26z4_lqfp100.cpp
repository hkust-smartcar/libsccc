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
		return (Uint)pin - (Uint)Pin::Name::kPta0 + 39;
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

}

}
}
