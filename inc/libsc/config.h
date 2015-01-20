/*
 * config.h
 * To include the corresponding config file
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#if defined(K60_INNO)
#include "libsc/k60/config/inno.h"

#elif defined(K60_2013_GEN2)
#include "libsc/k60/config/2013_gen2.h"

#elif defined(K60_2013_CCD)
#include "libsc/k60/config/2013_ccd.h"

#elif defined(K60_2013_MAGNETIC)
#include "libsc/k60/config/2013_magnetic.h"

#elif defined(K60_2014)
#include "libsc/k60/config/2014.h"

#elif defined(K60_2014_CAMERA)
#include "libsc/k60/config/2014_camera.h"

#elif defined(K60_2014_CCD)
#include "libsc/k60/config/2014_ccd.h"

#elif defined(K60_2014_INNO)
#include "libsc/k60/config/2014_inno.h"

#elif defined(K60_2014_MAGNETIC)
#include "libsc/k60/config/2014_magnetic.h"

#elif defined(K60_VCAN_FX15DEV)
#include "libsc/k60/config/vcan_fx15dev.h"

#else
#error Unknown board

#endif
