/*
 * hardware.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#if defined(MK60DZ10)
#include "libbase/k60/cmsis/mk60dz10.h"
#include "libbase/k60/cmsis/system_mk60dz10.h"

#elif defined(MK60D10)
#include "libbase/k60/cmsis/mk60d10.h"
#include "libbase/k60/cmsis/system_mk60dz10.h"

#else
#error Unknown MCU

#endif
