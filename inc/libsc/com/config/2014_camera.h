/*
 * 2014_camera.h
 * Config file for 2014 Gen 1 board (camera car)
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_2014_CAMERA_H_
#define LIBSC_2014_CAMERA_H_

/*
 * The number should represent the number of devices available, e.g.,
 * define LIBSC_USE_UART to 2 when there are two UART devices connected. Comment
 * out the specific defines instead of defining them to 0 for unused devices
 */
#define LIBSC_USE_UART 1
// Support 2 encoders only when using FTM
#define LIBSC_USE_ENCODER 2
#define LIBSC_USE_ENCODER_FTM
#define LIBSC_USE_LCD 1
//#define LIBSC_USE_LCD_HW_SPI
#define LIBSC_USE_LED 4
#define LIBSC_USE_MOTOR 1
#define LIBSC_USE_CAMERA 1

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
#define LIBSC_MOTOR1_PWM PTA9
#define LIBSC_MOTOR1_DIR PTD9

#define LIBSC_CAMERA_SDA PTB11
#define LIBSC_CAMERA_SCL PTB20
#define LIBSC_CAMERA_VSYNC PTA29

#define LIBSC_LCD_RST PTB2
#define LIBSC_LCD_DC PTB3
#define LIBSC_LCD_CS PTB4
#define LIBSC_LCD_SDAT PTB5
#define LIBSC_LCD_SCLK PTB6

#endif /* LIBSC_2014_CAMERA_H_ */
