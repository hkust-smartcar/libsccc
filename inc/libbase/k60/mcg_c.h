/*
 * mcg_c.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void LibbaseK60McgInit(void);
uint32_t LibbaseK60McgGetCoreClock(void);

#ifdef __cplusplus
}
#endif
