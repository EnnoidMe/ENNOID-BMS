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
 * @file    algo.c
 * @author  foxBMS Team
 * @date    18.12.2017 (date of creation)
 * @ingroup DRIVERS
 * @prefix  ALGO
 *
 * @brief   Main module to handle the execution of algorithms
 *
 *
 */


/*================== Includes =============================================*/
#include "algo.h"

#include "os.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/
void ALGO_Init(void) {

    /* Currently nothing to initialize */

    return;
}


void ALGO_MainFunction(void) {

    static uint32_t counter_ticks = 0;

    for (uint16_t i = 0; i < algo_length; i++) {

        if (counter_ticks % algo_algorithms[i].cycleTime_ms == 0) {
            /* Cycle time elapsed -> call function */
            if (algo_algorithms[i].state == ALGO_READY) {

                /* Set state to running -> reset to READY before leaving algo function */
                algo_algorithms[i].state = ALGO_RUNNING;
                algo_algorithms[i].startTime = OS_getOSSysTick();
                algo_algorithms[i].func(i);
            } else if (algo_algorithms[i].state == ALGO_WAIT_FOR_OTHER) {
                algo_algorithms[i].state = ALGO_RDY_BUT_WAITING;
            } else {
                /* do nothing */
                ;
            }
        } else if (algo_algorithms[i].state == ALGO_EXECUTE_ASAP) {
            /* Waited for other algo to finish -> can now be executed */
            /* Set state to running -> reset to READY before leaving algo function */
            algo_algorithms[i].state = ALGO_RUNNING;
            algo_algorithms[i].startTime = OS_getOSSysTick();
            algo_algorithms[i].func(i);
        }
    }

    counter_ticks += ALGO_TICK_MS;
}


void ALGO_MonitorExecutionTime(void) {

    uint32_t timestamp = OS_getOSSysTick();

    for (uint16_t i = 0; i < algo_length; i++) {

        if ((algo_algorithms[i].startTime != 0) && algo_algorithms[i].state == ALGO_RUNNING &&
                ((algo_algorithms[i].startTime + algo_algorithms[i].maxCalcDuration_ms) < timestamp)) {

            /* Block task from further execution because of runtime violation, but task will finish its execution */
            algo_algorithms[i].state = ALGO_BLOCKED;

            /* TODO: Add diag call to notify error in algorithm module */
        }
    }
}
