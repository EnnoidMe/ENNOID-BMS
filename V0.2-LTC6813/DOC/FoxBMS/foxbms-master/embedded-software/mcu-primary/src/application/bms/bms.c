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
 * @file    bms.c
 * @author  foxBMS Team
 * @date    21.09.2015 (date of creation)
 * @ingroup ENGINE
 * @prefix  BMS
 *
 * @brief   bms driver implementation
 */


/*================== Includes =============================================*/
#include "bms.h"

#include "bal.h"
#include "batterycell_cfg.h"
#include "database.h"
#include "diag.h"
#include "interlock.h"
#include "os.h"


/*================== Macros and Definitions ===============================*/

/**
 * Saves the last state and the last substate
 */
#define BMS_SAVELASTSTATES()    bms_state.laststate = bms_state.state; \
                                bms_state.lastsubstate = bms_state.substate;

/*================== Constant and Variable Definitions ====================*/

/**
 * contains the state of the contactor state machine
 */
static BMS_STATE_s bms_state = {
    .timer                  = 0,
    .statereq               = BMS_STATE_NO_REQUEST,
    .state                  = BMS_STATEMACH_UNINITIALIZED,
    .substate               = BMS_ENTRY,
    .laststate              = BMS_STATEMACH_UNINITIALIZED,
    .lastsubstate           = 0,
    .triggerentry           = 0,
    .ErrRequestCounter      = 0,
    .counter                = 0,
};

/*================== Function Prototypes ==================================*/

static BMS_RETURN_TYPE_e BMS_CheckStateRequest(BMS_STATE_REQUEST_e statereq);
static BMS_STATE_REQUEST_e BMS_GetStateRequest(void);
static BMS_STATE_REQUEST_e BMS_TransferStateRequest(void);
static uint8_t BMS_CheckReEntrance(void);
static uint8_t BMS_CheckCANRequests(void);
static STD_RETURN_TYPE_e BMS_CheckAnyErrorFlagSet(void);
static void BMS_CheckVoltages(void);
static void BMS_CheckTemperatures(void);
static void BMS_CheckCurrent(void);
static void BMS_CheckSlaveTemperatures(void);
/*================== Function Implementations =============================*/

/**
 * @brief   re-entrance check of SYS state machine trigger function
 *
 * @details This function is not re-entrant and should only be called time- or event-triggered. It
 *          increments the triggerentry counter from the state variable ltc_state. It should never
 *          be called by two different processes, so if it is the case, triggerentry should never
 *          be higher than 0 when this function is called.
 *
 * @return  retval  0 if no further instance of the function is active, 0xff else
 */
static uint8_t BMS_CheckReEntrance(void) {
    uint8_t retval = 0;
    OS_TaskEnter_Critical();
    if (!bms_state.triggerentry) {
        bms_state.triggerentry++;
    } else {
        retval = 0xFF;  /* multiple calls of function */
    }
    OS_TaskExit_Critical();
    return (retval);
}

/**
 * @brief   gets the current state request.
 *
 * @details This function is used in the functioning of the SYS state machine.
 *
 * @return  current state request, taken from BMS_STATE_REQUEST_e
 */
static BMS_STATE_REQUEST_e BMS_GetStateRequest(void) {
    BMS_STATE_REQUEST_e retval = BMS_STATE_NO_REQUEST;

    OS_TaskEnter_Critical();
    retval    = bms_state.statereq;
    OS_TaskExit_Critical();

    return (retval);
}


BMS_STATEMACH_e BMS_GetState(void) {
    return (bms_state.state);
}

/**
 * @brief   transfers the current state request to the state machine.
 *
 * @details This function takes the current state request from cont_state and transfers it to th
 *          state machine. It resets the value from cont_state to BMS_STATE_NO_REQUEST
 *
 * @return  retVal          current state request, taken from BMS_STATE_REQUEST_e
 */
static BMS_STATE_REQUEST_e BMS_TransferStateRequest(void) {
    BMS_STATE_REQUEST_e retval = BMS_STATE_NO_REQUEST;

    OS_TaskEnter_Critical();
    retval    = bms_state.statereq;
    bms_state.statereq = BMS_STATE_NO_REQUEST;
    OS_TaskExit_Critical();
    return (retval);
}



BMS_RETURN_TYPE_e BMS_SetStateRequest(BMS_STATE_REQUEST_e statereq) {
    BMS_RETURN_TYPE_e retVal = BMS_STATE_NO_REQUEST;

    OS_TaskEnter_Critical();
    retVal = BMS_CheckStateRequest(statereq);

    if (retVal == BMS_OK) {
            bms_state.statereq = statereq;
    }
    OS_TaskExit_Critical();

    return (retVal);
}

/**
 * @brief   checks the state requests that are made.
 *
 * @details This function checks the validity of the state requests. The results of the checked is
 *          returned immediately.
 *
 * @param   statereq    state request to be checked
 *
 * @return  result of the state request that was made, taken from BMS_RETURN_TYPE_e
 */
static BMS_RETURN_TYPE_e BMS_CheckStateRequest(BMS_STATE_REQUEST_e statereq) {
    if (statereq == BMS_STATE_ERROR_REQUEST) {
        return BMS_OK;
    }

    if (bms_state.statereq == BMS_STATE_NO_REQUEST) {
        /* init only allowed from the uninitialized state */
        if (statereq == BMS_STATE_INIT_REQUEST) {
            if (bms_state.state == BMS_STATEMACH_UNINITIALIZED) {
                return BMS_OK;
            } else {
                return BMS_ALREADY_INITIALIZED;
            }
        } else {
            return BMS_ILLEGAL_REQUEST;
        }
    } else {
        return BMS_REQUEST_PENDING;
    }
}

void BMS_Trigger(void) {
    BMS_STATE_REQUEST_e statereq = BMS_STATE_NO_REQUEST;
    CONT_STATEMACH_e contstate = CONT_STATEMACH_UNDEFINED;
    DATA_BLOCK_SYSTEMSTATE_s systemstate = {0};

    DIAG_SysMonNotify(DIAG_SYSMON_BMS_ID, 0);  /* task is running, state = ok */

    if (bms_state.state != BMS_STATEMACH_UNINITIALIZED) {
        BMS_CheckVoltages();
        BMS_CheckTemperatures();
        BMS_CheckCurrent();
        BMS_CheckSlaveTemperatures();
    }
    /* Check re-entrance of function */
    if (BMS_CheckReEntrance()) {
        return;
    }

    if (bms_state.timer) {
        if (--bms_state.timer) {
            bms_state.triggerentry--;
            return;    /* handle state machine only if timer has elapsed */
        }
    }

    /****Happens every time the state machine is triggered**************/
    switch (bms_state.state) {
        /****************************UNINITIALIZED***********************************/
        case BMS_STATEMACH_UNINITIALIZED:
            /* waiting for Initialization Request */
            statereq = BMS_TransferStateRequest();
            if (statereq == BMS_STATE_INIT_REQUEST) {
                BMS_SAVELASTSTATES();
                bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                bms_state.state = BMS_STATEMACH_INITIALIZATION;
                bms_state.substate = BMS_ENTRY;
            } else if (statereq == BMS_STATE_NO_REQUEST) {
                /* no actual request pending /*  */
            } else {
                bms_state.ErrRequestCounter++;  /* illegal request pending */
            }
            break;


        /****************************INITIALIZATION**********************************/
        case BMS_STATEMACH_INITIALIZATION:
            BMS_SAVELASTSTATES();
            /* CONT_SetStateRequest(CONT_STATE_INIT_REQUEST); */

            bms_state.timer = BMS_STATEMACH_LONGTIME_MS;
            bms_state.state = BMS_STATEMACH_INITIALIZED;
            bms_state.substate = BMS_ENTRY;

            break;

        /****************************INITIALIZED*************************************/
        case BMS_STATEMACH_INITIALIZED:
            BMS_SAVELASTSTATES();
            bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
            bms_state.state = BMS_STATEMACH_IDLE;
            bms_state.substate = BMS_ENTRY;
            break;

        /****************************IDLE*************************************/
        case BMS_STATEMACH_IDLE:
            BMS_SAVELASTSTATES();

            if (bms_state.substate == BMS_ENTRY) {
                DB_ReadBlock(&systemstate, DATA_BLOCK_ID_SYSTEMSTATE);
                systemstate.bms_state = BMS_STATEMACH_IDLE;
                DB_WriteBlock(&systemstate, DATA_BLOCK_ID_SYSTEMSTATE);
                bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                bms_state.substate = BMS_CHECK_ERROR_FLAGS;
                break;
            } else if (bms_state.substate == BMS_CHECK_ERROR_FLAGS) {
                if (BMS_CheckAnyErrorFlagSet() == E_NOT_OK) {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.state = BMS_STATEMACH_ERROR;
                    bms_state.substate = BMS_ENTRY;
                    break;
                } else {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.substate = BMS_CHECK_STATE_REQUESTS;
                    break;
                }
            } else if (bms_state.substate == BMS_CHECK_STATE_REQUESTS) {
                if (BMS_CheckCANRequests() == BMS_REQ_ID_STANDBY) {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.state = BMS_STATEMACH_STANDBY;
                    bms_state.substate = BMS_ENTRY;
                    break;
                } else {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.substate = BMS_CHECK_ERROR_FLAGS;
                    break;
                }
            }
            break;


        /****************************STANDBY*************************************/
        case BMS_STATEMACH_STANDBY:
            BMS_SAVELASTSTATES();

            if (bms_state.substate == BMS_ENTRY){
                BAL_SetStateRequest(BAL_STATE_ALLOWBALANCING_REQUEST);
#if BUILD_MODULE_ENABLE_CONTACTOR == 1
                CONT_SetStateRequest(CONT_STATE_STANDBY_REQUEST);
#endif
#if BUILD_MODULE_ENABLE_ILCK == 1
                ILCK_SetStateRequest(ILCK_STATE_CLOSE_REQUEST);
#endif

                bms_state.timer = BMS_STATEMACH_MEDIUMTIME_MS;
                bms_state.substate = BMS_CHECK_ERROR_FLAGS_INTERLOCK;
                DB_ReadBlock(&systemstate, DATA_BLOCK_ID_SYSTEMSTATE);
                systemstate.bms_state = BMS_STATEMACH_STANDBY;
                DB_WriteBlock(&systemstate, DATA_BLOCK_ID_SYSTEMSTATE);
                break;
            } else if (bms_state.substate == BMS_CHECK_ERROR_FLAGS_INTERLOCK) {
                if (BMS_CheckAnyErrorFlagSet() == E_NOT_OK) {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.state = BMS_STATEMACH_ERROR;
                    bms_state.substate = BMS_ENTRY;
                    break;
                } else {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.substate = BMS_INTERLOCK_CHECKED;
                    break;
                }
            }
            else if (bms_state.substate == BMS_INTERLOCK_CHECKED){
                bms_state.timer = BMS_STATEMACH_VERYLONGTIME_MS;
                bms_state.substate = BMS_CHECK_ERROR_FLAGS;
                break;
            } else if (bms_state.substate == BMS_CHECK_ERROR_FLAGS) {
                if (BMS_CheckAnyErrorFlagSet() == E_NOT_OK) {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.state = BMS_STATEMACH_ERROR;
                    bms_state.substate = BMS_ENTRY;
                    break;
                } else {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.substate = BMS_CHECK_STATE_REQUESTS;
                    break;
                }
            } else if (bms_state.substate == BMS_CHECK_STATE_REQUESTS) {
                if (BMS_CheckCANRequests() == BMS_REQ_ID_NORMAL) {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.state = BMS_STATEMACH_PRECHARGE;
                    bms_state.substate = BMS_ENTRY;
                    break;
                }
                if (BMS_CheckCANRequests() == BMS_REQ_ID_CHARGE){
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.state = BMS_STATEMACH_CHARGE_PRECHARGE;
                    bms_state.substate = BMS_ENTRY;
                    break;
                } else {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.substate = BMS_CHECK_ERROR_FLAGS;
                    break;
                }
            }
            break;

        /****************************PRECHARGE*************************************/
        case BMS_STATEMACH_PRECHARGE:
            BMS_SAVELASTSTATES();

            if (bms_state.substate == BMS_ENTRY) {
                BAL_SetStateRequest(BAL_STATE_NOBALANCING_REQUEST);
                DB_ReadBlock(&systemstate, DATA_BLOCK_ID_SYSTEMSTATE);
                systemstate.bms_state = BMS_STATEMACH_PRECHARGE;
                DB_WriteBlock(&systemstate, DATA_BLOCK_ID_SYSTEMSTATE);
#if BUILD_MODULE_ENABLE_CONTACTOR == 1
                CONT_SetStateRequest(CONT_STATE_NORMAL_REQUEST);
#endif
                bms_state.substate = BMS_CHECK_ERROR_FLAGS;
                bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                break;
            } else if (bms_state.substate == BMS_CHECK_ERROR_FLAGS) {
                if (BMS_CheckAnyErrorFlagSet() == E_NOT_OK) {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.state = BMS_STATEMACH_ERROR;
                    bms_state.substate = BMS_ENTRY;
                    break;
                } else {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.substate = BMS_CHECK_STATE_REQUESTS;
                    break;
                }
            } else if (bms_state.substate == BMS_CHECK_STATE_REQUESTS) {
                if (BMS_CheckCANRequests() == BMS_REQ_ID_STANDBY) {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.state = BMS_STATEMACH_STANDBY;
                    bms_state.substate = BMS_ENTRY;
                    break;
                } else {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
#if BUILD_MODULE_ENABLE_CONTACTOR == 1
                    bms_state.substate = BMS_CHECK_CONTACTOR_NORMAL_STATE;
#else
                    bms_state.state = BMS_STATEMACH_NORMAL;
                    bms_state.substate = BMS_ENTRY;
#endif
                    break;
                }
#if BUILD_MODULE_ENABLE_CONTACTOR == 1
            } else if (bms_state.substate == BMS_CHECK_CONTACTOR_NORMAL_STATE) {
                contstate = CONT_GetState();
                if (contstate == CONT_STATEMACH_NORMAL) {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.state = BMS_STATEMACH_NORMAL;
                    bms_state.substate = BMS_ENTRY;
                    break;
                } else if (contstate == CONT_STATEMACH_ERROR) {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.state = BMS_STATEMACH_ERROR;
                    bms_state.substate = BMS_ENTRY;
                    break;
                } else {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.substate = BMS_CHECK_ERROR_FLAGS;
                }
#endif
            }
            break;

        /****************************NORMAL*************************************/
        case BMS_STATEMACH_NORMAL:
            BMS_SAVELASTSTATES();

            if (bms_state.substate == BMS_ENTRY) {
                DB_ReadBlock(&systemstate, DATA_BLOCK_ID_SYSTEMSTATE);
                systemstate.bms_state = BMS_STATEMACH_NORMAL;
                DB_WriteBlock(&systemstate, DATA_BLOCK_ID_SYSTEMSTATE);
                bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                bms_state.substate = BMS_CHECK_ERROR_FLAGS;
                break;
            } else if (bms_state.substate == BMS_CHECK_ERROR_FLAGS) {
                if (BMS_CheckAnyErrorFlagSet() == E_NOT_OK) {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.state = BMS_STATEMACH_ERROR;
                    bms_state.substate = BMS_ENTRY;
                    break;
                } else {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.substate = BMS_CHECK_STATE_REQUESTS;
                    break;
                }
            } else if (bms_state.substate == BMS_CHECK_STATE_REQUESTS) {
                if (BMS_CheckCANRequests() == BMS_REQ_ID_STANDBY) {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.state = BMS_STATEMACH_STANDBY;
                    bms_state.substate = BMS_ENTRY;
                    break;
                } else {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.substate = BMS_CHECK_ERROR_FLAGS;
                    break;
                }
            }
            break;

            /****************************CHARGE_PRECHARGE*************************************/
            case BMS_STATEMACH_CHARGE_PRECHARGE:
                BMS_SAVELASTSTATES();

                if (bms_state.substate == BMS_ENTRY){
                    BAL_SetStateRequest(BAL_STATE_NOBALANCING_REQUEST);
                    DB_ReadBlock(&systemstate, DATA_BLOCK_ID_SYSTEMSTATE);
                    systemstate.bms_state = BMS_STATEMACH_CHARGE_PRECHARGE;
                    DB_WriteBlock(&systemstate, DATA_BLOCK_ID_SYSTEMSTATE);
#if BUILD_MODULE_ENABLE_CONTACTOR == 1
                    CONT_SetStateRequest(CONT_STATE_CHARGE_REQUEST);
#endif
                    bms_state.substate = BMS_CHECK_ERROR_FLAGS;
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    break;
                }
                else if (bms_state.substate == BMS_CHECK_ERROR_FLAGS){
                    if (BMS_CheckAnyErrorFlagSet() == E_NOT_OK){
                        bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                        bms_state.state = BMS_STATEMACH_ERROR;
                        bms_state.substate = BMS_ENTRY;
                        break;
                    } else{
                        bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                        bms_state.substate = BMS_CHECK_STATE_REQUESTS;
                        break;
                    }
                } else if (bms_state.substate == BMS_CHECK_STATE_REQUESTS){
                    if (BMS_CheckCANRequests() == BMS_REQ_ID_STANDBY){
                        bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                        bms_state.state = BMS_STATEMACH_STANDBY;
                        bms_state.substate = BMS_ENTRY;
                        break;
                    } else {
                        bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
#if BUILD_MODULE_ENABLE_CONTACTOR == 1
                        bms_state.substate = BMS_CHECK_CONTACTOR_CHARGE_STATE;
#else
                        bms_state.state = BMS_STATEMACH_CHARGE;
                        bms_state.substate = BMS_ENTRY;
#endif
                        break;
                    }
#if BUILD_MODULE_ENABLE_CONTACTOR == 1
                } else if (bms_state.substate == BMS_CHECK_CONTACTOR_CHARGE_STATE){
                    contstate = CONT_GetState();
                    if (contstate == CONT_STATEMACH_CHARGE){
                        bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                        bms_state.state = BMS_STATEMACH_CHARGE;
                        bms_state.substate = BMS_ENTRY;
                        break;
                    } else if (contstate == CONT_STATEMACH_ERROR){
                        bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                        bms_state.state = BMS_STATEMACH_ERROR;
                        bms_state.substate = BMS_ENTRY;
                        break;
                    } else{
                        bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                        bms_state.substate = BMS_CHECK_ERROR_FLAGS;
                    }
#endif
                }
                break;




            /****************************CHARGE*************************************/
            case BMS_STATEMACH_CHARGE:
                BMS_SAVELASTSTATES();

                if (bms_state.substate == BMS_ENTRY){
                    DB_ReadBlock(&systemstate, DATA_BLOCK_ID_SYSTEMSTATE);
                    systemstate.bms_state = BMS_STATEMACH_CHARGE;
                    DB_WriteBlock(&systemstate, DATA_BLOCK_ID_SYSTEMSTATE);
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.substate = BMS_CHECK_ERROR_FLAGS;
                    break;
                } else if (bms_state.substate == BMS_CHECK_ERROR_FLAGS){
                    if (BMS_CheckAnyErrorFlagSet() == E_NOT_OK){
                        bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                        bms_state.state = BMS_STATEMACH_ERROR;
                        bms_state.substate = BMS_ENTRY;
                        break;
                    } else{
                        bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                        bms_state.substate = BMS_CHECK_STATE_REQUESTS;
                        break;
                    }
                } else if (bms_state.substate == BMS_CHECK_STATE_REQUESTS){
                    if (BMS_CheckCANRequests() == BMS_REQ_ID_STANDBY){
                        bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                        bms_state.state = BMS_STATEMACH_STANDBY;
                        bms_state.substate = BMS_ENTRY;
                        break;
                    } else {
                        bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                        bms_state.substate = BMS_CHECK_ERROR_FLAGS;
                        break;
                    }
                }
                break;

        /****************************ERROR*************************************/
        case BMS_STATEMACH_ERROR:
            BMS_SAVELASTSTATES();

            if (bms_state.substate == BMS_ENTRY) {
                BAL_SetStateRequest(BAL_STATE_NOBALANCING_REQUEST);
#if BUILD_MODULE_ENABLE_CONTACTOR == 1
                CONT_SetStateRequest(CONT_STATE_ERROR_REQUEST);
#endif
                bms_state.timer = BMS_STATEMACH_MEDIUMTIME_MS;
#if BUILD_MODULE_ENABLE_ILCK == 1
                bms_state.substate = BMS_OPEN_INTERLOCK;
#else
                bms_state.substate = BMS_CHECK_ERROR_FLAGS;
#endif
                DB_ReadBlock(&systemstate, DATA_BLOCK_ID_SYSTEMSTATE);
                systemstate.bms_state = BMS_STATEMACH_ERROR;
                DB_WriteBlock(&systemstate, DATA_BLOCK_ID_SYSTEMSTATE);
                break;
#if BUILD_MODULE_ENABLE_ILCK == 1
            } else if (bms_state.substate == BMS_OPEN_INTERLOCK) {
                ILCK_SetStateRequest(ILCK_STATE_OPEN_REQUEST);
                bms_state.timer = BMS_STATEMACH_VERYLONGTIME_MS;
                bms_state.substate = BMS_CHECK_ERROR_FLAGS;
                break;
#endif
            } else if (bms_state.substate == BMS_CHECK_ERROR_FLAGS) {
                if (BMS_CheckAnyErrorFlagSet() == E_NOT_OK) {
                    /* we stay already in requested state, nothing to do */
                } else {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.substate = BMS_CHECK_STATE_REQUESTS;
                    break;
                }
            } else if (bms_state.substate == BMS_CHECK_STATE_REQUESTS) {
                if (BMS_CheckCANRequests() == BMS_REQ_ID_STANDBY) {
#if BUILD_MODULE_ENABLE_ILCK == 1
                    ILCK_SetStateRequest(ILCK_STATE_CLOSE_REQUEST);
                    bms_state.substate = BMS_CHECK_INTERLOCK_CLOSE_AFTER_ERROR;
#else
                    bms_state.state = BMS_STATEMACH_STANDBY;
                    bms_state.substate = BMS_ENTRY;
#endif
                    bms_state.timer = BMS_STATEMACH_MEDIUMTIME_MS;
                    break;
                } else {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.substate = BMS_CHECK_ERROR_FLAGS;
                    break;
                }
#if BUILD_MODULE_ENABLE_ILCK == 1
            } else if (bms_state.substate == BMS_CHECK_INTERLOCK_CLOSE_AFTER_ERROR) {
                if (ILCK_GetInterlockFeedback() == ILCK_SWITCH_ON) {
                    /* TODO: check */
                    BAL_SetStateRequest(BAL_STATE_ALLOWBALANCING_REQUEST);
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.state = BMS_STATEMACH_STANDBY;
                    bms_state.substate = BMS_ENTRY;
                    break;
                } else {
                    bms_state.timer = BMS_STATEMACH_SHORTTIME_MS;
                    bms_state.substate = BMS_CHECK_ERROR_FLAGS;
                    break;
                }
#endif
            }
            break;
        default:
            break;
    }  /* end switch(bms_state.state) */

    bms_state.triggerentry--;
    bms_state.counter++;
}

/*================== Static functions =====================================*/
/*
 * @brief   Checks the state requests made to the BMS state machine
 *
 * @details Checks of the state request in the database and sets this value as return value
 *
 * @return  requested state
 */
static uint8_t BMS_CheckCANRequests(void) {
    uint8_t retVal = BMS_REQ_ID_NOREQ;
    DATA_BLOCK_STATEREQUEST_s request;

    DB_ReadBlock(&request, DATA_BLOCK_ID_STATEREQUEST);

    if (request.state_request == BMS_REQ_ID_STANDBY) {
        retVal = BMS_REQ_ID_STANDBY;
    } else if (request.state_request == BMS_REQ_ID_NORMAL) {
        retVal = BMS_REQ_ID_NORMAL;
    }
#if BS_SEPARATE_POWERLINES == 1
    else if (request.state_request == BMS_REQ_ID_CHARGE){
        retVal = BMS_REQ_ID_CHARGE;
    }
#endif /*  BS_SEPARATE_POWERLINES == 1 */

    return retVal;
}

/**
 * @brief   checks the abidance by the safe operating area
 *
 * @details verify for cell voltage measurements (U), if minimum and maximum values are out of range
 */
static void BMS_CheckVoltages(void) {
    DATA_BLOCK_MINMAX_s minmax;

    DB_ReadBlock(&minmax, DATA_BLOCK_ID_MINMAX);
    uint16_t vol_max = minmax.voltage_max;
    uint16_t vol_min = minmax.voltage_min;


    if (vol_max >= BC_VOLTMAX_MOL) {
        /* Over voltage maximum operating limit violated */
        DIAG_Handler(DIAG_CH_CELLVOLTAGE_OVERVOLTAGE_MOL, DIAG_EVENT_NOK, 0, NULL_PTR);
        if (vol_max >= BC_VOLTMAX_RSL) {
            /* Over voltage recommended safety limit violated */
            DIAG_Handler(DIAG_CH_CELLVOLTAGE_OVERVOLTAGE_RSL, DIAG_EVENT_NOK, 0, NULL_PTR);
            if (vol_max >= BC_VOLTMAX_MSL) {
                /* Over voltage maximum safety limit violated */
                DIAG_Handler(DIAG_CH_CELLVOLTAGE_OVERVOLTAGE_MSL, DIAG_EVENT_NOK, 0, NULL_PTR);
            }
        }
    }
    if (vol_max < BC_VOLTMAX_MSL) {
        /* over voltage maximum safety limit NOT violated */
        DIAG_Handler(DIAG_CH_CELLVOLTAGE_OVERVOLTAGE_MSL, DIAG_EVENT_OK, 0, NULL_PTR);
        if (vol_max < BC_VOLTMAX_RSL) {
            /* over voltage recommended safety limit NOT violated */
            DIAG_Handler(DIAG_CH_CELLVOLTAGE_OVERVOLTAGE_RSL, DIAG_EVENT_OK, 0, NULL_PTR);
            if (vol_max < BC_VOLTMAX_MOL) {
                /* over voltage maximum operating limit NOT violated */
                DIAG_Handler(DIAG_CH_CELLVOLTAGE_OVERVOLTAGE_MOL, DIAG_EVENT_OK, 0, NULL_PTR);
            }
        }
    }

    if (vol_min <= BC_VOLTMIN_MOL) {
        /* Under voltage maximum operating limit violated */
        DIAG_Handler(DIAG_CH_CELLVOLTAGE_UNDERVOLTAGE_MOL, DIAG_EVENT_NOK, 0, NULL_PTR);
        if (vol_min <= BC_VOLTMIN_RSL) {
            /* Under voltage recommended safety limit violated */
            DIAG_Handler(DIAG_CH_CELLVOLTAGE_UNDERVOLTAGE_RSL, DIAG_EVENT_NOK, 0, NULL_PTR);
            if (vol_min <= BC_VOLTMIN_MSL) {
                /* Under voltage maximum safety limit violated */
                DIAG_Handler(DIAG_CH_CELLVOLTAGE_UNDERVOLTAGE_MSL, DIAG_EVENT_NOK, 0, NULL_PTR);
            }
        }
    }
    if (vol_min > BC_VOLTMIN_MSL) {
        /* under voltage maximum safety limit NOT violated */
        DIAG_Handler(DIAG_CH_CELLVOLTAGE_UNDERVOLTAGE_MSL, DIAG_EVENT_OK, 0, NULL_PTR);
        if (vol_min > BC_VOLTMIN_RSL) {
            /* under voltage recommended safety limit NOT violated */
            DIAG_Handler(DIAG_CH_CELLVOLTAGE_UNDERVOLTAGE_RSL, DIAG_EVENT_OK, 0, NULL_PTR);
            if (vol_min > BC_VOLTMIN_MOL) {
                /* under voltage maximum operating limit NOT violated */
                DIAG_Handler(DIAG_CH_CELLVOLTAGE_UNDERVOLTAGE_MOL, DIAG_EVENT_OK, 0, NULL_PTR);
            }
        }
    }
}


/**
 * @brief   checks the abidance by the safe operating area
 *
 * @details verify for cell temperature measurements (T), if minimum and maximum values are out of range
 */
static void BMS_CheckTemperatures(void) {
    DATA_BLOCK_MINMAX_s minmax;
    DATA_BLOCK_CURRENT_SENSOR_s curr_tab;

    DB_ReadBlock(&curr_tab, DATA_BLOCK_ID_CURRENT_SENSOR);
    DB_ReadBlock(&minmax, DATA_BLOCK_ID_MINMAX);

    float i_current = curr_tab.current;
    uint16_t temp_min = minmax.temperature_min;
    uint16_t temp_max = minmax.temperature_max;

    /* Over temperature check */
    if (BS_CheckCurrentValue_Direction(i_current) == BS_CURRENT_DISCHARGE) {
        /* Discharge */
        if (temp_max >= BC_TEMPMAX_DISCHARGE_MOL) {
            /* Over temperature maximum operating limit violated*/
            DIAG_Handler(DIAG_CH_TEMP_OVERTEMPERATURE_DISCHARGE_MOL, DIAG_EVENT_NOK, 0, NULL_PTR);
            if (temp_max >= BC_TEMPMAX_DISCHARGE_RSL) {
                /* Over temperature recommended safety limit violated*/
                DIAG_Handler(DIAG_CH_TEMP_OVERTEMPERATURE_DISCHARGE_RSL, DIAG_EVENT_NOK, 0, NULL_PTR);
                if (temp_max >= BC_TEMPMAX_DISCHARGE_MSL) {
                    /* Over temperature maximum safety limit violated */
                    DIAG_Handler(DIAG_CH_TEMP_OVERTEMPERATURE_DISCHARGE_MSL, DIAG_EVENT_NOK, 0, NULL_PTR);
                }
            }
        }
        if (temp_max < BC_TEMPMAX_DISCHARGE_MSL) {
            /* over temperature maximum safety limit NOT violated */
            DIAG_Handler(DIAG_CH_TEMP_OVERTEMPERATURE_DISCHARGE_MSL, DIAG_EVENT_OK, 0, NULL_PTR);
            if (temp_max < BC_TEMPMAX_DISCHARGE_RSL) {
                /* over temperature recommended safety limit NOT violated */
                DIAG_Handler(DIAG_CH_TEMP_OVERTEMPERATURE_DISCHARGE_RSL, DIAG_EVENT_OK, 0, NULL_PTR);
                if (temp_max < BC_TEMPMAX_DISCHARGE_MOL) {
                    /* over temperature maximum operating limit NOT violated */
                    DIAG_Handler(DIAG_CH_TEMP_OVERTEMPERATURE_DISCHARGE_MOL, DIAG_EVENT_OK, 0, NULL_PTR);
                }
            }
        }

    } else {
        /* Charge */
        if (temp_max >= BC_TEMPMAX_CHARGE_MOL) {
            /* Over temperature maximum operating limit violated */
            DIAG_Handler(DIAG_CH_TEMP_OVERTEMPERATURE_CHARGE_MOL, DIAG_EVENT_NOK, 0, NULL_PTR);
            if (temp_max >= BC_TEMPMAX_CHARGE_RSL) {
                /* Over temperature recommended safety limit violated */
                DIAG_Handler(DIAG_CH_TEMP_OVERTEMPERATURE_CHARGE_RSL, DIAG_EVENT_NOK, 0, NULL_PTR);
                /* Over temperature maximum safety limit violated */
                if (temp_max >= BC_TEMPMAX_CHARGE_MSL) {
                    DIAG_Handler(DIAG_CH_TEMP_OVERTEMPERATURE_CHARGE_MSL, DIAG_EVENT_NOK, 0, NULL_PTR);
                }
            }
        }
        if (temp_max < BC_TEMPMAX_CHARGE_MSL) {
            /* over temperature maximum safety limit NOT violated */
            DIAG_Handler(DIAG_CH_TEMP_OVERTEMPERATURE_CHARGE_MSL, DIAG_EVENT_OK, 0, NULL_PTR);
            if (temp_max < BC_TEMPMAX_CHARGE_RSL) {
                /* over temperature recommended safety limit NOT violated */
                DIAG_Handler(DIAG_CH_TEMP_OVERTEMPERATURE_CHARGE_RSL, DIAG_EVENT_OK, 0, NULL_PTR);
                if (temp_max < BC_TEMPMAX_CHARGE_MOL) {
                    /* over temperature maximum operating limit NOT violated*/
                    DIAG_Handler(DIAG_CH_TEMP_OVERTEMPERATURE_CHARGE_MOL, DIAG_EVENT_OK, 0, NULL_PTR);
                }
            }
        }
    }

    /* Under temperature check */
    if (BS_CheckCurrentValue_Direction(i_current) == BS_CURRENT_DISCHARGE) {
        /* Discharge */
        if (temp_min <= BC_TEMPMIN_DISCHARGE_MOL) {
            /* Under temperature maximum operating limit violated */
            DIAG_Handler(DIAG_CH_TEMP_UNDERTEMPERATURE_DISCHARGE_MOL, DIAG_EVENT_NOK, 0, NULL_PTR);
            if (temp_min <= BC_TEMPMIN_DISCHARGE_RSL) {
                /* Under temperature recommended safety limit violated*/
                DIAG_Handler(DIAG_CH_TEMP_UNDERTEMPERATURE_DISCHARGE_RSL, DIAG_EVENT_NOK, 0, NULL_PTR);
                if (temp_min <= BC_TEMPMIN_DISCHARGE_MSL) {
                    /* Under temperature maximum safety limit violated */
                    DIAG_Handler(DIAG_CH_TEMP_UNDERTEMPERATURE_DISCHARGE_MSL, DIAG_EVENT_NOK, 0, NULL_PTR);
                }
            }
        }
        if (temp_min > BC_TEMPMIN_DISCHARGE_MSL) {
            /* under temperature maximum safety limit NOT violated */
            DIAG_Handler(DIAG_CH_TEMP_UNDERTEMPERATURE_DISCHARGE_MSL, DIAG_EVENT_OK, 0, NULL_PTR);
            if (temp_min > BC_TEMPMIN_DISCHARGE_RSL) {
                /* under temperature recommended safety limit NOT violated */
                DIAG_Handler(DIAG_CH_TEMP_UNDERTEMPERATURE_DISCHARGE_RSL, DIAG_EVENT_OK, 0, NULL_PTR);
                if (temp_min > BC_TEMPMIN_DISCHARGE_MOL) {
                    /* under temperature maximum operating limit NOT violated*/
                    DIAG_Handler(DIAG_CH_TEMP_UNDERTEMPERATURE_DISCHARGE_MOL, DIAG_EVENT_OK, 0, NULL_PTR);
                }
            }
        }
    } else {
        /* Charge */
        if (temp_min <= BC_TEMPMIN_CHARGE_MOL) {
            /* Under temperature maximum operating limit violated */
            DIAG_Handler(DIAG_CH_TEMP_UNDERTEMPERATURE_CHARGE_MOL, DIAG_EVENT_NOK, 0, NULL_PTR);
            if (temp_min <= BC_TEMPMIN_CHARGE_RSL) {
                /* Under temperature recommended safety limit violated */
                DIAG_Handler(DIAG_CH_TEMP_UNDERTEMPERATURE_CHARGE_RSL, DIAG_EVENT_NOK, 0, NULL_PTR);
                if (temp_min <= BC_TEMPMIN_CHARGE_MSL) {
                    /* Under temperature maximum safety limit violated */
                    DIAG_Handler(DIAG_CH_TEMP_UNDERTEMPERATURE_CHARGE_MSL, DIAG_EVENT_NOK, 0, NULL_PTR);
                }
            }
        }
        if (temp_min > BC_TEMPMIN_CHARGE_MSL) {
            /* under temperature maximum safety limit NOT violated */
            DIAG_Handler(DIAG_CH_TEMP_UNDERTEMPERATURE_CHARGE_MSL, DIAG_EVENT_OK, 0, NULL_PTR);
            if (temp_min > BC_TEMPMIN_CHARGE_RSL) {
                /* under temperature recommended safety limit NOT violated */
                DIAG_Handler(DIAG_CH_TEMP_UNDERTEMPERATURE_CHARGE_RSL, DIAG_EVENT_OK, 0, NULL_PTR);
                if (temp_min > BC_TEMPMIN_CHARGE_MOL) {
                    /* under temperature maximum operating limit NOT violated*/
                    DIAG_Handler(DIAG_CH_TEMP_UNDERTEMPERATURE_CHARGE_MOL, DIAG_EVENT_OK, 0, NULL_PTR);
                }
            }
        }
    }
}



/**
 * @brief   checks the abidance by the safe operating area
 *
 * @details verify for cell current measurements (I), if minimum and maximum values are out of range
 */
static void BMS_CheckCurrent(void) {
    DATA_BLOCK_SOX_s sof_tab;
    DATA_BLOCK_CURRENT_SENSOR_s curr_tab;

    DB_ReadBlock(&sof_tab, DATA_BLOCK_ID_SOX);
    DB_ReadBlock(&curr_tab, DATA_BLOCK_ID_CURRENT_SENSOR);

    float i_current = curr_tab.current;

    float i_current_abs = 0.0;
    if (i_current < 0.0) {
        i_current_abs = - i_current;
    } else {
        i_current_abs = i_current;
    }

#if MEAS_TEST_CELL_SOF_LIMITS == TRUE
    if (((i_current < (-1000*(sof_tab.sof_continuous_charge))) ||
            (i_current > (1000*sof_tab.sof_continuous_discharge)))) {
        retVal = FALSE;
    }
#endif

    if (BS_CheckCurrentValue_Direction(i_current) == BS_CURRENT_CHARGE) {
        /* Charge */
        if (i_current_abs >= BC_CURRENTMAX_CHARGE_MOL) {
            /* Over current maximum operating limit violated */
            DIAG_Handler(DIAG_CH_OVERCURRENT_CHARGE_MOL, DIAG_EVENT_NOK, 0, NULL_PTR);
            if (i_current_abs >= BC_CURRENTMAX_CHARGE_RSL) {
                /* Over current recommended safety limit violated */
                DIAG_Handler(DIAG_CH_OVERCURRENT_CHARGE_RSL, DIAG_EVENT_NOK, 0, NULL_PTR);
                if (i_current_abs >= BC_CURRENTMAX_CHARGE_MSL) {
                    /* Over current maximum safety limit violated */
                    DIAG_Handler(DIAG_CH_OVERCURRENT_CHARGE_MSL, DIAG_EVENT_NOK, 0, NULL_PTR);
                }
            }
        }
        if (i_current_abs < BC_CURRENTMAX_CHARGE_MSL) {
            /* over current maximum safety limit NOT violated */
            DIAG_Handler(DIAG_CH_OVERCURRENT_CHARGE_MSL, DIAG_EVENT_OK, 0, NULL_PTR);
            if (i_current_abs < BC_CURRENTMAX_CHARGE_RSL) {
                /* over current recommended safety limit NOT violated */
                DIAG_Handler(DIAG_CH_OVERCURRENT_CHARGE_RSL, DIAG_EVENT_OK, 0, NULL_PTR);
                if (i_current_abs < BC_CURRENTMAX_CHARGE_MOL) {
                    /* over current maximum operating limit NOT violated */
                    DIAG_Handler(DIAG_CH_OVERCURRENT_CHARGE_MOL, DIAG_EVENT_OK, 0, NULL_PTR);
                }
            }
        }
    } else if (BS_CheckCurrentValue_Direction(i_current) == BS_CURRENT_DISCHARGE) {
        /* Discharge */
        if (i_current_abs >= BC_CURRENTMAX_DISCHARGE_MOL) {
            /* Over current maximum operating limit violated */
            DIAG_Handler(DIAG_CH_OVERCURRENT_DISCHARGE_MOL, DIAG_EVENT_NOK, 0, NULL_PTR);
            if (i_current_abs >= BC_CURRENTMAX_DISCHARGE_RSL) {
                /* Over current recommended safety limit violated */
                DIAG_Handler(DIAG_CH_OVERCURRENT_DISCHARGE_RSL, DIAG_EVENT_NOK, 0, NULL_PTR);
                if (i_current_abs >= BC_CURRENTMAX_DISCHARGE_MSL) {
                    /* Over current error */
                    DIAG_Handler(DIAG_CH_OVERCURRENT_DISCHARGE_MSL, DIAG_EVENT_NOK, 0, NULL_PTR);
                }
            }
        }
        if (i_current_abs < BC_CURRENTMAX_DISCHARGE_MSL) {
            /* over current maximum safety limit NOT violated */
            DIAG_Handler(DIAG_CH_OVERCURRENT_DISCHARGE_MSL, DIAG_EVENT_OK, 0, NULL_PTR);
            if (i_current_abs < BC_CURRENTMAX_CHARGE_RSL) {
                /* over current recommended safety limit NOT violated */
                DIAG_Handler(DIAG_CH_OVERCURRENT_DISCHARGE_RSL, DIAG_EVENT_OK, 0, NULL_PTR);
                if (i_current_abs < BC_CURRENTMAX_DISCHARGE_MOL) {
                    /* over current maximum operating limit NOT violated */
                    DIAG_Handler(DIAG_CH_OVERCURRENT_DISCHARGE_MOL, DIAG_EVENT_OK, 0, NULL_PTR);
                }
            }
        }
    } else {
        ; /* BS_CURRENT_NO_CURRENT */
    }

}

/**
 * @brief   FOR FUTURE COMPATIBILITY; DUMMY FUNCTION; DO NOT USE
 *
 * @details FOR FUTURE COMPATIBILITY; DUMMY FUNCTION; DO NOT USE
 */
static void BMS_CheckSlaveTemperatures(void) {
    ;
}

/**
 * @brief   Checks the error flags
 *
 * @details Checks all the error flags from the database and returns an error if at least one is set.
 *
 * @return  E_OK if no error flag is set, otherwise E_NOT_OK
 */
static STD_RETURN_TYPE_e BMS_CheckAnyErrorFlagSet(void) {
    STD_RETURN_TYPE_e retVal = E_OK;  /* is set to E_NOT_OK if error detected */
    DATA_BLOCK_ERRORSTATE_s error_flags;
    DATA_BLOCK_MSL_FLAG_s msl_flags;

    DB_ReadBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
    DB_ReadBlock(&msl_flags, DATA_BLOCK_ID_MSL);

    /* Check maximum safety limit flags */
    if (msl_flags.over_current_charge         == 1 ||
        msl_flags.over_current_discharge      == 1 ||
        msl_flags.over_voltage                == 1 ||
        msl_flags.under_voltage               == 1 ||
        msl_flags.over_temperature_charge     == 1 ||
        msl_flags.over_temperature_discharge  == 1 ||
        msl_flags.under_temperature_charge    == 1 ||
        msl_flags.under_temperature_discharge == 1) {

        retVal = E_NOT_OK;
        msl_flags.general_MSL = 1;
    } else {
        msl_flags.general_MSL = 0;
    }

    /* Check system error flags */
    if (error_flags.main_plus                 == 1 ||
        error_flags.main_minus                == 1 ||
        error_flags.precharge                 == 1 ||
        error_flags.charge_main_plus          == 1 ||
        error_flags.charge_main_minus         == 1 ||
        error_flags.charge_precharge          == 1 ||
        error_flags.interlock                 == 1 ||
        error_flags.crc_error                 == 1 ||
        error_flags.mux_error                 == 1 ||
        error_flags.spi_error                 == 1 ||
        error_flags.currentsensorresponding   == 1 ||
#if BMS_OPEN_CONTACTORS_ON_INSULATION_ERROR == TRUE
        error_flags.insulation_error          == 1 ||
#endif /* BMS_OPEN_CONTACTORS_ON_INSULATION_ERROR */
        error_flags.can_timing_cc             == 1 ||
        error_flags.can_timing                == 1 ) {

        retVal = E_NOT_OK;
        error_flags.general_error = 1;
    } else {
        error_flags.general_error = 0;
    }
    /* Save values back to DB */
    DB_WriteBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
    DB_WriteBlock(&msl_flags, DATA_BLOCK_ID_MSL);

    return retVal;
}
