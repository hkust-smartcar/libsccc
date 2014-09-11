/*
 * log.h
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#pragma once

#include <stdio.h>

#define LOG_E(fmt, ...) printf("E " fmt "\n", ##__VA_ARGS__)
#define LOG_W(fmt, ...) printf("W " fmt "\n", ##__VA_ARGS__)
#define LOG_I(fmt, ...) printf("I " fmt "\n", ##__VA_ARGS__)
#ifdef DEBUG
	#define LOG_D(fmt, ...) printf("D " fmt "\n", ##__VA_ARGS__)
	#define LOG_V(fmt, ...) printf("V " fmt "\n", ##__VA_ARGS__)
#else
	#define LOG_D(fmt, ...)
	#define LOG_V(fmt, ...)
#endif /* DEBUG */

#define LOG_EL(literal) printf("E " literal "\n")
#define LOG_WL(literal) printf("W " literal "\n")
#define LOG_IL(literal) printf("I " literal "\n")
#ifdef DEBUG
	#define LOG_DL(literal) printf("D " literal "\n")
	#define LOG_VL(literal) printf("V " literal "\n")
#else
	#define LOG_DL(fmt, ...)
	#define LOG_VL(fmt, ...)
#endif /* DEBUG */
