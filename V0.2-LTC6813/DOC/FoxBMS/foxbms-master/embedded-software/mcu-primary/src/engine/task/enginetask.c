/**
 *
 * @copyright &copy; 2010 - 2018, Fraunhofer-Gesellschaft zur Foerderung der
 *  angewandten Forschung e.V. All rights reserved.
 *
 * BSD 3-Clause License
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1.  Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * We kindly request you to use one or more of the following phrases to refer
 * to foxBMS in your hardware, software, documentation or advertising
 * materials:
 *
 * &Prime;This product uses parts of foxBMS&reg;&Prime;
 *
 * &Prime;This product includes parts of foxBMS&reg;&Prime;
 *
 * &Prime;This product is derived from foxBMS&reg;&Prime;
 *
 */

/**
 * @file    enginetask.c
 * @author  foxBMS Team
 * @date    27.08.2015 (date of creation)
 * @ingroup ENGINE
 * @prefix  ENG
 *
 * @brief   Calls of functions within engine task
 *
 */

/*================== Includes =============================================*/
#include "enginetask.h"

#include "database.h"
#include "nvramhandler.h"
#include "nvram_cfg.h"

/*================== Macros and Definitions ===============================*/


/*================== Constant and Variable Definitions ====================*/
static OS_Task_Definition_s eng_tskdef_engine  = { 0,      1,  OS_PRIORITY_REALTIME,          1024/4};

/**
 * Definition of task handle of the engine task
 */
static TaskHandle_t eng_handle_engine;

/**
 * Definition of task handle 1 millisecond task
 */
static TaskHandle_t eng_handle_tsk_1ms;

/**
 * Definition of task handle 10 milliseconds task
 */
static TaskHandle_t eng_handle_tsk_10ms;

/**
 * Definition of task handle 100 milliseconds task
 */
static TaskHandle_t eng_handle_tsk_100ms;

/**
 * Definition of task handle Diagnosis task
 */
static TaskHandle_t eng_handle_tsk_diagnosis;

/**
 * Definition of task handle EventHandler
 */
static TaskHandle_t eng_handle_tsk_eventhandler;

/*================== Function Prototypes ==================================*/


/*================== Function Implementations =============================*/

void ENG_CreateTask(void) {

    /* Database Task */
    if (xTaskCreate((TaskFunction_t)ENG_TSK_Engine,(const portCHAR *)"TSK_Engine",
            eng_tskdef_engine.Stacksize, NULL, eng_tskdef_engine.Priority,
                &eng_handle_engine) != pdPASS)  {

      while (1) {
          ;     /* TODO: do something */
      }
    }

    /* Cyclic Task 1ms */
    if (xTaskCreate((TaskFunction_t)ENG_TSK_Cyclic_1ms,(const portCHAR *)"TSK_Cyclic_1ms",
            eng_tskdef_cyclic_1ms.Stacksize, NULL, eng_tskdef_cyclic_1ms.Priority,
                &eng_handle_tsk_1ms) != pdPASS)  {

      while (1) {
          ;     /* TODO: do something */
      }
    }

    /* Cyclic Task 10ms */
    if (xTaskCreate((TaskFunction_t)ENG_TSK_Cyclic_10ms,(const portCHAR *)"TSK_Cyclic_10ms",
            eng_tskdef_cyclic_10ms.Stacksize, NULL, eng_tskdef_cyclic_10ms.Priority,
                &eng_handle_tsk_10ms) != pdPASS)  {

      while (1) {
          ;     /* TODO: do something */
      }
    }

    /* Cyclic Task 100ms */
    if (xTaskCreate((TaskFunction_t)ENG_TSK_Cyclic_100ms,(const portCHAR *)"TSK_Cyclic_100ms",
            eng_tskdef_cyclic_100ms.Stacksize, NULL, eng_tskdef_cyclic_100ms.Priority,
                &eng_handle_tsk_100ms) != pdPASS)  {

      while (1) {
          ;     /* TODO: do something */
      }
    }

    /* EventHandler Task */
    if (xTaskCreate((TaskFunction_t)ENG_TSK_EventHandler,(const portCHAR *)"TSK_EventHandler",
            eng_tskdef_eventhandler.Stacksize, NULL, eng_tskdef_eventhandler.Priority,
                &eng_handle_tsk_eventhandler) != pdPASS)  {

      while (1) {
          ;     /*  TODO: do something */
      }
    }

    /* Diagnosis Task */
    if (xTaskCreate((TaskFunction_t)ENG_TSK_Diagnosis,(const portCHAR *)"TSK_Diagnosis",
            eng_tskdef_diagnosis.Stacksize, NULL, eng_tskdef_diagnosis.Priority,
                &eng_handle_tsk_diagnosis) != pdPASS)  {

      while (1) {
          ;     /* TODO: do something */
      }
    }
}

void ENG_CreateMutex(void) {
}

void ENG_CreateEvent(void) {
}

void ENG_CreateQueues(void) {

}

void ENG_TSK_Engine(void) {

    DATA_Init();
    ENG_PostOSInit();

    os_boot = OS_SYSTEM_RUNNING;

    for (;;) {
        DATA_Task();    /* Call database manager */
        DIAG_SysMon();  /* Call Overall System Monitoring */
    }
}


void ENG_TSK_Cyclic_1ms(void) {
    while (os_boot != OS_SYSTEM_RUNNING) {
        ;
    }

    if (eng_init == FALSE) {
        ENG_Init();
        eng_init = TRUE;
    }

    OS_taskDelayUntil(&os_schedulerstarttime, eng_tskdef_cyclic_1ms.Phase);

    while (1) {
        uint32_t currentTime = OS_getOSSysTick();
        OS_TimerTrigger(&os_timer);    /* Increment system timer os_timer */
        NVM_setOperatingHours(&bkpsram_op_hours);
        ENG_Cyclic_1ms();
        OS_taskDelayUntil(&currentTime, eng_tskdef_cyclic_1ms.CycleTime);
    }
}


void ENG_TSK_Cyclic_10ms(void) {
    while (os_boot != OS_SYSTEM_RUNNING) {
        ;
    }

    OS_taskDelayUntil(&os_schedulerstarttime, eng_tskdef_cyclic_10ms.Phase);

    while (1) {
        uint32_t currentTime = OS_getOSSysTick();
        ENG_Cyclic_10ms();
        OS_taskDelayUntil(&currentTime, eng_tskdef_cyclic_10ms.CycleTime);
    }
}

void ENG_TSK_Cyclic_100ms(void) {
    while (os_boot != OS_SYSTEM_RUNNING) {
        ;
    }

    OS_taskDelayUntil(&os_schedulerstarttime, eng_tskdef_cyclic_100ms.Phase);

    while (1) {
        uint32_t currentTime = OS_getOSSysTick();
        ENG_Cyclic_100ms();
        OS_taskDelayUntil(&currentTime, eng_tskdef_cyclic_100ms.CycleTime);
    }
}


void ENG_TSK_EventHandler(void) {
    while (os_boot != OS_SYSTEM_RUNNING) {
        ;
    }

    OS_taskDelayUntil(&os_schedulerstarttime, eng_tskdef_eventhandler.Phase);

    while (1) {
        uint32_t currentTime = OS_getOSSysTick();
        ENG_EventHandler();
        OS_taskDelayUntil(&currentTime, eng_tskdef_eventhandler.CycleTime);
    }
}

void ENG_TSK_Diagnosis(void) {
    while (os_boot != OS_SYSTEM_RUNNING) {
        ;
    }

    OS_taskDelayUntil(&os_schedulerstarttime, eng_tskdef_diagnosis.Phase);

    while (1) {
        uint32_t currentTime = OS_getOSSysTick();
        ENG_Diagnosis();
        OS_taskDelayUntil(&currentTime, eng_tskdef_diagnosis.CycleTime);
    }
}
