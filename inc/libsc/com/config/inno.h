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
#define LIBSC_USE_ENCODER 1
//#define LIBSC_USE_LCD 1
#define LIBSC_USE_LED 4
#define LIBSC_USE_MOTOR 1
#define LIBSC_USE_SERVO 1
#define LIBSC_USE_LINEAR_CCD 1

#define LIBSC_BT_UART UART3
#define LIBSC_BT_UART_BAUD 115200
#define LIBSC_ENCODER0 PTA6
#define LIBSC_LCD_RST PTC4
#define LIBSC_LCD_DC PTC6
#define LIBSC_LCD_CS PTC8
#define LIBSC_LCD_SDAT PTC10
#define LIBSC_LCD_SCLK PTC12
#define LIBSC_LED0 PTE24
#define LIBSC_LED1 PTE25
#define LIBSC_LED2 PTE26
#define LIBSC_LED3 PTE27
#define LIBSC_MOTOR0_PWM PTA8
#define LIBSC_MOTOR0_DIR PTD7
#define LIBSC_SERVO0 PTA10
#define LIBSC_LINEAR_CCD0_AO PTC4
#define LIBSC_LINEAR_CCD0_CLK PTC6
#define LIBSC_LINEAR_CCD0_SI PTC8

#endif /* LIBSC_CONFIG_INNO_H_ */
