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
 * @file    vic_cfg.c
 * @author  foxBMS Team
 * @date    10.09.2015 (date of creation)
 * @ingroup GENERAL,GENERAL_CONF
 * @prefix  VIC
 *
 * @brief   Configuration of interrupts
 *
 */

/*================== Includes =============================================*/
#include "vic_cfg.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/

/**
 IntPrio |
 ----------------------------------------------
 0      |  -
 1      | RTC Wakeup, Window Watchdog Prewarn
 2      | DMA
 3      | SPI
 4      |  -
 ---------------------------------------------
 5      | ADC
 6      | ADC
 7      | CAN
 8      | UART
 9-14   |  -
 15     | OS Scheduler
 ----------------------------------------------
 Tasks  |

 A lower number equals a higher priority. Priority needs to be between 0 and 15

 If irqlock is set to NVIC_IRQ_LOCK_ENABLE, the interrupt is locked and will
 always have the state defined in nvic_interrupts[]
 */
VIC_InitStruct_s vic_interrupts[] = {

        { USART2_IRQn, 7, VIC_IRQ_LOCK_ENABLE, VIC_IRQ_ENABLE },
        { USART3_IRQn, 7, VIC_IRQ_LOCK_ENABLE, VIC_IRQ_ENABLE },

        { DMA2_Stream2_IRQn, 2, VIC_IRQ_LOCK_ENABLE, VIC_IRQ_ENABLE },
        { DMA2_Stream3_IRQn, 2, VIC_IRQ_LOCK_ENABLE, VIC_IRQ_ENABLE },

        { ADC_IRQn, 8, VIC_IRQ_LOCK_DISABLE, VIC_IRQ_ENABLE },

        { SPI6_IRQn, 3, VIC_IRQ_LOCK_ENABLE, VIC_IRQ_ENABLE },

};

const uint8_t vic_cfg_length = sizeof(vic_interrupts) / sizeof(vic_interrupts[0]);

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/
