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
 * @file    sys.c
 * @author  foxBMS Team
 * @date    21.09.2015 (date of creation)
 * @ingroup ENGINE
 * @prefix  SYS
 *
 * @brief   Sys driver implementation
 */


/*================== Includes =============================================*/
#include "sys.h"

#include "bms.h"
#include "diag.h"
#include "meas.h"
#include "interlock.h"
#include "FreeRTOS.h"
#include "task.h"

/*================== Macros and Definitions ===============================*/

/**
 * Saves the last state and the last substate
 */
#define SYS_SAVELASTSTATES()    sys_state.laststate=sys_state.state; \
                                sys_state.lastsubstate = sys_state.substate

/*================== Constant and Variable Definitions ====================*/

/**
 * contains the state of the contactor state machine
 *
 */
static SYS_STATE_s sys_state = {
    .timer                  = 0,
    .statereq               = SYS_STATE_NO_REQUEST,
    .state                  = SYS_STATEMACH_UNINITIALIZED,
    .substate               = SYS_ENTRY,
    .laststate              = SYS_STATEMACH_UNINITIALIZED,
    .lastsubstate           = 0,
    .triggerentry           = 0,
    .ErrRequestCounter      = 0,
};

/*================== Function Prototypes ==================================*/

static SYS_RETURN_TYPE_e SYS_CheckStateRequest(SYS_STATE_REQUEST_e statereq);
static SYS_STATE_REQUEST_e SYS_GetStateRequest(void);
static SYS_STATE_REQUEST_e SYS_TransferStateRequest(void);
static uint8_t SYS_CheckReEntrance(void);

/*================== Function Implementations =============================*/

/**
 * @brief   re-entrance check of SYS state machine trigger function
 *
 * This function is not re-entrant and should only be called time- or event-triggered.
 * It increments the triggerentry counter from the state variable ltc_state.
 * It should never be called by two different processes, so if it is the case, triggerentry
 * should never be higher than 0 when this function is called.
 *
 *
 * @return  retval  0 if no further instance of the function is active, 0xff else
 *
 */
static uint8_t SYS_CheckReEntrance(void) {
    uint8_t retval = 0;

    taskENTER_CRITICAL();
    if (!sys_state.triggerentry) {
        sys_state.triggerentry++;
    } else {
        retval = 0xFF;  /* multiple calls of function */
    }
    taskEXIT_CRITICAL();

    return retval;
}




/**
 * @brief   gets the current state request.
 *
 * This function is used in the functioning of the SYS state machine.
 *
 * @return  retval  current state request, taken from SYS_STATE_REQUEST_e
 */
static SYS_STATE_REQUEST_e SYS_GetStateRequest(void) {
    SYS_STATE_REQUEST_e retval = SYS_STATE_NO_REQUEST;

    taskENTER_CRITICAL();
    retval    = sys_state.statereq;
    taskEXIT_CRITICAL();

    return (retval);
}


SYS_STATEMACH_e SYS_GetState(void) {
    return (sys_state.state);
}


/**
 * @brief   transfers the current state request to the state machine.
 *
 * This function takes the current state request from cont_state and transfers it to the state machine.
 * It resets the value from cont_state to SYS_STATE_NO_REQUEST
 *
 * @return  retVal          current state request, taken from SYS_STATE_REQUEST_e
 *
 */
static SYS_STATE_REQUEST_e SYS_TransferStateRequest(void) {
    SYS_STATE_REQUEST_e retval = SYS_STATE_NO_REQUEST;

    taskENTER_CRITICAL();
    retval    = sys_state.statereq;
    sys_state.statereq = SYS_STATE_NO_REQUEST;
    taskEXIT_CRITICAL();

    return (retval);
}



SYS_RETURN_TYPE_e SYS_SetStateRequest(SYS_STATE_REQUEST_e statereq) {
    SYS_RETURN_TYPE_e retVal = SYS_STATE_NO_REQUEST;

    taskENTER_CRITICAL();
    retVal = SYS_CheckStateRequest(statereq);

    if (retVal == SYS_OK) {
            sys_state.statereq  = statereq;
        }
    taskEXIT_CRITICAL();

    return (retVal);
}



/**
 * @brief   checks the state requests that are made.
 *
 * This function checks the validity of the state requests.
 * The results of the checked is returned immediately.
 *
 * @param   statereq    state request to be checked
 *
 * @return              result of the state request that was made, taken from SYS_RETURN_TYPE_e
 */
static SYS_RETURN_TYPE_e SYS_CheckStateRequest(SYS_STATE_REQUEST_e statereq) {
    if (statereq == SYS_STATE_ERROR_REQUEST) {
        return SYS_OK;
    }

    if (sys_state.statereq == SYS_STATE_NO_REQUEST) {
        /* init only allowed from the uninitialized state */
        if (statereq == SYS_STATE_INIT_REQUEST) {
            if (sys_state.state == SYS_STATEMACH_UNINITIALIZED) {
                return SYS_OK;
            } else {
                return SYS_ALREADY_INITIALIZED;
            }
        } else {
            return SYS_ILLEGAL_REQUEST;
        }
    } else {
        return SYS_REQUEST_PENDING;
    }
}


void SYS_Trigger(void) {
    /* STD_RETURN_TYPE_e retVal=E_OK; */
    SYS_STATE_REQUEST_e statereq = SYS_STATE_NO_REQUEST;
    ILCK_STATEMACH_e ilckstate = ILCK_STATEMACH_UNDEFINED;
    BMS_STATEMACH_e bmsstate = BMS_STATEMACH_UNDEFINED;


    DIAG_SysMonNotify(DIAG_SYSMON_SYS_ID, 0);  /* task is running, state = ok */
    /* Check re-entrance of function */
    if (SYS_CheckReEntrance()) {
        return;
    }

    if (sys_state.timer) {
        if (--sys_state.timer) {
            sys_state.triggerentry--;
            return;  /* handle state machine only if timer has elapsed */
        }
    }

    /****Happens every time the state machine is triggered**************/


    switch (sys_state.state) {
        /****************************UNINITIALIZED***********************************/
        case SYS_STATEMACH_UNINITIALIZED:
            /* waiting for Initialization Request */
            statereq = SYS_TransferStateRequest();
            if (statereq == SYS_STATE_INIT_REQUEST) {
                SYS_SAVELASTSTATES();
                sys_state.timer = SYS_STATEMACH_SHORTTIME_MS;
                sys_state.state = SYS_STATEMACH_INITIALIZATION;
                sys_state.substate = SYS_ENTRY;
            } else if (statereq == SYS_STATE_NO_REQUEST) {
                /* no actual request pending /*  */
            } else {
                sys_state.ErrRequestCounter++;   /* illegal request pending */
            }
            break;
        /****************************INITIALIZATION**********************************/
        case SYS_STATEMACH_INITIALIZATION:

            SYS_SAVELASTSTATES();
            /* Initializations done here */
            sys_state.timer = SYS_STATEMACH_SHORTTIME_MS;
            sys_state.state = SYS_STATEMACH_INITIALIZED;
            sys_state.substate = SYS_ENTRY;
            break;

        /****************************INITIALIZED*************************************/
        case SYS_STATEMACH_INITIALIZED:
            SYS_SAVELASTSTATES();
            MEAS_StartMeasurement();
            sys_state.timer = SYS_STATEMACH_SHORTTIME_MS;
            sys_state.state = SYS_STATEMACH_INITIALIZE_INTERLOCK;
            sys_state.substate = SYS_ENTRY;
            break;

        /****************************INITIALIZE INTERLOCK*************************************/
        case SYS_STATEMACH_INITIALIZE_INTERLOCK:
            SYS_SAVELASTSTATES();

            if (sys_state.substate == SYS_ENTRY) {
                ILCK_SetStateRequest(ILCK_STATE_INIT_REQUEST);
                sys_state.timer = SYS_STATEMACH_SHORTTIME_MS;
                sys_state.substate = SYS_WAIT_INITIALIZATION_INTERLOCK;
                sys_state.InitCounter = 0;
                break;
            } else if (sys_state.substate == SYS_WAIT_INITIALIZATION_INTERLOCK) {
                ilckstate = ILCK_GetState();
                if (ilckstate == ILCK_STATEMACH_WAIT_FIRST_REQUEST) {
                    ILCK_SetFeedbackIgnoreCounter(10);
                    ILCK_SetStateRequest(ILCK_STATE_CLOSE_REQUEST);
                    sys_state.timer = SYS_STATEMACH_SHORTTIME_MS;
                    sys_state.state = SYS_STATEMACH_INITIALIZE_MISC;
                    sys_state.substate = SYS_ENTRY;
                    break;
                } else {
                    if (sys_state.InitCounter > 1000) {
                        sys_state.timer = SYS_STATEMACH_SHORTTIME_MS;
                        sys_state.state = SYS_STATEMACH_ERROR;
                        sys_state.substate = SYS_ILCK_INIT_ERROR;
                        break;
                    }
                    sys_state.timer = SYS_STATEMACH_SHORTTIME_MS;
                    sys_state.InitCounter++;
                    break;
                }
            }

            break;


                /****************************INITIALIZED_MISC*************************************/
                case SYS_STATEMACH_INITIALIZE_MISC:
                    SYS_SAVELASTSTATES();


                    sys_state.timer = SYS_STATEMACH_MEDIUMTIME_MS;
                    sys_state.state = SYS_STATEMACH_INITIALIZE_BMS;
                    sys_state.substate = SYS_ENTRY;
                    break;

            /****************************INITIALIZE BMS*************************************/
            case SYS_STATEMACH_INITIALIZE_BMS:
                SYS_SAVELASTSTATES();

                if (sys_state.substate == SYS_ENTRY){
                    BMS_SetStateRequest(BMS_STATE_INIT_REQUEST);
                    sys_state.timer = SYS_STATEMACH_SHORTTIME_MS;
                    sys_state.substate = SYS_WAIT_INITIALIZATION_BMS;
                    sys_state.InitCounter = 0;
                    break;
                } else if (sys_state.substate == SYS_WAIT_INITIALIZATION_BMS) {
                    bmsstate = BMS_GetState();
                    if (bmsstate == BMS_STATEMACH_IDLE || bmsstate == BMS_STATEMACH_STANDBY) {
                        sys_state.timer = SYS_STATEMACH_SHORTTIME_MS;
                        sys_state.state = SYS_STATEMACH_RUNNING;
                        sys_state.substate = SYS_ENTRY;
                        break;
                    } else {
                        if (sys_state.InitCounter > 1000) {
                            sys_state.timer = SYS_STATEMACH_SHORTTIME_MS;
                            sys_state.state = SYS_STATEMACH_ERROR;
                            sys_state.substate = SYS_BMS_INIT_ERROR;
                            break;
                        }
                        sys_state.timer = SYS_STATEMACH_SHORTTIME_MS;
                        sys_state.InitCounter++;
                        break;
                    }
                }
                break;

        /****************************RUNNNIG*************************************/
        case SYS_STATEMACH_RUNNING:
            SYS_SAVELASTSTATES();
            sys_state.timer = SYS_STATEMACH_LONGTIME_MS;
            break;

        /****************************ERROR*************************************/
        case SYS_STATEMACH_ERROR:
            SYS_SAVELASTSTATES();
            sys_state.timer = SYS_STATEMACH_LONGTIME_MS;
            break;
    }  /* end switch(sys_state.state) */
    sys_state.triggerentry--;
}
