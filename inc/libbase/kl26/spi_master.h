/*
 * SpiMaster.h
 *
 *  Created on: Apr 30, 2015
 *      Author: harrison
 */

#ifndef INC_LIBBASE_KL26_SPI_MASTER_H_
#define INC_LIBBASE_KL26_SPI_MASTER_H_

#include <libbase/kl26/spi_master_interface.h>

namespace libbase {
namespace kl26 {

class SpiMaster: public SpiMasterInterface {

public:
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

//			OnTxFillListener tx_isr;
//			OnRxDrainListener rx_isr;
		};
	SpiMaster(const Config &config);
	~SpiMaster();
	operator bool() const{
		return m_is_init;
	};

	/**
	 * Exchange data with slave, this is a blocking operation. Do NOT use this
	 * method with listeners being setup
	 *
	 * @param slave_id The target slave, see Config::slaves for details
	 * @param data The data to be sent, in 3-wire setup without sout, this value
	 * will be ignored
	 * @return The received data, or 0 in 3-wire setup without sin
	 */
	uint16_t ExchangeData(const uint8_t slave_id, const uint16_t data) override;

	/**
	 * Use with listeners, ensure SPI is enabled and running
	 */
	void KickStart() override{

	}
	size_t PushData(const uint8_t slave_id, const uint16_t *data,
			const size_t size) override{

	}
	size_t PushData(const uint8_t slave_id, const uint8_t *data,
			const size_t size) override{

	}


private:
	uint8_t m_module;
	Pin m_sin;
	Pin m_sout;
	Pin m_sck;
	Pin m_cs[kSlaveCount];

	bool m_is_init;
};
} /* namespace kl26 */
} /* namespace libbase */

#endif /* INC_LIBBASE_KL26_SPI_MASTER_H_ */
