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
 * @file    adc.h
 * @author  foxBMS Team
 * @date    26.08.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  ADC
 *
 * @brief   Header for the driver for the analog to digital converter
 *
 * This adc module provides support for analog/digital conversion.
 * It must be initialized during startup.
 *
 */

#ifndef ADC_H_
#define ADC_H_

/*================== Includes =============================================*/
#include "adc_cfg.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/
/**
 * @brief   initializes the different ADC devices by enabling their
 *          corresponding clocks. It is called during startup.
 *
 * @param   AdcHandle: pointer to ADC hardware handle
 */
extern void ADC_Init(ADC_HandleTypeDef *AdcHandle);

/**
 * starts the ADC conversion.
 *
 * @param AdcHandle: pointer to ADC hardware handle
 */
extern void ADC_Convert(ADC_HandleTypeDef *AdcHandle);

/**
 * @brief   determines which kind of measurements are made by the ADCs and stores result in database.
 *
 * It alternates between measurement of the voltage of the backup battery
 * and the voltage of the internal temperature sensor.
 * It starts the conversion with ADC_Convert().
 * If the conversion is completed, the result is stored in the database.
 */
extern void ADC_Ctrl(void);

/**
 * @brief   callback function to the ADC conversion.
 *
 * It is called automatically when an ADC conversion is complete.
 * Before getting the converted value, it stops the ADC with HAL_ADC_Stop_IT().
 * Then it gets the converted raw value with HAL_ADC_GetValue().
 * The raw value is then scaled.
 *
 * @param   AdcHandle: pointer to ADC hardware handle
 */
extern void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle);

/**
 * @brief get coin cell battery voltage
 */
extern float ADC_GetVBAT_mV(void);

/**
 * @brief get MCU temperature
 */
extern float ADC_GetMCUTemp_C(void);

/*================== Function Implementations =============================*/

#endif /* ADC_H_ */
