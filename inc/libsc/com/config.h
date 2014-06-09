/*
 * config.h
 * To include the corresponding config file
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#ifndef LIBSC_CONFIG_H_
#define LIBSC_CONFIG_H_

#if defined(K60_INNO)
#include "config/inno.h"

#elif defined(K60_2013_GEN2)
#include "config/2013_gen2.h"

#elif defined(K60_2013_CCD)
#include "config/2013_ccd.h"

#elif defined(K60_2013_MAGNETIC)
#include "config/2013_magnetic.h"

#elif defined(K60_2014)
#include "config/2014.h"

#elif defined(K60_2014_CAMERA)
#include "config/2014_camera.h"

#elif defined(K60_2014_CCD)
#include "config/2014_ccd.h"

#elif defined(K60_2014_MAGNETIC)
#include "config/2014_magnetic.h"

#else
#error Unknown board

#endif

#endif /* LIBSC_CONFIG_H_ */
