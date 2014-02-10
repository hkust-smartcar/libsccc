/*
 * smart_car.h
 * Smart car abstraction
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_SMART_CAR_H_
#define LIBSC_SMART_CAR_H_

#include <cstdint>

#include "libsc/com/bluetooth.h"
#include "libsc/com/led.h"
#include "libsc/com/motor.h"
#include "libsc/com/servo.h"

namespace libsc
{

class SmartCar
{
public:
	SmartCar();

	/**
	 * Start the wheel motor
	 *
	 * @param is_forward Is going forward or not
	 * @param power Power scale in [0, 1000]
	 */
	void StartMotor(const bool is_forward, const uint16_t power)
	{
		SetMotorDirection(is_forward);
		SetMotorPower(power);
	}

	void StopMotor()
	{
		SetMotorPower(0);
	}

	void SetMotorDirection(const bool is_forward);
	void SetMotorPower(const uint16_t power);
	void AddMotorPower(const uint16_t factor);
	void AddMotorPowerTil(const uint16_t factor, const uint16_t max);
	void DropMotorPower(const uint16_t factor);
	void DropMotorPowerTil(const uint16_t factor, const uint16_t min);

	/**
	 * Turn the robot to the left
	 *
	 * @param percentage Specifying how aggressively should the car turn,
	 * in [0, 100], where passing 0 basically means going straight
	 */
	void TurnLeft(const uint8_t percentage);
	void TurnRight(const uint8_t percentage);
	/**
	 * Combining TurnLeft() and TurnRight() into one
	 *
	 * @param percentage 0 means turning left, and 70 means slightly turning
	 * right
	 * @return
	 */
	void SetRightPercentage(const uint8_t percentage);
	void GoStraight()
	{
		TurnLeft(0);
	}

	/**
	 * Switch on/off the LEDs
	 *
	 * @param obj
	 * @param id The id of the LED, [0, 3]
	 * @param flag
	 */
	void SwitchLed(const uint8_t id, const bool flag);

	void BluetoothSendStr(const char *str);
	bool BluetoothPeekChar(char *out_ch);

	bool IsMotorForward() const;
	bool IsMotorStop() const;
	uint16_t GetMotorPower() const;

	uint8_t GetRightPercentge() const;

private:
	Bluetooth m_bt;
	Led m_leds[4];
	Motor m_motor;
	Servo m_servo;
};

}

#endif /* LIBSC_SMART_CAR_H_ */
