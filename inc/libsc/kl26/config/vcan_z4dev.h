/*
 * vcan_z4dev.h
 * Config file for the VCAN KL26 development board
 *
 * Author: Harrison Ng
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#define LIBSC_USE_BUTTON 1
#define LIBSC_USE_LED 4
#define LIBSC_USE_MOTOR 1
#define LIBSC_USE_SERVO 1
//#define LIBSC_USE_UART 1

#define LIBSC_BUTTON0 libbase::kl26::Pin::Name::kPta20

#define LIBSC_LED0 libbase::kl26::Pin::Name::kPtd4
#define LIBSC_LED1 libbase::kl26::Pin::Name::kPtd5
#define LIBSC_LED2 libbase::kl26::Pin::Name::kPtd6
#define LIBSC_LED3 libbase::kl26::Pin::Name::kPtd7

//#define LIBSC_MOTOR0_PWM libbase::kl26::Pin::Name::kPte20
//#define LIBSC_MOTOR0_DIR libbase::kl26::Pin::Name::kPtd3

#define LIBSC_MOTOR0_PWMA libbase::kl26::Pin::Name::kPte20
#define LIBSC_MOTOR0_PWMB libbase::kl26::Pin::Name::kPte21

#define LIBSC_SERVO0 libbase::kl26::Pin::Name::kPte20

//#define LIBSC_UART0_TX libbase::kl26::Pin::Name::kPta14
//#define LIBSC_UART0_RX libbase::kl26::Pin::Name::kPta15
