# C Programming and I<sup>2</sup>C interfacing in Arduino Zero

## Objectives
* Strengthen your knowledge and abilities about C programming and serial communication protocols, specifically I<sup>2</sup>C. 
* Learn how to program timers and develop interfaces to I<sup>2</sup>C peripherals specifically the real time clock (RTC).

## Introduction
This lab provides you the opportunity to learn how to develop embedded software applications by following the steps and practices commonly used in industry. In this lab, you will learn to write C language code using the header files and source files provided by ATMEL. Your will also develop your own header and source files to access and manipulate a real time clock. This lab will also provide you the opportunity to strengthen your knowledge about the i2c protocol by programming an interface to the real time clock and by observing and measuring timing of the i2c signals using the oscilloscope. The oscilloscope for this lab has additional software licenses (and expensive) software to show a decoded view of the i2c signals. 

## Materials and Software
<div align="center">

Qty | Material
:---: | :---
1  | Arduino Zero
1  | Micro-USB cable
1  | Real Time Clock
1  | Oscilloscope (serial communications-capable)
1  | FTDIO for serial communications
1  | Miniusb cable
 As required  | Wiring cables (male to female pin header cables)

 | Software
 | :---
 | Latest version of [Microchip Studio for AVR and SAM Devices](https://www.microchip.com/en-us/development-tools-tools-and-software/microchip-studio-for-avr-and-sam-devices) running on Windows
 | Latest version of [PuTTY [2.7 MB]](https://www.putty.org/) running on Windows

 </div>

## Part I. Translate to C language the code of a timer
Given the following code:
```assembly
main:  //PORT configuration for general-purpose PIN
                 	LDR  R0, =0x41004400 //PORT Base Address
                 	LDR  R1, =0b00000000 //config value
                 	LDR  R2, =0x4E
                 	STRB R1, [R0, R2]	//PINCFG14
                 	LDR  R2, =0x00004000 //PA14 bit set position
                 	STR  R2, [R0, #0x08] //DIRSET -- data direction as output
    	//TIMER configuration
          	//Configure the POWER MANAGER to enable the TC3 module
                 	LDR  R3, =0x40000400 //PM Base Address
                 	LDR  R1, [R3, #0x20] //read register APBCMASK
                 	LDR  R4, =0x00000800 //mask value
                 	ORRS R1, R1, R4  	//to set desired bit(s)
                 	STR  R1, [R3, #0x20] //APBCMASK
          	//Configure the GENERIC CLOCK CONTROL used by the TC3 module
                 	LDR  R3, =0x40000C00 //GCLK Base Address
                 	LDR  R1, =0      	//config value
                 	STR  R1, [R3, #0x08] //GENDIV
                 	LDR  R1, =0x10600	//config value
                 	STR  R1, [R3, #0x04] //GENCTRL
                 	LDR  R1, =0x401B 	//config value
                 	STRH R1, [R3, #0x02] //CLKCTRL
          	//Determine the initial count
          	//Time = Toscillator * (2^TIMERMODE - COUNT) * Prescaler
                 	LDR  R3, =0x42002C00 //TC3 Base Address
                 	LDR  R6, =65307  	//initial count of the timer + 27 cycles
                 	STRH R6, [R3, #0x10] //COUNT          	
          	//Configure the TC3 peripheral
                 	LDR  R1, =0      	//config value
                 	STRH R1, [R3]    	//CTRLA
                 	STRB R1, [R3, #0x05] //CTRLBSET
                 	LDR  R5, =0x1    	//to clear OVF flag
                 	STRB R5, [R3, #0x0E] //INTFLAG
          	//Enabling the TC3 peripheral
                 	LDR  R1, [R3]    	//reads CTRLA
                 	LDR  R4, =0x2    	//mask value
                 	ORRS R1, R1, R4  	//to set desired bit(s)
                 	STRH R1, [R3]    	//CTRLA
          	//Loop to sync the TC clock with the processor/main clock
                 	LDR  R4, =0x80   	//mask value
read:     	   	LDRB R1, [R3, #0x0F] //reads STATUS
                 	ANDS R1, R1, R4  	//to test SYNCBUSY bit
                 	BNE  read        	//jump if it not synchronize
   	//***************************************************************************
          	//Loop to generate the square-wave signal
            //EXTRA cycles to ADD to the initial count---it will be still a //polling-error of +/- 7 us in the period of the signal
            //LDRB(4) + ANDS(1) + BEQ(2) + LDRB(4) + ANDS(1) + BEQ(1) + STR(4) + //STRB(4) + STRH(4) + B(2) = 27 cycles
again:    	   	LDRB R1, [R3, #0x0E] //reads the INTFLAG
                 	ANDS R1, R1, R5  	//to test OVF flag
                 	BEQ  again       	//jump if is not overflowed
                 	STR  R2, [R0, #0x1C] //toggle the output pin
                 	STRB R5, [R3, #0x0E] //clear OVF flag
                 	STRH R6, [R3, #0x10] //recharge the initial COUNT
                 	B	again

```

## Part II

