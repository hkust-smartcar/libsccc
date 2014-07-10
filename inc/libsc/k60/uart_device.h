/*
 * uart_device.h
 * UART device abstraction
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_UART_DEVICE_H_
#define LIBSC_UART_DEVICE_H_

#include <cstdint>

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/uart.h"

namespace libbase
{
namespace k60
{

class Uart;

}
}

namespace libsc
{
namespace k60
{

class UartDevice
{
public:
	typedef std::function<void(const Byte *bytes,
			const size_t size)> OnReceiveListener;

	UartDevice(const uint8_t id,
			const libbase::k60::Uart::Config::BaudRate baud_rate);
	~UartDevice();

	/**
	 * Send a string through UART. A copy will be queued
	 *
	 * @param str
	 */
	void SendStr(const char *str);
	/**
	 * Send a string through UART. A moved copy will be queued
	 *
	 * @param str
	 */
	void SendStr(std::unique_ptr<char[]> &&str);
	/**
	 * Send a string through UART. A moved copy will be queued
	 *
	 * @param str
	 */
	void SendStr(std::string &&str);

	/**
	 * Send a buffer through UART. A copy will be queued
	 *
	 * @param buf
	 * @param len
	 */
	void SendBuffer(const Byte *buf, const size_t len);
	/**
	 * Send a buffer through UART. A moved copy will be queued
	 *
	 * @param buf
	 * @param len
	 */
	void SendBuffer(std::unique_ptr<Byte[]> &&buf, const size_t len);
	/**
	 * Send a buffer through UART. A moved copy will be queued
	 *
	 * @param buf
	 */
	void SendBuffer(std::vector<Byte> &&buf);

	/**
	 * Send a string literal through UART. MUST ONLY be used with string
	 * literals
	 *
	 * @param str
	 */
	void SendStrLiteral(const char *str);

	void SendStr(const std::string &str)
	{
		SendBuffer(reinterpret_cast<const Byte*>(str.data()), str.size());
	}
	void SendBuffer(const std::vector<Byte> &buf)
	{
		SendBuffer(buf.data(), buf.size());
	}

	void EnableRx(const OnReceiveListener &listener);
	void EnableRx()
	{
		EnableRx(nullptr);
	}
	void DisableRx();
	bool PeekChar(char *out_char);

	void SetLoopMode(const bool flag)
	{
		m_uart.SetLoopMode(flag);
	}

private:
	struct RxBuffer;
	struct TxBuffer;

	inline void EnableTx();
	inline void DisableTx();

	void OnTxEmpty(libbase::k60::Uart *uart);
	void OnRxFull(libbase::k60::Uart *uart);

	std::unique_ptr<volatile RxBuffer> m_rx_buf;
	OnReceiveListener m_listener;

	std::unique_ptr<TxBuffer> m_tx_buf;
	volatile bool m_is_tx_idle;

	libbase::k60::Uart m_uart;
};

}
}

#endif /* LIBSC_UART_DEVICE_H_ */
