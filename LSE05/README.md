# I2C Interfacing with C (draft)

## Objectives
1. To transmit data through a I2C bus using the Raspbian terminal.
2. To implement a bus architecture using the I2C serial protocol with two sensors: a Real Time Clock (RTC) and Thermometer.

## Pre-lab (Setup)
1. Open the RXTerminal and install the **NTPdate** utility by typing in the following command:
```
$ sudo apt-get install build-essential manpages-dev ntpdate
```
2. Download the C library for Broadcom BCM 2835 microprocessor (detailed info at http://www.airspayce.com/mikem/bcm2835):
```
$ wget "http://www.airspayce.com/mikem/bcm2835/bcm2835-1.58.tar.gz"
```
3. Uncompress the downloaded package:
```
$ sudo tar zxvf bcm2835-1.58.tar.gz
```
4. Once unpackaged, proceed to install the software:
```
$ cd bcm2835-1.58
$ ./configure
$ make
$ sudo make check
$ sudo make install
```
5. To verify the installation, go inside the bcm2835-1.58 folder and locate the folder with the name src. From the Terminal, insert the next command to go to this subfolder and introduce the following commands:
```
$ make
$ sudo ./test
```
6. IF SOMETHING GOES WRONG. On terminal, type in:
```
$ sudo raspi-config
```
  - Choose option (5) Interfacing Options:
  - Then in the list, select option (P5) I2C. Activate by selecting YES.
  - It is necessary to restart the Raspberry Pi to take the change into account.
  ```
  $ sudo reboot
  ```
  - Then, try again step 5.

## Lab work
### Hardware and Fire Test
Investigate the hardware arrangements (Raspberry Pi 3 + Tiny RTC i2C modules DS1307 + TC74). Report the final schematic.

In your project, the Raspberry Pi must be able to communicate with booth devices. You can install the i2c tools to test whether the modules are fully loaded. To install the tools to communicate with i2c devices in Python (python-smbus) and diagnostic tools (i2c-tools).
```
$ sudo apt-get install -y python-smbus i2c-tools
```
Once the installation is complete, you can already test whether the modules are fully loaded:
```
$ lsmod | grep i2c_
```
For test the connection and find out the device address on the i2c bus, you can execute on the terminal next command:
```
$ sudo i2cdetect -y 1
```
If everything goes well you must see on terminal something like:

Report all the required steps and the printout of each step. 

Inside the folder bcm2835, you should find the examples folder with the code for the I2C. Go there from Terminal and compile it (you will need files bcm2835.c and bcm2835.h on this folder; you may acquire them from the src folder). Here is a hint:
```
$ gcc -o i2c i2c.c bcm2835.c
```
Identify the following code sections in the I2C.c file. Report these findings in your report document.

1. How to begin the transfer?
2. How to set the address of the slave?
3. How to define read write operations?
4. What is the meaning of  dlen ?

Try to run this program and obtain the data of seconds, minutes and hours from the RTC (you can review the DS1307 data sheet from this link). Display this info at the terminal. Your program should also set the current date and time onto the RTC. Demonstrate to your professor.  Demonstrate how to read individually each location? of the RTC (seconds, minutes, etc.) by adjusting the RTC internal pointer. Document the commands and show print screens. Also show how to read all the memory locations at once? using the commands. Show the commands and print screens.

### Bus Topology
Both, the TC74 (check out the datasheet at https://ww1.microchip.com/downloads/en/DeviceDoc/21462D.pdf) and the RTC should be hooked up to the same I2C bus and must work as Slaves; the Raspberry Pi should be setup as Master. Report your files with the final code. Report the commands to build and execute the program. Report printouts illustrating the correct operation of your program. Demonstrate to the professor your program working correctly.

Display the collected data on the terminal and log it in, in a .txt file. The logged data should follow the format showed below:
```
RECEIVER> Temperature: 24°C
RECEIVER> Record 1: 12/09/18 Sat 08:55:44 PM
RECEIVER> Record 2: 12/09/18 Sat 09:01:35 PM
RECEIVER> Record 3: 12/09/18 Sat 09:08:20 PM
```
Consider the following requeriments:
* The recording table can only contain the 3 more recent events.
* These records must be initialized with the following date:
```
01/01/01 Mon 12:00:00 AM	
```
* If all the records are used, you must replace the oldest with the new one.
* You should report the temperature and the records every 10 seconds. 
