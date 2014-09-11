/*
 * inno.h
 * Config file for Innovation Carnival board
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

//#define LIBSC_USE_AMBIENT_LIGHT 1
//#define LIBSC_USE_CAMERA 1
#define LIBSC_USE_ENCODER 1
#define LIBSC_USE_SOFT_ENCODER
//#define LIBSC_USE_INFRA_RED_SENSOR 1
//#define LIBSC_USE_LCD 1
#define LIBSC_USE_LED 4
#define LIBSC_USE_LIGHT_SENSOR 1
//#define LIBSC_USE_LINEAR_CCD 1
#define LIBSC_USE_MOTOR 1
//#define LIBSC_USE_SERVO 1
//#define LIBSC_USE_TEMPERATURE_SENSOR 1
#define LIBSC_USE_UART 1

#define LIBSC_AMBIENT_LIGHT0 libbase::k60::Pin::Name::kPta8

#define LIBSC_CAMERA_SDA PTC7
#define LIBSC_CAMERA_SCL PTC6
#define LIBSC_CAMERA_PCLK PTC5
#define LIBSC_CAMERA_HREF PTA10
#define LIBSC_CAMERA_VSYNC PTC4
#define LIBSC_CAMERA_DMA_SRC_ADDR PTC8
#define LIBSC_CAMERA_DMA_CH 0

#define LIBSC_ENCODER0_QDA libbase::k60::Pin::Name::kPta6
#define LIBSC_ENCODER0_QDB libbase::k60::Pin::Name::kPtc4

#define LIBSC_INFRA_RED0 libbase::k60::Pin::Name::kPtc4

#define LIBSC_LCD_RST libbase::k60::Pin::Name::kPta8
#define LIBSC_LCD_DC libbase::k60::Pin::Name::kPtd7
#define LIBSC_LCD_CS libbase::k60::Pin::Name::kPte4
#define LIBSC_LCD_SDAT libbase::k60::Pin::Name::kPte5
#define LIBSC_LCD_SCLK libbase::k60::Pin::Name::kPta6

#define LIBSC_LED0 libbase::k60::Pin::Name::kPte24
#define LIBSC_LED1 libbase::k60::Pin::Name::kPte25
#define LIBSC_LED2 libbase::k60::Pin::Name::kPte26
#define LIBSC_LED3 libbase::k60::Pin::Name::kPte27

#define LIBSC_LIGHT_SENSOR0 libbase::k60::Pin::Name::kPta10

#define LIBSC_LINEAR_CCD0_AO libbase::k60::Pin::Name::kPtc4
#define LIBSC_LINEAR_CCD0_CLK libbase::k60::Pin::Name::kPtc6
#define LIBSC_LINEAR_CCD0_SI libbase::k60::Pin::Name::kPtc8

#define LIBSC_MOTOR0_PWM libbase::k60::Pin::Name::kPta8
#define LIBSC_MOTOR0_DIR libbase::k60::Pin::Name::kPtd7

#define LIBSC_SERVO0 libbase::k60::Pin::Name::kPta10

#define LIBSC_SYSTEM_TIMER_PIT_CHANNEL 0

#define LIBSC_TEMPERATURE0 libbase::k60::Pin::Name::kPta10

#define LIBSC_UART0_TX libbase::k60::Pin::Name::kPte4
#define LIBSC_UART0_RX libbase::k60::Pin::Name::kPte5
