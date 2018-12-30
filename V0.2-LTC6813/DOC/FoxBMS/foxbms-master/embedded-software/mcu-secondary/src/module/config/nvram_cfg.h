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
 * @file    nvram_cfg.h
 * @author  foxBMS Team
 * @date    02.03.2016 (date of creation)
 * @ingroup MODULE_CONF
 * @prefix  NVM
 *
 * @brief   Headers for the configuration for static backup RAM.
 *
 */

#ifndef NVRAM_CFG_H_
#define NVRAM_CFG_H_

/*================== Includes =============================================*/
#include "general.h"

#include "diag.h"

/*================== Macros and Definitions ===============================*/

/**
 * Operating hours-Timer
 */
typedef struct {
    uint8_t  Timer_1ms;   /*!< milliseconds       */
    uint8_t  Timer_10ms;  /*!< 10*milliseconds    */
    uint8_t  Timer_100ms; /*!< 100*milliseconds   */
    uint8_t  Timer_sec;   /*!< seconds            */
    uint8_t  Timer_min;   /*!< minutes            */
    uint8_t  Timer_h;     /*!< hours              */
    uint16_t Timer_d;     /*!< days               */
    uint16_t reserved[2]; /*!< reserved for future use */
} NVRAM_OPERATING_HOURS_s;


typedef struct {
    DIAG_CONTACTOR_s data;
    uint32_t previous_timestamp;
    uint32_t timestamp;
    uint32_t checksum;
} NVRRAM_CH_CONT_COUNT_s;

typedef struct {
    NVRAM_OPERATING_HOURS_s data;
    uint32_t previous_timestamp;
    uint32_t timestamp;
    uint32_t checksum;
} NVRAM_CH_OP_HOURS_s;

/*================== Constant and Variable Definitions ====================*/
extern const NVRRAM_CH_CONT_COUNT_s default_contactors_count;
extern const NVRAM_CH_OP_HOURS_s default_operating_hours;


/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

#endif /* NVRAM_CFG_H_ */
