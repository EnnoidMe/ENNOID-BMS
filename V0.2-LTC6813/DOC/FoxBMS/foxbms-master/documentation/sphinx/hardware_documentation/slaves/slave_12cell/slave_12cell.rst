.. include:: /macros.rst

.. _hw_slave_12cell:

##################################
Slave 12-Cell ``v2.1.2 and above``
##################################

========
Overview
========

.. important::

   The following description only applies for the 12-cell |BMS-Slave| hardware versions ``2.1.2`` and above.

   The documentation for the 12-cell |BMS-Slave| version ``2.0.3`` to ``2.1.1`` can be found in section :ref:`hw_slave_12cell_v2_0_3`.

   The documentation for the 12-cell |BMS-Slave| version ``1.x.x`` can be found in section :ref:`hw_slave_12cell_v1_x`.

.. hint::

   All connector pinouts described below follow the :ref:`sec-connector-convention`.


-------------
Block Diagram
-------------

A block diagram of the |BMS-Slave| is shown in :numref:`fig. %s <BMS-Slave_12cell>`

.. _BMS-Slave_12cell:
.. figure:: ./slave-block-12cell.png
   :width: 100 %

   |BMS-Slave| 12-Cell Block Diagram
..


--------------------------
Schematic and Board Layout
--------------------------

More information about the board schematic and layout files can be found in section :ref:`hardware_design_resources`.


---------------------
Mechanical Dimensions
---------------------

The size of the foxBMS Slave PCB is 160x100mm. A ``*.step`` file and a ``3D-PDF`` of the PCB can be found in section :ref:`hardware_design_resources`.


=========
Functions
=========

The following general description applies to both, the primary and the secondary of the |BMS-Slave|. If there are any differences in hardware between the primary and the secondary they will be marked as such.

------------------------
Cell Voltage Measurement
------------------------

The cell voltage sense lines are input on the connector X1503. The pinout is described in :numref:`table %s <cell_sense_connector>`.

.. figure:: ./twentyfourcon.png
   :width: 50 %

.. _cell_sense_connector:

.. table:: Cell voltage sense connector

    ====   =============    ============    =================================
    Pin    Signal           Direction       Description
    ====   =============    ============    =================================
    1      VBAT-            Input           Battery module negative terminal
    2      CELL_0+          Input           Cell 0 positve terminal
    3      CELL_2+          Input           Cell 2 positve terminal
    4      CELL_4+          Input           Cell 4 positve terminal
    5      CELL_6+          Input           Cell 6 positve terminal
    6      CELL_8+          Input           Cell 8 positve terminal
    7      CELL_10+         Input           Cell 10 positve terminal
    8      VBAT+            Input           Battery module positive terminal
    9      NC               --              --
    10     NC               --              --
    11     NC               --              --
    12     NC               --              --
    13     CELL_0-          Input           Cell 0 negative terminal
    14     CELL_1+          Input           Cell 1 positive terminal
    15     CELL_3+          Input           Cell 3 positive terminal
    16     CELL_5+          Input           Cell 5 positive terminal
    17     CELL_7+          Input           Cell 7 positive terminal
    18     CELL_9+          Input           Cell 9 positive terminal
    19     CELL_11+         Input           Cell 11 positive terminal
    20     NC               --              --
    21     NC               --              --
    22     NC               --              --
    23     NC               --              --
    24     NC               --              --
    ====   =============    ============    =================================

Each of these lines is protected by a 250mA fast fuse surface mount device (F402 - F414) on the board except of the VBAT+ and VBAT- lines which are protected by a value of 500mA (F401 and F415). This is especially important for a test environment. The VBAT+ and VBAT- connections are used for the internal power supply of the |BMS-Slave| board. If the battery module does not contain these separate wires to the positive and negative module terminals, the solder jumpers SJ401 and SJ402 have to be shorted. In this case the power required by the |BMS-Slave| will be supplied through the sense lines CELL_0- and CELL_11+. Running the |BMS-Slave| in this configuration could result in cell measurement errors due to voltage drop over the sense wires.

The cell input lines are filtered by a grounded or differential capacitor filter: both possibilities are provided on the PCB of the |BMS-Slave|. More information on the corner frequency of this filtering can be found in the schematic. The grounded capacitor filter should be used in environments affected with a high noise as it offers a high level of battery voltage ripple rejection. The differential capacitor filter can be used when noise is less occurrent or the design is subjected to cost optimization.

----------------------
Passive Cell Balancing
----------------------

The passive balancing circuit is realized by a parallel connection of two 68Ω discharge resistors that can be connected to each single cell in parallel. The MOSFET switches (T701 - T712) that control the connection to the cells are controlled by the primary |LTC| monitoring IC. The secondary |LTC| does not support balancing. The resistor value of 2x 68Ω results in a balancing current of about 100mA at a cell voltage of 3.6V. This current results in a power dissipation of about 0.36W per balancing channel (at 3.6V).

------------------------------
Global Cell Balancing Feedback
------------------------------

In order to check the proper function of the balancing process or to detect a malfunction in the balancing circuit, a global balancing feedback signal is connected to the |LTC|. This allows the |BMS-Master| to check wheather any balancing action is currently taking place. The feedback signal is connected to the GPIO3 of the |LTC|. The signal remains in a logic zero state until any balancing action on at least one cell in the module starts.

.. _slave_voltage_divider:

------------------------------
Temperature Sensor Measurement
------------------------------

The cell temperature sensors are connected to the connectors X1506 (primary) and X1507 (secondary). The pinout is identical for the primary and secondary and is described in :numref:`table %s <temp_sense_connector>`.

.. figure:: ./sixteencon.png
   :width: 30 %

.. _temp_sense_connector:

.. table:: Temperature sensor connector

    ====   =============    ============    ========================
    Pin    Signal           Direction       Description
    ====   =============    ============    ========================
    1      T-SENSOR_0       Input           NTC Sensor 0 terminal 1
    2      T-SENSOR_1       Input           NTC Sensor 1 terminal 1
    3      T-SENSOR_2       Input           NTC Sensor 2 terminal 1
    4      T-SENSOR_3       Input           NTC Sensor 3 terminal 1
    5      T-SENSOR_4       Input           NTC Sensor 4 terminal 1
    6      T-SENSOR_5       Input           NTC Sensor 5 terminal 1
    7      T-SENSOR_6       Input           NTC Sensor 6 terminal 1
    8      T-SENSOR_7       Input           NTC Sensor 7 terminal 1
    9      FUSED_VBAT-      Input           NTC Sensor 0 terminal 2
    10     FUSED_VBAT-      Input           NTC Sensor 1 terminal 2
    11     FUSED_VBAT-      Input           NTC Sensor 2 terminal 2
    12     FUSED_VBAT-      Input           NTC Sensor 3 terminal 2
    13     FUSED_VBAT-      Input           NTC Sensor 4 terminal 2
    14     FUSED_VBAT-      Input           NTC Sensor 5 terminal 2
    15     FUSED_VBAT-      Input           NTC Sensor 6 terminal 2
    16     FUSED_VBAT-      Input           NTC Sensor 7 terminal 2
    ====   =============    ============    ========================

Standard 10kΩ NTC resistors (e.g., Farnell-Nr. 1299926) are recommended for use. When using other values than these, the series resistors (R901-R908 / R1001-R1008) on the board may have to be adjusted. Please note that the accuracy of the internal voltage reference VREF2 decreases heavily with a load of over 3mA. Using 8x 10kΩ NTC resistors with the corresponding 10kΩ series resistors results in a current of 1.2mA (at 20°C) which is drawn from VREF2.

Each of the 8 temperture sensors are connected to an analog multiplexer. The analog multiplexer can be controlled via I²C by the |LTC| (7-bit address: 0x4C). In order to ensure fast settling times after swiching the multiplexer input, the output signal of the multiplexer is buffered by an operational amplifier. Finally the analog voltage of the selected sensor is measured on the GPIO1 pin of the |LTC|.

---------------
On-board EEPROM
---------------

.. attention::

   The |BMS-Slave| hardware versions ``2.1.0`` and above use a different EEPROM IC (ST M24M02) than all other previous hardware versions.

The primary as well as the secondary unit of the |BMS-Slave| board is equipped with an EEPROM (IC1301 / IC1401). The EEPROM for example can be used for storing data such as calibration values or minimum and maximum temperatures seen by the module during its lifetime. Similar to the analog multiplexers, the EEPROM device is connected to the I²C bus of the |LTC| (7-bit address: 0x50).

-----------------------------------
On-board Ambient Temperature Sensor
-----------------------------------

For an additional monitoring of the ambient temperature an on-board temperature sensor is used. This temperature sensor can be read by the |LTC| via the I²C bus (7-bit address: 0x48). It is possible to program an alert temperature level. Once the measured temperature reaches this alert temperature level, the alert pin of the IC is set to a logic low level. Currently, this signal is not used on the |BMS-Slave| board, but it is accessible on the connector X1508.

-----------------------------
Additional Inputs and Outputs
-----------------------------

Several additional analog and digital inputs and outputs are provided on the |BMS-Slave| board via pin headers. Each 16 analog inputs are provided on connector X1501 (primary) and X1502 (secondary). The pinout for the connectors for the primary and secondary unit is identical and is described in :numref:`table %s <analog_in_connector>`.

.. _analog_in_connector:

.. table:: Connector for analog inputs

    ====   =============    ============    =============================
    Pin    Signal           Direction       Description
    ====   =============    ============    =============================
    1      ANALOG-IN_0      Input           Analog input 0
    2      ANALOG-IN_1      Input           Analog input 1
    3      ANALOG-IN_2      Input           Analog input 2
    4      ANALOG-IN_3      Input           Analog input 3
    5      ANALOG-IN_4      Input           Analog input 4
    6      ANALOG-IN_5      Input           Analog input 5
    7      ANALOG-IN_6      Input           Analog input 6
    8      ANALOG-IN_7      Input           Analog input 7
    9      ANALOG-IN_8      Input           Analog input 8
    10     ANALOG-IN_9      Input           Analog input 9
    11     ANALOG-IN_10     Input           Analog input 10
    12     ANALOG-IN_11     Input           Analog input 11
    13     ANALOG-IN_12     Input           Analog input 12
    14     ANALOG-IN_13     Input           Analog input 13
    15     ANALOG-IN_14     Input           Analog input 14
    16     ANALOG-IN_15     Input           Analog input 15
    17     +3.0V_VREF2      Output          |LTC| 3.0V voltage reference
    18     FUSED_VBAT-      Output          GND
    ====   =============    ============    =============================

Each 8 analog inputs are connected to an analog multiplexer. The analog multiplexers can be controlled via I²C by the |LTC| (7-bit addresses: 0x4D and 0x4E). In order to ensure fast settling times after switching the multiplexer input, the output signals of the multiplexers are buffered by operational amplifiers. Finally the analog voltage of the selected sensor can be measured on the GPIO2 pin of the |LTC|.

Each 8 digital inputs/outputs are provided on the connectors X1504 (primary) and X1505 (secondary). The pinout for the connectors for the primary and secondary unit is identical and is described in :numref:`table %s <digital_io_connector>`.

.. _digital_io_connector:

.. table:: Connector for digital IOs

    ====   =============    ============    =============================
    Pin    Signal           Direction       Description
    ====   =============    ============    =============================
    1      DIGITAL-IO_0     Input/Output    Digital input/output 0
    2      DIGITAL-IO_1     Input/Output    Digital input/output 1
    3      DIGITAL-IO_2     Input/Output    Digital input/output 2
    4      DIGITAL-IO_3     Input/Output    Digital input/output 3
    5      DIGITAL-IO_4     Input/Output    Digital input/output 4
    6      DIGITAL-IO_5     Input/Output    Digital input/output 5
    7      DIGITAL-IO_6     Input/Output    Digital input/output 6
    8      +5.0V_VREG       Output          |LTC| 5.0V regulated voltage
    9      FUSED_VBAT-      Output          GND
    ====   =============    ============    =============================

Each 8 digital inputs/outputs are connected to an I²C controlled port expander (7-bit address: 0x20). The direction of the inputs/outputs as well as the logic levels on the pins can be selected by register settings. Each of the 8 digital inputs/outputs has a discrete pull up resistor that for example can be used for directly connecting a tactile switch.

-----------------------------
isoSPI Daisy Chain Connection
-----------------------------

The data transmission between the slaves and between the slaves and the basic board takes place using the isoSPI interface. The isoSPI signals are input on the connectors X1509 (primary) and X1511 (secondary). The isoSPI signals for daisy-chaining are output on the connectors X1510 (primary) and X1512 (secondary). The isoSPI connections are isolated galvanically using pulse transformers (TR201 / TR301). The voltage amplitude of the differential signal can be adjusted by setting resistors (see paragraph :ref:`hw_slave_daisy_current_12cell`).

The pinout of the isoSPI connectors is described in :numref:`table %s <daisy_input_connectors>` and :numref:`table %s <daisy_output_connectors>`.

.. figure:: ./twocon.png
   :width: 5 %

.. _daisy_input_connectors:

.. table:: isoSPI Daisy Chain Input Connectors

   =============  ==============================
   Connector Pin  Daisy Chain
   =============  ==============================
   1              IN+ (Primary/Secondary |LTC|)
   2              IN- (Primary/Secondary |LTC|)
   =============  ==============================

.. _daisy_output_connectors:

.. table:: isoSPI Daisy Chain Output Connectors

   =============  ==============================
   Connector Pin  Daisy Chain
   =============  ==============================
   1              OUT+ (Primary/Secondary |LTC|)
   2              OUT- (Primary/Secondary |LTC|)
   =============  ==============================

---------------------------
Hardware Settings / Options
---------------------------

Software Timer
--------------

The internal software timer of the |LTC| can be enabled/disabled by a dedicated external pin (SWTEN, pin 36 of the |LTC|). In order to support all features, the |BMS-Slave| board offers a possibility to switch the software timer. The software timer is enabled in the standard configuration, which means pin 36 is pulled to VREG via a zero-ohm resistor (R221/R321). The timer can be disabled by removing the resistor R221/R321 and placing a zero-ohm resistor to R220/R320.

.. _hw_slave_daisy_current_12cell:

Daisy Chain Communication Current
---------------------------------

The daisy chain communication current can be set by the resistors R206/R306 and R208/R308. The default value is 820Ω for R206/R306 and 1.21kΩ for R208/R308. This values result in a bias current of approximately 1mA and a differential signal amplitude of 1.18V. Theses values are suitable for high noise environments with cable lengths of over 50m. More information can be found in the |LTC| datasheet.

Status LED
----------

The status LEDs LD201 and LD301 show the current mode of each, the primary and secondary |LTC|. The LED is on in STANDBY, REFUP or MEASURE mode, whereas the LED is off in SLEEP mode. The LED can be disabled by removing the resistor R205 (primary) or R305 (secondary).

GPIO Extension Connector
------------------------

The internal GPIO lines of the primary or secondary |LTC| can be connected to the GPIO extension pin header X1508 via optional zero-ohm resistors. In the standard configuration these resistors are not placed. Of course it is possible to place each both resistors for a parallel connection of the internal signals to the GPIO extension connector. For more information see page 2/3 of the schematic file. The placement of the resistors and the resulting connection is shown in :numref:`table %s <gpio_connector>`.

.. _gpio_connector:

.. table:: GPIO extension connector

    ====   =====================    ============================
    GPIO   connect to pin header    connect to internal function
    ====   =====================    ============================
    1      R209/R309                R210/R310 (default)
    2      R211/R311                R212/R312 (default)
    3      R213/R313                R214/R314 (default)
    4      R215/R315                R216/R316 (default)
    5      R217/R317                R218/R318 (default)
    ====   =====================    ============================

The pinout of the extension connector X1508 is described in :numref:`table %s <extension_connector>`.

.. _extension_connector:

.. table:: Extension connector

    ====   ====================   ============    =========================================
    Pin    Signal                 Direction       Description
    ====   ====================   ============    =========================================
    1      +3.0V_VREF2_0          Output          Primary |LTC| 3.0V reference voltage 2
    2      +3.0V_VREF2_1          Output          Secondary |LTC| 3.0V reference voltage 2
    3      +5.0V_VREG_0           Output          Primary |LTC| 5.0V regulated voltage
    4      +5.0V_VREG_1           Output          Secondary |LTC| 5.0V regulated voltage
    5      PRIMARY-GPIO1-OPT      Input/Output    Primary |LTC| GPIO1
    6      SECONDARY-GPIO1-OPT    Input/Output    Secondary |LTC| GPIO1
    7      PRIMARY-GPIO2-OPT      Input/Output    Primary |LTC| GPIO2
    8      SECONDARY-GPIO2-OPT    Input/Output    Secondary |LTC| GPIO2
    9      PRIMARY-GPIO3-OPT      Input/Output    Primary |LTC| GPIO3
    10     SECONDARY-GPIO3-OPT    Input/Output    Secondary |LTC| GPIO3
    11     PRIMARY-GPIO4-OPT      Input/Output    Primary |LTC| GPIO4
    12     SECONDARY-GPIO4-OPT    Input/Output    Secondary |LTC| GPIO4
    13     PRIMARY-GPIO5-OPT      Input/Output    Primary |LTC| GPIO5
    14     SECONDARY-GPIO5-OPT    Input/Output    Secondary |LTC| GPIO5
    15     PRIMARY-WDT            Output          Primary |LTC| watchdog output
    16     SECONDARY-WDT          Output          Secondary |LTC| watchdog output
    17     PRIMARY-TEMP-ALERT     Output          Primary board T-sensor alarm output
    18     SECONDARY-TEMP-ALERT   Output          Secondary board T-sensor alarm output
    19     FUSED_VBAT-            Output          GND
    20     FUSED_VBAT-            Output          GND
    ====   ====================   ============    =========================================

---------------------------
External Isolated DC-Supply
---------------------------

.. note::

   The external isolated DC-supply is only available in the |BMS-Slave| hardware versions ``2.1.0`` and above.

It is possible to supply the |BMS-Slave| by an external DC power supply with a voltage range of 8V to 24V. The DC input is protected against reverse voltage and over-current (with a 1.25A fuse). The external DC supply has to be connected on connector X1513 or X1514 (both connectors are in parallel for daisy chaining the supply). The pinout of the connectors X1513 and X1514 is shown in :numref:`table %s <dc_connector>`.

.. figure:: ./twocon.png
   :width: 5 %

.. _dc_connector:

.. table:: External DC supply connector

    ====   =============    ============    ========================
    Pin    Signal           Direction       Description
    ====   =============    ============    ========================
    1      DC+              Input           positive supply terminal
    2      DC-              Input           negative supply terminal
    ====   =============    ============    ========================
