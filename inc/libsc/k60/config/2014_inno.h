/*
 * 2014_inno.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#define LIBSC_USE_BUTTON 2
//#define LIBSC_USE_BUZZER 1
//#define LIBSC_USE_JOYSTICK 1
#define LIBSC_USE_LED 4
#define LIBSC_USE_MOTOR 1
#define LIBSC_USE_SERVO 1
#define LIBSC_USE_UART 1

#define LIBSC_BUTTON0 libbase::k60::Pin::Name::kPtc0
#define LIBSC_BUTTON1 libbase::k60::Pin::Name::kPtb23

#define LIBSC_BUZZER0 libbase::k60::Pin::Name::kPta7
#define LIBSC_BUZZER_ACIVE_LEVEL 0

#define LIBSC_JOYSTICK0_UP libbase::k60::Pin::Name::kPtc7
#define LIBSC_JOYSTICK0_DOWN libbase::k60::Pin::Name::kPtc6
#define LIBSC_JOYSTICK0_LEFT libbase::k60::Pin::Name::kPtc5
#define LIBSC_JOYSTICK0_RIGHT libbase::k60::Pin::Name::kPtc12
#define LIBSC_JOYSTICK0_SELECT libbase::k60::Pin::Name::kPtc8

#define LIBSC_LED0 libbase::k60::Pin::Name::kPte12
#define LIBSC_LED1 libbase::k60::Pin::Name::kPte11
#define LIBSC_LED2 libbase::k60::Pin::Name::kPte10
#define LIBSC_LED3 libbase::k60::Pin::Name::kPte9

#define LIBSC_MOTOR0_PWMA libbase::k60::Pin::Name::kPtc1
#define LIBSC_MOTOR0_PWMB libbase::k60::Pin::Name::kPtc2
#define LIBSC_MOTOR0_DEADTIME 1000
#define LIBSC_ALTERNATE_MOTOR_CW_PWM 1

#define LIBSC_SERVO0 libbase::k60::Pin::Name::kPta10

#define LIBSC_UART0_TX libbase::k60::Pin::Name::kPtd7
#define LIBSC_UART0_RX libbase::k60::Pin::Name::kPtd6
