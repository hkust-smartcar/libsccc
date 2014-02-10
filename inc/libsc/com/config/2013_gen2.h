/*
 * 2013_gen2.h
 * Config file for 2013 Gen 2/3 board
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_2013_GEN2_H_
#define LIBSC_2013_GEN2_H_

#define LIBSC_USE_BT 1
#define LIBSC_USE_ENCODER 2
#define LIBSC_USE_LCD 1
#define LIBSC_USE_LCD_HW_SPI
#define LIBSC_USE_LED 4
#define LIBSC_USE_MOTOR 2
#define LIBSC_USE_SERVO 1

#define LIBSC_BT_UART UART3
#define LIBSC_ENCODER0 PTA6
#define LIBSC_ENCODER1 PTA7

#ifdef LIBSC_USE_LCD_HW_SPI
#define LIBSC_LCD_RST PTA13
#define LIBSC_LCD_DC PTB19
#define LIBSC_LCD_CS PTB20
// Connect SOUT pin with SDAT
#define LIBSC_LCD_MOSI PTB22
// MISO pin is not connected but is needed to get the SPI info
#define LIBSC_LCD_MISO PTB23
#define LIBSC_LCD_SCLK PTB21

#else
#define LIBSC_LCD_RST PTA13
#define LIBSC_LCD_DC PTB19
#define LIBSC_LCD_CS PTB20
#define LIBSC_LCD_SDAT PTB22
#define LIBSC_LCD_SCLK PTB21

#endif

#define LIBSC_LED0 PTE24
#define LIBSC_LED1 PTE25
#define LIBSC_LED2 PTE26
#define LIBSC_LED3 PTE27
#define LIBSC_MOTOR0_PWM PTA8
#define LIBSC_MOTOR0_DIR PTD7
#define LIBSC_MOTOR1_PWM PTA9
#define LIBSC_MOTOR1_DIR PTD9
#define LIBSC_SERVO0 PTA10

#endif /* LIBSC_2013_GEN2_H_ */
