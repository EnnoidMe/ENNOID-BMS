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
 * @file    vic_cfg.h
 * @author  foxBMS Team
 * @date    10.09.2015 (date of creation)
 * @ingroup GENERAL,GENERAL_CONF
 * @prefix  VIC
 *
 * @brief   Header for configuration of interrupts
 *
 */

#ifndef VIC_CFG_H_
#define VIC_CFG_H_


/*================== Includes =============================================*/
#include "general.h"

#include "cpu_cfg.h"

/*================== Macros and Definitions ===============================*/

typedef enum {
    VIC_IRQ_ENABLE = 0,
    VIC_IRQ_DISABLE = 1,
} VIC_IRQ_STATE_e;

typedef enum {
    VIC_IRQ_LOCK_DISABLE = 0,
    VIC_IRQ_LOCK_ENABLE = 1,
}VIC_IRQ_LOCK_e;

typedef struct {
    IRQn_Type IRQ;
    uint8_t Prio;
    VIC_IRQ_LOCK_e irqlock;
    VIC_IRQ_STATE_e state;
} VIC_InitStruct_s;


/*================== Constant and Variable Definitions ====================*/
extern VIC_InitStruct_s vic_interrupts[];
extern const uint8_t vic_cfg_length;

/*================== Function Prototypes ==================================*/


/*================== Function Implementations =============================*/


#endif /* VIC_CFG_H_ */
