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
 * @file    timer.h
 * @author  foxBMS Team
 * @date    30.09.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  TIM
 *
 * @brief   Headers for the driver to enable/disable/change the peripheral timers
 *
 * Header file to enable/disable/change peripheral timers.
 */

#ifndef TIMER_H_
#define TIMER_H_

/*================== Includes =============================================*/
#include "timer_cfg.h"

/*================== Macros and Definitions ===============================*/

/**
 * timer return types
 */
typedef enum {
    DIAG_TIM_OK         = 0,    /*!< error not occured or occured but threshold not reached    */
    DIAG_TIM_ERROR      = 1,    /*!< error occured                                             */
    DIAG_TIM_OVERFLOW   = 2,    /*!< timer overflow occured                                    */
    DIAG_TIM_NO_NEW_VAL = 3,    /*!< no new value captured                                     */
} TIM_RETURNTYPE_e;


/**
 * struct to store PWM input capture data
 * struct is according to IsoGuard driver
 */
typedef struct {
    uint32_t ActiveTime;    /*!< To store ActiveTime for GetDutyCycles API.  */
    uint32_t PeriodTime;    /*!< To store PeriodTime for GetDutyCycles API. */
} TIM_DutyCycleType_s;

/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/

/**
 * @brief   Interface function to enable the pwm output and pwm input caputure timers
 */
extern void TIM_Init(void);

/**
 * @brief   Initalizes the pwm output timer (timer4)
 */
extern void TIM_PWM_OUT_Init(void);

/**
 * @brief   Initalizes the pwm input capture timer (timer9)
 */
extern void TIM_PWM_IC_Init(void);

/**
 * @brief Initializes cyclic timer interrupt
 */
extern void TIM_CyclicInterrupt_Init(void);
/**
 * @brief   Sets the pwm output timer frequency
 *
 * @param   htim: pointer to timer handle struct
 * @param   frequency: frequency in Hz
 */
extern void TIM_PWM_SetFrequency(TIM_HandleTypeDef *htim, uint32_t frequency);

/**
 * @brief   sets the timer output pwm dutycycle
 *
 * @param   htim: pointer to timer handle struct
 * @param   dutycycle: dutycycle in percentage (0-100)
 */
extern void TIM_PWM_SetDutycycle(TIM_HandleTypeDef *htim, uint8_t dutycycle);

/**
 * @brief   Interface function for start of input capture measurement
 */
extern void TIM_Start_PWM_IC_Measurement(TIM_HandleTypeDef *htim);

/**
 * @brief   Interface function for start of pwm output
 */
extern void TIM_Start_PWM_Out(TIM_HandleTypeDef *htim);

/**
 * @brief Gets duty cycle data of PWM-measurement
 * Interface function for IsoGuard-Driver
 *
 * @param   htim: handle pointer to timer
 * @param   DutyCycleData: pointer where duty cycle data is stored
 *
 * @return  E_OK if duty cycle data is valid, E_NOT_OK if duty cycle data is invalid
 */
extern TIM_RETURNTYPE_e TIM_GetDutycycle(TIM_HandleTypeDef *htim, TIM_DutyCycleType_s* DutyCycleData);

/**
 * @brief   Interrupt Handler for the timer interrupt
 */
extern void TIM_IRQHandler(void);

/*================== Function Implementations =============================*/

#endif /* TIMER_H_ */
