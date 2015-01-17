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
#include "libbase/k60/pinout.h"

namespace libbase
{
namespace k60
{

class SpiMaster
{
public:
	typedef std::function<void(SpiMaster *spi)> OnTxFillListener;
	typedef std::function<void(SpiMaster *spi)> OnRxDrainListener;

	static constexpr Uint kSlaveCount = 6;

	struct Config
	{
		struct Slave
		{
			/**
			 * Set the chip select/slave select pin, Pin::Name::kDisable will
			 * be ignored
			 */
			Pin::Name cs_pin = Pin::Name::kDisable;
			/**
			 * Set the chip select/slave select polarity
			 */
			bool is_active_high = false;
		};

		// At least one among SIN/SOUT have to be set
		Pin::Name sin_pin = Pin::Name::kDisable;
		Pin::Name sout_pin = Pin::Name::kDisable;
		Pin::Name sck_pin;

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
		/**
		 * # bits transfered per frame, [4, 16]
		 */
		uint8_t frame_size = 8;
		/**
		 * Set the clock polarity
		 */
		bool is_sck_idle_low = true;
		/**
		 * Set the clock phase, if true, data is captured on the leading edge of
		 * SCK and changed on the following edge
		 */
		bool is_sck_capture_first = true;
		/**
		 * Specifies whether the LSB or MSB of the frame is transferred first
		 */
		bool is_msb_firt = true;

		OnTxFillListener tx_isr;
		OnRxDrainListener rx_isr;

		/**
		 * Slaves connected to this SPI instance, a maximum of 6 slaves are
		 * supported. The array should be filled in sequence (from [0] to [5]).
		 * The position will serve as the slave id used in Exchange()
		 */
		Slave slaves[kSlaveCount];
	};

	explicit SpiMaster(const Config &config);
	explicit SpiMaster(nullptr_t);
	SpiMaster(const SpiMaster&) = delete;
	SpiMaster(SpiMaster &&rhs);
	~SpiMaster();

	SpiMaster& operator=(const SpiMaster&) = delete;
	SpiMaster& operator=(SpiMaster &&rhs);
	operator bool() const
	{
		return m_is_init;
	}

	/**
	 * Exchange data with slave, this is a blocking operation. Do NOT use this
	 * method with listeners being setup
	 *
	 * @param slave_id The target slave, see Config::slaves for details
	 * @param data The data to be sent, in 3-wire setup without sout, this value
	 * will be ignored
	 * @return The received data, or 0 in 3-wire setup without sin
	 */
	uint16_t ExchangeData(const uint8_t slave_id, const uint16_t data);

	/**
	 * Use with listeners, ensure SPI is enabled and running
	 */
	void KickStart();
	size_t PushData(const uint8_t slave_id, const uint16_t *data,
			const size_t size);
	size_t PushData(const uint8_t slave_id, const uint8_t *data,
			const size_t size);

	uint8_t GetFrameSize() const
	{
		return m_frame_size;
	}

private:
	bool InitModule(const Config &config);
	void InitPin(const Config &config);
	void InitMcrReg(const Config &config);
	void InitCtarReg(const Config &config);
	void SetBaudRate(const uint32_t baud_rate_khz, uint32_t *reg);
	void SetPcsSckDelay(const uint32_t pcs_sck_delay_ns, uint32_t *reg);
	void SetAfterSckDelay(const uint32_t after_sck_delay_ns, uint32_t *reg);
	void SetAfterTransferDelay(const uint32_t after_transfer_delay_ns,
			uint32_t *reg);
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
