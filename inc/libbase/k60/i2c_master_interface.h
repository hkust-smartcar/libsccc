/*
 * i2c_master_interface.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include <vector>

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"

namespace libbase
{
namespace k60
{

class I2cMasterInterface
{
public:
	struct Config
	{
		Pin::Name scl_pin;
		Pin::Name sda_pin;
		uint16_t scl_low_timeout = UINT16_MAX;
		/**
		 * Generate a START signal followed by a calling command without
		 * generating a STOP signal first. This saves the time needed for the
		 * STOP signal
		 */
		bool is_use_repeated_start = true;
	};

	virtual ~I2cMasterInterface()
	{}

	virtual operator bool() const = 0;

	/**
	 * Receive a single byte from a slave device
	 *
	 * @param slave_addr 7-bit address of the slave device
	 * @param reg_addr Address of the interesting register
	 * @param out_byte The received byte, no change if false is returned
	 * @return true if successful, false otherwise
	 */
	virtual bool GetByte(const Byte slave_addr, const Byte reg_addr,
			Byte *out_byte)  = 0;
	/**
	 * Receive multiple bytes from a slave device
	 *
	 * @param slave_addr 7-bit address of the slave device
	 * @param reg_addr Address of the first interesting register
	 * @param size Number of register reads
	 * @return Received data, if the size of the returned data is < than @a size,
	 * an error has occurred
	 */
	virtual std::vector<Byte> GetBytes(const Byte slave_addr,
			const Byte reg_addr, const uint8_t size) = 0;
	/**
	 * Send a single byte to a slave device
	 *
	 * @param slave_addr 7-bit address of the slave device
	 * @param reg_addr Address of the interesting register
	 * @param byte The byte to send
	 * @return true if successful, false otherwise
	 */
	virtual bool SendByte(const Byte slave_addr, const Byte reg_addr,
			const Byte byte) = 0;
	/**
	 * Send multiple bytes to a slave device
	 *
	 * @param slave_addr 7-bit address of the slave device
	 * @param reg_addr Address of the first interesting register
	 * @param bytes Data to send
	 * @param size Size of the @a bytes array
	 * @return true if successful, false otherwise
	 */
	virtual bool SendBytes(const Byte slave_addr, const Byte reg_addr,
			const Byte *bytes, const size_t size) = 0;
};

}
}
