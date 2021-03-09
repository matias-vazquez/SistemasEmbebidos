@ delayExample.s
@ D. Thiebaut
@ outputs "Hello World!" 10 times with 1 sec
@ or 0.5 sec delay between each output.
@ Use the function sleep() for second delays,
@ and the function usleep() for finer delays.	
@
@ ---------------------------------------
@     Data Section
@ ---------------------------------------
	
          .data
          .balign 4	
hello:    .asciz "Hello World!\n\n"
seconds:
          .int    1		@ 1 sec
microsec:
          .int    500000	@ 0.5 sec
	
N         .req    r6            @ counter
Max       .req    r5		@ max # iterations

@ ---------------------------------------
@     Code Section
@ ---------------------------------------
	
          .text
          .global main
          .extern printf
          .extern sleep
          .extern usleep

main:     push    {ip, lr}	@ push return address + dummy register for alignment
          mov     n, #0
          mov     max, #10
	
loop:     cmp     n, max	@ n>max?
          bgt     done		@ yes, done
                                @ no, output string
          ldr     r0, =hello
          bl      printf	@ print string and r1 as param

@         ldr     r0, =seconds	@ use sleep for integer # of 
@         ldr     r0, [r0]	@ seconds
@         bl      sleep

          Ldr     r0, =microsec	@ use usleep for microsecond
          Ldr     r0, [r0]	@ delays.
          Bl      usleep
	
          Add     n, #1		@ n++
          B       loop

done:	
          pop     {ip, pc}	@ pop return address into pc
