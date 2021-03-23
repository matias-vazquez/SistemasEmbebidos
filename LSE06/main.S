/*
 * Blink, for SAMD10 Xplained Mini, in assembler.
 *
 * How to build an pure assembler project in Atmel Studio.
 * 1) The ARM Assembler is the gcc assembler, which is part of the C toolchain.
 *    So start by creating a "C Executable project" with the correct device.
 * 2) Modify the project Properties in the Toolchain panel:
 *    - for ARM/GNU Preprocessing Assembler, define a symbol __ASSEMBLY__
 *    - for ARM/GNU Linker under "general", check the (3) boxes to NOT use the
 *       standard libraries, start-files, or default libraries (all of which are in C)
 * 3) Delete or disable the .c files from the Device_Startup folder.  You'll need to replace all
 *    of the functionality that was there.  Also, we'll be using the linker scripts, which
 *    means you'll need to define the sections that they're expecting.  Particularly ".vectors"
 * 4) Delete main.c
 * 5) Populate the project with added .S files. These will be pre-processed by the C preprocessor.
 */

/*
 * 6) Add the option "-Wl,--defsym,STACK_SIZE=n" to the linker miscellaneous flags, were n is the
 *    stack size in bytes.
 *    The linker script will automatically create a .stack section of the requested size in RAM.
 *    If you omit this option, an MCU specific default will be used (1024 bytes for the samd10d14).
 *
 * 7) Create an "include" folder in the project, and populate it with a modified top level header as
 *    described bellow:
 *
 *    The new header files are more assembly friendly, we no longer need to define addresses without 
 *    the type suffixes.
 *    However they still need improvement. For this reason, I created a modified version of the top
 *    level header "samd10d14am.h" and included it in the project.
 *    The change is as follows: there is a section in this file, "CMSIS DEFINITIONS FOR SAMD10D14AM"
 *    that needs to be disabled when __ASSEMBLY__ is defined, so I added
 *    "#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))" before this section,
 *    and "#endif" after.
 *    With this modification, all we need to do is include the modified top level header.
 */
#include "include\my_samd21g18a.h"

			.section .text
			.syntax unified
			.global main


main:

			// WRITE YOUR CODE HERE¡ //

loop:		
			bne loop
forever:
			wfi										/* go to sleep */
			b.n		forever

			ldr		r1, =foo	/* Use the dummy variable, or it won't reserve .bss memory */

			.section .bss
foo:		.word 0			/* Dummy variable for testing */


/* Valuable information for creating this section can be obtained from the startup files present in the
 * Device_Startup folder, especially the .c files that are created for a normal C project.
 * They were renamed to header files in this project, to disable them.
 */
			.section ".vectors"
			.global exceptiontable
exceptiontable:
			.extern _estack			/* the linker script creates this symbol, pointing to the top of the stack section */
			.word _estack		/* Initial Stack Pointer */
			.word main+1		/* Start address (+1 means "thumb mode") */
			.end
