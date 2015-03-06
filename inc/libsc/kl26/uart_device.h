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

#include "libbase/helper.h"
#include "libbase/misc_types.h"
#include LIBBASE_H(uart)

namespace libsc
{
namespace kl26
{

class UartDevice
{
public:
	typedef std::function<bool(const Byte)> OnReceiveListener;

	struct Config
	{
		uint8_t id;
		LIBBASE_MODULE(Uart)::Config::BaudRate baud_rate;

		/**
		 * The size of the Tx buffer. Old data will be poped when the buffer
		 * overflows. Notice that this size is not in bytes, but rather the
		 * number of Send* calls. Depending on the use case, the actualy buffer
		 * size in bytes will vary
		 */
		uint8_t tx_buf_size = 14;

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

		virtual LIBBASE_MODULE(Uart)::Config GetUartConfig() const;

		const Config &config;
	};

	explicit UartDevice(const Initializer &initializer);
	explicit UartDevice(nullptr_t);

private:
	struct RxBuffer;
	class TxBuffer;

	inline void EnableTx();
	inline void DisableTx();

	void OnTxEmpty(LIBBASE_MODULE(Uart) *uart);
	void OnRxFull(LIBBASE_MODULE(Uart) *uart);

	std::unique_ptr<volatile RxBuffer> m_rx_buf;
	OnReceiveListener m_rx_isr;

	std::unique_ptr<TxBuffer> m_tx_buf;
	volatile bool m_is_tx_idle;

	LIBBASE_MODULE(Uart) m_uart;
};

}
}
