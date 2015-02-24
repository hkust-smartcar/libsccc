/*
 * misc_utils.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <cstdint>

#include "libbase/misc_utils_c.h"

#define VOID
#define STATE_GUARD(clz, ret) if (!(*this)) { LOG_W(#clz " in illegal state @%d", __LINE__); return ret; }
