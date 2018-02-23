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

20/02/2018:
The idea behind this project has been started on this thread:

https://endless-sphere.com/forums/viewtopic.php?f=14&t=92952


This project aims to create a scalable open source BMS for lithium-ion battery packs of up to 400V.

## Features
- Master board with STM32f3 MCU
- Slaves boards equiped with 16 cells Texas Instrument bq76PL455
- Isolated CAN bus interface for cell voltage monitoring and charger detection, status monitoring + much more with future updates (like state of charge / state of health).
- USB interface for serial communication and firmware upgrades (no need for a programmer -> HW serial bootloader is used).
- SD Card  for data storage
