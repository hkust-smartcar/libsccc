/* GNUC Startup library
 *    Copyright � 2005 Freescale semiConductor Inc. All Rights Reserved.
 *
 * $Date: 2011/09/21 06:41:34 $
 * $Revision: 1.4 $
 */

#include "libbase/k60/hardware.h"

#include <string.h>
#include <stdlib.h>

#include "libbase/k60/cmsis/system.h"
#include "libbase/k60/watchdog_c.h"
#include "libbase/syscall.h"

extern void __libc_init_array(void);
extern int main(void);
extern char __SP_INIT[];

void __thumb_startup(void);

/* format of the ROM table info entry ... */
typedef struct RomInfo {
	unsigned long	Source;
	unsigned long	Target;
	unsigned long 	Size;
} RomInfo;

RomInfo __S_romp __attribute__((__weak__));		/* linker defined symbol */

/*
 * Routine to flush cache follow the ROM to RAM copy
 */
static void __flush_cache(unsigned long dst, unsigned long size)
{
	//TBD
	(void)dst;
	(void)size;
}

/*
 *	Routine to copy a single section from ROM to RAM ...
 */
static void __copy_rom_section(unsigned long dst, unsigned long src, unsigned long size)
{
	unsigned long len = size;

	const unsigned int mask_int = sizeof(int)-1;
	const unsigned int mask_short = sizeof(short)-1;

	if( dst == src || size == 0)
	{
		return;
	}


	while( len > 0)
	{
		if( !(src & mask_int) && !(dst & mask_int) && len >= sizeof(int))
		{
			*((int *)dst)  = *((int*)src);
			dst += sizeof(int);
			src += sizeof(int);
			len -= sizeof(int);
		}
		else if( !(src & mask_short) && !(dst & mask_short) && len >= sizeof(short))
		{
			*((short *)dst)  = *((short*)src);
			dst += sizeof(short);
			src += sizeof(short);
			len -= sizeof(short);
		}
		else
		{
			*((char *)dst)  = *((char*)src);
			dst += sizeof(char);
			src += sizeof(char);
			len -= sizeof(char);
		}
	}
}

/*
 *	Routine that copies all sections the user marked as ROM into
 *	their target RAM addresses ...
 *
 *	__S_romp is defined in the linker command file
 *  It is a table of RomInfo
 *	structures.  The final entry in the table has all-zero
 *	fields.
 */
static void __copy_rom_sections_to_ram(void)
{
	int index;

	if (&__S_romp == 0L) return;

	/*
	 *	Go through the entire table, copying sections from ROM to RAM.
	 */
	for (index = 0;
		 (&__S_romp)[index].Source != 0 ||
		 (&__S_romp)[index].Target != 0 ||
		 (&__S_romp)[index].Size != 0;
		 ++index)
	{
		__copy_rom_section( (&__S_romp)[index].Target,
							(&__S_romp)[index].Source,
							(&__S_romp)[index].Size );

		__flush_cache( (&__S_romp)[index].Target, (&__S_romp)[index].Size);
	}
}

static void zero_fill_bss(void)
{
	extern char __START_BSS[];
	extern char __END_BSS[];

	unsigned long len = __END_BSS - __START_BSS;
	unsigned long dst = (unsigned long) __START_BSS;

	const unsigned int mask_int = sizeof(int)-1;
	const unsigned int mask_short = sizeof(short)-1;

	if( len == 0)
	{
		return;
	}


	while( len > 0)
	{
		if( !(dst & mask_int) && len >= sizeof(int))
		{
			*((int *)dst)  = 0;
			dst += sizeof(int);
			len -= sizeof(int);
		}
		else if( !(dst & mask_short) && len >= sizeof(short))
		{
			*((short *)dst)  = 0;
			dst += sizeof(short);
			len -= sizeof(short);
		}
		else
		{
			*((char *)dst)  = 0;
			dst += sizeof(char);
			len -= sizeof(char);
		}
	}
}

// To keep iar debugger happy
void __iar_program_start(void)
{
	__thumb_startup();
}

void __thumb_startup(void)
{
	int addr = (int)__SP_INIT;

	LibbaseK60WatchdogStartupInitialize();

	// setup the stack before we attempt anything else
	// skip stack setup if __SP_INIT is 0
	// assume sp is already setup.
	__asm (
			"mov	r0,%0\n\t"
			"cmp	r0,#0\n\t"
			"beq	skip_sp\n\t"
			"mov	sp,r0\n\t"
			"sub	sp,#4\n\t"
			"mov	r0,#0\n\t"
			"mvn	r0,r0\n\t"
			"str	r0,[sp,#0]\n\t"
			"add	sp,#4\n\t"
			"skip_sp:\n\t"
			::"r"(addr));

	//	zero-fill the .bss section
	zero_fill_bss();
	// SUPPORT_ROM_TO_RAM
	__copy_rom_sections_to_ram();

	SystemInit();
	__libc_init_array();

	main();
	while (1);
}

// Called in __libc_init_array()
void _init(void)
{
	KeepSyscallSymbols();
}
