/*
 * pinout.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#if defined(MK60DZ10) || defined(MK60D10)
#include "libbase/k60/pinout/mk60d10_lqfp144.h"
#define PINOUT libbase::k60::Mk60d10Lqfp144

#else
#error Unknown MCU

#endif
