/*
 * crc_utils.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstddef>
#include <cstdint>

#include "libbase/k60/crc.h"
#include "libbase/k60/crc_utils.h"
#include "libbase/k60/misc_utils.h"

namespace libbase
{
namespace k60
{

uint32_t CrcUtils::Calc(Crc *crc, const Byte *data, const size_t size)
{
	crc->NewInstance();
	const size_t size32 = size >> 2;
	for (size_t i = 0; i < size32; ++i)
	{
		uint32_t data32 = 0;
		data32 |= data[i * 4] << 24;
		data32 |= data[i * 4 + 1] << 16;
		data32 |= data[i * 4 + 2] << 8;
		data32 |= data[i * 4 + 3];
		crc->PushData(data32);
	}
	for (size_t i = size32 * 4; i < size; ++i)
	{
		crc->PushData(data[i]);
	}
	return crc->GetCrc();
}

}
}
