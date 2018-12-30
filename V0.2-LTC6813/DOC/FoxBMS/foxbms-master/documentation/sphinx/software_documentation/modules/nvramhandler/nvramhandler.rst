.. include:: ../../../macros.rst

.. _NVRAMHANDLER:

=============
NVRAM-Handler
=============

.. highlight:: C

The |mod_nvmh| is part of the ``Engine`` layer.

The |mod_nvmh| provides an interface to store or read data in the non-volatile
memory. The data can either by stored cyclic (i.e. every 120s) or triggered
(i.e. when opening the contacors while current is floating). The |mod_nvmh| is
an abstraction layer to the actual implementation of the non-volatile ram
(i.e. EEPROM or memory card).

Module Files
~~~~~~~~~~~~

Driver:
 - ``embedded-software\mcu-primary\src\engine\nvramhandler\nvramhandler.c``  (:ref:`nvramhandlerc`)
 - ``embedded-software\mcu-primary\src\engine\nvramhandler\nvramhandler.h``  (:ref:`nvramhandlerh`)

Driver Configuration:
 - ``embedded-software\mcu-primary\src\engine\config\nvramhandler_cfg.c``  (:ref:`nvramhandlercfgc`)
 - ``embedded-software\mcu-primary\src\engine\config\nvramhandler_cfg.h``  (:ref:`nvramhandlercfgh`)

Structure
~~~~~~~~~

The struct ``NVRAM_BLOCK_s`` contains the definition of an data block that is stored in the non-volatile ram..

.. code-block:: C

    typedef struct NVRAM_BLOCK {
        NVRAM_state_e state;                /*!< state of datahandler block          */
        uint32_t lastUpdate;                /*!< time of last nvram update           */
        NVRAM_UpdateType_e mode;            /*!< update mode (cyclic or triggered)   */
        uint32_t updateCycleTime_ms;        /*!< cycle time of algorithm             */
        uint32_t phase_ms;                  /*!< start time when executing algorithm */
        STD_RETURN_TYPE_e (*funcRD)(void);  /*!< read callback function              */
        STD_RETURN_TYPE_e (*funcWR)(void);  /*!< write callback function             */
    } NVRAM_BLOCK_s;

``state`` contains the current state of the nvramhandler:

.. code-block:: C

    typedef enum NVRAM_state {
        NVRAM_wait    = 0,
        NVRAM_write   = 1,
        NVRAM_read    = 2,
    } NVRAM_state_e;

Each block should be configured with the state ``NVRAM_wait``. ``lastUpdate``
holds the timestamp when the data block was updated (written) the last time.
``mode`` is either ``NVRAM_Cyclic`` or ``NVRAM_Triggered``.
``updateCycleTime_ms`` is the cyclic update time of the data block when
configured in cyclic mode. ``phase_ms`` is a constant timing offset to prevent
multiple write/read requests to the non-volatile storage at the same time.
``(*funcRD)(void)``/``(*funcWR)(void)`` is the callback pointer to the specific
read/write function of the data block which are dependent on the specific
implementation of the non-volatile ram.

The function ``NVRAM_dataHandlerInit`` intializes the struct
``NVRAM_BLOCK_s nvram_dataHandlerBlocks``. It sets all states to ``NVRAM_wait``
and the lastUpdate to 0.

``NVRAM_dataHandler`` loops over the different data blocks and updates the
respective datablocks if either the cycle time is elapsed or a trigger has been
received via the function ``NVRAM_setWriteRequest(NVRAM_BLOCK_ID_TYPE_e blockID)``.
If a block is configured in cyclic mode it can still be triggered. It is then
updated after receiving the trigger and afterwards again is updated every
``updateCycleTime_ms``. Furthermore the function ``NVRAM_dataHandler`` can
execute asynchronous read requests from the non-volatile storage over the
function ``NVRAM_setReadRequest(NVRAM_BLOCK_ID_TYPE_e blockID)``.

``NVRAM_BLOCK_ID_TYPE_e blockID`` is used to indicate which data block should
be accessed. This implementation structure is analog the implementation of the
database.

.. code-block:: C

    #define NVRAM_BLOCK_ID_OPERATING_HOURS         NVRAM_BLOCK_00
    #define NVRAM_BLOCK_ID_CELLTEMPERATURE         NVRAM_BLOCK_01
    #define NVRAM_BLOCK_ID_CONT_COUNTER            NVRAM_BLOCK_02

Defines are mapped to the enum and the enum corresponds to the order of the
configured non-volatile data blocks in strcut
``NVRAM_BLOCK_s nvram_dataHandlerBlocks``.
