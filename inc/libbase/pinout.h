/*
 * pinout.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#if MK60DZ10 || MK60D10 || MK60F15
#include "libbase/k60/pinout.h"

#elif MKL26Z4
#include "libbase/kl26/pinout.h"

#else
#error Unknown MCU

#endif
