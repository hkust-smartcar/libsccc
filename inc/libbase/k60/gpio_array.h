/*
 * gpio_array.h
 * Parallel GPIO
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include <vector>

#include "libbase/k60/gpio.h"

namespace libbase
{
namespace k60
{

class GpiArray
{
public:
	struct Config
	{
		Pin::Name start_pin;
		uint8_t count;
		std::bitset<Pin::Config::ConfigBit::kSize> config;
	};

	explicit GpiArray(const Config &config);
	explicit GpiArray(nullptr_t);
	GpiArray(GpiArray &&rhs);
	~GpiArray();

	GpiArray& operator=(const GpiArray&) = delete;
	GpiArray& operator=(GpiArray &&rhs);
	operator bool() const
	{
		return !m_pins.empty();
	}

	std::vector<bool> Get() const;

	Gpi* GetChild(const uint8_t position)
	{
		return &m_pins[position];
	}

	/**
	 * Config this GpiArray up to be ready to serve as DMA source, and set
	 * @a config accordingly. The following options are also set besides src:<br>
	 * Dma::Config::minor_bytes = 1~4 (depending on Config::count)<br>
	 * Dma::Config::major_count = 1
	 *
	 * Notice that Gpi value is byte-aligned when working with DMA, so that if
	 * Config::start_pin is not the 0/8/16/24th pin, the data received would be
	 * pre-padded with data from other pins. Similarly, the data would be
	 * post-padded if Config::start_pin + Config::count does not fit in 1/2/3/4
	 * byte boundary
	 *
	 * @param config
	 */
	void ConfigValueAsDmaSrc(Dma::Config *config);

private:
	void Uninit();

	std::vector<Gpi> m_pins;
};

}
}
