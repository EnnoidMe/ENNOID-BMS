.. include:: ../../macros.rst

.. _software_documentation_modules:

================
Software Modules
================

This section contains the documentation of the most important software modules.

The is section is organized the same way the sources are organized (``Driver``,
``foxBMS-modules``, ``Engine``, ``Utils`` and ``Application``).

.. highlight:: C

.. toctree::
    :maxdepth: 2
    :caption: Application

    ./algo/algo
    ./balancing/balancing
    ./bms/bms
    ./com/com
    ./sox/sox

.. toctree::
    :maxdepth: 2
    :caption: Engine

    ./database/database
    ./diag/diag
    ./sys/sys
    ./nvramhandler/nvramhandler

.. toctree::
    :maxdepth: 2
    :caption: foxBMS-Modules

    ./cansignal/cansignal
    ./contactor/contactor
    ./interlock/interlock
    ./isoguard/isoguard
    ./ltc/ltc

.. toctree::
    :maxdepth: 2
    :caption: Driver

    ./can/can
    ./chksum/chksum
    ./io/io
    ./nvm/nvm
    ./uart/uart
