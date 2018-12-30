.. include:: ../../../macros.rst

.. _COM:

===
COM
===

.. highlight:: C

The |mod_com| is part of the ``Application`` layer.

The |mod_com| handles communication between the MCU and external devices. It
makes use of different interfaces (e.g., |mod_uart|) for this purpose.



Module Files
~~~~~~~~~~~~

Driver:
 - ``embedded-software\mcu-primary\src\application\com\com.h`` (:ref:`comc`)
 - ``embedded-software\mcu-primary\src\application\com\com.c`` (:ref:`comh`)


Establishing Connection with PC
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#. Connect PC with USB cable to primary USB interface
#. Start a terminal program (e.g., HTerm)
#. Select correct COM Port
#. Select following settings:

  - Baudrate: 115200 Bd
  - Databits: 8, Stopbits: 1, Parity: None
  - DTR: disabled, RTS: disabled
  - Send on enter: CR


Functionality
~~~~~~~~~~~~~

The |mod_com| provides possibilities for communication with external devices using UART. For incoming requests, it provides some kind of basic command parser/handler and access to the |mod_sys|. Furthermore, it provides the user with some information about the system and a testmode where the system settings can be alternated and basic system tests can be performed.

The |mod_com| can be enabled by using the BUILD_MODULE_ENABLE_COM define. Its handler needs to be called periodically (e.g., by using ``ENG_TSK_Cyclic_10ms()``). Currently the COM_Decoder supports following commands:

====================  ========================================================================================================
Command               Description
====================  ========================================================================================================
help                  get available command list
gettime               get system time
getruntime            get runtime since last reset
printdiaginfo         get diagnosis entries of DIAG module (entries can only be printed once)
printcontactorinfo    get contactor information (number of switches/hard switches) (entries can only be printed once)
teston                enable testmode, testmode will be disabled after a predefined timeout of 30s when no new command is sent
====================  ========================================================================================================

Following commands are only available during enabled testmode:

==========================  ==========================================================================================
Command                     Description
==========================  ==========================================================================================
testoff                     disable testmode
settime YY MM DD HH MM SS   set mcu time and date (YY-year, MM-month, DD-date, HH-hours, MM-minutes, SS-seconds)
reset                       enforces complete software reset using HAL_NVIC_SystemReset()
watchdogtest                performs watchdog test, watchdog timeout results in system reset (predefined 1s)
setsoc xxx.xxx              set SOC value (000.000% - 100.000%)
ceX                         enables contactor number X (only possible if BMS is in no error state)
cdX                         disables contactor number X (only possible if BMS is in no error state)
==========================  ==========================================================================================
