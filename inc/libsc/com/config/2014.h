/*
 * 2014.h
 * Config file for 2014 Gen 1 board
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_2014_H_
#define LIBSC_2014_H_

/*
 * The number should represent the number of devices available, e.g.,
 * define LIBSC_USE_UART to 2 when there are two UART devices connected. Comment
 * out the specific defines instead of defining them to 0 for unused devices
 */
#define LIBSC_USE_UART 1
// Support 2 encoders only when using FTM
#define LIBSC_USE_ENCODER 2
#define LIBSC_USE_ENCODER_FTM
//#define LIBSC_USE_LCD 1
//#define LIBSC_USE_LCD_HW_SPI
#define LIBSC_USE_LED 4
#define LIBSC_USE_MOTOR 1
#define LIBSC_USE_SERVO 1
#define LIBSC_USE_LINEAR_CCD 1

#define LIBSC_BT_UART UART3
#define LIBSC_BT_UART_BAUD 115200

#ifdef LIBSC_USE_ENCODER_FTM
#define LIBSC_ENCODER0_OUT PTB0
#define LIBSC_ENCODER0_VCC PTB1
#define LIBSC_ENCODER1_OUT PTB18
#define LIBSC_ENCODER1_VCC PTB19

#else
#define LIBSC_ENCODER0 PTB18

#endif

#define LIBSC_LED0 PTE9
#define LIBSC_LED1 PTE10
#define LIBSC_LED2 PTE11
#define LIBSC_LED3 PTE12
#define LIBSC_MOTOR0_PWM PTA8
#define LIBSC_MOTOR0_DIR PTD7
#define LIBSC_SERVO0 PTC2
#define LIBSC_LINEAR_CCD0_AO PTB10
#define LIBSC_LINEAR_CCD0_CLK PTB9
#define LIBSC_LINEAR_CCD0_SI PTB8

#endif /* LIBSC_2014_H_ */
