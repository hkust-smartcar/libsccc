/*
 * pinout.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#if defined(MKL26Z4)
#include "libbase/kl26/pinout/mkl26z4_lqfp100.h"
#define PINOUT libbase::kl26::Mkl26z4Lqfp100

#else
#error Unknown MCU

#endif
