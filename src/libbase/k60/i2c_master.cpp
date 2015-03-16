/*
 * i2c_master.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/k60/hardware.h"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cmath>

#include <vector>

#include "libbase/log.h"
#include "libbase/k60/clock_utils.h"
#include "libbase/k60/i2c.h"
#include "libbase/k60/i2c_master.h"
#include "libbase/k60/i2c_master_interface.h"
#include "libbase/k60/i2c_utils.h"
#include "libbase/k60/pinout.h"
#include "libbase/k60/sim.h"

#include "libutil/misc.h"

using namespace std;
using namespace libutil;

#if MK60DZ10 || MK60D10
	#define CLOCKGATE0 Sim::ClockGate::kI2c0
#elif MK60F15
	#define CLOCKGATE0 Sim::ClockGate::kIic0
#endif

#define SEND_BYTE_GUARDED(x, ret) do { if (!SendByte_(x)) { Stop(); return ret; } } while (false)

namespace libbase
{
namespace k60
{

namespace
{

constexpr I2C_Type* MEM_MAPS[PINOUT::GetI2cCount()] = {I2C0, I2C1};

I2cMaster* g_instances[PINOUT::GetI2cCount()] = {};


class BaudRateCalculator
{
public:
	void Calc(const I2cMaster::Config &config);

	uint8_t GetBestMult() const
	{
		return m_best_mult;
	}

	uint8_t GetBestIcr() const
	{
		return m_best_icr;
	}

private:
	static const uint16_t kIcrTable[0x40][4];

	Uint m_best_mult;
	Uint m_best_icr;
};

// Find a better way to do this
const uint16_t BaudRateCalculator::kIcrTable[0x40][4] =
{
	{20, 7, 6, 11},
	{22, 7, 7, 12},
	{24, 8, 8, 13},
	{26, 8, 9, 14},
	{28, 9, 10, 15},
	{30, 9, 11, 16},
	{34, 10, 13, 18},
	{40, 10, 16, 21},
	{28, 7, 10, 15},
	{32, 7, 12, 17},
	{36, 9, 14, 19},
	{40, 9, 16, 21},
	{44, 11, 18, 23},
	{48, 11, 20, 25},
	{56, 13, 24, 29},
	{68, 13, 30, 35},
	{48, 9, 18, 25},
	{56, 9, 22, 29},
	{64, 13, 26, 33},
	{72, 13, 30, 37},
	{80, 17, 34, 41},
	{88, 17, 38, 45},
	{104, 21, 46, 53},
	{128, 21, 58, 65},
	{80, 9, 38, 41},
	{96, 9, 46, 49},
	{112, 17, 54, 57},
	{128, 17, 62, 65},
	{144, 25, 70, 73},
	{160, 25, 78, 81},
	{192, 33, 94, 97},
	{240, 33, 118, 121},
	{160, 17, 78, 81},
	{192, 17, 94, 97},
	{224, 33, 110, 113},
	{256, 33, 126, 129},
	{288, 49, 142, 145},
	{320, 49, 158, 161},
	{384, 65, 190, 193},
	{480, 65, 238, 241},
	{320, 33, 158, 161},
	{384, 33, 190, 193},
	{448, 65, 222, 225},
	{512, 65, 254, 257},
	{576, 97, 286, 289},
	{640, 97, 318, 321},
	{768, 129, 382, 385},
	{960, 129, 478, 481},
	{640, 65, 318, 321},
	{768, 65, 382, 385},
	{896, 129, 446, 449},
	{1024, 129, 510, 513},
	{1152, 193, 574, 577},
	{1280, 193, 638, 641},
	{1536, 257, 766, 769},
	{1920, 257, 958, 961},
	{1280, 129, 638, 641},
	{1536, 129, 766, 769},
	{1792, 257, 894, 897},
	{2048, 257, 1022, 1025},
	{2304, 385, 1150, 1153},
	{2560, 385, 1278, 1281},
	{3072, 513, 1534, 1537},
	{3840, 513, 1918, 1921},
};

void BaudRateCalculator::Calc(const I2cMaster::Config &config)
{
	m_best_mult = m_best_icr = 0;
	Uint min_baud_rate_diff = static_cast<Uint>(-1);
	for (int i = 0; i < 3; ++i)
	{
		const Uint this_mult = pow(2, i);
		for (int j = 0; j < 0x40; ++j)
		{
			const uint32_t this_baud_rate_khz = ClockUtils::GetBusClockKhz()
					/ (this_mult * kIcrTable[j][0]);
			const Uint this_baud_rate_diff = abs((int32_t)(this_baud_rate_khz
					- config.baud_rate_khz));
			if (this_baud_rate_diff < min_baud_rate_diff)
			{
				const float clock_period_ns = 1000.0f
						/ ClockUtils::GetBusClockMhz();
				const float factor = clock_period_ns * this_mult;

				const uint32_t this_sda_hold_ns = factor * kIcrTable[j][1];
				const uint32_t this_scl_start_ns = factor * kIcrTable[j][2];
				const uint32_t this_scl_stop_ns = factor * kIcrTable[j][3];
				if (config.min_sda_hold_time_ns
						&& this_sda_hold_ns < config.min_sda_hold_time_ns)
				{
					// Failed SDA hold time requirement
					continue;
				}
				if (config.min_scl_start_hold_time_ns
						&& this_scl_start_ns < config.min_scl_start_hold_time_ns)
				{
					// Failed SCL start hold time requirement
					continue;
				}
				if (config.min_scl_stop_hold_time_ns
						&& this_scl_stop_ns < config.min_scl_stop_hold_time_ns)
				{
					// Failed SCL stop hold time requirement
					continue;
				}

				// All requirements fulfilled
				min_baud_rate_diff = this_baud_rate_diff;
				m_best_mult = i;
				m_best_icr = j;
			}
		}
	}
}

}

I2cMaster::I2cMaster(const Config &config)
		: m_is_use_repeated_start(config.is_use_repeated_start),
		  m_is_init(false)
{
	if (!InitModule(config.scl_pin, config.sda_pin))
	{
		assert(false);
		return;
	}

	m_is_init = true;

	Sim::SetEnableClockGate(EnumAdvance(CLOCKGATE0, m_module), true);

	// Disable
	MEM_MAPS[m_module]->C1 = 0;

	g_instances[m_module] = this;
	InitPin(config.scl_pin, config.sda_pin);
	InitBaudRate(config);
	InitC2Reg(config);
	InitSltReg(config);

	// Enable
	SET_BIT(MEM_MAPS[m_module]->C1, I2C_C1_IICEN_SHIFT);
}

I2cMaster::I2cMaster(I2cMaster &&rhs)
		: I2cMaster(nullptr)
{
	*this = std::move(rhs);
}

I2cMaster::I2cMaster(nullptr_t)
		: m_module(0),
		  m_scl(nullptr),
		  m_sda(nullptr),
		  m_is_use_repeated_start(false),
		  m_is_init(false)
{}

I2cMaster::~I2cMaster()
{
	Uninit();
}

I2cMaster& I2cMaster::operator=(I2cMaster &&rhs)
{
	if (this != &rhs)
	{
		Uninit();
		if (rhs)
		{
			rhs.m_is_init = false;

			m_is_use_repeated_start = rhs.m_is_use_repeated_start;

			m_scl = std::move(rhs.m_scl);
			m_sda = std::move(rhs.m_sda);

			m_is_init = true;
		}
	}
	return *this;
}

bool I2cMaster::InitModule(const Pin::Name scl_pin, const Pin::Name sda_pin)
{
	const I2c::Name scl_i2c = PINOUT::GetI2c(scl_pin);
	const int scl_module = I2cUtils::GetI2cModule(scl_i2c);

	const I2c::Name sda_i2c = PINOUT::GetI2c(sda_pin);
	const int sda_module = I2cUtils::GetI2cModule(sda_i2c);

	if (scl_module == sda_module && scl_i2c != I2c::Name::kDisable)
	{
		m_module = scl_module;
		return true;
	}
	else
	{
		return false;
	}
}

void I2cMaster::InitPin(const Pin::Name scl_pin, const Pin::Name sda_pin)
{
	Pin::Config scl_config, sda_config;
	scl_config.pin = scl_pin;
	scl_config.mux = PINOUT::GetI2cMux(scl_pin);
	sda_config.pin = sda_pin;
	sda_config.mux = PINOUT::GetI2cMux(sda_pin);

	m_scl = Pin(scl_config);
	m_sda = Pin(sda_config);
}

void I2cMaster::InitC2Reg(const Config &config)
{
	uint8_t reg = 0;
	if (config.is_high_drive_mode)
	{
		SET_BIT(reg, I2C_C2_HDRS_SHIFT);
	}

	MEM_MAPS[m_module]->C2 = reg;
}

void I2cMaster::InitSmbReg()
{
	uint8_t reg = 0;
	// Use bus clock as the ref of timeout counter
	SET_BIT(reg, I2C_SMB_TCKSEL_SHIFT);

	MEM_MAPS[m_module]->SMB = reg;
}

void I2cMaster::InitSltReg(const Config &config)
{
	MEM_MAPS[m_module]->SLTH = I2C_SLTH_SSLT(config.scl_low_timeout >> 8);
	MEM_MAPS[m_module]->SLTL = I2C_SLTL_SSLT(config.scl_low_timeout);
}

void I2cMaster::InitBaudRate(const Config &config)
{
	BaudRateCalculator calc;
	calc.Calc(config);

	uint8_t reg = 0;
	reg |= I2C_F_MULT(calc.GetBestMult());
	reg |= I2C_F_ICR(calc.GetBestIcr());

	MEM_MAPS[m_module]->F = reg;
}

void I2cMaster::Uninit()
{
	if (m_is_init)
	{
		m_is_init = false;

		// Disable
		CLEAR_BIT(MEM_MAPS[m_module]->C1, I2C_C1_IICEN_SHIFT);

		Sim::SetEnableClockGate(EnumAdvance(CLOCKGATE0, m_module), false);
		g_instances[m_module] = nullptr;
	}
}

void I2cMaster::Start()
{
	if (GET_BIT(MEM_MAPS[m_module]->C1, I2C_C1_MST_SHIFT))
	{
		// Already started, generate a restart
		Stop();
	}
	SET_BIT(MEM_MAPS[m_module]->C1, I2C_C1_TX_SHIFT);
	SET_BIT(MEM_MAPS[m_module]->C1, I2C_C1_MST_SHIFT);
	// Wait until started
	while (!GET_BIT(MEM_MAPS[m_module]->S, I2C_S_BUSY_SHIFT))
	{}
}

void I2cMaster::RepeatStart()
{
	if (m_is_use_repeated_start)
	{
		SET_BIT(MEM_MAPS[m_module]->C1, I2C_C1_RSTA_SHIFT);
	}
	else
	{
		// User don't want a repeat start signal, do a Stop/Start cycle instead
		Stop();
		Start();
	}
}

void I2cMaster::Stop()
{
	if (!GET_BIT(MEM_MAPS[m_module]->C1, I2C_C1_MST_SHIFT))
	{
		// Already stopped, do nothing
		return;
	}
	CLEAR_BIT(MEM_MAPS[m_module]->C1, I2C_C1_MST_SHIFT);
	CLEAR_BIT(MEM_MAPS[m_module]->C1, I2C_C1_TX_SHIFT);
	// Wait until stopped
	while (GET_BIT(MEM_MAPS[m_module]->S, I2C_S_BUSY_SHIFT))
	{}
}

bool I2cMaster::SendByte_(const Byte byte)
{
	SET_BIT(MEM_MAPS[m_module]->S, I2C_S_IICIF_SHIFT);
	MEM_MAPS[m_module]->D = byte;
	// Wait until data is sent
	while (!GET_BIT(MEM_MAPS[m_module]->S, I2C_S_IICIF_SHIFT))
	{
		if (GET_BIT(MEM_MAPS[m_module]->SMB, I2C_SMB_SLTF_SHIFT))
		{
			LOG_DL("i2c scl timeout");
			return false;
		}
	}
	SET_BIT(MEM_MAPS[m_module]->S, I2C_S_IICIF_SHIFT);
	return !GET_BIT(MEM_MAPS[m_module]->S, I2C_S_RXAK_SHIFT);
}

bool I2cMaster::ReadByte_(const bool is_last_byte, Byte *out_byte)
{
	if (GET_BIT(MEM_MAPS[m_module]->C1, I2C_C1_TX_SHIFT))
	{
		CLEAR_BIT(MEM_MAPS[m_module]->C1, I2C_C1_TX_SHIFT);
	}
	if (!is_last_byte)
	{
		// ACK
		CLEAR_BIT(MEM_MAPS[m_module]->C1, I2C_C1_TXAK_SHIFT);
	}
	else
	{
		// NACK
		SET_BIT(MEM_MAPS[m_module]->C1, I2C_C1_TXAK_SHIFT);
	}

	// Initiate receive
	SET_BIT(MEM_MAPS[m_module]->S, I2C_S_IICIF_SHIFT);
	*out_byte = MEM_MAPS[m_module]->D;
	// Wait until data is received
	while (!GET_BIT(MEM_MAPS[m_module]->S, I2C_S_IICIF_SHIFT))
	{
		if (GET_BIT(MEM_MAPS[m_module]->SMB, I2C_SMB_SLTF_SHIFT))
		{
			LOG_DL("i2c scl timeout");
			return false;
		}
	}
	SET_BIT(MEM_MAPS[m_module]->S, I2C_S_IICIF_SHIFT);
	
	if (is_last_byte)
	{
		Stop();
	}
	*out_byte = MEM_MAPS[m_module]->D;
	return true;
}

bool I2cMaster::GetByte(const Byte slave_addr, const Byte reg_addr,
		Byte *out_byte)
{
	STATE_GUARD(I2cMaster, false);

	Start();
	SEND_BYTE_GUARDED(slave_addr << 1, false);
	SEND_BYTE_GUARDED(reg_addr, false);
	RepeatStart();
	SEND_BYTE_GUARDED((slave_addr << 1) | 0x1, false);
	return ReadByte_(true, out_byte);
}

vector<Byte> I2cMaster::GetBytes(const Byte slave_addr, const Byte reg_addr,
		const uint8_t size)
{
	STATE_GUARD(I2cMaster, {});

	vector<Byte> bytes;
	bytes.reserve(size);

	Start();
	SEND_BYTE_GUARDED(slave_addr << 1, {});
	SEND_BYTE_GUARDED(reg_addr, {});
	RepeatStart();
	SEND_BYTE_GUARDED((slave_addr << 1) | 0x1, {});
	for (uint8_t i = 0; i < size; ++i)
	{
		Byte byte;
		// NACK if last bit
		if (!ReadByte_((i == size - 1), &byte))
		{
			Stop();
			return bytes;
		}
		bytes.push_back(byte);
	}
	return bytes;
}

bool I2cMaster::SendByte(const Byte slave_addr, const Byte reg_addr,
		const Byte byte)
{
	STATE_GUARD(I2cMaster, false);

	Start();
	SEND_BYTE_GUARDED(slave_addr << 1, false);
	SEND_BYTE_GUARDED(reg_addr, false);
	SEND_BYTE_GUARDED(byte, false);
	Stop();
	return true;
}

bool I2cMaster::SendBytes(const Byte slave_addr, const Byte reg_addr,
		const Byte *bytes, const size_t size)
{
	STATE_GUARD(I2cMaster, false);

	Start();
	SEND_BYTE_GUARDED(slave_addr << 1, false);
	SEND_BYTE_GUARDED(reg_addr, false);
	for (size_t i = 0; i < size; ++i)
	{
		SEND_BYTE_GUARDED(bytes[i], false);
	}
	Stop();
	return true;
}

}
}
