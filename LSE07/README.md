<div align="right">
    <img width=200, src="img/teclogo.png">
</div>

# Laboratory 7. SPI and SD Interfacing with Arduino Zero (draft)

## **Introduction**
This lab provides you the opportunity to learn how to develop embedded software applications by following the steps and practices commonly used in industry. In this lab, you will learn to write C language code using the header files and source files provided by ATMEL. You will also develop your own header and source files to access and manipulate a SD shield. This lab will also provide you the opportunity to strengthen your knowledge about the SPI protocol by programming an interface to the SD shield and by observing and measuring the SPI signals using the oscilloscope. 

### Objectives
* Strengthen your knowledge and abilities about C programming and serial communication protocols, specifically SPI. 
* Learn how to write programs using the SPI protocol to interface to an SD card.

### Materials and software
1 Arduino Zero
1 USB-microusb cable
1 SD (4GB) formatted at FAT32
1 SD shield 
1 Oscilloscope
1 FTDIO for serial communication
1 USB-miniusb cable
Wiring cables (male to female pin header cables)
The latest code for “myprintf” and “uart.c” and “uart.h”
The latest version of Atmel must be installed on a computer running Windows OS.
The latest version of putty must be installed on a computer running Windows OS.


## **Procedure**

### __Part I. SPI protocol and interfacing with the SD shield__
1. Connect the SPI port from the SAMD21 microcontroller to the SD shield. Report an schematic diagram with the connections between the Arduino Zero and the microSD shield.

<img src="img/sd_shield.png">
<img src="img/arduino_connection.png">

2. Create a new project on the Atmel Studio as “GCC C Executable Project” for the board “ATSAMD21G18A”.

3. Create the header file _spi.h_, and paste in the following code listing:
```c
#ifndef SPI_H_
#define SPI_H_

#define BAUDRATE 9600

void spiInit( void );
uint8_t spiSend( uint8_t data );

#endif /* SPI_H_ */
```

4. Then, create _spi.c_, and add the following code listing:
```c
void spiInit( void ) {
	/* Switch to 8MHz clock (disable prescaler) */
	SYSCTRL->OSC8M.bit.PRESC = 0;

	PM->APBCMASK.bit.SERCOM1_ = 1; //enable the clock for SERCOM1 peripheral
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_SERCOM1_CORE;
	while( GCLK->STATUS.bit.SYNCBUSY ) { }
	
	//structures to configure the SERCOM1 peripheral
	const SERCOM_SPI_CTRLA_Type ctrla = {
		.bit.DORD = ??, // MSB first
		.bit.CPHA = ??, // Mode 0
		.bit.CPOL = ??,
		.bit.FORM = ??, // SPI frame
		.bit.DIPO = ??, // MISO on PAD[3]
		.bit.DOPO = ??, // MOSI on PAD[0], SCK on PAD[1], SS_ on PAD[2]
		.bit.MODE = ??  // Master Mode
	};
	SERCOM1->SPI.CTRLA.reg = ctrla.reg;	
	const SERCOM_SPI_CTRLB_Type ctrlb = {
		.bit.RXEN = ??,   // RX enabled
		.bit.MSSEN = ??,  // Manual SC
		.bit.CHSIZE = ??  // 8-bit
	};
	SERCOM1->SPI.CTRLB.reg = ctrlb.reg;

	//Formula to configure the desired baudrate
	uint32_t br = ( double )( 8000000 / ( 2 * BAUDRATE ) ) - 1 ;
	SERCOM1->SPI.BAUD.reg = SERCOM_SPI_BAUD_BAUD( ( uint8_t )br );

	//structure to configure multiple PINs
	const PORT_WRCONFIG_Type wrconfig = {
		.bit.WRPINCFG = 1,
		.bit.WRPMUX = 1,
		.bit.PMUX = MUX_PA16C_SERCOM1_PAD0 | MUX_PA17C_SERCOM1_PAD1 | MUX_PA19C_SERCOM1_PAD3,
		.bit.PMUXEN = 1,
		.bit.HWSEL = 1,
		.bit.PINMASK = ( uint16_t )( ( PORT_PA16 | PORT_PA17 | PORT_PA19 ) >> 16 )
	};
	PORT->Group[0].WRCONFIG.reg = wrconfig.reg;

	//SS/CS (Slave Select/Chip Select) PIN 18 configuration
	REG_PORT_DIRSET0 = PORT_PA18;
	REG_PORT_OUTSET0 = PORT_PA18;
	
	//enable the SPI
	SERCOM1->SPI.CTRLA.bit.ENABLE = 1;
	while( SERCOM1->SPI.SYNCBUSY.bit.ENABLE ) { }
}
```
Read pages 478-510 of the SAMD21 datasheet and replace the `??` in the code above with the respective values. Draw a flow chart of the initialization process for the SPI.

5. Add the remain following code to your _spi.c_ file:
```c
uint8_t spiSend( uint8_t data ) {
	uint8_t temp;
	while( ?? ) { } //wait until buffer is empty
	?? = SERCOM_SPI_DATA_DATA( data ); //transmit data
	while( ?? ) { } //wait until a data is received
	temp = ??; //read data
	while( !SERCOM1->SPI.INTFLAG.bit.TXC ) { } //wait until there is no data to transmit
	myprintf( " %x", temp ); //printf the value in putty
	return temp;
}
```
Continue replacing the “??” with their respective values for the function `spiSend()`. Draw a flow chart indicating the steps to send values using spiSend( ).

6. Capture the following code in your main program.
```c
int main(void)
{
   	SystemInit();
   	spiInit();
 
   	volatile uint8_t rData;
   	volatile uint8_t sData = 85;
   	while (1) {
			REG_PORT_OUTCLR0 = PORT_PA18; //initiate transaction by SS_low
          	rData = spiSend( sData );
			REG_PORT_OUTSET0 = PORT_PA18; //finish transaction by SS_high
   	}
}
```
7. Share your code written in C language and the program working correctly with the professor. Remember to write the code using functions. Add the image of the generated SPI signal over the MOSI and SS channels. Justify the expected waveform, where the observed signal at the oscilloscope the expected waveform?

8. Add the “myprint.h” and “myprint.c” files to the project. Create and define the content of the “uart.c” and “uart.h” files. These files should have the code needed to configure the serial port UART to be used by the myprintf( ) function to display data at the terminal. Report and demonstrate your files.

### __Part II. Communicating with the SD__
The SD has a set of commands available for communication. For further information on this, check out lecture on SPI and SD cards from Universitty at Buffalo:
<div align="center">

[![Generic badge](https://img.shields.io/badge/SPI_and-SD_Cards-orange.svg)](http://www.dejazzer.com/ee379/lecture_notes/lec12_sd_card.pdf)
</div>

The following table shows relevant commands with arguments and their corresponding response: 

<div align="center">

Command | ID | Argument | Response
:--- | :--- | :--- | :---
Go Idle State | CMD0 | 0 | R1
Send If Cond | CMD8 | ... | R7
Stop Transmission | CMD12 | 0 | R1
Set Block Length | CMD16 | Length | R1
Read Single Block | CMD17 | Block | R1
Read Multiple Blocks | CMD18 | Start Block | R1
Application Command | CMD55 | 0 | R1
Read OCR | CMD58 | ... | R3
</div>

To communicate with the SD card, your program has to place the SD card into the SPI mode. To do this, set the MOSI and CS/SS lines to logic value 1 and toggle SD CLK for at least 74 cycles. After the 74 cycles (or more) have occurred, your program should set the CS line to 0 and send the command CMD0:

`01000000 00000000 00000000 00000000 00000000 10010101`

The command is captured in C language as illustrated below:

```c
#define kCMD00 0x40
#define SIZE_SD_CMD 0x06
const uint8_t CMD00[SIZE_SD_CMD]  ={0x40, 0x00, 0x00, 0x00, 0x00, 0x95};
```

This is the reset command, which puts the SD card into the SPI mode if executed when the CS/SS line is low. The SD card will respond to the reset command by sending a basic 8-bit response on the MISO line. The structure of this response is shown in Fig. 2 below. The first bit is always a 0, while the other bits specify any errors that may have occurred when processing the last message. If the command you sent was successfully received, then you will receive the message (00000001).

<img src="img/command_responses.png">

To receive this message, your program should continuously toggle the SD CLK signal and observe the MISO line for data, while keeping the MOSI line high and the CS line low. Your program can detect the message, because every message begins with a 0 bit, and when the SD card sends no data it keeps the MISO line high. Note that the response to each command is sent by the card a few SD CLK cycles later. If the expected response is not received within 16 clock cycles after sending the reset command, the reset command has to be sent again.
Following a successful reset, test if your system can successfully communicate with the SD card by sending a different command. For example, send one of the following commands, while keeping the CS at value 0:

`01001000 00000000 00000000 00000001 10101010 10000111`

(CMD08) - request the contents of the operating conditions register for the connected card.

A response to these commands consists of 40 bits, where the first 8 bits are identical to the basic 8-bit response, while the remaining 32 bits contain specific information about the SD card. If successful, the first 8 bits of the response will be either 00000001 or 00000101 depending on the version of your SD card.

#### _Sending commands to the SD_
Complete the C-language program illustrated below that allows a user to input a 48-bit command on the keyboard and send it to the SD card via SPI. 

1. Complete the line associated to the CMD08 command. Type the missing hex values.

2. Review the function spiXchg( ). Identify the part of this function that sends the command to the SD. Report the identified code and a flowchart of this part. 

3. Review the function spiXchg( ). Identify the part of this function that receives the response from the SD. Report the identified code and a flowchart of this part. 

4. Complete the main program with the code to send the first two commands (CMD00 and CMD08) to the SD and receive the two responses from the SD. Identify the responses and explain their meaning. Report the code and the interpretation of the responses. Demonstrate to the professor the execution of the program and the interpretation of responses. 

Review the program and the values displayed at the terminal, report the answer to these questions.

1. When sending each byte of the command, what is the value being received from the SD?

2. When receiving the response from the SD, what is the value being sent to the SD?

```c
#define RXBUFSIZE 0x400
#define LENGTH_R1 0x03
#define LENGTH_R7 0x07

void initCycles(void);

#define SIZE_SD_CMD 0x06
#define kCMD00 0x40
#define kCMD08 0x48
#define kCMD55 0x77
#define kCMD41 0x69

const uint8_t CMD00[SIZE_SD_CMD]  ={0x40, 0x00, 0x00, 0x00, 0x00, 0x95};
const uint8_t CMD08[SIZE_SD_CMD]  ={__________________________________};
uint8_t CMD17[SIZE_SD_CMD]  ={0x51, 0x00, 0x00, 0x00, 0x00, 0x01};
uint8_t CMD172[SIZE_SD_CMD]  ={0x51, 0x00, 0x00, 0x08, 0x00, 0x01};
const uint8_t CMD18[SIZE_SD_CMD]  ={0x52, 0x00, 0x00, 0x00, 0x00, 0x01};
const uint8_t CMD55[SIZE_SD_CMD]  ={0x77, 0x00, 0x00, 0x00, 0x00, 0x65};
const uint8_t CMD41[SIZE_SD_CMD] = {0x69, 0x40, 0x00, 0x00, 0x00, 0x77};

uint8_t RxBuffer[RXBUFSIZE];

int main(void)
{
  uint32_t temp = 0xFF;
  /* Initialize the SAM system */
  SystemInit();

  initUART();
  spiInit();

  initCycles();
  //myprintf("\n");
	
  spiXchg( CMD00, SIZE_SD_CMD, RxBuffer );  /* reset instruction */
  myprintf("\n");

  //Write your code here

  myprintf("\nDone");
}

void initCycles(void){
	uint32_t i;
	REG_PORT_OUTSET0 = PORT_PA18;
	for(i=0;i<76;i++)
	spiSend(0xFF);
}

//ADD spiXchg FUNCTION TO THE “spi.c” FILE

uint32_t spiXchg(const uint8_t * send_buff, uint32_t bc, uint8_t * receive_buff ) {
	uint8_t temp = 0xFF;
	uint32_t i;
	uint8_t temp_cmd = send_buff[0];
	
	REG_PORT_OUTCLR0 = PORT_PA18;
	for(i=0; i< bc; i++) {
		temp = spiSend(*(send_buff++));
		myprintf(" %x", temp);
	}
	switch(temp_cmd) {
		case kCMD00 :
			for(i=0; i<LENGTH_R1; i++) {
				temp = spiSend(0xFF);
				myprintf(" %x", temp);
			}
			break;
		case kCMD08 :
			for(i=0; i<LENGTH_R7; i++) {
				temp = spiSend(0xFF);
				myprintf(" %x", temp);
			}
			break;
		case kCMD41 :
			for(i=0; i<LENGTH_R1-1; i++) {
				temp = spiSend(0xFF);
				myprintf(" %x", temp);
			}
			spiSend(0xFF);
			break;
		case kCMD55 :
			for(i=0; i<LENGTH_R1; i++) {
				temp = spiSend(0xFF);
				//myprintf(" %x", temp);
			}
			break;
		default :
			myprintf("\n Error in CMD");
	}
	REG_PORT_OUTSET0 = PORT_PA18;
	return(temp);
}
```

### __Part III. Initializating the SD__
In this part, you will complete the SD card initialization in the main program. To initialize the card correctly, you will have to send several commands in a sequence and process the responses received. Once you complete this procedure, you will be able to access the data stored on the SD card.

The steps necessary to complete the SD card initialization are shown in the flowchart in Fig. 3. The flowchart consists of boxes with a specific command at each box. Starting at the top of the flowchart, each command has to be sent to the SD card, and a response has to be received. In some cases, a response from the card needs to be processed to decide the next course of action, as indicated by the diamond-shaped decision boxes.

<img src="img/flow_diagram.png">


To begin initialization, the program should set the MOSI and CS lines to logic 1 and toggle the SD CLK for at least 74 cycles. Next, the program should follow the initialization procedure outlined in Fig. 3, while keeping the CS line low at all times. The first command to send is the reset command; CMD0 sets the SD card into SPI mode. The SD card should respond to this command with the 8-bit message (00000001) indicating no errors. If the SD fails to respond, the command should be sent again. The second command, CMD08, determines if the SD card is compliant with version 2.00 of the SD card specifications. In reply to CMD08, the SD card will issue a 40-bit response. The response indicates how to proceed with the initialization. In particular, if bit 34 is set to 1 then the SD card is an older model and should be initialized following the left branch of the flowchart in Fig. 3. If the command is valid and no other errors occurred, then verify if the check pattern in the response is 10101010 and that the voltage field is set to 0001. If either of these conditions fails, the SD card will not function correctly and you should stop the initialization procedure. Otherwise, proceed with the initialization steps of version 2.00 compliant SD card, following the right-hand side of the flowchart. In the case of an older SD card, the next step of the initialization procedure is to send CMD58 to obtain the contents of the Operating Conditions Register (OCR). The OCR indicates the voltage levels the SD card can work with. To determine if the SD card is ready for exchange of data, the program should send two commands, CMD55 followed by ACMD41, to the SD card. The argument in each of these commands must be 0. After each command is sent, the SD card will respond. Of particular interest is the response to command ACMD41. The response is a basic 8-bit response. It is expected that the bit ”in idle state” will remain as 1 for a while. However, the bit will be set to 0 when the card is ready. If it is not, send the pair of commands again and again until the response received is 00000000 (zero).

Initializing the SD. Complete the main program to initialize the SD card as described above. The program should send the sequence of commands (all with the argument field set to 0, except for CMD08) to initialize the SD card and display the response to each command in the terminal window. When needed, your program should scan the response and proceed with the initialization only if the correct response has been received.

a.	What is the purpose of the initCycles(  ) function? Report the answer to this question.

b.	Complete the command sequence at the main program just below the code written for PART II. Write the code to send the additional commands CMD55 and CMD41 (also known as ACMD41). The code should make sure that the SD card is ready to be able to continue to PARTIV. Report the modifications to the main program. Demonstrate to your professor that the program checks for the responses and continues when the SD is ready (response is zero).

### __Part IV. Reading the SD sector__
In this part, you will use a read command to access data on the SD card. CMD17 can be used to read a 512-byte segment of data from the card. A standard-capacity SD card takes as argument the byte-wise address of the data to be accessed on the card and returns a block of 512 bytes that begins at the specified address. The command is only valid if the argument is a multiple of 512. Any other address passed as an argument will result in a response with the “address error” bit set. If a correct address is provided as an argument in CMD17 and the card is able to access the specified memory segment, a response 00000000 (zero) giving an indication of no errors will be sent by the SD card. Following this response, the card will immediately begin sending the requested data on the MISO line. The data sent by the card should be ignored until a byte comprising (0xFE) is received. This byte signifies the beginning of a data packet.

a.	Review the function rcvr_datablock( ). Identify the part of this function that sends the command to the SD.

i.	Complete the code that is missing.
ii.	Report the identified code and a flowchart of this part. 
iii.	Report the answer to the following questions:

1.	What arguments of the function are related to this part of the code? What information are these arguments providing?

2.	Explain how the CMD17 command argument is being passed to the SD. What is this argument for?

b.	Review the function rcvr_datablock( ). Identify the part of this function that receives the response from the SD. Report the identified code and a flowchart of this part. 
What arguments of the rcvr_datablock function are related to this part of the code? What information are these arguments providing? Report the answer to these questions.

c.	Complete the main program with the code to read the first 512-byte sector of the SD. Identify the responses and explain their meaning. Report the code and the interpretation of the responses. Demonstrate to the professor the execution of the program and the interpretation of responses. Report the content of the memory locations of the first 512-byte block of the SD. Demonstrate the content of this first block to the professor.

```c
void rcvr_datablock(const uint8_t * send_buff, uint32_t lba, uint8_t * receive_buff, uint32_t bs ) {
	uint8_t temp = 0xFF;
	uint32_t i;
	
	REG_PORT_OUTCLR0 = PORT_PA18;
	myprintf("\n\n");

	temp = send_buff[0];
	temp = spiSend(temp);
	myprintf(" %x", temp);
	temp = ((uint8_t*)&lba)[3];
	temp = spiSend(temp);
	myprintf(" %x", temp);

	// Complete the code that is missing

	temp = send_buff[5];
	temp = spiSend(temp);
	myprintf(" %x", temp);

	// Reading to find the beginning of the sector

	temp = spiSend(0xFF);
	while(temp != 0xFE){
		temp = spiSend(0xFF);
		myprintf(" %x", temp);
	}
	
	// Receiving the memory sector/block
	
	myprintf("\n\n");
	for(i=0; i< bs; i++) {
		while(SERCOM1->SPI.INTFLAG.bit.DRE == 0);
		SERCOM1->SPI.DATA.reg = 0xFF;
		while(SERCOM1->SPI.INTFLAG.bit.TXC == 0);
		while(SERCOM1->SPI.INTFLAG.bit.RXC == 0);
		temp = SERCOM1->SPI.DATA.reg;
		*(receive_buff++) = temp;
		myprintf(" %x", temp);
	}
	REG_PORT_OUTSET0 = PORT_PA18;
	myprintf("\n\n");
}
```

## Deliverables