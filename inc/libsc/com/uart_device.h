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

#include <vectors.h>

namespace libsc
{

class UartDevice
{
public:
	UartDevice(const uint8_t uart_port);
	~UartDevice();

	void StartReceive();
	void StopReceive();
	void SendChar(const char ch);
	void SendStr(const char *str);
	void SendBuffer(const uint8_t *buf, const uint32_t len);
	bool PeekChar(char *out_char);

private:
	static __ISR void IrqHandler();

	struct Chunk;
	Chunk *m_head;
	volatile Chunk *m_tail;

	uint8_t m_uart_port;
	uint8_t m_device_id;
};

}

#endif /* LIBSC_UART_DEVICE_H_ */
