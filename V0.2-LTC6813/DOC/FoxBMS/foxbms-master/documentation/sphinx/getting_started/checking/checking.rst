.. include:: ../../macros.rst

.. _getting_started_checking:

====================
Checking-up |foxbms|
====================

This section describes how to test the |BMS-Master| with one |BMS-Slave|
connected. These tests show if the hardware and software components of the
|BMS-Master| and |BMS-Slave| are working correctly:

-----------------
Required Hardware
-----------------

 - 1 * |BMS-Master| including a supply cable
 - 1 * |BMS-Slave|
 - 1 * Battery voltage simulation at the |BMS-Slave| (e.g., voltage divider)
 - 1 * CAN-Bus to PC adapter
 - 1 * Normally closed switch for opening the |il| (hereafter referred to as
   |eo|)
 - 3 * Contactors with normally open feedback (hereafter referred to as |pmc|,
   |ppc|, |mmc|)
 - 1 * Debugger (optionally, but recommended)


Testing Without Debugger
------------------------

It is also possible to get little information if the system is running
correctly without variable checking on the debugger. When requesting the later
described states to the BMS, the contactors opening and closing can be heard.

Testing With Debugger
---------------------

This test procedure gives more detailed test information when a debugger is
used. The following variables should be checked during the test:

- ``os_timer``
- ``sys_state``
- ``bms_state``
- ``ltc_cellvoltage``
- ``cont_contactor_states``
- ``ilck_interlock_state``

-----------------
Required Software
-----------------

- |foxbms| binaries for |MCU0| (`latest release on GitHub for primary <https://github.com/foxbms/mcu-primary>`_)
- |foxbms| binaries for |MCU1| (`latest release on GitHub for secondary <https://github.com/foxbms/mcu-secondary>`_)
- Debugger-Software
- Software to send CAN messages from the PC on the CAN bus.
- CAN message for |normal| and |stdby|

--------------
Test Procedure
--------------

The test procedure consists of three steps:

 #. Preparing the hardware
 #. Building the software from source
 #. Requesting bms-states to |BMS-Master| and checking variable values on the
    debugger

If no debugger is available, only a partially test if all parts of |foxbms| are
running correctly can be performed. As mentioned above, it can be checked
accoustically if the contactors are opening and closing. This is marked in the
`Test`_ with |hear| and the corresponding explanation if needed.

Hardware Setup
--------------

#. Apply voltages to the |BMS-Slave| cell measurement inputs (e.g., voltage divider)
#. Connect the daisy chain connector from the |BMS-Slave| to the
   |BMS-Master|.
#. Connect the |eo| to the |il| of the |BMS-Master|.
#. Connect the following contactors to the |BMS-Master|:

   #. ``contactor 0`` the |pmc| in the positive current path
   #. ``contactor 1`` the |ppc| in the positive current path
   #. ``contactor 2`` the |mmc| in the negative current path

#. Connect the CAN-interface to the PC
#. The debugger is connected from the PC to the JTAG-interface of |MCU0| of
   |BMS-Master| (optional).

Software Setup
--------------

#. Build the |foxbms| binaries for both, |MCU0| and |MCU1|.

Test
----

#. Power |BMS-Master|
#. Flash |foxbms| |MCU0| binaries on |MCU0|
#. Flash |foxbms| |MCU1| binaries on |MCU1|
#. Start CAN-communication viewer (e.g., PCAN-View when using PCAN-USB)
#. Restart |BMS-Master|
#. Send CAN message for |req_stdby|
#. Check on the debugger if the system timer is running; variable: ``os_timer``
#. Check if |BMS-Slave| reads voltages; variable: ``ltc_cellvoltage``
#. |req_stdby|

   #. Check on the debugger if |il| is closed (|var_il| with ``ilck_interlock_state.feedback=ILCK_SWITCH_ON``)

#. |req_normal|

   #. Check on the debugger if contactors are closed in the correct order (|var_co|)

      #. close |mmc|
      #. close |ppc|
      #. close |pmc|
      #. open |ppc|

   |hear|: If this test is performed with no debugger, contactors can be clicking four times.


#. |req_stdby|

   #. Check on the debugger if |il| is closed (|var_il|)
   #. Check on the debugger if contactors are opened (|var_co|)

   |hear|: If this test is performed with no debugger, each contactor can be heard clicking one time.


#. |req_normal|

   #. Check on the debugger if contactors are closed in the correct order (|var_co|)

      #. close |mmc|
      #. close |ppc|
      #. close |pmc|
      #. open |ppc|

   |hear|: If this test is performed with no debugger, contactors can be heard clicking four times.


#. Open |il| by pressing |eo|

   #. Check on the debugger if |il| is opened (|var_il|)
   #. Check on the debugger if contactors are opened (|var_co|)

   |hear|: If this test is performed with no debugger, each contactor can be heard clicking one time.

#. |req_normal|

   #. BMS should switch to ``bms_state.state=BMS_STATEMACH_ERROR`` as the |il| is still open (|var_bms_state|)

#. Close |il| by releasing |eo|

   #. Check on the debugger if |il| is still open (|var_il|)
   #. BMS has to stay in error state (check |var_bms_state|)

#. |req_stdby|

   #. Check on the debugger if |il| is closed (|var_il|)
   #. Check on the debugger if contactors are still open (|var_co|)

#. |req_normal|

   #. Check on the debugger if contactors are closed in the correct order (|var_co|)

      #. close |mmc|
      #. close |ppc|
      #. close |pmc|
      #. open |ppc|

   |hear|: If this test is performed with no debugger, contactors can be heard clicking four times.

#. |req_stdby|

   #. Check on the debugger if |il| is closed (|var_il|)
   #. Check on the debugger if contactors are still open (|var_co|)

#. done
