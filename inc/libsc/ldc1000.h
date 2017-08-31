/*
 * ldc1000.h
 *
 * Author: Petel__
 * Copyright (c) 2014-2016 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cassert>
#include <cstdint>
#include "libbase/helper.h"
#include LIBBASE_H(gpio)

using namespace LIBBASE_NS;

namespace libsc
{

class Ldc1000
{

public:

	//Need configuration
	struct Config
	{
		uint8_t id;
	};

	//	explicit Lcd1000(const Config &config);

	Ldc1000(const Config &config);
	void Update(void);

	uint16_t GetData(void);
	uint32_t GetFreq(void);

private:

	void InitPin(uint8_t id);

	uint8_t	DataRW(uint8_t rwData);
	uint8_t ReadData(const uint8_t reg);
	uint8_t WriteData(const uint8_t reg, const uint8_t data);
	void ReadBuffer(void);

	uint8_t				m_buf[12];

	Gpo					m_mosi;
	Gpo					m_cs;
	Gpo					m_sck;
	Gpio				m_miso;

	uint16_t			m_proxData;
	uint32_t			m_freq;

};

}
