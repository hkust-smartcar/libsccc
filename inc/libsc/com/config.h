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

#else
#error Unknown board

#endif

#endif /* LIBSC_CONFIG_H_ */
