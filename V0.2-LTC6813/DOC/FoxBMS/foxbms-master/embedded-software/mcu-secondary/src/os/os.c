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
 * @file    os.c
 * @author  foxBMS Team
 * @date    27.08.2015 (date of creation)
 * @ingroup OS
 * @prefix  OS
 *
 * @brief   Implementation of the tasks used by the system
 *
 */

/*================== Includes =============================================*/
#include "os.h"
#include "enginetask.h"
#include "appltask.h"

/*================== Macros and Definitions ===============================*/


/*================== Constant and Variable Definitions ====================*/
volatile OS_BOOT_STATE_e os_boot;
volatile OS_TIMER_s os_timer;
uint8_t eng_init = FALSE;

/**
 * Scheduler "zero" time for task phase control
 */
uint32_t os_schedulerstarttime;

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

void OS_TaskInit(void) {

    /* Configuration of RTOS Queues */
    os_boot = OS_ENG_CREATE_QUEUES;
    ENG_CreateQueues();

    /* Configuration of RTOS Mutexes */
    os_boot = OS_ENG_CREATE_MUTEX;
    ENG_CreateMutex();

    /* Configuration of RTOS Events */
    os_boot = OS_ENG_CREATE_EVENT;
    ENG_CreateEvent();

    /* Configuration of RTOS Tasks */
    os_boot = OS_ENG_CREATE_TASKS;
    ENG_CreateTask();

    /* Configuration of RTOS Mutexes */
    os_boot = OS_APPL_CREATE_MUTEX;
    APPL_CreateMutex();

    /* Configuration of RTOS Events */
    os_boot = OS_APPL_CREATE_EVENT;
    APPL_CreateEvent();

    /* Configuration of RTOS Tasks */
    os_boot = OS_APPL_CREATE_TASKS;
    APPL_CreateTask();
}

void vApplicationIdleHook(void) {

    ENG_IdleTask();
}


void OS_TimerTrigger(volatile OS_TIMER_s* timer) {

    if(++timer->Timer_1ms > 9 ) {
        /* 10ms */
        timer->Timer_1ms = 0;

        if(++timer->Timer_10ms > 9) {
        /* 100ms */
            timer->Timer_10ms = 0;

            if(++timer->Timer_100ms > 9) {
            /* 1s */
                timer->Timer_100ms = 0;

                if(++timer->Timer_sec > 59) {
                /* 1min */
                    timer->Timer_sec = 0;

                    if(++timer->Timer_min > 59) {
                    /* 1h */
                        timer->Timer_min=0;

                        if(++timer->Timer_h > 23) {
                        /* 1d */
                            timer->Timer_h=0;
                            ++timer->Timer_d;
                        }
                    }
                }
            }
        }
    }
}

uint8_t OS_Check_Context(void)
{

    /* use define from port.c :   portVECTACTIVE_MASK */
    if((portNVIC_INT_CTRL_REG & 0xFFUL) == 0)  /* and mask off all bits but the VECTACTIVE bits in the ICSR register */
    {
        return 0;   /* Context of caller function is a TASK (Thread)*/
    }
    else
        return 1;   /* Context of caller function is an ISR (FreeRTOS-Interrupt or Realtime Interrupt)*/

}

void OS_TaskEnter_Critical(void)
{

    /* use define from port.c :   portVECTACTIVE_MASK */
    if((portNVIC_INT_CTRL_REG & 0xFFUL) == 0)  /* and mask off all bits but the VECTACTIVE bits in the ICSR register */
    {
        taskENTER_CRITICAL();               /* Call enter critical function within task context */
    }
    else
        taskENTER_CRITICAL_FROM_ISR();      /*  Call enter critical function within ISR context (FreeRTOS-Interrupt) */

}

void OS_TaskExit_Critical(void)
{

    /* use define from port.c :   portVECTACTIVE_MASK */
    if((portNVIC_INT_CTRL_REG & 0xFFUL) == 0)  /* and mask off all bits but the VECTACTIVE bits in the ICSR register */
    {
        taskEXIT_CRITICAL();                /* Call exit critical function within task context */
    }
    else
        taskEXIT_CRITICAL_FROM_ISR(0);      /*  Call exit critical function within ISR context (FreeRTOS-Interrupt) */

}


uint32_t OS_getOSSysTick(void) {
    uint32_t regIPSR = 0xFFFFFFFF;

    /* read out IPSR register */
    __asm volatile( "mrs %0, ipsr" : "=r"( regIPSR ) );

    /* regIPSR == 0 if controller is in thread mode */
    if (regIPSR != 0) {
      return xTaskGetTickCountFromISR();
    }
    else {
      return xTaskGetTickCount();
    }
}


void OS_taskDelay(uint32_t delay_ms)
{
#if INCLUDE_vTaskDelay
    TickType_t ticks = delay_ms / portTICK_PERIOD_MS;

    vTaskDelay(ticks ? ticks : 1);          /* Minimum delay = 1 tick */
#else
#error "Can't use OS_taskDelay."
#endif
}


void OS_taskDelayUntil (uint32_t *PreviousWakeTime, uint32_t millisec)
{
#if INCLUDE_vTaskDelayUntil
  TickType_t ticks = (millisec / portTICK_PERIOD_MS);
  vTaskDelayUntil((TickType_t *) PreviousWakeTime, ticks ? ticks : 1);

#else
#error "Can't use OS_taskDelayUntil."
#endif
}

void OS_SysTickHandler(void) {

#if (INCLUDE_xTaskGetSchedulerState  == 1 )
    /* Only increment operating systick timer if scheduler started */
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        xPortSysTickHandler();
    }
#else
    xPortSysTickHandler();
#endif  /* INCLUDE_xTaskGetSchedulerState */
}
