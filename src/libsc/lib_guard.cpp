/*
 * libsc_guard.cpp
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include "libsc/lib_guard.h"

namespace libsc
{

#if defined(K60_INNO)
const int kGuardK60Inno = 0;

#elif defined(K60_2013_GEN2)
const int kGuardK602013Gen2 = 0;

#elif defined(K60_2013_CCD)
const int kGuardK602013Ccd = 0;

#elif defined(K60_2013_MAGNETIC)
const int kGuardK602013Magnetic = 0;

#elif defined(K60_2014)
const int kGuardK602014 = 0;

#elif defined(K60_2014_CAMERA)
const int kGuardK602014Camera = 0;

#elif defined(K60_2014_CCD)
const int kGuardK602014Ccd = 0;

#elif defined(K60_2014_INNO)
const int kGuardK602014Inno = 0;

#elif defined(K60_2014_MAGNETIC)
const int kGuardK602014Magnetic = 0;

#else
const int kGuardNull = 0;

#endif

}
