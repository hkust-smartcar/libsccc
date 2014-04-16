/*
 * 2013_ccd.h
 * Config file for 2013 Gen 2/3 board
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_2013_CCD_H_
#define LIBSC_2013_CCD_H_

/*
 * The number should represent the number of devices available, e.g.,
 * define LIBSC_USE_UART to 2 when there are two UART devices connected. Comment
 * out the specific defines instead of defining them to 0 for unused devices
 */
#define LIBSC_USE_UART 1
#define LIBSC_USE_BUTTON 4
#define LIBSC_USE_ENCODER 1
#define LIBSC_USE_ENCODER_FTM
#define LIBSC_USE_GYRO 1
#define LIBSC_USE_LCD 1
//#define LIBSC_USE_LCD_HW_SPI
#define LIBSC_USE_LED 4
#define LIBSC_USE_MOTOR 1
#define LIBSC_USE_SERVO 1
//#define LIBSC_USE_LIGHT_SENSOR 2
#define LIBSC_USE_LINEAR_CCD 1

#define LIBSC_BT_UART UART3
#define LIBSC_BT_UART_BAUD 115200

#define LIBSC_BUTTON0 PTE6
#define LIBSC_BUTTON1 PTE7
#define LIBSC_BUTTON2 PTE8
#define LIBSC_BUTTON3 PTE9

#ifdef LIBSC_USE_ENCODER_FTM
#define LIBSC_ENCODER0_QDA PTA10
#define LIBSC_ENCODER0_QDB PTA11

#else
#define LIBSC_ENCODER0 PTA10
#define LIBSC_ENCODER1 PTA6

#endif

#define LIBSC_GYRO0_ANGULAR PTC1

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
#define LIBSC_LCD_RST PTC14
#define LIBSC_LCD_DC PTC13
#define LIBSC_LCD_CS PTC3
#define LIBSC_LCD_SDAT PTC7
#define LIBSC_LCD_SCLK PTC6

#endif

#define LIBSC_LED0 PTE24
#define LIBSC_LED1 PTE25
#define LIBSC_LED2 PTE26
#define LIBSC_LED3 PTE27

#define LIBSC_MOTOR0_PWM PTA9
#define LIBSC_MOTOR0_DIR PTD9
//#define LIBSC_MOTOR1_PWM PTA9
//#define LIBSC_MOTOR1_DIR PTD9
#define LIBSC_SERVO0 PTA7
#define LIBSC_LINEAR_CCD0_AO PTB10
#define LIBSC_LINEAR_CCD0_CLK PTB9
#define LIBSC_LINEAR_CCD0_SI PTB8

#define LIBSC_LIGHT_SENSOR0 PTC7
#define LIBSC_LIGHT_SENSOR1 PTC13

#endif /* LIBSC_2013_CCD_H_ */
