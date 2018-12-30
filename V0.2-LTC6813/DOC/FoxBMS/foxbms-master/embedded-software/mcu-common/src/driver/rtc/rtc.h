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
 * @file    rtc.h
 * @author  foxBMS Team
 * @date    20.10.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  RTC
 *
 * @brief   Headers for the driver for the real time clock.
 *
 */

#ifndef RTC_H_
#define RTC_H_

/*================== Includes =============================================*/
#include "rtc_cfg.h"

/*================== Macros and Definitions ===============================*/

/**
 * Systemwide low level control, status Information of system
 */
typedef struct {
    RTC_Date_s date;    /*!< RCC clock control & status register at startup */
    RTC_Time_s time;    /*!<  state of Eeprom Driver State Machine          */
    /* uint32_t date;    !< RCC clock control & status register at startup */
    /* uint32_t time;    !<  state of Eeprom Driver State Machine          */
    uint32_t dummy[4];  /*!<                                                */
} RTC_RESETSOURCE_s;


/**
 * Systemwide low level control, status Information of system
 */
typedef struct {
    uint32_t CSR;                       /*!< RCC clock control & status register at startup */
    RTC_Date_s boot_rtcdate;            /*!< boot date                                      */
    RTC_Time_s boot_rtctime;            /*!< boot time                                      */
    RTC_Date_s terminate_rtcdate;       /*!< TODO                                           */
    RTC_Time_s terminate_rtctime;       /*!< TODO                                           */
    uint32_t dummy[4];                  /*!<                                                */
    RTC_RESETSOURCE_s resetsource[20]; /*!< in counts of 1ms                       */
} RTC_STATUS_s;

/*================== Constant and Variable Definitions ====================*/
extern RTC_HandleTypeDef hrtc;
extern RTC_STATUS_s main_state;

/*================== Function Prototypes ==================================*/

/**
 * @brief   initializes rtc module
 */
extern void RTC_Init(void);

/**
 * @brief   sets RTC time
 */
extern void RTC_setTime(RTC_TimeTypeDef* time);

/**
 * @brief   gets RTC time
 */
extern void RTC_getTime(RTC_TimeTypeDef* time);

/**
 * @brief   sets RTC date
 */
extern void RTC_setDate(RTC_DateTypeDef* date);

/**
 * @brief   gets RTC date
 */
extern void RTC_getDate(RTC_DateTypeDef* date);

/*
 * @brief gets current unix time
 */
extern uint32_t RTC_getUnixTime(void);

/*
 * @brief converts current RTC time into unix format
 */
extern uint32_t RTC_convertRTCtoUnixTime(RTC_TimeTypeDef* time, RTC_DateTypeDef* date);

/*================== Function Implementations =============================*/

#endif /* RTC_H_ */
