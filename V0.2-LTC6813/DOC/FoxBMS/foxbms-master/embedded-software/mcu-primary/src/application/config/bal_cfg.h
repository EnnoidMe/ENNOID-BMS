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
 * @file    bal_cfg.h
 * @author  foxBMS Team
 * @date    26.02.2016 (date of creation)
 * @ingroup DRIVERS_CONF
 * @prefix  BAL
 *
 * @brief   Header for the configuration for the driver for balancing
 *
 */

#ifndef BAL_CFG_H_
#define BAL_CFG_H_

/*================== Includes =============================================*/
#include "general.h"

/*================== Macros and Definitions ===============================*/

/**
 * BAL statemachine short time definition in 100*ms
 */

#define BAL_STATEMACH_SHORTTIME_100MS     1

/**
 * BAL statemachine long time definition in 100*ms
 */

#define BAL_STATEMACH_LONGTIME_100MS     50

/**
 * BAL statemachine balancing time in 100*ms
 */
#define BAL_STATEMACH_BALANCINGTIME_100MS     10

/**
 * BAL time to wait in seconds before balancing once no current flowing
 */

#define BAL_TIME_BEFORE_BALANCING_S     600

/**
 * BAL voltage threshold for balancing in mV
 */

#define BAL_THRESHOLD_MV     200

/**
 * BAL hysteresis for voltage threshold when balancing was finished in mV
 */

#define BAL_HYSTERESIS_MV     200

/**
 * BAL lower voltage limit in MV
 */

#define BAL_LOWER_VOLTAGE_LIMIT_MV     2000

/**
 * BAL upper temperature limit in Celsius
 */

#define BAL_UPPER_TEMPERATURE_LIMIT_DEG     70

/**
 * If set to TRUE, SOC-history based balancing is used.
 * An OC-SOC look-up table is needed.
 * If set to FALSE, voltage-based balancing is used.
 *
*/
#define BALANCING_VOLTAGE_BASED           TRUE


/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/


#endif /* BAL_CFG_H_ */
