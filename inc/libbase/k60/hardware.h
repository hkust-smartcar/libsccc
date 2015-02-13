/*
 * hardware.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#if defined(MK60DZ10)
#include "libbase/k60/cmsis/mk60dz10.h"

#elif defined(MK60D10)
#include "libbase/k60/cmsis/mk60d10.h"

#elif defined(MK60F15)
#include "libbase/k60/cmsis/mk60f15.h"

#else
#error Unknown MCU

#endif
