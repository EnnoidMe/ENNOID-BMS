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
 * @file    interlock.c
 * @author  foxBMS Team
 * @date    23.09.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  ILCK
 *
 * @brief   Driver for the interlock.
 *
 */

/*================== Includes =============================================*/
#include "interlock.h"

#include "database.h"
#include "diag.h"
#include "FreeRTOS.h"
#include "task.h"

#if BUILD_MODULE_ENABLE_ILCK == 1
/*================== Macros and Definitions ===============================*/

/**
 * Saves the last state and the last substate
 */
#define ILCK_SAVELASTSTATES()   ilck_state.laststate = ilck_state.state; \
                                ilck_state.lastsubstate = ilck_state.substate;

#define ILCK_CLOSEINTERLOCK()   ILCK_SwitchInterlockOn();
#define ILCK_OPENINTERLOCK()    ILCK_SwitchInterlockOff();

/*================== Constant and Variable Definitions ====================*/

/**
 * contains the state of the contactor state machine
 *
 */
static ILCK_STATE_s ilck_state = {
    .timer                  = 0,
    .statereq               = ILCK_STATE_NO_REQUEST,
    .state                  = ILCK_STATEMACH_UNINITIALIZED,
    .substate               = ILCK_ENTRY,
    .laststate              = ILCK_STATEMACH_UNINITIALIZED,
    .lastsubstate           = 0,
    .triggerentry           = 0,
    .ErrRequestCounter      = 0,
    .counter                = 0,
};


/*================== Function Prototypes ==================================*/

static ILCK_RETURN_TYPE_e ILCK_CheckStateRequest(ILCK_STATE_REQUEST_e statereq);
static ILCK_STATE_REQUEST_e ILCK_GetStateRequest(void);
static ILCK_STATE_REQUEST_e ILCK_TransferStateRequest(void);
static uint8_t ILCK_CheckReEntrance(void);
static void ILCK_CheckFeedback(void);


/*================== Function Implementations =============================*/

ILCK_ELECTRICAL_STATE_TYPE_s ILCK_GetInterlockSetValue() {
    ILCK_ELECTRICAL_STATE_TYPE_s interlockSetInformation = FALSE;
    taskENTER_CRITICAL();
    interlockSetInformation = ilck_interlock_state.set;
    taskEXIT_CRITICAL();
    return interlockSetInformation;
}




ILCK_ELECTRICAL_STATE_TYPE_s ILCK_GetInterlockFeedback(void) {
    ILCK_ELECTRICAL_STATE_TYPE_s measuredInterlockState = ILCK_SWITCH_UNDEF;
    IO_PIN_STATE_e pinstate = IO_PIN_RESET;
    taskENTER_CRITICAL();
    pinstate = IO_ReadPin(ilck_interlock_config.feedback_pin);
    taskEXIT_CRITICAL();
    if (IO_PIN_SET == pinstate) {
        measuredInterlockState = ILCK_SWITCH_ON;
    } else if (IO_PIN_RESET == pinstate) {
        measuredInterlockState = ILCK_SWITCH_OFF;
    }
    ilck_interlock_state.feedback = measuredInterlockState;
    return measuredInterlockState;
}



STD_RETURN_TYPE_e ILCK_SetInterlockState(ILCK_ELECTRICAL_STATE_TYPE_s requestedInterlockState) {
    STD_RETURN_TYPE_e retVal = E_OK;

    if (requestedInterlockState == ILCK_SWITCH_ON) {
        ilck_interlock_state.set = ILCK_SWITCH_ON;
        IO_WritePin(ilck_interlock_config.control_pin, IO_PIN_SET);
    } else if (requestedInterlockState  ==  ILCK_SWITCH_OFF) {
        ilck_interlock_state.set = ILCK_SWITCH_OFF;
        IO_WritePin(ilck_interlock_config.control_pin, IO_PIN_RESET);
    } else {
        retVal = E_NOT_OK;
    }

    return retVal;
}



STD_RETURN_TYPE_e ILCK_SwitchInterlockOff(void) {
    STD_RETURN_TYPE_e retVal = E_NOT_OK;
    retVal = ILCK_SetInterlockState(ILCK_SWITCH_OFF);
    return retVal;
}


STD_RETURN_TYPE_e ILCK_SwitchInterlockOn(void) {
    STD_RETURN_TYPE_e retVal = E_NOT_OK;
    retVal = ILCK_SetInterlockState(ILCK_SWITCH_ON);
    return retVal;
}




/**
 * @brief   re-entrance check of ILCK state machine trigger function
 *
 * This function is not re-entrant and should only be called time- or event-triggered.
 * It increments the triggerentry counter from the state variable ilck_state.
 * It should never be called by two different processes, so if it is the case, triggerentry
 * should never be higher than 0 when this function is called.
 *
 * @return  retval  0 if no further instance of the function is active, 0xff else
 *
 */
static uint8_t ILCK_CheckReEntrance(void) {
    uint8_t retval = 0;

    taskENTER_CRITICAL();
    if (!ilck_state.triggerentry) {
        ilck_state.triggerentry++;
    } else {
        retval = 0xFF;    /* multiple calls of function */
    }
    taskEXIT_CRITICAL();

    return retval;
}




/**
 * @brief   gets the current state request.
 *
 * This function is used in the functioning of the ILCK state machine.
 *
 * @return  retval  current state request, taken from ILCK_STATE_REQUEST_e
 */
static ILCK_STATE_REQUEST_e ILCK_GetStateRequest(void) {
    ILCK_STATE_REQUEST_e retval = ILCK_STATE_NO_REQUEST;

    taskENTER_CRITICAL();
    retval    = ilck_state.statereq;
    taskEXIT_CRITICAL();

    return retval;
}


/**
 * @brief   gets the current state.
 *
 * This function is used in the functioning of the ILCK state machine.
 *
 * @return  current state, taken from ILCK_STATEMACH_e
 */
ILCK_STATEMACH_e ILCK_GetState(void) {
    return ilck_state.state;
}


/**
 * @brief   transfers the current state request to the state machine.
 *
 * This function takes the current state request from ilck_state and transfers it to the state machine.
 * It resets the value from ilck_state to ILCK_STATE_NO_REQUEST
 *
 * @return  retVal          current state request, taken from ILCK_STATE_REQUEST_e
 *
 */
static ILCK_STATE_REQUEST_e ILCK_TransferStateRequest(void) {
    ILCK_STATE_REQUEST_e retval = ILCK_STATE_NO_REQUEST;

    taskENTER_CRITICAL();
    retval    = ilck_state.statereq;
    ilck_state.statereq = ILCK_STATE_NO_REQUEST;
    taskEXIT_CRITICAL();

    return retval;
}


/**
 * @brief   sets the current state request of the state variable ilck_state.
 *
 * This function is used to make a state request to the state machine,e.g, start voltage measurement,
 * read result of voltage measurement, re-initialization
 * It calls ILCK_CheckStateRequest() to check if the request is valid.
 * The state request is rejected if is not valid.
 * The result of the check is returned immediately, so that the requester can act in case
 * it made a non-valid state request.
 *
 * @param   statereq                state request to set
 *
 * @return  retVal                  current state request, taken from ILCK_STATE_REQUEST_e
 */
ILCK_RETURN_TYPE_e ILCK_SetStateRequest(ILCK_STATE_REQUEST_e statereq) {
    ILCK_RETURN_TYPE_e retVal = ILCK_STATE_NO_REQUEST;

    taskENTER_CRITICAL();
    retVal = ILCK_CheckStateRequest(statereq);

    if (retVal == ILCK_OK) {
            ilck_state.statereq = statereq;
    }
    taskEXIT_CRITICAL();

    return retVal;
}


/**
 * @brief   checks the state requests that are made.
 *
 * This function checks the validity of the state requests.
 * The results of the checked is returned immediately.
 *
 * @param   statereq    state request to be checked
 *
 * @return              result of the state request that was made, taken from ILCK_RETURN_TYPE_e
 */
static ILCK_RETURN_TYPE_e ILCK_CheckStateRequest(ILCK_STATE_REQUEST_e statereq) {
    if (statereq == ILCK_STATE_ERROR_REQUEST) {
        return ILCK_OK;
    }

    if (ilck_state.statereq == ILCK_STATE_NO_REQUEST) {
        /* init only allowed from the uninitialized state */
        if (statereq == ILCK_STATE_INIT_REQUEST) {
            if (ilck_state.state == ILCK_STATEMACH_UNINITIALIZED) {
                return ILCK_OK;
            } else {
                return ILCK_ALREADY_INITIALIZED;
            }
        }

        if ((statereq == ILCK_STATE_OPEN_REQUEST) || (statereq == ILCK_STATE_CLOSE_REQUEST)) {
            return ILCK_OK;
        } else {
            return ILCK_ILLEGAL_REQUEST;
        }
    } else {
        return ILCK_REQUEST_PENDING;
    }
}


void ILCK_SetFeedbackIgnoreCounter(uint32_t ignores) {
    taskENTER_CRITICAL();
    ilck_state.counter = ignores;
    taskEXIT_CRITICAL();
}


/**
 * @brief   trigger function for the ILCK driver state machine.
 *
 * This function contains the sequence of events in the ILCK state machine.
 * It must be called time-triggered, every 1ms.
 */
void ILCK_Trigger(void) {
    ILCK_STATE_REQUEST_e statereq = ILCK_STATE_NO_REQUEST;

    /* Check re-entrance of function */
    if (ILCK_CheckReEntrance()) {
        return;
    }

    DIAG_SysMonNotify(DIAG_SYSMON_ILCK_ID,0);        /* task is running, state = ok */

    /****Happens every time the state machine is triggered**************/
    if (ilck_state.state != ILCK_STATEMACH_UNINITIALIZED) {
        if (ilck_state.state > 0) {
            ilck_state.counter--;
        } else {
            ILCK_CheckFeedback();
        }
    }

    if (ilck_state.timer) {
        if (--ilck_state.timer) {
            ilck_state.triggerentry--;
            return;    /* handle state machine only if timer has elapsed */
        }
    }


    switch (ilck_state.state) {
        /****************************UNINITIALIZED***********************************/
        case ILCK_STATEMACH_UNINITIALIZED:
            /* waiting for Initialization Request */
            statereq = ILCK_TransferStateRequest();
            if (statereq == ILCK_STATE_INIT_REQUEST) {
                ILCK_SAVELASTSTATES();
                ilck_state.timer = ILCK_STATEMACH_SHORTTIME_MS;
                ilck_state.state = ILCK_STATEMACH_INITIALIZATION;
                ilck_state.substate = ILCK_ENTRY;
            } else if (statereq == ILCK_STATE_NO_REQUEST) {
                /* no actual request pending   */
            } else {
                ilck_state.ErrRequestCounter++;   /* illegal request pending */
            }
            break;

        /****************************INITIALIZATION**********************************/
        case ILCK_STATEMACH_INITIALIZATION:
            ILCK_SAVELASTSTATES();
            ILCK_OPENINTERLOCK();

            ilck_state.timer = ILCK_STATEMACH_SHORTTIME_MS;
            ilck_state.state = ILCK_STATEMACH_INITIALIZED;
            ilck_state.substate = ILCK_ENTRY;
            break;

        /****************************INITIALIZED*************************************/
        case ILCK_STATEMACH_INITIALIZED:
            ILCK_SAVELASTSTATES();
            ilck_state.timer = ILCK_STATEMACH_SHORTTIME_MS;
            ilck_state.state = ILCK_STATEMACH_WAIT_FIRST_REQUEST;
            ilck_state.substate = ILCK_ENTRY;
            break;

        /****************************INITIALIZED*************************************/
        case ILCK_STATEMACH_WAIT_FIRST_REQUEST:
            ILCK_SAVELASTSTATES();
            statereq = ILCK_TransferStateRequest();
            if (statereq == ILCK_STATE_OPEN_REQUEST) {
                ilck_state.timer = ILCK_STATEMACH_SHORTTIME_MS;
                ilck_state.state = ILCK_STATEMACH_OPEN;
                ilck_state.substate = ILCK_ENTRY;
                break;
            } else if (statereq == ILCK_STATE_CLOSE_REQUEST) {
                ilck_state.timer = ILCK_STATEMACH_SHORTTIME_MS;
                ilck_state.state = ILCK_STATEMACH_CLOSED;
                ilck_state.substate = ILCK_ENTRY;
                break;
            } else {
                ilck_state.timer = ILCK_STATEMACH_SHORTTIME_MS;
                break;
            }
            break;

        /****************************OPEN*************************************/
        case ILCK_STATEMACH_OPEN:
            ILCK_SAVELASTSTATES();
            ILCK_OPENINTERLOCK();
            ilck_state.timer = ILCK_STATEMACH_SHORTTIME_MS;
            statereq = ILCK_TransferStateRequest();
            if (statereq == ILCK_STATE_CLOSE_REQUEST) {
                ilck_state.timer = ILCK_STATEMACH_SHORTTIME_MS;
                ilck_state.state = ILCK_STATEMACH_CLOSED;
                ilck_state.substate = ILCK_ENTRY;
                break;
            }
            break;

        /****************************CLOSED*************************************/
        case ILCK_STATEMACH_CLOSED:
            ILCK_SAVELASTSTATES();
            ILCK_CLOSEINTERLOCK();
            ilck_state.timer = ILCK_STATEMACH_SHORTTIME_MS;
            statereq = ILCK_TransferStateRequest();
            if (statereq == ILCK_STATE_OPEN_REQUEST) {
                ilck_state.timer = ILCK_STATEMACH_SHORTTIME_MS;
                ilck_state.state = ILCK_STATEMACH_OPEN;
                ilck_state.substate = ILCK_ENTRY;
                break;
            }
            break;

        default:
            break;
    }  /* end switch(ilck_state.state) */

    ilck_state.triggerentry--;
}


void ILCK_CheckFeedback(void) {
    DATA_BLOCK_ILCKFEEDBACK_s ilckfeedback_tab;
    uint8_t interlock_feedback = 0;

    interlock_feedback = ILCK_GetInterlockFeedback();

    ilckfeedback_tab.interlock_feedback = interlock_feedback;

    DB_WriteBlock(&ilckfeedback_tab, DATA_BLOCK_ID_ILCKFEEDBACK);

    if (interlock_feedback != ILCK_GetInterlockSetValue()) {
        DIAG_Handler(DIAG_CH_INTERLOCK_FEEDBACK, DIAG_EVENT_NOK, 0, NULL_PTR);
    } else {
        DIAG_Handler(DIAG_CH_INTERLOCK_FEEDBACK, DIAG_EVENT_OK, 0, NULL_PTR);
    }


}
#endif
