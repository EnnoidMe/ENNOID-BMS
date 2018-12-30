.. include:: ../../../macros.rst

.. _ALGO:

=========
Algorithm
=========

.. highlight:: C

The |mod_algo| module is part of the ``Application`` layer.


The |mod_algo| provides an environment for algorithms running on the
|BMS-Master|. It handles the execution of algorithms and monitors execution
times of the individual algorithms. Currently the current and power moving
averages for 1s, 5s, 10s, 30s and 60 seconds is calculated.


Module Files
~~~~~~~~~~~~

Driver:
 - ``embedded-software\mcu-primary\src\application\algo\algo.c`` (:ref:`algoc`)
 - ``embedded-software\mcu-primary\src\application\algo\algo.h`` (:ref:`algoh`)

Driver Configuration:
 - ``embedded-software\mcu-primary\src\application\config\algo_cfg.c`` (:ref:`algocfgc`)
 - ``embedded-software\mcu-primary\src\application\config\algo_cfg.h`` (:ref:`algocfgc`)


Structure
~~~~~~~~~

The struct ``ALGO_TASKS_s`` contains the definition of an algorithm.

.. code-block:: C

    typedef struct ALGO_TASKS {
        ALGO_STATE_e state;              /* !< current execution state */
        uint32_t cycleTime_ms;           /*!< cycle time of algorithm */
        uint32_t maxCalcDuration_ms;     /* !< maximum allowed calculation duration for task */
        uint32_t startTime;              /* !< start time when executing algorithm */
        void (*func)(uint32_t algoIdx);  /*!< callback function */
    } ALGO_TASKS_s;

``state`` contains the current state of the algorithm:

.. code-block:: C

    typedef enum ALGO_STATE {
        ALGO_READY           = 0,
        ALGO_RUNNING         = 1,
        ALGO_WAIT_FOR_OTHER  = 2,
        ALGO_RDY_BUT_WAITING = 3,
        ALGO_EXECUTE_ASAP    = 4,
        ALGO_BLOCKED         = 5,
    } ALGO_STATE_e;

``cycleTime_ms`` defines the periodic execution time of the algorithm.
``maxCalcDuration_ms`` specifies the maximum execution time of the algorithm.
If an algorithm takes longer to execute than specified, it is set to the
``ALGO_BLOCKED`` state and will not be executed again. An error will be set in
the |mod_diag|. ``startTime`` is set when the execution of an algorithm starts.
``void (*func)(uint32_t algoIdx)`` is the function pointer to the implementation
of the algorithm.


``ALGO_MainFunction`` loops over the different algorithms and executes them one
after another if the cycleTime_ms is elapsed. Before calling the algorithm, the
algorithm state is set to ``ALGO_RUNNING`` and the start time for the execution
time monitoring is set:

.. code-block:: C

    /* Set state to running -> reset to READY before leaving algo function */
    algo_algorithms[i].state = ALGO_RUNNING;
    algo_algorithms[i].startTime = OS_getOSSysTick();
    algo_algorithms[i].func(i);

.. note::

    After finishing the execution of the algorithm and before leaving the function,
    the algorithm state needs to be set back to ``ALGO_READY``. Otherwise it will not
    be executed again.

.. code-block:: C

    // Only set task to ready state if it isn't blocked by the monitoring unit because of a runtime violation
    if (algo_algorithms[algoIdx].state != ALGO_BLOCKED) {
        algo_algorithms[algoIdx].state = ALGO_READY;
    }
