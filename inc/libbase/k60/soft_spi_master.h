/*
 * soft_spi_master.h
 * Emulated SPI through GPIO with a compatible interface with SpiMaster
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "libbase/k60/gpio.h"
#include "libbase/k60/spi_master_interface.h"

namespace libbase
{
namespace k60
{

class SoftSpiMaster : public SpiMasterInterface
{
public:
	typedef SpiMasterInterface::Config Config;

	static constexpr Uint kSlaveCount = SpiMasterInterface::kSlaveCount;

	/**
	 * Construct a new instance. Only the following parameters are supported,
	 * others will be ignored:
	 * sin_pin, sout_pin, sck_pin, frame_size, is_sck_idle_low, is_msb_firt,
	 * slaves
	 *
	 * @param config
	 */
	explicit SoftSpiMaster(const Config &config);
	explicit SoftSpiMaster(nullptr_t);
	SoftSpiMaster(const SoftSpiMaster&) = delete;
	SoftSpiMaster(SoftSpiMaster &&rhs);
	~SoftSpiMaster();

	SoftSpiMaster& operator=(const SoftSpiMaster&) = delete;
	SoftSpiMaster& operator=(SoftSpiMaster &&rhs);
	operator bool() const override
	{
		return m_is_init;
	}

	uint16_t ExchangeData(const uint8_t slave_id, const uint16_t data) override;

	void KickStart() override;
	size_t PushData(const uint8_t slave_id, const uint16_t *data,
			const size_t size) override;
	size_t PushData(const uint8_t slave_id, const uint8_t *data,
			const size_t size) override;

	uint8_t GetFrameSize() const
	{
		return m_frame_size;
	}

private:
	Gpi m_sin;
	Gpo m_sout;
	Gpo m_sck;
	Gpo m_cs[kSlaveCount];

	uint8_t m_frame_size;
	bool m_is_sck_capture_first;
	bool m_is_msb_first;

	bool m_is_init;
};

}
}
