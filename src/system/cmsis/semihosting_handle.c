/*
 * semihosting_handle.c
 *
 *  Created on: 21 Dec 2018
 *      Author: lch19
 */

void HardFault_Handler(void) {
	__asm( ".syntax unified\n"
			// Check which stack is in use
			"MOVS   R0, #4  \n"
			"MOV    R1, LR  \n"
			"TST    R0, R1  \n"
			"BEQ    _MSP    \n"
			"MRS    R0, PSP \n"
			"B  _process      \n"
			"_MSP:  \n"
			"MRS    R0, MSP \n"
			// Load the instruction that triggered hard fault
			"_process:     \n"
			"LDR    R1,[R0,#24] \n"
			"LDRH    R2,[r1] \n"
			// Semihosting instruction is "BKPT 0xAB" (0xBEAB)
			"LDR    R3,=0xBEAB \n"
			"CMP     R2,R3 \n"
			"BEQ    _semihost_return \n"
			// Wasn't semihosting instruction so enter infinite loop
			"BKPT	255 \n"
			"B . \n"
			// Was semihosting instruction, so adjust location to
			// return to by 1 instruction (2 bytes), then exit function
			"_semihost_return: \n"
			"ADDS    R1,#2 \n"
			"STR    R1,[R0,#24] \n"
			// Set a return value from semihosting operation.
			// 32 is slightly arbitrary, but appears to allow most
			// C Library IO functions sitting on top of semihosting to
			// continue to operate to some degree
			"MOVS   R1,#32 \n"
			"STR R1,[ R0,#0 ] \n"// R0 is at location 0 on stack
			// Return from hard fault handler to application
			"BX LR \n"
			".syntax divided\n");
}
