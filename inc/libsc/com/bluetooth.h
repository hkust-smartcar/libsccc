/*
 * bluetooth.h
 * Bluetooth abstraction
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_BLUETOOTH_H_
#define LIBSC_BLUETOOTH_H_

#include <cstdint>

#include <vectors.h>

namespace libsc
{

class Bluetooth
{
public:
	Bluetooth();
	~Bluetooth();

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

	static Bluetooth *m_instance;
	static bool m_is_initialized;
};

}

#endif /* LIBSC_BLUETOOTH_H_ */
