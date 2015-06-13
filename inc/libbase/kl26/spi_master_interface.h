/*
 * spi_master_interface.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "libbase/misc_types.h"
#include "libbase/kl26/pin.h"

namespace libbase
{
namespace kl26
{

class SpiMasterInterface
{
public:
	struct Config
	{
		// At least one among SIN/SOUT have to be set
		Pin::Name sin_pin = Pin::Name::kDisable;
		Pin::Name sout_pin = Pin::Name::kDisable;
		Pin::Name sck_pin;
		Pin::Name pcs_pin;

		/**
		 * # bits transfered per frame, [4, 16]
		 */
		uint8_t frame_size = 8;
		/**
		 * Set the clock polarity
		 */
		bool is_sck_idle_low = true;
		/**
		 * Specifies whether the LSB or MSB of the frame is transferred first
		 */
		bool is_msb_firt = true;
		/**
		 * Set the clock phase, if true, data is captured on the leading edge of
		 * SCK and changed on the following edge
		 */
		bool is_sck_capture_first = true;
	};

	virtual ~SpiMasterInterface()
	{}

	virtual operator bool() const = 0;

	/**
	 * Exchange data with slave, this is a blocking operation. Do NOT use this
	 * method with listeners being setup
	 *
	 * @param slave_id The target slave, see Config::slaves for details
	 * @param data The data to be sent, in 3-wire setup without sout, this value
	 * will be ignored
	 * @return The received data, or 0 in 3-wire setup without sin
	 */
	virtual uint16_t ExchangeData(const uint8_t slave_id, const uint16_t data) = 0;

	/**
	 * Use with listeners, ensure SPI is enabled and running
	 */
	virtual void KickStart() = 0;
	virtual size_t PushData(const uint8_t slave_id, const uint16_t *data,
			const size_t size) = 0;
	virtual size_t PushData(const uint8_t slave_id, const uint8_t *data,
			const size_t size) = 0;
};

}
}
