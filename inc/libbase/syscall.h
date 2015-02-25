/*
 * syscall.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*FwriteHandler)(int file, char *ptr, int len);
extern FwriteHandler g_fwrite_handler;

/**
 * Retain all the syscall symbols
 */
void KeepSyscallSymbols(void);

#ifdef __cplusplus
}
#endif
