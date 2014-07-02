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
#include <list>
#include <memory>
#include <string>

#include "libbase/k60/misc_utils.h"
#include "libbase/k60/uart.h"

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
			const libbase::k60::UartConfig::BaudRate baud_rate);
	~UartDevice();

	void StartReceive(OnReceiveListener listener);
	void StopReceive();

	void StartReceive()
	{
		StartReceive(nullptr);
	}

	void SendByte(const Byte b);
	void SendStr(const char *str)
	{
		while (*str)
		{
			SendByte(*str++);
		}
	}

	void SendBuffer(const Byte *buf, const size_t len)
	{
		for (size_t i = 0; i < len; ++i)
		{
			SendByte(static_cast<char>(buf[i]));
		}
	}

	bool PeekChar(char *out_char);

private:
	struct Chunk;

	void OnTxEmpty(libbase::k60::Uart *uart);
	void OnRxFull(libbase::k60::Uart *uart);

	std::list<Chunk> m_rx_buf;
	OnReceiveListener m_listener;

	std::list<Chunk> m_tx_buf;
	volatile bool m_is_tx_idle;

	libbase::k60::Uart m_uart;
};

}
}

#endif /* LIBSC_UART_DEVICE_H_ */
