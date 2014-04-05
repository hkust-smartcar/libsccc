/*
 * 2013_magnetic.h
 * Config file for 2013 Gen 2/3 board
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_2013_MAGNETIC_H_
#define LIBSC_2013_MAGNETIC_H_

/*
 * The number should represent the number of devices available, e.g.,
 * define LIBSC_USE_UART to 2 when there are two UART devices connected. Comment
 * out the specific defines instead of defining them to 0 for unused devices
 */
#define LIBSC_USE_UART 1
//#define LIBSC_USE_ENCODER 1
//#define LIBSC_USE_ENCODER_FTM
//#define LIBSC_USE_LCD 1
//#define LIBSC_USE_LCD_HW_SPI
#define LIBSC_USE_LED 4
#define LIBSC_USE_MOTOR 2
#define LIBSC_USE_SERVO 1

#define LIBSC_BT_UART UART3
#define LIBSC_BT_UART_BAUD 115200

#ifdef LIBSC_USE_ENCODER_FTM
#define LIBSC_ENCODER0_OUT PTB18
#define LIBSC_ENCODER0_VCC PTB19

#else
#define LIBSC_ENCODER0 PTA6
#define LIBSC_ENCODER1 PTA7

#endif

#ifdef LIBSC_USE_LCD_HW_SPI
#define LIBSC_LCD_RST PTA13
#define LIBSC_LCD_DC PTA17
#define LIBSC_LCD_CS PTA14
// Connect SOUT pin with SDAT
#define LIBSC_LCD_MOSI PTA16
// MISO pin is not used (one way transfer) but is needed to get the SPI info
#define LIBSC_LCD_MISO PTA17
#define LIBSC_LCD_SCLK PTA15

#else
#define LIBSC_LCD_RST PTA13
#define LIBSC_LCD_DC PTA17
#define LIBSC_LCD_CS PTA14
#define LIBSC_LCD_SDAT PTA16
#define LIBSC_LCD_SCLK PTA15

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

#endif /* LIBSC_2013_MAGNETIC_H_ */
