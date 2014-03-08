/*
 * bt_controller.h
 * Bluetooth controller
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef BT_CONTROLLER_H_
#define BT_CONTROLLER_H_

#include <mini_common.h>
#include <cstdint>

#include "libutil/clock.h"
#include "smart_car.h"

namespace libsc
{

class BtController
{
public:
	static const uint8_t kFlagForward = 0x1;
	static const uint8_t kFlagBackward = 0x2;
	static const uint8_t kFlagStop = 0x4;
	static const uint8_t kFlagSpecial = 0x8;
	static const uint8_t kFlagTurn = 0x80;

	explicit BtController(SmartCar *sc);

	Byte Loop();
	void SetTurnEnable(const bool flag);

private:
	SmartCar *m_sc;
	libutil::Clock::ClockInt m_prev_command;
	libutil::Clock::ClockInt m_prev_accel;
	bool m_is_turn_enable;
};

}

#endif /* BT_CONTROLLER_H_ */
