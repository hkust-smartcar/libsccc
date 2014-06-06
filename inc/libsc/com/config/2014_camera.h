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
//#define LIBSC_USE_UART 1
// Support 2 encoders only when using FTM
//#define LIBSC_USE_ENCODER 2
//#define LIBSC_USE_ENCODER_FTM
#define LIBSC_USE_LCD 1
#define LIBSC_USE_LCD_HW_SPI
#define LIBSC_USE_LED 4
#define LIBSC_USE_MOTOR 2
#define LIBSC_USE_CAMERA 1
#define LIBSC_USE_JOYSTICK 1

#define LIBSC_USE_TEMPERATURE_SENSOR 1
#define LIBSC_TEMPERATURE0 PTC2

#define LIBSC_BT_UART UART3
#define LIBSC_BT_UART_BAUD 9600

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
#define LIBSC_MOTOR0_PWM PTD6
#define LIBSC_MOTOR0_DIR PTE6
#define LIBSC_MOTOR1_PWM PTD7
#define LIBSC_MOTOR1_DIR PTE7

#define LIBSC_CAMERA_SDA PTB17
#define LIBSC_CAMERA_SCL PTB16
#define LIBSC_CAMERA_PCLK PTB2
#define LIBSC_CAMERA_HREF PTB7
#define LIBSC_CAMERA_VSYNC PTB4
#define LIBSC_CAMERA_DMA_SRC_ADDR PTD8
#define LIBSC_CAMERA_DMA_CH 0

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

#define LIBSC_VR ADC1_SE4b

#define RXADC ADC0_SE17


#define LIBSC_JOYSTICK0_UP PTC6
#define LIBSC_JOYSTICK0_DOWN PTC5
#define LIBSC_JOYSTICK0_LEFT PTC8
#define LIBSC_JOYSTICK0_RIGHT PTC7
#define LIBSC_JOYSTICK0_SELECT PTC9

#define LIBSC_BUTTON0 PTC10

#endif /* LIBSC_2014_CAMERA_H_ */
