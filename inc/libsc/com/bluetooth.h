/*
 * bluetooth.h
 * Bluetooth abstraction. For compatibility only, consider using UartDevice
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_BLUETOOTH_H_
#define LIBSC_BLUETOOTH_H_

#include "libsc/com/uart_device.h"

namespace libsc
{

class Bluetooth : public UartDevice
{
public:
	Bluetooth();
};

}

#endif /* LIBSC_BLUETOOTH_H_ */
