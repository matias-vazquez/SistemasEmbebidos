# I2C Interfacing with C (draft)

## Objectives
1. To transmit data through a I2C bus using the Raspbian terminal.
2. To implement a bus architecture using the I2C serial protocol with two sensors: a Real Time Clock (RTC) and Thermometer.

## Pre-lab (Setup)
1. Open the Terminal and enable the I2C port and the Device Tree:
```
$ sudo raspi-config
```
   * Go to *Interfacing Options* 

![Interfacing options screen](img/fig01.png)

   * and enable **P5 I2C**

![I2C enabling screen](img/fig02.png)

2. Then, restart the Raspberry Pi:
```
$ sudo reboot
```
3. From a new Terminal window, install the **NTPdate** utility by typing in the following command:
```
$ sudo apt-get install build-essential manpages-dev ntpdate
```
4. Download the C library for Broadcom BCM 2835 microprocessor (detailed info at http://www.airspayce.com/mikem/bcm2835):
```
$ wget "http://www.airspayce.com/mikem/bcm2835/bcm2835-1.58.tar.gz"
```
5. Uncompress the downloaded package:
```
$ sudo tar zxvf bcm2835-1.58.tar.gz
```
6. Once unpackaged, install the library:
```
$ cd bcm2835-1.58
$ ./configure
$ make
$ sudo make check
$ sudo make install
```

## Lab work
### Part I. Hardware and Fire Test
The hardware setup, including the Raspberry Pi 3 + the Tiny RTC i2C modules DS1307 + the TC74 temperature sensor is shown below:
![Hardware setup](img/fig04.png)

In your project, the Raspberry Pi must be able to communicate with both devices through the I2C protocol. To install the Python tools that allow I2C communication between the Raspberry Pi and hooked devices, as well as diagnostic tools, follow the next procedure:
```
$ sudo apt-get install -y python-smbus i2c-tools
```
Once the installation is complete, you can already test whether the modules are fully loaded:
```
$ lsmod | grep i2c_
i2c_bcm2835		16384 0
i2c_bcm2708		16384 0
```
You need the module i2c_bcm2708 (or i2c_bcm2835 on the Raspberry Pi Zero W). If you see the either modules, they are correctly loaded. To test the connections and find out the devices address on the i2c bus, execute:
```
$ sudo i2cdetect -y 1
```
If everything goes well, you must see on something like:

![1i2 detect screen](img/fig03.png)

**Report all the required steps and the printout of each step.**

Inside the folder bcm2835, go to the I2C example following the route *bcm2835-1.58/examples/i2c/*. Compile it (you will need files bcm2835.c and bcm2835.h on this folder; you may acquire them from the *src* folder). Here is a hint:
```
$ gcc -o i2c i2c.c ../../bcm2835.c
```
Identify the corresponding code sections in i2c.c (the full source code can be also found at http://www.airspayce.com/mikem/bcm2835/i2c_8c-example.html) and answer the following:
1. How to begin the transfer?
2. How to set the address of the slave?
3. How to define read write operations?
4. What is the meaning of  dlen ?

### Part II. Bus Topology
Backup the i2c.c file and modify a new copy to obtain the data of seconds, minutes and hours from the RTC (checkout the DS1307 datasheet at https://datasheets.maximintegrated.com/en/ds/DS1307.pdf), and display it on the terminal. Your program should also set the current date and time onto the RTC. Demonstrate how to read individually each location of the RTC (seconds, minutes, etc.) by adjusting the RTC internal pointer. Document the commands and take screenshots. Also, show how to read all the memory locations at once using the commands. Show the commands and screenshots.

Both, the TC74 (check out the datasheet at https://ww1.microchip.com/downloads/en/DeviceDoc/21462D.pdf) and the RTC should be hooked up to the same I2C bus and must work as *slaves*; the Raspberry Pi should be set as *master*. Report your files with the final code. Report the commands to build and execute the program. Report printouts illustrating the correct operation of your program. Demonstrate to the professor your program working correctly.

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
