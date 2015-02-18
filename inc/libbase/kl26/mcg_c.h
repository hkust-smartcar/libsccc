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

void LibbaseKL26McgInit(void);
uint32_t LibbaseKL26McgGetCoreClock(void);

#ifdef __cplusplus
}
#endif
