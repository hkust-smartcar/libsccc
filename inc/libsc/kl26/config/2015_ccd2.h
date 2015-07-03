/*
 * 2015_ccd2.h
 * Config file for the 2015 CCD2 Gen2 board
 *
 * Author: Harrison Ng
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#define LIBSC_USE_LED 4
#define LIBSC_USE_UART 1
#define LIBSC_USE_JOYSTICK 1
#define LIBSC_USE_BUTTON 3
#define LIBSC_USE_MOTOR 2
#define LIBSC_USE_ENCODER 2
#define LIBSC_USE_MPU6050 1
//#define LIBSC_USE_SOFT_MPU6050 0
#define LIBSC_USE_MMA8451Q 1
#define LIBSC_USE_LINEAR_CCD 2
#define LIBSC_USE_BATTERY_METER 1
#define LIBSC_USE_LCD 1
//#define LIBSC_USE_SOFT_ST7735R 1
//#define LIBSC_USE_BUZZER 1

#define LIBSC_MOTOR0_PWM libbase::kl26::Pin::Name::kPta5
#define LIBSC_MOTOR0_DIR libbase::kl26::Pin::Name::kPta6

#define LIBSC_MOTOR1_PWM libbase::kl26::Pin::Name::kPta7
#define LIBSC_MOTOR1_DIR libbase::kl26::Pin::Name::kPta12

#define LIBSC_ENCODER0_QDA libbase::kl26::Pin::Name::kPtc12
#define LIBSC_ENCODER0_QDB libbase::kl26::Pin::Name::kPtc16
#define LIBSC_ENCODER0_TPM_MODULE 1

#define LIBSC_ENCODER1_QDA libbase::kl26::Pin::Name::kPtc13
#define LIBSC_ENCODER1_QDB libbase::kl26::Pin::Name::kPtc17
#define LIBSC_ENCODER1_TPM_MODULE 2

#define LIBSC_JOYSTICK0_UP libbase::kl26::Pin::Name::kPtd0
#define LIBSC_JOYSTICK0_LEFT libbase::kl26::Pin::Name::kPtd1
#define LIBSC_JOYSTICK0_DOWN libbase::kl26::Pin::Name::kPtd2
#define LIBSC_JOYSTICK0_SELECT libbase::kl26::Pin::Name::kPtd3
#define LIBSC_JOYSTICK0_RIGHT libbase::kl26::Pin::Name::kPtd4

#define LIBSC_BUTTON0 libbase::kl26::Pin::Name::kPtc9
#define LIBSC_BUTTON1 libbase::kl26::Pin::Name::kPtc10
#define LIBSC_BUTTON2 libbase::kl26::Pin::Name::kPtc11

#define LIBSC_ST7735R_RST  libbase::kl26::Pin::Name::kPtc8
#define LIBSC_ST7735R_DC  libbase::kl26::Pin::Name::kPtc7
#define LIBSC_ST7735R_CS libbase::kl26::Pin::Name::kPtc4
#define LIBSC_ST7735R_SDAT  libbase::kl26::Pin::Name::kPtc6
#define LIBSC_ST7735R_SCLK  libbase::kl26::Pin::Name::kPtc5

#define LIBSC_LED0 libbase::kl26::Pin::Name::kPtb20
#define LIBSC_LED1 libbase::kl26::Pin::Name::kPtb21
#define LIBSC_LED2 libbase::kl26::Pin::Name::kPtb22
#define LIBSC_LED3 libbase::kl26::Pin::Name::kPtb23

#define LIBSC_UART0_TX libbase::kl26::Pin::Name::kPta14
#define LIBSC_UART0_RX libbase::kl26::Pin::Name::kPta15

#define LIBSC_MPU6050_SCL libbase::kl26::Pin::Name::kPte24
#define LIBSC_MPU6050_SDA libbase::kl26::Pin::Name::kPte25

#define LIBSC_MMA8451Q0_SCL libbase::kl26::Pin::Name::kPte1
#define LIBSC_MMA8451Q0_SDA libbase::kl26::Pin::Name::kPte0

#define LIBSC_LINEAR_CCD0_CLK libbase::kl26::Pin::Name::kPte18
#define LIBSC_LINEAR_CCD0_SI libbase::kl26::Pin::Name::kPte17
#define LIBSC_LINEAR_CCD0_AD libbase::kl26::Pin::Name::kPte16

#define LIBSC_LINEAR_CCD1_CLK libbase::kl26::Pin::Name::kPte21
#define LIBSC_LINEAR_CCD1_SI libbase::kl26::Pin::Name::kPte20
#define LIBSC_LINEAR_CCD1_AD libbase::kl26::Pin::Name::kPte19

#define LIBSC_BATTERY_METER libbase::kl26::Adc::Name::kAdc0DAd3

#define LIBSC_BUZZER0 libbase::kl26::Pin::Name::kPta13;
