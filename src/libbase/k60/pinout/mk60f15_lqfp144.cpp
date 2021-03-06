/*
 * mk60f15_lqfp144.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>

#include <bitset>

#include "libbase/k60/pinout/mk60f15_lqfp144.h"

#include "libbase/k60/adc.h"
#include "libbase/k60/dma_mux.h"
#include "libbase/k60/ftm.h"
#include "libbase/k60/i2c.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/pin_utils.h"
#include "libbase/k60/uart.h"
#include "libbase/misc_types.h"

using namespace std;

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
	else if (pin == Adc::Name::kAdc2Ad16)
	{
		return 10;
	}
	else if (pin == Adc::Name::kAdc2Ad17)
	{
		return 11;
	}
	else if (pin == Adc::Name::kAdc3Ad16)
	{
		return 13;
	}
	else if (pin == Adc::Name::kAdc3Ad17)
	{
		return 14;
	}
	else if (pin == Adc::Name::kAdc2DAd2|| pin == Adc::Name::kAdc2DAd0
			|| pin == Adc::Name::kAdc3DAd3 || pin == Adc::Name::kAdc0DAd1)
	{
		return 22;
	}
	else if (pin == Adc::Name::kAdc3DAd2 || pin == Adc::Name::kAdc3DAd0
			|| pin == Adc::Name::kAdc2DAd3 || pin == Adc::Name::kAdc1DAd1)
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
	else if (pin == Adc::Name::kAdc3Ad5B)
	{
		return 46;
	}
	else if (pin == Adc::Name::kAdc3Ad4B)
	{
		return 47;
	}
	else if (pin == Adc::Name::kAdc3Ad7A)
	{
		return 48;
	}
	else if (pin == Adc::Name::kAdc3Ad6A)
	{
		return 57;
	}
	else if (pin == Adc::Name::kAdc0Ad10)
	{
		return 58;
	}
	else if (pin == Adc::Name::kAdc0Ad11)
	{
		return 59;
	}
	else if (pin == Adc::Name::kAdc3Ad5A)
	{
		return 60;
	}
	else if (pin == Adc::Name::kAdc3Ad4A)
	{
		return 61;
	}
	else if (pin == Adc::Name::kAdc3Ad15)
	{
		return 62;
	}
	else if (pin == Adc::Name::kAdc1Ad17)
	{
		return 68;
	}
	else if (pin == Adc::Name::kAdc2Ad15)
	{
		return 76;
	}
	else if (pin == Adc::Name::kAdc2Ad14)
	{
		return 77;
	}
	else if (pin == Adc::Name::kAdc2Ad13)
	{
		return 78;
	}
	else if (pin == Adc::Name::kAdc2Ad12)
	{
		return 79;
	}
	else if (pin == Adc::Name::kAdc0Ad8 || pin == Adc::Name::kAdc1Ad8
			|| pin == Adc::Name::kAdc2Ad8 || pin == Adc::Name::kAdc3Ad8)
	{
		return 80;
	}
	else if (pin == Adc::Name::kAdc0Ad9 || pin == Adc::Name::kAdc1Ad9
			|| pin == Adc::Name::kAdc2Ad9 || pin == Adc::Name::kAdc3Ad9)
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
	else if (pin == Adc::Name::kAdc2Ad4A)
	{
		return 98;
	}
	else if (pin == Adc::Name::kAdc2Ad5A)
	{
		return 99;
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
		return 37;

	case Dac::Name::kDac1:
		return 38;

	default:
		assert(false);
		return -1;
	}
}

}

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

Adc::Name Mk60f15Lqfp144::GetAdc(const Pin::Name pin)
{
	switch (pin)
	{
	case Pin::Name::kPta6:
		return Adc::Name::kAdc3Ad6A;

	case Pin::Name::kPta7:
		return Adc::Name::kAdc0Ad10;

	case Pin::Name::kPta8:
		return Adc::Name::kAdc0Ad11;

	case Pin::Name::kPta9:
		return Adc::Name::kAdc3Ad5A;

	case Pin::Name::kPta10:
		return Adc::Name::kAdc3Ad4A;

	case Pin::Name::kPta11:
		return Adc::Name::kAdc3Ad15;

	case Pin::Name::kPta17:
		return Adc::Name::kAdc1Ad17;

	case Pin::Name::kPta26:
		return Adc::Name::kAdc2Ad15;

	case Pin::Name::kPta27:
		return Adc::Name::kAdc2Ad14;

	case Pin::Name::kPta28:
		return Adc::Name::kAdc2Ad13;

	case Pin::Name::kPta29:
		return Adc::Name::kAdc2Ad12;

	case Pin::Name::kPtb0:
		return Adc::Name::kAdc0Ad8;

	case Pin::Name::kPtb1:
		return Adc::Name::kAdc0Ad9;

	case Pin::Name::kPtb2:
		return Adc::Name::kAdc0Ad12;

	case Pin::Name::kPtb3:
		return Adc::Name::kAdc0Ad13;

	case Pin::Name::kPtb4:
		return Adc::Name::kAdc1Ad10;

	case Pin::Name::kPtb5:
		return Adc::Name::kAdc1Ad11;

	case Pin::Name::kPtb6:
		return Adc::Name::kAdc1Ad12;

	case Pin::Name::kPtb7:
		return Adc::Name::kAdc1Ad13;

	case Pin::Name::kPtb10:
		return Adc::Name::kAdc1Ad14;

	case Pin::Name::kPtb11:
		return Adc::Name::kAdc1Ad15;

	case Pin::Name::kPtb20:
		return Adc::Name::kAdc2Ad4A;

	case Pin::Name::kPtb21:
		return Adc::Name::kAdc2Ad5A;

	case Pin::Name::kPtc0:
		return Adc::Name::kAdc0Ad14;

	case Pin::Name::kPtc1:
		return Adc::Name::kAdc0Ad15;

	case Pin::Name::kPtc2:
		return Adc::Name::kAdc0Ad4B;

	case Pin::Name::kPtc8:
		return Adc::Name::kAdc1Ad4B;

	case Pin::Name::kPtc9:
		return Adc::Name::kAdc1Ad5B;

	case Pin::Name::kPtc10:
		return Adc::Name::kAdc1Ad6B;

	case Pin::Name::kPtc11:
		return Adc::Name::kAdc1Ad7B;

	case Pin::Name::kPtd1:
		return Adc::Name::kAdc0Ad5B;

	case Pin::Name::kPtd5:
		return Adc::Name::kAdc0Ad6B;

	case Pin::Name::kPtd6:
		return Adc::Name::kAdc0Ad7B;

	case Pin::Name::kPte0:
		return Adc::Name::kAdc1Ad4A;

	case Pin::Name::kPte1:
		return Adc::Name::kAdc1Ad5A;

	case Pin::Name::kPte2:
		return Adc::Name::kAdc1Ad6A;

	case Pin::Name::kPte3:
		return Adc::Name::kAdc1Ad7A;

	case Pin::Name::kPte8:
		return Adc::Name::kAdc2Ad16;

	case Pin::Name::kPte9:
		return Adc::Name::kAdc2Ad17;

	case Pin::Name::kPte11:
		return Adc::Name::kAdc3Ad16;

	case Pin::Name::kPte12:
		return Adc::Name::kAdc3Ad17;

	case Pin::Name::kPte24:
		return Adc::Name::kAdc0Ad17;

	case Pin::Name::kPte25:
		return Adc::Name::kAdc0Ad18;

	case Pin::Name::kPte26:
		return Adc::Name::kAdc3Ad5B;

	case Pin::Name::kPte27:
		return Adc::Name::kAdc3Ad4B;

	case Pin::Name::kPte28:
		return Adc::Name::kAdc3Ad7A;

	default:
		return Adc::Name::kDisable;
	}
}

Ftm::Name Mk60f15Lqfp144::GetFtm(const Pin::Name pin)
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

	case Pin::Name::kPte5:
	case Pin::Name::kPtd0:
		return Ftm::Name::kFtm3Ch0;

	case Pin::Name::kPte6:
	case Pin::Name::kPtd1:
		return Ftm::Name::kFtm3Ch1;

	case Pin::Name::kPte7:
	case Pin::Name::kPtd2:
		return Ftm::Name::kFtm3Ch2;

	case Pin::Name::kPte8:
	case Pin::Name::kPtd3:
		return Ftm::Name::kFtm3Ch3;

	case Pin::Name::kPte9:
	case Pin::Name::kPtc8:
		return Ftm::Name::kFtm3Ch4;

	case Pin::Name::kPte10:
	case Pin::Name::kPtc9:
		return Ftm::Name::kFtm3Ch5;

	case Pin::Name::kPte11:
	case Pin::Name::kPtc10:
		return Ftm::Name::kFtm3Ch6;

	case Pin::Name::kPte12:
	case Pin::Name::kPtc11:
		return Ftm::Name::kFtm3Ch7;

	default:
		return Ftm::Name::kDisable;
	}
}

Pin::Config::MuxControl Mk60f15Lqfp144::GetFtmMux(const Pin::Name pin)
{
	const Uint port = PinUtils::GetPort(pin);
	const Uint pin_num = PinUtils::GetPinNumber(pin);
	switch (port)
	{
	case 0:
	case 1:
		return Pin::Config::MuxControl::kAlt3;

	case 2:
		if (pin_num <= 4)
		{
			return Pin::Config::MuxControl::kAlt4;
		}
		else
		{
			return Pin::Config::MuxControl::kAlt3;
		}

	case 3:
		return Pin::Config::MuxControl::kAlt4;

	case 4:
		return Pin::Config::MuxControl::kAlt6;

	default:
		return Pin::Config::MuxControl::kGpio;
	}
}

Ftm::QdName Mk60f15Lqfp144::GetFtmQd(const Pin::Name pin)
{
	switch (pin)
	{
	case Pin::Name::kPta8:
	case Pin::Name::kPta12:
	case Pin::Name::kPtb0:
		return Ftm::QdName::kFtm1QdPha;

	case Pin::Name::kPta9:
	case Pin::Name::kPta13:
	case Pin::Name::kPtb1:
		return Ftm::QdName::kFtm1QdPhb;

	case Pin::Name::kPta10:
	case Pin::Name::kPtb18:
		return Ftm::QdName::kFtm2QdPha;

	case Pin::Name::kPta11:
	case Pin::Name::kPtb19:
		return Ftm::QdName::kFtm2QdPhb;

	default:
		return Ftm::QdName::kDisable;
	}
}

Pin::Config::MuxControl Mk60f15Lqfp144::GetFtmQdMux(const Pin::Name pin)
{
	if (pin == Pin::Name::kPta12 || pin == Pin::Name::kPta13)
	{
		return Pin::Config::MuxControl::kAlt7;
	}
	else
	{
		return Pin::Config::MuxControl::kAlt6;
	}
}

I2c::Name Mk60f15Lqfp144::GetI2c(const Pin::Name pin)
{
	switch (pin)
	{
	case Pin::Name::kPtb0:
	case Pin::Name::kPtb2:
	case Pin::Name::kPtd8:
		return I2c::Name::kI2c0Scl;

	case Pin::Name::kPtb1:
	case Pin::Name::kPtb3:
	case Pin::Name::kPtd9:
		return I2c::Name::kI2c0Sda;

	case Pin::Name::kPte1:
	case Pin::Name::kPtc10:
		return I2c::Name::kI2c1Scl;

	case Pin::Name::kPte0:
	case Pin::Name::kPtc11:
		return I2c::Name::kI2c1Sda;

	default:
		return I2c::Name::kDisable;
	}
}

Pin::Config::MuxControl Mk60f15Lqfp144::GetI2cMux(const Pin::Name pin)
{
	if (pin == Pin::Name::kPte0 || pin == Pin::Name::kPte1)
	{
		return Pin::Config::MuxControl::kAlt6;
	}
	else
	{
		return Pin::Config::MuxControl::kAlt2;
	}
}

Spi::SinName Mk60f15Lqfp144::GetSpiSin(const Pin::Name pin)
{
	switch (pin)
	{
	case Pin::Name::kPta17:
	case Pin::Name::kPtc7:
	case Pin::Name::kPtd3:
		return Spi::SinName::kSpi0Sin;

	case Pin::Name::kPtb17:
	case Pin::Name::kPte1:
	case Pin::Name::kPte3:
		return Spi::SinName::kSpi1Sin;

	case Pin::Name::kPtb23:
	case Pin::Name::kPtd14:
		return Spi::SinName::kSpi2Sin;

	default:
		return Spi::SinName::kDisable;
	}
}

Pin::Config::MuxControl Mk60f15Lqfp144::GetSpiSinMux(const Pin::Name pin)
{
	switch (pin)
	{
	default:
		assert(false);
		// no break

	case Pin::Name::kPta17:
	case Pin::Name::kPtb17:
	case Pin::Name::kPtb23:
	case Pin::Name::kPtc7:
	case Pin::Name::kPtd3:
	case Pin::Name::kPtd14:
	case Pin::Name::kPte3:
		return Pin::Config::MuxControl::kAlt2;

	case Pin::Name::kPte1:
		return Pin::Config::MuxControl::kAlt7;
	}
}

Spi::SoutName Mk60f15Lqfp144::GetSpiSout(const Pin::Name pin)
{
	switch (pin)
	{
	case Pin::Name::kPta16:
	case Pin::Name::kPtc6:
	case Pin::Name::kPtd2:
		return Spi::SoutName::kSpi0Sout;

	case Pin::Name::kPtb16:
	case Pin::Name::kPte1:
	case Pin::Name::kPte3:
		return Spi::SoutName::kSpi1Sout;

	case Pin::Name::kPtb22:
	case Pin::Name::kPtd13:
		return Spi::SoutName::kSpi2Sout;

	default:
		return Spi::SoutName::kDisable;
	}
}

Pin::Config::MuxControl Mk60f15Lqfp144::GetSpiSoutMux(const Pin::Name pin)
{
	switch (pin)
	{
	default:
		assert(false);
		// no break

	case Pin::Name::kPta16:
	case Pin::Name::kPtb16:
	case Pin::Name::kPtb22:
	case Pin::Name::kPtc6:
	case Pin::Name::kPtd2:
	case Pin::Name::kPtd13:
	case Pin::Name::kPte1:
		return Pin::Config::MuxControl::kAlt2;

	case Pin::Name::kPte3:
		return Pin::Config::MuxControl::kAlt7;
	}
}

Spi::SckName Mk60f15Lqfp144::GetSpiSck(const Pin::Name pin)
{
	switch (pin)
	{
	case Pin::Name::kPta15:
	case Pin::Name::kPtc5:
	case Pin::Name::kPtd1:
		return Spi::SckName::kSpi0Sck;

	case Pin::Name::kPtb11:
	case Pin::Name::kPte2:
		return Spi::SckName::kSpi1Sck;

	case Pin::Name::kPtb21:
	case Pin::Name::kPtd12:
		return Spi::SckName::kSpi2Sck;

	default:
		return Spi::SckName::kDisable;
	}
}

Pin::Config::MuxControl Mk60f15Lqfp144::GetSpiSckMux(const Pin::Name)
{
	return Pin::Config::MuxControl::kAlt2;
}

Spi::PcsName Mk60f15Lqfp144::GetSpiPcs(const Pin::Name pin)
{
	switch (pin)
	{
	case Pin::Name::kPta14:
	case Pin::Name::kPtc4:
	case Pin::Name::kPtd0:
		return Spi::PcsName::kSpi0Pcs0;

	case Pin::Name::kPtc3:
	case Pin::Name::kPtd4:
		return Spi::PcsName::kSpi0Pcs1;

	case Pin::Name::kPtc2:
	case Pin::Name::kPtd5:
		return Spi::PcsName::kSpi0Pcs2;

	case Pin::Name::kPtc1:
	case Pin::Name::kPtd6:
		return Spi::PcsName::kSpi0Pcs3;

	case Pin::Name::kPtc0:
		return Spi::PcsName::kSpi0Pcs4;

	case Pin::Name::kPtb23:
		return Spi::PcsName::kSpi0Pcs5;

	case Pin::Name::kPtb10:
	case Pin::Name::kPte4:
		return Spi::PcsName::kSpi1Pcs0;

	case Pin::Name::kPtb9:
	case Pin::Name::kPte0:
		return Spi::PcsName::kSpi1Pcs1;

	case Pin::Name::kPte5:
		return Spi::PcsName::kSpi1Pcs2;

	case Pin::Name::kPte6:
		return Spi::PcsName::kSpi1Pcs3;

	case Pin::Name::kPtb20:
	case Pin::Name::kPtd11:
		return Spi::PcsName::kSpi2Pcs0;

	case Pin::Name::kPtd15:
		return Spi::PcsName::kSpi2Pcs1;

	default:
		return Spi::PcsName::kDisable;
	}
}

Pin::Config::MuxControl Mk60f15Lqfp144::GetSpiPcsMux(const Pin::Name pin)
{
	switch (pin)
	{
	default:
		assert(false);
		// no break

	case Pin::Name::kPta14:
	case Pin::Name::kPtb9:
	case Pin::Name::kPtb10:
	case Pin::Name::kPtb20:
	case Pin::Name::kPtc0:
	case Pin::Name::kPtc1:
	case Pin::Name::kPtc2:
	case Pin::Name::kPtc3:
	case Pin::Name::kPtc4:
	case Pin::Name::kPtd0:
	case Pin::Name::kPtd4:
	case Pin::Name::kPtd5:
	case Pin::Name::kPtd6:
	case Pin::Name::kPtd11:
	case Pin::Name::kPtd15:
	case Pin::Name::kPte0:
	case Pin::Name::kPte4:
	case Pin::Name::kPte5:
	case Pin::Name::kPte6:
		return Pin::Config::MuxControl::kAlt2;

	case Pin::Name::kPtb23:
		return Pin::Config::MuxControl::kAlt3;
	}
}

uint8_t Mk60f15Lqfp144::GetDmaMuxSource(const DmaMux::Source src, const Uint mux)
{
	assert(mux < PINOUT_DMA_MUX_COUNT);

	if ((Uint)src >= (Uint)DmaMux::Source::kUart0Rx
			&& (Uint)src <= (Uint)DmaMux::Source::kUart5Tx)
	{
		return (Uint)src - (Uint)DmaMux::Source::kUart0Rx + 2;
	}
	else if ((Uint)src >= (Uint)DmaMux::Source::kSpi0Rx
			&& (Uint)src <= (Uint)DmaMux::Source::kSpi2Tx)
	{
		return (Uint)src - (Uint)DmaMux::Source::kSpi0Rx + 16;
	}
	else if (src == DmaMux::Source::kI2c0)
	{
		return (mux == 0) ? 22 : static_cast<uint8_t>(-1);
	}
	else if (src == DmaMux::Source::kI2c1or2)
	{
		return (mux == 0) ? 23 : static_cast<uint8_t>(-1);
	}
	else if ((Uint)src >= (Uint)DmaMux::Source::kFtm0Ch0
			&& (Uint)src <= (Uint)DmaMux::Source::kFtm2Ch1)
	{
		return (mux == 0) ? ((Uint)src - (Uint)DmaMux::Source::kFtm0Ch0 + 24)
				: static_cast<uint8_t>(-1);
	}
	else if ((Uint)src >= (Uint)DmaMux::Source::kFtm3Ch0
			&& (Uint)src <= (Uint)DmaMux::Source::kFtm3Ch7)
	{
		return (mux == 0) ? static_cast<uint8_t>(-1)
				: ((Uint)src - (Uint)DmaMux::Source::kFtm3Ch0 + 24);
	}
	else if (src == DmaMux::Source::kAdc0)
	{
		return 40;
	}
	else if (src == DmaMux::Source::kAdc1)
	{
		return 41;
	}
	else if (src == DmaMux::Source::kAdc2)
	{
		return (mux == 0) ? static_cast<uint8_t>(-1) : 42;
	}
	else if (src == DmaMux::Source::kAdc3)
	{
		return (mux == 0) ? static_cast<uint8_t>(-1) : 43;
	}
	else if (src == DmaMux::Source::kDac0)
	{
		return 45;
	}
	else if (src == DmaMux::Source::kDac1)
	{
		return 46;
	}
	else if ((Uint)src >= (Uint)DmaMux::Source::kPortA
			&& (Uint)src <= (Uint)DmaMux::Source::kPortE)
	{
		return (mux == 0) ? ((Uint)src - (Uint)DmaMux::Source::kPortA + 49)
				: static_cast<uint8_t>(-1);
	}
	else if (src == DmaMux::Source::kPortF)
	{
		return (mux == 0) ? static_cast<uint8_t>(-1) : 53;
	}
	else if ((Uint)src >= (Uint)DmaMux::Source::kSoftware0
			&& (Uint)src <= (Uint)DmaMux::Source::kSoftware9)
	{
		return (Uint)src - (Uint)DmaMux::Source::kSoftware0 + 54;
	}
	else
	{
		return static_cast<uint8_t>(-1);
	}
}

Uart::Name Mk60f15Lqfp144::GetUart(const Pin::Name pin)
{
	switch (pin)
	{
	case Pin::Name::kPta1:
	case Pin::Name::kPta15:
	case Pin::Name::kPtb16:
	case Pin::Name::kPtd6:
		return Uart::Name::kUart0Rx;

	case Pin::Name::kPta2:
	case Pin::Name::kPta14:
	case Pin::Name::kPtb17:
	case Pin::Name::kPtd7:
		return Uart::Name::kUart0Tx;

	case Pin::Name::kPtc3:
	case Pin::Name::kPte1:
		return Uart::Name::kUart1Rx;

	case Pin::Name::kPtc4:
	case Pin::Name::kPte0:
		return Uart::Name::kUart1Tx;

	case Pin::Name::kPtd2:
		return Uart::Name::kUart2Rx;

	case Pin::Name::kPtd3:
		return Uart::Name::kUart2Tx;

	case Pin::Name::kPtb10:
	case Pin::Name::kPtc16:
	case Pin::Name::kPte5:
		return Uart::Name::kUart3Rx;

	case Pin::Name::kPtb11:
	case Pin::Name::kPtc17:
	case Pin::Name::kPte4:
		return Uart::Name::kUart3Tx;

	case Pin::Name::kPtc14:
	case Pin::Name::kPte25:
		return Uart::Name::kUart4Rx;

	case Pin::Name::kPtc15:
	case Pin::Name::kPte24:
		return Uart::Name::kUart4Tx;

	case Pin::Name::kPtd8:
	case Pin::Name::kPte9:
		return Uart::Name::kUart5Rx;

	case Pin::Name::kPtd9:
	case Pin::Name::kPte8:
		return Uart::Name::kUart5Tx;

	default:
		return Uart::Name::kDisable;
	}
}

Pin::Config::MuxControl Mk60f15Lqfp144::GetUartMux(const Pin::Name pin)
{
	if (pin == Pin::Name::kPta1 || pin == Pin::Name::kPta2)
	{
		return Pin::Config::MuxControl::kAlt2;
	}
	else
	{
		return Pin::Config::MuxControl::kAlt3;
	}
}

}
}
