/*
 * assert.c
 *
 *  Created on: Aug 29, 2014
 *      Author: ming
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
