.. include:: ../../macros.rst



.. _general_information_overview:

========
Overview
========

Hereafter a general overview of the content of |foxbms| can be found.

The |master| consists of 3 boards: |BMS-Master|, |BMS-Interface|, |BMS-Extension|. Two ARM-based microcontroller units (Cortex-M4) are used on |BMS-Master|: |MCU0| (also called primary MCU) and |MCU1| (also called secondary MCU). The BMS software runs on |MCU0|, while |MCU1| is used for redundant safety.

|MCU0| communicates with the outside world via a CAN bus (|CAN0|). The current flowing through the battery system is measured via a current sensor connected via the CAN bus. The sensor is controlled via CAN by |MCU0| and sends the resulting measurement via CAN.

The |slave| (|BMS-Slave|) are used to measure cell voltages and cell temperatures in the battery modules. The |slaves| are linked via a proprietary daisy chain from the company Linear Technology (i.e., isoSPI).

In order for the |master| to communicate with the |slaves|, an interface board (i.e., |BMS-Interface|) is needed. It converts the SPI signals from the |BMS-Master| into differential signals used by the daisy chain and vice versa.

Three power contactors are used to connect and disconnect the battery modules (or pack) from the load:

 - Main Contactor Plus
 - Main Contactor Minus
 - Pre-charge contactor

These contactors are driven by |MCU0|. Requests are made via CAN to the system to open and close the contactors. Based on the measurements and the algorithms implemented in the software, |MCU0| decides if the contactors should be closed or opened. It sends information via CAN so that the user knows the state of the system.

An interlock line is also present. If it is opened, either by |MCU0|, |MCU1| or somewhere else (e.g., emergency stop), all contactors will immediately open.

A secondary ARM-based microcontroller is present on the master, called |MCU1| (or secondary MCU). It monitors the slaves via a second and separate daisy chain. Like the |MCU1|, it can open the interlock in case something goes wrong with the system.

In case more inputs and outputs and further functions are required, an |BMS-Interface| is available and can be easily adapted to specific needs.

This description reflects the current state of foxBMS. Due to the open nature of the system, many other possibilities can be implemented, like for example:

 - Use of other types of current sensors (e.g., shunt-based or Hall-effect based)
 - No |slave| needs to be used: a direct measurement of the cell voltages and cell temperatures can be performed by the |master|
 - A higher number of contactors can be controlled (e.g., up to 9)
 - etc...
