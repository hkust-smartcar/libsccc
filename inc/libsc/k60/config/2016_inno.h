#pragma once

#define LIBSC_USE_BATTERY_METER 1
#define LIBSC_USE_BUTTON 2
#define LIBSC_USE_BUZZER 1
#define LIBSC_USE_ENCODER 2
#define LIBSC_USE_JOYSTICK 1
#define LIBSC_USE_LCD 1
#define LIBSC_USE_LED 4
#define LIBSC_USE_MOTOR 2
#define LIBSC_USE_LINEAR_CCD 2
#define LIBSC_USE_OV7725 1
#define LIBSC_USE_SERVO 1
#define LIBSC_USE_UART 1

#define LIBSC_BATTERY_METER libbase::k60::Adc::Name::kAdc3Ad6A

#define LIBSC_BUTTON0 libbase::k60::Pin::Name::kPte7
#define LIBSC_BUTTON1 libbase::k60::Pin::Name::kPtc12

#define LIBSC_BUZZER0 libbase::k60::Pin::Name::kPta8
//#define LIBSC_BUZZER_ACIVE_LEVEL 0

#define LIBSC_ENCODER0_QDA libbase::k60::Pin::Name::kPta10
#define LIBSC_ENCODER0_QDB libbase::k60::Pin::Name::kPta11
#define LIBSC_ENCODER1_QDA libbase::k60::Pin::Name::kPta12
#define LIBSC_ENCODER1_QDB libbase::k60::Pin::Name::kPta13

#define LIBSC_JOYSTICK0_UP libbase::k60::Pin::Name::kPtc8
#define LIBSC_JOYSTICK0_DOWN libbase::k60::Pin::Name::kPtc4
#define LIBSC_JOYSTICK0_LEFT libbase::k60::Pin::Name::kPtc5
#define LIBSC_JOYSTICK0_RIGHT libbase::k60::Pin::Name::kPtc6
#define LIBSC_JOYSTICK0_SELECT libbase::k60::Pin::Name::kPtc7

#define LIBSC_ST7735R_RST libbase::k60::Pin::Name::kPte3
#define LIBSC_ST7735R_DC libbase::k60::Pin::Name::kPte0
#define LIBSC_ST7735R_CS libbase::k60::Pin::Name::kPte4
#define LIBSC_ST7735R_SDAT libbase::k60::Pin::Name::kPte1
#define LIBSC_ST7735R_SCLK libbase::k60::Pin::Name::kPte2

#define LIBSC_LED0 libbase::k60::Pin::Name::kPte12
#define LIBSC_LED1 libbase::k60::Pin::Name::kPte11
#define LIBSC_LED2 libbase::k60::Pin::Name::kPte10
#define LIBSC_LED3 libbase::k60::Pin::Name::kPte9

//#define LIBSC_MOTOR0_PWM libbase::k60::Pin::Name::kPtd0
//#define LIBSC_MOTOR0_DIR libbase::k60::Pin::Name::kPtd1
//#define LIBSC_MOTOR1_PWM libbase::k60::Pin::Name::kPtd2
//#define LIBSC_MOTOR1_DIR libbase::k60::Pin::Name::kPtd3

#define LIBSC_MOTOR0_PWMA libbase::k60::Pin::Name::kPtd0
#define LIBSC_MOTOR0_PWMB libbase::k60::Pin::Name::kPtd1
#define LIBSC_MOTOR0_DEADTIME 2500


#define LIBSC_MOTOR1_PWMA libbase::k60::Pin::Name::kPtd2
#define LIBSC_MOTOR1_PWMB libbase::k60::Pin::Name::kPtd3
#define LIBSC_MOTOR1_DEADTIME 1000
#define LIBSC_ALTERNATE_MOTOR_CW_PWM 1

#define LIBSC_LINEAR_CCD0_SI libbase::k60::Pin::Name::kPtb7
#define LIBSC_LINEAR_CCD0_CLK libbase::k60::Pin::Name::kPtb10
#define LIBSC_LINEAR_CCD0_AD libbase::k60::Pin::Name::kPtb4
#define LIBSC_LINEAR_CCD1_SI libbase::k60::Pin::Name::kPta28
#define LIBSC_LINEAR_CCD1_CLK libbase::k60::Pin::Name::kPtb2
#define LIBSC_LINEAR_CCD1_AD libbase::k60::Pin::Name::kPtb3

#define LIBSC_OV77250_VSYNC libbase::k60::Pin::Name::kPtc2
#define LIBSC_OV77250_PCLK libbase::k60::Pin::Name::kPtb6
#define LIBSC_OV77250_DATA0 libbase::k60::Pin::Name::kPtb16
#define LIBSC_OV77250_SDA libbase::k60::Pin::Name::kPtc1
#define LIBSC_OV77250_SCL libbase::k60::Pin::Name::kPtc0
#define LIBSC_OV77250_DMA_CH 1

#define LIBSC_SERVO0 libbase::k60::Pin::Name::kPta7

#define LIBSC_UART0_TX libbase::k60::Pin::Name::kPtd7
#define LIBSC_UART0_RX libbase::k60::Pin::Name::kPtd6
