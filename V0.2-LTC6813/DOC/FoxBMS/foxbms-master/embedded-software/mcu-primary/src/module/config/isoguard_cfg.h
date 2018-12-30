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
 * @file    isoguard_cfg.h
 * @author  foxBMS Team
 * @date    08.12.2015 (date of creation)
 * @ingroup DRIVERS_CONF
 * @prefix  ISO
 *
 * @brief   Headers for the configuration for the isolation monitoring.
 *
 * Configuration header file for isguard module
 */

#ifndef ISOGUARD_CFG_H_
#define ISOGUARD_CFG_H_

/*================== Includes =============================================*/
#include "general.h"

/*================== Macros and Definitions ===============================*/

/**
 * @ingroup CONFIG_ISOGUARD
 * Enable/Disable switch for the isoguard module
 * \par Type:
 * toggle
 * \par Default:
 * True
*/

/**
 * Enable/Disable switch for the isoguard module
 */
#define ISO_ISOGUARD_ENABLE

/**
 * @ingroup CONFIG_ISOGUARD
 * Periodic calling time of isoguard measurement. Specifies the hysteresis
 * for valid measurement values after startup or reset
 * \par Type:
 * int
 * \par Unit:
 * ms
 * \par Range:
 * 100 < x
 * \par Default:
 * 200
*/

/**
 * isoguard call cycle time
 */
#define ISO_CYCLE_TIME              200

/**
 * @ingroup CONFIG_ISOGUARD
 * Resistance threshold in kOhm to differentiate of ISOIR_RESIST_MEAS_GOOD and
 * ISOIR_RESIST_MEAS_BAD in NORMAL_MODE within duty cycle of 10%..90%
 * \par Type:
 * int
 * \par Unit:
 * kOhm
 * \par Range:
 * 100 < x
 * \par Default:
 * 400
*/

/**
 * Resistance threshold in kOhm to differentiate of ISOIR_RESIST_MEAS_GOOD and
 * ISOIR_RESIST_MEAS_BAD in NORMAL_MODE within duty cycle of 10%..90%
 */
#define ISO_RESISTANCE_THRESHOLD_kOhm    400

/*================== Constant and Variable Definitions ====================*/
typedef enum {
    ISO_STATE_UNINITIALIZED = 0,
    ISO_STATE_INITIALIZED   = 1,
} ISO_INIT_STATE_e;

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

#endif /* ISOGUARD_CFG_H_ */
