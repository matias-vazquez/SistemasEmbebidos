#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MODE_READ 0
#define MODE_WRITE 1

#define MAX_LEN 32

char wbuf[MAX_LEN];

typedef enum {
    NO_ACTION,
    I2C_BEGIN,
    I2C_END
} i2c_init;

uint8_t  init = NO_ACTION;
uint16_t clk_div = BCM2835_I2C_CLOCK_DIVIDER_148;
uint8_t slave_address = 0x00;
uint32_t len = 0;
uint8_t  mode = MODE_READ;

//*******************************************************************************
//  comparse: Parse the command line and return EXIT_SUCCESS or EXIT_FAILURE
//    argc: number of command-line arguments
//    argv: array of command-line argument strings
//*******************************************************************************

int comparse(int argc, char **argv) {
    int argnum, i, xmitnum;
	
    if (argc < 2) {  // must have at least program name and len arguments
                     // or -ie (I2C_END) or -ib (I2C_BEGIN)
        fprintf(stderr, "Insufficient command line arguments\n");
        return EXIT_FAILURE;
    }
    
    argnum = 1;
    while (argnum < argc && argv[argnum][0] == '-') {

        switch (argv[argnum][1]) {

            case 'i':  // I2C init
                switch (argv[argnum][2]) {
                    case 'b': init = I2C_BEGIN; break;
                    case 'e': init = I2C_END; break;
                    default:
                        fprintf(stderr, "%c is not a valid init option\n", argv[argnum][2]);
                        return EXIT_FAILURE;
                }
                break;

            case 'd':  // Read/Write Mode
                switch (argv[argnum][2]) {
                    case 'r': mode = MODE_READ; break;
                    case 'w': mode = MODE_WRITE; break;
                    default:
                        fprintf(stderr, "%c is not a valid init option\n", argv[argnum][2]);
                        return EXIT_FAILURE;
                }
                break;

            case 'c':  // Clock divider
                clk_div = atoi(argv[argnum]+2);
                break;

            case 's':  // Slave address
                slave_address = atoi(argv[argnum]+2);
                break;

            default:
                fprintf(stderr, "%c is not a valid option\n", argv[argnum][1]);
                return EXIT_FAILURE;
        }

        argnum++;   // advance the argument number

    }

    // If command is used for I2C_END or I2C_BEGIN only
    if (argnum == argc && init != NO_ACTION) // no further arguments are needed
        return EXIT_SUCCESS;
	
    // Get len
    if (strspn(argv[argnum], "0123456789") != strlen(argv[argnum])) {
        fprintf(stderr, "Invalid number of bytes specified\n");
        return EXIT_FAILURE;
    }
    
    len = atoi(argv[argnum]);

    if (len > MAX_LEN) {
    	fprintf(stderr, "Invalid number of bytes specified\n");
        return EXIT_FAILURE;
    }
    
    argnum++;   // advance the argument number

    xmitnum = argc - argnum;    // number of xmit bytes

    memset(wbuf, 0, sizeof(wbuf));

    for (i = 0; i < xmitnum; i++) {
        if (strspn(argv[argnum + i], "0123456789abcdefABCDEFxX") != strlen(argv[argnum + i])) {
            fprintf(stderr, "Invalid data: ");
	    fprintf(stderr, "%d \n", xmitnum);
            return EXIT_FAILURE;
        }
        wbuf[i] = (char)strtoul(argv[argnum + i], NULL, 0);
    }

    return EXIT_SUCCESS;
}

//*******************************************************************************
//  showusage: Print the usage statement and return errcode.
//*******************************************************************************
int showusage(int errcode) {
    printf("i2c \n");
    printf("Usage: \n");
    printf("  i2c [options] len [rcv/xmit bytes]\n");
    printf("\n");
    printf("  Invoking i2c results in an I2C transfer of a specified\n");
    printf("    number of bytes.  Additionally, it can be used to set the appropriate\n");
    printf("    GPIO pins to their respective I2C configurations or return them\n");
    printf("    to GPIO input configuration.  Options include the I2C clock frequency,\n");
    printf("    initialization option (i2c_begin and i2c_end).  i2c must be invoked\n");
    printf("    with root privileges.\n");
    printf("\n");
    printf("  The following are the options, which must be a single letter\n");
    printf("    preceded by a '-' and followed by another character.\n");
    printf("    -dx where x is 'w' for write and 'r' is for read.\n");
    printf("    -ix where x is the I2C init option, b[egin] or e[nd]\n");
    printf("      The begin option must be executed before any transfer can happen.\n");
    printf("        It may be included with a transfer.\n");
    printf("      The end option will return the I2C pins to GPIO inputs.\n");
    printf("        It may be included with a transfer.\n");
    printf("    -cx where x is the clock divider from 250MHz. Allowed values\n");
    printf("      are 150 through 2500.\n");
    printf("      Corresponding frequencies are specified in bcm2835.h.\n");
    printf("\n");
    printf("    len: The number of bytes to be transmitted or received.\n");
    printf("    The maximum number of bytes allowed is %d\n", MAX_LEN);
    printf("\n");
    printf("\n");
    printf("\n");
    return errcode;
}

char buf[MAX_LEN];
char vuf[MAX_LEN];

char print1[MAX_LEN];
char print2[MAX_LEN];
char print3[MAX_LEN];

int i, counterRecord=0, setup=0;
char day[7];
uint8_t data;

    bcm2835i2c_setSlaveAddress(slave_address);
int main(int argc, char **argv) {
	
    	day[0]="Sunday";
   	day[1]="Monday";
	day[2]="Tuesday";
	day[3]="Wednesday";
	day[4]="Thursday";
	day[5]="Friday";
	day[6]="Sabadazo";

    printf("Running ... \n");
    
    // parse the command line
    if (comparse(argc, argv) == EXIT_FAILURE) return showusage (EXIT_FAILURE);

    if (!bcm2835_init()) return 1;
      
    // I2C begin if specified    
    if (init == I2C_BEGIN) bcm2835_i2c_begin();

    // If len is 0, no need to continue, but do I2C end if specified
    if (len == 0) {
         if (init == I2C_END) bcm2835_i2c_end();
	 printf("... done!\n");
         return EXIT_SUCCESS;
    }

    bcm2835_i2c_setClockDivider(clk_div);
    fprintf(stderr, "Clock divider set to: %d\n", clk_div);
    fprintf(stderr, "len set to: %d\n", len);
    fprintf(stderr, "Slave address set to: %d\n", slave_address);   
     
	int j, count=1, flag=0;
    while(1){
	   
	
	   data=bcm2835_i2c_write(wbuf,1); //  
   	   bcm2835_i2c_setSlaveAddress(104);
	   data = bcm2835_i2c_read(buf , 7);
	    for(j=0;j<MAX_LEN;j++)
		vuf[j]=buf[j];	  
		
	   data=bcm2835_i2c_write(wbuf,1); 
	   bcm2835_i2c_setSlaveAddress(77);
	   data = bcm2835_i2c_read(buf , 1); 
	   
	   //printf("debug\n");

			if( flag == 0 && buf[0] >= 0x1C){
				flag = 1;
				switch(counterRecord){
	
					case 0:
					for(j=0;j<MAX_LEN;j++)
					print1[j]=vuf[j];
					counterRecord++;
					setup++;
					break;
					case 1:
					for(j=0;j<MAX_LEN;j++)
						print2[j]=vuf[j];
					counterRecord++;
					setup++;	
					break;
					case 2:
					for(j=0;j<MAX_LEN;j++)
						print3[j]=vuf[j];
					counterRecord=0;	
						break;
					default: printf("Hoa\n");
				}
              }

				 printf("Temperatura: %d `C \n", buf[0]);

			if(setup==1){
		 printf("Record %d %x/%x/%x  %d %x:%x:%x PM \n\n",1, print1[4],print1[5],print1[6],print1[3],print1[2],print1[1],print1[0]);}
			else if(setup==2){
		 printf("Record %d %x/%x/%x  %d %x:%x:%x PM \n",1, print1[4],print1[5],print1[6],print1[3],print1[2],print1[1],print1[0]);
		 printf("Record %d %x/%x/%x  %d %x:%x:%x PM \n\n",2, print2[4],print2[5],print2[6],print2[3],print2[2],print2[1],print2[0]);
			}else if(setup>=3){
	     printf("Record %d %x/%x/%x  %d %x:%x:%x PM \n",1, print1[4],print1[5],print1[6],print1[3],print1[2],print1[1],print1[0]);	
		 printf("Record %d %x/%x/%x  %d %x:%x:%x PM \n",2, print2[4],print2[5],print2[6],print2[3],print2[2],print2[1],print2[0]);
		 printf("Record %d %x/%x/%x  %d %x:%x:%x PM \n\n",3, print3[4],print3[5],print3[6],print3[3],print3[2],print3[1],print3[0]);
			}
		if (flag == 1 && buf[0] < 0x1C)
			flag=0;
		sleep(2);			

	}
  

    if (mode == MODE_READ) {
    	for (i=0; i<MAX_LEN; i++) buf[i] = 'n';
    	data = bcm2835_i2c_read(buf, len);
    	printf("Read Result = %d\n", data);   
    	for (i=0; i<MAX_LEN; i++) {
    		if(buf[i] != 'n') printf("Read Buf[%d] = %x\n", i, buf[i]);
	}    
    }
    if (mode == MODE_WRITE) {
    	data = bcm2835_i2c_write(wbuf, len);
    	printf("Write Result = %d\n", data);
    }   

    // This I2C end is done after a transfer if specified
    if (init == I2C_END) bcm2835_i2c_end();   
    bcm2835_close();
    printf("... done!\n");
    return 0;
}
