# I2C Interfacing

## Objectives
1. To transmit data trhough the I2C bus using the Raspbian terminal.
2. To implement a bus architecture using the I2C serial protocol with two sensors: a Real Time Clock and Thermometer.

## Pre-lab (Installation)
1. Open the RXTerminal and install the NTPdate utility by typing in the following:
```
$sudo apt-get install build-essential manpages-dev ntpdate
```
2. Add the main library. Go to your web browser inside the Raspbian Environment and go to this page: http://www.airspayce.com/mikem/bcm2835/bcm2835-1.58.tar.gz
```
$ wget "http://www.airspayce.com/mikem/bcm2835/bcm2835-1.58.tar.gz"
```
3. Uncompress the downloaded package by:
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

## Lab work 1 (Hardware and Fire Test)

## Lab work 2 (Bus topology) 
Both, the TC74 (check out the datasheet at "https://ww1.microchip.com/downloads/en/DeviceDoc/21462D.pdf") and the RTC should be connected hooked to the same I2C bus and must work as Slaves; the Raspberry Pi should be setup as Master. Report your files with the final code. Report the commands to build and execute the program. Report printouts illustrating the correct operation of your program. Demonstrate to the professor your program working correctly.

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
