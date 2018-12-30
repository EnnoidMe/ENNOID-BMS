.. include:: ../../../macros.rst

.. _CANSIGNAL:

==========
CAN Signal
==========

.. highlight:: C

The |mod_cansignal| is part of the ``Driver`` layer.

The |mod_cansignal| is a software module to handle the conversion from data
providers like a database or measurement modules to the |mod_can|. It works
similar to a typical IPO (input-processing-output) pattern.

Module Files
~~~~~~~~~~~~

Driver:
 - ``embedded-software\mcu-common\src\module\cansignal\cansignal.h``
 - ``embedded-software\mcu-common\src\module\cansignal\cansignal.c``

Driver Configuration:
 - ``embedded-software\mcu-primary\src\module\config\cansignal_cfg.h``
 - ``embedded-software\mcu-primary\src\module\config\cansignal_cfg.c``

Detailed Description
~~~~~~~~~~~~~~~~~~~~

File Structure and Interfaces
-----------------------------
The |mod_cansignal| is a simple one-file module with one-file configuration:

 - The module itself consists of one file ``cansignal.c`` and its associated ``cansignal.h``
 - The configuration is given in ``cansignal_cfg.c`` and its associated ``cansignal_cfg.h``

The external interface to the |mod_cansignal| is very easy and consists of just two functions:

 - CANS_Init
 - CANS_MainFunction

CANS_Init is for parameter and configuration checking of the |mod_cansignal|.

CANS_MainFunction is for data processing and should be called periodically.


Data Flow
---------
The data flow is generally divided in two different domains, one for reception of CAN messages and signal value distribution, the other for transmission of CAN message and signal value assembling/message composition.

When a CAN message is received physically, it is stored in a buffer which is polled periodically by ``CANS_MainFunction()``.
In case of a match, the corresponding signals of the message are extracted, scaled and handed over to a data consumer via the callback
setter function.

When a CAN message needs to be transmitted physically, the signals data belonging to this CAN message are collected via their getter callback function. From this signals data, the CAN message is assembled. If everything worked fine, it is stored in a buffer and sent out over the CAN peripheral hardware.

:numref:`fig. %s <cansignal_figure1>` shows the data flow used to assemble a CAN message.

.. _cansignal_figure1:
.. figure:: cans_msg_handling_xxl.png
   :width: 100 %

   CAN data flow to assemble messages


Control Flow
------------

The module operates from a single function call to ``CANS_MainFunction()``.

First the periodic message transmission is handled in this function. If correspondence of an internal tick counter to a message repetition time and phase is detected (i.e., the periodic time expired) the CAN message is composed from its signals. Therefore all signals, which are included in a message, are collected via their getter callback and written to the message data block at the right position in the right length. This message data block together with the ID, Data Length Code and so on, is handed over to the |mod_can|, which handles the low level transmission to the CAN specific peripheral registers.

The message reception in turn is done by reading out the buffer of the |mod_can|. Then the signals configuration is searched for this message(s). If one signal is represented in these received messages it is extracted and handed over to the setter callback function configured for this signal.

.. _CANSIGNAL_CAN_CONFIG:

CAN Configurations
~~~~~~~~~~~~~~~~~~

Default Configuration
---------------------

The configuration comprises:

* enums for both receive and transmit message names (``CANS_messagesTx_e`` respectively ``CANS_messagesRx_e``)
* enums for both receive and transmit signal names (``CANS_CANx_signalsTx_e`` respectively ``CANS_CANx_signalsRx_e``)
* arrays for signal definition, for both receive and transmit signals
* callback function implementations for getting and setting of signals
* callback function implementations for post-processing after transmission and reception of messages

Custom Configuration Examples
-----------------------------

See FAQ section on how to manually add/delete a CAN message.

Usage/Examples
~~~~~~~~~~~~~~

To use |mod_can| and |mod_cansignal| with a correct :ref:`CANSIGNAL_CAN_CONFIG`, just call ``CANS_MainFunction`` in the cyclic tasks timeslot, that is configured in ``CANS_TICK_MS``.
