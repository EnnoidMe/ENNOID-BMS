.. include:: /macros.rst

.. _hw_slave_18cell:

##################################
Slave 18-Cell ``v1.1.3 and above``
##################################

========
Overview
========

.. important::

   The following description only applies for the 18-cell |BMS-Slave| hardware versions ``1.1.3`` and above.

.. hint::

   All connector pinouts described below follow the :ref:`sec-connector-convention`.

-------------
Block Diagram
-------------

.. _BMS-Slave_18cell:
.. figure:: ./slave-block-18cell.png
   :width: 100 %

   foxBMS BMS-Slave 18-Cell Block Diagram
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

The following general descriptions apply to both, the primary and the secondary unit on the foxBMS BMS-Slave. If there are any differences in hardware
between the primary and the secondary unit they will be marked as such.

------------------------
Cell Voltage Measurement
------------------------

The cell voltage sense lines are input on the connector X200. The pinout is described in :numref:`table %s <cell_sense_connector_18>`.

.. figure:: ./twentyfourcon.png
   :width: 50 %

.. _cell_sense_connector_18:

.. table:: Cell voltage sense connector

    ====   =============    ============    =================================
    Pin    Signal           Direction       Description
    ====   =============    ============    =================================
    1      VBAT-            Input           Battery module negative terminal
    2      CELL_0+          Input           Cell 0 positive terminal
    3      CELL_2+          Input           Cell 2 positive terminal
    4      CELL_4+          Input           Cell 4 positive terminal
    5      CELL_6+          Input           Cell 6 positive terminal
    6      CELL_8+          Input           Cell 8 positive terminal
    7      CELL_10+         Input           Cell 10 positive terminal
    8      CELL_12+         Input           Cell 12 positive terminal
    9      CELL_14+         Input           Cell 14 positive terminal
    10     CELL_16+         Input           Cell 16 positive terminal
    11     VBAT+            Input           Battery module positive terminal
    12     NC               --              --
    13     CELL_0-          Input           Cell 0 negative terminal
    14     CELL_1+          Input           Cell 1 positive terminal
    15     CELL_3+          Input           Cell 3 positive terminal
    16     CELL_5+          Input           Cell 5 positive terminal
    17     CELL_7+          Input           Cell 7 positive terminal
    18     CELL_9+          Input           Cell 9 positive terminal
    19     CELL_11+         Input           Cell 11 positive terminal
    20     CELL_13+         Input           Cell 13 positive terminal
    21     CELL_15+         Input           Cell 15 positive terminal
    22     CELL_17+         Input           Cell 17 positive terminal
    23     NC               --              --
    24     NC               --              --
    ====   =============    ============    =================================

Each of these lines is fused with a fast acting 250 mA surface-mount fuse (F301 - F319) on the board except of the VBAT+ and VBAT- lines which are fused with a
value of 500 mA (F300 and F320). This essentially is important for an evaluation environment. The VBAT+ and VBAT- connection is used for the internal power
supply of the slave board. If the battery module does not contain these separate wires to the positive and negative module terminal the solder jumpers SJ300
and SJ301 have to be shorted. In this case the slave will be supplied through the sense lines CELL_0- and CELL_11+. Running the slave in this configuration
could result in cell measurement errors due to voltage drop on the sense wires.

The cell input lines are filtered by a grounded or differential capacitor filter (both possibilities provided on the PCB). More information on the
corner frequency of this filtering can be found in the schematic. The grounded capacitor filter should be used in environments affected with
high noise as it offers a high level of battery voltage ripple rejection. The differential capacitor filter can be used when noise is less occurrent or the
design is subjected to cost optimization.

----------------------
Passive Cell Balancing
----------------------

The passive balancing circuit is realized by a parallel connection of two 130 Ohm discharge-resistors that can be connected to each single cell in parallel.
The MOSFET switches (T1500 - T1517) that control the connection to the cells are controlled by the primary |LTC-18| monitoring IC. The |LTC-18| on the secondary
unit does not support balancing. The resistor value of 2x 130 Ohm results in a balancing current of about 55mA at a cell voltage of 3.6V. This current results
in a power dissipation of about 0.2W per balancing channel (at 3.6V).

------------------------------
Global Cell Balancing Feedback
------------------------------

In order to check the proper function of the balancing process or to detect a malfunction in the balancing control circuit, a global balancing
feedback signal is connected to the |LTC-18|. This allows the system to check whether any balancing action is currently taking place at any time. The feedback
signal is connected to the GPIO3 of the |LTC-18|. The signal remains in a logic zero state until any balancing action on any cell starts.

------------------------------
Temperature Sensor Measurement
------------------------------

The cell temperature sensors are connected to the connectors X201 (primary) and X202 (secondary). The pinout is identical for the primary and secondary
unit and is described in :numref:`table %s <temp_sense_connector_18>`.

.. figure:: ./sixteencon.png
   :width: 30 %

.. _temp_sense_connector_18:

.. table:: Temperature sensor connector

    ====   =============    ============    ========================
    Pin    Signal           Direction       Description
    ====   =============    ============    ========================
    1      T-SENSOR_0       Input           NTC Sensor 0 terminal 1
    2      FUSED_VBAT-      Input           NTC Sensor 0 terminal 2
    3      T-SENSOR_1       Input           NTC Sensor 1 terminal 1
    4      FUSED_VBAT-      Input           NTC Sensor 1 terminal 2
    5      T-SENSOR_2       Input           NTC Sensor 2 terminal 1
    6      FUSED_VBAT-      Input           NTC Sensor 2 terminal 2
    7      T-SENSOR_3       Input           NTC Sensor 3 terminal 1
    8      FUSED_VBAT-      Input           NTC Sensor 3 terminal 2
    9      T-SENSOR_4       Input           NTC Sensor 4 terminal 1
    10     FUSED_VBAT-      Input           NTC Sensor 4 terminal 2
    11     T-SENSOR_5       Input           NTC Sensor 5 terminal 1
    12     FUSED_VBAT-      Input           NTC Sensor 5 terminal 2
    13     T-SENSOR_6       Input           NTC Sensor 6 terminal 1
    14     FUSED_VBAT-      Input           NTC Sensor 6 terminal 2
    15     T-SENSOR_7       Input           NTC Sensor 7 terminal 1
    16     FUSED_VBAT-      Input           NTC Sensor 7 terminal 2
    ====   =============    ============    ========================

Standard 10kΩ NTC resistors (e.g. Farnell-Nr. 1299926) are recommended for use. When using other values than these, the series resistors
(R100-R107) on the board may have to be adjusted. Please note that the accuracy of the internal voltage reference VREF2 decreases heavily with
a load of over 3mA. Using 8x 10kΩ NTC resistors with the corresponding 10kΩ series resistors results in a current of 1.2mA (at 20°C) which is drawn
from VREF2.

Each 8 temperature sensors are connected to an analog multiplexer.
The analog multiplexer can be controlled via I²C by the |LTC-18| (7-bit address: 0x4C). In order to ensure fast settling times after switching the
multiplexer input, the output signal of the multiplexer is buffered by an operational amplifier. Finally the analog voltage of the selected sensor
is measured on the GPIO1 pin of the |LTC-18|.

------------------------------
On-board EEPROM with unique ID
------------------------------

The primary unit as well as the secondary unit of the foxBMS BMS-Slave board is equipped with an EEPROM IC (IC801).
The EEPROM for example can be used for storing data such as calibration values. Similar to the analog multiplexers, the EEPROM device is connected to the I²C bus of the |LTC-18| (7-bit address: 0x50).

-----------------------------------
On-board Ambient Temperature Sensor
-----------------------------------

For an additional monitoring of the ambient temperature an on-board temperature sensor is used. This temperature sensor can be read by the |LTC-18| via
the I²C bus (7-bit address: 0x48). It is possible to program an alert temperature. Once the measured temperature reaches this alert temperature the alert pin
of the IC is set to a logic low level. Currently this signal is not used on the BMS-Slave board, but it is accessible on the connector X404.

-----------------------------
Additional Inputs and Outputs
-----------------------------

Several additional analog and digital inputs and outputs are provided on the BMS-Slave board via pin headers. Each 16 analog inputs are provided on
connector X400 (primary) and X401 (secondary). The pinout for the connectors for the primary and secondary unit is identical and is described in :numref:`table %s <analog_in_connector_18>`.

.. _analog_in_connector_18:

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
    17     +3.0V_VREF2      Output          |LTC-18| 3.0V voltage reference
    18     FUSED_VBAT-      Output          GND
    ====   =============    ============    =============================

Each 8 analog inputs are connected to an analog multiplexer. The analog multiplexers can be controlled via I²C by the
|LTC-18| (7-bit addresses: 0x4D and 0x4E). In order to ensure fast settling times after switching the multiplexer input, the output signals of the
multiplexers are buffered by operational amplifiers. Finally the analog voltage of the selected sensor can be measured on the GPIO2 pin of the |LTC-18|.

Each 8 digital inputs/outputs are provided on the connectors X402 (primary) and X403 (secondary). The pinout for the connectors for the primary
and secondary unit is identical and is described in :numref:`table %s <digital_io_connector_18>`.

.. _digital_io_connector_18:

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
    8      +5.0V_VREG       Output          |LTC-18| 5.0V regulated voltage
    9      FUSED_VBAT-      Output          GND
    ====   =============    ============    =============================

Each 8 digital inputs/outputs are connected to an I²C controlled port expander (7-bit address: 0x20). The direction of the inputs/outputs as well as the logic
levels on the pins can be selected by register settings. Each of the 8 digital inputs/outputs has a discrete pull up resistor that for example can be used
for directly connecting a tactile switch.

-----------------------------
isoSPI Daisy Chain Connection
-----------------------------

The data transmission between the slaves and between the slaves and the basic board takes place using the isoSPI interface. The isoSPI signals are input/output on
the connectors X500/X501 (primary) and X502/X503 (secondary). The isoSPI ports are bidirectional, that means they can be used in forward and reverse direction.
The isoSPI connections are isolated galvanically using pulse transformers (TR1400). The voltage amplitude of the differential signal can be adjusted
by setting resistors (see section :ref:`hw_slave_daisy_current_18cell`).

The pinout of the isoSPI connectors is described in :numref:`table %s <daisy_input_connectors_18>` and :numref:`table %s <daisy_output_connectors_18>`.

.. figure:: ./twocon.png
   :width: 5 %

.. _daisy_input_connectors_18:

.. table:: isoSPI Daisy Chain Input Connectors

   =============  ==============================
   Connector Pin  Daisy Chain
   =============  ==============================
   1              IN+ (Primary/Secondary |LTC-18|)
   2              IN- (Primary/Secondary |LTC-18|)
   =============  ==============================

.. _daisy_output_connectors_18:

.. table:: isoSPI Daisy Chain Output Connectors

   =============  ==============================
   Connector Pin  Daisy Chain
   =============  ==============================
   1              OUT+ (Primary/Secondary |LTC-18|)
   2              OUT- (Primary/Secondary |LTC-18|)
   =============  ==============================

---------------------------
Hardware Settings / Options
---------------------------

Software Timer
--------------

The internal software timer of the |LTC-18| can be enabled/disabled by a dedicated external pin (SWTEN, pin 36 of the |LTC-18|). In order to support all features, the foxBMS
BMS-Slave board offers a possibility to switch the software timer. The software timer is enabled in the standard configuration, which means pin 36 is
pulled to VREG via a zero-ohm resistor (R1407). The timer can be disabled by removing the resistor R1407 and placing a zero-ohm resistor to R1406.

.. _hw_slave_daisy_current_18cell:

Daisy Chain Communication Current
---------------------------------

The daisy chain communication current can be set by the resistors R1400 and R1402. The default value is 820Ω for R1402 and 1.2kΩ for R1400.
This values result in a bias current of approx. 1mA and a differential signal amplitude of 1.18V. Theses values are suitable for high noise environments with
cable lengths of over 50m. For more information please have a look at the |LTC-18| datasheet.

Status LED
----------

The status LEDs LD1400 show the current mode of each, the primary and secondary |LTC-18|. The LED is on in STANDBY, REFUP or MEASURE mode, whereas
the LED is off in SLEEP mode. The LED can be disabled by removing the resistor R1403 next to the LED.

GPIO Extension Connector
------------------------

The internal GPIO lines 1 to 5 of the primary or secondary |LTC-18| can be connected to the GPIO extension pin header X404 via optional zero-ohm resistors. In the
standard configuration these resistors are not placed. Of course it is possible to place each both resistors for a parallel connection of the internal
signals to the GPIO extension connector. For more information see the corresponding page of the schematics. The placement of the resistors and the resulting connection
is shown in :numref:`table %s <gpio_connector_18>`.

.. _gpio_connector_18:

.. table:: GPIO extension connector

    ====   =====================    ============================
    GPIO   connect to pin header    connect to internal function
    ====   =====================    ============================
    1      R1408                    R1409 (default)
    2      R1410                    R1411 (default)
    3      R1412                    R1413 (default)
    4      R1414                    R1415 (default)
    5      R1416                    R1417 (default)
    ====   =====================    ============================

The pinout of the extension connector X404 is described in :numref:`table %s <extension_connector_18>`.

.. _extension_connector_18:

.. table:: Extension connector

    ====   ====================   ============    =========================================
    Pin    Signal                 Direction       Description
    ====   ====================   ============    =========================================
    1      +3.0V_VREF2_0          Output          Primary |LTC-18| 3.0V reference voltage 2
    2      +3.0V_VREF2_1          Output          Secondary |LTC-18| 3.0V reference voltage 2
    3      +5.0V_VREG_0           Output          Primary |LTC-18| 5.0V regulated voltage
    4      +5.0V_VREG_1           Output          Secondary |LTC-18| 5.0V regulated voltage
    5      PRIMARY-GPIO1-OPT      Input/Output    Primary |LTC-18| GPIO1
    6      SECONDARY-GPIO1-OPT    Input/Output    Secondary |LTC-18| GPIO1
    7      PRIMARY-GPIO2-OPT      Input/Output    Primary |LTC-18| GPIO2
    8      SECONDARY-GPIO2-OPT    Input/Output    Secondary |LTC-18| GPIO2
    9      PRIMARY-GPIO3-OPT      Input/Output    Primary |LTC-18| GPIO3
    10     SECONDARY-GPIO3-OPT    Input/Output    Secondary |LTC-18| GPIO3
    11     PRIMARY-GPIO4-OPT      Input/Output    Primary |LTC-18| GPIO4
    12     SECONDARY-GPIO4-OPT    Input/Output    Secondary |LTC-18| GPIO4
    13     PRIMARY-GPIO5-OPT      Input/Output    Primary |LTC-18| GPIO5
    14     SECONDARY-GPIO5-OPT    Input/Output    Secondary |LTC-18| GPIO5
    15     PRIMARY-WDT            Output          Primary |LTC-18| watchdog output
    16     SECONDARY-WDT          Output          Secondary |LTC-18| watchdog output
    17     PRIMARY-TEMP-ALERT     Output          Primary board temp. sensor alarm output
    18     SECONDARY-TEMP-ALERT   Output          Secondary board temp. sensor alarm output
    19     FUSED_VBAT-            Output          GND
    20     FUSED_VBAT-            Output          GND
    ====   ====================   ============    =========================================

The GPIO lines 6 to 9 are wired to the connector X405 permanently. There is no internal function for this GPIO lines. The pinout of the extension
connector X405 is described in :numref:`table %s <gpio2_connector>`.

.. _gpio2_connector:

.. table:: Additional GPIO extension connector

    ====   ================       ============    ======================
    Pin    Signal                 Direction       Description
    ====   ================       ============    ======================
    1      PRIMARY-GPIO6          Input/Output    Primary |LTC-18| GPIO6
    2      SECONDARY-GPIO6        Input/Output    Secondary |LTC-18| GPIO6
    3      PRIMARY-GPIO7          Input/Output    Primary |LTC-18| GPIO7
    4      SECONDARY-GPIO7        Input/Output    Secondary |LTC-18| GPIO7
    5      PRIMARY-GPIO8          Input/Output    Primary |LTC-18| GPIO8
    6      SECONDARY-GPIO8        Input/Output    Secondary |LTC-18| GPIO8
    7      PRIMARY-GPIO9          Input/Output    Primary |LTC-18| GPIO9
    8      SECONDARY-GPIO9        Input/Output    Secondary |LTC-18| GPIO9
    ====   ================       ============    ======================

---------------------------
External Isolated DC-Supply
---------------------------

It is possible to supply the |BMS-Slave| by an external DC power supply with a voltage range of 8V to 24V. The DC input is protected against reverse voltage and over-current (with a 1.25A fuse).
The external DC supply has to be connected on connector X1001 or X1002 (both connectors are in parallel for daisy chaining the supply). The pinout of the connectors X1001 and X1002 is shown
in :numref:`table %s <dc_connector_18>`.

.. figure:: ./twocon.png
   :width: 5 %

.. _dc_connector_18:

.. table:: External DC supply connector

    ====   =============    ============    ========================
    Pin    Signal           Direction       Description
    ====   =============    ============    ========================
    1      DC+              Input           positive supply terminal
    2      DC-              Input           negative supply terminal
    ====   =============    ============    ========================
