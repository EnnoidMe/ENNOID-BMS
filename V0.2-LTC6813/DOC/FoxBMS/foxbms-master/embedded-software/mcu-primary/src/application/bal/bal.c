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
 * @file    bal.c
 * @author  foxBMS Team
 * @date    26.02.2016 (date of creation)
 * @ingroup DRIVERS
 * @prefix  BAL
 *
 * @brief   Driver for the Balancing module
 *
 */

/*================== Includes =============================================*/
#include "bal.h"

#include "batterycell_cfg.h"
#include "database.h"
#include "FreeRTOS.h"
#include "sox.h"
#include "task.h"

/*================== Macros and Definitions ===============================*/

/**
 * Saves the last state and the last substate
 */
#define BAL_SAVELASTSTATES()    bal_state.laststate=bal_state.state; \
                                bal_state.lastsubstate = bal_state.substate


/*================== Constant and Variable Definitions ====================*/
static DATA_BLOCK_MINMAX_s bal_minmax;
static DATA_BLOCK_BALANCING_CONTROL_s bal_balancing;
static DATA_BLOCK_CELLVOLTAGE_s bal_cellvoltage;
DATA_BLOCK_STATEREQUEST_s bal_request;

/**
 * contains the state of the contactor state machine
 *
 */
static BAL_STATE_s bal_state = {
    .timer                         = 0,
    .statereq                      = BAL_STATE_NO_REQUEST,
    .state                         = BAL_STATEMACH_UNINITIALIZED,
    .substate                      = BAL_ENTRY,
    .laststate                     = BAL_STATEMACH_UNINITIALIZED,
    .lastsubstate                  = 0,
    .triggerentry                  = 0,
    .ErrRequestCounter             = 0,
    .active                        = FALSE,
    .resting                       = TRUE,
    .rest_timer                    = BAL_TIME_BEFORE_BALANCING_S*10,
    .balancing_threshold           = BAL_THRESHOLD_MV + BAL_HYSTERESIS_MV,
    .balancing_allowed             = TRUE,
    .balancing_global_allowed      = FALSE,
};


/*================== Function Prototypes ==================================*/

static BAL_RETURN_TYPE_e BAL_CheckStateRequest(BAL_STATE_REQUEST_e statereq);
static BAL_STATE_REQUEST_e BAL_GetStateRequest(void);
static BAL_STATE_REQUEST_e BAL_TransferStateRequest(void);
static uint8_t BAL_CheckReEntrance(void);

static void BAL_Init(void);
static void BAL_Deactivate(void);
#if BALANCING_VOLTAGE_BASED == TRUE
static uint8_t BAL_Activate_Balancing_Voltage(void);
#else
static uint8_t BAL_Check_Imbalances(void);
static void BAL_Compute_Imbalances(void);
static void BAL_Activate_Balancing_History(void);
#endif


/*================== Function Implementations =============================*/
static void BAL_Init(void) {

    DB_ReadBlock(&bal_balancing, DATA_BLOCK_ID_BALANCING_CONTROL_VALUES);
    bal_balancing.enable_balancing = 0;
    DB_WriteBlock(&bal_balancing, DATA_BLOCK_ID_BALANCING_CONTROL_VALUES);
}

static void BAL_Deactivate(void) {
    uint16_t i;

    DB_ReadBlock(&bal_balancing, DATA_BLOCK_ID_BALANCING_CONTROL_VALUES);

    for (i=0;i<BS_NR_OF_BAT_CELLS;i++) {
        bal_balancing.balancing_state[i] = 0;
        bal_balancing.delta_charge[i] = 0;
    }

    bal_balancing.enable_balancing = 0;
    bal_state.active = FALSE;

    bal_balancing.previous_timestamp = bal_balancing.timestamp;
    bal_balancing.timestamp = OS_getOSSysTick();
    DB_WriteBlock(&bal_balancing, DATA_BLOCK_ID_BALANCING_CONTROL_VALUES);

}

#if BALANCING_VOLTAGE_BASED == TRUE

static uint8_t BAL_Activate_Balancing_Voltage(void) {
    uint32_t i = 0;
    uint16_t min = 0;
    uint8_t finished = TRUE;

    DB_ReadBlock(&bal_balancing, DATA_BLOCK_ID_BALANCING_CONTROL_VALUES);
    DB_ReadBlock(&bal_cellvoltage, DATA_BLOCK_ID_CELLVOLTAGE);
    DB_ReadBlock(&bal_minmax, DATA_BLOCK_ID_MINMAX);


    min = bal_minmax.voltage_min;

    for (i=0;i<BS_NR_OF_BAT_CELLS;i++) {
        if ( bal_cellvoltage.voltage[i] > min+bal_state.balancing_threshold ) {
            bal_balancing.balancing_state[i] = 1;
            finished = FALSE;
            bal_state.active = TRUE;
            bal_balancing.enable_balancing = 1;
        } else {
            bal_balancing.balancing_state[i] = 0;
        }
    }

    bal_balancing.previous_timestamp = bal_balancing.timestamp;
    bal_balancing.timestamp = OS_getOSSysTick();
    DB_WriteBlock(&bal_balancing, DATA_BLOCK_ID_BALANCING_CONTROL_VALUES);

    return finished;

}

#else

static uint8_t BAL_Check_Imbalances(void) {
    uint16_t i;
    uint8_t retVal = FALSE;

    DB_ReadBlock(&bal_balancing, DATA_BLOCK_ID_BALANCING_CONTROL_VALUES);

    for (i=0;i<BS_NR_OF_BAT_CELLS;i++) {
        if ( bal_balancing.delta_charge[i] > 0 ) {
            retVal = TRUE;
        }
    }

    return retVal;

}

static void BAL_Compute_Imbalances(void) {
    uint16_t i=0;
    uint16_t voltageMin=0;
    uint16_t minVoltageIndex=0;
    float SOC = 0.0;
    uint32_t DOD = 0.0;
    uint32_t maxDOD = 0.0;

    DB_ReadBlock(&bal_balancing, DATA_BLOCK_ID_BALANCING_CONTROL_VALUES);
    DB_ReadBlock(&bal_cellvoltage, DATA_BLOCK_ID_CELLVOLTAGE);

    voltageMin = bal_cellvoltage.voltage[0];
    minVoltageIndex = 0;

    for (i=0;i<BS_NR_OF_BAT_CELLS;i++) {
        if ( bal_cellvoltage.voltage[i] <= voltageMin ) {
            voltageMin = bal_cellvoltage.voltage[i];
            minVoltageIndex = i;
        }
    }

    SOC = SOC_GetFromVoltage(((float)(bal_cellvoltage.voltage[minVoltageIndex]))/1000.0);
    maxDOD = BC_CAPACITY * (uint32_t)((1.0 - SOC) * 3600.0);
    bal_balancing.delta_charge[minVoltageIndex] = 0;

    for (i=0;i<BS_NR_OF_BAT_CELLS;i++) {
        if ( i != minVoltageIndex ) {
            if (bal_cellvoltage.voltage[i] >= voltageMin + bal_state.balancing_threshold ) {
                SOC = SOC_GetFromVoltage(((float)(bal_cellvoltage.voltage[i]))/1000.0);
                DOD = BC_CAPACITY * (uint32_t)((1.0 - SOC) * 3600.0);
                bal_balancing.delta_charge[i] = (maxDOD - DOD);
            }
        }
    }

    DB_WriteBlock(&bal_balancing, DATA_BLOCK_ID_BALANCING_CONTROL_VALUES);

}

static void BAL_Activate_Balancing_History(void) {
    uint16_t i;
    float cellBalancingCurrent;
    uint32_t difference;

    DB_ReadBlock(&bal_balancing, DATA_BLOCK_ID_BALANCING_CONTROL_VALUES);
    DB_ReadBlock(&bal_cellvoltage, DATA_BLOCK_ID_CELLVOLTAGE);

    for (i=0;i<BS_NR_OF_BAT_CELLS;i++) {
        if (bal_state.balancing_allowed == FALSE){
            bal_balancing.balancing_state[i] = 0;
        } else {
            if ( bal_balancing.delta_charge[i] > 0 ) {
                bal_balancing.balancing_state[i] = 1;
                cellBalancingCurrent = ((float)(bal_cellvoltage.voltage[i]))/BS_BALANCING_RESISTANCE_OHM;
                difference = (BAL_STATEMACH_BALANCINGTIME_100MS/10) * (uint32_t)(cellBalancingCurrent);
                bal_state.active = TRUE;
                bal_balancing.enable_balancing = 1;
                /* we are working with unsigned integers */
                if (difference>bal_balancing.delta_charge[i]){
                    bal_balancing.delta_charge[i] = 0;
                } else {
                bal_balancing.delta_charge[i] -= difference;
                }
            } else {
                bal_balancing.balancing_state[i] = 0;
            }
        }
    }

    DB_WriteBlock(&bal_balancing, DATA_BLOCK_ID_BALANCING_CONTROL_VALUES);

}

#endif


/**
 * @brief   re-entrance check of BAL state machine trigger function
 *
 * This function is not re-entrant and should only be called time- or event-triggered.
 * It increments the triggerentry counter from the state variable bal_state.
 * It should never be called by two different processes, so if it is the case, triggerentry
 * should never be higher than 0 when this function is called.
 *
 *
 * @return  retval  0 if no further instance of the function is active, 0xff else
 *
 */
static uint8_t BAL_CheckReEntrance(void)
{
    uint8_t retval=0;

    taskENTER_CRITICAL();
    if(!bal_state.triggerentry)
    {
        bal_state.triggerentry++;
    }
    else
        retval = 0xFF;    /* multiple calls of function */
    taskEXIT_CRITICAL();

    return (retval);
}



/**
 * @brief   gets the current state request.
 *
 * This function is used in the functioning of the BAL state machine.
 *
 * @return  retval  current state request, taken from BAL_STATE_REQUEST_e
 */
static BAL_STATE_REQUEST_e BAL_GetStateRequest(void) {

    BAL_STATE_REQUEST_e retval = BAL_STATE_NO_REQUEST;

    taskENTER_CRITICAL();
    retval    = bal_state.statereq;
    taskEXIT_CRITICAL();

    return (retval);
}


/**
 * @brief   gets the current state.
 *
 * This function is used in the functioning of the BAL state machine.
 *
 * @return  current state, taken from BAL_STATEMACH_e
 */
BAL_STATEMACH_e BAL_GetState(void) {
    return (bal_state.state);
}



/**
 * @brief   transfers the current state request to the state machine.
 *
 * This function takes the current state request from bal_state and transfers it to the state machine.
 * It resets the value from bal_state to BAL_STATE_NO_REQUEST
 *
 * @return  retVal          current state request, taken from BAL_STATE_REQUEST_e
 *
 */
static BAL_STATE_REQUEST_e BAL_TransferStateRequest(void)
{
    BAL_STATE_REQUEST_e retval = BAL_STATE_NO_REQUEST;

    taskENTER_CRITICAL();
    retval    = bal_state.statereq;
    bal_state.statereq = BAL_STATE_NO_REQUEST;
    taskEXIT_CRITICAL();

    return (retval);
}


/**
 * @brief   sets the current state request of the state variable bal_state.
 *
 * This function is used to make a state request to the state machine,e.g, start voltage measurement,
 * read result of voltage measurement, re-initialization
 * It calls BAL_CheckStateRequest() to check if the request is valid.
 * The state request is rejected if is not valid.
 * The result of the check is returned immediately, so that the requester can act in case
 * it made a non-valid state request.
 *
 * @param   statereq                state request to set
 *
 * @return  retVal                  current state request, taken from BAL_STATE_REQUEST_e
 */
BAL_RETURN_TYPE_e BAL_SetStateRequest(BAL_STATE_REQUEST_e statereq)
{
    BAL_RETURN_TYPE_e retVal = BAL_STATE_NO_REQUEST;

    taskENTER_CRITICAL();
    retVal=BAL_CheckStateRequest(statereq);

    if (retVal==BAL_OK)
        {
            bal_state.statereq   = statereq;
        }
    taskEXIT_CRITICAL();

    return (retVal);
}



/**
 * @brief   checks the state requests that are made.
 *
 * This function checks the validity of the state requests.
 * The resuls of the checked is returned immediately.
 *
 * @param   statereq    state request to be checked
 *
 * @return              result of the state request that was made, taken from BAL_RETURN_TYPE_e
 */
static BAL_RETURN_TYPE_e BAL_CheckStateRequest(BAL_STATE_REQUEST_e statereq) {

    if (statereq == BAL_STATE_ERROR_REQUEST){
        return BAL_OK;
    }
    if (statereq == BAL_STATE_GLOBAL_ENABLE_REQUEST){
        bal_state.balancing_global_allowed = 1;
        return BAL_OK;
    }
    if (statereq == BAL_STATE_GLOBAL_DISABLE_REQUEST){
        bal_state.balancing_global_allowed = 0;
        return BAL_OK;
    }
    if ( (statereq == BAL_STATE_NOBALANCING_REQUEST) || (statereq == BAL_STATE_ALLOWBALANCING_REQUEST) ){
        return BAL_OK;
    }

    if (bal_state.statereq == BAL_STATE_NO_REQUEST){
        /* init only allowed from the uninitialized state */
        if (statereq == BAL_STATE_INIT_REQUEST) {
            if (bal_state.state==BAL_STATEMACH_UNINITIALIZED) {
                return BAL_OK;
            } else {
                return BAL_ALREADY_INITIALIZED;
            }
        /* request to forbid balancing */
        } else {
            return BAL_ILLEGAL_REQUEST;
        }
    }
    else {
        return BAL_REQUEST_PENDING;
    }

}




/**
 * @brief   trigger function for the BAL driver state machine.
 *
 * This function contains the sequence of events in the BAL state machine.
 * It must be called time-triggered, every 1ms.
 */
void BAL_Trigger(void)
{

    BAL_STATE_REQUEST_e statereq = BAL_STATE_NO_REQUEST;
    uint8_t finished = FALSE;

    if (bal_state.rest_timer > 0) {
        bal_state.rest_timer--;
    }

    /* Check re-entrance of function */
    if (BAL_CheckReEntrance()) {
        return;
    }

    if(bal_state.timer) {
        if(--bal_state.timer) {
            bal_state.triggerentry--;
            return;    /* handle state machine only if timer has elapsed */
        }
    }

    /* Check if battery is resting */
    if (BS_CheckCurrent_Direction() == BS_CURRENT_NO_CURRENT) {
        if (bal_state.resting == FALSE) {
            bal_state.resting = TRUE;
            bal_state.rest_timer = BAL_TIME_BEFORE_BALANCING_S*10;
        }
    } else {
        bal_state.resting = FALSE;
    }

    switch(bal_state.state) {

        /****************************UNINITIALIZED***********************************/
        case BAL_STATEMACH_UNINITIALIZED:
            /* waiting for Initialization Request */
            statereq = BAL_TransferStateRequest();
            if(statereq == BAL_STATE_INIT_REQUEST) {
                BAL_SAVELASTSTATES();
                bal_state.timer = BAL_STATEMACH_SHORTTIME_100MS;
                bal_state.state = BAL_STATEMACH_INITIALIZATION;
                bal_state.substate = BAL_ENTRY;
            } else if(statereq == BAL_STATE_NO_REQUEST) {
                /* no actual request pending */
            } else{
                bal_state.ErrRequestCounter++;   /* illegal request pending */
            }
            break;


        /****************************INITIALIZATION**********************************/
        case BAL_STATEMACH_INITIALIZATION:

            BAL_SAVELASTSTATES();
            BAL_Init();

            bal_state.timer = BAL_STATEMACH_SHORTTIME_100MS;
            bal_state.state = BAL_STATEMACH_INITIALIZED;
            bal_state.substate = BAL_ENTRY;

            break;

        /****************************INITIALIZED*************************************/
        case BAL_STATEMACH_INITIALIZED:
            BAL_SAVELASTSTATES();
            bal_state.timer = BAL_STATEMACH_SHORTTIME_100MS;
            bal_state.state = BAL_STATEMACH_CHECK_BALANCING;
            bal_state.substate = BAL_ENTRY;

            break;

    #if BALANCING_VOLTAGE_BASED == FALSE

        /****************************CHECK_BALANCING*************************************/
        case BAL_STATEMACH_CHECK_BALANCING:
            BAL_SAVELASTSTATES();

            if (bal_state.substate == BAL_ENTRY) {
                if (bal_state.balancing_global_allowed == FALSE) {
                    if (bal_state.active == TRUE) {
                        BAL_Deactivate();
                    }
                    bal_state.active = FALSE;
                    bal_state.substate = BAL_ENTRY;
                } else {
                    bal_state.substate = BAL_CHECK_IMBALANCES;
                }

                bal_state.timer = BAL_STATEMACH_SHORTTIME_100MS;
                break;
            } else if (bal_state.substate == BAL_CHECK_IMBALANCES) {
                if (bal_state.active == TRUE) {
                    BAL_Deactivate();
                }
                if (BAL_Check_Imbalances()==TRUE) {
                    bal_state.state = BAL_STATEMACH_BALANCE;
                    bal_state.substate = BAL_ENTRY;
                } else {
                    bal_state.substate = BAL_COMPUTE_IMBALANCES;
                }
                bal_state.timer = BAL_STATEMACH_SHORTTIME_100MS;
                break;
            } else if (bal_state.substate == BAL_COMPUTE_IMBALANCES) {
                if (bal_state.rest_timer==0) {
                    BAL_Compute_Imbalances();
                    bal_state.state = BAL_STATEMACH_BALANCE;
                    bal_state.substate = BAL_ENTRY;
                } else {
                    bal_state.substate = BAL_CHECK_IMBALANCES;
                }
                bal_state.timer = BAL_STATEMACH_SHORTTIME_100MS;
                break;
            }

            break;

            /****************************BALANCE*************************************/
            case BAL_STATEMACH_BALANCE:
                BAL_SAVELASTSTATES();

                if (bal_state.substate == BAL_ENTRY) {
                    if (bal_state.balancing_global_allowed == FALSE) {
                        if (bal_state.active == TRUE) {
                            BAL_Deactivate();
                        }
                        bal_state.active = FALSE;
                        bal_state.substate = BAL_STATEMACH_CHECK_BALANCING;
                    } else {
                        bal_state.substate = BAL_ACTIVATE_BALANCING;
                    }
                    bal_state.timer = BAL_STATEMACH_SHORTTIME_100MS;
                    break;
                } else if (bal_state.substate == BAL_ACTIVATE_BALANCING) {
                    DB_ReadBlock(&bal_minmax,DATA_BLOCK_ID_MINMAX);
                    /* do not balance under a certain voltage level */
                    if (bal_minmax.voltage_min <= BAL_LOWER_VOLTAGE_LIMIT_MV ||
                        bal_minmax.temperature_max >= BAL_UPPER_TEMPERATURE_LIMIT_DEG ||
                        BAL_Check_Imbalances() == FALSE ||
                        bal_state.balancing_global_allowed == FALSE) {
                        if (bal_state.active == TRUE) {
                            BAL_Deactivate();
                        }
                        bal_state.state = BAL_STATEMACH_CHECK_BALANCING;
                        bal_state.substate = BAL_ENTRY;
                        break;
                    } else {
                        BAL_Activate_Balancing_History();
                        bal_state.timer = BAL_STATEMACH_BALANCINGTIME_100MS;
                        break;
                    }
                }

                break;

    #else /* voltage-based balancing */
        case BAL_STATEMACH_CHECK_BALANCING:
            BAL_SAVELASTSTATES();

            statereq=BAL_TransferStateRequest();
            if (statereq == BAL_STATE_NOBALANCING_REQUEST){
                bal_state.balancing_allowed = FALSE;
            }
            if (statereq == BAL_STATE_ALLOWBALANCING_REQUEST){
                bal_state.balancing_allowed = TRUE;
            }

            bal_state.timer = BAL_STATEMACH_SHORTTIME_100MS;

            if (bal_state.balancing_allowed == FALSE || bal_state.balancing_global_allowed == FALSE) {
                BAL_Deactivate();
                bal_state.active = FALSE;
            } else {
                if (bal_state.rest_timer == 0) {
                    bal_state.state = BAL_STATEMACH_BALANCE;
                    bal_state.substate = BAL_ENTRY;
                }
            }

            break;

        /****************************BALANCE*************************************/
        case BAL_STATEMACH_BALANCE:
            BAL_SAVELASTSTATES();

            /* Check if balancing is still allowed */
            statereq = BAL_TransferStateRequest();
            if (statereq == BAL_STATE_NOBALANCING_REQUEST) {
                bal_state.balancing_allowed = FALSE;
            }
            if (statereq == BAL_STATE_ALLOWBALANCING_REQUEST) {
                bal_state.balancing_allowed = TRUE;
            }

            if (bal_state.balancing_global_allowed == FALSE) {
                if (bal_state.active == TRUE) {
                    BAL_Deactivate();
                }
                bal_state.active = FALSE;
                bal_state.state = BAL_STATEMACH_CHECK_BALANCING;
                bal_state.substate = BAL_ENTRY;
                bal_state.timer = BAL_STATEMACH_SHORTTIME_100MS;
                break;
            }

            if (bal_state.substate == BAL_ENTRY){
                if (bal_state.balancing_allowed == FALSE) {
                    if (bal_state.active == TRUE) {
                        BAL_Deactivate();
                    }
                    bal_state.active = FALSE;
                    bal_state.state = BAL_STATEMACH_CHECK_BALANCING;
                    bal_state.substate = BAL_ENTRY;
                } else {
                    bal_state.substate = BAL_CHECK_LOWEST_VOLTAGE;
                }
                bal_state.timer = BAL_STATEMACH_SHORTTIME_100MS;
                break;
            } else if (bal_state.substate == BAL_CHECK_LOWEST_VOLTAGE) {
                DB_ReadBlock(&bal_minmax, DATA_BLOCK_ID_MINMAX);
                /* stop balacing if minimum voltage is below minimum threshold or maximum cell temperature breached upper temperature limit */
                if (bal_minmax.voltage_min <= BAL_LOWER_VOLTAGE_LIMIT_MV ||
                        bal_minmax.temperature_max >= BAL_UPPER_TEMPERATURE_LIMIT_DEG) {
                    if (bal_state.active == TRUE) {
                        BAL_Deactivate();
                    }
                    bal_state.state = BAL_STATEMACH_CHECK_BALANCING;
                    bal_state.substate = BAL_ENTRY;
                } else {
                    bal_state.substate = BAL_CHECK_CURRENT;
                }
                bal_state.timer = BAL_STATEMACH_BALANCINGTIME_100MS;
                break;
            } else if (bal_state.substate == BAL_CHECK_CURRENT) {
                if (bal_state.rest_timer == 0) {
                    bal_state.substate = BAL_ACTIVATE_BALANCING;
                } else {
                    if (bal_state.active == TRUE) {
                        BAL_Deactivate();
                    }
                    bal_state.state = BAL_STATEMACH_CHECK_BALANCING;
                    bal_state.substate = BAL_ENTRY;
                }
                bal_state.timer = BAL_STATEMACH_BALANCINGTIME_100MS;
                break;
            } else if (bal_state.substate == BAL_ACTIVATE_BALANCING) {
                if (bal_state.balancing_allowed == FALSE) {
                    if (bal_state.active == TRUE) {
                        BAL_Deactivate();
                    }
                    bal_state.active = FALSE;
                    bal_state.state = BAL_STATEMACH_CHECK_BALANCING;
                    bal_state.substate = BAL_ENTRY;
                } else {
                    bal_state.balancing_threshold = BAL_THRESHOLD_MV;
                    finished = BAL_Activate_Balancing_Voltage();
                    if (finished == TRUE) {
                        bal_state.balancing_threshold = BAL_THRESHOLD_MV + BAL_HYSTERESIS_MV;
                        bal_state.state = BAL_STATEMACH_CHECK_BALANCING;
                        bal_state.substate = BAL_ENTRY;
                    } else {
                        bal_state.state = BAL_STATEMACH_BALANCE;
                        bal_state.substate = BAL_ENTRY;
                    }
                }
                bal_state.timer = BAL_STATEMACH_BALANCINGTIME_100MS;
                break;
            }
            break;

    #endif /* endif balancing type */

        default:
            break;

    } /*  end switch(bal_state.state) */

    bal_state.triggerentry--;

}
