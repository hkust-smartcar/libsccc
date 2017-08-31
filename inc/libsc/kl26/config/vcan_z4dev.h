/*
 * vcan_z4dev.h
 * Config file for the VCAN KL26 development board
 *
 * Author: Harrison Ng
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#define LIBSC_USE_BUTTON 1
#define LIBSC_USE_ENCODER 2
#define LIBSC_USE_LCD 1
#define LIBSC_USE_LED 4
#define LIBSC_USE_LINEAR_CCD 1
#define LIBSC_USE_MOTOR 1
#define LIBSC_USE_MPU6050 1
#define LIBSC_USE_SERVO 1
#define LIBSC_USE_UART 1
#define LIBSC_USE_US100 1

#define LIBSC_BUTTON0 libbase::kl26::Pin::Name::kPta20

#define LIBSC_ENCODER0_QDA libbase::kl26::Pin::Name::kPtc12
#define LIBSC_ENCODER0_QDB libbase::kl26::Pin::Name::kPtc18
#define LIBSC_ENCODER0_TPM_MODULE 1
#define LIBSC_ENCODER1_QDA libbase::kl26::Pin::Name::kPta12
#define LIBSC_ENCODER1_QDB libbase::kl26::Pin::Name::kPta13
#define LIBSC_ENCODER1_TPM_MODULE 2

#define LIBSC_LED0 libbase::kl26::Pin::Name::kPtd4
#define LIBSC_LED1 libbase::kl26::Pin::Name::kPtd5
#define LIBSC_LED2 libbase::kl26::Pin::Name::kPtd6
#define LIBSC_LED3 libbase::kl26::Pin::Name::kPtd7

#define LIBSC_LINEAR_CCD0_CLK libbase::kl26::Pin::Name::kPte18
#define LIBSC_LINEAR_CCD0_SI libbase::kl26::Pin::Name::kPte17
#define LIBSC_LINEAR_CCD0_AD libbase::kl26::Pin::Name::kPte16

//#define LIBSC_MOTOR0_PWM libbase::kl26::Pin::Name::kPte20
//#define LIBSC_MOTOR0_DIR libbase::kl26::Pin::Name::kPtd3

#define LIBSC_MOTOR0_PWMA libbase::kl26::Pin::Name::kPte20
#define LIBSC_MOTOR0_PWMB libbase::kl26::Pin::Name::kPte21

#define LIBSC_MPU6050_SCL libbase::kl26::Pin::Name::kPtb0
#define LIBSC_MPU6050_SDA libbase::kl26::Pin::Name::kPtb1

#define LIBSC_SERVO0 libbase::kl26::Pin::Name::kPte20

#define LIBSC_ST7735R_RST  libbase::kl26::Pin::Name::kPtc8
#define LIBSC_ST7735R_DC  libbase::kl26::Pin::Name::kPtc7
#define LIBSC_ST7735R_CS libbase::kl26::Pin::Name::kPtc4
#define LIBSC_ST7735R_SDAT  libbase::kl26::Pin::Name::kPtc6
#define LIBSC_ST7735R_SCLK  libbase::kl26::Pin::Name::kPtc5

#define LIBSC_UART0_TX libbase::kl26::Pin::Name::kPte0
#define LIBSC_UART0_RX libbase::kl26::Pin::Name::kPte1

#define LIBSC_US1000_TX libbase::kl26::Pin::Name::kPte0
#define LIBSC_US1000_RX libbase::kl26::Pin::Name::kPte1
