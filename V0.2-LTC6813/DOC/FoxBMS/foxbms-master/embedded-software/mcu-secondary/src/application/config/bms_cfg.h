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
 * @file    bms_cfg.h
 * @author  foxBMS Team
 * @date    21.09.2015 (date of creation)
 * @ingroup ENGINE_CONF
 * @prefix  BMS
 *
 * @brief   bms driver configuration header
 */

#ifndef BMS_CFG_H_
#define BMS_CFG_H_

/*================== Includes =============================================*/
#include "general.h"

/*================== Macros and Definitions ===============================*/
#define BMS_ECU_GOTOSLEEP()          /* CPU_GoToSleep(60)/* EcuMode_GoToSleep();         TODO Review: dummy macro with no effect */
#define BMS_STARTAWAKESELFCHECK()    /* Start SelfCheck in case of awaking from sleep    TODO Review: dummy macro with no effect */
#define BMS_STARTPOWERONSELFCHECK()  /* Start SelfCheck in case of starting by power on  TODO Review: dummy macro with no effect */

/* #define BMS_RESETSLEEPFLAG() RTC_IF_ResetSleepFlag () TODO Review: function RTC_IF_ResetSleepFlag not existing */

/**
 * @ingroup CONFIG_BMS
 * maximum current safe operating limit
 * When limit is violated, error state is requested and contactors will open.
 * \par Type:
 * int
 * \par Unit:
 * mA
 * \par Default:
 * 15000
*/
#define BMS_CURRENTMAX 15000

/**
 * @ingroup CONFIG_BMS
 * minimum current safe operating limit
 * When limit is violated, error state is requested and contactors will open.
 * \par Type:
 * int
 * \par Unit:
 * mA
 * \par Default:
 * -15000
*/
#define BMS_CURRENTMIN -15000

/**
 * @ingroup CONFIG_BMS
 * checking if current is in SOF limits of cells. Set FALSE for test without checking against SOF derating.
 * \par Type:
 * select(2)
 * \par Default:
 * 1
*/
/* #define BMS_TEST_CELL_SOF_LIMITS  TRUE */
#define BMS_TEST_CELL_SOF_LIMITS FALSE







#define BMS_REQ_ID_NOREQ 0

/**
 * @ingroup CONFIG_BMS
 * this is the ID that should be requested via CAN signal to go to STANDBY state (ready, but no contactors closed)
 * \par Type:
 * int
 * \par Default:
 * 8
 * \par Range:
 * 0<=x
*/
#define BMS_REQ_ID_STANDBY 8

/**
 * @ingroup CONFIG_BMS
 * this is the ID that should be requested via CAN signal to go to NORMAL state (contactors closing procedure)
 * \par Type:
 * int
 * \par Default:
 * 3
 * \par Range:
 * 0<=x
*/
#define BMS_REQ_ID_NORMAL 3

/**
 * @ingroup CONFIG_BMS
 * this is the ID that should be requested via CAN signal to go to CHARGE state (contactors closing procedure)
 * \par Type:
 * int
 * \par Default:
 * 4
 * \par Range:
 * 0<=x
*/
#define BMS_REQ_ID_CHARGE 4



/**
 * BMS statemachine short time definition in ms
 */
#define BMS_STATEMACH_SHORTTIME_MS 1

/**
 * BMS statemachine medium time definition in ms
 */
#define BMS_STATEMACH_MEDIUMTIME_MS 5

/**
 * BMS statemachine long time definition in ms
 */
#define BMS_STATEMACH_LONGTIME_MS 100

/**
 * BMS statemachine very long time definition in ms
 */
#define BMS_STATEMACH_VERYLONGTIME_MS 2000


/**
 * @ingroup CONFIG_BMS
 * \par Type:
 * int
 * \par Default:
 * 10
 * \par Range:
 * [5,15]
 * \par Unit:
 * 10*ms
*/
#define BMS_SELFPOWERONCHECK_TIMEOUT    10  /* 100ms */

/**
 * @ingroup CONFIG_BMS
 * \par Type:
 * int
 * \par Default:
 * 10
 * \par Range:
 * [5,15]
 * \par Unit:
 * 10*ms
*/
#define BMS_SELFAWAKECHECK_TIMEOUT      10  /* 100ms */


/**
 * @ingroup CONFIG_BMS
 * \par Type:
 * int
 * \par Default:
 * 50
 * \par Range:
 * [40,60]
 * \par Unit:
 * 10*ms
*/
#define BMS_IDLE_TIMEOUT                500  /* 5s timeout to go to sleep or power off in idle state */

#define BMS_GETSELFCHECK_STATE()            BMS_CHECK_OK            /* function could return: BMS_CHECK_NOT_OK or OK BMS_CHECK_BUSY */
#define BMS_GETPOWERONSELFCHECK_STATE()     BMS_CHECK_OK            /* function could return: BMS_CHECK_NOT_OK or OK BMS_CHECK_BUSY */
#define BMS_CHECKPRECHARGE()                BMS_CheckPrecharge()    /* DIAG_CheckPrecharge() */


/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

#endif /* BMS_CFG_H_ */
