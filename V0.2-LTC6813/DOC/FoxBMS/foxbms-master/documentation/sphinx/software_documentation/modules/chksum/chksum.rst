.. include:: ../../../macros.rst

.. _CHECKSUM:

========
Checksum
========

.. highlight:: C

The |mod_chksum| is part of the ``foxBMS-Modules`` layer.

This section describes the checksum feature in |foxbms|. For the
toolchain/buildprocess of the checksum feature, see
:ref:`software_documentation_checksum_tool`.

The |mod_chksum| provides the capability to verify data integrity during
runtime for multiple purposes (e.g., firmware validation and verification of
sent/received data).


Module Files
~~~~~~~~~~~~

Source:
 - ``embedded-software\mcu-common\src\module\chksum\chksum.h``
 - ``embedded-software\mcu-common\src\module\chksum\chksum.c``


Detailed Description
~~~~~~~~~~~~~~~~~~~~

The |mod_chksum| offers 2 possibilities to check data integrity,
``Modulo32BitAddition`` and a hardware based, slightly modified CRC32
implementation. The following short examples demonstrate the usage of the
|mod_chksum|:

.. code-block:: c

    uint32_t chksum = CHK_crc32((uint8_t*)0x08000000, 0x1000) -> hashes 4kB of code, starting at 0x08000000 using CRC32 algorithm

    uint32_t chksum = CHK_modulo32addition((uint8_t*)0x08000000, 0x1000) -> hashes 4kB of code, starting at 0x08000000 using Modulo32BitAddition algorithm


During the startup, the ``CHK_crc32`` function is used to verify the integrity
of the flashed firmware image. After the calculation of the CRC32 checksum, it
is compared to a hardcoded value within the flashheader struct. When both
values match, the firmware is valid and execution continues. If both values do
not match, an error is reported.

The flashheader contains the validation and checksum information with memory
areas of separate software partitions, it gets generated during compilation by
an external checksum tool, written in python. Furthermore it also provides the
possibility for software versioning.


Checksum Configuration
~~~~~~~~~~~~~~~~~~~~~~
Enabling/disabling of the checksum verification at startup is set by the
following macro:

.. code-block:: C

  // deactivating the chechsum
  #define BUILD_MODULE_ENABLE_FLASHCHECKSUM           0

  // activating the chechsum
  #define BUILD_MODULE_ENABLE_FLASHCHECKSUM           1

Checksum Default Configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The default value enables checksum verification at startup.


Related Modules
~~~~~~~~~~~~~~~
The checksum tool is related on the ``Checksum Tool`` configuration (see
:ref:`software_documentation_checksum_tool`).
