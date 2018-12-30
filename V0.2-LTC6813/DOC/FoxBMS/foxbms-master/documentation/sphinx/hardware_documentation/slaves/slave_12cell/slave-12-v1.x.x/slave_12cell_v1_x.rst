.. include:: /macros.rst

.. _hw_slave_12cell_v1_x:

########################
Slave 12-Cell ``v1.x.x``
########################

========
Overview
========

.. note::

   The following description only applies for the |BMS-Slave| hardware versions ``1.x.x``.

.. hint::

   All connector pinouts described below follow the :ref:`sec-connector-convention`.

-------------
Block Diagram
-------------


A block diagram of a |BMS-Slave| is shown below:

.. _slave_diagram:
.. figure:: ./blockdiagram.png
   :width: 100 %

   Block diagram of a |BMS-Slave|
..

The connector indicated as ``Daisy Chain`` in :numref:`fig. %s <foxbmsfront_quickstart>` must be used on the |BMS-Master|. Its layout is described in the table below:

.. figure:: ./connector_16pin.png
   :width: 30 %

.. _master_daisy_connector:

.. table:: |BMS-Master| Daisy Chain Connector

   ====   ======================
   Pin    Signal
   ====   ======================
   1      NC
   2      OUT+ (Secondary |LTC-old|)
   3      OUT- (Secondary |LTC-old|)
   4      NC
   5      NC
   6      OUT+ (Primary |LTC-old|)
   7      OUT- (Primary |LTC-old|)
   8      NC
   9      NC
   10     NC
   11     NC
   12     NC
   13     NC
   14     NC
   15     NC
   16     NC
   ====   ======================

.. note::
   This connector pin out is only valid for use of a foxBMS Master Interface board for the |LTC-old| monitoring IC.

On the |BMS-Slave|, the connectors indicated as ``Primary Daisy Chain connector`` and ``Secondary Daisy Chain connector`` in :numref:`fig. %s <slave_diagram>` must be used. Their layout is described in :numref:`table %s <primary_daisy_connector>`.

.. figure:: ./connector_4pin.png
   :width: 10 %

.. _primary_daisy_connector:

.. table:: Primary Daisy Chain Connector

   =============  ============
   Connector Pin  Daisy Chain
   =============  ============
   1              IN+ (Primary |LTC-old|)
   2              OUT- (Primary |LTC-old|)
   3              IN- (Primary |LTC-old|)
   4              OUT+ (Primary |LTC-old|)
   =============  ============

The OUT+ and OUT- pins of the |BMS-Master| go to the IN+ and IN- pins of the |BMS-Slave|.  A cable with a receptable on both ends must be crimped correctly to make the connection.

In case a second |BMS-Slave| must be connected to the daisy chain, the OUT+ and OUT- pins of the first |BMS-Slave| must be connected to the IN+ and IN- pins of the second |BMS-Slave|.




Cell Voltage Connector on the |slaves|
--------------------------------------

The connector indicated as ``Battery cell connector (16 pin)`` in :numref:`fig. %s <slave_diagram>` has two purposes:

 * Supply of the |BMS-Slave|
 * Input of the cell voltages to the |LTC-old| monitoring chip

The layout of the connector is described in :numref:`table %s <cell_voltage_connector>`. Up to 12 battery cells can be connected in series, between VBAT+ and VBAT-. The |BMS-Slave| is supplied by VBAT+ and VBAT-. The total voltage of all cells in series must be between 11V and 55V (see [ltc_datasheet6804]_ and [ltc_datasheet6811]_). 0- correspond to the negative pole of cell 0, 0+ to the positive pole of cell 0, 1- correspond to the negative pole of cell 1, 1+ to the positive pole of cell 1 and so one till 11+, the positive pole of cell 11. As the cells are connected in series, the positive pole of one cell is connected to the negative pole of the next cell: 0+ to 1-, 1+ to 2+ and so on. The poles should be connected to the cell voltage connector as shown in :numref:`table %s <cell_voltage_connector>`.

If less than 12 battery cells are used, information on how to connect them can be found |LTC-old| datasheets ([ltc_datasheet6804]_ and [ltc_datasheet6811]_).

.. _cell_voltage_connector_layout:
.. figure:: ./connector_16pin.png
   :width: 30 %

.. _cell_voltage_connector:

.. table:: |BMS-Slave|, battery cell voltage connector

   =============  ============
   Connector Pin  Battery Cell
   =============  ============
   1              VBAT-
   2              0+ (1-)
   3              2+ (3-)
   4              4+ (5-)
   5              6+ (7-)
   6              8+ (9-)
   7              10+ (11-)
   8              NC
   9              0-
   10             1+ (2-)
   11             3+ (4-)
   12             5+ (6-)
   13             7+ (8-)
   14             9+ (10-)
   15             11+
   16             VBAT+
   =============  ============

In case no cells are available, they can be simulated with a series of voltage divider. A voltage supplied of 30V should be used and 12 resistors with the same value connected in series between the positive and negative connectors of the voltage supply. The positive connector is linked to VBAT+, the negative connector to VBAT- and each pole of a resistor correspond to  a pole of a battery cell. The voltage of 30V  is chosen so that every simulated cell voltage lies around 2.5V, which lies in the center of the safe operating area defined by default in the |foxbms| software.

Cell Temperature Connector on the |slaves|
------------------------------------------

The connector indicated as ``Temperature sensor connector (24 pin)`` in :numref:`fig. %s <slave_diagram>` is used to connect temperature sensors to the |BMS-Slave|.

:numref:`Table %s <cell_temperature_connector>` describes the temperature connector.

.. _cell_temperature_connector_layout:
.. figure:: ./connector_24pin.png
   :width: 45 %

.. _cell_temperature_connector:

.. table:: Temperature Sensor Connector

   =============  ==================
   Connector Pin  Temperature Sensor
   =============  ==================
   1, 24          T-Sensor 0
   2, 23          T-Sensor 1
   3, 22          T-Sensor 2
   4, 21          T-Sensor 3
   5, 20          T-Sensor 4
   6, 19          T-Sensor 5
   =============  ==================

:numref:`Fig. %s <temp_sensor>` shows the functioning of a temperature sensor.

.. _temp_sensor:
.. figure:: ./temperature_sensor.png
   :width: 30 %

   Temperature sensor circuit.

The voltage VREF (3V) is generated by the |LTC-old| chip. A temperature-dependent resistor must be added to build a voltage divider (drawn as a dashed line in :numref:`fig. %s <temp_sensor>`, not delivered with the |BMS-Slaves|). The resulting voltage is measured by the |LTC-old| chip. Knowing the temperature dependence of the resistor, the relation between measured voltage and temperature can be determined.

A function is present in the code to make the conversion between measured voltage and temperature. It must be adapted to the temperature sensor used. This is described in the software FAQ (:ref:`sw_faq_temperature_sensors`). In case temperatures are read incorrectly, this function is the first step to verify.

It must be noted that if no sensor is connected, 3V are measured. For the quickstart guide, no sensor needs to be connected: the conversion function is simply a multiplication by 10, so 30°C will be displayed, which again lies in the center of the safe operating area defined by default in the |foxbms| software.

If sensors are added, they must be connected between the connector pins corresponding to the sensors 0 to 5, as shown in :numref:`table %s <cell_temperature_connector>`.
