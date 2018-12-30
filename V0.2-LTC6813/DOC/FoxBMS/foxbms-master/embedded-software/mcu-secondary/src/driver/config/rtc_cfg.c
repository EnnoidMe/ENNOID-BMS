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
 * @file    rtc_cfg.c
 * @author  foxBMS Team
 * @date    13.07.2016 (date of creation)
 * @ingroup DRIVERS_CONF
 * @prefix  RTC
 *
 * @brief   Configuration for the real time clock
 *
 */

/*================== Includes =============================================*/
#include "rtc_cfg.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/

#if 0       /* Configuration with RTC clock source: LSI oscillator */
RTC_CFG_s rtc_cfg = {
    .oscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI,      /* MCU1: LSE (external 32 kHz oscillator) not implemented, */
    .oscInitStruct.LSIState      = RCC_LSI_ON,                   /* so use LSI (Internal low-speed oscillator) */

    .clkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC,
    .clkInitStruct.RTCClockSelection   = RCC_RTCCLKSOURCE_LSI,   /* RTC Clocksourse is LSI */

    .initconfig.HourFormat     = RTC_HOURFORMAT_24,
    .initconfig.AsynchPrediv   = (125-1),                 /* LSI = 32kHz:    32kHz/(125*256) = 1Hz */
    .initconfig.SynchPrediv    = (256-1),                 /* Subsecond runs with 32kHz/125 */
    .initconfig.OutPut         = RTC_OUTPUT_ALARMA,
    .initconfig.OutPutPolarity = RTC_OUTPUT_POLARITY_LOW,
    .initconfig.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN,

    .timeformat                 = FORMAT_BIN,
    .defaultTime.Hours          = 18,
    .defaultTime.Minutes        = 20,
    .defaultTime.Seconds        = 0,
    .defaultTime.SubSeconds     = 0,
    .defaultTime.TimeFormat     = RTC_HOURFORMAT12_PM,
    .defaultTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE,
    .defaultTime.StoreOperation = RTC_STOREOPERATION_RESET,

    .defaultDate.WeekDay        = RTC_WEEKDAY_TUESDAY,
    .defaultDate.Month          = RTC_MONTH_JULY,
    .defaultDate.Date           = 12,
    .defaultDate.Year           = 16,

};
#endif

/* Configuration with RTC clock source: HSE cyrstal */
RTC_CFG_s rtc_cfg = {
    .oscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE,      /* MCU1: LSE (external 32 kHz oscillator) not implemented, */
    .oscInitStruct.HSEState      = RCC_HSE_ON,                   /* so use HSE 8 MHz (external high-speed oscillator) */

    .clkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC,
    .clkInitStruct.RTCClockSelection   = RCC_RTCCLKSOURCE_HSE_DIV16,   /* RTC Clocksourse is HSE, HSE_RTC = 8MHz/16 = 500kHz */

    .initconfig.HourFormat     = RTC_HOURFORMAT_24,
    .initconfig.AsynchPrediv   = (125-1),                 /* HSE_RTC = 500kHz:    500kHz/(125*4000) = 1Hz */
    .initconfig.SynchPrediv    = (4000-1),                /* Subsecond runs with 500kHz/125 = 250us */
    .initconfig.OutPut         = RTC_OUTPUT_ALARMA,
    .initconfig.OutPutPolarity = RTC_OUTPUT_POLARITY_LOW,
    .initconfig.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN,

    .timeformat                 = FORMAT_BIN,
    .defaultTime.Hours          = 18,
    .defaultTime.Minutes        = 20,
    .defaultTime.Seconds        = 0,
    .defaultTime.SubSeconds     = 0,
    .defaultTime.TimeFormat     = RTC_HOURFORMAT12_PM,
    .defaultTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE,
    .defaultTime.StoreOperation = RTC_STOREOPERATION_RESET,

    .defaultDate.WeekDay        = RTC_WEEKDAY_TUESDAY,
    .defaultDate.Month          = RTC_MONTH_JULY,
    .defaultDate.Date           = 12,
    .defaultDate.Year           = 16,

};

RTC_HandleTypeDef hrtc;

/*================== Function Prototypes ===================================*/

/*================== Function Implementations ==============================*/

uint32_t RTC_getRegisterValueBKPSRAM(RTC_BKPREGISTER_e registerNumber) {
    uint32_t retval = 0;
    if (registerNumber == BKPREGISTER_DATA_VALID) {
        retval = RTC_DATAVALID_VARIABLE;
    } else if (registerNumber == BKPREGISTER_BKPSRAM_VALID) {
        retval = RTC_BKPSRAM_DATAVALID_VARIABLE;
    } else if (registerNumber == BKPREGISTER_BKPSRAM_DIAG_VALID) {
        retval = RTC_BKPDIAG_DATAVALID_VARIABLE;
    } else if (registerNumber == BKPREGISTER_NVRAM_VALID) {
        retval = RTC_NVMRAM_DATAVALID_VARIABLE;
    } else if (registerNumber == BKPREGISTER_WDG_RESETCOUNTER) {
        retval = RTC_WDG_RESETCOUNTER;
    } else {
        ;
    }
    return retval;
}


void RTC_setRegisterValueBKPSRAM(RTC_BKPREGISTER_e registerNumber, uint32_t value) {
    if (registerNumber == BKPREGISTER_DATA_VALID) {
        RTC_DATAVALID_VARIABLE = value;
    } else if (registerNumber == BKPREGISTER_BKPSRAM_VALID) {
        RTC_BKPSRAM_DATAVALID_VARIABLE = value;
    } else if (registerNumber == BKPREGISTER_BKPSRAM_DIAG_VALID) {
        RTC_BKPDIAG_DATAVALID_VARIABLE = value;
    } else if (registerNumber == BKPREGISTER_NVRAM_VALID) {
        RTC_NVMRAM_DATAVALID_VARIABLE = value;
    } else if (registerNumber == BKPREGISTER_WDG_RESETCOUNTER) {
        RTC_WDG_RESETCOUNTER = value;
    }
}
