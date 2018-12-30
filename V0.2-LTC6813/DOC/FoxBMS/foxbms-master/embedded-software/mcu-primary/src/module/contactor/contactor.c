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
 * @file    contactor.c
 * @author  foxBMS Team
 * @date    23.09.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  CONT
 *
 * @brief   Driver for the contactors.
 *
 */

/*================== Includes =============================================*/
#include "contactor.h"

#include "database.h"
#include "diag.h"
#include "FreeRTOS.h"
#include "task.h"

#if BUILD_MODULE_ENABLE_CONTACTOR == 1
/*================== Macros and Definitions ===============================*/

/**
 * used to locally copy the current-sensor value from the global database
 * current table
 */
static DATA_BLOCK_CURRENT_SENSOR_s cont_current_tab = {0};

/**
 * Saves the last state and the last substate
 */
#define CONT_SAVELASTSTATES()   cont_state.laststate = cont_state.state; \
                                cont_state.lastsubstate = cont_state.substate;

#define CONT_OPENALLCONTACTORS()   CONT_SwitchAllContactorsOff();

#define CONT_OPENMINUS()       CONT_SetContactorState(CONT_MAIN_MINUS, CONT_SWITCH_OFF);
#define CONT_CLOSEMINUS()       CONT_SetContactorState(CONT_MAIN_MINUS, CONT_SWITCH_ON);

#define CONT_OPENPLUS()       CONT_SetContactorState(CONT_MAIN_PLUS, CONT_SWITCH_OFF);
#define CONT_CLOSEPLUS()       CONT_SetContactorState(CONT_MAIN_PLUS, CONT_SWITCH_ON);

#define CONT_OPENPRECHARGE()       CONT_SetContactorState(CONT_PRECHARGE_PLUS, CONT_SWITCH_OFF);
#define CONT_CLOSEPRECHARGE()       CONT_SetContactorState(CONT_PRECHARGE_PLUS, CONT_SWITCH_ON);

#if BS_SEPARATE_POWERLINES == 1
#define CONT_OPENCHARGEMINUS()       CONT_SetContactorState(CONT_CHARGE_MAIN_MINUS, CONT_SWITCH_OFF);
#define CONT_CLOSECHARGEMINUS()       CONT_SetContactorState(CONT_CHARGE_MAIN_MINUS, CONT_SWITCH_ON);

#define CONT_OPENCHARGEPLUS()       CONT_SetContactorState(CONT_CHARGE_MAIN_PLUS, CONT_SWITCH_OFF);
#define CONT_CLOSECHARGEPLUS()       CONT_SetContactorState(CONT_CHARGE_MAIN_PLUS, CONT_SWITCH_ON);

#define CONT_OPENCHARGEPRECHARGE()       CONT_SetContactorState(CONT_CHARGE_PRECHARGE_PLUS, CONT_SWITCH_OFF);
#define CONT_CLOSECHARGEPRECHARGE()       CONT_SetContactorState(CONT_CHARGE_PRECHARGE_PLUS, CONT_SWITCH_ON);
#endif /* BS_SEPARATE_POWERLINES == 1 */

/*================== Constant and Variable Definitions ====================*/

/**
 * contains the state of the contactor state machine
 *
 */
static CONT_STATE_s cont_state = {
    .timer                  = 0,
    .statereq               = CONT_STATE_NO_REQUEST,
    .state                  = CONT_STATEMACH_UNINITIALIZED,
    .substate               = CONT_ENTRY,
    .laststate              = CONT_STATEMACH_UNINITIALIZED,
    .lastsubstate           = 0,
    .triggerentry           = 0,
    .ErrRequestCounter      = 0,
    .OscillationCounter     = 0,
    .PrechargeTryCounter    = 0,
    .PrechargeTimeOut       = 0,
    .counter                = 0,
};


/*================== Function Prototypes ==================================*/

static CONT_RETURN_TYPE_e CONT_CheckStateRequest(CONT_STATE_REQUEST_e statereq);
static CONT_STATE_REQUEST_e CONT_GetStateRequest(void);
static CONT_STATE_REQUEST_e CONT_TransferStateRequest(void);
static uint8_t CONT_CheckReEntrance(void);
static void CONT_CheckFeedback(void);

/*================== Function Implementations =============================*/

CONT_ELECTRICAL_STATE_TYPE_s CONT_GetContactorSetValue(CONT_NAMES_e contactor) {
    CONT_ELECTRICAL_STATE_TYPE_s contactorSetInformation = FALSE;
    taskENTER_CRITICAL();
    contactorSetInformation = cont_contactor_states[contactor].set;
    taskEXIT_CRITICAL();
    return contactorSetInformation;
}


CONT_ELECTRICAL_STATE_TYPE_s CONT_GetContactorFeedback(CONT_NAMES_e contactor) {

    CONT_ELECTRICAL_STATE_TYPE_s measuredContactorState = CONT_SWITCH_UNDEF;
    if (CONT_HAS_NO_FEEDBACK == cont_contactors_config[contactor].feedback_pin_type) {
            measuredContactorState = cont_contactor_states[contactor].set;
    } else {
        /* the contactor has a feedback pin, but it has to be differenced if the feedback pin is normally open or normally closed */
        if (CONT_FEEDBACK_NORMALLY_OPEN == cont_contactors_config[contactor].feedback_pin_type) {
            IO_PIN_STATE_e pinstate = IO_PIN_RESET;
            taskENTER_CRITICAL();
            pinstate = IO_ReadPin(cont_contactors_config[contactor].feedback_pin);
            taskEXIT_CRITICAL();
            if (IO_PIN_RESET == pinstate) {
                measuredContactorState = CONT_SWITCH_ON;
            } else if (IO_PIN_SET == pinstate) {
                measuredContactorState = CONT_SWITCH_OFF;
            } else {
                measuredContactorState = CONT_SWITCH_UNDEF;
            }
        }
        if (CONT_FEEDBACK_NORMALLY_CLOSED == cont_contactors_config[contactor].feedback_pin_type) {
            IO_PIN_STATE_e pinstate = IO_PIN_SET;
            taskENTER_CRITICAL();
            pinstate = IO_ReadPin(cont_contactors_config[contactor].feedback_pin);
            taskEXIT_CRITICAL();
            if (IO_PIN_SET == pinstate) {
                measuredContactorState = CONT_SWITCH_ON;
            } else if (IO_PIN_RESET == pinstate) {
                measuredContactorState = CONT_SWITCH_OFF;
            } else {
                measuredContactorState = CONT_SWITCH_UNDEF;
            }
        }
    }
    cont_contactor_states[contactor].feedback = measuredContactorState;
    return measuredContactorState;
}


STD_RETURN_TYPE_e CONT_AcquireContactorFeedbacks(void) {
    STD_RETURN_TYPE_e retVal = E_NOT_OK;
    taskENTER_CRITICAL();
    for (CONT_NAMES_e i = 0; i < (CONT_NAMES_e) cont_contactors_config_length; i++) {
        cont_contactor_states[i].feedback = CONT_GetContactorFeedback(i);
    }
    retVal = E_OK;
    taskEXIT_CRITICAL();
    return retVal;
}

STD_RETURN_TYPE_e CONT_SetContactorState(CONT_NAMES_e contactor, CONT_ELECTRICAL_STATE_TYPE_s requestedContactorState) {
    STD_RETURN_TYPE_e retVal = E_OK;

    if (requestedContactorState  ==  CONT_SWITCH_ON) {
        cont_contactor_states[contactor].set = CONT_SWITCH_ON;
        IO_WritePin(cont_contactors_config[contactor].control_pin, IO_PIN_SET);
        if(DIAG_HANDLER_RETURN_OK != DIAG_Handler(DIAG_CH_CONTACTOR_CLOSING, DIAG_EVENT_OK, (uint8_t) contactor, NULL)) {
            ;
        }
    }
    else if(requestedContactorState  ==  CONT_SWITCH_OFF) {
        DB_ReadBlock(&cont_current_tab, DATA_BLOCK_ID_CURRENT_SENSOR);
        float currentAtSwitchOff = cont_current_tab.current;
        if ( ((BAD_SWITCHOFF_CURRENT_POS < currentAtSwitchOff) && (0 < currentAtSwitchOff)) ||
             ((BAD_SWITCHOFF_CURRENT_NEG > currentAtSwitchOff) && (0 > currentAtSwitchOff))) {
            if(DIAG_HANDLER_RETURN_OK != DIAG_Handler(DIAG_CH_CONTACTOR_DAMAGED, DIAG_EVENT_NOK, (uint8_t) contactor, &currentAtSwitchOff)) {
                /* currently no error handling, just logging */
            }
        } else {
            if(DIAG_HANDLER_RETURN_OK != DIAG_Handler(DIAG_CH_CONTACTOR_OPENING, DIAG_EVENT_OK, (uint8_t) contactor, NULL)) {
                ;
            }
       }
        cont_contactor_states[contactor].set = CONT_SWITCH_OFF;
        IO_WritePin(cont_contactors_config[contactor].control_pin, IO_PIN_RESET);
    } else {
        retVal = E_NOT_OK;
    }

    return retVal;
}


STD_RETURN_TYPE_e CONT_SwitchAllContactorsOff(void) {
    STD_RETURN_TYPE_e retVal = E_NOT_OK;
    uint8_t offCounter = 0;
    STD_RETURN_TYPE_e successfullSet = E_NOT_OK;

    for (CONT_NAMES_e i = 0; i < (CONT_NAMES_e) cont_contactors_config_length; i++) {
        successfullSet = CONT_SetContactorState(i, CONT_SWITCH_OFF);
        if (E_OK == successfullSet) {
            offCounter = offCounter + 1;
        }
        successfullSet = E_NOT_OK;
    }

    if (BS_NR_OF_CONTACTORS == offCounter) {
        retVal = E_OK;
    } else {
        retVal = E_NOT_OK;
    }
    return retVal;
}



/**
 * @brief   re-entrance check of CONT state machine trigger function
 *
 * @details This function is not re-entrant and should only be called time- or event-triggered. It
 *          increments the triggerentry counter from the state variable ltc_state.
 *          It should never be called by two different processes, so if it is the case,
 *          triggerentry should never be higher than 0 when this function is called.
 *
 *
 * @return  0 if no further instance of the function is active, 0xff else
 *
 */
static uint8_t CONT_CheckReEntrance(void) {
    uint8_t retval = 0;

    taskENTER_CRITICAL();
    if (!cont_state.triggerentry) {
        cont_state.triggerentry++;
    } else {
        retval = 0xFF;
    }
    taskEXIT_CRITICAL();
    return retval;
}




/**
 * @brief   gets the current state request.
 *
 * @details This function is used in the functioning of the CONT state machine.
 *
 * @return  return the current pending state request
 */
static CONT_STATE_REQUEST_e CONT_GetStateRequest(void) {
    CONT_STATE_REQUEST_e retval = CONT_STATE_NO_REQUEST;

    taskENTER_CRITICAL();
    retval = cont_state.statereq;
    taskEXIT_CRITICAL();

    return retval;
}


/**
 * @brief   gets the current state.
 *
 * @details This function is used in the functioning of the CONT state machine.
 *
 * @return  current state, taken from CONT_STATEMACH_e
 */
CONT_STATEMACH_e CONT_GetState(void) {
    return (cont_state.state);
}



/**
 * @brief   transfers the current state request to the state machine.
 *
 * @details This function takes the current state request from cont_state and transfers it to the
 *          state machine. It resets the value from cont_state to CONT_STATE_NO_REQUEST
 *
 * @return  current state request, taken from CONT_STATE_REQUEST_e
 *
 */
static CONT_STATE_REQUEST_e CONT_TransferStateRequest(void) {
    CONT_STATE_REQUEST_e retval = CONT_STATE_NO_REQUEST;

    taskENTER_CRITICAL();
    retval    = cont_state.statereq;
    cont_state.statereq = CONT_STATE_NO_REQUEST;
    taskEXIT_CRITICAL();

    return (retval);
}


/**
 * @brief   sets the current state request of the state variable cont_state.
 *
 * @details This function is used to make a state request to the state machine,e.g, start voltage
 *          measurement, read result of voltage measurement, re-initialization.
 *          It calls CONT_CheckStateRequest() to check if the request is valid. The state request
 *          is rejected if is not valid. The result of the check is returned immediately, so that
 *          the requester can act in case it made a non-valid state request.
 *
 * @param   state request to set
 *
 * @return  CONT_OK if a state request was made, CONT_STATE_NO_REQUEST if no state request was made
 */
CONT_RETURN_TYPE_e CONT_SetStateRequest(CONT_STATE_REQUEST_e statereq) {
    CONT_RETURN_TYPE_e retVal = CONT_STATE_NO_REQUEST;

    taskENTER_CRITICAL();
    retVal = CONT_CheckStateRequest(statereq);

    if (retVal == CONT_OK) {
            cont_state.statereq   = statereq;
    }
    taskEXIT_CRITICAL();

    return retVal;
}



/**
 * @brief   checks the state requests that are made.
 *
 * @details This function checks the validity of the state requests. The results of the checked is
 *          returned immediately.
 *
 * @param   statereq    state request to be checked
 *
 * @return  result of the state request that was made, taken from (type: CONT_RETURN_TYPE_e)
 */
static CONT_RETURN_TYPE_e CONT_CheckStateRequest(CONT_STATE_REQUEST_e statereq) {
    if (statereq == CONT_STATE_ERROR_REQUEST) {
        return CONT_OK;
    }

    if (cont_state.statereq == CONT_STATE_NO_REQUEST) {
        /* init only allowed from the uninitialized state */
        if (statereq == CONT_STATE_INIT_REQUEST) {
            if (cont_state.state == CONT_STATEMACH_UNINITIALIZED) {
                return CONT_OK;
            } else {
                return CONT_ALREADY_INITIALIZED;
            }
        }

        if( (statereq == CONT_STATE_STANDBY_REQUEST) || (statereq == CONT_STATE_NORMAL_REQUEST) || (statereq == CONT_STATE_CHARGE_REQUEST)){
            return CONT_OK;
        }
        else if( statereq == CONT_STATE_NORMAL_REQUEST ){
            if (cont_state.state==CONT_STATEMACH_CHARGE_PRECHARGE || cont_state.state==CONT_STATEMACH_CHARGE) {
                return CONT_REQUEST_IMPOSSIBLE;
            } else {
                return CONT_OK;
            }
        }
        else if( statereq == CONT_STATE_CHARGE_REQUEST ){
            if (cont_state.state==CONT_STATEMACH_PRECHARGE || cont_state.state==CONT_STATEMACH_NORMAL) {
                return CONT_REQUEST_IMPOSSIBLE;
            } else {
                return CONT_OK;
            }
        } else {
            return CONT_ILLEGAL_REQUEST;
        }
    } else {
        return CONT_REQUEST_PENDING;
    }
}


/**
 * @brief   trigger function for the CONT driver state machine.
 *
 * @details This function contains the sequence of events in the CONT state machine. It must be
 *          called time-triggered, every 1ms. It exits without effect, if the function call is
 *          a reentrance.
 */
void CONT_Trigger(void) {
    STD_RETURN_TYPE_e retVal = E_OK;
    CONT_STATE_REQUEST_e statereq = CONT_STATE_NO_REQUEST;

    if (CONT_CheckReEntrance()) {
        return;
    }

    DIAG_SysMonNotify(DIAG_SYSMON_CONT_ID, 0);  /* task is running, state = ok */

    if (cont_state.state != CONT_STATEMACH_UNINITIALIZED) {
        CONT_CheckFeedback();
    }

    if(cont_state.OscillationCounter>0) {
        cont_state.OscillationCounter--;
    }

    if(cont_state.PrechargeTimeOut > 0) {
        if(cont_state.PrechargeTimeOut > CONT_TASK_CYCLE_CONTEXT_MS) {
            cont_state.PrechargeTimeOut -= CONT_TASK_CYCLE_CONTEXT_MS;
        } else {
            cont_state.PrechargeTimeOut = 0;
        }
    }

    if(cont_state.timer) {
        if (cont_state.timer > CONT_TASK_CYCLE_CONTEXT_MS) {
            cont_state.timer -= CONT_TASK_CYCLE_CONTEXT_MS;
        } else {
            cont_state.timer = 0;
        }
        if(cont_state.timer) {
            cont_state.triggerentry--;
            return;    /* handle state machine only if timer has elapsed */
        }
    }


    switch (cont_state.state) {
        /****************************UNINITIALIZED***********************************/
        case CONT_STATEMACH_UNINITIALIZED:
            /* waiting for Initialization Request */
            statereq = CONT_TransferStateRequest();
            if (statereq == CONT_STATE_INIT_REQUEST) {
                CONT_SAVELASTSTATES();
                cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
                cont_state.state = CONT_STATEMACH_INITIALIZATION;
                cont_state.substate = CONT_ENTRY;
            } else if (statereq == CONT_STATE_NO_REQUEST) {
                /* no actual request pending */
            } else {
                cont_state.ErrRequestCounter++;   /* illegal request pending */
            }
            break;


        /****************************INITIALIZATION**********************************/
        case CONT_STATEMACH_INITIALIZATION:

            CONT_SAVELASTSTATES();
            CONT_OPENALLCONTACTORS();

            cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
            cont_state.state = CONT_STATEMACH_INITIALIZED;
            cont_state.substate = CONT_ENTRY;

            break;

        /****************************INITIALIZED*************************************/
        case CONT_STATEMACH_INITIALIZED:
            CONT_SAVELASTSTATES();
            cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
            cont_state.state = CONT_STATEMACH_IDLE;
            cont_state.substate = CONT_ENTRY;
            break;

        /****************************IDLE*************************************/
        case CONT_STATEMACH_IDLE:
            CONT_SAVELASTSTATES();
            cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
            cont_state.state = CONT_STATEMACH_STANDBY;
            cont_state.substate = CONT_ENTRY;
            break;

        /****************************STANDBY*************************************/
        case CONT_STATEMACH_STANDBY:
            CONT_SAVELASTSTATES();

            /* first precharge process */
            if (cont_state.substate == CONT_ENTRY){
                cont_state.OscillationCounter = CONT_OSCILLATION_LIMIT;
                CONT_OPENPRECHARGE();
                #if BS_SEPARATE_POWERLINES == 1
                    CONT_OPENCHARGEPRECHARGE();
                #endif
                cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
                cont_state.substate = CONT_OPEN_FIRST_CONTACTOR;
                break;
            } else if (cont_state.substate == CONT_OPEN_FIRST_CONTACTOR){
                if (BS_CheckCurrent_Direction() == BS_CURRENT_DISCHARGE){
                    CONT_OPENPLUS();
                    #if BS_SEPARATE_POWERLINES == 1
                        CONT_OPENCHARGEPLUS();
                    #endif
                    cont_state.timer = CONT_DELAY_BETWEEN_OPENING_CONTACTORS_MS;
                    cont_state.substate = CONT_OPEN_SECOND_CONTACTOR_MINUS;
                } else{
                    CONT_OPENMINUS();
                    #if BS_SEPARATE_POWERLINES == 1
                        CONT_OPENCHARGEMINUS();
                    #endif
                    cont_state.timer = CONT_DELAY_BETWEEN_OPENING_CONTACTORS_MS;
                    cont_state.substate = CONT_OPEN_SECOND_CONTACTOR_PLUS;
                }
                break;
            } else if (cont_state.substate == CONT_OPEN_SECOND_CONTACTOR_MINUS){
                CONT_OPENMINUS();
                #if BS_SEPARATE_POWERLINES == 1
                    CONT_OPENCHARGEMINUS();
                #endif
                cont_state.timer = CONT_DELAY_AFTER_OPENING_SECOND_CONTACTORS_MS;
                cont_state.substate = CONT_STANDBY;
                break;
            } else if (cont_state.substate == CONT_OPEN_SECOND_CONTACTOR_PLUS){
                CONT_OPENPLUS();
                #if BS_SEPARATE_POWERLINES == 1
                    CONT_OPENCHARGEPLUS();
                #endif
                cont_state.timer = CONT_DELAY_AFTER_OPENING_SECOND_CONTACTORS_MS;
                cont_state.substate = CONT_STANDBY;
                break;
            } else if (cont_state.substate == CONT_STANDBY) {
                /* when process done, look for requests */
                statereq = CONT_TransferStateRequest();
                if (statereq == CONT_STATE_STANDBY_REQUEST) {
                    /* we stay already in requested state, nothing to do */
                } else if (statereq == CONT_STATE_NORMAL_REQUEST) {
                    CONT_SAVELASTSTATES();
                    cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
                    cont_state.state = CONT_STATEMACH_PRECHARGE;
                    cont_state.substate = CONT_ENTRY;
                }
#if BS_SEPARATE_POWERLINES == 1
                else if( statereq == CONT_STATE_CHARGE_REQUEST) {
                    CONT_SAVELASTSTATES();
                    cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
                    cont_state.state = CONT_STATEMACH_CHARGE_PRECHARGE;
                    cont_state.substate = CONT_ENTRY;
                }
#endif /* BS_SEPARATE_POWERLINES == 1 */
                else if(statereq == CONT_STATE_ERROR_REQUEST) {
                    CONT_SAVELASTSTATES();
                    cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
                    cont_state.state = CONT_STATEMACH_ERROR;
                    cont_state.substate = CONT_ENTRY;
                } else if (statereq == CONT_STATE_NO_REQUEST) {
                    /* no actual request pending */
                } else {
                    cont_state.ErrRequestCounter++;  /* illegal request pending */
                }
                break;
            }
            break;

        /****************************PRECHARGE*************************************/
        case CONT_STATEMACH_PRECHARGE:
            CONT_SAVELASTSTATES();
            /*  check state requests */
            statereq = CONT_TransferStateRequest();
            if (statereq == CONT_STATE_ERROR_REQUEST) {
                CONT_SAVELASTSTATES();
                cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
                cont_state.state = CONT_STATEMACH_ERROR;
                cont_state.substate = CONT_ENTRY;
                break;
            }
            if (statereq == CONT_STATE_STANDBY_REQUEST) {
                CONT_SAVELASTSTATES();
                cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
                cont_state.state = CONT_STATEMACH_STANDBY;
                cont_state.substate = CONT_ENTRY;
                break;
            }

            /* precharge process, can be interrupted anytimeby the requests above */
            if (cont_state.substate == CONT_ENTRY){
                if (cont_state.OscillationCounter > 0){
                    cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
                    break;
                } else {
                    cont_state.PrechargeTryCounter = 0;
                    cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
                    cont_state.substate = CONT_PRECHARGE_CLOSE_MINUS;
                    break;
                }
            } else if (cont_state.substate == CONT_PRECHARGE_CLOSE_MINUS) {
                cont_state.PrechargeTryCounter++;
                cont_state.PrechargeTimeOut = CONT_PRECHARGE_TIMEOUT_MS;
                CONT_CLOSEMINUS();
                cont_state.timer = CONT_STATEMACH_WAIT_AFTER_CLOSING_MINUS_MS;
                cont_state.substate = CONT_PRECHARGE_CLOSE_PRECHARGE;
                break;
            } else if (cont_state.substate == CONT_PRECHARGE_CLOSE_PRECHARGE) {
                CONT_CLOSEPRECHARGE();
                cont_state.timer = CONT_STATEMACH_WAIT_AFTER_CLOSING_PRECHARGE_MS;
                cont_state.substate = CONT_PRECHARGE_CHECK_VOLTAGES;
                break;
            } else if (cont_state.substate == CONT_PRECHARGE_CHECK_VOLTAGES) {
                retVal = CONT_CheckPrecharge(CONT_POWERLINE_NORMAL);
                if (retVal == E_OK) {
                    CONT_CLOSEPLUS();
                    cont_state.timer = CONT_STATEMACH_WAIT_AFTER_CLOSING_PLUS_MS;
                    cont_state.substate = CONT_PRECHARGE_OPEN_PRECHARGE;
                    break;
                } else if (cont_state.PrechargeTimeOut > 0) {
                    break;
                } else {
                    if (cont_state.PrechargeTryCounter < CONT_PRECHARGE_TRIES) {
                        CONT_OPENALLCONTACTORS();
                        cont_state.timer = CONT_STATEMACH_TIMEAFTERPRECHARGEFAIL_MS;
                        cont_state.substate = CONT_PRECHARGE_CLOSE_MINUS;
                        break;
                    } else {
                        CONT_OPENALLCONTACTORS();
                        cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
                        cont_state.state = CONT_STATEMACH_ERROR;
                        cont_state.substate = CONT_ENTRY;
                        break;
                    }
                }
            } else if (cont_state.substate == CONT_PRECHARGE_OPEN_PRECHARGE) {
                CONT_OPENPRECHARGE();
                cont_state.timer = CONT_STATEMACH_WAIT_AFTER_OPENING_PRECHARGE_MS;
                cont_state.state = CONT_STATEMACH_NORMAL;
                cont_state.substate = CONT_ENTRY;
                break;
            }

            break;

        /****************************NORMAL*************************************/
        case CONT_STATEMACH_NORMAL:
            CONT_SAVELASTSTATES();
            statereq = CONT_TransferStateRequest();
            if (statereq == CONT_STATE_ERROR_REQUEST) {
                CONT_SAVELASTSTATES();
                cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
                cont_state.state = CONT_STATEMACH_ERROR;
                cont_state.substate = CONT_ENTRY;
                break;
            }
            if (statereq == CONT_STATE_STANDBY_REQUEST) {
                CONT_SAVELASTSTATES();
                cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
                cont_state.state = CONT_STATEMACH_STANDBY;
                cont_state.substate = CONT_ENTRY;
                break;
            }

            break;

#if BS_SEPARATE_POWERLINES == 1
        /****************************CHARGE_PRECHARGE*************************************/
        case CONT_STATEMACH_CHARGE_PRECHARGE:
            CONT_SAVELASTSTATES();

            /* check state requests */
            statereq=CONT_TransferStateRequest();
            if (statereq == CONT_STATE_ERROR_REQUEST){
                CONT_SAVELASTSTATES();
                cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
                cont_state.state = CONT_STATEMACH_ERROR;
                cont_state.substate = CONT_ENTRY;
                break;
            }
            if(statereq == CONT_STATE_STANDBY_REQUEST){
                CONT_SAVELASTSTATES();
                cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
                cont_state.state = CONT_STATEMACH_STANDBY;
                cont_state.substate = CONT_ENTRY;
                break;
            }

            /* precharge process, can be interrupted anytime by the requests above */
            if (cont_state.substate == CONT_ENTRY){
                if (cont_state.OscillationCounter > 0){
                    cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
                    break;
                } else{
                    cont_state.PrechargeTryCounter = 0;
                    cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
                    cont_state.substate = CONT_PRECHARGE_CLOSE_MINUS;
                    break;
                }
            } else if (cont_state.substate == CONT_PRECHARGE_CLOSE_MINUS){
                cont_state.PrechargeTryCounter++;
                cont_state.PrechargeTimeOut = CONT_CHARGE_PRECHARGE_TIMEOUT_MS;
                CONT_CLOSECHARGEMINUS();
                cont_state.timer = CONT_STATEMACH_CHARGE_WAIT_AFTER_CLOSING_MINUS_MS;
                cont_state.substate = CONT_PRECHARGE_CLOSE_PRECHARGE;
                break;
            }
            else if (cont_state.substate == CONT_PRECHARGE_CLOSE_PRECHARGE){
                CONT_CLOSECHARGEPRECHARGE();
                cont_state.timer = CONT_STATEMACH_CHARGE_WAIT_AFTER_CLOSING_PRECHARGE_MS;
                cont_state.substate = CONT_PRECHARGE_CHECK_VOLTAGES;
                break;
            } else if (cont_state.substate == CONT_PRECHARGE_CHECK_VOLTAGES){
                retVal = CONT_CheckPrecharge(CONT_POWERLINE_CHARGE);
                if (retVal == E_OK){
                    CONT_CLOSECHARGEPLUS();
                    cont_state.timer = CONT_STATEMACH_CHARGE_WAIT_AFTER_CLOSING_PLUS_MS;
                    cont_state.substate = CONT_PRECHARGE_OPEN_PRECHARGE;
                    break;
                } else if (cont_state.PrechargeTimeOut>0){
                    break;
                } else{
                    if (cont_state.PrechargeTryCounter < CONT_PRECHARGE_TRIES){
                        CONT_OPENALLCONTACTORS();
                        cont_state.timer = CONT_STATEMACH_TIMEAFTERPRECHARGEFAIL_MS;
                        cont_state.substate = CONT_PRECHARGE_CLOSE_MINUS;
                        break;
                    } else{
                        CONT_OPENALLCONTACTORS();
                        cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
                        cont_state.state = CONT_STATEMACH_ERROR;
                        cont_state.substate = CONT_ENTRY;
                        break;
                    }
                }
            }
            else if (cont_state.substate == CONT_PRECHARGE_OPEN_PRECHARGE){
                CONT_OPENCHARGEPRECHARGE();
                cont_state.timer = CONT_STATEMACH_WAIT_AFTER_OPENING_PRECHARGE_MS;
                cont_state.state = CONT_STATEMACH_CHARGE;
                cont_state.substate = CONT_ENTRY;
                break;
            }

            break;

        /****************************CHARGE*************************************/
        case CONT_STATEMACH_CHARGE:
            CONT_SAVELASTSTATES();

            statereq=CONT_TransferStateRequest();
            if (statereq == CONT_STATE_ERROR_REQUEST){
                CONT_SAVELASTSTATES();
                cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
                cont_state.state = CONT_STATEMACH_ERROR;
                cont_state.substate = CONT_ENTRY;
                break;
            }
            if(statereq == CONT_STATE_STANDBY_REQUEST){
                CONT_SAVELASTSTATES();
                cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
                cont_state.state = CONT_STATEMACH_STANDBY;
                cont_state.substate = CONT_ENTRY;
                break;
            }

            break;
#endif /* BS_SEPARATE_POWERLINES == 1 */

        /****************************ERROR*************************************/
        case CONT_STATEMACH_ERROR:
            CONT_SAVELASTSTATES();

            /* first error process */
            if (cont_state.substate == CONT_ENTRY){
                cont_state.OscillationCounter = CONT_OSCILLATION_LIMIT;
                CONT_OPENPRECHARGE();
                #if BS_SEPARATE_POWERLINES == 1
                    CONT_OPENCHARGEPRECHARGE();
                #endif
                cont_state.timer = CONT_DELAY_BETWEEN_OPENING_CONTACTORS_MS;
                cont_state.substate = CONT_OPEN_FIRST_CONTACTOR;
                break;

            } else if (cont_state.substate == CONT_OPEN_FIRST_CONTACTOR) {
                if (BS_CheckCurrent_Direction() == BS_CURRENT_DISCHARGE) {
                    CONT_OPENPLUS();
                    #if BS_SEPARATE_POWERLINES == 1
                        CONT_OPENCHARGEPLUS();
                    #endif
                    cont_state.timer = CONT_DELAY_BETWEEN_OPENING_CONTACTORS_MS;
                    cont_state.substate = CONT_OPEN_SECOND_CONTACTOR_MINUS;
                } else {
                    CONT_OPENMINUS();
                    #if BS_SEPARATE_POWERLINES == 1
                        CONT_OPENCHARGEMINUS();
                    #endif
                    cont_state.timer = CONT_DELAY_BETWEEN_OPENING_CONTACTORS_MS;
                    cont_state.substate = CONT_OPEN_SECOND_CONTACTOR_PLUS;
                }
                break;

            } else if (cont_state.substate == CONT_OPEN_SECOND_CONTACTOR_MINUS) {
                CONT_OPENMINUS();
                #if BS_SEPARATE_POWERLINES == 1
                    CONT_OPENCHARGEMINUS();
                #endif
                cont_state.timer = CONT_DELAY_AFTER_OPENING_SECOND_CONTACTORS_MS;
                cont_state.substate = CONT_ERROR;
                break;

            } else if (cont_state.substate == CONT_OPEN_SECOND_CONTACTOR_PLUS) {
                CONT_OPENPLUS();
                #if BS_SEPARATE_POWERLINES == 1
                    CONT_OPENCHARGEPLUS();
                #endif
                cont_state.timer = CONT_DELAY_AFTER_OPENING_SECOND_CONTACTORS_MS;
                cont_state.substate = CONT_ERROR;
                break;

            } else if (cont_state.substate == CONT_ERROR) {
                /* when process done, look for requests */
                statereq = CONT_TransferStateRequest();
                if (statereq == CONT_STATE_ERROR_REQUEST) {
                    /* we stay already in requested state, nothing to do */
                } else if (statereq == CONT_STATE_STANDBY_REQUEST) {
                    CONT_SAVELASTSTATES();
                    cont_state.timer = CONT_STATEMACH_SHORTTIME_MS;
                    cont_state.state = CONT_STATEMACH_STANDBY;
                    cont_state.substate = CONT_ENTRY;
                } else if (statereq == CONT_STATE_NO_REQUEST) {
                    /* no actual request pending */
                } else {
                    cont_state.ErrRequestCounter++;   /* illegal request pending */
                }
                break;
            }
            break;

        default:
            break;
    }  /* end switch(cont_state.state) */

    cont_state.triggerentry--;
    cont_state.counter++;
}

/**
 * @brief   checks the feedback of the contactors
 *
 * @details makes a DIAG entry for each contactor when the feedback does not match the set value
 */
void CONT_CheckFeedback(void) {
    DATA_BLOCK_CONTFEEDBACK_s contfeedback_tab;
    CONT_ELECTRICAL_STATE_TYPE_s feedback;
    uint16_t contactor_feedback_state = 0;

    DB_ReadBlock(&contfeedback_tab, DATA_BLOCK_ID_CONTFEEDBACK);

    for (CONT_NAMES_e i = 0; i < (CONT_NAMES_e) cont_contactors_config_length; i++) {
        feedback = CONT_GetContactorFeedback(i);

        switch(i){
            case CONT_MAIN_PLUS:
                contactor_feedback_state |= feedback << CONT_MAIN_PLUS;
                break;
            case CONT_MAIN_MINUS:
                contactor_feedback_state |= feedback << CONT_MAIN_MINUS;
                break;
            case CONT_PRECHARGE_PLUS:
                contactor_feedback_state |= feedback << CONT_PRECHARGE_PLUS;
                break;
#if BS_SEPARATE_POWERLINES == 1
            case CONT_CHARGE_MAIN_PLUS:
                contactor_feedback_state |= feedback << CONT_CHARGE_MAIN_PLUS;
                break;
            case CONT_CHARGE_MAIN_MINUS:
                contactor_feedback_state |= feedback << CONT_CHARGE_MAIN_MINUS;
                break;
            case CONT_CHARGE_PRECHARGE_PLUS:
                contactor_feedback_state |= feedback << CONT_CHARGE_PRECHARGE_PLUS;
                break;
#endif /* BS_SEPARATE_POWERLINES == 1 */
            default:
                break;
        }

        contfeedback_tab.contactor_feedback &= (~0x3F);
        contfeedback_tab.contactor_feedback |= contactor_feedback_state;

        if (feedback != CONT_GetContactorSetValue(i) ) {

            switch(i){
                case CONT_MAIN_PLUS:
                    DIAG_Handler(DIAG_CH_CONTACTOR_MAIN_PLUS_FEEDBACK,DIAG_EVENT_NOK,0, NULL_PTR);
                    break;
                case CONT_MAIN_MINUS:
                    DIAG_Handler(DIAG_CH_CONTACTOR_MAIN_MINUS_FEEDBACK,DIAG_EVENT_NOK,0, NULL_PTR);
                    break;
                case CONT_PRECHARGE_PLUS:
                    DIAG_Handler(DIAG_CH_CONTACTOR_PRECHARGE_FEEDBACK,DIAG_EVENT_NOK,0, NULL_PTR);
                    break;
#if BS_SEPARATE_POWERLINES == 1
                case CONT_CHARGE_MAIN_PLUS:
                    DIAG_Handler(DIAG_CH_CONTACTOR_CHARGE_MAIN_PLUS_FEEDBACK,DIAG_EVENT_NOK,0, NULL_PTR);
                    break;
                case CONT_CHARGE_MAIN_MINUS:
                    DIAG_Handler(DIAG_CH_CONTACTOR_CHARGE_MAIN_MINUS_FEEDBACK,DIAG_EVENT_NOK,0, NULL_PTR);
                    break;
                case CONT_CHARGE_PRECHARGE_PLUS:
                    DIAG_Handler(DIAG_CH_CONTACTOR_CHARGE_PRECHARGE_FEEDBACK,DIAG_EVENT_NOK,0, NULL_PTR);
                    break;
#endif /* BS_SEPARATE_POWERLINES == 1 */
                default:
                    break;
            }
        }
        else{
            switch(i){
                case CONT_MAIN_PLUS:
                    DIAG_Handler(DIAG_CH_CONTACTOR_MAIN_PLUS_FEEDBACK,DIAG_EVENT_OK,0, NULL_PTR);
                    break;
                case CONT_MAIN_MINUS:
                    DIAG_Handler(DIAG_CH_CONTACTOR_MAIN_MINUS_FEEDBACK,DIAG_EVENT_OK,0, NULL_PTR);
                    break;
                case CONT_PRECHARGE_PLUS:
                    DIAG_Handler(DIAG_CH_CONTACTOR_PRECHARGE_FEEDBACK,DIAG_EVENT_OK,0, NULL_PTR);
                    break;
#if BS_SEPARATE_POWERLINES == 1
                case CONT_CHARGE_MAIN_PLUS:
                    DIAG_Handler(DIAG_CH_CONTACTOR_CHARGE_MAIN_PLUS_FEEDBACK,DIAG_EVENT_OK,0, NULL_PTR);
                    break;
                case CONT_CHARGE_MAIN_MINUS:
                    DIAG_Handler(DIAG_CH_CONTACTOR_CHARGE_MAIN_MINUS_FEEDBACK,DIAG_EVENT_OK,0, NULL_PTR);
                    break;
                case CONT_CHARGE_PRECHARGE_PLUS:
                    DIAG_Handler(DIAG_CH_CONTACTOR_CHARGE_PRECHARGE_FEEDBACK,DIAG_EVENT_OK,0, NULL_PTR);
                    break;
#endif /* BS_SEPARATE_POWERLINES == 1 */
                default:
                    break;
            }
        }
    }

    DB_WriteBlock(&contfeedback_tab, DATA_BLOCK_ID_CONTFEEDBACK);
}

#endif /* BUILD_MODULE_ENABLE_CONTACTOR */
