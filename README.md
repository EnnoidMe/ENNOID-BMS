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


This project aims to create a scalable & modular open source BMS for up to 1000V lithium-ion battery packs .

See another similar project at www.foxbms.org/

## Features
- Master board with STM32 MCU
- Slaves boards equiped with LTC6813 18-cell battery monitor
- Isolated CAN bus interface for cell voltage monitoring and charger detection, status monitoring + much more with future updates (like state of charge / state of health).
- USB interface for serial communication and firmware upgrades (no need for a programmer -> HW serial bootloader is used).
- SD Card for data storage
