/*
 * 2015_m2.h
 * Config file for the 2015 Magnetic Team 2 KL26 board
 *
 * Author: Harrison Ng
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once
//#define LIBSC_USE_SOFT_ST7735R 1
#define LIBSC_USE_JOYSTICK 1
#define LIBSC_USE_ENCODER 1
#define LIBSC_USE_LED 4
#define LIBSC_USE_MOTOR 1
#define LIBSC_USE_SERVO 1
#define LIBSC_USE_UART 2
//#define LIBSC_USE_LCD 1
#define LIBSC_USE_BATTERY_METER 1

#define LIBSC_JOYSTICK0_UP libbase::kl26::Pin::Name::kPta5
#define LIBSC_JOYSTICK0_DOWN libbase::kl26::Pin::Name::kPta6
#define LIBSC_JOYSTICK0_LEFT libbase::kl26::Pin::Name::kPta7
#define LIBSC_JOYSTICK0_RIGHT libbase::kl26::Pin::Name::kPta12
#define LIBSC_JOYSTICK0_SELECT libbase::kl26::Pin::Name::kPta13

#define LIBSC_ENCODER0_QDA libbase::kl26::Pin::Name::kPte30
#define LIBSC_ENCODER0_QDB libbase::kl26::Pin::Name::kPtc18
#define LIBSC_ENCODER0_TPM_MODULE 0

#define LIBSC_UART0_TX libbase::kl26::Pin::Name::kPta14
#define LIBSC_UART0_RX libbase::kl26::Pin::Name::kPta15

#define LIBSC_UART1_TX libbase::kl26::Pin::Name::kPte0
#define LIBSC_UART1_RX libbase::kl26::Pin::Name::kPte1

#define LIBSC_ST7735R_RST  libbase::kl26::Pin::Name::kPtd3 //DIN
#define LIBSC_ST7735R_DC  libbase::kl26::Pin::Name::kPtc18 //CS0
#define LIBSC_ST7735R_CS  liwbbase::kl26::Pin::Name::kPtd0 //CS2
#define LIBSC_ST7735R_SDAT  libbase::kl26::Pin::Name::kPtd2 //DOUT
#define LIBSC_ST7735R_SCLK  libbase::kl26::Pin::Name::kPtd1 //CLK

#define LIBSC_LED0 libbase::kl26::Pin::Name::kPtc10
#define LIBSC_LED1 libbase::kl26::Pin::Name::kPtc11
#define LIBSC_LED2 libbase::kl26::Pin::Name::kPtc12
#define LIBSC_LED3 libbase::kl26::Pin::Name::kPtc13

#define LIBSC_MOTOR0_PWM libbase::kl26::Pin::Name::kPtb1
#define LIBSC_MOTOR0_DIR libbase::kl26::Pin::Name::kPtb0

#define LIBSC_SERVO0 libbase::kl26::Pin::Name::kPtb2

#define LIBSC_BATTERY_METER libbase::kl26::Adc::Name::kAdc0DAd3


