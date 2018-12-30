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
 * @file    contactor_cfg.h
 * @author  foxBMS Team
 * @date    23.09.2015 (date of creation)
 * @ingroup DRIVERS_CONF
 * @prefix  CONT
 *
 * @brief   Header for the configuration for the driver for the contactors
 *
 */

#ifndef CONTACTOR_CFG_H_
#define CONTACTOR_CFG_H_

/*================== Includes =============================================*/
#include "general.h"
#include "batterysystem_cfg.h"
#include "io.h"

/*================== Macros and Definitions ===============================*/

/**
 * @ingroup CONFIG_CONTACTOR
 * defines the number of bad countings of opening contactors at too
 *  high current
 * \par Type:
 * int
 * \par Default:
 * 10
 * \par Range:
 * [9,11]
*/
#define CONT_NUMBER_OF_BAD_COUNTINGS 10

/**
 * @ingroup CONFIG_CONTACTOR
 * This macro describes the limiting current from the the positive to
 * negative side of the contactor at which a damaging of the
 * contactor occurs. If this limit is exceeded the contacor
 * module makes an entry in the diagnosis module indicating a
 * switching off of the contactors under a bad  condition
 *
 * \par Type:
 * float
 * \par Default:
 * 1.0
 * \par Range:
 * [1.0,2.0]
*/
#define BAD_SWITCHOFF_CURRENT_POS 100000.0

/**
 * @ingroup CONFIG_CONTACTOR
 * This macro describes the limiting current from the the negative to
 * positive side of the contactor at which a damaging of the
 * contactor occurs. If this limit is exceeded the contacor
 * module makes an entry in the diagnosis module indicating a
 * switching off of the contactors under a bad  condition
 *
 * \par Type:
 * float
 * \par Default:
 * -1.0
 * \par Range:
 * [-2.0,-1.0]
*/
#define BAD_SWITCHOFF_CURRENT_NEG -1000000.0

/*
 * The number of defines per contactor must be the same as the length
 *  of the array cont_contactors_cfg in contactor_cfg.c
 * Every contactor consists of 1 control pin and 1 feedback pin
 * counting together as 1 contactor.
 * E.g. if you have 1 contactor your define has to be:
 *      #define CONT_INTERLOCK_CONTROL       PIN_MCU_0_INTERLOCK_CONTROL
 *      #define CONT_INTERLOCK_FEEDBACK      PIN_MCU_0_INTERLOCK_FEEDBACK
 */
#define CONT_INTERLOCK_CONTROL                  IO_PIN_MCU_0_INTERLOCK_CONTROL
#define CONT_INTERLOCK_FEEDBACK                 IO_PIN_MCU_0_INTERLOCK_FEEDBACK

#define CONT_MAIN_PLUS_CONTROL                  IO_PIN_MCU_0_CONTACTOR_0_CONTROL
#define CONT_MAIN_PLUS_FEEDBACK                 IO_PIN_MCU_0_CONTACTOR_0_FEEDBACK

#define CONT_PRECHARGE_PLUS_CONTROL             IO_PIN_MCU_0_CONTACTOR_1_CONTROL
#define CONT_PRECHARGE_PLUS_FEEDBACK            IO_PIN_MCU_0_CONTACTOR_1_FEEDBACK

#define CONT_MAIN_MINUS_CONTROL                 IO_PIN_MCU_0_CONTACTOR_2_CONTROL
#define CONT_MAIN_MINUS_FEEDBACK                IO_PIN_MCU_0_CONTACTOR_2_FEEDBACK

#if BS_SEPARATE_POWERLINES == 1
#define CONT_CHARGE_MAIN_PLUS_CONTROL           IO_PIN_MCU_0_CONTACTOR_3_CONTROL
#define CONT_CHARGE_MAIN_PLUS_FEEDBACK          IO_PIN_MCU_0_CONTACTOR_3_FEEDBACK

#define CONT_CHARGE_PRECHARGE_PLUS_CONTROL      IO_PIN_MCU_0_CONTACTOR_4_CONTROL
#define CONT_CHARGE_PRECHARGE_PLUS_FEEDBACK     IO_PIN_MCU_0_CONTACTOR_4_FEEDBACK

#define CONT_CHARGE_MAIN_MINUS_CONTROL          IO_PIN_MCU_0_CONTACTOR_5_CONTROL
#define CONT_CHARGE_MAIN_MINUS_FEEDBACK         IO_PIN_MCU_0_CONTACTOR_5_FEEDBACK
#endif /* BS_SEPARATE_POWERLINES == 1 */
/*
 * additional possible contactors from the io definition
#define CONT_X0_CONTROL                         PIN_MCU_0_CONTACTOR_3_CONTROL
#define CONT_X0_FEEDBACK                        PIN_MCU_0_CONTACTOR_3_FEEDBACK

#define CONT_X1_CONTROL                         PIN_MCU_0_CONTACTOR_4_CONTROL
#define CONT_X1_FEEDBACK                        PIN_MCU_0_CONTACTOR_4_FEEDBACK

#define CONT_X2_CONTROL                         PIN_MCU_0_CONTACTOR_5_CONTROL
#define CONT_X2_FEEDBACK                        PIN_MCU_0_CONTACTOR_5_FEEDBACK

#define CONT_X3_CONTROL                         PIN_MCU_0_CONTACTOR_6_CONTROL
#define CONT_X3_FEEDBACK                        PIN_MCU_0_CONTACTOR_6_FEEDBACK

#define CONT_X4_CONTROL                         PIN_MCU_0_CONTACTOR_7_CONTROL
#define CONT_X4_FEEDBACK                        PIN_MCU_0_CONTACTOR_7_FEEDBACK

#define CONT_X5_CONTROL                         PIN_MCU_0_CONTACTOR_8_CONTROL
#define CONT_X5_FEEDBACK                        PIN_MCU_0_CONTACTOR_8_FEEDBACK
*/

/**
 * This define MUST represent the cycle time of the task in which context the
 * functions run, e.g., if the CONT_Trigger() is running in the 10 ms task
 * then the define must be set to 10.
 *
 * This define also sets the minimum time.
 */

#define CONT_TASK_CYCLE_CONTEXT_MS (10)

/**
 * Counter limit used to prevent contactor oscillation
 */

#define CONT_OSCILLATION_LIMIT 500


/**
 * Number of allowed tries to close contactors
 */
#define CONT_PRECHARGE_TRIES 3

/**
 * Delay between open first and second contactor
 */

#define CONT_DELAY_BETWEEN_OPENING_CONTACTORS_MS        ((50) * (CONT_TASK_CYCLE_CONTEXT_MS))

/**
 * Delay after opening second contactor
 */

#define CONT_DELAY_AFTER_OPENING_SECOND_CONTACTORS_MS   ((50) *  (CONT_TASK_CYCLE_CONTEXT_MS))


/**
 * CONT statemachine short time definition in ms
 */

#define CONT_STATEMACH_SHORTTIME_MS                     (CONT_TASK_CYCLE_CONTEXT_MS)


/**
 * CONT statemachine time to wait after contactors opened because precharge failed in ms
 */
#define CONT_STATEMACH_TIMEAFTERPRECHARGEFAIL_MS        ((100) * (CONT_TASK_CYCLE_CONTEXT_MS))


/*================== Main precharge configuration ====================*/

/**
 * Precharge timeout in ms
 */
#define CONT_PRECHARGE_TIMEOUT_MS ((500) * (CONT_TASK_CYCLE_CONTEXT_MS))

/**
 * Delay after closing main minus in ms
 */
#define CONT_STATEMACH_WAIT_AFTER_CLOSING_MINUS_MS ((50) * (CONT_TASK_CYCLE_CONTEXT_MS))

/**
 * Delay after closing precharge in ms
 */

#define CONT_STATEMACH_WAIT_AFTER_CLOSING_PRECHARGE_MS ((100) * (CONT_TASK_CYCLE_CONTEXT_MS))

/**
 * Delay after closing main plus in ms
 */
#define CONT_STATEMACH_WAIT_AFTER_CLOSING_PLUS_MS ((100) * (CONT_TASK_CYCLE_CONTEXT_MS))

/**
 * Delay after opening precharge in ms
 */
#define CONT_STATEMACH_WAIT_AFTER_OPENING_PRECHARGE_MS ((50) * (CONT_TASK_CYCLE_CONTEXT_MS))

/**
 * @ingroup CONFIG_CONTACTOR
 * \par Type:
 * int
 * \par Default:
 * 1000
 * \par Range:
 * [1000,3000]
 * \par Unit:
 * V
*/
#define CONT_PRECHARGE_VOLTAGE_THRESHOLD_mV     1000  /* mV */

/**
 * @ingroup CONFIG_CONTACTOR
 * \par Type:
 * int
 * \par Default:
 * 10
 * \par Range:
 * [50,500]
 * \par Unit:
 * mA
*/
#define CONT_PRECHARGE_CURRENT_THRESHOLD_mA     50  /* mA */


/*================== Charge precharge configuration ====================*/

/**
 * Charge precharge timeout in ms
 */
#define CONT_CHARGE_PRECHARGE_TIMEOUT_MS ((500) * (CONT_TASK_CYCLE_CONTEXT_MS))

/**
 * Delay after closing charge minus in ms
 */
#define CONT_STATEMACH_CHARGE_WAIT_AFTER_CLOSING_MINUS_MS ((50) * (CONT_TASK_CYCLE_CONTEXT_MS))

/**
 * Delay after closing charge precharge in ms
 */

#define CONT_STATEMACH_CHARGE_WAIT_AFTER_CLOSING_PRECHARGE_MS ((100) * (CONT_TASK_CYCLE_CONTEXT_MS))

/**
 * Delay after closing charge plus in ms
 */
#define CONT_STATEMACH_CHARGE_WAIT_AFTER_CLOSING_PLUS_MS ((100) * (CONT_TASK_CYCLE_CONTEXT_MS))

/**
 * @ingroup CONFIG_CONTACTOR
 * \par Type:
 * int
 * \par Default:
 * 1000
 * \par Range:
 * [1000,3000]
 * \par Unit:
 * V
*/
#define CONT_CHARGE_PRECHARGE_VOLTAGE_THRESHOLD_mV   1000  /* mV */

/**
 * @ingroup CONFIG_CONTACTOR
 * \par Type:
 * int
 * \par Default:
 * 10
 * \par Range:
 * [50,500]
 * \par Unit:
 * mA
*/
#define CONT_CHARGE_PRECHARGE_CURRENT_THRESHOLD_mA  50  /* mA */


/*================== Constant and Variable Definitions ====================*/

/**
 * Symbolic names for contactors' possible states
 */
typedef enum {
    CONT_SWITCH_OFF     = 0,    /*!< Contactor off         --> Contactor is open           */
    CONT_SWITCH_ON      = 1,    /*!< Contactor on          --> Contactor is closed         */
    CONT_SWITCH_UNDEF   = 2,    /*!< Contactor undefined   --> Contactor state not known   */
} CONT_ELECTRICAL_STATE_TYPE_s;

/**
 * Symbolic names for the contactors, which are used in
 * the contactor_config[] array
 */
typedef enum {
    CONT_MAIN_PLUS      = 0,    /*!< Main contactor in the positive path of the powerline      */
    CONT_PRECHARGE_PLUS = 1,    /*!< Precharge contactor in the positive path of the powerline */
    CONT_MAIN_MINUS     = 2,    /*!< Main contactor in the negative path of the powerline      */
    CONT_CHARGE_MAIN_PLUS       = 3,    /*!< Main contactor in the positive charge path of the powerline      */
    CONT_CHARGE_PRECHARGE_PLUS  = 4,    /*!< Precharge contactor in the positive charge path of the powerline */
    CONT_CHARGE_MAIN_MINUS      = 5,    /*!< Main contactor in the negative charge path of the powerline      */
} CONT_NAMES_e;

/**
 * Symbolic names defining the electric behavior of the contactor
 */
typedef enum {
    CONT_FEEDBACK_NORMALLY_OPEN     = 0,    /*!< Feedback line of a contactor is normally open      */
    CONT_FEEDBACK_NORMALLY_CLOSED   = 1,    /*!< Feedback line of a contactor is normally closed    */
    CONT_HAS_NO_FEEDBACK    = 0xFF  /*!< Feedback line of the contactor is not used         */
} CONT_FEEDBACK_TYPE_e;

typedef struct {
    CONT_ELECTRICAL_STATE_TYPE_s set;
    CONT_ELECTRICAL_STATE_TYPE_s feedback;
} CONT_ELECTRICAL_STATE_s;

typedef struct {
    IO_PORTS_e control_pin;
    IO_PORTS_e feedback_pin;
    CONT_FEEDBACK_TYPE_e feedback_pin_type;
} CONT_CONFIG_s;


typedef enum {
    CONT_POWERLINE_NORMAL   = 0,
#if BS_SEPARATE_POWERLINES == 1
    CONT_POWERLINE_CHARGE   = 1
#endif /* BS_SEPARATE_POWERLINES == 1 */
} CONT_WHICH_POWERLINE_e;

extern const CONT_CONFIG_s cont_contactors_config[BS_NR_OF_CONTACTORS];
extern CONT_ELECTRICAL_STATE_s cont_contactor_states[BS_NR_OF_CONTACTORS];

extern const uint8_t cont_contactors_config_length;
extern const uint8_t cont_contactors_states_length;

/*================== Function Prototypes ==================================*/

/**
 * @brief   Checks if the current limitations are violated
 *
 * @return  E_OK if the current limitations are NOT violated, else E_NOT_OK (type: STD_RETURN_TYPE_e)
 */
extern STD_RETURN_TYPE_e CONT_CheckPrecharge(CONT_WHICH_POWERLINE_e);

/*================== Function Implementations =============================*/


#endif /* CONTACTOR_CFG_H_ */
