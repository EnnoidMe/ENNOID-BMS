This repository is part of the open-source project: EVGE - Electric Vehicle Global Equipment:

- EVSE - Electric Vehicle Supply Equipment 
- EVCE - Electric Vehicle Charge Equipment
- EVBE - Electric Vehicle Battery Equipment 
  - **BMS (This repository)**
  - Scalable Battery Pack
- EVPE - Electric Vehicle Powertrain Equipment
  - Motor Driver (https://github.com/EnnoidMe/VESC-controller)
  - Gate driver (https://github.com/EnnoidMe/ENNOID-Dual-Gate-Driver)
  - Scalable Three-Phase Electric Motor

# ENNOID - BMS

This project aims to create a scalable & modular open source BMS for up to 1000V lithium-ion battery packs.

20/02/2018:
The idea behind this project has been started on this thread:

https://endless-sphere.com/forums/viewtopic.php?f=14&t=92952

# Update 

31/12/2018:
- Copying DieBieMS schematics into kicad
- Switching balancing circuit to LTC6813 
- Adapting the schematic to have the MPU isolated from PSU, Balancing circuit & High voltage measurements.
- Removing high current circuits from the PCB (Traces & FET switches are now removed, the current will flow through external contactor instead. ex: EV-200 contactor)



### Features:
- Modular **master** board for sub 18 cells ~80V packs & additionnal slave boards for up to 1000V pack
- Isolated output for Charge, discharge & precharge contactor
- Isolated power supply unit for sub 400V packs (external PSU possible through the dedicated connector above 400V)
- Isolated voltage measurement for charge, discharge & pack buses
- Current monitoring via external isolated delta-sigma current sensor
- Isolated CAN bus interface for cell voltage monitoring and charger detection, status monitoring + much more with future updates (like state of charge / state of health).
- USB interface for serial communication and firmware upgrades (no need for a programmer -> HW serial bootloader is used).
- Wireless 
- SDCard for logging (to develop a SoC and SoH algorithm)

### Components:

The IC's used with their corresponding functionality:

- LTC6813    -> Battery stack cell voltage monitor.
- STM32F303  -> Main microcontroller.
- INA226     -> Power measurement.
- AMC1301    -> Isolated voltage measurement
- AMC1303    -> Current measurement
- BQ76200    -> FET driver for LOAD+ CHARGE+ and Pre-charge switch driving.
- LM5165     -> SMPS Buck converter, converting the external voltage to +3.3V.
- ISO1050    -> isolated CAN-Bus transceiver.
- CP2104-F03 -> USB to serial converter for bootloader based firmware updates and debugging.



### Schematics - master

![alt text](V0.2-LTC6813/PIC/MainSchematic.png)

