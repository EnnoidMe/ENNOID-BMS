.. include:: ../../macros.rst

.. _software_documentation_defines:

==========================
Important Switches in Code
==========================

.. highlight:: C

System Relevant Switches
========================

In ``embedded-software\mcu-primary\src\general\config\batterysystem_cfg.h``,
three important switches are defined.

The switch

.. code-block:: C

    #define CURRENT_SENSOR_PRESENT               TRUE

is by default set to ``TRUE``. In this configuration, the ``SYS`` statmachine
will go into an error state during startup if no current sensor is detected.
The switch must be set to ``FALSE`` for |foxBMS| to start without current
sensor.

The switch

.. code-block:: C

    #define CHECK_CAN_TIMING                     TRUE

is set to ``TRUE`` by default. In this configuration, the ``BMS`` statemachine
will go into an error state if no requests are made periodically per CAN with
a period of 100ms. When the switch is set to ``FALSE``, the check is not made.

The switch

.. code-block:: C

    #define BALANCING_DEFAULT_INACTIVE           TRUE

is set to ``TRUE`` by default. This prevents any balancing. In order for
balancing to be possiblem the switch must be set to ``FALSE``.

Battery Cell Relevant Switches
==============================

In ``embedded-software\mcu-primary\src\general\config\batterycell_cfg.h``, the
Safe Operating Area for the cells is defined:

 - Upper and lower voltage
 - Upper and lower temperature for charging
 - Upper and lower temperature for discharging
 - Upper current for charging and discharging

The battery cell capacity is also defined with

.. code-block:: C

    #define BC_CAPACITY 3500

The unit is mAh.


Deactivating Cell Tests
=======================

In ``embedded-software\mcu-primary\src\engine\config\diag_cfc.c``, the structure

.. code-block:: C

    DIAG_CH_CFG_s  diag_ch_cfg[]

allows disabling the checks made by the |mod_diag|. Disabling is made by replacing

.. code-block:: C

    DIAG_RECORDING_ENABLED, DIAG_ENABLED

with

.. code-block:: C

    DIAG_RECORDING_DISABLED, DIAG_DISABLED

Important checks are:

 - ``CELLVOLTAGE_OVERVOLTAGE``
 - ``CELLVOLTAGE_UNDERVOLTAGE``
 - ``OVERTEMPERATURE_CHARGE``
 - ``OVERTEMPERATURE_DISCHARGE``
 - ``UNDERTEMPERATURE_CHARGE``
 - ``UNDERTEMPERATURE_DISCHARGE``
 - ``OVERCURRENT_CHARGE``
 - ``OVERCURRENT_DISCHARGE``
 - ``LTC_PEC``

.. note::

    Setting the cell limits is safety relevant and must be done with care.

.. note::

    **Working without configuring the right battery cell voltage limits is
    dangerous and should never be done when real batteries are connected,
    since they may burn and explode when overcharged or shorted.**

Using Current Counter from Current Sensor
=========================================

Current-counting can now be made using the current sensor. |foxBMS| will
detect automatically if the corresponding data is being sent by the
current sensor. If yes, the hardware current-counter from the sensor is used.
If not, the software integrator is used.

Defining the Convention for the Current Direction
=================================================

Two functions have been defined in
``embedded-software\mcu-primary\src\general\config\batterycell_cfg.h`` to test
the current direction conveniently.

The first one

.. code-block:: C

    BS_CURRENT_DIRECTION_e BS_CheckCurrent_Direction(void);

gets the current from the database and returns ``BS_CURRENT_DISCHARGE`` if a
discharge current is flowing throug the battery, ``BS_CURRENT_CHARGE``
otherwise.

The second one

.. code-block:: C

    BS_CURRENT_DIRECTION_e BS_CheckCurrentValue_Direction(void);

functions the same way. The only difference is that is does not get the
current value from the database but uses the value passed to the function.

The define ``POSITIVE_DISCHARGE_CURRENT`` is used to define the current
direction convention:

 - if set to ``TRUE``, discharge currents are positive
 - if set to ``FALSE``, charge currents are positive

This define affect the calculation in the ``SOX`` module too.


Complete List of Switches
=========================
A complete list of switches can be found here:

-   The online version of the latest released version is found here:

    .. warning::
       This documentation always points to the latest realsed version on Github
       and the local changes can not be found in that online version.

    -   `Online documentation: Primary switches
        <https://iisb-foxbms.iisb.fraunhofer.de/foxbms/documentation/primary/html/CONFIG_PAGE.html>`_
    -   `Online documentation: Secondary switches
        <https://iisb-foxbms.iisb.fraunhofer.de/foxbms/documentation/secondary/html/CONFIG_PAGE.html>`_

-   If the project was compiled locally, this documentation can be accesed here:

    .. warning::
       This only exists after building the documentation locally. If this was
       not done, the link will not work.

    - `Local documentation: Primary switches <../../../../../../../primary/doxygen/html/CONFIG_PAGE.html>`_
    - `Local documentation: Secondary switches <../../../../../../../secondary/doxygen/html/CONFIG_PAGE.html>`_
