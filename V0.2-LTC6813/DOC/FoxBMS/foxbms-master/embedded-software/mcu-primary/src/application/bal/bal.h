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
 * @file    bal.h
 * @author  foxBMS Team
 * @date    26.02.2016 (date of creation)
 * @ingroup DRIVERS
 * @prefix  BAL
 *
 * @brief   Header for the driver for balancing
 *
 */

#ifndef BAL_H_
#define BAL_H_

/*================== Includes =============================================*/
#include "bal_cfg.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

/*================== Constant and Variable Definitions ====================*/

/**
 * States of the BAL state machine
 */
typedef enum {
    /* Init-Sequence */
    BAL_STATEMACH_UNINITIALIZED             = 0,    /*!<    */
    BAL_STATEMACH_INITIALIZATION            = 1,    /*!<    */
    BAL_STATEMACH_INITIALIZED               = 2,    /*!<    */
    BAL_STATEMACH_CHECK_BALANCING           = 3,    /*!<    */
    BAL_STATEMACH_BALANCE                   = 4,    /*!<    */
    BAL_STATEMACH_NOBALANCING               = 5,    /*!<    */
    BAL_STATEMACH_ALLOWBALANCING            = 6,    /*!<    */
    BAL_STATEMACH_GLOBALDISABLE             = 7,    /*!<    */
    BAL_STATEMACH_GLOBALENABLE              = 8,    /*!<    */
    BAL_STATEMACH_UNDEFINED                 = 20,   /*!< undefined state                                */
    BAL_STATEMACH_RESERVED1                 = 0x80, /*!< reserved state                                 */
    BAL_STATEMACH_ERROR                     = 0xF0, /*!< Error-State:  */
} BAL_STATEMACH_e;


/**
 * Substates of the BAL state machine
 */
typedef enum {
    BAL_ENTRY                                    = 0,    /*!< Substate entry state       */
    BAL_CHECK_IMBALANCES                         = 1,  /*!< Check if balancing has been initialized       */
    BAL_COMPUTE_IMBALANCES                       = 2,  /*!< Compute imbalances */
    BAL_ACTIVATE_BALANCING                       = 3,  /*!< Activated balancing resistors */
    BAL_CHECK_LOWEST_VOLTAGE                     = 4,  /*!< Check if lowest voltage is still  above limit */
    BAL_CHECK_CURRENT                            = 5,  /*!< Check if current is still  under limit */
} BAL_STATEMACH_SUB_e;


/**
 * State requests for the BAL statemachine
 */
typedef enum {
    BAL_STATE_INIT_REQUEST                     = BAL_STATEMACH_INITIALIZATION,           /*!<    */
    BAL_STATE_ERROR_REQUEST                    = BAL_STATEMACH_ERROR,   /*!<    */
    BAL_STATE_NOBALANCING_REQUEST              = BAL_STATEMACH_NOBALANCING,     /*!<    */
    BAL_STATE_ALLOWBALANCING_REQUEST           = BAL_STATEMACH_ALLOWBALANCING,     /*!<    */
    BAL_STATE_GLOBAL_DISABLE_REQUEST           = BAL_STATEMACH_GLOBALDISABLE,     /*!<    */
    BAL_STATE_GLOBAL_ENABLE_REQUEST            = BAL_STATEMACH_GLOBALENABLE,     /*!<    */
    BAL_STATE_NO_REQUEST                       = BAL_STATEMACH_RESERVED1,                /*!<    */
} BAL_STATE_REQUEST_e;


/**
 * Possible return values when state requests are made to the BAL statemachine
 */
typedef enum {
    BAL_OK                                 = 0,    /*!< CONT --> ok                             */
    BAL_BUSY_OK                            = 1,    /*!< CONT under load --> ok                  */
    BAL_REQUEST_PENDING                    = 2,    /*!< requested to be executed               */
    BAL_ILLEGAL_REQUEST                    = 3,    /*!< Request can not be executed            */
    BAL_INIT_ERROR                         = 7,    /*!< Error state: Source: Initialization    */
    BAL_OK_FROM_ERROR                      = 8,    /*!< Return from error --> ok               */
    BAL_ERROR                              = 20,   /*!< General error state                    */
    BAL_ALREADY_INITIALIZED                = 30,   /*!< Initialization of LTC already finished */
    BAL_ILLEGAL_TASK_TYPE                  = 99,   /*!< Illegal                                */
} BAL_RETURN_TYPE_e;




/**
 * This structure contains all the variables relevant for the BAL state machine.
 * The user can get the current state of the BAL state machine with this variable
 */
typedef struct {
    uint16_t timer;                         /*!< time in ms before the state machine processes the next state, e.g. in counts of 1ms    */
    BAL_STATE_REQUEST_e statereq;           /*!< current state request made to the state machine                                        */
    BAL_STATEMACH_e state;                  /*!< state of Driver State Machine                                                          */
    BAL_STATEMACH_SUB_e substate;                       /*!< current substate of the state machine                                                  */
    BAL_STATEMACH_e laststate;              /*!< previous state of the state machine                                                    */
    uint8_t lastsubstate;                   /*!< previous substate of the state machine                                                 */
    uint8_t triggerentry;                   /*!< counter for re-entrance protection (function running flag) */
    uint32_t ErrRequestCounter;             /*!< counts the number of illegal requests to the BAL state machine */
    uint8_t active;                         /*!< indicate if balancing active or not */
    uint8_t resting;                        /*!< indicate if current flowing through battery or not */
    uint32_t rest_timer;                    /*!< counter since last timestamp with no current flowing */
    uint32_t balancing_threshold;           /*!< effective balancing threshod */
    uint8_t balancing_allowed;              /*!< flag to disable balancing */
    uint8_t balancing_global_allowed;       /*!< flag to globally disable balancing */
} BAL_STATE_s;


/*================== Function Prototypes ==================================*/

extern BAL_RETURN_TYPE_e BAL_SetStateRequest(BAL_STATE_REQUEST_e statereq);
extern  BAL_STATEMACH_e BAL_GetState(void);
extern void BAL_Trigger(void);

#endif /* BAL_H_ */
