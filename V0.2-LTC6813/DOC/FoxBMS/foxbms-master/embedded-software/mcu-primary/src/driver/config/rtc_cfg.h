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
 * @file    rtc_cfg.h
 * @author  foxBMS Team
 * @date    02.10.2015 (date of creation)
 * @ingroup DRIVERS_CONF
 * @prefix  RTC
 *
 * @brief   Headers for the configuration for the real time clock
 *
 */

#ifndef RTC_CFG_H_
#define RTC_CFG_H_

/*================== Includes =============================================*/
#include "general.h"

#include "cpu_cfg.h"

/*================== Macros and Definitions ===============================*/
typedef RTC_InitTypeDef     RTC_INIT_s;
typedef RTC_TimeTypeDef     RTC_Time_s;
typedef RTC_DateTypeDef     RTC_Date_s;

/* backup size of the registers 80 Byte */
#define RTC_DATAVALID_VARIABLE              (hrtc.Instance->BKP0R)  /* rtc backup register valid */
#define RTC_BKPSRAM_DATAVALID_VARIABLE      (hrtc.Instance->BKP1R)  /* bkpsram data valid */
#define RTC_BKPDIAG_DATAVALID_VARIABLE      (hrtc.Instance->BKP2R)  /* bpksram diag data valid */
#define RTC_NVMRAM_DATAVALID_VARIABLE       (hrtc.Instance->BKP3R)  /* non-volatile data backups valid */
#define RTC_WDG_RESETCOUNTER                (hrtc.Instance->BKP5R)  /* wdg resetcounter */

/**
 * struct for the initialization and configuration of RTC
 */
typedef struct {
    RCC_OscInitTypeDef          oscInitStruct;  /*!< clock configuration of RTCs clock source */
    RCC_PeriphCLKInitTypeDef    clkInitStruct;  /*!< clock source of RTC:  RTCClk can be derived from HSE (ext. crystal), LSE (ext. crystal), LSI (internal RC oscillator) */
    RTC_INIT_s                  initconfig;     /*!< RTC configuration: hour format, prescaler,... */
    uint8_t                     timeformat;     /*!< date and time format configuration (binary or BCD) */
    RTC_Time_s                  defaultTime;    /*!< default time configuration */
    RTC_Date_s                  defaultDate;    /*!< default date configuration */
} RTC_CFG_s;


typedef enum {
    BKPREGISTER_DATA_VALID          = 0,
    BKPREGISTER_BKPSRAM_VALID       = 1,
    BKPREGISTER_BKPSRAM_DIAG_VALID  = 2,
    BKPREGISTER_NVRAM_VALID         = 3,
    BKPREGISTER_WDG_RESETCOUNTER    = 5,
} RTC_BKPREGISTER_e;


typedef enum {
    BKPREGISTER_NOT_VALID = 0,
    BKPREGISTER_VALID = 1,
}RTC_BKPREGISTER_VALIDSTATE_e;

/*================== Constant and Variable Definitions ====================*/
extern RTC_CFG_s rtc_cfg;
extern RTC_HandleTypeDef hrtc;

/*================== Function Prototypes ===================================*/

/**
 * @brief   get backup register value
 */
uint32_t RTC_getRegisterValueBKPSRAM(RTC_BKPREGISTER_e registerNumber);

/**
 * @brief   set backup register value
 */
void RTC_setRegisterValueBKPSRAM(RTC_BKPREGISTER_e registerNumber, uint32_t value);

/*================== Function Implementations ==============================*/

#endif /* RTC_CFG_H_ */
