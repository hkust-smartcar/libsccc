#pragma once

#define __FPU_PRESENT 1

#define LIBSC_USE_BATTERY_METER 1
#define LIBSC_USE_BUZZER 1
#define LIBSC_USE_ENCODER 1
#define LIBSC_USE_JOYSTICK 1
#define LIBSC_USE_LCD 1
#define LIBSC_USE_RGB_LED 4
#define LIBSC_USE_MOTOR 1
#define LIBSC_USE_MT9V034 1
#define LIBSC_USE_SERVO 1
#define LIBSC_USE_UART 1

#define LIBSC_BATTERY_METER libbase::k60::Adc::Name::kAdc3Ad6A

#define LIBSC_BUZZER0 libbase::k60::Pin::Name::kPta13

#define LIBSC_ENCODER0_QDA libbase::k60::Pin::Name::kPta10
#define LIBSC_ENCODER0_QDB libbase::k60::Pin::Name::kPta11

#define LIBSC_JOYSTICK0_UP libbase::k60::Pin::Name::kPte26
#define LIBSC_JOYSTICK0_DOWN libbase::k60::Pin::Name::kPte9
#define LIBSC_JOYSTICK0_LEFT libbase::k60::Pin::Name::kPte8
#define LIBSC_JOYSTICK0_RIGHT libbase::k60::Pin::Name::kPte27
#define LIBSC_JOYSTICK0_SELECT libbase::k60::Pin::Name::kPte28

#define LIBSC_ST7735R_RST libbase::k60::Pin::Name::kPte3
#define LIBSC_ST7735R_DC libbase::k60::Pin::Name::kPte0
#define LIBSC_ST7735R_CS libbase::k60::Pin::Name::kPte4
#define LIBSC_ST7735R_SDAT libbase::k60::Pin::Name::kPte1
#define LIBSC_ST7735R_SCLK libbase::k60::Pin::Name::kPte2

#define LIBSC_LED0_R libbase::k60::Pin::Name::kPta24
#define LIBSC_LED1_R libbase::k60::Pin::Name::kPta12
#define LIBSC_LED2_R libbase::k60::Pin::Name::kPta19
#define LIBSC_LED3_R libbase::k60::Pin::Name::kPta28
#define LIBSC_LED0_G libbase::k60::Pin::Name::kPta1
#define LIBSC_LED1_G libbase::k60::Pin::Name::kPta8
#define LIBSC_LED2_G libbase::k60::Pin::Name::kPta14
#define LIBSC_LED3_G libbase::k60::Pin::Name::kPta16
#define LIBSC_LED0_B libbase::k60::Pin::Name::kPta7
#define LIBSC_LED1_B libbase::k60::Pin::Name::kPta9
#define LIBSC_LED2_B libbase::k60::Pin::Name::kPta15
#define LIBSC_LED3_B libbase::k60::Pin::Name::kPta17

#define LIBSC_MOTOR0_PWMA libbase::k60::Pin::Name::kPtd0
#define LIBSC_MOTOR0_PWMB libbase::k60::Pin::Name::kPtd1
#define LIBSC_MOTOR0_DEADTIME 100

#define LIBSC_MT9V034_VSYNC libbase::k60::Pin::Name::kPta29
#define LIBSC_MT9V034_PCLK libbase::k60::Pin::Name::kPtb10
#define LIBSC_MT9V034_LINE libbase::k60::Pin::Name::kPtb11
#define LIBSC_MT9V034_DATA0 libbase::k60::Pin::Name::kPtb0
#define LIBSC_MT9V034_SDA libbase::k60::Pin::Name::kPtb8
#define LIBSC_MT9V034_SCL libbase::k60::Pin::Name::kPtb9
#define LIBSC_MT9V034_DMA_CH 1

#define LIBSC_SERVO0 libbase::k60::Pin::Name::kPtc4

#define LIBSC_UART0_TX libbase::k60::Pin::Name::kPte25
#define LIBSC_UART0_RX libbase::k60::Pin::Name::kPte24
