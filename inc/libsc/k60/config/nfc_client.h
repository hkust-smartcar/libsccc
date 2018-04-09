/*
 * vcan_fx15dev.h
 * Config file for the VCAN K60FX15 development board
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#define LIBSC_USE_BUTTON 1
#define LIBSC_USE_LCD 1
#define LIBSC_USE_LED 1
#define LIBSC_USE_MOTOR 1
#define LIBSC_USE_MPU6050 1
#define LIBSC_USE_OV7725 1
#define LIBSC_USE_OV7725_FIFO 1
#define LIBSC_USE_SERVO 1
#define LIBSC_USE_UART 2

#define LIBSC_BUTTON0 libbase::k60::Pin::Name::kPtd7

#define LIBSC_LED0 libbase::k60::Pin::Name::kPtb20
#define LIBSC_LED1 libbase::k60::Pin::Name::kPtb21
#define LIBSC_LED2 libbase::k60::Pin::Name::kPtb22
#define LIBSC_LED3 libbase::k60::Pin::Name::kPtb23

#define LIBSC_MOTOR0_PWMA libbase::k60::Pin::Name::kPtc1
#define LIBSC_MOTOR0_PWMB libbase::k60::Pin::Name::kPtc2
#define LIBSC_MOTOR0_DEADTIME 1000
#define LIBSC_ALTERNATE_MOTOR_CW_PWM 1

#define LIBSC_MPU6050_SCL libbase::k60::Pin::Name::kPtb0
#define LIBSC_MPU6050_SDA libbase::k60::Pin::Name::kPtb1

#define LIBSC_OV77250_SCL libbase::k60::Pin::Name::kPtc16
#define LIBSC_OV77250_SDA libbase::k60::Pin::Name::kPtc17
// Must be byte-aligned
#define LIBSC_OV77250_DATA0 libbase::k60::Pin::Name::kPtc8
#define LIBSC_OV77250_PCLK libbase::k60::Pin::Name::kPtb9
#define LIBSC_OV77250_VSYNC libbase::k60::Pin::Name::kPta29
#define LIBSC_OV77250_DMA_CH 1

#define LIBSC_OV7725_FIFO0_SCL libbase::k60::Pin::Name::kPtc16
#define LIBSC_OV7725_FIFO0_SDA libbase::k60::Pin::Name::kPtc17
// Must be byte-aligned
#define LIBSC_OV7725_FIFO0_DATA0 libbase::k60::Pin::Name::kPtc8
#define LIBSC_OV7725_FIFO0_VSYNC libbase::k60::Pin::Name::kPta29
#define LIBSC_OV7725_FIFO0_WEN libbase::k60::Pin::Name::kPtb5
#define LIBSC_OV7725_FIFO0_WRST libbase::k60::Pin::Name::kPtb0
#define LIBSC_OV7725_FIFO0_RCLK libbase::k60::Pin::Name::kPtd5
#define LIBSC_OV7725_FIFO0_RRST libbase::k60::Pin::Name::kPtb2

#define LIBSC_SERVO0 libbase::k60::Pin::Name::kPtd0

#define LIBSC_ST7735R_RST libbase::k60::Pin::Name::kPta13
#define LIBSC_ST7735R_DC libbase::k60::Pin::Name::kPta17
#define LIBSC_ST7735R_CS libbase::k60::Pin::Name::kPta14
#define LIBSC_ST7735R_SDAT libbase::k60::Pin::Name::kPta16
#define LIBSC_ST7735R_SCLK libbase::k60::Pin::Name::kPta15

#define LIBSC_UART0_TX libbase::k60::Pin::Name::kPte4
#define LIBSC_UART0_RX libbase::k60::Pin::Name::kPte5

#define LIBSC_UART1_TX libbase::k60::Pin::Name::kPte8
#define LIBSC_UART1_RX libbase::k60::Pin::Name::kPte9
