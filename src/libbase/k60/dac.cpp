/*
 * dac.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstddef>
#include <cstdint>

#include <algorithm>

#include "libbase/k60/dac.h"
#include "libbase/k60/dac_utils.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pinout.h"
#include "libbase/k60/sim.h"
#include "libutil/misc.h"

using namespace libutil;

namespace libbase
{
namespace k60
{

namespace
{

constexpr DAC_Type* MEM_MAPS[] = {DAC0, DAC1};

}

Dac::Dac(const Config &config)
		: m_name(Name::kDisable)
{
	if (config.module == Name::kDisable || !PINOUT::RegPin(config.module))
	{
		assert(false);
		return;
	}

	Sim::SetEnableClockGate(EnumAdvance(Sim::ClockGate::kDac0,
			DacUtils::GetModule(m_name)), true);
	InitC0Reg(config);
	SetData(config.data, config.data_size, config.buffer_mode);

	SET_BIT(MEM_MAPS[DacUtils::GetModule(m_name)]->C0, DAC_C0_DACEN_SHIFT);
}

Dac::Dac(Dac &&rhs)
		: Dac(nullptr)
{
	*this = std::move(rhs);
}

Dac::Dac(nullptr_t)
		: m_name(Name::kDisable)
{}

Dac::~Dac()
{
	Uninit();
}

Dac& Dac::operator=(Dac &&rhs)
{
	if (this != &rhs)
	{
		Uninit();
		if (rhs)
		{
			const Name name = rhs.m_name;
			rhs.m_name = Name::kDisable;

			m_name = name;
		}
	}
	return *this;
}

void Dac::InitC0Reg(const Config &config)
{
	uint32_t reg = 0;

	switch (config.src)
	{
	default:
	case Config::Source::kVdda:
		SET_BIT(reg, DAC_C0_DACRFS_SHIFT);
		break;
	}

	SET_BIT(reg, DAC_C0_DACTRGSEL_SHIFT);

	MEM_MAPS[DacUtils::GetModule(m_name)]->C0 = reg;
}

void Dac::Uninit()
{
	if (m_name != Name::kDisable)
	{
		m_name = Name::kDisable;

		SetData(0);
		Sim::SetEnableClockGate(EnumAdvance(Sim::ClockGate::kDac0,
				DacUtils::GetModule(m_name)), false);
		PINOUT::UnregPin(m_name);
	}
}

void Dac::SetData(const uint16_t *data, const Uint size,
		const Config::BufferMode buffer_mode)
{
	if (!*this)
	{
		return;
	}
	if (size == 0)
	{
		assert(false);
		return;
	}

	const Uint module = DacUtils::GetModule(m_name);

	assert(data[0] < 0x1000);
	MEM_MAPS[module]->DAT[0].DATL = DAC_DATL_DATA(data[0]);
	MEM_MAPS[module]->DAT[0].DATH = DAC_DATH_DATA(data[0] >> 8);

	if (size == 1)
	{
		CLEAR_BIT(MEM_MAPS[module]->C1, DAC_C1_DACBFEN_SHIFT);
		MEM_MAPS[module]->C2 = 0x0F;
	}
	else
	{
		// Disable buffer while copying data
		CLEAR_BIT(MEM_MAPS[module]->C1, DAC_C1_DACBFEN_SHIFT);
		for (Uint i = 1; i < size; ++i)
		{
			assert(data[i] < 0x1000);
			MEM_MAPS[module]->DAT[i].DATL = DAC_DATL_DATA(data[i]);
			MEM_MAPS[module]->DAT[i].DATH = DAC_DATH_DATA(data[i] >> 8);
		}

		// Set buffer work mode
		MEM_MAPS[module]->C1 &= ~DAC_C1_DACBFMD_MASK;
		switch (buffer_mode)
		{
		case Config::BufferMode::kNormal:
			break;

		case Config::BufferMode::kSwing:
			MEM_MAPS[module]->C1 |= DAC_C1_DACBFMD(0x1);
			break;

		case Config::BufferMode::kOneTimeScan:
			MEM_MAPS[module]->C1 |= DAC_C1_DACBFMD(0x2);
			break;
		}

		MEM_MAPS[module]->C2 = DAC_C2_DACBFUP(size - 1);
		SET_BIT(MEM_MAPS[module]->C1, DAC_C1_DACBFEN_SHIFT);
	}
}

void Dac::AdvanceData()
{
	if (!*this)
	{
		return;
	}
	SET_BIT(MEM_MAPS[DacUtils::GetModule(m_name)]->C0, DAC_C0_DACSWTRG_SHIFT);
}

void Dac::SetDataPosition(const Uint position)
{
	if (!*this)
	{
		return;
	}
	const Uint module = DacUtils::GetModule(m_name);
	assert(position <= (MEM_MAPS[module]->C2 & DAC_C2_DACBFUP_MASK));

	uint8_t reg = MEM_MAPS[module]->C2;

	reg &= ~DAC_C2_DACBFRP_MASK;
	const uint8_t limit = (reg & DAC_C2_DACBFUP_MASK) >> DAC_C2_DACBFUP_SHIFT;
	reg |= DAC_C2_DACBFRP(std::max<uint8_t>(position, limit));

	MEM_MAPS[module]->C2 = reg;
}

}
}
