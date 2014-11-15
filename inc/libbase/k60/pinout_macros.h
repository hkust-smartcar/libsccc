/*
 * pinout_constants.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#if defined(MK60DZ10) || defined(MK60D10)
#include "libbase/k60/pinout/mk60d10_lqfp144_macros.h"

#elif defined(MK60F15)
#include "libbase/k60/pinout/mk60f15_lqfp144_macros.h"

#else
#error Unknown MCU

#endif
