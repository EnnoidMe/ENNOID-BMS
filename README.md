This repository is part of the open-source project: EVGE - Electric Vehicle Global Equipment:

- EVCE - Electric Vehicle Charge Equipment
  - Onboard Battery Charger
  - Charging station
- EVBE - Electric Vehicle Battery Equipment 
  - **BMS (This repository)**
  - Scalable Battery Pack
- EVPE - Electric Vehicle Powertrain Equipment
  - Motor Driver (https://github.com/EnnoidMe/VESC-controller)
  - Gate driver (https://github.com/EnnoidMe/ENNOID-Dual-Gate-Driver)
  - Scalable Three-Phase Electric Motor

# ENNOID - BMS

This project aims to create a scalable & modular open source BMS for up to 1000V lithium-ion battery packs.

### Updates 

20/02/2018:
The idea behind this project has been started on this thread:

https://endless-sphere.com/forums/viewtopic.php?f=14&t=92952

31/12/2018:
- Copying DieBieMS schematics into kicad
- Switching balancing circuit to LTC6813 
- Adapting the schematics to have isolation between MCU, balancing circuit, current & high voltage measurements.
- Removing high current circuits from the PCB (Traces & FET switches are now removed, the current will flow through external contactor instead. ex: EV-200 contactor)

13/01/2019:

For cost reasons:
- Master & Slave boards will be on a 2 layers PCB 100mm x 100mm
- External power supply 12V or 24V is required for master. 

PCB cost = 10 pcs x (Master+ Slave) = 9.80$ + shipping 

28/01/2018:
- Started LTC6811 slave board design
- Started power supply board design (Will use EPM1205SJ + AD8280 for charging 3 lithium-ion batteries from HV battery pack. Batteries will be mounted on PCB)
- Current sensor PCB from paltatech, now in separate folder

## Features:

**Master board**
- Isolated voltages measurements for Charge, Discharge & Pack buses
- Isolated bi-directional current monitoring via external delta-sigma current sensor
- Isolated CAN bus interface for cell voltage monitoring and charger detection, status monitoring + much more with future updates (like state of charge / state of health).
- Outputs for Charge, Discharge & Precharge contactors
- USB interface for serial communication and firmware upgrades (no need for a programmer -> HW serial bootloader is used).
- Wireless transceiver
- SDCard for logging (to develop a SoC and SoH algorithm)

**Slave boards**
- 18 or 12 Cells balancing/board  (18 for LTC6813 & 12 for LTC6811)
- Cell voltage range 2.5V to 4.5V
- Stackable for up to 1000V packs 
- 9 or 2 Temperatures sensors/board (9 for LTC6813 & 2 for LTC6811)
- Daisychain Master/Slave ISOSPI communication 

**Current sensor board**
- Isolated delta sigma ADC current sensor
- +- 400A capable

**UPS board**
- Isolated 110-450V to 12V DC/DC converter
- Continuous 1A @12V, sevral amps peak
- BQ77904 voltage, current & temperature protection circuit


**Software**

- Firmware, bootloader & configuration tool are the same as [DieBieMS](https://github.com/DieBieEngineering). 
- Software for ENNOID-BMS and DieBieMS are almost identical because the schematics changes related to isolation are invisible for the MCU. Only the switch to LTC6813 slave needs some minimal modifications.

## Components:

- LTC6813 or LTC6811	-> Battery stack cell voltage monitor.
- LTC6820	-> ISOSPI communication.
- STM32F303	-> Microcontroller.
- AMC1301	-> Isolated voltage measurement
- AMC1303	-> Isolated current measurement
- INA226	-> Isolated power measurement.
- BQ76200	-> Driver for Discharge, Charge and Precharge switches.
- LM5165	-> SMPS Buck converter, converting the 24V or 12V voltage to +3.3V.
- ISO1050	-> Isolated CAN-Bus transceiver.
- CP2104-F03	-> USB to serial converter for bootloader based firmware updates and debugging.


## Master

![alt text](Master/PIC/MasterPCB.png)

## Slave LTC6813

![alt text](Slaves/LTC6813/PIC/LTC6813.png)

## Slave LTC6811

![alt text](Slaves/LTC6811/PIC/LTC6811.png)

## UPS

![alt text](UPS/PIC/UPS.png)

## Current sensor

![alt text](Current_Sensor/PIC/Current_Sensor.png)



