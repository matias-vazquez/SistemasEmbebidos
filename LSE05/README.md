# I2C Interfacing

## Objectives
1 To transmit data trhough the I2C bus using the Raspbian terminal.
2 To implement a bus architecture using the I2C serial protocol with two sensors: a Real Time Clock and Thermometer.

## Pre-lab (Install)
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