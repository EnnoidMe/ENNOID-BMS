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
 * @file    enginetask_cfg.h
 * @author  foxBMS Team
 * @date    26.08.2015 (date of creation)
 * @ingroup ENGINE_CONF
 * @prefix  ENG
 *
 * @brief   Engine task configuration header
 *
 */

#ifndef ENGINETASK_CFG_H_
#define ENGINETASK_CFG_H_

/*================== Includes =============================================*/
#include "general.h"

#include "os.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/

/**
 * @brief   Task configuration of the 1ms engine task
 *
 * @details Predefined 1ms task for 1ms cyclic engine task
 *
 * @ingroup API_OS
 */
extern OS_Task_Definition_s eng_tskdef_cyclic_1ms;

/**
 * @brief   Task configuration of the 10ms engine task
 *
 * @details Predefined 10ms task for 10ms cyclic engine task
 *
 * @ingroup API_OS
 */
extern OS_Task_Definition_s eng_tskdef_cyclic_10ms;

/**
 * @brief   Task configuration of the 100ms engine task
 *
 * @details Predefined 100ms task for 100ms cyclic engine task
 *
 * @ingroup API_OS
 */
extern OS_Task_Definition_s eng_tskdef_cyclic_100ms;


extern OS_Task_Definition_s eng_tskdef_eventhandler;
extern OS_Task_Definition_s eng_tskdef_diagnosis;

/*================== Function Prototypes ==================================*/
/**
 * @brief   Initializes modules that were not initialized before scheduler
 *          starts
 *
 * @details This function is called after the scheduler started but before any
 *          cyclic task runs. Here modules get initialized that are not used
 *          during the startup process.
 */
extern void ENG_Init(void);

/**
 * @brief   reads non-volatile memory and initializes the sys module
 *
 * Start up after scheduler start, called from 1ms-Task before the 1ms cyclic execution
 */
extern void ENG_PostOSInit(void);

/**
 * @brief   Cyclic 1ms task for the LTC measurement
 */
extern void ENG_Cyclic_1ms(void);

/**
 * @brief   Task for system- and bmscontrol and SOX algorithms
 *
 */
extern void ENG_Cyclic_10ms(void);

/**
 * @brief   Task for ADC control, balancing control and isolation
 *          measurement
 *
 */
extern void ENG_Cyclic_100ms(void);

/**
 * @brief   OS_IdleTask, called by vApplicationIdleHook()
 */
extern void ENG_IdleTask(void);

/**
 * @brief   Engine Task for handling of events
 *
 */
extern void ENG_EventHandler(void);

/**
 * @brief   Engine Task for diagnosis
 *
 */
extern void ENG_Diagnosis(void);

/**
 * @brief   Database-Task
 * @details The task manages the data exchange with the database and must have a
 *          higher task priority than any task using the database.
 *
 */
extern void ENG_TSK_Engine(void);

/*================== Function Implementations =============================*/

#endif /* ENGINETASK_CFG_H_ */
