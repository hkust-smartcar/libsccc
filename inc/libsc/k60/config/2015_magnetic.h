/*
 * 2015_magnetic.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#define LIBSC_USE_BATTERY_METER 1
#define LIBSC_USE_BUTTON 2
#define LIBSC_USE_SWITCH 8
#define LIBSC_USE_BUZZER 1
#define LIBSC_USE_JOYSTICK 1
#define LIBSC_USE_LCD 1
#define LIBSC_USE_LED 4
#define LIBSC_USE_LINEAR_CCD 1
#define LIBSC_USE_US100 1
#define LIBSC_USE_I2C 1
#define LIBSC_USE_MOTOR 1
#define LIBSC_USE_MAGSEN 6
#define LIBSC_USE_SERVO 1
#define LIBSC_USE_UART 1
#define LIBSC_USE_ENCODER 1
#define LIBSC_USE_SPI 1
#define LIBSC_USE_YUANYANG 1

#define LIBSC_BATTERY_METER libbase::k60::Adc::Name::kAdc0Ad5B

#define LIBSC_BUTTON0 libbase::k60::Pin::Name::kPte26
#define LIBSC_BUTTON1 libbase::k60::Pin::Name::kPte27

#define LIBSC_SWITCH0 libbase::k60::Pin::Name::kPte28
#define LIBSC_SWITCH1 libbase::k60::Pin::Name::kPta5
#define LIBSC_SWITCH2 libbase::k60::Pin::Name::kPta6
#define LIBSC_SWITCH3 libbase::k60::Pin::Name::kPta7
#define LIBSC_SWITCH4 libbase::k60::Pin::Name::kPta8
#define LIBSC_SWITCH5 libbase::k60::Pin::Name::kPta9
#define LIBSC_SWITCH6 libbase::k60::Pin::Name::kPta10
#define LIBSC_SWITCH7 libbase::k60::Pin::Name::kPta11

#define LIBSC_BUZZER0 libbase::k60::Pin::Name::kPtd4
#define LIBSC_BUZZER_ACIVE_LEVEL 0

#define LIBSC_JOYSTICK0_UP libbase::k60::Pin::Name::kPte8
#define LIBSC_JOYSTICK0_DOWN libbase::k60::Pin::Name::kPte7
#define LIBSC_JOYSTICK0_LEFT libbase::k60::Pin::Name::kPte6
#define LIBSC_JOYSTICK0_RIGHT libbase::k60::Pin::Name::kPte10
#define LIBSC_JOYSTICK0_SELECT libbase::k60::Pin::Name::kPte9

#define LIBSC_ST7735R_RST libbase::k60::Pin::Name::kPte3
#define LIBSC_ST7735R_DC libbase::k60::Pin::Name::kPte0
#define LIBSC_ST7735R_CS libbase::k60::Pin::Name::kPte4
#define LIBSC_ST7735R_SDAT libbase::k60::Pin::Name::kPte1
#define LIBSC_ST7735R_SCLK libbase::k60::Pin::Name::kPte2

#define LIBSC_US1000_TX libbase::k60::Pin::Name::kPtd9;
#define LIBSC_US1000_RX libbase::k60::Pin::Name::kPtd8;

#define LIBSC_LED0 libbase::k60::Pin::Name::kPtc4
#define LIBSC_LED1 libbase::k60::Pin::Name::kPtc5
#define LIBSC_LED2 libbase::k60::Pin::Name::kPtc6
#define LIBSC_LED3 libbase::k60::Pin::Name::kPtc7

#define LIBSC_LINEAR_CCD0_SI libbase::k60::Pin::Name::kPtd7
#define LIBSC_LINEAR_CCD0_CLK libbase::k60::Pin::Name::kPtd6
#define LIBSC_LINEAR_CCD0_AD libbase::k60::Pin::Name::kPtd5

#define LIBSC_I2C0_SCL libbase::k60::Pin::Name::kPtd8
#define LIBSC_I2C0_SDA libbase::k60::Pin::Name::kPtd9

#define LIBSC_MOTOR0_DIRMOTOR 1
//#define LIBSC_MOTOR0_ALTERMOTOR 1
#define LIBSC_MOTOR0_PWM libbase::k60::Pin::Name::kPtc8
#define LIBSC_MOTOR0_DIR libbase::k60::Pin::Name::kPtc9
//#define LIBSC_MOTOR0_SPARE_PWM libbase::k60::Pin::Name::kPtc9
//#define LIBSC_MOTOR0_SPARE_DIR libbase::k60::Pin::Name::kPtc13

#define LIBSC_MAGSEN0 libbase::k60::Pin::Name::kPta26
#define LIBSC_MAGSEN1 libbase::k60::Pin::Name::kPta27
#define LIBSC_MAGSEN2 libbase::k60::Pin::Name::kPta28
#define LIBSC_MAGSEN3 libbase::k60::Pin::Name::kPta29
#define LIBSC_MAGSEN4 libbase::k60::Pin::Name::kPtb0
#define LIBSC_MAGSEN5 libbase::k60::Pin::Name::kPtb1

#define LIBSC_ENCODER0_QDA libbase::k60::Pin::Name::kPta12
#define LIBSC_ENCODER0_QDB libbase::k60::Pin::Name::kPta13

#define LIBSC_SERVO0 libbase::k60::Pin::Name::kPtb18

#define LIBSC_SPI0_PCS0 libbase::k60::Pin::Name::kPtd11
#define LIBSC_SPI0_PCS1 libbase::k60::Pin::Name::kPtd15
#define LIBSC_SPI0_SCK libbase::k60::Pin::Name::kPtd12
#define LIBSC_SPI0_SOUT libbase::k60::Pin::Name::kPtd13
#define LIBSC_SPI0_SIN libbase::k60::Pin::Name::kPtd14

#define LIBSC_UART0_TX libbase::k60::Pin::Name::kPta14
#define LIBSC_UART0_RX libbase::k60::Pin::Name::kPta15

#define LIBSC_YUANYANG_DO libbase::k60::Pin::Name::kPtc13;
#define LIBSC_YUANYANG_STATE libbase::k60::Pin::Name::kPtc12;
