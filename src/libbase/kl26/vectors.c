//
// This file is part of the GNU ARM Eclipse Plug-ins distribution.
// Copyright (c) 2014 Liviu Ionescu
//

// ----------------------------------------------------------------------------
#include "libbase/kl26/hardware.h"

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>

#include "libbase/log.h"
#include "libbase/kl26/misc_utils_c.h"
#include "libbase/kl26/vectors.h"

__attribute__((__naked__)) void HardFaultHandlerAsm(void);

HardFaultHandler g_hard_fault_handler = NULL;

/* ISR prototype */
extern uint32_t __SP_INIT;
void __thumb_startup(void);

/* Interrupt vector table type definition */
typedef struct {
	void * __ptr;
	tIsrFunc __fun[0x2F];
} tVectorTable;

__attribute__((__section__(".vectortable")))
const tVectorTable __vect_table = { /* Interrupt vector table */
	/* ISR name                      No. Address      Pri Name                          Description */
	&__SP_INIT,                 /* 0x00  0x00000000   -   ivINT_Initial_Stack_Pointer   used by PE */
	{
		__thumb_startup,        /* 0x01  0x00000004   -   ivINT_Initial_Program_Counter used by PE */
		DefaultIsr,             /* 0x02  0x00000008   -2   ivINT_NMI                     used by PE */
		HardFaultHandlerAsm,    /* 0x03  0x0000000C   -1   ivINT_Hard_Fault              unused by PE */
		DefaultIsr,             /* 0x04  0x00000010   -   ivINT_Reserved4               unused by PE */
		DefaultIsr,             /* 0x05  0x00000014   -   ivINT_Reserved5               unused by PE */
		DefaultIsr,             /* 0x06  0x00000018   -   ivINT_Reserved6               unused by PE */
		DefaultIsr,             /* 0x07  0x0000001C   -   ivINT_Reserved7               unused by PE */
		DefaultIsr,             /* 0x08  0x00000020   -   ivINT_Reserved8               unused by PE */
		DefaultIsr,             /* 0x09  0x00000024   -   ivINT_Reserved9               unused by PE */
		DefaultIsr,             /* 0x0A  0x00000028   -   ivINT_Reserved10              unused by PE */
		DefaultIsr,             /* 0x0B  0x0000002C   -   ivINT_SVCall                  unused by PE */
		DefaultIsr,             /* 0x0C  0x00000030   -   ivINT_Reserved12              unused by PE */
		DefaultIsr,             /* 0x0D  0x00000034   -   ivINT_Reserved13              unused by PE */
		DefaultIsr,             /* 0x0E  0x00000038   -   ivINT_PendableSrvReq          unused by PE */
		DefaultIsr,             /* 0x0F  0x0000003C   -   ivINT_SysTick                 unused by PE */
		DefaultIsr,             /* 0x10  0x00000040   -   ivINT_DMA0                    unused by PE */
		DefaultIsr,             /* 0x11  0x00000044   -   ivINT_DMA1                    unused by PE */
		DefaultIsr,             /* 0x12  0x00000048   -   ivINT_DMA2                    unused by PE */
		DefaultIsr,             /* 0x13  0x0000004C   -   ivINT_DMA3                    unused by PE */
		DefaultIsr,             /* 0x14  0x00000050   -   ivINT_Reserved20              unused by PE */
		DefaultIsr,             /* 0x15  0x00000054   -   ivINT_FTFA                    unused by PE */
		DefaultIsr,             /* 0x16  0x00000058   -   ivINT_LVD_LVW                 unused by PE */
		DefaultIsr,             /* 0x17  0x0000005C   -   ivINT_LLW                     unused by PE */
		DefaultIsr,             /* 0x18  0x00000060   -   ivINT_I2C0                    unused by PE */
		DefaultIsr,             /* 0x19  0x00000064   -   ivINT_I2C1                    unused by PE */
		DefaultIsr,             /* 0x1A  0x00000068   -   ivINT_SPI0                    unused by PE */
		DefaultIsr,             /* 0x1B  0x0000006C   -   ivINT_SPI1                    unused by PE */
		DefaultIsr,             /* 0x1C  0x00000070   -   ivINT_UART0                   unused by PE */
		DefaultIsr,             /* 0x1D  0x00000074   -   ivINT_UART1                   unused by PE */
		DefaultIsr,             /* 0x1E  0x00000078   -   ivINT_UART2                   unused by PE */
		DefaultIsr,             /* 0x1F  0x0000007C   -   ivINT_ADC0                    unused by PE */
		DefaultIsr,             /* 0x20  0x00000080   -   ivINT_CMP0                    unused by PE */
		DefaultIsr,             /* 0x21  0x00000084   -   ivINT_TPM0                    unused by PE */
		DefaultIsr,             /* 0x22  0x00000088   -   ivINT_TPM1                    unused by PE */
		DefaultIsr,             /* 0x23  0x0000008C   -   ivINT_TPM2                    unused by PE */
		DefaultIsr,             /* 0x24  0x00000090   -   ivINT_RTC                     unused by PE */
		DefaultIsr,             /* 0x25  0x00000094   -   ivINT_RTC_Seconds             unused by PE */
		DefaultIsr,             /* 0x26  0x00000098   -   ivINT_PIT                     unused by PE */
		DefaultIsr,             /* 0x27  0x0000009C   -   ivINT_I2S0                    unused by PE */
		DefaultIsr,             /* 0x28  0x000000A0   -   ivINT_USB0                    unused by PE */
		DefaultIsr,             /* 0x29  0x000000A4   -   ivINT_DAC0                    unused by PE */
		DefaultIsr,             /* 0x2A  0x000000A8   -   ivINT_TSI0                    unused by PE */
		DefaultIsr,             /* 0x2B  0x000000AC   -   ivINT_MCG                     unused by PE */
		DefaultIsr,             /* 0x2C  0x000000B0   -   ivINT_LPTimer                 unused by PE */
		DefaultIsr,             /* 0x2D  0x000000B4   -   ivINT_Reserved45              unused by PE */
		DefaultIsr,             /* 0x2E  0x000000B8   -   ivINT_PORTA                   unused by PE */
		DefaultIsr              /* 0x2F  0x000000BC   -   ivINT_PORTC_PORTD             unused by PE */
	}
};

__attribute__((__section__ (".vectortableram"))) tVectorTable __vect_ram; /* Interrupt vector table in RAM */

void InitVectorTable(void)
{
	const Byte *rom = (const Byte*)&__vect_table;
	Byte *ram = (Byte*)&__vect_ram;

    for (uint32_t i = 0; i < sizeof(tVectorTable); ++i)
    {
    	*ram++ = *rom++;
    }

	assert((uint32_t)(&__vect_ram) % 0x200 == 0);   //Vector Table base offset field. This value must be a multiple of 0x200.
	/* Write the VTOR with the new value */
	SCB->VTOR = (uint32_t)(&__vect_ram);
}

void SetIsr(IRQn_Type irq, tIsrFunc handler)
{
	__vect_ram.__fun[irq + abs(NonMaskableInt_IRQn) + 1] = handler ? handler
			: DefaultIsr;
}

void EnableIrq(IRQn_Type irq)
{
	NVIC_EnableIRQ(irq);
}

void DisableIrq(IRQn_Type irq)
{
	NVIC_DisableIRQ(irq);
}

void DefaultIsr(void)
{
	LOG_W("Unhandled interrupt (VECTORn_t == %" PRIu32 ")\n", GetActiveVector());
}

/**
 * HardFaultHandler:
 * Alternative Hard Fault handler to help debug the reason for a fault.
 * To use, edit the vector table to reference this function in the HardFault vector
 * This code is suitable for Cortex-M3 and Cortex-M0 cores
 */
void HardFaultHandlerAsm(void)
{
	/*
	* Get the appropriate stack pointer, depending on our mode,
	* and use it as the parameter to the C handler
	*/
	__asm("MOVS   R0, #4  \n"
			"MOV    R1, LR  \n"
			"TST    R0, R1  \n"
			"BEQ    _MSP    \n"
			"MRS    R0, PSP \n"
			"B      HardFaultHandlerC      \n"
			"_MSP:  \n"
			"MRS    R0, MSP \n"
			"B      HardFaultHandlerC      \n") ;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"

/**
 * HardFaultHandlerC:
 * This is called from the HardFault_HandlerAsm with a pointer the Fault stack
 * as the parameter. We can then read the values from the stack and place them
 * into local variables for ease of reading.
 * We then read the various Fault Status and Address Registers to help decode
 * cause of the fault.
 * The function ends with a BKPT instruction to force control back into the debugger
 */
void HardFaultHandlerC(unsigned long *hardfault_args)
{
	volatile unsigned long stacked_r0;
	volatile unsigned long stacked_r1;
	volatile unsigned long stacked_r2;
	volatile unsigned long stacked_r3;
	volatile unsigned long stacked_r12;
	volatile unsigned long stacked_lr;
	volatile unsigned long stacked_pc;
	volatile unsigned long stacked_psr;
	volatile unsigned long _CFSR;
	volatile unsigned long _HFSR;
	volatile unsigned long _DFSR;
	volatile unsigned long _AFSR;
	volatile unsigned long _BFAR;
	volatile unsigned long _MMAR;

	stacked_r0 = ((unsigned long)hardfault_args[0]);
	stacked_r1 = ((unsigned long)hardfault_args[1]);
	stacked_r2 = ((unsigned long)hardfault_args[2]);
	stacked_r3 = ((unsigned long)hardfault_args[3]);
	stacked_r12 = ((unsigned long)hardfault_args[4]);
	stacked_lr = ((unsigned long)hardfault_args[5]);
	stacked_pc = ((unsigned long)hardfault_args[6]);
	stacked_psr = ((unsigned long)hardfault_args[7]);

	// Configurable Fault Status Register
	// Consists of MMSR, BFSR and UFSR
	_CFSR = (*((volatile unsigned long*)(0xE000ED28)));

	// Hard Fault Status Register
	_HFSR = (*((volatile unsigned long*)(0xE000ED2C)));

	// Debug Fault Status Register
	_DFSR = (*((volatile unsigned long*)(0xE000ED30)));

	// Auxiliary Fault Status Register
	_AFSR = (*((volatile unsigned long*)(0xE000ED3C)));

	// Read the Fault Address Registers. These may not contain valid values.
	// Check BFARVALID/MMARVALID to see if they are valid values
	// MemManage Fault Address Register
	_MMAR = (*((volatile unsigned long*)(0xE000ED34)));
	// Bus Fault Address Register
	_BFAR = (*((volatile unsigned long*)(0xE000ED38)));

	if (g_hard_fault_handler)
	{
		g_hard_fault_handler();
	}
	__BREAKPOINT(); // Break into the debugger
}

#pragma GCC diagnostic pop
