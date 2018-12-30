.. include:: ../../../macros.rst

.. _SOX:

===
SOX
===

.. highlight:: C

This section describes where and how to implement state estimation algorithms (e.g., SOC, SOH, SOF). The basic SOC calculation is done by a simple Coulomb counter. Its implementation is shown here. Current derating depending on cell voltages, temperatures and SOC is performed to compute the SOF.

Module Files
~~~~~~~~~~~~

Driver:
 - ``embedded-software\mcu-primary\src\application\sox\sox.h`` (:ref:`soxc`)
 - ``embedded-software\mcu-primary\src\application\sox\sox.c`` (:ref:`soxh`)

Driver Configuration:
 - ``embedded-software\mcu-primary\src\application\config\sox_cfg.h`` (:ref:`soxcfgc`)
 - ``embedded-software\mcu-primary\src\application\config\sox_cfg.c`` (:ref:`soxcfgh`)

Detailed Description
~~~~~~~~~~~~~~~~~~~~

The |mod_sox| gets the relevant measurement minimum and maximum values from the database and stores the current derating values in the database.

SOC - State of Charge
---------------------

The state of charge estimation (SOC) is implemented in the form of a simple Coulomb counter. The SOC initialization is done after startup by reading the value from the non-volatile memory. Not implemented right now is a initialization of the SOC by VOLTAGE-SOC relation (lookup table), but configuration placeholders are already in `SOX Configuration`_. These placeholders define the constraints at which the initialization with lookup table is valid.

SOF - State of Function
-----------------------

The state of function estimation (SOF) consists of current derating values.
These charge and discharge derating values are calculated according to battery
cell specific constraints. For this, three parameters are taken into account:

 - temperature
 - voltage
 - state-of-charge

Four different curves are calculated:

 - recommended operating limit (ROL) (this is the recommended operating current transmitted on the CAN bus)
 - maximum operating limit (MOL) (if the floating current exceeds this threshold, the overcurrent warning flag is set)
 - recommended safety limit (RSL) (if the floating current exceeds this threshold, the overcurrent alarm flag is set)
 - (absolut) maximum safety limit (MSL) (if the floating current exceeds this threshold, the overcurrent error flag is set and the contactors are opened)

Specific points in the following derating curves have to be defined by configurable defines:

.. figure:: sof_t_3alarms.png
   :width: 100 %

   Temperature dependent current derating with three alarm levels (warning, alarm, error)

.. figure:: sof_v_3alarms.png
   :width: 100 %

   Voltage dependent current derating with three alarm levels (warning, alarm, error)

.. figure:: sof_soc_3alarms.png
   :width: 100 %

   SOC dependent current derating with three alarm levels (warning, alarm, error)

These specific points are the values where derating starts and where it is at
full extent. They are described in `Configuration Variables`_ and battery
specific values for the maximum allowed current can be seen for example in
:ref:`SOX_CONFIG_EX_LTO` and :ref:`SOX_CONFIG_EX_NCA_NMC`.


.. _SOX_CONFIG:

SOX Configuration
~~~~~~~~~~~~~~~~~

Configuration Variables
-----------------------

For the Coulomb counting method the cell capacity (in case of parallel cell
configuration, it is the sum of the parallel connected cells) has to be given
here:

========================  =========   =====  ========   =============================================  ===============
NAME                      LEVEL       TYPE     UNIT     DESCRIPTION                                    DEFAULT
========================  =========   =====  ========   =============================================  ===============
SOX_CELL_CAPACITY         devel       float    mAh      cell capacity in SOC formula coulomb counter   20000.0
========================  =========   =====  ========   =============================================  ===============

Currently there is only placeholder for the initialization by a Voltage-SOC relation. The following configuration can be used after implementation:

===========================   =====  ========   ============================================  ===============
NAME                          TYPE     UNIT     DESCRIPTION                                   DEFAULT
===========================   =====  ========   ============================================  ===============
SOX_SOC_INIT_CURRENT_LIMIT    int      mA       at initialization the current must be below    100
SOX_DELTA_MIN_LIMIT           int      mV       see source code                                10
SOX_DELTA_MAX_LIMIT           int      mV       see source code                                10
===========================   =====  ========   ============================================  ===============

These are the configuration variables of the ROL, MOL, RSL and MSL:

==================================== ===== ===== ====== ============================================= ===============
NAME                                 LEVEL TYPE  UNIT   DESCRIPTION                                   VALIDATOR
==================================== ===== ===== ====== ============================================= ===============
SOX_CURRENT_MAX_CONTINUOUS_CHARGE    devel float A      maximum current continuous charge              1<x<240
SOX_CURRENT_MAX_CONTINUOUS_DISCHARGE devel float A      maximum current continuous discharge           1<x<240
SOX_CURRENT_LIMP_HOME                devel float A      discharge current in limp home emergency mode  1<x<40
SOX_TEMP_LOW_CUTOFF_DISCHARGE        devel float °C     low temperature discharge derating start      -40.0<x<80.0
SOX_TEMP_LOW_LIMIT_DISCHARGE         devel float °C     low temperature discharge derating full       -40.0<x<80.0
SOX_TEMP_LOW_CUTOFF_CHARGE           devel float °C     low temperature charge derating start         -40.0<x<80.0
SOX_TEMP_LOW_LIMIT_CHARGE            devel float °C     low temperature charge derating full          -40.0<x<80.0
SOX_TEMP_HIGH_CUTOFF_DISCHARGE       devel float °C     low temperature discharge derating start      -40.0<x<80.0
SOX_TEMP_HIGH_LIMIT_DISCHARGE        devel float °C     low temperature discharge derating full       -40.0<x<80.0
SOX_TEMP_HIGH_CUTOFF_CHARGE          devel float °C     low temperature charge derating start          -40.0<x<80.0
SOX_TEMP_HIGH_LIMIT_CHARGE           devel float °C     low temperature charge derating full           -40.0<x<80.0
SOX_SOC_CUTOFF_CHARGE                devel int   0.01%  high SOC derating starts                       0<=x<=10000
SOX_SOC_LIMIT_CHARGE                 devel int   0.01%  high SOC derating full extent                  0<=x<=10000
SOX_SOC_CUTOFF_DISCHARGE             devel int   0.01%  low SOC derating starts                        0<=x<=10000
SOX_SOC_LIMIT_DISCHARGE              devel int   0.01%  low SOC derating full extent                   0<=x<=10000
SOX_VOLT_CUTOFF_CHARGE               devel int   mV     high voltage derating starts                   0<=x<=5000
SOX_VOLT_LIMIT_CHARGE                devel int   mV     high voltage derating full extent              0<=x<=5000
SOX_VOLT_CUTOFF_DISCHARGE            devel int   mV     low voltage derating starts                    0<=x<=5000
SOX_VOLT_LIMIT_DISCHARGE             devel int   mV     low voltage derating full extent               0<=x<=5000
==================================== ===== ===== ====== ============================================= ===============

These configuration values are building the main safety feature and are therefore considered highly safety-relevant.

.. _SOX_CONFIG_EX_LFP:

Configuration Example for Lithium-Ion LFP/Graphite Chemistry
------------------------------------------------------------

This configuration is very conservative and the limits are defensive. It is the default standard configuration.
These values must be adapted to the specific battery cells used.

==================================== ======  ======= =============================================
NAME                                 UNIT    VALUE   DESCRIPTION
==================================== ======  ======= =============================================
SOX_CURRENT_MAX_CONTINUOUS_CHARGE    A       10.00   maximum current continuous charge
SOX_CURRENT_MAX_CONTINUOUS_DISCHARGE A       10.00   maximum current continuous discharge
SOX_CURRENT_LIMP_HOME                A       3.00    discharge current in limp home emergency mode
SOX_TEMP_LOW_CUTOFF_DISCHARGE        °C      5.0     low temperature discharge derating start
SOX_TEMP_LOW_LIMIT_DISCHARGE         °C      -5.0    low temperature discharge derating full
SOX_TEMP_LOW_CUTOFF_CHARGE           °C      10.0    low temperature charge derating start
SOX_TEMP_LOW_LIMIT_CHARGE            °C      0.0     low temperature charge derating full
SOX_TEMP_HIGH_CUTOFF_DISCHARGE       °C      45.0    low temperature discharge derating start
SOX_TEMP_HIGH_LIMIT_DISCHARGE        °C      55.0    low temperature discharge derating full
SOX_TEMP_HIGH_CUTOFF_CHARGE          °C      30.0    low temperature charge derating start
SOX_TEMP_HIGH_LIMIT_CHARGE           °C      37.0    low temperature charge derating full
SOX_SOC_CUTOFF_CHARGE                0.01%   8500    high SOC derating starts
SOX_SOC_LIMIT_CHARGE                 0.01%   9500    high SOC derating full extent
SOX_SOC_CUTOFF_DISCHARGE             0.01%   1500    low SOC derating starts
SOX_SOC_LIMIT_DISCHARGE              0.01%    500    low SOC derating full extent
SOX_VOLT_CUTOFF_CHARGE               mV      3300    high voltage derating starts
SOX_VOLT_LIMIT_CHARGE                mV      3550    high voltage derating full extent
SOX_VOLT_CUTOFF_DISCHARGE            mV      2700    low voltage derating starts
SOX_VOLT_LIMIT_DISCHARGE             mV      2300    low voltage derating full extent
==================================== ======  ======= =============================================

.. _SOX_CONFIG_EX_LTO:

Configuration Example for Lithium-Ion NMC/LTO Chemistry
-------------------------------------------------------

==================================== ======  ======= =============================================
NAME                                 UNIT    VALUE   DESCRIPTION
==================================== ======  ======= =============================================
SOX_CURRENT_MAX_CONTINUOUS_CHARGE    A       120.00  maximum current continuous charge
SOX_CURRENT_MAX_CONTINUOUS_DISCHARGE A       120.00  maximum current continuous discharge
SOX_CURRENT_LIMP_HOME                A       20.00   discharge current in limp home emergency mode
SOX_TEMP_LOW_CUTOFF_DISCHARGE        °C      0.0     low temperature discharge derating start
SOX_TEMP_LOW_LIMIT_DISCHARGE         °C      -10.0   low temperature discharge derating full
SOX_TEMP_LOW_CUTOFF_CHARGE           °C      0.0     low temperature charge derating start
SOX_TEMP_LOW_LIMIT_CHARGE            °C      -10.0   low temperature charge derating full
SOX_TEMP_HIGH_CUTOFF_DISCHARGE       °C      45.0    low temperature discharge derating start
SOX_TEMP_HIGH_LIMIT_DISCHARGE        °C      55.0    low temperature discharge derating full
SOX_TEMP_HIGH_CUTOFF_CHARGE           °C     45.0    low temperature charge derating start
SOX_TEMP_HIGH_LIMIT_CHARGE            °C     55.0    low temperature charge derating full
SOX_SOC_CUTOFF_CHARGE                 0.01%  8500    high SOC derating starts
SOX_SOC_LIMIT_CHARGE                  0.01%  9500    high SOC derating full extent
SOX_SOC_CUTOFF_DISCHARGE              0.01%  1500    low SOC derating starts
SOX_SOC_LIMIT_DISCHARGE               0.01%   500    low SOC derating full extent
SOX_VOLT_CUTOFF_CHARGE                mV     2400    high voltage derating starts
SOX_VOLT_LIMIT_CHARGE                 mV     2550    high voltage derating full extent
SOX_VOLT_CUTOFF_DISCHARGE             mV     2000    low voltage derating starts
SOX_VOLT_LIMIT_DISCHARGE              mV     1750    low voltage derating full extent
==================================== ======  ======= =============================================

.. _SOX_CONFIG_EX_NCA_NMC:

Configuration Example for Lithium-Ion NCA/Graphite or NMC/Graphite Chemistries
------------------------------------------------------------------------------

==================================== ======  ======= =============================================
NAME                                 UNIT    VALUE   DESCRIPTION
==================================== ======  ======= =============================================
SOX_CURRENT_MAX_CONTINUOUS_CHARGE    A        80.00  maximum current continuous charge
SOX_CURRENT_MAX_CONTINUOUS_DISCHARGE A       200.00  maximum current continuous discharge
SOX_CURRENT_LIMP_HOME                A       40.00   discharge current in limp home emergency mode
SOX_TEMP_LOW_CUTOFF_DISCHARGE        °C      25.0    low temperature discharge derating start
SOX_TEMP_LOW_LIMIT_DISCHARGE         °C      -10.0   low temperature discharge derating full
SOX_TEMP_LOW_CUTOFF_CHARGE           °C      20.0    low temperature charge derating start
SOX_TEMP_LOW_LIMIT_CHARGE            °C      10.0    low temperature charge derating full
SOX_TEMP_HIGH_CUTOFF_DISCHARGE       °C      45.0    low temperature discharge derating start
SOX_TEMP_HIGH_LIMIT_DISCHARGE        °C      55.0    low temperature discharge derating full
SOX_TEMP_HIGH_CUTOFF_CHARGE           °C     35.0    low temperature charge derating start
SOX_TEMP_HIGH_LIMIT_CHARGE            °C     45.0    low temperature charge derating full
SOX_SOC_CUTOFF_CHARGE                 0.01%  8500    high SOC derating starts
SOX_SOC_LIMIT_CHARGE                  0.01%  9500    high SOC derating full extent
SOX_SOC_CUTOFF_DISCHARGE              0.01%  1500    low SOC derating starts
SOX_SOC_LIMIT_DISCHARGE               0.01%   500    low SOC derating full extent
SOX_VOLT_CUTOFF_CHARGE                mV     4000    high voltage derating starts
SOX_VOLT_LIMIT_CHARGE                 mV     4100    high voltage derating full extent
SOX_VOLT_CUTOFF_DISCHARGE             mV     3100    low voltage derating starts
SOX_VOLT_LIMIT_DISCHARGE              mV     2750    low voltage derating full extent
==================================== ======  ======= =============================================
