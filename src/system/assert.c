/*
 * assert.c
 *
 *  Created on: Aug 30, 2018
 *      Author: LeeChunHei
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

void __assert_func(const char *file, int line, const char *fn,
		const char *expression)
{
#ifdef DEBUG
	while (1)
	{
		char error_message[100]={};
		sprintf(error_message, "Assertion(%s) failed in %s:%s at line %d\n", expression, file,fn, line);
		printf(error_message);
		asm("BKPT 255");
		// Arbitrary delay
		for (uint32_t i = 0; i < 50000000; ++i)
		{
			asm("nop");
		}
	}
#endif
}


