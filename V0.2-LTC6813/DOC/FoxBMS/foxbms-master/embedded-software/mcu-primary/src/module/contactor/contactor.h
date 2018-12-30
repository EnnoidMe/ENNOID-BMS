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
 * @file    contactor.h
 * @author  foxBMS Team
 * @date    23.09.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  CONT
 *
 * @brief   Headers for the driver for the contactors.
 *
 */

#ifndef CONTACTOR_H_
#define CONTACTOR_H_

/*================== Includes =============================================*/
#include "contactor_cfg.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/
/**
 * @brief   Checks the configuration of the contactor-module
 *
 * @return  retVal (type: STD_RETURN_TYPE_e)
 */
extern STD_RETURN_TYPE_e CONT_Init(void);

/**
 * @brief   Gets the latest value (TRUE, FALSE) the contactors were set to.
 *
 * @param   contactor (type: CONT_NAMES_e)
 *
 * @return  returns CONT_SWITCH_OFF or CONT_SWITCH_ON
 */
extern CONT_ELECTRICAL_STATE_TYPE_s CONT_GetContactorSetValue(CONT_NAMES_e contactor);


/**
 * @brief   Reads the feedback pin of every contactor and returns its current value
 *          (CONT_SWITCH_OFF/CONT_SWITCH_ON).
 *
 * @details If the contactor has a feedback pin the measured feedback is returned. If the contactor
 *          has no feedback pin, it is assumed that after a certain time the contactor has reached
 *          the requested state.
 *
 * @param   contactor (type: CONT_NAMES_e)
 *
 * @return  measuredContactorState (type: CONT_ELECTRICAL_STATE_TYPE_s)
 */
extern CONT_ELECTRICAL_STATE_TYPE_s CONT_GetContactorFeedback(CONT_NAMES_e contactor);

/**
 * @brief   Reads the feedback pins of all contactors and updates the contactors_cfg[] array with
 *          their current states.
 *
 * @return  Returns E_OK if all feedbacks could be acquired (type: STD_RETURN_TYPE_e)
 */
extern STD_RETURN_TYPE_e CONT_AcquireContactorFeedbacks(void);

/**
 * @brief   Sets the contactor state to its requested state, if the contactor is at that time not
 *          in the requested state.
 *
 * @details If the new state was already requested, but not reached (meaning the measured feedback
 *          does not return the requested state), there are two states: it can be still ok (E_OK),
 *          because the contactor has some time left to get physically in the requested state
 *          (passed time since the request is lower than the limit) or it can be not ok (E_NOT_OK),
 *          because there is timing violation, i.e. the contactor has surpassed the maximum time
 *          for getting in the requested state. It returns E_OK if the requested state was
 *          successfully set or if the contactor was at the requested state before.
 *
 * @param   contactor (type: CONT_NAMES_e)
 * @param   requestedContactorState (type: CONT_ELECTRICAL_STATE_TYPE_s)
 *
 * @return  retVal (type: STD_RETURN_TYPE_e)
 */
extern STD_RETURN_TYPE_e CONT_SetContactorState(CONT_NAMES_e contactor, CONT_ELECTRICAL_STATE_TYPE_s requestedContactorState);


/**
 * @brief   Iterates over the contactor array and switches all contactors off
 *
 * @return  E_OK if all contactors were opened, E_NOT_OK if not all contactors could be opened
 *          (type: STD_RETURN_TYPE_e)
 */
extern STD_RETURN_TYPE_e CONT_SwitchAllContactorsOff(void);


/*================== Function Implementations =============================*/


/*================== Constant and Variable Definitions ====================*/

/**
 * States of the CONT state machine
 */
typedef enum {
    /* Init-Sequence */
    CONT_STATEMACH_UNINITIALIZED             = 0,    /*!<    */
    CONT_STATEMACH_INITIALIZATION            = 1,    /*!<    */
    CONT_STATEMACH_INITIALIZED               = 2,    /*!<    */
    CONT_STATEMACH_IDLE                      = 3,    /*!<    */
    CONT_STATEMACH_STANDBY                   = 4,    /*!<    */
    CONT_STATEMACH_PRECHARGE                 = 5,    /*!<    */
    CONT_STATEMACH_NORMAL                    = 6,    /*!<    */
    CONT_STATEMACH_CHARGE_PRECHARGE          = 7,    /*!<    */
    CONT_STATEMACH_CHARGE                    = 8,    /*!<    */
    CONT_STATEMACH_UNDEFINED                 = 20,   /*!< undefined state                                */
    CONT_STATEMACH_RESERVED1                 = 0x80, /*!< reserved state                                 */
    CONT_STATEMACH_ERROR                     = 0xF0, /*!< Error-State:  */
} CONT_STATEMACH_e;

/**
 * Substates of the CONT state machine
 */
typedef enum {
    CONT_ENTRY                                    = 0,    /*!< Substate entry state       */
    CONT_OPEN_FIRST_CONTACTOR                     = 1,    /*!< Open-sequence: first contactor */
    CONT_OPEN_SECOND_CONTACTOR_MINUS              = 2,    /*!< Open-sequence: second contactor */
    CONT_OPEN_SECOND_CONTACTOR_PLUS               = 3,    /*!< Open-sequence: second contactor */
    CONT_STANDBY                                  = 4,    /*!< Substate stanby */
    CONT_PRECHARGE_CLOSE_MINUS                    = 5,    /*!< Begin of precharge sequence: close main minus */
    CONT_PRECHARGE_CLOSE_PRECHARGE                = 6,    /*!< Next step of precharge sequence: close precharge */
    CONT_PRECHARGE_CLOSE_PLUS                     = 7,    /*!< Next step of precharge sequence: close main plus */
    CONT_PRECHARGE_CHECK_VOLTAGES                 = 8,    /*!< Next step of precharge sequence: check if voltages OK */
    CONT_PRECHARGE_OPEN_PRECHARGE                 = 9,    /*!< Next step of precharge sequence: open precharge */
    CONT_ERROR                                    = 10,   /*!< Error state */
} CONT_STATEMACH_SUB_e;

/**
 * State requests for the CONT statemachine
 */
typedef enum {
    CONT_STATE_INIT_REQUEST                = CONT_STATEMACH_INITIALIZATION,           /*!<    */
    CONT_STATE_STANDBY_REQUEST             = CONT_STATEMACH_STANDBY,                     /*!<    */
    CONT_STATE_NORMAL_REQUEST              = CONT_STATEMACH_NORMAL,                /*!<    */
    CONT_STATE_CHARGE_REQUEST              = CONT_STATEMACH_CHARGE,                /*!<    */
    CONT_STATE_ERROR_REQUEST               = CONT_STATEMACH_ERROR,   /*!<    */
    CONT_STATE_NO_REQUEST                  = CONT_STATEMACH_RESERVED1,                /*!<    */
} CONT_STATE_REQUEST_e;

/**
 * Possible return values when state requests are made to the CONT statemachine
 */
typedef enum {
    CONT_OK                                 = 0,    /*!< CONT --> ok                            */
    CONT_BUSY_OK                            = 1,    /*!< CONT under load --> ok                 */
    CONT_REQUEST_PENDING                    = 2,    /*!< requested to be executed               */
    CONT_REQUEST_IMPOSSIBLE                 = 3,    /*!< requested not possible                 */
    CONT_ILLEGAL_REQUEST                    = 4,    /*!< Request can not be executed            */
    CONT_INIT_ERROR                         = 5,    /*!< Error state: Source: Initialization    */
    CONT_OK_FROM_ERROR                      = 6,    /*!< Return from error --> ok               */
    CONT_ALREADY_INITIALIZED                = 30,   /*!< Initialization of LTC already finished */
    CONT_ILLEGAL_TASK_TYPE                  = 99,   /*!< Illegal                                */
} CONT_RETURN_TYPE_e;

/**
 * This structure contains all the variables relevant for the CONT state machine.
 * The user can get the current state of the CONT state machine with this variable
 */
typedef struct {
    uint16_t timer;                          /*!< time in ms before the state machine processes the next state, e.g. in counts of 1ms    */
    CONT_STATE_REQUEST_e statereq;           /*!< current state request made to the state machine                                        */
    CONT_STATEMACH_e state;                  /*!< state of Driver State Machine                                                          */
    CONT_STATEMACH_SUB_e substate;           /*!< current substate of the state machine                                                  */
    CONT_STATEMACH_e laststate;              /*!< previous state of the state machine                                                    */
    CONT_STATEMACH_SUB_e lastsubstate;       /*!< previous substate of the state machine                                                 */
    uint32_t ErrRequestCounter;              /*!< counts the number of illegal requests to the LTC state machine    */
    uint16_t OscillationCounter;             /*!< timeout to prevent oscillation of contactors */
    uint8_t PrechargeTryCounter;             /*!< timeout to prevent oscillation of contactors */
    uint16_t PrechargeTimeOut;               /*!< time to wait when precharge has been closed for voltages to settle */
    uint8_t triggerentry;                    /*!< counter for re-entrance protection (function running flag) */
    uint8_t counter;                         /*!< general purpose counter */
} CONT_STATE_s;


/*================== Function Prototypes ==================================*/

extern CONT_RETURN_TYPE_e CONT_SetStateRequest(CONT_STATE_REQUEST_e statereq);
extern  CONT_STATEMACH_e CONT_GetState(void);
extern void CONT_Trigger(void);

#endif /* CONTACTOR_H_ */
