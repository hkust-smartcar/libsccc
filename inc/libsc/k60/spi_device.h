/*
 * spi_device.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include <list>
#include <memory>
#include <string>
#include <vector>

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/spi_master.h"

#include "libutil/dynamic_block_buffer.h"

namespace libsc
{
namespace k60
{

class SpiDevice
{
public:
	explicit SpiDevice(const libbase::k60::SpiMaster::Config &config);
	explicit SpiDevice(nullptr_t);
	SpiDevice(const SpiDevice&) = delete;
	SpiDevice(SpiDevice &&rhs);
	~SpiDevice();

	/**
	 * Send a buffer through SPI. A copy will be queued
	 *
	 * @param buf
	 * @param len
	 */
	void SendBuffer(const Byte *buf, const size_t len);
	void SendBuffer(const uint16_t *buf, const size_t len)
	{
		SendBuffer(reinterpret_cast<const Byte*>(buf), len * 2);
	}
	/**
	 * Send a buffer through SPI. A moved copy will be queued
	 *
	 * @param buf
	 * @param len
	 */
	void SendBuffer(std::unique_ptr<Byte[]> &&buf, const size_t len);
	void SendBuffer(std::unique_ptr<uint16_t[]> &&buf, const size_t len)
	{
		SendBuffer(std::unique_ptr<Byte[]>(reinterpret_cast<Byte*>(buf.release())),
				len * 2);
	}
	/**
	 * Send a buffer through SPI. A moved copy will be queued
	 *
	 * @param buf
	 */
	void SendBuffer(std::vector<Byte> &&buf);

private:
	struct Buffer;

	void OnTxFill(libbase::k60::SpiMaster *spi);
	void OnRxDrain(libbase::k60::SpiMaster *spi);

	//std::list<Buffer> m_rx_buf;
	libutil::DynamicBlockBuffer m_tx_buf;

	libbase::k60::SpiMaster m_spi;
};

}
}
