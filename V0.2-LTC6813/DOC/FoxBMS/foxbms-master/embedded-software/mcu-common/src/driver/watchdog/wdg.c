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
 * @file    wdg.c
 * @author  foxBMS Team
 * @date    16.03.2016 (date of creation)
 * @ingroup DRIVERS
 * @prefix  WDG
 *
 * @brief   Driver for the watchdog
 *
 * Source file implements the watchdog interfaces
 */

/*================== Includes =============================================*/
#include "wdg.h"
#include "cpu_cfg.h"

/*================== Macros and Definitions ===============================*/

/**
 * defines the watchdog refresh time
 */
#define IWDG_REFRESH_VALUE      (4094/8)     /* ~1000 ms */


/*================== Constant and Variable Definitions ====================*/
IWDG_HandleTypeDef hiwdg;

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

void WDG_Init(void)
{
    hiwdg.Instance = IWDG;
    hiwdg.Init.Prescaler = IWDG_PRESCALER_64;        /* IWDG prescaler set to 64 -> 32kHz/64 (2ms) */
    hiwdg.Init.Reload = IWDG_REFRESH_VALUE;
    __HAL_DBGMCU_FREEZE_IWDG();
    HAL_IWDG_Init(&hiwdg);                          /* initialize and start independent watchdog */
}



void WDG_IWDG_Refresh(void)
{
    HAL_IWDG_Refresh(&hiwdg);                           /* refresh independent watchdog */
}
