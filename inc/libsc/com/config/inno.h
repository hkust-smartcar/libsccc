/*
 * inno.h
 * Config file for Innovation Carnival board
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_CONFIG_INNO_H_
#define LIBSC_CONFIG_INNO_H_

#define LIBSC_USE_UART 1
//#define LIBSC_USE_CAMERA 1
//#define LIBSC_USE_ENCODER 1
//#define LIBSC_USE_LCD 1
#define LIBSC_USE_LED 4
//#define LIBSC_USE_MOTOR 1
//#define LIBSC_USE_SERVO 1
//#define LIBSC_USE_TEMPERATURE_SENSOR 1
//#define LIBSC_USE_LINEAR_CCD 1
//#define LIBSC_USE_INFRA_RED_SENSOR 1

#define LIBSC_BT_UART UART3
#define LIBSC_BT_UART_BAUD 115200

#define LIBSC_CAMERA_SDA PTC7
#define LIBSC_CAMERA_SCL PTC6
#define LIBSC_CAMERA_PCLK PTC5
#define LIBSC_CAMERA_HREF PTA10
#define LIBSC_CAMERA_VSYNC PTC4
#define LIBSC_CAMERA_DMA_SRC_ADDR PTC8
#define LIBSC_CAMERA_DMA_CH 0

#define LIBSC_ENCODER0 PTA6

#define LIBSC_INFRA_RED0 libbase::k60::PinConfig::Name::PTC4

#define LIBSC_LCD_RST PTA8
#define LIBSC_LCD_DC PTD7
#define LIBSC_LCD_CS PTE4
#define LIBSC_LCD_SDAT PTE5
#define LIBSC_LCD_SCLK PTA6

#define LIBSC_LED0 libbase::k60::PinConfig::Name::PTE24
#define LIBSC_LED1 libbase::k60::PinConfig::Name::PTE25
#define LIBSC_LED2 libbase::k60::PinConfig::Name::PTE26
#define LIBSC_LED3 libbase::k60::PinConfig::Name::PTE27

#define LIBSC_LINEAR_CCD0_AO PTC4
#define LIBSC_LINEAR_CCD0_CLK PTC6
#define LIBSC_LINEAR_CCD0_SI PTC8

#define LIBSC_MOTOR0_PWM PTA8
#define LIBSC_MOTOR0_DIR PTD7

#define LIBSC_SERVO0 PTA10

#define LIBSC_TEMPERATURE0 PTA10

#define LIBSC_UART0_TX libbase::k60::PinConfig::Name::PTE4
#define LIBSC_UART0_RX libbase::k60::PinConfig::Name::PTE5

#endif /* LIBSC_CONFIG_INNO_H_ */
