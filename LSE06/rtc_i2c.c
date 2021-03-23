/*
 * RTC_i2C.c
 *
 * Created: 26/09/2019 10:05:21 p. m.
 * Author : Asus X556U
 */ 

#include "sam.h"
#include "stdbool.h"

#define SLAVE_ADDR 0x68u
#define BUF_SIZE 4
#define DAY_ADDR 0x03u

// Control variables for data transfer and reception
volatile uint32_t i;
uint8_t tx_buf[BUF_SIZE] = {DAY_ADDR, 0x03u, 0x22u, 0x04u};
uint8_t rx_buf[BUF_SIZE];

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	
	/* Switch to 8MHz clock (disable prescaler) */
	SYSCTRL->OSC8M.bit.PRESC = 0;
	
	/* port mux configuration */
	PORT->Group[0].PINCFG[PIN_PA22].reg = PORT_PINCFG_PMUXEN | PORT_PINCFG_INEN | PORT_PINCFG_PULLEN; /* SDA */
	PORT->Group[0].PINCFG[PIN_PA23].reg = PORT_PINCFG_PMUXEN | PORT_PINCFG_INEN | PORT_PINCFG_PULLEN; /* SCL */
	
	/* PMUX: even = n/2, odd: (n-1)/2 */
	PORT->Group[0].PMUX[11].reg |= 0x02u;
	PORT->Group[0].PMUX[11].reg |= 0x20u;
	
	/* APBCMASK */
	PM->APBCMASK.reg |= PM_APBCMASK_SERCOM3;

	/*GCLK configuration for sercom3 module*/
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID (SERCOM3_GCLK_ID_CORE) |
	GCLK_CLKCTRL_ID (SERCOM3_GCLK_ID_SLOW) |
	GCLK_CLKCTRL_GEN(4) |
	GCLK_CLKCTRL_CLKEN;
	GCLK->GENCTRL.reg |= GCLK_GENCTRL_SRC_OSC8M|GCLK_GENCTRL_GENEN|GCLK_GENCTRL_ID(4);

	/* set configuration for SERCOM3 I2C module */
	SERCOM3->I2CM.CTRLB.reg = SERCOM_I2CM_CTRLB_SMEN; /* smart mode enable */
	while (SERCOM3->I2CM.SYNCBUSY.reg); // waiting loading
	
	/* calculate BAUDRATE */
	uint64_t tmp_baud =((8000000/100000)-10-(8000000*250 /1000000000))/2;
	SERCOM3->I2CM.BAUD.bit.BAUD = SERCOM_I2CM_BAUD_BAUD((uint32_t)tmp_baud);
	while (SERCOM3->I2CM.SYNCBUSY.reg); // waiting loading
	// value equals 0x22 or decimal 34
	
	SERCOM3->I2CM.CTRLA.reg = SERCOM_I2CM_CTRLA_ENABLE   |/* enable module */
	SERCOM_I2CM_CTRLA_MODE_I2C_MASTER |		/* i2c master mode */
	SERCOM_I2CM_CTRLA_SDAHOLD(3);		 /* SDA hold time to 600ns */
	while (SERCOM3->I2CM.SYNCBUSY.reg);  /* waiting loading */

	SERCOM3->I2CM.STATUS.reg |= SERCOM_I2CM_STATUS_BUSSTATE(1); /* set to idle state */
	while (SERCOM3->I2CM.SYNCBUSY.reg);  /* waiting loading */
	
	/******************  SENDING SEQUENCE  **********************/
	// Sending 4 bytes to an RTC. First Byte is the pointer of the RTC memory.
	/* slave address with write signal (0) */
	SERCOM3->I2CM.ADDR.reg = (SLAVE_ADDR << 1) | 0;    /* Sending slave address in write mode */
	while(!SERCOM3->I2CM.INTFLAG.bit.MB);		   /* MB = 1 if slave NACKS the address */
	
	for(i=0; i<BUF_SIZE; i++) {
		/* placing the data from transmitting buffer to DATA register*/
		SERCOM3->I2CM.DATA.reg = tx_buf[i];
		while(!SERCOM3->I2CM.INTFLAG.bit.MB){}; /* MB = 1 if slave NACKS the address */
	}

	/* After transferring the last byte stop condition will be sent */
	SERCOM3->I2CM.CTRLB.bit.CMD = 0x3;
	
	/******************  RELOCATING POINTER BEFORE RECEIVING  *************************/
	/* slave address with write signal (0) */
	SERCOM3->I2CM.ADDR.reg = (SLAVE_ADDR << 1) | 0;    /* Sending slave address in write mode */
	
	while(SERCOM3->I2CM.INTFLAG.bit.MB ==0);		   /* MB = 1 if slave NACKS the address */
	SERCOM3->I2CM.DATA.reg = DAY_ADDR;                 /* Sending address (seconds) for internal pointer */
	
	while(SERCOM3->I2CM.INTFLAG.bit.MB ==0){};         /* MB = 1 if slave NACKS the address */
	
	SERCOM3->I2CM.CTRLB.bit.CMD = 0x1;                 /* Sending repetead start condition */	
	
	/******************  RECIEVING SEQUENCE  *************************/
	/* slave address with read signal (1) */
	SERCOM3->I2CM.ADDR.reg = (SLAVE_ADDR << 1) | 1;
	while(SERCOM3->I2CM.INTFLAG.bit.SB==0){};

	for(i=0; i< BUF_SIZE-1; i++) {
		rx_buf[i] = SERCOM3->I2CM.DATA.reg;
		while(SERCOM3->I2CM.INTFLAG.bit.SB==0){};
	}
	
	/* After receiving the last byte stop condition will be sent */
	SERCOM3->I2CM.CTRLB.bit.CMD = 0x3;
	
	// Turn off the I2C
	SERCOM3->I2CM.CTRLA.bit.ENABLE = 0x0u;
}