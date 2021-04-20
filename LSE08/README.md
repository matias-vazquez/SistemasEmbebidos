# Laboratory 8: CAN interfacing with CANoe and John Deere Demo Box
 
## Objectives
- Strengthen your knowledge and skills on serial data communication protocols, specifically CAN and SAE J1939. 
- Learn how to configure the VECTOR tools, how to set up the CAN controller, and how to send CAN messages, based on the SAE J1939 standard.

## Introduction
This lab provides you the opportunity to learn how to develop embedded software applications using the CAN and SAE J1939 standard. In this lab, you will interface the VN1610 CAN controller from VECTOR to the JD Demonstration Box. 

### Equipment
1 VN1610 Interface from VECTOR
1 CAN-serial cable
1 JD Demonstration Box
1 Protoboard
Wiring cables (male to female pin header cables)

### Software
CANoe 10.0 SP3
SAE J1939 Documentation
SAE J1939 Slides
JD Demo Box 

## Procedure 
### Part I. Initial setup
1. Open software CANoe 10.0 SP3.
2. Click on File->New and select ‘General->Default’ Template.
3. Click on Simulation->Simulation Setup.
4. Expand: CAN Networks / CAN / Channels. 
5. Connect to the PC the VN1610 CAN Interface from VECTOR.
6. Double click on the Network block from the left window.
7. In the Network Hardware Configuration window, select CAN 2 from the left panel. Then click on Scan… in the right panel. 
8. In the Baud Rate Scanner window, select Active for Mode and then click on Scan. A standard 250.000 kBaud should be detected. Then, click OK and OK again.
9. In the Simulation Setup window, right click on the Network block and select Assign Channel… Then, select CAN 2 and click OK.
10.	Go to Home and click on Start. A Trace window will appear. In this window, you can see the CAN messages received from the JD Demonstration Box. Demonstrate to your instructor that the messages are received correctly.

### Part II. Create and send CAN messages

1. __Now, is time to send CAN messages to the JD Demonstration Box.__

2. __First you need to create a Message Database__
   1. In Home, click Stop.
   2. In the Simulation Setup window, right-click on Database and click on Add…
   3. Find and open the ‘EmptyTemplate.dbc’ file in the Database folder:

            (C:\Users\Public\Documents\Vector\CANoe\10.0 (x64)\Templates\Database)

   4. Double click on the created EmptyTemplate database.
   5. In the Vector CANdb++ Editor configure the Message and the Signal to be transmitted. Right-click on Messages and select New…
   6. Before configuring the message, go to the JD Demo Box Datasheet to select the message (PGN and SPN) you want to send. Also you need to be familiar with the J1939 standard (view the example in SAE J1939 Documentation or SAE J1939 Presentation). 
   7. In the Message window, configure and report the messages you select to communicate:
      1. Name: PGN Name.
      2. Type: CAN Extended
      3. ID: All the PGN Identifier (29 bits) for the message. The source address must be A3 always.
      4. DLC: 8.
   8. Click Apply, then OK.
   9. Right-click on Signals and select New…
   10.	In the Signal window, configure and report the signals you select to control:
        1. Name: SPN Name.
        2. Value Type: Unsigned.
        3. Length, Factor (resolution), Unit, Offset, Minimum-Maximum (range): go to the JD Box Datasheet and the desired SPN.
   11. Click Apply, then OK.
   12. Double click on Messages, then double click on the created message. In the Signals bar next to Definition, click on Add… Then, select the created signal and click OK.
   13. In the left panel, expand the Messages / [Name of the Message] and double-click on the created Signal. In the Start bit write the right number according to the standard.
   14. Save changes in the CANdb++ Editor and close.

3.	__Second, you need to create the Interactive Generator block.__
    1. In the Simulation Setup window, right-click on Interactive Generator and select Insert Can Interactive Generator. Double click on the I-Generator block.
    2. In the CAN IG window, click on the first icon (Add Frame from Database…). Select the message you created and click OK.

4.	__Finally, you can send the message.__
    1. Go to Home and click on Start. A Trace window will appear. Go back to the Simulation Setup window and double click on the I-Generator block. In the Phys Value option from the signal you can modify the value to be transmitted and finally click on the Send button. NOTE: Verify that Channel CAN 2 for the message is selected.
    2. Report the values you configure and the output from the JD Demo Box.
    3. Demonstrate to your instructor that the gauge from the JD Demo Box is moving or changing or an indicator is turned on and off. Every team must configure two different messages and signals to control two different gauges or indicators.

## Deliverables
Turn in a technical report that includes the following information:

1. **Introduction**

   * Brief exaplanation of the overall procedure followed on this laboratory

2. **Results**

    Part I
    - [x] _Trace_ window screenshot showing the CAN messages received from the John Deere demo box.

    Part II
    - [x] Configured values and  output from the JD Demo Box.
    - [x] Moving gauge from the JD Demo Box, or an indicator is turned on and off. Every team must configure two different messages and signals to control two different gauges or indicators.


3. **Individual conclusions**

   * Interpretation of results
   * Applications of and improvements of exercises
   * Justification in case of any errors

4. **Appendix A**
   * Link to a video showing your working project and explaining the indicated items for Parts I and II. 