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
 * @file    interlock.h
 * @author  foxBMS Team
 * @date    23.09.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  ILCK
 *
 * @brief   Headers for the driver for the interlock.
 *
 */

#ifndef INTERLOCK_H_
#define INTERLOCK_H_

/*================== Includes =============================================*/
#include "interlock_cfg.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/
/**
 * @brief   Checks the configuration of the interlock-module
 * @return  retVal (type: STD_RETURN_TYPE_e)
 */
extern STD_RETURN_TYPE_e ILCK_Init(void);

/**
 * @brief   Gets the latest value (TRUE, FALSE) the interlock was set to.
 *
 * Meaning of the return value:
 *   - FALSE means interlock was set to be opened
 *   - TRUE means interlock was set to be closed
 *
 * @return  setInformation (type: ILCK_ELECTRICAL_STATE_TYPE_s)
 */
extern ILCK_ELECTRICAL_STATE_TYPE_s ILCK_GetInterlockSetValue(void);

/**
 * @brief   Reads the feedback pin of the interlock and returns its current value
 *          (ILCK_SWITCH_OFF/ILCK_SWITCH_ON)
 * @return  measuredInterlockState (type: ILCK_ELECTRICAL_STATE_TYPE_s)
 */
extern ILCK_ELECTRICAL_STATE_TYPE_s ILCK_GetInterlockFeedback(void);

/**
 * @brief   Sets the interlock state to its requested state, if the interlock is at that time not in the requested state.
 *
 * It returns E_OK if the requested state was successfully set or if the interlock was at the requested state before.
 * @param   requstedInterlockState (type: ILCK_ELECTRICAL_STATE_TYPE_s)
 * @return  retVal (type: STD_RETURN_TYPE_e)
 */
extern STD_RETURN_TYPE_e ILCK_SetInterlockState(ILCK_ELECTRICAL_STATE_TYPE_s requstedInterlockState);

/**
 * @brief   Switches the interlock off and returns E_NOT_OK on success.
 * @return  retVal (type: STD_RETURN_TYPE_e)
 */
extern STD_RETURN_TYPE_e ILCK_SwitchInterlockOff(void);

/**
 * @brief   Switches the interlock on and returns E_OK on success.
 * @return  retVal (type: STD_RETURN_TYPE_e)
 */
extern STD_RETURN_TYPE_e ILCK_SwitchInterlockOn(void);

/**
 * @brief   Sets interlock counter. This counter is used to ignore the
 *          evaluation of the interlock feedback for x times.
 *
 * @param   number of times the feedback signal is not evaluated
 */
extern void ILCK_SetFeedbackIgnoreCounter(uint32_t ignores);
/*================== Function Implementations =============================*/


/*================== Constant and Variable Definitions ====================*/


/**
 * States of the ILCK state machine
 */
typedef enum {
    /* Init-Sequence */
    ILCK_STATEMACH_UNINITIALIZED             = 0,    /*!<    */
    ILCK_STATEMACH_INITIALIZATION            = 1,    /*!<    */
    ILCK_STATEMACH_INITIALIZED               = 2,    /*!<    */
    ILCK_STATEMACH_WAIT_FIRST_REQUEST             = 3,    /*!<    */
    ILCK_STATEMACH_OPEN                      = 4,    /*!<    */
    ILCK_STATEMACH_CLOSED                    = 5,    /*!<    */
    ILCK_STATEMACH_UNDEFINED                 = 20,   /*!< undefined state                                */
    ILCK_STATEMACH_RESERVED1                 = 0x80, /*!< reserved state                                 */
    ILCK_STATEMACH_ERROR                     = 0xF0, /*!< Error-State:  */
} ILCK_STATEMACH_e;


/**
 * Substates of the ILCK state machine
 */
typedef enum {
    ILCK_ENTRY                                    = 0,    /*!< Substate entry state       */
    ILCK_OPEN_FIRST_CONTACTOR                     = 1,    /*!< Open-sequence: first contactor */
    ILCK_OPEN_SECOND_CONTACTOR_MINUS              = 2,    /*!< Open-sequence: second contactor */
    ILCK_OPEN_SECOND_CONTACTOR_PLUS               = 3,    /*!< Open-sequence: second contactor */
    ILCK_STANDBY                                  = 4,    /*!< Substate stanby */
    ILCK_PRECHARGE_CLOSE_MINUS                    = 5,    /*!< Begin of precharge sequence: close main minus */
    ILCK_PRECHARGE_CLOSE_PRECHARGE                = 6,    /*!< Next step of precharge sequence: close precharge */
    ILCK_PRECHARGE_CLOSE_PLUS                     = 7,    /*!< Next step of precharge sequence: close main plus */
    ILCK_PRECHARGE_CHECK_VOLTAGES                 = 8,    /*!< Next step of precharge sequence: check if voltages OK */
    ILCK_PRECHARGE_OPEN_PRECHARGE                 = 9,    /*!< Next step of precharge sequence: open precharge */
} ILCK_STATEMACH_SUB_e;


/**
 * State requests for the ILCK statemachine
 */
typedef enum {
    ILCK_STATE_INIT_REQUEST                = ILCK_STATEMACH_INITIALIZATION,           /*!<    */
    ILCK_STATE_OPEN_REQUEST             = ILCK_STATEMACH_OPEN,                     /*!<    */
    ILCK_STATE_CLOSE_REQUEST              = ILCK_STATEMACH_CLOSED,                /*!<    */
    ILCK_STATE_ERROR_REQUEST               = ILCK_STATEMACH_ERROR,   /*!<    */
    ILCK_STATE_NO_REQUEST                  = ILCK_STATEMACH_RESERVED1,                /*!<    */
} ILCK_STATE_REQUEST_e;


/**
 * Possible return values when state requests are made to the ILCK statemachine
 */
typedef enum {
    ILCK_OK                                 = 0,    /*!< ILCK --> ok                             */
    ILCK_BUSY_OK                            = 1,    /*!< ILCK under load --> ok                  */
    ILCK_REQUEST_PENDING                    = 2,    /*!< requested to be executed               */
    ILCK_ILLEGAL_REQUEST                    = 3,    /*!< Request can not be executed            */
    ILCK_INIT_ERROR                         = 7,    /*!< Error state: Source: Initialization    */
    ILCK_OK_FROM_ERROR                      = 8,    /*!< Return from error --> ok               */
    ILCK_ERROR                              = 20,   /*!< General error state                    */
    ILCK_ALREADY_INITIALIZED                = 30,   /*!< Initialization of LTC already finished */
    ILCK_ILLEGAL_TASK_TYPE                  = 99,   /*!< Illegal                                */
} ILCK_RETURN_TYPE_e;


/**
 * This structure contains all the variables relevant for the ILCK state machine.
 * The user can get the current state of the ILCK state machine with this variable
 */
typedef struct {
    uint16_t timer;                         /*!< time in ms before the state machine processes the next state, e.g. in counts of 1ms    */
    ILCK_STATE_REQUEST_e statereq;          /*!< current state request made to the state machine                                        */
    ILCK_STATEMACH_e state;                 /*!< state of Driver State Machine                                                          */
    ILCK_STATEMACH_SUB_e substate;          /*!< current substate of the state machine                                                  */
    ILCK_STATEMACH_e laststate;             /*!< previous state of the state machine                                                    */
    ILCK_STATEMACH_SUB_e lastsubstate;      /*!< previous substate of the state machine                                                 */
    uint32_t ErrRequestCounter;             /*!< counts the number of illegal requests to the LTC state machine */
    uint8_t triggerentry;                   /*!< counter for re-entrance protection (function running flag) */
    uint8_t counter;                        /*!< general purpose counter */
} ILCK_STATE_s;


/*================== Function Prototypes ==================================*/

extern ILCK_RETURN_TYPE_e ILCK_SetStateRequest(ILCK_STATE_REQUEST_e statereq);
extern  ILCK_STATEMACH_e ILCK_GetState(void);
extern void ILCK_Trigger(void);

#endif /* INTERLOCK_H_ */
