/*
 * reg_file.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/kl26/hardware.h"

#include <cassert>
#include <cstddef>
#include <cstring>

#include <algorithm>
#include <vector>

#include "libbase/kl26/reg_file.h"
#include "libbase/kl26/sim.h"
#include "libbase/misc_types.h"

using namespace std;

typedef struct
{
	__IO uint32_t REG[8];                            /**< Register file register, array offset: 0x0, array step: 0x4 */
} RFSYS_Type;

/* REG Bit Fields */
#define RFSYS_REG_LL_MASK                        0xFFu
#define RFSYS_REG_LL_SHIFT                       0
#define RFSYS_REG_LL(x)                          (((uint32_t)(((uint32_t)(x))<<RFSYS_REG_LL_SHIFT))&RFSYS_REG_LL_MASK)
#define RFSYS_REG_LH_MASK                        0xFF00u
#define RFSYS_REG_LH_SHIFT                       8
#define RFSYS_REG_LH(x)                          (((uint32_t)(((uint32_t)(x))<<RFSYS_REG_LH_SHIFT))&RFSYS_REG_LH_MASK)
#define RFSYS_REG_HL_MASK                        0xFF0000u
#define RFSYS_REG_HL_SHIFT                       16
#define RFSYS_REG_HL(x)                          (((uint32_t)(((uint32_t)(x))<<RFSYS_REG_HL_SHIFT))&RFSYS_REG_HL_MASK)
#define RFSYS_REG_HH_MASK                        0xFF000000u
#define RFSYS_REG_HH_SHIFT                       24
#define RFSYS_REG_HH(x)                          (((uint32_t)(((uint32_t)(x))<<RFSYS_REG_HH_SHIFT))&RFSYS_REG_HH_MASK)

/* RFSYS - Peripheral instance base addresses */
#define RFSYS_BASE                               (0x40041000u)
#define RFSYS                                    ((RFSYS_Type *)RFSYS_BASE)

namespace libbase
{
namespace kl26
{

RegFile::RegFile()
{}

void RegFile::Init()
{}

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
