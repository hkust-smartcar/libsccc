/*
 * 2016_magnetic.h
 *
 *  Author: petel__
 *  Copyright (c) 2015-2016 HKUST SmartCar Team
 *	Refer to LICENSE for details
 */

#pragma once

#define LIBSC_USE_BATTERY_METER 1
#define LIBSC_USE_BUTTON 3
#define LIBSC_USE_BUZZER 1
#define LIBSC_USE_JOYSTICK 1
#define LIBSC_USE_LCD 1
#define LIBSC_USE_LED 4
#define LIBSC_USE_MOTOR 2
#define LIBSC_USE_MAGSEN 6
#define LIBSC_USE_UART 1
#define LIBSC_USE_ENCODER 2
#define LIBSC_USE_MPU6050 1
#define LIBSC_USE_MMA8451Q 1

#define LIBSC_BATTERY_METER libbase::k60::Adc::Name::kAdc0Ad14

#define LIBSC_BUTTON0 libbase::k60::Pin::Name::kPtc13
#define LIBSC_BUTTON1 libbase::k60::Pin::Name::kPtc14
#define LIBSC_BUTTON2 libbase::k60::Pin::Name::kPtc15

#define LIBSC_BUZZER0 libbase::k60::Pin::Name::kPta5
#define LIBSC_BUZZER_ACIVE_LEVEL 0

#define LIBSC_JOYSTICK0_UP libbase::k60::Pin::Name::kPtc19
#define LIBSC_JOYSTICK0_DOWN libbase::k60::Pin::Name::kPtd0
#define LIBSC_JOYSTICK0_LEFT libbase::k60::Pin::Name::kPtd1
#define LIBSC_JOYSTICK0_RIGHT libbase::k60::Pin::Name::kPtd2
#define LIBSC_JOYSTICK0_SELECT libbase::k60::Pin::Name::kPtd3

#define LIBSC_ST7735R_RST libbase::k60::Pin::Name::kPta17
#define LIBSC_ST7735R_DC libbase::k60::Pin::Name::kPtc3
#define LIBSC_ST7735R_CS libbase::k60::Pin::Name::kPta14
#define LIBSC_ST7735R_SDAT libbase::k60::Pin::Name::kPta16
#define LIBSC_ST7735R_SCLK libbase::k60::Pin::Name::kPta15

#define LIBSC_LED0 libbase::k60::Pin::Name::kPte8
#define LIBSC_LED1 libbase::k60::Pin::Name::kPte9
#define LIBSC_LED2 libbase::k60::Pin::Name::kPte10
#define LIBSC_LED3 libbase::k60::Pin::Name::kPte11

#define LIBSC_MOTOR0_DIRMOTOR 2
#define LIBSC_MOTOR0_PWM libbase::k60::Pin::Name::kPtc8
#define LIBSC_MOTOR0_DIR libbase::k60::Pin::Name::kPtc6
#define LIBSC_MOTOR1_PWM libbase::k60::Pin::Name::kPtc9
#define LIBSC_MOTOR1_DIR libbase::k60::Pin::Name::kPtc7

#define LIBSC_MAGSEN0 libbase::k60::Pin::Name::kPte26
#define LIBSC_MAGSEN1 libbase::k60::Pin::Name::kPte28
#define LIBSC_MAGSEN2 libbase::k60::Pin::Name::kPte27
#define LIBSC_MAGSEN3 libbase::k60::Pin::Name::kPte12
#define LIBSC_MAGSEN4 libbase::k60::Pin::Name::kPte24
#define LIBSC_MAGSEN5 libbase::k60::Pin::Name::kPte25

#define LIBSC_UART0_TX libbase::k60::Pin::Name::kPtd7
#define LIBSC_UART0_RX libbase::k60::Pin::Name::kPtd6

#define LIBSC_ENCODER0_QDA libbase::k60::Pin::Name::kPta8
#define LIBSC_ENCODER0_QDB libbase::k60::Pin::Name::kPta9
#define LIBSC_ENCODER1_QDA libbase::k60::Pin::Name::kPta10
#define LIBSC_ENCODER1_QDB libbase::k60::Pin::Name::kPta11

#define LIBSC_MPU6050_SCL libbase::k60::Pin::Name::kPtd8
#define LIBSC_MPU6050_SDA libbase::k60::Pin::Name::kPtd9

#define LIBSC_MMA8451Q0_SCL libbase::k60::Pin::Name::kPtc10
#define LIBSC_MMA8451Q0_SDA libbase::k60::Pin::Name::kPtc11
