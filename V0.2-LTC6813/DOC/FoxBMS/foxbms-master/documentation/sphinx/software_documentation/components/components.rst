.. include:: ../../macros.rst



.. _software_documentation_components:

===================
Software Components
===================

.. highlight:: C

The |foxbms| embedded software is made out of the following components:

 - ``mcu-common``
 - ``mcu-freertos``
 - ``mcu-hal``
 - ``mcu-primary``
 - ``mcu-secondary``

mcu-common
==========

The ``mcu-common`` directory contains drivers that are common to |MCU0| and
|MCU1|. This means that any change made in the common directory affects both
|MCU0| and |MCU1|.

driver
------

The ``driver`` directory contains a |foxbms| specific abstraction layer for
the hardware.

+-------------+-------------------------------------------------------------------------------------------+
| Element     | Description                                                                               |
+=============+===========================================================================================+
| adc         | Driver for analog to digital converter, measurement of lithium backup battery voltage     |
+-------------+-------------------------------------------------------------------------------------------+
| can         | Driver to receive/transmit CAN message                                                    |
+-------------+-------------------------------------------------------------------------------------------+
| chksum      | Checksum algorithms for modulo 32-bit addition and CRC32                                  |
+-------------+-------------------------------------------------------------------------------------------+
| dma         | Configuration for Direct Memory Access (e.g. used for SPI Communication)                  |
+-------------+-------------------------------------------------------------------------------------------+
| io          | Driver and interfaces for I/O ports (control of output pins and read of input pins)       |
+-------------+-------------------------------------------------------------------------------------------+
| mcu         | Driver for the interlock                                                                  |
+-------------+-------------------------------------------------------------------------------------------+
| rcc         | Configuration of the prescaler for the MCU clock system                                   |
+-------------+-------------------------------------------------------------------------------------------+
| rtc         | Real time clock driver, Control and Access of Backup SRAM Registers                       |
+-------------+-------------------------------------------------------------------------------------------+
| spi         | Driver for communication via Serial Peripheral Interface (SPI bus)                        |
+-------------+-------------------------------------------------------------------------------------------+
| uart        | Driver for serial communication (UART, RS232 , RS485)                                     |
+-------------+-------------------------------------------------------------------------------------------+
| vic         | Interrupt configuration                                                                   |
+-------------+-------------------------------------------------------------------------------------------+
| watchdog    | Driver for the watchdog timer                                                             |
+-------------+-------------------------------------------------------------------------------------------+

engine
------

The ``engine`` directory contains all the core functions of the BMS.

+-------------+------------------------------------------------------------------------------------+
| Element     | Description                                                                        |
+=============+====================================================================================+
| database    | Implementation of the asynchronous data exchange                                   |
+-------------+------------------------------------------------------------------------------------+

module
------

The ``module`` directory contains all the software modules needed by the BMS.

+-------------+-------------------------------------------------------------------------------------------+
| Element     | Description                                                                               |
+=============+===========================================================================================+
| cansignal   | Definition of CAN messages and signals                                                    |
+-------------+-------------------------------------------------------------------------------------------+
| hwinfo      | Functions to return information about the hardware (e.g., cpu temperature)                |
+-------------+-------------------------------------------------------------------------------------------+
| interlock   | Driver for the interlock                                                                  |
+-------------+-------------------------------------------------------------------------------------------+
| led         | Functions for using the LEDs                                                              |
+-------------+-------------------------------------------------------------------------------------------+
| ltc         | Driver for battery cell monitoring IC                                                     |
+-------------+-------------------------------------------------------------------------------------------+
| meas        | Uses ltc module to perform measurements                                                   |
+-------------+-------------------------------------------------------------------------------------------+

util
----

The ``util`` directory contains additional functions.

+-------------+------------------------------------------------------------------------------------+
| Element     | Description                                                                        |
+=============+====================================================================================+
| foxmath     | Implementation of a math library                                                   |
+-------------+------------------------------------------------------------------------------------+
| misc        | miscellaneous functions (e.g., functions to reverse a string                       |
+-------------+------------------------------------------------------------------------------------+

mcu-freertos
============

The ``mcu-freertos`` directory contains the operating system software (FreeRTOS).

+-----------------------+--------------------------------------------------------------------------------+
| Element               | Description                                                                    |
+=======================+================================================================================+
| Source                | FreeRTOS                                                                       |
+-----------------------+--------------------------------------------------------------------------------+

mcu-hal
=======

The ``mcu-hal`` directory contains the Hardware Abstraction Layer (HAL). It is
used by the system but is provided by the MCU manufacturer, in this case
ST-Microelectronics. It is used by |foxbms| but not part of |foxbms|.

+-----------------------+--------------------------------------------------------------------------------+
| Element               | Description                                                                    |
+=======================+================================================================================+
| CMSIS                 | Interface and configuration of CMSIS                                           |
+-----------------------+--------------------------------------------------------------------------------+
| STM32F4xx_HAL_Driver  | STM32F4xx family Hardware Abstraction Layer drivers                            |
+-----------------------+--------------------------------------------------------------------------------+

mcu-primary
===========

The ``mcu-primary`` directory contains the software specific to |MCU0|.

application
-----------

The ``application`` directory contains the user applications.

+-------------+-----------------------------------------------------------------------------------+
| Element     | Description                                                                       |
+=============+===================================================================================+
| algo        | Task for integration various algorithms                                           |
+-------------+-----------------------------------------------------------------------------------+
| bal         | Driver for balancing                                                              |
+-------------+-----------------------------------------------------------------------------------+
| bms         | Decision are taken here by the BMS (e.g., open contactors in case of a problem)   |
+-------------+-----------------------------------------------------------------------------------+
| com         | Serial port communication layer (for debug purposes)                              |
+-------------+-----------------------------------------------------------------------------------+
| config      | Contains the configuration for the user applications (e.g., task configuration)   |
+-------------+-----------------------------------------------------------------------------------+
| sox         | Coulomb-counter (current integrator) and State-of-Function calculator             |
+-------------+-----------------------------------------------------------------------------------+
| task        | User specific cyclic tasks (10ms and 100ms)                                       |
+-------------+-----------------------------------------------------------------------------------+

Application tasks should be used to call user-defined functions.

driver
------

The ``driver`` directory contains a |foxbms| specific abstraction layer for
the hardware.

+-------------+-------------------------------------------------------------------------------------------+
| Element     | Description                                                                               |
+=============+===========================================================================================+
| config      | Configuration of the drivers (from mcu-common and primary specfific)                      |
+-------------+-------------------------------------------------------------------------------------------+
| sdram       | Drivers for the external SDRAM                                                            |
+-------------+-------------------------------------------------------------------------------------------+
| timer       | Drivers for the internal timers                                                           |
+-------------+-------------------------------------------------------------------------------------------+

engine
------

The ``engine`` directory contains all the core functions of the BMS.

+--------------+------------------------------------------------------------------------------------+
| Element      | Description                                                                        |
+==============+====================================================================================+
| config       | Contains the configuration of engine components (e.g., task configuration)         |
+--------------+------------------------------------------------------------------------------------+
| diag         | With this software module, other modules can report problems                       |
+--------------+------------------------------------------------------------------------------------+
| nvramhandler | Handler for the non volatile memories                                              |
+--------------+------------------------------------------------------------------------------------+
| sys          | System state machine, starts all other state machines                              |
+--------------+------------------------------------------------------------------------------------+
| task         | Cyclic engine tasks (1, 10 and 100ms) that call system related functions           |
+--------------+------------------------------------------------------------------------------------+

general
-------

The ``general`` directory contains the main function and configuration files.

+-------------+-----------------------------------------------------------------------------------+
| Element     | Description                                                                       |
+=============+===================================================================================+
| main        | Initialization of hardware modules, of interrupts and of the operating system     |
+-------------+-----------------------------------------------------------------------------------+
| config      | Contains the configuration for the system initialization:                         |
|             | configuration and interface functions to HAL and FreeRTOS,                        |
|             | global definitions, interrupt configurations and startup code                     |
+-------------+-----------------------------------------------------------------------------------+
| includes    | Contains the standard types                                                       |
+-------------+-----------------------------------------------------------------------------------+
| version     | Sets the version number                                                           |
+-------------+-----------------------------------------------------------------------------------+


module
------

The ``module`` directory contains all the software modules needed by the BMS.

+-------------+-------------------------------------------------------------------------------------------+
| Element     | Description                                                                               |
+=============+===========================================================================================+
| config      | Contains the configuration for the software modules                                       |
+-------------+-------------------------------------------------------------------------------------------+
| contactor   | Driver to open/close contactors and read contactor feedback                               |
+-------------+-------------------------------------------------------------------------------------------+
| isoguard    | Driver for monitoring galvanic isolation in the system                                    |
+-------------+-------------------------------------------------------------------------------------------+
| nvram       | Non-volatile Memory: Eeprom and button cell buffered SRAM (BKP_SRAM)                      |
+-------------+-------------------------------------------------------------------------------------------+

os
--

The ``os`` directory contains configurations for the operating system FreeRTOS.

+-------------+----------------------------------------------------------------------------------------+
| Element     | Description                                                                            |
+=============+========================================================================================+
| os          | Interface to FreeRTOS (e.g., wrapper functions of cyclic application and engine tasks) |
+-------------+----------------------------------------------------------------------------------------+

mcu-secondary
=============

Contains the software specific to |MCU1|. These are the same elements as
``mcu-primary``, adapted for |MCU1|.
