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
 * @file    ir155.h
 * @author  foxBMS Team
 * @date    27.02.2013 (date of creation)
 * @ingroup DRIVERS
 * @prefix  IR155
 *
 * @brief   Headers for the driver for the isolation monitoring
 *
 * Module header for insulation measurements. Sets up the interface to the
 * IO and TIM module. Also sets up Bender Isometer specific settings.
 *
 */

#ifndef IR155_H_
#define IR155_H_

/*================== Includes =============================================*/
#include "general.h"

#include "io.h"
#include "timer.h"

/*================== Macros and Definitions ===============================*/
#define IR155_BENDER_IC_HANDLE   (htim9)

/**
 * @ingroup CONFIG_IR155
 * enable Bender history, designed for debugging purposes only
 * \par Type:
 * toggle
 * \par Default:
 * False
*/
/* #define IR155_HISTORYENABLE */



/**
 * @ingroup CONFIG_IR155
 * history depth, if history is enabled by IR155_KEY_HISTORYENABLE
 * \par Type:
 * int
 * \par Range:
 * x > 0
 * \par Default:
 * 5
*/
#define IR155_HISTORY_LENGTH            5

/**
 * @ingroup CONFIG_IR155
 * IR155_PERIODE_RESOLUTION
 * \par Type:
 * int
 * \par Range:
 * x > 0
 * \par Default:
 * 75
*/
#define IR155_PERIODE_RESOLUTION        75

/**
 * @ingroup CONFIG_IR155
 * IR155_WAIT_TIME_AFTER_GNDERROR % IR155_CYCLE_TIME == 0 !!!!
 * During this time, the results of Bender Isometer aren't to be trusted
 * wait time in [ms] <= 65535;
 * IR155_WAIT_TIME_AFTER_GNDERROR
 * \par Type:
 * int
 * \par Range:
 * 0 < x <=65535
 * \par Unit:
 * ms
 * \par Default:
 * 25000
*/
#define IR155_WAIT_TIME_AFTER_GNDERROR  25000


/**
 * @ingroup CONFIG_IR155
 * Response value for Bender IR155-3204 with Article No. B91068139V4
 * \par Type:
 * int
 * \par Range:
 * x > 0
 * \par Default:
 * 100
*/
#define IR155_RESISTANCE_RESPONSE_VALUE 100

/* Enable/Disable Bender Hardware */
#define IR155_ENABLE_BENDER_HW()        IO_WritePin(IO_PIN_MCU_0_BENDER_SUPPLY_ENABLE, IO_PIN_SET)
#define IR155_DISABLE_BENDER_HW()       IO_WritePin(IO_PIN_MCU_0_BENDER_SUPPLY_ENABLE, IO_PIN_RESET)

/* Timer-modul interface functions */
#define IR155_START_PWM_MEASUREMENT(x)  TIM_Start_PWM_IC_Measurement(x)
#define IR155_GET_DUTYCYCLE(x)          TIM_GetDutycycle(&IR155_BENDER_IC_HANDLE, x)

/* Read pin state of MHS and OKHS Pin */
#define IR155_GET_MHS()                 IO_ReadPin(IO_PIN_MCU_0_BENDER_PWM)
#define IR155_GET_OKHS()                IO_ReadPin(IO_PIN_MCU_0_BENDER_OK)

/* Clear TIM update flag */
#define IR155_RESET_TIM()               __HAL_TIM_CLEAR_FLAG(&IR155_BENDER_IC_HANDLE, TIM_FLAG_UPDATE)

/**
 * symbolic names for the different possible periods of Bender Isometer.
 * Min and max values are defined for tolerance purposes of the measurement.
 * @ingroup ISO
 */
typedef enum {
    /*Periodes in units of in units of  (1/75) ms*/
    IR155_PERIODE_MAX                   = 200*IR155_PERIODE_RESOLUTION,     /* 5    Hz*/

    IR155_NORMALCONDITION_PERIODE_MAX   = 120*IR155_PERIODE_RESOLUTION,     /* 8,3  Hz*/
    IR155_NORMALCONDITION_PERIODE       = 100*IR155_PERIODE_RESOLUTION,     /* 10   Hz*/
    IR155_NORMALCONDITION_PERIODE_MIN   = 80*IR155_PERIODE_RESOLUTION,      /* 12,5 Hz*/

    IR155_UNDERVOLATGE_PERIODE_MAX      = 60*IR155_PERIODE_RESOLUTION,      /* 16,7  Hz*/
    IR155_UNDERVOLATGE_PERIODE          = 50*IR155_PERIODE_RESOLUTION,      /* 20    Hz*/
    IR155_UNDERVOLATGE_PERIODE_MIN      = 40*IR155_PERIODE_RESOLUTION,      /* 25    Hz*/

    IR155_SPEEDSTART_PERIODE_MAX        = 37*IR155_PERIODE_RESOLUTION,      /* 27    Hz*/
    IR155_SPEEDSTART_PERIODE            = 33*IR155_PERIODE_RESOLUTION,      /* 30    Hz*/
    IR155_SPEEDSTART_PERIODE_MIN        = 29*IR155_PERIODE_RESOLUTION,      /* 34,5  Hz*/

    IR155_IMDERROR_PERIODE_MAX          = 28*IR155_PERIODE_RESOLUTION,      /* 35,7  Hz*/
    IR155_IMDERROR_PERIODE              = 25*IR155_PERIODE_RESOLUTION,      /* 40    Hz*/
    IR155_IMDERROR_PERIODE_MIN          = 23*IR155_PERIODE_RESOLUTION,      /* 43,5  Hz*/

    IR155_GROUNDERROR_PERIODE_MAX       = 22*IR155_PERIODE_RESOLUTION,      /* 45,5  Hz*/
    IR155_GROUNDERROR_PERIODE           = 20*IR155_PERIODE_RESOLUTION,      /* 50    Hz*/
    IR155_GROUNDERROR_PERIODE_MIN       = 17*IR155_PERIODE_RESOLUTION,      /* 58,8  Hz*/
} IR155_SIGPERIODE_e;


/**
 * symbolic names for the different operating modes Bender Isometer.
 * Defined through the frequency of the measurement signal.
 * @ingroup ISO
 */
typedef enum {
    IR155_NORMAL_MODE       = 0,        /* */
    IR155_SPEEDSTART_MODE   = 1,        /* */
    IR155_UNDERVOLATGE_MODE = 2,        /* */
    IR155_IMDERROR_MODE     = 3,        /* */
    IR155_GROUNDERROR_MODE  = 4,        /* */
    IR155_SHORT_KL31        = 5,        /* */
    IR155_SHORT_KL15        = 6,        /* */
    IR155_UNDEFINED_FRQMAX  = 7,        /* illegal frequency detected*/
    IR155_DCM_CORRUPT       = 8,        /* corrupt signal measurement (e.g. T_on > T_periode, )*/
    IR155_DCM_NOSIGNAL      = 9,        /* no signal (e.g. if 100% -> wire break, if 0% -> shortcut to GND */
    IR155_UNKNOWN           = 15,       /* */
}IR155_SIGMODE_e;


/**
 * symbolic names for the different operating states Bender Isometer.
 * Defined through the duty cycle of the measurement signal.
 * @ingroup ISO
 */
typedef enum {
    IR155_RESIST_MEAS_GOOD      = 0,        /* valid normal measurement available and ok*/
    IR155_RESIST_ESTIM_GOOD     = 1,        /* SPEEDSTART estimation ok */
    IR155_RESIST_MEAS_BAD       = 2,        /* valid normal measurement available and nok*/
    IR155_RESIST_ESTIM_BAD      = 3,        /* SPEEDSTART estimation nok */
    IR155_RESIST_ESTIM_UNKNOWN  = 4,        /* SPEEDSTART estimation unknown */
    IR155_RESIST_MEAS_UNKNOWN   = 5,        /* valid normal measurement available but unknown */
    IR155_GROUND_ERROR          = 6,        /* */
    IR155_IMD_ERROR             = 7,        /* */
    IR155_STATE_UNDEFINED       = 8,        /* */
    IR155_SIGNALSHORT_KL15      = 9,        /* */
    IR155_SIGNALSHORT_KL31      = 10,       /* */
    IR155_MEAS_NOT_VALID        = 11,       /* Measurement results are not valid*/
    IR155_TODO_2                = 12,       /* */
    IR155_TODO_3                = 13,       /* */
    IR155_TODO_4                = 14,       /* */
    IR155_UNINITIALIZED         = 15,       /* */
}IR155_STATE_e;

/**
 * type definition for structure of insulation measurement
 *  Active and period timer ticks of duty cycle signal,
 *  resistance in [kOhm],
 *  duty cycle in [percentage],
 *  pinstate of OKHS pin,
 *  operating mode,
 *  operating state
 * @ingroup ISO
 */
typedef struct {
    TIM_DutyCycleType_s sigICU;
    uint32_t resistance;
    uint8_t dutycycle;
    IO_PIN_STATE_e OKHS_state;
    IR155_SIGMODE_e mode : 4;
    IR155_STATE_e state  : 4;
} IR155_MEASUREMENT_s;

/**
 * type definition for structure when debugging
 * insulation measurement values,
 * timestamp in [ms]
 * @ingroup ISO
 */
typedef struct {
    IR155_MEASUREMENT_s ir155_dc_Meas;
    uint32_t timestamp_ms;
} IR155_MEASUREMENT_HISTORY_s;

/**
 * type definition for structure when debugging
 * operating state,
 * measured insulation resistance,
 * pinstate of OKHS pin,
 * previous operating state,
 * previous measured insulation resistance,
 * previous pinstate of OKHS pin
 * @ingroup ISO
 */
typedef struct {
    IR155_STATE_e   state : 4;
    uint16_t        resistance : 3;
    uint16_t        OKHS_state : 1;
    IR155_STATE_e   state_old : 4;
    uint16_t        resistance_old : 3;
    uint16_t        OKHS_State_old : 1;
} IR155_INSULATION_s;


/*================== Constant and Variable Definitions ====================*/


/*================== Function Prototypes ==================================*/
/**
 * @brief Software initialization of Timer-modul
 * @param cycleTime     cyclic call time of IR155_MeasureResistance function
 */
extern void IR155_Init(uint8_t cycleTime);

/**
 * @brief Software DeInitalization of Timer-modul
 */
extern void IR155_DeInit(void);

/**
 * @brief Interface function which delivers the actual signal measurement (duty cyle) and evaluation.
 *        Use of intervals because of measuring and signal inaccuracy. The evaluated results are
 *        finally written in the database.
 * @param state             pointer to write measurement state into
 * @param resistance        pointer to write measured resistance into
 * @param ohks_state        pointer to write OHKS pin state into
 *
 * @return E_OK if no error occurred, otherwise E_NOT_OK
 */
extern STD_RETURN_TYPE_e IR155_MeasureResistance(IR155_STATE_e* state, uint32_t* resistance, IO_PIN_STATE_e* ohks_state);

/*================== Function Implementations =============================*/

#endif /* IR155_H_ */
