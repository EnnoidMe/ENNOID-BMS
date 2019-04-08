Related ENNOID projects:
  - [Onboard Battery Charger](https://github.com/EnnoidMe/ENNOID-Charger)
  - [BMS](https://github.com/EnnoidMe/ENNOID-BMS)
  - [Scalable Battery Pack](https://github.com/EnnoidMe/ENNOID-PACK)
  - [Motor Drive](https://github.com/EnnoidMe/VESC-controller)
  - [Dual Isolated Gate driver](https://github.com/EnnoidMe/ENNOID-Dual-Gate-Driver)

# ENNOID - BMS

This project aims to create a scalable & modular open source BMS for up to 1000V lithium-ion battery packs.

The idea behind this project has been started on this thread:

https://endless-sphere.com/forums/viewtopic.php?f=14&t=92952

## Features:

**Master board**
- Isolated voltages measurements for Discharge & Pack buses
- Isolated bi-directional current monitoring via external delta-sigma current sensor
- Isolated CAN bus interface 
- Outputs for Charge, Discharge & Precharge contactors
- USB interface for serial communication and firmware upgrades (no need for a programmer -> HW serial bootloader is used).
- Wireless transceiver
- SDCard for logging (to develop a SoC and SoH algorithm)

**Slave boards**
- 18 or 12 Cells balancing/board  (18 for LTC6813 & 12 for LTC6811)
- Cell voltage range 2.5V to 4.5V
- 2 watt power dissipation per level
- Stackable for up to 1000V packs 
- 9 or 5 Temperatures sensors/board (9 for LTC6813 & 5 for LTC6811)
- Daisychain Master/Slave ISOSPI communication 

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


## Slave LTC6811

![alt text](Slaves/LTC6811/PIC/LTC6811.png)





