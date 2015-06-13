/*
 * spi_master.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include <functional>

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/pin.h"
#include "libbase/k60/spi_master_interface.h"

namespace libbase
{
namespace k60
{

class SpiMaster : public SpiMasterInterface
{
public:
	typedef std::function<void(SpiMaster *spi)> OnTxFillListener;
	typedef std::function<void(SpiMaster *spi)> OnRxDrainListener;

	struct Config : public SpiMasterInterface::Config
	{
		/**
		 * Baud rate hint, the closest possible value will be used
		 */
		uint32_t baud_rate_khz;
		/**
		 * The PCS to SCK delay is the length of time from assertion of the PCS
		 * signal to the first SCK edge. Time hint, the closest possible value
		 * will be used
		 */
		uint32_t pcs_to_sck_delay_ns = 0;
		/**
		 * The After SCK Delay is the length of time between the last edge of
		 * SCK and the negation of PCS. Time hint, the closest possible value
		 * will be used
		 */
		uint32_t after_sck_delay_ns = 0;
		/**
		 * The Delay after Transfer is the minimum time between negation of the
		 * PCS signal for a frame and the assertion of the PCS signal for the
		 * next frame. Time hint, the closest possible value will be used
		 */
		uint32_t after_transfer_delay_ns = 0;
		/**
		 * To allow for high-speed communication with peripherals that require
		 * longer setup times. The DSPI can sample the incoming data later than
		 * halfway through the cycle to give the peripheral more setup time
		 */
		bool is_modified_timing = false;

		OnTxFillListener tx_isr;
		OnRxDrainListener rx_isr;
	};

	explicit SpiMaster(const Config &config);
	explicit SpiMaster(nullptr_t);
	SpiMaster(const SpiMaster&) = delete;
	SpiMaster(SpiMaster &&rhs);
	~SpiMaster();

	SpiMaster& operator=(const SpiMaster&) = delete;
	SpiMaster& operator=(SpiMaster &&rhs);
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

	/**
	 * Enable Tx/Rx interrupt, by default they are both disabled after
	 * initialization and require programmer to explicitly enable them
	 *
	 * @param flag
	 */
	void SetEnableRxIrq(const bool flag);
	void SetEnableTxIrq(const bool flag);

	uint8_t GetFrameSize() const
	{
		return m_frame_size;
	}

private:
	bool InitModule(const Config &config);
	void InitPin(const Config &config);
	void InitMcrReg(const Config &config);
	void InitCtarReg(const Config &config);
	void InitInterrupt(const Config &config);
	void SetBaudRate(const uint32_t baud_rate_khz, uint32_t *reg);
	void SetPcsSckDelay(const uint32_t pcs_sck_delay_ns, uint32_t *reg);
	void SetAfterSckDelay(const uint32_t after_sck_delay_ns, uint32_t *reg);
	void SetAfterTransferDelay(const uint32_t after_transfer_delay_ns,
			uint32_t *reg);
	void SetInterrupt(const bool tx_flag, const bool rx_flag);
	void Uninit();

	void SetHalt(const bool flag);

	static __ISR void IrqHandler();

	uint8_t m_module;
	uint8_t m_frame_size;

	Pin m_sin;
	Pin m_sout;
	Pin m_sck;
	Pin m_cs[kSlaveCount];

	OnTxFillListener m_tx_isr;
	OnRxDrainListener m_rx_isr;

	bool m_is_init;
};

}
}
