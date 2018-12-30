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
 * @file    ir155.c
 * @author  foxBMS Team
 * @date    11.02.2014 (date of creation)
 * @ingroup DRIVERS
 * @prefix  IR155
 *
 * @brief   Driver for the isolation monitoring
 *
 * main file of bender isometer IR155 driver
 *
 */


/*================== Includes =============================================*/
#include "ir155.h"

#include "diag.h"

#if BUILD_MODULE_ENABLE_ISOGUARD == 1
/*================== Macros and Definitions ===============================*/
/* Defines for diagnosis handling */
#define DIAG_TIMER_OVERFLOW     0
#define DIAG_TIMER_NO_VALUE     1
#define DIAG_ERR_MEAS_CORRUPT   2
#define DIAG_ERR_DEVICE_ERROR   3
#define DIAG_ERR_SHORT_KL_15    4
#define DIAG_ERR_SHORT_KL_31    5
#define DIAG_ERR_UNDERVOLTAGE   6
#define DIAG_ERR_UNDEFINED      7


/*================== Constant and Variable Definitions ====================*/

/**
 *
 */
uint8_t MEM_BKP_SRAM isobender_grounderror = 0;

static TIM_DutyCycleType_s sig_ir155_dutycycle = {
        .ActiveTime = 0,
        .PeriodTime = 0,
};
static uint8_t measCycleTime = 0xFF;
static uint16_t hysteresisCounter = 0;

IR155_MEASUREMENT_s ir155_DC = {
        .sigICU.ActiveTime = 0,
        .sigICU.PeriodTime = 0,
        .resistance        = 0,
        .dutycycle         = 0,
        .OKHS_state        = 0,
        .mode              = IR155_UNKNOWN,
        .state             = IR155_UNINITIALIZED,
};

#ifdef IR155_HISTORYENABLE
unsigned char isoIR_his_idx = 0;
IR155_MEASUREMENT_HISTORY_s isoIR_his[IR155_HISTORY_LENGTH];  /* Initialization of array in ISOIR_Init() */
#endif

/*================== Function Prototypes ==================================*/
static IR155_SIGMODE_e IR155_GetMode(uint32_t signalperiod);

/*================== Function Implementations =============================*/

void IR155_Init(uint8_t cycleTime) {
#ifdef IR155_HISTORYENABLE
    for (int i = 0; i < IR155_HISTORY_LENGTH; i++) {
        isoIR_his[i].ir155_dc_Meas.sigICU.PeriodTime = 0;
        isoIR_his[i].ir155_dc_Meas.mode = IR155_UNKNOWN;
    }
#endif

    /* Timer peripheral initialization if not already done. */
    IR155_START_PWM_MEASUREMENT(&IR155_BENDER_IC_HANDLE);

    /* Check grounderror flag */
    if(isobender_grounderror != 0) {
        /* GROUNDERROR occured before shutting down */
        hysteresisCounter = IR155_WAIT_TIME_AFTER_GNDERROR;
    }
    else {
        /* Normal startup delay -> wait 2.2s until first measurement is trustworthy */
        hysteresisCounter = 2200;
    }

    measCycleTime = cycleTime;

    /* Set diagnosis message that measurement is not trustworthy */
    DIAG_Handler(DIAG_CH_ISOMETER_MEAS_INVALID, DIAG_EVENT_NOK, 0, NULL);
}



void IR155_DeInit(void) {
    /* Reset cycle time */
    measCycleTime = 0;

    /* IR155_MeasureResistance now always returns state IR155_MEAS_NOT_VALID
     * until IR155_Init() function is called again */
    hysteresisCounter = 1;

    /* Reset timer duty cycle struct */
    sig_ir155_dutycycle.ActiveTime = 0;
    sig_ir155_dutycycle.PeriodTime = 0;

    /* Set diagnosis message that measurement is not trustworthy */
    DIAG_Handler(DIAG_CH_ISOMETER_MEAS_INVALID, DIAG_EVENT_NOK, 0, NULL);
}

/**
 * Determines frequency-dependent operating state.
 * Use of intervals because of measuring and signal inaccuracy
 *
 * @param       signalperiod   period of captured signal in [1/75 ms], type uint32_t
 *
 * @return      ISO_SIGMODE_enum        bender operating state
 */
static IR155_SIGMODE_e IR155_GetMode(uint32_t signalperiod) {
    IR155_SIGMODE_e ret_val = IR155_UNKNOWN;

    if((signalperiod >= IR155_NORMALCONDITION_PERIODE_MIN) && (signalperiod < IR155_NORMALCONDITION_PERIODE_MAX))
        ret_val = IR155_NORMAL_MODE;
    else if((signalperiod >= IR155_UNDERVOLATGE_PERIODE_MIN) && (signalperiod < IR155_UNDERVOLATGE_PERIODE_MAX))
        ret_val = IR155_UNDERVOLATGE_MODE;        /* should not be detected as default threshold 0V, EOL Bender configurable! */
    else if((signalperiod >= IR155_SPEEDSTART_PERIODE_MIN) && (signalperiod < IR155_SPEEDSTART_PERIODE_MAX))
        ret_val = IR155_SPEEDSTART_MODE;
    else if((signalperiod >= IR155_IMDERROR_PERIODE_MIN) && (signalperiod < IR155_IMDERROR_PERIODE_MAX))
        ret_val = IR155_IMDERROR_MODE;
    else if((signalperiod >= IR155_GROUNDERROR_PERIODE_MIN) && (signalperiod < IR155_GROUNDERROR_PERIODE_MAX))
        ret_val = IR155_GROUNDERROR_MODE;
    else  if(signalperiod < IR155_GROUNDERROR_PERIODE_MIN)
        ret_val = IR155_UNDEFINED_FRQMAX;
    else  if(signalperiod >= IR155_PERIODE_MAX) {
        if (IR155_GET_MHS()) {
            ret_val = IR155_SHORT_KL15;        /* level at port: high */
        }
        else {
            ret_val = IR155_SHORT_KL31;     /* level at port: low */
        }
    }
    return ret_val;
}


STD_RETURN_TYPE_e IR155_MeasureResistance(IR155_STATE_e* state, uint32_t* resistance, IO_PIN_STATE_e* ohks_state) {
#ifdef IR155_HISTORYENABLE
    static IR155_INSULATION_s ir155_insulation_loc = {
            .state = IR155_UNINITIALIZED,
            .resistance = 0,
            .OKHS_state = 0,
            .state_old = IR155_UNINITIALIZED,
            .resistance_old = 0,
            .OKHS_State_old = 0,
    };
#endif

    /* Check parameters against null */
    if(state == NULL || resistance == NULL) {
        return E_NOT_OK;
    }

    TIM_RETURNTYPE_e retValTim = DIAG_TIM_ERROR;
    uint8_t retDiag = DIAG_HANDLER_RETURN_OK;
    STD_RETURN_TYPE_e retVal = E_OK;

    /* read value of IsoGuard Insulation_Good digital input */
    ir155_DC.OKHS_state = IR155_GET_OKHS();

    /* get actual active-time (t_ON)) and period from ICU */
    retValTim = IR155_GET_DUTYCYCLE(&sig_ir155_dutycycle);

    sig_ir155_dutycycle.ActiveTime *= TIM9_CLOCK_TICK_DURATION_IN_US * 75; /* in units of  (1/75) us */
    sig_ir155_dutycycle.PeriodTime *= TIM9_CLOCK_TICK_DURATION_IN_US * 75; /* in units of  (1/75) us */

    if(retValTim == DIAG_TIM_OK) {

        ir155_DC.sigICU.ActiveTime = sig_ir155_dutycycle.ActiveTime;            /* in units of  (1/75) us */
        ir155_DC.sigICU.PeriodTime = (sig_ir155_dutycycle.PeriodTime+500)/1000; /* in units of  (1/75) ms */

        DIAG_Handler(DIAG_CH_ISOMETER_TIM_ERROR, DIAG_EVENT_OK, DIAG_TIMER_NO_VALUE, NULL);
    }
    else {
        /* Error in call of ISOIR_GET_DUTYCYCLE or invalid values measured */
        if(retValTim == DIAG_TIM_OVERFLOW) {
            retDiag = DIAG_Handler(DIAG_CH_ISOMETER_TIM_ERROR, DIAG_EVENT_NOK, DIAG_TIMER_OVERFLOW, NULL);
        }
        else {
            retDiag = DIAG_Handler(DIAG_CH_ISOMETER_TIM_ERROR, DIAG_EVENT_NOK, DIAG_TIMER_NO_VALUE, NULL);
        }
        ir155_DC.sigICU.ActiveTime = 0;
        ir155_DC.sigICU.PeriodTime = 0;

        if(retDiag != 0)
            retVal = E_NOT_OK;
    }


    /* calculate actual duty cycle and frequency (which identifies mode)  */
    if(sig_ir155_dutycycle.ActiveTime < sig_ir155_dutycycle.PeriodTime) {

        /* both edges (high and low) have been detected within call time of this function */
        ir155_DC.dutycycle = ((ir155_DC .sigICU.ActiveTime)/ir155_DC .sigICU.PeriodTime + 5)/10;    /*  in units of % with rounding */
        ir155_DC.mode      = IR155_GetMode(ir155_DC .sigICU.PeriodTime);
    }
    else {
        /* this branch should never be entered if duty cycle-measurement works fine */
        if(sig_ir155_dutycycle.PeriodTime != 0) {
           /* implausible, SW-system error,Spikes...? */
            ir155_DC.mode      = IR155_DCM_CORRUPT;
        }
        else {
            /* no signal edges detected, */
            /* HW-system error? -> sensor wire break (pullup) */
            /* SW-system error? -> ICU not working */
            ir155_DC.mode      = IR155_DCM_NOSIGNAL;
        }
        /* Nevertheless, use pin level for low (0%) and high (100%) differentiation */
        if (IR155_GET_MHS())
            ir155_DC.dutycycle = 100;   /* max in units of % */
        else
            ir155_DC.dutycycle = 0;     /* min in units of % */

        retDiag = DIAG_Handler(DIAG_CH_ISOMETER_ERROR, DIAG_EVENT_NOK, DIAG_ERR_MEAS_CORRUPT, NULL);

        if(retDiag != 0)
            retVal = E_NOT_OK;
    }


    /* calculate resistance if possible which depends on mode of measurement output signal of isometer */
    /* (identified by frequency) */
    if (ir155_DC.mode == IR155_NORMAL_MODE ) {

        /* 5%..10%*: because of measuring inaccuracy it is used [3%..12%]*/
        if ((ir155_DC.dutycycle) >= 3 && (ir155_DC.dutycycle <= 12)) {
            /* insulation good, sensor working good */
            ir155_DC.state = IR155_RESIST_MEAS_GOOD;

            if(ir155_DC.dutycycle<=5) {
                ir155_DC.resistance = 106800;        /* max 106.8 MOhm */
            }
            else {
                ir155_DC.resistance= (90*1200)/(ir155_DC.dutycycle-5)-1200;
            }

        }
        else if (ir155_DC.dutycycle<3) {
            /* insulation unknown, sensor working bad */
            ir155_DC.state = IR155_RESIST_MEAS_UNKNOWN; /* sensor error dc < 3% */
            ir155_DC.resistance = 0;                    /* resistance not readable */
        }
        else if (ir155_DC .dutycycle>12 && ir155_DC.dutycycle<=90) {
          /* 10%..90%*: because of measuring inaccuracy it is used ]12%..90%]*/

            /* insulation getting worse, sensor working good */
            /* sensor dc < 10%..90% */
            ir155_DC.resistance= (90*1200)/(ir155_DC.dutycycle-5)-1200; /* resistance  20MOhm...70kOhm */
            if(ir155_DC.resistance > IR155_RESISTANCE_RESPONSE_VALUE)
                ir155_DC.state = IR155_RESIST_MEAS_GOOD;
            else
                ir155_DC.state = IR155_RESIST_MEAS_BAD;
        }
        else if (ir155_DC.dutycycle>90 && ir155_DC.dutycycle<=97) {
            /* 90%..95%*: because of measuring inaccuracy it is used ]90%..97%]*/

            /* insulation bad, sensor working good */
            /* sensor dc < 90%..95% */
            if(ir155_DC .dutycycle >= 95) {
                ir155_DC.state = IR155_RESIST_MEAS_BAD;
                ir155_DC.resistance = 1;                /* 0kOhm will be interpreted as "not readable", so use 1kOhm */
            }
            else {
                ir155_DC.state = IR155_RESIST_MEAS_BAD;
                ir155_DC.resistance= (90*1200)/(ir155_DC.dutycycle-5)-1200;    /* resistance  70kOhm..0kOhm */
            }
        }
        else if (ir155_DC.dutycycle>97 && ir155_DC.dutycycle<=100) { /* ]97%..100%] */
            /* insulation unknown, sensor working bad */
            ir155_DC.state = IR155_RESIST_MEAS_UNKNOWN;    /* sensor error dc > 95% */
            ir155_DC.resistance = 0;                        /* resistance not readable */
        }
    }
    else if (ir155_DC.mode == IR155_SPEEDSTART_MODE ) {
         /* SSD Mode (SPEED STARTUP) will be entered first after startup (within 2sec.) for resistance estimation */
        /* as NORMAL MODE (Normal Condition measurement) could take maximum time of 17.5sec. */

        /* 5%..10%*: because of measuring inaccuracy it is used [3%..12%]*/
        if (ir155_DC.dutycycle>=3 && ir155_DC.dutycycle<=12) {
            /* insulation good estimation, sensor working good */
            ir155_DC.state = IR155_RESIST_ESTIM_GOOD;    /* sensor error dc = 5..10% */
            ir155_DC.resistance= 200;                      /* R >= 200kOhm */
        }
        else if (ir155_DC.dutycycle>=88 && ir155_DC.dutycycle<=97) {
            /* 90%..95%*: because of measuring inaccuracy it is used [88%..97%]*/
            /* insulation bad, sensor working good */
            ir155_DC.state = IR155_RESIST_ESTIM_BAD;    /* sensor error dc = 90..95% */
            ir155_DC.resistance= 50;           /* R <= 50kOhm */
        }
        else {
            /* insulation unknown, sensor working good, dc measurement corrupt */
            ir155_DC.state = IR155_RESIST_ESTIM_UNKNOWN;    /* sensor error dc = 0..5%, dc = 10..90% or dc > 95% */
            ir155_DC.resistance = 0;                    /* resistance not readable */
        }
    }
    else if (ir155_DC.mode == IR155_GROUNDERROR_MODE) {
        /* Condition "KL31 fault"  is identified by base frequency 50Hz */
        /* and duty cycle  47.5% ... 52.5% */

        /* 47.5%..52.5%*: because of measuring inaccuracy it is used ]40%..60%]  */
        if (ir155_DC.dutycycle>40 && ir155_DC.dutycycle<=60) {
            /* insulation unknown, sensor working bad */
            ir155_DC.state = IR155_GROUND_ERROR;

            if(DIAG_HANDLER_RETURN_OK != DIAG_Handler(DIAG_CH_ISOMETER_GROUNDERROR, DIAG_EVENT_NOK, 0, NULL)) {

                /* Set grounderror flag */
                isobender_grounderror = 1;
            }
        }
        else {
            /* insulation unknown, sensor working good, DC measurement corrupt */
            ir155_DC.state = IR155_STATE_UNDEFINED;
        }
        ir155_DC.resistance = 0;          /* resistance unknown */

    }
    else if (ir155_DC.mode == IR155_IMDERROR_MODE) {
        /* Condition "device error" is identified by base frequency 40Hz */
        /* with dutycycle  47.5% ... 52.5% */

        /* 47.5%..52.5%*: because of measuring inaccuracy it is used ]40%..60%]  */
        if (ir155_DC.dutycycle>40 && ir155_DC.dutycycle<=60) {
           /* insulation unknown, sensor working bad */
            ir155_DC.state = IR155_IMD_ERROR;
        }
        else {
            /* insulation unknown, sensor working unknown, DC measurement corrupt */
            ir155_DC.state = IR155_STATE_UNDEFINED;
        }
        ir155_DC.resistance = 0;          /* resistance unknown */
        retDiag = DIAG_Handler(DIAG_CH_ISOMETER_ERROR, DIAG_EVENT_NOK, DIAG_ERR_DEVICE_ERROR, NULL);
    }
    else if (ir155_DC.mode == IR155_SHORT_KL15) {

        ir155_DC.state = IR155_SIGNALSHORT_KL15;
        ir155_DC.resistance = 0;
        retDiag = DIAG_Handler(DIAG_CH_ISOMETER_ERROR, DIAG_EVENT_NOK, DIAG_ERR_SHORT_KL_15, NULL);
    }
    else if (ir155_DC.mode == IR155_SHORT_KL31) {

        ir155_DC.state = IR155_SIGNALSHORT_KL31;
        ir155_DC.resistance = 0;
        retDiag = DIAG_Handler(DIAG_CH_ISOMETER_ERROR, DIAG_EVENT_NOK, DIAG_ERR_SHORT_KL_31, NULL);
    }
    else if (ir155_DC.mode == IR155_UNDERVOLATGE_MODE ||
             ir155_DC.mode == IR155_UNDEFINED_FRQMAX  ||
             ir155_DC.mode == IR155_UNKNOWN           ||
             ir155_DC.mode == IR155_DCM_CORRUPT       ||
             ir155_DC.mode == IR155_DCM_NOSIGNAL
            ) {
        /* insulation unknown, sensor working unknown, DC measurement corrupt */
        if(ir155_DC.mode == IR155_UNDERVOLATGE_MODE) {
            retDiag = DIAG_Handler(DIAG_CH_ISOMETER_ERROR, DIAG_EVENT_NOK, DIAG_ERR_UNDERVOLTAGE, NULL);
        }
        else {
            retDiag = DIAG_Handler(DIAG_CH_ISOMETER_ERROR, DIAG_EVENT_NOK, DIAG_ERR_UNDEFINED, NULL);
        }
        ir155_DC.state = IR155_STATE_UNDEFINED;
        ir155_DC.resistance = 0;
    }

    if(retDiag != 0)
        retVal = E_NOT_OK;

    /* Measurement is not valid, either because of startup delay
       or detected ground error before startup */
    if(hysteresisCounter > 0) {

        if(hysteresisCounter >= measCycleTime) {
            hysteresisCounter -= measCycleTime;
            ir155_DC.state = IR155_MEAS_NOT_VALID;
        }
        else {
            /* 0 < hysteresisCounter < measCycleTime
             * Measurement is valid from that moment on */
            hysteresisCounter = 0;
        }

        if(hysteresisCounter == 0) {
            /* If hysteresis is over, reset diag flag and reset grounderror flag */
            DIAG_Handler(DIAG_CH_ISOMETER_GROUNDERROR, DIAG_EVENT_OK, 0, NULL);
            DIAG_Handler(DIAG_CH_ISOMETER_MEAS_INVALID, DIAG_EVENT_OK, 0, NULL);

            isobender_grounderror = 0;
        }
    }

    /* Write measurement result into insulation_meas pointer */
    *resistance = ir155_DC.resistance;
    *state = ir155_DC.state;
    *ohks_state = ir155_DC.OKHS_state;


    /* Reduce diag-counter if no error occurred */
    if((retDiag == DIAG_HANDLER_RETURN_OK) ||
            ((retDiag != DIAG_HANDLER_RETURN_OK) && (ir155_DC .mode == IR155_GROUNDERROR_MODE))) {
        DIAG_Handler(DIAG_CH_ISOMETER_ERROR, DIAG_EVENT_OK, 0, 0);
    }

#ifdef IR155_HISTORYENABLE
    ir155_insulation_loc.state = ir155_DC.state;
    ir155_insulation_loc.OKHS_state = ir155_DC.OKHS_state;
    ir155_insulation_loc.resistance = ir155_DC.resistance;
    if (ir155_insulation_loc.state != ir155_insulation_loc.state_old ||
            ir155_insulation_loc.resistance != ir155_insulation_loc.resistance_old ||
            ir155_insulation_loc.OKHS_state != ir155_insulation_loc.OKHS_State_old) {

        isoIR_his[isoIR_his_idx].ir155_dc_Meas = ir155_DC;
        isoIR_his[isoIR_his_idx].ir155_dc_Meas.state = ir155_insulation_loc.state;
        isoIR_his[isoIR_his_idx].ir155_dc_Meas.OKHS_state = ir155_insulation_loc.OKHS_state;
        isoIR_his[isoIR_his_idx++].timestamp_ms = OS_getOSSysTick();

        if(isoIR_his_idx >= IR155_HISTORY_LENGTH)
            isoIR_his_idx = 0;

        ir155_insulation_loc.state_old = ir155_insulation_loc.state;
        ir155_insulation_loc.resistance_old = ir155_insulation_loc.resistance;
        ir155_insulation_loc.OKHS_State_old = ir155_insulation_loc.OKHS_state;
    }
#endif

    return retVal;
}
#endif
