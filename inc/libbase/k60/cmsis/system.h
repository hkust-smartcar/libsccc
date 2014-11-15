/*
 * system.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * System Clock Frequency (Core Clock & System Clock)
 */
extern uint32_t g_system_core_clock;

/**
 * Setup the microcontroller system. This function should be used only after
 * reset
 */
void SystemInit(void);

#ifdef __cplusplus
}
#endif
