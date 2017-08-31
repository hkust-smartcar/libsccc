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

#elif defined(K60_2015_CAMERA)
#include "libsc/k60/config/2015_camera.h"

#elif defined(K60_2015_CAMERA2)
#include "libsc/k60/config/2015_camera2.h"

#elif defined(K60_2015_MAGNETIC)
#include "libsc/k60/config/2015_magnetic.h"

#elif defined(K60_2015_CCD)
#include "libsc/k60/config/2015_ccd.h"

#elif defined(K60_2015_CCD_GEN2)
#include "libsc/k60/config/2015_ccd_gen2.h"

#elif defined(KL26_2015_CCD2)
#include "libsc/kl26/config/2015_ccd2.h"

#elif defined(KL26_VCAN_Z4DEV)
#include "libsc/kl26/config/vcan_z4dev.h"

#elif defined(KL26_2015_M2)
#include "libsc/kl26/config/2015_m2.h"

#elif defined(K60_2016_INNO)
#include "libsc/k60/config/2016_inno.h"

#elif defined(K60_2016_CAMERA)
#include "libsc/k60/config/2016_camera.h"

#elif defined(K60_2016_CCD)
#include "libsc/k60/config/2016_ccd.h"

#elif defined(K60_2016_RAIL)
#include "libsc/k60/config/2016_rail.h"

#elif defined(K60_2016_MAGNETIC)
#include "libsc/k60/config/2016_magnetic.h"

#elif defined(K60_2016_MAGNETIC_YANG)
#include "libsc/k60/config/2016_magnetic_yang.h"

#elif defined(K60_2017_INNO)
#include "libsc/k60/config/2017_inno.h"

#elif defined(K60_2017_CREATIVE)
#include "libsc/k60/config/2017_creative.h"

#else
#error Unknown board

#endif
