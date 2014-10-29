/*
 * libsc_guard.h
 * Protect you from unmatched symbol and linked library
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

namespace libsc
{

#if defined(K60_INNO)
extern const int kGuardK60Inno;
#define LIBSC_GUARD_VAR kGuardK60Inno

#elif defined(K60_2013_GEN2)
extern const int kGuardK602013Gen2;
#define LIBSC_GUARD_VAR kGuardK602013Gen2

#elif defined(K60_2013_CCD)
extern const int kGuardK602013Ccd;
#define LIBSC_GUARD_VAR kGuardK602013Ccd

#elif defined(K60_2013_MAGNETIC)
extern const int kGuardK602013Magnetic;
#define LIBSC_GUARD_VAR kGuardK602013Magnetic

#elif defined(K60_2014)
extern const int kGuardK602014;
#define LIBSC_GUARD_VAR kGuardK602014

#elif defined(K60_2014_CAMERA)
extern const int kGuardK602014Camera;
#define LIBSC_GUARD_VAR kGuardK602014Camera

#elif defined(K60_2014_CCD)
extern const int kGuardK602014Ccd;
#define LIBSC_GUARD_VAR kGuardK602014Ccd

#elif defined(K60_2014_INNO)
extern const int kGuardK602014Inno;
#define LIBSC_GUARD_VAR kGuardK602014Inno

#elif defined(K60_2014_MAGNETIC)
extern const int kGuardK602014Magnetic;
#define LIBSC_GUARD_VAR kGuardK602014Magnetic

#else
extern const int kGuardNull;
#define LIBSC_GUARD_VAR kGuardNull

#endif

#define LIBSC_GUARD() do { if (libsc::LIBSC_GUARD_VAR != 0) { asm("nop"); } } while (false)

}
