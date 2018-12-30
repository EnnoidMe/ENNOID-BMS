.. include:: ../../../macros.rst

.. _DIAGNOSIS:

Diagnosis
=========

.. highlight:: C

The |mod_uart| is part of the ``Engine`` layer.

The diagnose module is responsible for error handling, error reporting and
system monitoring.

Module Files
~~~~~~~~~~~~

Driver:
 - ``embedded-software\mcu-primary\src\engine\diag\diag.c`` (:ref:`diagprimaryc`)
 - ``embedded-software\mcu-primary\src\engine\diag\diag.h`` (:ref:`diagprimaryh`)
 - ``embedded-software\mcu-secondary\src\engine\diag\diag.c`` (:ref:`diagsecondaryc`)
 - ``embedded-software\mcu-secondary\src\engine\diag\diag.h`` (:ref:`diagsecondaryh`)

Driver Configuration:
 - ``embedded-software\mcu-primary\src\engine\config\diag_cfg.c``      (:ref:`diagcfgprimaryc`)
 - ``embedded-software\mcu-primary\src\engine\config\diag_cfg.h``      (:ref:`diagcfgprimaryh`)
 - ``embedded-software\mcu-primary\src\engine\config\diag_id_cfg.h``   (:ref:`diagidcfgprimaryh`)
 - ``embedded-software\mcu-secondary\src\engine\config\diag_cfg.c`` (:ref:`diagcfgsecondaryc`)
 - ``embedded-software\mcu-secondary\src\engine\config\diag_cfg.h`` (:ref:`diagcfgsecondaryh`)
 - ``embedded-software\mcu-secondary\src\engine\config\diag_id_cfg.h`` (:ref:`diagidcfgsecondaryh`)

Description
~~~~~~~~~~~

The |mod_diag| consists of 2 independent main parts, diagnosis handling and system monitoring. When configured, reported errors are logged into the global diagnosis memory and a callback function can be triggered.

The handler counts errors and calls a callback function when the configured error threshold is exceeded. The callback function is called again only when the error counter go back to zero after the error threshold was reached.

The initialization of the diagnosis module has to be done during start-up after the diagnosis memory is available (e.g. after Backup-SRAM is accessible). The Backup-SRAM Flag ``DIAG_DATA_IS_VALID`` indicates the data validity in diagnosis memory.

Two types of handling are defined:

- the general handler with debounce filter and thresholds for entering and exiting error state
- the contactor handler which counts all switching actions and reports when contactors are opened while the current flowing through the battery is above the configured threshold

Module Files
~~~~~~~~~~~~

Driver:
 - embedded-software\mcu-primary\src\engine\diag\diag.c
 - embedded-software\mcu-primary\src\engine\diag\diag.h

Driver Configuration:
 - embedded-software\mcu-primary\src\engine\config\diag_cfg.c
 - embedded-software\mcu-primary\src\engine\config\diag_cfg.h
 - embedded-software\mcu-primary\src\engine\config\diag_id.h

Usage
~~~~~

Diagnosis Handling
------------------

For using the diagnosis handler for a specific check in a module, a free diagnosis id has to be defined in ``diag_cfg.h``
and included as an additional diagnosis channel in ``diag_cfg.c``:

.. code-block:: C

   diag_cfg.h:
   #define     DIAG_ISOMETER_ERROR                 DIAG_ID_37           // Device error, invalid measurement result

   diag_cfg.c:
   DIAG_CH_CFG_s  diag_ch_cfg[]= {
     ...
     {DIAG_ISOMETER_ERROR,DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_MID,  DIAG_RECORDING_ENABLED, DIAG_ENABLED, callbackfunction},
     ...
   };

Where error counting is needed, the diagnosis handler has to be called in the following way:

.. code-block:: C

     if( <error detected>)
     {
          retVal = DIAG_Handler(DIAG_ISOMETER_ERROR, DIAG_EVENT_OK, 0, 0);
          if (retVAl != DIAG_HANDLER_RETURN_OK)
          {
               /* here implement local (directly) diagnosis handling */
          }
     }
     else
     {
          DIAG_Handler(DIAG_ISOMETER_ERROR, DIAG_EVENT_NOK, 0, 0);
     }


The callback function

.. code-block:: C

   void callbackfunction(void) {
      /* here implement (indirectly) diagnosis handling */
   }

is called when the error threshold is reached or when the counter goes back to zero after the threshold was reached. Typically, an error flag stored in the database is set or unset in the callback function.


System Monitoring
-----------------

For using the system monitor for a specific task or function, a free monitoring channel ID has to be defined in ``diag_cfg.h``:

.. code-block:: C

   typedef enum {
      DIAG_SYSMON_DATABASE_ID         = 0,
      DIAG_SYSMON_SYS_ID          = 1,
      DIAG_SYSMON_LTC_ID              = 2,
      DIAG_SYSMON_ISOGUARD_ID         = 3,
      DIAG_SYSMON_CANS_ID             = 4,
      DIAG_SYSMON_APPL_CYCLIC_10ms    = 5,
      DIAG_SYSMON_APPL_CYCLIC_100ms   = 6,
      DIAG_SYSMON_BMS_ID          = 7,
      DIAG_SYSMON_MODULE_ID_MAX       = 8    /* do not delete endmarker */
   } DIAG_SYSMON_MODULE_ID_e;

and a new channel configured in  ``diag_cfg.c``:

.. code-block:: C

   DIAG_SYSMON_CH_CFG_s  diag_sysmon_ch_cfg[] = {
      ...
      {DIAG_SYSMON_ISOGUARD_ID,   DIAG_SYSMON_CYCLICTASK,  400, DIAG_RECORDING_ENABLED, DIAG_ENABLED, callbackfunction},
      ...
   };

In this example, a timeout of 400 ms is defined. In the corresponding task or cyclic called function, a notification to the system monitor has to be done within the configured timeout by passing the state value, here 0 (ok),

Example:

.. code-block:: C

   my_isoguardfunction() {
      ...
      DIAG_SysMonNotify(DIAG_SYSMON_ISOGUARD_ID,0);
      ...
   }
