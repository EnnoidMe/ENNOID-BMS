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
 * @file    os.h
 * @author  foxBMS Team
 * @date    27.08.2015 (date of creation)
 * @ingroup OS
 * @prefix  OS
 *
 * @brief   Implementation of the tasks used by the system, headers
 *
 */

#ifndef OS_H_
#define OS_H_

/*================== Includes =============================================*/
#include "general.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "event_groups.h"

/*================== Macros and Definitions ===============================*/
/**
 *Number of mutexes for the engine
 */
#define OS_ENG_NUM_OF_MUTEXES   0
/**
 *Number of events for the ENINGE
 */
#define OS_ENG_NUM_OF_EVENTS    0

/**
 *Number of mutexes for the application
 */
#define OS_APPL_NUM_OF_MUTEXES  0
/**
 *Number of events for the application
 */
#define OS_APPL_NUM_OF_EVENTS   0

/**
 * typedef for thread priority
 */
typedef enum  {
  OS_PRIORITY_IDLE           = 0,    /*!< priority: idle (lowest)        */
  OS_PRIORITY_LOW            = 1,    /*!< priority: low                  */
  OS_PRIORITY_BELOW_NORMAL   = 2,    /*!< priority: below normal         */
  OS_PRIORITY_NORMAL         = 3,     /*!< priority: normal (default)     */
  OS_PRIORITY_ABOVE_NORMAL   = 4,    /*!< priority: above normal         */
  OS_PRIORITY_HIGH           = 5,    /*!< priority: high                 */
  OS_PRIORITY_ABOVE_HIGH     = 6,    /*!< priority: above high           */
  OS_PRIORITY_VERY_HIGH      = 7,    /*!< priority: very high          */
  OS_PRIORITY_BELOW_REALTIME = 8,    /*!< priority: below realtime     */
  OS_PRIORITY_REALTIME       = 9,    /*!< priority: realtime (highest) */
} OS_PRIORITY_e;

/**
 * enum of OS boot states
 */
typedef enum {
    OS_OFF                            = 0,    /*!< */
    OS_ENG_CREATE_QUEUES              = 1,    /*!< */
    OS_ENG_CREATE_MUTEX               = 2,    /*!< */
    OS_ENG_CREATE_EVENT               = 3,    /*!< */
    OS_ENG_CREATE_TASKS               = 4,    /*!< */
    OS_APPL_CREATE_MUTEX              = 5,    /*!< */
    OS_APPL_CREATE_EVENT              = 6,    /*!< */
    OS_APPL_CREATE_TASKS              = 7,    /*!< */
    OS_INIT_PREOS                     = 8,    /*!< */
    OS_INIT_OSRESOURCES               = 9,    /*!< */
    OS_INIT_OSSTARTKERNEL             = 10,    /*!< */
    OS_RUNNING                        = 11,   /*!< */
    OS_EEPR_INIT                      = 12,   /*!< */
    OS_BMS_INIT                   = 13,   /*!< */
    OS_SYSTEM_RUNNING                 = 14,   /*!< system is running  */
    OS_SAFETY_FEATURE_ENABLED         = 15,
    OS_SAFETY_FEATURE_DISABLED        = 16,
    OS_INIT_OS_FATALERROR_SCHEDULE    = 0x80, /*!< error in scheduler */
} OS_BOOT_STATE_e;


/**
 * OS-Timer
 */
typedef struct {
    uint8_t  Timer_1ms;   /*!< milliseconds       */
    uint8_t  Timer_10ms;  /*!< 10*milliseconds    */
    uint8_t  Timer_100ms; /*!< 100*milliseconds   */
    uint8_t  Timer_sec;   /*!< seconds            */
    uint8_t  Timer_min;   /*!< minutes            */
    uint8_t  Timer_h;     /*!< hours              */
    uint16_t Timer_d;     /*!< days               */
} OS_TIMER_s;

/**
 * struct for FreeRTOS task definition
 */
typedef struct {
    uint32_t Phase;          /*!< (ms)                                                                   */
    uint32_t CycleTime;      /*!< (ms)                                                                   */
    UBaseType_t Priority;    /*!<                                                                        */
    uint16_t Stacksize;      /*!<  Defines the size, in words, of the stack allocated to the idle task.  */
} OS_Task_Definition_s;

/*================== Constant and Variable Definitions ====================*/
extern volatile OS_BOOT_STATE_e os_boot;
extern volatile OS_BOOT_STATE_e os_safety_state;
extern uint32_t os_schedulerstarttime;
extern volatile  OS_TIMER_s os_timer;

extern SemaphoreHandle_t ENG_Mutexes[];
extern EventGroupHandle_t ENG_Events[];

extern SemaphoreHandle_t APPL_Mutexes[];
extern EventGroupHandle_t APPL_Events[];

extern uint8_t eng_init;

/*================== Function Prototypes ==================================*/

/**
 * @brief   Initialization the RTOS interface
 *
 * The initialization functions inits mutexes, eventgroups and tasks.
 */
extern void OS_TaskInit();


/**
 * @brief   Hook function for the idle task
 */
extern void vApplicationIdleHook(void);


/**
 * @brief  auxiliary function to distinguish OS Task from an ISR
 *
 * checks the function context: task (thread) mode or interrupt (handler) mode
 *
 * @return  0 when OS-Task, 0x1 when ISR
 */
extern uint8_t OS_Check_Context(void);


/**
 * @brief  Enter Critical interface function for use in FreeRTOS-Tasks and FreeRTOS-ISR
 *
 * checks the function context (task/thread mode or interrupt (handler) mode) and calls the corresponding
 * enter-critical function
 */
extern void OS_TaskEnter_Critical(void);


/**
 * @brief  Exit Critical interface function for use in FreeRTOS-Tasks and FreeRTOS-ISR
 *
 * checks the function context (task/thread mode or interrupt (handler) mode) and calls the corresponding
 * exit-critical function
 */
extern void OS_TaskExit_Critical(void);

/**
 * @brief   increments the system timer os_timer
 *
 * The os_timer is a runtime-counter, counting the time since the last reset.
 */
extern void OS_TimerTrigger(volatile OS_TIMER_s* timer);

/**
 * @brief   returns OS based system tick value.
 *
 * @return  time stamp in ms, based on the OS time.
 */
extern uint32_t OS_getOSSysTick(void);

/**
 * @brief   Delay in millisecond
 * @param   millisec      time delay value
 */
extern void OS_taskDelay(uint32_t delay_ms);

/**
* @brief  Delay a task until a specified time
* @param   PreviousWakeTime   Pointer to a variable that holds the time at which the
*          task was last unblocked. PreviousWakeTime must be initialized with the current time
*          prior to its first use (PreviousWakeTime = OS_osSysTick() )
* @param   millisec    time delay value
*/
extern void OS_taskDelayUntil(uint32_t *PreviousWakeTime, uint32_t delay);


/**
 * @brief  Handles the tick increment of operating systick timer
 */
extern void OS_SysTickHandler(void);

/**
 * FreeRTOS operating systick timer increment function
 */
extern void xPortSysTickHandler(void);

/*================== Function Implementations =============================*/

#endif /* OS_H_ */
