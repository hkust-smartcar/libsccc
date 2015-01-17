/*
 * system.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Setup the microcontroller system. This function should be used only after
 * reset
 */
void SystemInit(void);

#ifdef __cplusplus
}
#endif
