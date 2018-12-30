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
 * @file    adc.c
 * @author  foxBMS Team
 * @date    19.11.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  ADC
 *
 * @brief   Driver for the analog to digital converter
 *
 * This adc module provides support for analog/digital conversion.
 * It must be initialized during startup.
 */




/*================== Includes =============================================*/
#include "adc.h"

/*================== Macros and Definitions ===============================*/
#define ADC_CONVERT             0
#define ADC_WAITFORCONVERSION   1
#define ADC_STOREDATA           2

/* Macros for converting voltage -> temperature */
#define ADC_VBAT_VOLTAGE_DIVIDER    4.0
#define ADC_FULL_RANGE              4095.0        /* 12bit adc converter --> range [0, ..., 4095] */
#define ADC_VREF_EXT                2.5
#define ADC_V25                     0.76
#define ADC_AVG_SLOPE               2.5

/*================== Constant and Variable Definitions ====================*/
static ADC_ChannelConfTypeDef adc_cfg = {
    .Channel = ADC_CHANNEL_VBAT,
    .Rank = 1,
    .SamplingTime = ADC_SAMPLETIME_3CYCLES,
    .Offset = 0,
};

static uint8_t adc_conversion_state = ADC_CONVERT;
static float adc_vbat_mV = 0;
static float adc_mcuTemp = 0;

/*================== Function Prototypes ==================================*/


/*================== Function Implementations =============================*/

void ADC_Init(ADC_HandleTypeDef *AdcHandle) {
    uint8_t i = 0;

    if (AdcHandle != NULL) {
        for (i = 0; i < adc_number_of_used_devices; i++) {
            if (AdcHandle[i].Instance == ADC1) {
                __HAL_RCC_ADC1_CLK_ENABLE();
            }
            if (AdcHandle[i].Instance == ADC2) {
                __HAL_RCC_ADC2_CLK_ENABLE();
            }
            if (AdcHandle[i].Instance == ADC3) {
                __HAL_RCC_ADC3_CLK_ENABLE();
            }
            HAL_ADC_Init(&AdcHandle[i]);
        }
    }
}


void ADC_Convert(ADC_HandleTypeDef *AdcHandle) {
    /* Enables Interrupt and starts ADC conversion */
    HAL_ADC_Start_IT(AdcHandle);
}


void ADC_Ctrl(void) {
    static uint8_t vbat = 0;

    adc_cfg.Rank = 1;
    adc_cfg.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    adc_cfg.Offset = 0;

    /* Alternate ADC conversion between temperature sensor and battery voltage */
    if (adc_conversion_state == ADC_CONVERT) {
        if (vbat == 0) {
            vbat = 1;
            adc_cfg.Channel = ADC_CHANNEL_VBAT;
        } else if (vbat == 1) {
            vbat = 0;
            adc_cfg.Channel = ADC_CHANNEL_TEMPSENSOR;
        }
    }

    if (adc_conversion_state == ADC_CONVERT) {
        /* Configure ADC Channel */
        HAL_ADC_ConfigChannel(&adc_devices[0], &adc_cfg);
        /* Enable conversion */
        ADC_Convert(&adc_devices[0]);
        adc_conversion_state = ADC_WAITFORCONVERSION;
    }
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle) {
    uint32_t raw_voltage = 0;
    float scaled_voltage = 0.0;
    float scaled_temperature = 0.0;

    /* Disable interrupt and stop ADC conversion */
    HAL_ADC_Stop_IT(AdcHandle);
    raw_voltage = HAL_ADC_GetValue(AdcHandle);

    if (adc_cfg.Channel == ADC_CHANNEL_VBAT) {
        scaled_voltage = ((float)(raw_voltage)*(ADC_VREF_EXT)*ADC_VBAT_VOLTAGE_DIVIDER)/(ADC_FULL_RANGE);
        adc_vbat_mV = scaled_voltage * 1000;  /* convert V to mV */
    }

    if (adc_cfg.Channel == ADC_CHANNEL_TEMPSENSOR) {
        scaled_voltage = ((float)(raw_voltage)*(1000.0*(float)(ADC_VREF_EXT)))/(ADC_FULL_RANGE);
        scaled_temperature = (scaled_voltage - ADC_V25)/(1000.0*ADC_AVG_SLOPE) + 25.0;
        adc_mcuTemp = scaled_temperature;
    }

    /* Start new conversion */
    adc_conversion_state = ADC_CONVERT;
}


/**
 * @brief get coin cell battery voltage
 */
extern float ADC_GetVBAT_mV(void) {
    return adc_vbat_mV;
}

/**
 * @brief get MCU temperature
 */
extern float ADC_GetMCUTemp_C(void) {
    return adc_mcuTemp;
}
