/*
 * reg_file.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>

#include <vector>

#include "libbase/misc_types.h"

namespace libbase
{
namespace kl26
{

class RegFile
{
public:
	RegFile();

	void WriteSystem(const Uint seek, const Byte *data, const size_t size);
	void WriteSystem(const Uint seek, const Byte data);
	Byte ReadSystem(const Uint seek);
	std::vector<Byte> ReadSystem(const Uint seek, const size_t size);
};

}
}
