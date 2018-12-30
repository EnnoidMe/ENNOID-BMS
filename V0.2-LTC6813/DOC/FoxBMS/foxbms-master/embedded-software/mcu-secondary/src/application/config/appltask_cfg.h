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
 * @file    appltask_cfg.h
 * @author  foxBMS Team
 * @date    26.08.2015 (date of creation)
 * @ingroup APPLICATION_CONF
 * @prefix  APPL
 *
 * @brief   Configuration for application tasks
 *
 */

#ifndef APPLTASK_CFG_H_
#define APPLTASK_CFG_H_



/*================== Includes =============================================*/
#include "general.h"

#include "os.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/

/**
 * @brief   Task configuration of the 1ms application task
 *
 * @details Predefined 1ms task for user code
 *
 * @ingroup API_OS
 */
extern OS_Task_Definition_s appl_tskdef_cyclic_1ms;

/**
 * @brief   Task configuration of the 10ms application task
 *
 * @details Predefined 10ms task for user code
 *
 * @ingroup API_OS
 */
extern OS_Task_Definition_s appl_tskdef_cyclic_10ms;

/**
 * @brief   Task configuration of the 100ms application task
 *
 * @details Predefined 100ms task for user code
 *
 * @ingroup API_OS
 */
extern OS_Task_Definition_s appl_tskdef_cyclic_100ms;

/*================== Function Prototypes ==================================*/

/**
 * @brief   user application task 1 milliseconds
 *
 * @ingroup API_OS
 */
extern void APPL_Cyclic_1ms(void);

/**
 * @brief   user application task 10 milliseconds
 *
 * @ingroup API_OS
 */
extern void APPL_Cyclic_10ms(void);

/**
 * @brief   user application task 100 milliseconds
 *
 * @ingroup API_OS
 */
extern void APPL_Cyclic_100ms(void);

/*================== Function Implementations =============================*/

#endif /* APPLTASK_CFG_H_ */
