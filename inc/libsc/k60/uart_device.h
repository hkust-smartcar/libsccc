/*
 * uart_device.h
 * Generic class for UART devices, also handles Tx and Rx buffering
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "libbase/k60/dma.h"
#include "libbase/k60/misc_utils.h"
#include "libbase/k60/uart.h"

namespace libsc
{
namespace k60
{

class UartDevice
{
public:
	typedef std::function<bool(const Byte *data, const size_t size)>
			OnReceiveListener;

	struct Config
	{
		uint8_t id;
		libbase::k60::Uart::Config::BaudRate baud_rate;

		/**
		 * The number of bytes in the Rx buffer needed to fire the interrupt.
		 * This will affect how often new bytes are pushed to the internal
		 * buffer, or your listener being triggered, depending on the config. If
		 * a buffer is not available for your device, this setting has no effect
		 */
		uint8_t rx_irq_threshold = 1;
		/// To treat rx_irq_threshold as a percentage of Rx buffer size
		bool is_rx_irq_threshold_percentage = false;

		/**
		 * The size of the Tx buffer. Old data will be poped when the buffer
		 * overflows. Notice that this size is not in bytes, but rather the
		 * number of Send* calls. Depending on the use case, the actualy buffer
		 * size in bytes will vary
		 */
		uint8_t tx_buf_size = 14;
		/**
		 * (Experimental) If value != -1, DMA will be enabled for this UART's Tx,
		 * using the DMA channel specified here
		 */
		uint8_t tx_dma_channel = static_cast<uint8_t>(-1);

		/**
		 * The listener for Rx events. Return true if the listener has consumed
		 * the data. In that case, the data won't be pushed to the internal
		 * buffer
		 */
		OnReceiveListener rx_isr;
	};

	virtual ~UartDevice();

	/**
	 * Send a string through UART. A copy will be queued
	 *
	 * @param str
	 * @return true if successful, false otherwise (say, tx buffer is full)
	 */
	bool SendStr(const char *str);
	/**
	 * Send a string through UART. A moved copy will be queued
	 *
	 * @param str
	 * @return true if successful, false otherwise (say, tx buffer is full)
	 */
	bool SendStr(std::unique_ptr<char[]> &&str);
	/**
	 * Send a string through UART. A moved copy will be queued
	 *
	 * @param str
	 * @return true if successful, false otherwise (say, tx buffer is full)
	 */
	bool SendStr(std::string &&str);

	/**
	 * Send a buffer through UART. A copy will be queued
	 *
	 * @param buf
	 * @param len
	 * @return true if successful, false otherwise (say, tx buffer is full)
	 */
	bool SendBuffer(const Byte *buf, const size_t len);
	/**
	 * Send a buffer through UART. A moved copy will be queued
	 *
	 * @param buf
	 * @param len
	 * @return true if successful, false otherwise (say, tx buffer is full)
	 */
	bool SendBuffer(std::unique_ptr<Byte[]> &&buf, const size_t len);
	/**
	 * Send a buffer through UART. A moved copy will be queued
	 *
	 * @param buf
	 * @return true if successful, false otherwise (say, tx buffer is full)
	 */
	bool SendBuffer(std::vector<Byte> &&buf);

	/**
	 * Send a string literal through UART. MUST ONLY be used with string
	 * literals
	 *
	 * @param str
	 * @return true if successful, false otherwise (say, tx buffer is full)
	 */
	bool SendStrLiteral(const char *str);

	bool SendStr(const std::string &str)
	{
		return SendBuffer(reinterpret_cast<const Byte*>(str.data()), str.size());
	}
	bool SendBuffer(const std::vector<Byte> &buf)
	{
		return SendBuffer(buf.data(), buf.size());
	}

	bool PeekChar(char *out_char);

	void SetLoopMode(const bool flag)
	{
		m_uart.SetLoopMode(flag);
	}

protected:
	/**
	 * Use to initialize the UartDevice in possibly a polymorphic way, notice
	 * that Initializer::config is stored as a reference only
	 */
	struct Initializer
	{
		explicit Initializer(const Config &config)
				: config(config)
		{}

		virtual libbase::k60::Uart::Config GetUartConfig() const;

		const Config &config;
	};

	explicit UartDevice(const Initializer &initializer);
	explicit UartDevice(nullptr_t);

private:
	struct RxBuffer;
	class TxBuffer;

	inline void EnableTx();
	inline void DisableTx();
	inline bool IsUseDma();

	void NextTxDma();

	void OnTxEmpty(libbase::k60::Uart *uart);
	void OnTxDmaComplete(libbase::k60::Dma *dma);
	void OnRxFull(libbase::k60::Uart *uart);

	std::unique_ptr<volatile RxBuffer> m_rx_buf;
	OnReceiveListener m_rx_isr;

	std::unique_ptr<TxBuffer> m_tx_buf;
	volatile bool m_is_tx_idle;

	std::unique_ptr<libbase::k60::Dma::Config> m_dma_config;
	libbase::k60::Dma *m_dma;

	libbase::k60::Uart m_uart;
};

}
}
