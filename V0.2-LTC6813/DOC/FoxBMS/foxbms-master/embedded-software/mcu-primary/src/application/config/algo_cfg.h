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
 * @file    algo_cfg.h
 * @author  foxBMS Team
 * @date    18.12.2017 (date of creation)
 * @ingroup DRIVERS_CONF
 * @prefix  ALGO
 *
 * @brief   Headers for the configuration of the algorithm module
 *
 */

#ifndef ALGO_CFG_H_
#define ALGO_CFG_H_

/*================== Includes =============================================*/
#include "general.h"

/*================== Macros and Definitions ===============================*/

#define MOVING_AVERAGE_DURATION_CURRENT_CONFIG_MS       3000
#define MOVING_AVERAGE_DURATION_POWER_CONFIG_MS         3000

#define ISA_CURRENT_CYCLE_TIME_MS                       20
#define ISA_POWER_CYCLE_TIME_MS                         20

/**
 * @ingroup CONFIG_ALGO
 * task timeslot where the ALGO main function is called. Repetition time of algorithm cycle time must be multiple of this
 * \par Type:
 * select(3)
 * \par Default:
 * 2
*/
/* #define ALGO_TICK_MS 1 */
/* #define ALGO_TICK_MS 10 */
#define ALGO_TICK_MS 100


typedef enum ALGO_STATE {
    ALGO_READY           = 0,
    ALGO_RUNNING         = 1,
    ALGO_WAIT_FOR_OTHER  = 2,
    ALGO_RDY_BUT_WAITING = 3,
    ALGO_EXECUTE_ASAP    = 4,
    ALGO_BLOCKED         = 5,
} ALGO_STATE_e;

typedef struct ALGO_TASKS {
    ALGO_STATE_e state;              /* !< current execution state */
    uint32_t cycleTime_ms;           /* !< cycle time of algorithm */
    uint32_t maxCalcDuration_ms;     /* !< maximum allowed calculation duration for task */
    uint32_t startTime;              /* !< start time when executing algorithm */
    void (*func)(uint32_t algoIdx);  /*!< callback function */
} ALGO_TASKS_s;

/*================== Constant and Variable Definitions ====================*/

/*
 * Array with pointer to the different algorithms
 */
extern ALGO_TASKS_s algo_algorithms[];

/**
 * number of executed algorithms
 */
extern const uint16_t algo_length;

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

#endif /* ALGO_CFG_H_ */
