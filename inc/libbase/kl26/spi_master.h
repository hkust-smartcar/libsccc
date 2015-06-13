/*
 * spi_master.h
 *
 * Author: Harrison Ng
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include <functional>

#include "libbase/kl26/pin.h"
#include "libbase/kl26/spi_master_interface.h"

namespace libbase
{
namespace kl26
{

class SpiMaster: public SpiMasterInterface
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

		OnTxFillListener tx_isr;
		OnRxDrainListener rx_isr;
	};

	explicit SpiMaster(const Config &config);
	~SpiMaster();

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

private:
	bool InitModule(const Config &config);
	void InitPin(const Config &config);
	void InitBrReg(const Config &config);
	void InitC2Reg(const Config &config);
	void InitC1Reg(const Config &config);
	void Uninit();

	void SetEnable(const bool flag);

	uint8_t m_module;

	Pin m_sin;
	Pin m_sout;
	Pin m_sck;
	Pin m_cs;

	OnTxFillListener m_tx_isr;
	OnRxDrainListener m_rx_isr;

	bool m_is_init;
};

}
}
