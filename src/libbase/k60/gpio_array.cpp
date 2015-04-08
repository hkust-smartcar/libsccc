/*
 * gpio_array.cpp
 * Parallel GPIO
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

#include <vector>

#include "libbase/log.h"
#include "libbase/k60/gpio.h"
#include "libbase/k60/gpio_array.h"
#include "libbase/k60/pin_utils.h"

#include "libutil/endian_utils.h"
#include "libutil/misc.h"

using namespace libutil;
using namespace std;

namespace libbase
{
namespace k60
{

namespace
{

constexpr GPIO_Type* MEM_MAPS[PINOUT::GetPortCount()] = {PTA, PTB, PTC, PTD, PTE};

}

GpiArray::GpiArray(const Config &config)
{
	assert(PinUtils::GetPinNumber(config.start_pin) + config.count
			< PINOUT::GetPortPinCount());

	m_pins.reserve(config.count);
	Gpi::Config pin_config;
	pin_config.config = config.config;
	for (Uint i = 0; i < config.count; ++i)
	{
		pin_config.pin = EnumAdvance(config.start_pin, i);
		m_pins.push_back(Gpi(pin_config));
	}
}

GpiArray::GpiArray(GpiArray &&rhs)
		: GpiArray(nullptr)
{
	*this = std::move(rhs);
}

GpiArray::GpiArray(nullptr_t)
{}

GpiArray::~GpiArray()
{
	Uninit();
}

GpiArray& GpiArray::operator=(GpiArray &&rhs)
{
	if (this != &rhs)
	{
		Uninit();
		if (rhs)
		{
			m_pins = std::move(rhs.m_pins);
		}
	}
	return *this;
}

void GpiArray::Uninit()
{
	m_pins.clear();
}

void GpiArray::ConfigValueAsDmaSrc(Dma::Config *config)
{
	STATE_GUARD(GpiArray, VOID);

	const int start_pin = PinUtils::GetPinNumber(m_pins[0].GetPin()->GetName());
	const int last_pin = start_pin + m_pins.size() - 1;
	config->src.addr =
			(void*)&MEM_MAPS[PinUtils::GetPort(m_pins[0].GetPin()->GetName())]
					->PDIR;
	if (libutil::EndianUtils::IsBigEndian())
	{
		config->src.addr = (Byte*)config->src.addr + 3 - (last_pin / 8);
	}
	else
	{
		config->src.addr = (Byte*)config->src.addr + (start_pin / 8);
	}
	config->src.offset = 0;
	int size = (m_pins.size() + 7) / 8;
	if (start_pin % 8 + m_pins.size() % 8 > 8)
	{
		++size;
	}
	switch (size)
	{
	case 1:
	case 3:
		config->src.size = Dma::Config::TransferSize::k1Byte;
		break;

	case 2:
		config->src.size = Dma::Config::TransferSize::k2Byte;
		break;

	case 4:
		config->src.size = Dma::Config::TransferSize::k4Byte;
		break;
	}
	config->src.major_offset = 0;
	config->minor_bytes = size;
	config->major_count = 1;
}

vector<bool> GpiArray::Get() const
{
	vector<bool> product(m_pins.size(), false);
	for (Uint i = 0; i < m_pins.size(); ++i)
	{
		if (m_pins[i].Get())
		{
			product[i] = true;
		}
	}
	return product;
}

void GpiArray::Get(Byte *out_data, size_t size) const
{
	memset(out_data, 0, size);
	size_t pos = 0;
	Uint bit_pos = 0;
	for (size_t i = 0; i < m_pins.size() && pos < size; ++i)
	{
		if (m_pins[i].Get())
		{
			out_data[pos] |= 1 << (7 - bit_pos);
		}

		if (++bit_pos >= 8)
		{
			++pos;
			bit_pos = 0;
		}
	}
}

}
}
