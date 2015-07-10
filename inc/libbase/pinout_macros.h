/*
 * pinout_macros.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#if MK60DZ10 || MK60D10 || MK60F15
#include "libbase/k60/pinout_macros.h"

#elif MKL26Z4
#include "libbase/kl26/pinout_macros.h"

#else
#error Unknown MCU

#endif
