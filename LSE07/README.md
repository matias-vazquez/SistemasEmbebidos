<div align="right">
    <img width=200, src="img/teclogo.png">
</div>

# Laboratory 7. SPI and SD Interfacing with Arduino Zero

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

2. Create a new project on the Atmel Studio as “GCC C Executable Project” for the board “ATSAMD21G18A”.

3. Create the files “spi.h” and “spi.c.” The .h file must contain:
```c
#ifndef SPI_H_
#define SPI_H_

#define BAUDRATE 9600

void spiInit( void );
uint8_t spiSend( uint8_t data );

#endif /* SPI_H_ */
```

4. Now on the “spi.c” file, add the following:
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
```
By reviewing the SAMD21 datasheet on pages 478-510, fill the “??” with their respective values.  Draw a flow chart of the initialization process for the SPI.

5. Add the remain following code to your “spi.c” file:
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
By reviewing the SAMD21 datasheet on pages 478-510, fill the “??” with their respective values. Draw a flow chart indicating the steps to send values using spiSend( ).

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
### __Part III. Initializating the SD__
### __Part IV. Reading the SD sector__
