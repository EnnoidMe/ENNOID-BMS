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
 * @file    vic.c
 * @author  foxBMS Team
 * @date    10.09.2015 (date of creation)
 * @ingroup GENERAL
 * @prefix  VIC
 *
 * @brief   Functions for setting and enabling interrupts
 *
 * The MCU has 16 interrupt priority levels:
 * Priority number 0 means highest (strongest) priority
 * Priority number 15 means lowest (weakest) priority
 *
 * FreeRTOS priority model:
 * The systick interrupt has the lowest priority
 * FreeRTOSConfig.h defines the max syscall priority in configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
 * Interrupts which uses FreeRTOS syscalls have to be assigned to a priority number >= max syscall priority
 * (priority is not stronger than max syscall priority), however they can only use syscalls like xxx_fromISR().
 * Real-Time-Interrupts should be configured to a priority number < max syscall priority, thus they could not be blocked by FreeRTOS
 * because the blocking priority level is configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY.
 * Real-Time-Interrupts must not use FreeRTOS-API like xxx_fromISR().
 *
 * All task priorities are weaker than the interrupt priorities, the strongest task priority will be interrupted by the weakest
 * interrupt priority.
 *
 * Example: max syscall priority = 5
 *
 *     Priority number        description
 * |    Task priorities       Always weaker than interrupts
 * |    15                    Systick interrupt, weakest interrupt
 * |    14..5                 Interrupts which use FreeRTOS syscalls
 * |    4..1                  Real-Time Interrupts
 * V    0                     Real-Time Interrupt, strongest interrupt
 * down = stronger
 *
 *
 */

/*================== Includes =============================================*/
#include "vic.h"

/*================== Macros and Definitions ===============================*/

/**
  * @brief  Sets the specified EXTI line.
  * @param  __EXTI_LINE__: specifies the EXTI line flag to check.
  *         This parameter can be GPIO_PIN_x where x can be(0..15)
  * @retval void
  */
#define __NVIC_EXTI_SELECT_LINE(__EXTI_LINE__)          (EXTI->IMR | (__EXTI_LINE__))

/**
  * @brief  Sets the specified EXTI line to trigger on rising edges.
  * @param  __EXTI_LINE__: specifies the EXTI line flag to check.
  *         This parameter can be GPIO_PIN_x where x can be(0..15)
  * @retval void
  */
#define __NVIC_EXTI_SELECT_RISING_EDGE(__EXTI_LINE__)   (EXTI->RTSR | (__EXTI_LINE__))


/**
  * @brief  Sets the specified EXTI line to trigger on falling edges.
  * @param  __EXTI_LINE__: specifies the EXTI line flag to check.
  *         This parameter can be GPIO_PIN_x where x can be(0..15)
  * @retval void
  */
#define __NVIC_EXTI_SELECT_FALLING_EDGE(__EXTI_LINE__)   (EXTI->RTSR | (__EXTI_LINE__))

/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/
static uint32_t VIC_InitEXTI(void);

/*================== Function Implementations =============================*/

uint32_t VIC_PreOsInterruptInit(void) {
    uint32_t retErr = 0;
    return retErr;
}


uint32_t VIC_PostOsInterruptInit(void) {

    uint32_t retErr = 0;

    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t indexnumber = 0;
    uint8_t invalidpriority = 0;

    /* Check if every interrupt is only defined once */
    for (i = 0; i < vic_cfg_length - 1; i++) {
        for (j = i + 1; j < vic_cfg_length; j++) {
            if (vic_interrupts[i].IRQ == vic_interrupts[j].IRQ) {
                indexnumber = i;
                break;
            }
        }
    }

    /* Check interrupts for valid priority */
    for (i = 0; i < vic_cfg_length; i++) {
        if (vic_interrupts[i].Prio > 15) {
            invalidpriority = vic_interrupts[i].Prio;
            indexnumber = i;
            break;
        }
    }

    /* Wait here if error occurred */
    if (indexnumber != 0 || invalidpriority != 0) {
        /* indexnumber holds entry in vic_interrupts where multiple initializations occur */
        /* or if invalidpriority is also unequal 0, it holds the index where the invalid priority, */
        /* invalidpriority, is defined. */

        retErr |= STD_ERR_BIT_1;
    }


    /* Set SysTick_IRQn interrupt priority */
    HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);

    /* Set Interrupt priority */
    for (i = 0; i < vic_cfg_length; i++) {
            HAL_NVIC_SetPriority(vic_interrupts[i].IRQ, vic_interrupts[i].Prio, 0);
    }

    /* Initialize external interrupts */
    uint32_t tmp = 0;
    tmp = VIC_InitEXTI();

    /* Error occurred initializing external interrupts */
    if (tmp != 0) {
        retErr |= STD_ERR_BIT_2;
    }

    /* Enable Interrupts */
    for (i = 0; i < vic_cfg_length; i++) {
        if (vic_interrupts[i].state == VIC_IRQ_ENABLE) {
            HAL_NVIC_EnableIRQ(vic_interrupts[i].IRQ);
        }
    }
    return retErr;
}


void VIC_EnableInterrupts(IRQn_Type interrupt) {
    for (uint8_t i = 0; i < vic_cfg_length; i++) {
        if (vic_interrupts[i].IRQ == interrupt && vic_interrupts[i].irqlock == VIC_IRQ_LOCK_DISABLE) {
            vic_interrupts[i].state = VIC_IRQ_ENABLE;
            HAL_NVIC_EnableIRQ(interrupt);
        }
    }
}


void VIC_DisableInterrupt(IRQn_Type interrupt) {
    for (uint8_t i = 0; i < vic_cfg_length; i++) {
        if (vic_interrupts[i].IRQ == interrupt && vic_interrupts[i].irqlock == VIC_IRQ_LOCK_DISABLE) {
            vic_interrupts[i].state = VIC_IRQ_DISABLE;
            HAL_NVIC_DisableIRQ(interrupt);
        }
    }
}

static uint32_t VIC_InitEXTI(void) {
    uint32_t retErr = 0;
/*     __HAL_RCC_SYSCFG_CLK_ENABLE();

    SYSCFG->EXTICR[3] |= 1;
    EXTI->IMR = __NVIC_EXTI_SELECT_LINE(GPIO_PIN_12);
    EXTI->RTSR = __NVIC_EXTI_SELECT_RISING_EDGE(GPIO_PIN_12);
    EXTI->FTSR = __NVIC_EXTI_SELECT_FALLING_EDGE(GPIO_PIN_12);
*/
    return retErr;
}
