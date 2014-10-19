/*
 * assert.c
 *
 * Author: Ming Tsang
 * Copyright (c) 2014 HKUST SmartCar Team
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "libbase/k60/misc_utils_c.h"

void __assert_func(const char *file, int line, const char *fn,
		const char *expression)
{
	while (1)
	{
		iprintf("Assertion(%s) failed in %s:%s at line %d\n", expression, file,
				fn, line);
		// Arbitrary delay
		for (uint32_t i = 0; i < 50000000; ++i)
		{
			asm("nop");
		}
		__BREAKPOINT();
		__HARDFAULT();
	}
}
