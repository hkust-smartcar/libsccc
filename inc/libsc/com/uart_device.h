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

#include <list>

#include <vectors.h>

namespace libsc
{

class UartDevice
{
public:
	typedef void (*OnReceiveCharListener)(const char);

	UartDevice(const uint8_t uart_port, const uint32_t baud_rate);
	~UartDevice();

	void StartReceive(OnReceiveCharListener listener);
	void StopReceive();

	void StartReceive()
	{
		StartReceive(nullptr);
	}

	void SendChar(const char ch);
	void SendStr(const char *str);
	void SendBuffer(const uint8_t *buf, const uint32_t len);
	bool PeekChar(char *out_char);

private:
	static __ISR void IrqHandler();

	void OnInterruptRx();
	void OnInterruptTx();

	struct Chunk;
	std::list<Chunk> m_receive_buf;
	OnReceiveCharListener m_listener;

	std::list<Chunk> m_send_buf;
	volatile uint8_t m_send_buf_size;
	volatile bool m_is_tx_idle;

	uint8_t m_uart_port;
	uint8_t m_device_id;
	uint8_t m_txfifo_size;
};

}

#endif /* LIBSC_UART_DEVICE_H_ */
