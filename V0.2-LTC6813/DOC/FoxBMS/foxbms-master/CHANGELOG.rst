=========
Changelog
=========

**Release 1.5.2**

Software:

* Toolchain:

  * updated to ``waf-2.0.12`` (from ``waf-2.0.11``)

* Bugfixes:

  * fixed bug that delay after SPI wake-up byte was not long enough


* Enhancements:

  * increased CPU clock frequency from 168MHz to 180MHz
  * increased SPI bitrate from 656.25kHz to 703.125kHz
  * Added CAN boot message with SW-version and flash checksum (0x101)
  * CAN messages are now always sent, even if system error was detected
  * foxBMS SW-version requestable via CAN (request ID: 0x777, response ID: 0x101)
  * added insulation error flag to ``DATA_BLOCK_ERRORSTATE_s``
  * configurable behavior if contactors should be open on insulation error or not
  * separate configurable precharging for charge/discharge path possible

Hardware:

* adapted CAN filter circuit on master and extension board for improved fault tolerance at short of CAN_L to GND or CAN_H to supply

Documentation:

* updated instruction for flashing primary MCU
* updated FAQ section

------------------------------------------------------------------------------

**Release 1.5.1**

Software:

* Toolchain:

  * toolchain compatible with POSIX operating systems
  * updated to ``waf-2.0.11`` (from ``waf-2.0.10``)
  * fixed missing files in eclipse workspace (CHANGELOG.rst and compiler-flags.yml)

* Bugfixes:

  * fixed bug updating BKPSRAM values to EEPROM: BKPSRAM checksum was calculated wrong


* Enhancements:

  * modules CONTACTOR, INTERLOCK and ISOGUARD can be disabled if not needed
  * selected new EEPROM M95M02 as default EEPROM (equipped on foxBMS-Master since v1.0.5)

Hardware:

* none

Documentation:

* added a section on how to build and include a library
* removed references to directory ``foxbms-setup``, as it is now simply called ``foxbms``
* removed references to script ``bootstrap.py``, as this script is no longer used

------------------------------------------------------------------------------

**Release 1.5.0**

* **foxBMS has been migrated from Python 2.7 to Python 3.6. The foxconda3 installer is found at https://iisb-foxbms.iisb.fraunhofer.de/foxbms/. foxconda3 must be installed to C:\foxconda3.**
* **EEPROM addresses on the BMS-Master were changed. Previous saved EEPROM data will be lost with new update.**
* **introduction of an improved software structure to differentiate between hardware-dependent and hardware-independent software layers**

Software:

* Toolchain:

  * switched to monolithic repository structure to simplify the versioning
  * raised compiler warning ``[-Wimplicit-function-declaration]`` to error level
  * avoid ``shell=True`` in python subprocess
  * updated python checksum script
  * updated to ``waf-2.0.10`` and renamed the waf binary to simply ``waf``

* Bugfixes:

  * fixed bug passing *mV* instead of *V* to function ``LTC_Convert_MuxVoltages_to_Temperatures()``
  * typedef ``DATA_BLOCK_ID_TYPE_e`` starts at 0x00 instead of 0x01 (renamed DATA_BLOCK_1 to DATA_BLOCK_00) for consistency
  * fixed bug in ltc module: wrote wrong values to database when using filtered mode for measuring cell voltages and temperatures
  * #define ``CONT_PRECHARGE_VOLTAGE_THRESHOLD`` used *V* instead of *mV*
  * fixed bug in function ``CAN_WakeUp()``: wrong HAL function call was corrected
  * fixed bug in diag module: did not evaluated diagnostic errors with ``DIAG_ERROR_SENSITIVITY_HIGH``


* Enhancements:

  * adapted wscripts to new restructured software architecture
  * added timestamp to MCU backup SRAM and external EEPROM entries
  * added three alarm levels (maximum operating limit, recommended safety limit, maximum safety limit)
  * enhanced voltage based balancing algorithm
  * updated .dbc file
  * added measure AllGPIO state to ltc module
  * added CAN message for pack voltage (CAN-ID: 0x1F0)
  * added algorithm module to enable future advanced algorithms
  * increased FreeRTOS heap size from 15kByte to 20kByte
  * increased stack size of 100ms application task from 512bytes to 1024bytes
  * increased size of CAN TX message buffer from 16 to 24 messages
  * added calculation of moving average values (1s, 5s, 10s, 30s and 60s) for current and power
  * database timestamp are now automatically written on DB_Write - no need to manually update timestamps anymore
  * added native matlab datatypes support
  * cleanup of ASCII conversion functions (uint to ASCII, hex to ASCII, int to ASCII)
  * added nvramhandler to automatically update non-volatile memory (i.e., external EEPROM on BMS-Master)
  * renamed various structs, variables and functions for an improved code understanding and increased readability

Hardware:

* added hardware changelogs
* ported hardware PCB design files to Altium Designer format (AutoDesk Eagle files no longer supported)
* updated hardware PCB designs: Master V1.0.5, Extension V1.0.4, Interface 1.9.3, Slave 12-cell (LTC6811-1) V2.1.5, Slave 18-cell (LTC6813-1) V1.1.3

Documentation:

* added foxbms styleguide
* fixed spelling errors
* added documentation of software architecture
* added documentation of algorithm module
* added documentation of nvramhandler
* updated isoguard documentation

------------------------------------------------------------------------------

**Release 1.1.0**

foxbms-setup(v1.0.1):

* updated build scripts
* updated waf script
* updated README.md

mcu-common(v1.1.0):

* updated license header
* seperated database entries to prevent concurrent read/write requests to the database
* updated wscripts to build specific files only for primary/secondary
* moved sdram from common repository to primary repository
* renamed database functions to ``DB_WriteBlock()`` and ``DB_ReadBlock()``
* There was a compile error when CAN0 and CAN1 are deactivated
* updated README.md

mcu-freertos(v1.1.0):

* updated license header
* updated wscripts to build specific files only for primary/secondary
* moved sdram from common repository to primary repository
* updated README.md

mcu-hal(v1.0.1):

* updated license header
* updated README.md

mcu-primary(v1.1.0):

* uses now wafs feature of variant builds
* baudrate of CAN0 and CAN1 can now be set independently
* the setup of the tasks in engine and application layer is now consistent
* updated license header
* fixed a bug in contactor module to write unnecessary often into the database which caused a high cpuload
* seperated database entries to prevent concurrent read/write requests to the database
* added support of external SDRAM using keyword ``MEM_EXT_SDRAM``
* moved sdram from common repository to primary repository
* fixed a bug that closed the interlock for a short period of time after restart even if no CAN message was received to switch to STANDBY state
* renamed database functions to ``DB_WriteBlock()`` and ``DB_ReadBlock()``
* updated README.md

mcu-secondary(v1.1.0):

* uses now wafs feature of variant builds
* the setup of the tasks in engine and application layer is now consistent
* updated license header
* seperated database entries to prevent concurrent read/write requests to the database
* renamed database functions to ``DB_WriteBlock()`` and ``DB_ReadBlock()``
* deleted unused code
* updated README.md

tools(v1.0.2):

* Updated waf
* Updated copyright
* Updated the Eclipse Project
* Updated checksum tool from gdb-based to object-copy-based toolchain
* updated README.md

documentation(v1.0.2):

* updated documenation for the build process
* updated FAQ section
* updated copyright
* updated README.md

------------------------------------------------------------------------------

**Release 1.0.1**

* updated build scripts
* updated waf script

------------------------------------------------------------------------------

**Release 1.0.0**

* renamed repository from ``foxBMS-setup`` to ``foxbms-setup``.
* Removed update functionallity
* Moved arm-none-eabi-size call as post function in build process
* added a ``.config.yaml`` file which includes a list of repositories which are
  boostrapped and their bootstrap location.

------------------------------------------------------------------------------

**Release 0.5.2**

Release notes:
We fixed a bug in the ltc driver, leading to a non-functional temperature
sensing for foxBMS Slave Hardware version 1.xx. The slave version is
configuration for the primary MCU in foxBMS-primary\src\module\config\ltc_cfg.h
by the define SLAVE_BOARD_VERSION and for the secondary MCU in
foxBMS-secondary\src\module\config\ltc_cfg.h by the define SLAVE_BOARD_VERSION.

* Set SLAVE_BOARD_VERSION to "1" if you are using version 1.xx of the foxBMS
  Slave.
* Set SLAVE_BOARD_VERSION to "2" if you are using version 2.xx of the foxBMS
  Slave. Version 2.xx is the default configuration.

Changelog:

* foxBMS-primary

  * fixed LTC temperature sensing bug

* foxBMS-secondary

  * fixed LTC temperature sensing bug

------------------------------------------------------------------------------

**Release 0.5.1**

* foxBMS-setup

  * added parameter '-u', '--update' to bootstrap.py for updating the setup
    repository.

* foxBMS-primary

  * updates for waf 1.9.13 support
  * updated module/EEPROM and migrated to module/nvmram
  * minor code adaptations and cleanup

* foxBMS-secondary

  * support for waf 1.9.13
  * minor code adaptations and cleanup

* foxbMS-tools

  * updated waf from version 1.8.12 to version 1.9.13

------------------------------------------------------------------------------

**Release 0.5.0**

A new project structure is now used by foxBMS. The documentation is no more
contained in the embedded software sources and has its own repository. FreeRTOS
and hal have their own repository, too.
A central repository called foxBMS-setup is now used. It contains
several scripts:


* bootstrap.py gets all the repositories needed to work with foxBMS
* build.py is used to compile binaries and to generate the documentation
* clean.py is used to removed the generated binaries and documentation

Release notes:

* New project structure
* Added support for external (SPI) EEPROM on the BMS-Master
* Redesign of can and cansignal module to simplify the usage
* Added support for triggered and cyclic current measurement of Isabellenhütte
  current sensor (IVT)
* Current sensor now functions by default in non-triggered modus (no
  reprogramming needed for the sensor)
* Updated and restructured complete documentation
* Restructured file and folder structure for the documentation
* Added safety and risk analysis section
* Cleaning up of non-used files in the documentation
* Consistency check and correction of the naming and wording used
* Addition of the source files (e.g., Microsoft Visio diagrams) used to
  generate the figures in the documentation
* Reformatted the licenses text formatting (no changes in the licenses
  content)
* Updated the battery junction box (BJB) section with up-to-date components
  and parameters

------------------------------------------------------------------------------

**Release 0.4.4**

The checksum tool is now automatically called when building binaries.
Therefore the command
``python tools/waf-1.8.12 configure build chksum``
is NOT longer supported. The command to build binaries with checksum support is
``python tools/waf-1.8.12 configure build``
This is the build command used in foxBMS FrontDesk, that is, FrontDesk software
is compatible with this change and now supports automatic checksum builds.

Release notes:

* Improved checksum-feature
* Updated copyright 2010 - 2017

------------------------------------------------------------------------------

**Release 0.4.3**

Starting from this version, a checksum mechanism was implemented in foxBMS. If
the checksum is active and it is not computed correctly, it will prevent the
flashed program from running. Details on deactivating the checksum can be found
in the Software FAQ, in How to use and deactivate the checksum.

Release notes:


* Important: Changed contactor configuration order in the software to match
  the labels on the front

  * Contactor 0: CONT_PLUS_MAIN
  * Contactor 1: CONT_PLUS_PRECHARGE
  * Contactor 2: CONT_MINUS_MAIN

* Fixed an bug which could cause an unintended closing of the contactors after
  recovering from error mode
* Increased stack size for the engine tasks to avoid stack overflow in some
  special conditions
* Added a note in the documentation to indicate the necessity to send a
  periodic CAN message to the BMS
* Fixed DLC of CAN message for the current sensor measurement
* Added checksum verification for the flashed binaries
* Updated linker script to allow integration of the checksum tool
* Activated debug without JTAG interface via USB

------------------------------------------------------------------------------

**Release 0.4.2**

Release notes:

* Removed schematic files from documentation, registration needed to obtain
  the files
* Added entries to the software FAQ

------------------------------------------------------------------------------

**Release 0.4.1**

Release notes:

* Corrected daisy chain connector pinout in quickstart guide
* Corrected code for contactors, to allow using contactors without feedback
* Corrected LTC code for reading balancing feedback
* Quickstart restructured, with mention of the necessity to generate the HTML
  documentation

------------------------------------------------------------------------------

**Release 0.4.0**

Beta version of foxBS that was supplied to selected partners for evaluation.

Release notes:

------------------------------------------------------------------------------

**foxBMS Hardware Change Log (deprecated)**

The hardware changelog is now included in the regular changelog (since version
1.5.0).

*foxBMS Master*

+--------+------------------------------------------------------------------------------------------------------+
| V1.0.5 | schematic cleanup, improved fonts and sizes on PCB                                                   |
+--------+------------------------------------------------------------------------------------------------------+
| V1.0.4 | | introduced minor improvements to design                                                            |
|        | | replaced EEPROM with M95M02-DRMN6TP                                                                |
+--------+------------------------------------------------------------------------------------------------------+
| V1.0.3 | | ported schematics and layout to Altium Designer                                                    |
|        | | created hierarchical design                                                                        |
|        | | introduced minor improvements to design                                                            |
+--------+------------------------------------------------------------------------------------------------------+
| V1.0.2 | replaced ADuM14XX isolators by ADuM34XX                                                              |
+--------+------------------------------------------------------------------------------------------------------+
| V1.0.1 | added fuse protection on power supply input                                                          |
+--------+------------------------------------------------------------------------------------------------------+
| V1.0.0 | initial release                                                                                      |
+--------+------------------------------------------------------------------------------------------------------+

*foxBMS Extension*

+--------+------------------------------------------------------------------------------------------------------+
| V1.0.4 | schematic cleanup, improved fonts and sizes on PCB                                                   |
+--------+------------------------------------------------------------------------------------------------------+
| V1.0.3 | | ported schematics and layout to Altium Designer                                                    |
|        | | created hierarchical design                                                                        |
|        | | introduced minor improvements to design                                                            |
+--------+------------------------------------------------------------------------------------------------------+
| V1.0.2 | replaced ADuM14XX isolators by ADuM34XX                                                              |
+--------+------------------------------------------------------------------------------------------------------+
| V1.0.1 | swapped input protection of isolated GPIOs                                                           |
+--------+------------------------------------------------------------------------------------------------------+
| V1.0.0 | initial release                                                                                      |
+--------+------------------------------------------------------------------------------------------------------+

*foxBMS Interface*
+--------+------------------------------------------------------------------------------------------------------+
| V1.9.3 | replace NAND-gate with SN74LVC00AQPWRQ1                                                              |
+--------+------------------------------------------------------------------------------------------------------+
| V1.9.2 | replace OR-gate with NAND-gate and add direction pin                                                 |
+--------+------------------------------------------------------------------------------------------------------+
| V1.9.1 | | rotate pinout of Daisy-Chain-Connectors in order to mirror Slave-Connectors                        |
|        | | add labels to Daisy-Chain-Connectors                                                               |
|        | | update with new layermarker                                                                        |
|        | | replace OR-gate with AEC-Q100 qualified COTS                                                       |
+--------+------------------------------------------------------------------------------------------------------+
| V1.9.0 | | update design for reverse isoSPI with second channel                                               |
|        | | port to Altium Designer                                                                            |
+--------+------------------------------------------------------------------------------------------------------+
| V1.1.0 | replaced isoSPI transformer HX1188 by HM2102                                                         |
+--------+------------------------------------------------------------------------------------------------------+
| V1.0.2 | modified connection of isoSPI transformer HX1188                                                     |
+--------+------------------------------------------------------------------------------------------------------+
| V1.0.1 | added fiducials                                                                                      |
+--------+------------------------------------------------------------------------------------------------------+
| V1.0.0 | initial release                                                                                      |
+--------+------------------------------------------------------------------------------------------------------+

*foxBMS Slave 12-cell (LTC6811-1)*

+--------+------------------------------------------------------------------------------------------------------+
| V2.1.5 | | Replaced Opamps, Port Expanders and Optocouplers with AEC-Q100 compliant ones                      |
|        | | Modified silkscreen texts                                                                          |
+--------+------------------------------------------------------------------------------------------------------+
| V2.1.4 | | Primary software timer is now switched on by default                                               |
|        | | added layermarker on PCB                                                                           |
+--------+------------------------------------------------------------------------------------------------------+
| V2.1.3 | replaced EOL port expander with PCF8574                                                              |
+--------+------------------------------------------------------------------------------------------------------+
| V2.1.2 | | ported schematics and layout to Altium Designer                                                    |
|        | | created hierarchical design                                                                        |
|        | | introduced minor improvements to design                                                            |
+--------+------------------------------------------------------------------------------------------------------+
| V2.1.1 | improved isolation distances between external DC/DC converter supply and battery module signals      |
+--------+------------------------------------------------------------------------------------------------------+
| V2.1.0 | added DC/DC converter for external power supply                                                      |
+--------+------------------------------------------------------------------------------------------------------+
| V2.0.3 | fixed isoSPI transformer CMC issue                                                                   |
+--------+------------------------------------------------------------------------------------------------------+
| V2.0.2 | | replaced LTC1380 MUXs with ADG728 (400 kHz I2C)                                                    |
|        | | adjusted connection of 100 ohm resistors for V+/V_REG supply                                       |
|        | | reduced value of I2C pullups to 1k2                                                                |
+--------+------------------------------------------------------------------------------------------------------+
| V2.0.1 | | added missing cooling areas on bottom side, adjusted silk screen                                   |
|        | | enlarged PCB tracks, R201/202/301/302 other package                                                |
|        | | enlarged T201/301 cooling area                                                                     |
|        | | Replaced PCF8574 with PCA8574 (400 kHz I2C)                                                        |
+--------+------------------------------------------------------------------------------------------------------+
| V2.0.0 | initial release                                                                                      |
+--------+------------------------------------------------------------------------------------------------------+

*foxBMS Slave 18-cell (LTC6813-1)*

+--------+------------------------------------------------------------------------------------------------------+
| V1.1.3 | schematic cleanup, improved fonts and sizes on PCB                                                   |
+--------+------------------------------------------------------------------------------------------------------+
| V1.1.2 | | replaced ACPL-247 with ACPL-217 optocoupler in order to be able to use automotive components       |
+--------+------------------------------------------------------------------------------------------------------+
| V1.1.1 | | replaced port expander with TCA6408APWR (automotive)                                               |
|        | | replaced analog buffer opamp with AD8628ARTZ-R2 (automotive)                                       |
|        | | replaced DC/DC buck controller with LM5161QPWPRQ1 (automotive)                                     |
+--------+------------------------------------------------------------------------------------------------------+
| V1.1.0 | | ported schematics and layout to Altium Designer                                                    |
|        | | created hierarchical design                                                                        |
|        | | introduced minor improvements to design                                                            |
|        | | replaced linear regulation (PNP transistor) for LTC power supply with DC/DC converters             |
|        | | improved isolation distances between external DC/DC converter supply and battery module signals    |
|        | | added 8-24 V isolated external power supply                                                        |
|        | | replaced I2C EEPROM 24AA02UID with M24M02-DR (ECC)                                                 |
|        | | replaced isoSPI transformers HX1188NL with HM2102NL                                                |
|        | | reduced balancing resistors from 2x 68 Ohm to 2x 130 Ohm due to shrinked cooling areas             |
|        | | added layermarker on PCB                                                                           |
|        | | Primary discharge timer is now switched on by default                                              |
+--------+------------------------------------------------------------------------------------------------------+
| V1.0.1 | replaced all LTC1380 MUXs with ADG728 MUXs                                                           |
+--------+------------------------------------------------------------------------------------------------------+
| V1.0.0 | initial release                                                                                      |
+--------+------------------------------------------------------------------------------------------------------+
