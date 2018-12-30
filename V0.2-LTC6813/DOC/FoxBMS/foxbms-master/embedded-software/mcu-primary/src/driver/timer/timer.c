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
 * @file    timer.c
 * @author  foxBMS Team
 * @date    30.09.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  TIM
 *
 * @brief   Driver to enable/disable/change the peripheral timers
 *
 * Source file to enable/disable/change peripheral timers.
 */


/*================== Includes =============================================*/
#include "timer.h"

/*================== Macros and Definitions ===============================*/


/*================== Constant and Variable Definitions ====================*/


/*================== Function Prototypes ==================================*/


/*================== Function Implementations =============================*/
void TIM_Init(void) {
    TIM_PWM_IC_Init();
    TIM_PWM_OUT_Init();
}


void TIM_PWM_OUT_Init(void) {
    TIM_OC_InitTypeDef sConfigOC;
    uint16_t prescaler = 0;
    uint16_t frequency = 0;
    float dutycycle = 0.0;
    float period = 0.0;

    /* set default values */
    dutycycle = TIM4_PWM_OUT_DEFAULT_DUTYCYCLE;  /* dutycycle */
    frequency = TIM4_PWM_OUT_DEFAULT_FREQUENCY;  /* frequency */

    /* calculate timer peripheral clock */
    uint32_t timPeriphClock = 2 * HAL_RCC_GetPCLK1Freq();  /* timer peripheral clock frequency is 2 * APB1 clock frequency */
    timPeriphClock = timPeriphClock / 1000000;    /* converting Hz to MHz */

    /* calculate prescaler */
    /* calculating value, so that the timer clock equals TIM4_CL0CK_FREQUENCY */
    /* prescaler = APB1 timer clock / TIM4_CLOCK_FREQUENCY */
    /* -> one counter value = TIM4_CLOCK_TICK_DURATION_IN_S */
    prescaler =  (uint16_t)((timPeriphClock / TIM4_CLOCK_FREQUENCY) + 0.5);

    /* set prescaler */
    htim4.Init.Prescaler = prescaler - 1;

    /* calculate period */
    period = 1.0 / frequency;   /* frequency equal to pwm output frequency */
    period = period / TIM4_CLOCK_TICK_DURATION_IN_S;

    /* set period */
    htim4.Init.Period = (uint32_t)period - 1;

    /* calculate dutycycle */
    dutycycle = dutycycle / 100.0;  /* convert from percent to numerical value */
    dutycycle = dutycycle * period;

    /* enable timer clock */
    __TIM4_CLK_ENABLE();

    /* Init timer handle */
    HAL_TIM_PWM_Init(&htim4);

    sConfigOC.OCMode = TIM_OCMODE_PWM2;
    sConfigOC.Pulse = dutycycle;        /* Set duty-cycle */
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = 0;    /* only valid for TIM1 and TIM8; defined to prevent assert error */
    sConfigOC.OCIdleState = 0;    /* only valid for TIM1 and TIM8; defined to prevent assert error */
    sConfigOC.OCNIdleState = 0;   /* only valid for TIM1 and TIM8; defined to prevent assert error */
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    /* init timer channel*/
    HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2);
}



void TIM_PWM_IC_Init(void) {
    TIM_IC_InitTypeDef sConfigIC_CH3;
    TIM_IC_InitTypeDef sConfigIC_CH4;

    uint16_t prescaler = 0;

    /* calculate timer peripheral clock */
    uint32_t timPeriphClock = 2 * HAL_RCC_GetPCLK2Freq();  /* timer peripheral clock frequency is 2 * APB2 clock frequency */
    timPeriphClock = timPeriphClock / 1000000;    /* converting Hz to MHz */

    /* calculate prescaler */
    /* calculating value, so that the timer clock frequency equals 0.2 MHz */
    /* prescaler = APB2 timer clock / 0.2MHz */
    /* -> one counter value = 5us */
    prescaler =  (uint16_t)((timPeriphClock / 0.2) + 0.5);

    /* set prescaler */
    htim9.Init.Prescaler = prescaler - 1;

    __TIM9_CLK_ENABLE();

    HAL_TIM_IC_Init(&htim9);

    sConfigIC_CH3.ICFilter = 0x0;
    sConfigIC_CH3.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
    sConfigIC_CH3.ICSelection = TIM_ICSELECTION_INDIRECTTI;
    sConfigIC_CH3.ICPrescaler = 0x0;

    HAL_TIM_IC_ConfigChannel(&htim9, &sConfigIC_CH3, TIM_CHANNEL_1);

    sConfigIC_CH4.ICFilter = 0x0;
    sConfigIC_CH4.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    sConfigIC_CH4.ICSelection = TIM_ICSELECTION_DIRECTTI;
    sConfigIC_CH4.ICPrescaler = 0x0;

    HAL_TIM_IC_ConfigChannel(&htim9, &sConfigIC_CH4, TIM_CHANNEL_2);

    TIM_SlaveConfigTypeDef Slave_Config;

    Slave_Config.InputTrigger = TIM_TS_TI2FP2;
    Slave_Config.SlaveMode = TIM_SLAVEMODE_RESET;
    Slave_Config.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
    Slave_Config.TriggerFilter = 0;
    Slave_Config.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;
    HAL_TIM_SlaveConfigSynchronization(&htim9, &Slave_Config);

    /* Enable UIF flag only on overflow */
    __HAL_TIM_URS_ENABLE(&htim9);
    HAL_TIM_Base_Start_IT(&htim9);
}


void TIM_PWM_SetFrequency(TIM_HandleTypeDef *htim, uint32_t frequency) {
    float period = 0.0;
    float dutycycle = 0;

    if (frequency < 100) {
        frequency = 100;
    }
    if (frequency > 125000) {
        frequency = 125000;
    }

    if (htim != NULL) {
        /* timer clock is TIM4_CLOCK_FREQUENCY -> one tick equals TIM4_CLOCK_TICK_DURATION_IN_S */

        period = 1.0 / frequency;
        period = period / TIM4_CLOCK_TICK_DURATION_IN_S;

        /* Get set dutycycle and period value */
        uint32_t cycleValue = __HAL_TIM_GetCompare(htim, TIM_CHANNEL_1);
        uint32_t periodValue = __HAL_TIM_GetAutoreload(htim) + 1;

        /* calculate set dutycycle */
        dutycycle = (float)cycleValue / periodValue;

        /* calculate new dutycycle value */
        dutycycle = dutycycle * period;

        /* set new period */
        __HAL_TIM_SetAutoreload(htim, period - 1);

        /* set new dutycycle */
        __HAL_TIM_SetCompare(htim, TIM_CHANNEL_1, (uint32_t)dutycycle);
        __HAL_TIM_SetCompare(htim, TIM_CHANNEL_2, (uint32_t)dutycycle);
    }
}


void TIM_PWM_SetDutycycle(TIM_HandleTypeDef *htim, uint8_t dutycycle) {
    if (dutycycle > 100) {
        dutycycle = 100;
    }
    if (htim != NULL) {
        uint32_t period = __HAL_TIM_GetAutoreload(htim) + 1;

        /* calculate set dutycycle */
        float cycleValue = dutycycle / 100.0;

        /* calculate new dutycycle value */
        cycleValue = cycleValue * period;

        /* set new dutycycle value */
        __HAL_TIM_SetCompare(htim, TIM_CHANNEL_1, (uint32_t)cycleValue);
        __HAL_TIM_SetCompare(htim, TIM_CHANNEL_2, (uint32_t)cycleValue);
    }
}


void TIM_Start_PWM_IC_Measurement(TIM_HandleTypeDef *htim) {

    if (htim->Instance == TIM9) {

        /* Start input capture */
        HAL_TIM_IC_Start(htim, TIM_CHANNEL_1);    /* Timer-Enable Channel 1 */
        HAL_TIM_IC_Start(htim, TIM_CHANNEL_2);    /* Timer-Enable Channel 2 */
    }
}


void TIM_Start_PWM_Out(TIM_HandleTypeDef *htim) {


    if (htim->Instance == TIM4) {

        /* Start PWM output */
        HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(htim, TIM_CHANNEL_2);
    }
}


TIM_RETURNTYPE_e TIM_GetDutycycle(TIM_HandleTypeDef *htim, TIM_DutyCycleType_s* DutyCycleData) {
    TIM_RETURNTYPE_e retVal = DIAG_TIM_NO_NEW_VAL;
      if (__HAL_TIM_GET_FLAG(htim, TIM_FLAG_UPDATE) != RESET) {
        /* overflow occurred */

        retVal = DIAG_TIM_OVERFLOW;
        __HAL_TIM_CLEAR_FLAG(htim, TIM_FLAG_UPDATE);
    }

    /* flag is reset, when register values are read */
    if (__HAL_TIM_GET_FLAG(htim, TIM_FLAG_CC2) != RESET && retVal == DIAG_TIM_NO_NEW_VAL) {
        /* only read if new values are captured and no overflow is detected */

        DutyCycleData->ActiveTime = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); /*  Active time */
        DutyCycleData->PeriodTime = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2); /*  Period time */
        retVal = DIAG_TIM_OK;
    } else {
        /* no new values captured or overflow detected */

        DutyCycleData->ActiveTime = 0;
        DutyCycleData->PeriodTime = 0;
    }

    return retVal;
}
