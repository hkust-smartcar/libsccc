/*
 * reg_file.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstddef>
#include <cstring>

#include <algorithm>
#include <vector>

#include "libbase/k60/reg_file.h"
#include "libbase/k60/sim.h"
#include "libbase/misc_types.h"

using namespace std;

namespace libbase
{
namespace k60
{

RegFile::RegFile()
{
	Sim::SetEnableClockGate(Sim::ClockGate::kRegFile, true);
}

void RegFile::WriteVbat(const Uint seek, const Byte *data, const size_t size)
{
	if (seek >= sizeof(RFVBAT_Type))
	{
		assert(false);
		return;
	}
	void *to = ((Byte*)RFVBAT->REG) + seek;
	memcpy(to, data, std::min(size, sizeof(RFVBAT_Type) - seek));
}

void RegFile::WriteVbat(const Uint seek, const Byte data)
{
	if (seek >= sizeof(RFVBAT_Type))
	{
		assert(false);
		return;
	}
	((volatile Byte*)RFVBAT->REG)[seek] = data;
}

Byte RegFile::ReadVbat(const Uint seek)
{
	if (seek >= sizeof(RFVBAT_Type))
	{
		assert(false);
		return 0;
	}
	return ((volatile Byte*)RFVBAT->REG)[seek];
}

vector<Byte> RegFile::ReadVbat(const Uint seek, const size_t size)
{
	if (seek >= sizeof(RFVBAT_Type))
	{
		assert(false);
		return {};
	}
	vector<Byte> product(std::min(size, sizeof(RFVBAT_Type) - seek));
	void *from = ((Byte*)RFVBAT->REG) + seek;
	memcpy(product.data(), from, product.size());
	return product;
}

void RegFile::WriteSystem(const Uint seek, const Byte *data, const size_t size)
{
	if (seek >= sizeof(RFSYS_Type))
	{
		assert(false);
		return;
	}
	void *to = ((Byte*)RFSYS->REG) + seek;
	memcpy(to, data, std::min(size, sizeof(RFSYS_Type) - seek));
}

void RegFile::WriteSystem(const Uint seek, const Byte data)
{
	if (seek >= sizeof(RFSYS_Type))
	{
		assert(false);
		return;
	}
	((volatile Byte*)RFSYS->REG)[seek] = data;
}

Byte RegFile::ReadSystem(const Uint seek)
{
	if (seek >= sizeof(RFSYS_Type))
	{
		assert(false);
		return 0;
	}
	return ((volatile Byte*)RFSYS->REG)[seek];
}

vector<Byte> RegFile::ReadSystem(const Uint seek, const size_t size)
{
	if (seek >= sizeof(RFSYS_Type))
	{
		assert(false);
		return {};
	}
	vector<Byte> product(std::min(size, sizeof(RFSYS_Type) - seek));
	void *from = ((Byte*)RFSYS->REG) + seek;
	memcpy(product.data(), from, product.size());
	return product;
}

}
}
