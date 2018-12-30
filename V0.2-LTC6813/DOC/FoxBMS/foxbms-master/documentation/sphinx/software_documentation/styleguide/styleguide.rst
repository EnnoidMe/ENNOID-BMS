.. include:: ../../macros.rst



.. _software_styleguide:

====================
Software Style Guide
====================

.. highlight:: C

General
-------

All plain text files must end with a single empty line
(`POSIX, 3.206 Line <http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap03.html#tag_03_206>`_).

File Templates
--------------

All file templates are found in ``/tools/styleguide/file-templates``

Source Code
+++++++++++

In short: For foxbms we use for the embedded code
`1TBS <https://en.wikipedia.org/wiki/Indentation_style#Variant:_1TBS_(OTBS)>`_
(check by `cpplint`) and `flake8` for the python code.

.. literalinclude:: ../../../../tools/styleguide/file-templates/template.c
    :language: c
    :name: c_file_template
    :caption: Template for C files

.. literalinclude:: ../../../../tools/styleguide/file-templates/template.h
    :language: c
    :name: h_file_template
    :caption: Template for h files

.. literalinclude:: ../../../../tools/styleguide/file-templates/template.py
    :language: python
    :name: py_file_template
    :caption: Template for python scripts

Documentation
+++++++++++++

.. literalinclude:: ../../../../tools/styleguide/file-templates/template.rst
    :language: rst
    :name: rst_file_template
    :caption: Template for reStructuredText


Details
-------

C Sources
+++++++++

Doxygen
~~~~~~~

Every function has doxygen style documentation. For extern function these are
in the header file, for static function these are in the c file at the
implementation.

.. code-block:: c

    /**
     * @brief   sets the current state request of the state variable cont_state.
     *
     * @details This function is used to make a state request to the state machine,e.g, start voltage
     *          measurement, read result of voltage measurement, re-initialization.
     *          It calls CONT_CheckStateRequest() to check if the request is valid. The state request
     *          is rejected if is not valid. The result of the check is returned immediately, so that
     *          the requester can act in case it made a non-valid state request.
     *
     * @param   state request to set
     *
     * @return  CONT_OK if a state request was made, CONT_STATE_NO_REQUEST if no state request was made
     */
    CONT_RETURN_TYPE_e CONT_SetStateRequest(CONT_STATE_REQUEST_e statereq) {
        CONT_RETURN_TYPE_e retVal = CONT_STATE_NO_REQUEST;

        taskENTER_CRITICAL();
        retVal = CONT_CheckStateRequest(statereq);

        if (retVal == CONT_OK) {
                cont_state.statereq   = statereq;
        }
        taskEXIT_CRITICAL();

        return retVal;
    }

Include Guard
~~~~~~~~~~~~~

The include guard must have the following form (considerng the filename is
``template.h``)

.. code-block:: c

    #ifndef TEMPLATE_H_
    #define TEMPLATE_H_

    #endif /* TEMPLATE_H_ */

Naming conventions
~~~~~~~~~~~~~~~~~~

Variables

- The module acronym must be used as prefix.
- All variable names must be in lower case.
- No prefix or suffix is required to distinguish global from static variables.
- Underscores may be used in the name of variables.

Functions

- The module acronym must be used as prefix.
- All function names starts with its acronym in capital letters followed by a
  concatenation of words where first letter of each word should start with
  capital letters.
- No prefix or suffix is required to distinguish extern from static functions.
- Underscores may be used in the name of functions.

Macros

- The module acronym must be used as prefix.
- All macro names must be in uppercase letters.
- Underscores may be used in the name of macros.

Examples

+-----------------+-------------+-----------------+
| **Module**      | **Acronym** | **Example**     |
+-----------------+-------------+-----------------+
| spi.c/h         | SPI         | | SPI_Init()    |
|                 |             | | spi_state     |
|                 |             | | SPI_MACRO     |
+-----------------+-------------+-----------------+
| measurement.c/h | MEAS        | | MEAS_Init()   |
|                 |             | | meas_state    |
|                 |             | | MEAS_MACRO    |
+-----------------+-------------+-----------------+

Typedefs, Enums and Structs

- The module acronym must be used as prefix.
- All enum/struct names must be in uppercase letters.
- Underscores may be used in the name of enums/structs.
- Every enum/struct must be typedefed.
- Every enum/struct must have a general doxygen documentation comment in the
  form of ``/** documentation comment */``
- Every enum/struct entry must be documented by a doxygen comment in the form of
  ``/*!< documentation comment */``.

.. code-block:: c

    /** diagnosis handler return types */
    typedef enum {
        DIAG_HANDLER_RETURN_OK                  = 0,            /*!<  error not occured or occured but threshold not reached */
        DIAG_HANDLER_RETURN_ERR_OCCURED         = 2,            /*!<  error occured and enabled */
        DIAG_HANDLER_RETURN_WARNING_OCCURRED    = 3,            /*!<  warning occured (error occured but not enabled) */
        DIAG_HANDLER_RETURN_ERR_OCCURRED        = 4,            /*!<  error occured and enabled */
        DIAG_HANDLER_RETURN_WRONG_ID            = 5,            /*!<  wrong diagnosis id */
        DIAG_HANDLER_RETURN_UNKNOWN             = 6,            /*!<  unknown return type */
        DIAG_HANDLER_INVALID_TYPE               = 7,            /*!<  invalid diagnosis type, error in configuration */
        DIAG_HANDLER_INVALID_DATA               = 8,            /*!<  invalid data, dependent of the diagHandler */
        DIAG_HANDLER_RETURN_NOT_READY           = 0xFFFFFFFF,   /*!<  diagnosis handler not ready */
    } DIAG_RETURNTYPE_e;

Includes
++++++++

There are two scenarios to be considered for file includes. These are shown
below.

Scenario 1
~~~~~~~~~~

Normally a every software part has the following structure, considering a
driver called ``abc``:

- ``*\config\abc_cfg.c``
- ``*\config\abc_cfg.h``
- ``*\abc\abc.c``
- ``*\abc\abc.h``

The include strcture has then always to be like this:

.. code-block:: c
   :caption: abc_cfg.h

   /* THE DRIVER CONFIGURATION HEADER abc_cfg.h ALWAYS INCLUDES general.h FIRST */
   /* IF ADDITIONAL HEADERS ARE NEEDED FOR TYPES ETC. THESE ARE INCLUDED BELOW */
   #include "general.h"
   #include "additionally_needed_header_1.h"

.. code-block:: c
   :caption: abc_cfg.c

   /* THE DRIVER CONFIGURATION IMPLEMENTATION abc_cfg.c ALWAYS INCLUDES THE CONFIGURATION HEADER abc_cfg.h FIRST */
   /* NEVER INCLUDE general.h IN THE DRIVER CONFIGURATION IMPLEMENTATION FILE abc_cfg.c */
   /* IF ADDITIONAL HEADERS ARE NEEDED FOR TYPES ETC. THESE ARE INCLUDED BELOW */
   #include "abc_cfg.h"
   #include "additionally_needed_header_2.h"

.. code-block:: c
   :caption: abc.h

   /* THE DRIVER HEADER abc.h ALWAYS INCLUDES THE DRIVER CONFIGURATION HEADER abc_cfg.h FIRST */
   /* NEVER INCLUDE general.h IN THE DRIVER HEADER FILE abc.h */
   /* IF ADDITIONAL HEADERS ARE NEEDED FOR TYPES ETC. THESE ARE INCLUDED BELOW */
   #include "abc_cfg.h"
   #include "additionally_needed_header_3.h"

.. code-block:: c
   :caption: abc.c

   /* THE DRIVER IMPLEMENTATION abc.c ALWAYS INCLUDES THE DRIVER HEADER abc.h FIRST */
   /* NEVER INCLUDE general.h IN THE DRIVER IMPLEMENTATION FILE abc.c */
   /* IF ADDITIONAL HEADERS ARE NEEDED FOR TYPES ETC. THESE ARE INCLUDED BELOW */
   #include "abc.h"
   #include "additionally_needed_header_4.h"

Scenario 2
~~~~~~~~~~

If a software only has a driver header and a driver implementation

- ``*\abc\abc.c``
- ``*\abc\abc.h``

the include rules changed to

.. code-block:: c
   :caption: abc.h

   /* THE DRIVER HEADER abc.h ALWAYS INCLUDES general.h FIRST */
   /* IF ADDITIONAL HEADERS ARE NEEDED FOR TYPES ETC. THESE ARE INCLUDED BELOW */
   #include "general.h"
   #include "additionally_needed_header_1.h"

.. code-block:: c
   :caption: abc.c

   /* THE DRIVER IMPLEMENTATION abc.c ALWAYS INCLUDES THE DRIVER HEADER abc.h FIRST */
   /* NEVER INCLUDE general.h IN THE DRIVER IMPLEMENTATION FILE abc.c */
   /* IF ADDITIONAL HEADERS ARE NEEDED FOR TYPES ETC. THESE ARE INCLUDED BELOW */
   #include "abc.h"
   #include "additionally_needed_header_2.h"
