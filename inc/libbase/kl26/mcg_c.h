/*
 * mcg_c.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void LibbaseKl26McgInit(void);
uint32_t LibbaseKl26McgGetCoreClock(void);

#ifdef __cplusplus
}
#endif
