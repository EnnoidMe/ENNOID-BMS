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
 * @file    appltask_cfg.c
 * @author  foxBMS Team
 * @date    26.08.2015 (date of creation)
 * @ingroup APPLICATION_CONF
 * @prefix  APPL
 *
 * @brief   Configuration header for application tasks
 *
 */

/*================== Includes =============================================*/
#include "appltask_cfg.h"

#include "bms.h"
#include "diag.h"
#include "led.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/
OS_Task_Definition_s appl_tskdef_cyclic_1ms    = {     0,      1,  OS_PRIORITY_NORMAL,        1024/4};
OS_Task_Definition_s appl_tskdef_cyclic_10ms   = {     4,     10,  OS_PRIORITY_BELOW_NORMAL,  2560/4};
OS_Task_Definition_s appl_tskdef_cyclic_100ms  = {    58,    100,  OS_PRIORITY_LOW,            512/4};

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/
void APPL_Cyclic_1ms(void) {

    DIAG_SysMonNotify(DIAG_SYSMON_APPL_CYCLIC_1ms, 0);        /* task is running, state = ok */

    /* User specific implementations:   */
    /*   ...                            */
    /*   ...                            */
    BMS_Trigger();
}

void APPL_Cyclic_10ms(void) {

    DIAG_SysMonNotify(DIAG_SYSMON_APPL_CYCLIC_10ms, 0);        /* task is running, state = ok */

    /* User specific implementations:   */
    /*   ...                            */
    /*   ...                            */
    LED_Ctrl();
}

void APPL_Cyclic_100ms(void) {
    DIAG_SysMonNotify(DIAG_SYSMON_APPL_CYCLIC_100ms, 0);

    /* User specific implementations:   */
    /*   ...                            */
    /*   ...                            */

}
