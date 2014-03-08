/*
 * bt_controller.h
 * Bluetooth controller
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <mini_common.h>

#include "libsc/smart_car.h"
#include "libutil/clock.h"

#include "libsc/bt_controller.h"

#define INITIAL_SPEED 220

namespace libsc
{

BtController::BtController(SmartCar *sc)
		: m_sc(sc), m_prev_command(0), m_prev_accel(0), m_is_turn_enable(true)
{}

Byte BtController::Loop()
{
	using libutil::Clock;

	const Clock::ClockInt now = Clock::Time();
	char ch;
	if (m_sc->BluetoothPeekChar(&ch))
	{
		m_prev_command = now;
		if (ch & kFlagTurn)
		{
			if (!m_is_turn_enable)
			{
				return ch;
			}

			const int right_percentage = ch & ~kFlagTurn;
			if (right_percentage < 50)
			{
				m_sc->TurnLeft((50 - right_percentage) * 2);
			}
			else
			{
				m_sc->TurnRight((right_percentage - 50) * 2);
			}
		}
		else
		{
			if (ch & (kFlagForward | kFlagBackward))
			{
				const bool is_go_forward = ch & kFlagForward;
				if (m_sc->IsMotorForward() != is_go_forward || m_sc->IsMotorStop())
				{
					m_sc->StartMotor(is_go_forward, INITIAL_SPEED);
				}
				else if (libutil::Clock::TimeDiff(now, m_prev_accel) > 200)
				{
					m_sc->AddMotorPowerTil(5, 450);
					m_prev_accel = now;
				}
			}
			else if (ch & kFlagStop)
			{
				m_sc->StopMotor();
			}
		}
	}
	else
	{
		ch = 0;
		if (libutil::Clock::TimeDiff(now, m_prev_command) > 1250
				&& !m_sc->IsMotorStop())
		{
			// TODO
			m_sc->StopMotor();
		}
	}
	return ch;
}

void BtController::SetTurnEnable(const bool flag)
{
	m_is_turn_enable = flag;
}

}
