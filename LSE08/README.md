# Laboratory 9: FreeRTOS Threads and Queues

## Objective
To trengthen your knowledge and abilities about real time operating systems, specifically how to use threads and queues.

## Introduction
This lab provides you the opportunity to learn how to develop embedded software applications using a real time operating system. This lab will help you understand how to schedule tasks and how to exchange data between threads using queues. 

__HARDWARE__
Qty | Material
:---: | :---
1 | Arduino Zero
1 | USB-microusb cable
1 | Dip Switch
4 | Resistors 4.7k
4 | Resistors 1k
1 | FTDIO for serial communication
1 |USB-miniusb cable

__SOFTWARE__

- :white_check_mark: FreeRTOS code
- :white_check_mark: `myprintf`, `uart.c` and `uart.h` routines
- :white_check_mark: Atmel/Microchip Studio
- :white_check_mark: PuTTY

Part I: Setup FreeRTOS for the SAMD21g18 processor

1. Download the following FreeRTOS files adjusted for Arduino zero from this link. Place this folder as illustrated below:
    C:\Users\YOURUSErNAME\Documents\Atmel Studio\7.0\FreeRTOS

2. Create a new project in ATMEL studio.

3. Select the GCC C Executable version.

4. Change the GCCApplicationX  name to your prefered name (FRThreadQueue).

 


5. At the solution explorer, create two folders.

 

The “include” and the “source” folders as illustrated below.

 

6. Copy all the files from the folder indicated below and paste them into the include folder at the solution explorer as illustrated below. 
 C:\Users\YOURUSErNAME\Documents\Atmel Studio\7.0\FreeRTOS\include

 

7. Copy all the files from the folder indicated below and paste them into the source folder at the solution explorer. 
C:\Users\YOURUSErNAME\Documents\Atmel Studio\7.0\FreeRTOS\src

8. Modify the main program so it contains the next code:

/* Kernel includes. */
#include "sam.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "uart.h"
#include "myprintf.h"

/* Priorities at which the tasks are created. */
#define myTASK_TASK_PRIORITY         	( tskIDLE_PRIORITY + 1 )

void my_task(void *p) {
    	while(1) {
            	static uint32_t i = 0;
            	myprintf("\nHello World task 1 %d", i++);
            	vTaskDelay(1000);
    	}
}

int main()
{
    	SystemInit();
    	/* Switch to 8MHz clock (disable prescaler) */
    	SYSCTRL->OSC8M.bit.PRESC = 0;	
    	initUART();

    	
    	xTaskCreate( my_task,        	/* The function that implements the task. */
            	    "my_task",         /* The text name assigned to the task. */
            	    512,            	/* The size of the stack to allocate to the task. */
            	    NULL,           	/* The parameter passed to the task  */
            	    myTASK_TASK_PRIORITY,  /* The priority assigned to the task. */
            	    NULL );         	   /* The task handle is not required, so NULL is passed. */

    	/* Start the tasks and timer running. */
    	vTaskStartScheduler();

    	for( ;; );
    	return(0);
}

9. Build the application and connect your development board to the PC using the serial port. Run the program and demonstrate to your professor (print screens) that the messages are properly displayed at the Serial Port terminal (PuTTy). 


Part II. Getting familiar with Threads and Queues under FreeRTOS
View the video found in this link and follow the indications of the presenter step by step (you may want to stop the video to type the code).

1. Your program should display the following messages at the end:
Send 0 to receiver task
Received 0
Demonstrate to your professor (print screens) that these messages were displayed at the PC terminal and show you code. Report your code at this point within your report document. No need to upload files at this point.
2. Your program should display the following messages at the end:
Send 0 to receiver task
Received 0
Failed to receive data from queue
Send 1 to receiver task
Received 1
Demonstrate to your professor that these messages were displayed at the PC terminal and show you code. Report your final code including it in your document along with print screens of your program working correctly. Upload the files with your code as well. Report an explanation of the reason for displaying “Failed to receive data from queue”.


Part III. Write Code using the FreeRTOS to exchange messages and control peripherals

 
	Data flow of software application
Connect four switches to the parallel ports. The messages associated with each switch listed in the table below. Add the schematic diagram to the report.
SWITCH    	MESSAGE
1                  	Up		(is connected to D2 in Arduino Zero)
2                  	Down		(is connected to D3 in Arduino Zero)
3                  	Left		(is connected to D4 in Arduino Zero)
4                  	Right		(is connected to D5 in Arduino Zero)

Example code to read a push-button from Arduino Zero. The next code turns-on the TX LED while the push-button (pull-up configuration) is pressed. 

#include "sam.h"

PORT->Group[ 0 ].PINCFG[ PIN_PA16 ].reg = 0x2; //bit INEN must be set for input pins
PORT->Group[ 0 ].PINCFG[ PIN_PA27 ].reg = 0x0; //bit PMUXEN must be clear for GPIOs
    	
PORT->Group[ 0 ].DIRCLR.reg = PORT_PA16; //pin 16 declared as data input
PORT->Group[ 0 ].DIRSET.reg = PORT_PA27; //pin 27 TX LED declared as data output
 
while (1) {
if( ( PORT->Group[ 0 ].IN.reg & PORT_IN_IN( PORT_PA16 ) ) == _U_( 0x00000000 ) )
            	PORT->Group[ 0 ].OUTCLR.reg = PORT_PA27;
    	else
            	PORT->Group[ 0 ].OUTSET.reg = PORT_PA27;
}
Thread 1 should:
a) Read the switches from GPIO PORT
b) Send a message to Thread 2 with the switch id (#switch/message) that was pushed
c) Waits and reads the acknowledgement from Thread 2. After receiving the acknowledgement, it is ready to send the next message
Thread 2 should:
a) Receive and read the data from Thread 1
b) Display in the serial port the messages (“Up/Down/Right/Left”)
c) Acknowledge the message reception and waits for the message.
Demonstrate to your professor that these messages were displayed at the PC terminal and show you code. Report your final code including it in your document along with print screens of your program working correctly. Upload the files with your code as well.













In-lab work

1.	Complete and demonstrate PART I. 
2.	Complete and demonstrate PART II.
3.	Complete and demonstrate PART III.

Report
Make and compress ZIP file that contains the next:
●	Document (PDF) with the next content:
1.	Cover
✔	Laboratory Name, Course ID, Semester, Delivery Date, Team members…

2.	Introduction (it is not the objective of the practice document, instead identify the topics of the practice and write about it)

3.	Development (if you include “print screen” images, name the image and explain the meaning of the image)
✓	Answer to the questions and report requests of all the items of PART I, PART II and PART III, including the print screens and diagrams (schematic).
✓	Final version of the code of PART I, PART II and PART III (with comments).

4.	Describe the problems found and solutions

5.	Individual reflection about your learning process (what did you learn with this lab?)

6.	Team conclusion

7.	Bibliography

●	Folder (name it as sources), with the next content:
1.	The project folder with all the source files (.c [ PART I, II, III ], .h, .ld, .s, .ls, etc.) and WITHOUT OBJECT FILES AND EXECUTABLE FILES (.o, .elf, .bin, etc.) You can use the menu option “Build -> Clean Solution” to delete the object and executable files.
2.	Each source file must contain comments about the programming.


