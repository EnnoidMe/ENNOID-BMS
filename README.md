Related ENNOID project:
  - [Scalable Battery Pack](https://github.com/EnnoidMe/ENNOID-PACK)

# ENNOID - BMS

This project aims to create a scalable & modular open source BMS for up to 1000V lithium-ion battery packs.
The idea behind this project has been started on this thread:

https://endless-sphere.com/forums/viewtopic.php?f=14&t=92952

For ordering assembled BMS or battery packs, please visit:

https://www.ennoid.me/battery/bms

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
- 12 Cells balancing/board 
- Cell voltage range 2.5V to 4.5V
- 2 watt power dissipation per level
- Stackable for up to 1000V packs 
- 1 Analog input, 1 GPIO port, & 1 I2C communication channel available for external sensors
- Daisychain Master/Slave ISOSPI communication 

**Software**
- Firmware, bootloader & configuration tool are the same as [DieBieMS](https://github.com/DieBieEngineering). 

## Master

![alt text](Master/PIC/MasterPCB.png)


## Slave LTC6811

![alt text](Slaves/LTC6811/PIC/LTC6811.png)





