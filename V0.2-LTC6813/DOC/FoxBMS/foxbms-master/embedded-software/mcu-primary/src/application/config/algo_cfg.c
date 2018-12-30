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
 * @file    algo_cfg.c
 * @author  foxBMS Team
 * @date    18.12.2017 (date of creation)
 * @ingroup DRIVERS_CONF
 * @prefix  ALGO
 *
 * @brief   Configuration for the algorithm module
 *
 */

/*================== Includes =============================================*/
#include "algo_cfg.h"

#include "database.h"

/*================== Macros and Definitions ===============================*/
#if ALGO_TICK_MS > ISA_CURRENT_CYCLE_TIME_MS
#define NMBR_AVERAGES_CUR_1s           1000/ALGO_TICK_MS
#define NMBR_AVERAGES_CUR_5s           5000/ALGO_TICK_MS
#define NMBR_AVERAGES_CUR_10s          10000/ALGO_TICK_MS
#define NMBR_AVERAGES_CUR_30s          30000/ALGO_TICK_MS
#define NMBR_AVERAGES_CUR_60s          60000/ALGO_TICK_MS
#define NMBR_AVERAGES_CUR_cfg          MOVING_AVERAGE_DURATION_CURRENT_CONFIG_MS/ALGO_TICK_MS
#else
#define NMBR_AVERAGES_CUR_1s           1000/ISA_CURRENT_CYCLE_TIME_MS
#define NMBR_AVERAGES_CUR_5s           5000/ISA_CURRENT_CYCLE_TIME_MS
#define NMBR_AVERAGES_CUR_10s          10000/ISA_CURRENT_CYCLE_TIME_MS
#define NMBR_AVERAGES_CUR_30s          30000/ISA_CURRENT_CYCLE_TIME_MS
#define NMBR_AVERAGES_CUR_60s          60000/ISA_CURRENT_CYCLE_TIME_MS
#define NMBR_AVERAGES_CUR_cfg          MOVING_AVERAGE_DURATION_CURRENT_CONFIG_MS/ISA_CURRENT_CYCLE_TIME_MS
#endif

#if ALGO_TICK_MS > ISA_POWER_CYCLE_TIME_MS
#define NMBR_AVERAGES_POW_1s           1000/ALGO_TICK_MS
#define NMBR_AVERAGES_POW_5s           5000/ALGO_TICK_MS
#define NMBR_AVERAGES_POW_10s          10000/ALGO_TICK_MS
#define NMBR_AVERAGES_POW_30s          30000/ALGO_TICK_MS
#define NMBR_AVERAGES_POW_60s          60000/ALGO_TICK_MS
#define NMBR_AVERAGES_POW_cfg          MOVING_AVERAGE_DURATION_POWER_CONFIG_MS/ALGO_TICK_MS
#else
#define NMBR_AVERAGES_POW_1s           1000/ISA_POWER_CYCLE_TIME_MS
#define NMBR_AVERAGES_POW_5s           5000/ISA_POWER_CYCLE_TIME_MS
#define NMBR_AVERAGES_POW_10s          10000/ISA_POWER_CYCLE_TIME_MS
#define NMBR_AVERAGES_POW_30s          30000/ISA_POWER_CYCLE_TIME_MS
#define NMBR_AVERAGES_POW_60s          60000/ISA_POWER_CYCLE_TIME_MS
#define NMBR_AVERAGES_POW_cfg          MOVING_AVERAGE_DURATION_POWER_CONFIG_MS/ISA_POWER_CYCLE_TIME_MS
#endif

/*================== Constant and Variable Definitions ====================*/

/* Arrays in extern SDRAM to calculate moving mean current and power */

/* Check if minimum algo cycle time > current sensor sample time */
#if ALGO_TICK_MS > ISA_CURRENT_CYCLE_TIME_MS
#if MOVING_AVERAGE_DURATION_CURRENT_CONFIG_MS/ALGO_TICK_MS > 60000/ALGO_TICK_MS
/* If array length of configured time > 60s array take this array size */
static float MEM_EXT_SDRAM curValues[(MOVING_AVERAGE_DURATION_CURRENT_CONFIG_MS/ALGO_TICK_MS)+1] = { };
static uint32_t movMeanCurLength = (MOVING_AVERAGE_DURATION_CURRENT_CONFIG_MS/ALGO_TICK_MS)+1;
#else
/* Take array size of 60s moving average */
static float MEM_EXT_SDRAM curValues[(60000/ALGO_TICK_MS)+1] = { };
static uint32_t movMeanCurLength = (60000/ALGO_TICK_MS)+1;
#endif
#else
/* If array length of configured time > 60s array take this array size */
#if MOVING_AVERAGE_DURATION_CURRENT_CONFIG_MS/ISA_CURRENT_CYCLE_TIME_MS > 60000/ISA_CURRENT_CYCLE_TIME_MS
static float MEM_EXT_SDRAM curValues[(MOVING_AVERAGE_DURATION_CURRENT_CONFIG_MS/ISA_CURRENT_CYCLE_TIME_MS)+1] = { };
static uint32_t movMeanCurLength = (MOVING_AVERAGE_DURATION_CURRENT_CONFIG_MS/ISA_CURRENT_CYCLE_TIME_MS)+1;
#else
/* Take array size of 60s moving average */
static float MEM_EXT_SDRAM curValues[(60000/ISA_CURRENT_CYCLE_TIME_MS)+1] = { };
static uint32_t movMeanCurLength = (60000/ISA_CURRENT_CYCLE_TIME_MS)+1;
#endif
#endif

/* Check if minimum algo cycle time > current sensor sample time */
#if ALGO_TICK_MS > ISA_POWER_CYCLE_TIME_MS
#if MOVING_AVERAGE_DURATION_POWER_CONFIG_MS/ALGO_TICK_MS > 60000/ALGO_TICK_MS
/* If array length of configured time > 60s array take this array size */
static float MEM_EXT_SDRAM powValues[(MOVING_AVERAGE_DURATION_POWER_CONFIG_MS/ALGO_TICK_MS)+1] = { };
static uint32_t movMeanPowLength = (MOVING_AVERAGE_DURATION_POWER_CONFIG_MS/ALGO_TICK_MS)+1;
#else
/* Take array size of 60s moving average */
static float MEM_EXT_SDRAM powValues[(60000/ALGO_TICK_MS)+1] = { };
static uint32_t movMeanPowLength = (60000/ALGO_TICK_MS)+1;
#endif
#else
#if MOVING_AVERAGE_DURATION_POWER_CONFIG_MS/ISA_POWER_CYCLE_TIME_MS > 60000/ISA_POWER_CYCLE_TIME_MS
/* If array length of configured time > 60s array take this array size */
static float MEM_EXT_SDRAM powValues[(MOVING_AVERAGE_DURATION_POWER_CONFIG_MS/ISA_POWER_CYCLE_TIME_MS)+1] = { };
static uint32_t movMeanPowLength = (MOVING_AVERAGE_DURATION_POWER_CONFIG_MS/ISA_POWER_CYCLE_TIME_MS)+1;
#else
/* Take array size of 60s moving average */
static float MEM_EXT_SDRAM powValues[(60000/ISA_POWER_CYCLE_TIME_MS)+1] = { };
static uint32_t movMeanPowLength = (60000/ISA_POWER_CYCLE_TIME_MS)+1;
#endif
#endif


/* Pointer for current moving mean calculation */
static float *ptrMovMeanCur_new = &curValues[0];
static float *ptrMovMeanCur_1s = &curValues[0];
static float *ptrMovMeanCur_5s = &curValues[0];
static float *ptrMovMeanCur_10s = &curValues[0];
static float *ptrMovMeanCur_30s = &curValues[0];
static float *ptrMovMeanCur_60s = &curValues[0];
static float *ptrMovMeanCur_cfg = &curValues[0];

/* Pointer for power moving mean calculation */
static float *ptrMovMeanPow_new = &powValues[0];
static float *ptrMovMeanPow_1s = &powValues[0];
static float *ptrMovMeanPow_5s = &powValues[0];
static float *ptrMovMeanPow_10s = &powValues[0];
static float *ptrMovMeanPow_30s = &powValues[0];
static float *ptrMovMeanPow_60s = &powValues[0];
static float *ptrMovMeanPow_cfg = &powValues[0];

/*================== Function Prototypes ==================================*/
static void algo_movAverage(uint32_t algoIdx);

/*================== Function Implementations =============================*/

ALGO_TASKS_s algo_algorithms[] = {
    { ALGO_READY, 100, 1000, 0, &algo_movAverage },
};

const uint16_t algo_length = sizeof(algo_algorithms)/sizeof(algo_algorithms[0]);


static void algo_movAverage(uint32_t algoIdx) {
    static uint8_t curCounter = 0;
    static uint8_t powCounter = 0;
    static DATA_BLOCK_CURRENT_SENSOR_s curPow_tab;
    static DATA_BLOCK_MOVING_AVERAGE_s movMean_tab;
    static uint8_t curInit = 0;     /* bit0: 1s, bit1: 5s, bit2: 10s, bit3: 30s, bit4: 60s, bit5: cfg */
    static uint8_t powInit = 0;     /* bit0: 1s, bit1: 5s, bit2: 10s, bit3: 30s, bit4: 60s, bit5: cfg */
    static uint8_t newValues = 0;
    float divider = 0;

    DB_ReadBlock(&curPow_tab, DATA_BLOCK_ID_CURRENT_SENSOR);
    DB_ReadBlock(&movMean_tab, DATA_BLOCK_ID_MOV_AVERAGE);

    /* Check if new current value */
    if (curCounter != curPow_tab.newCurrent) {

        curCounter = curPow_tab.newCurrent;

        /* Check if valid value */
        if (curPow_tab.state_current == 0) {

            /* new Values -> Save later in database */
            newValues = 1;

            /* Add value to array and calculate new moving mean values */
            *ptrMovMeanCur_new = curPow_tab.current;

            /* Calculate new moving means - first add new value */
            divider = NMBR_AVERAGES_CUR_1s;
            movMean_tab.movAverage_current_1s += (*ptrMovMeanCur_new)/divider;
            divider = NMBR_AVERAGES_CUR_5s;
            movMean_tab.movAverage_current_5s += (*ptrMovMeanCur_new)/divider;
            divider = NMBR_AVERAGES_CUR_10s;
            movMean_tab.movAverage_current_10s += (*ptrMovMeanCur_new)/divider;
            divider = NMBR_AVERAGES_CUR_30s;
            movMean_tab.movAverage_current_30s += (*ptrMovMeanCur_new)/divider;
            divider = NMBR_AVERAGES_CUR_60s;
            movMean_tab.movAverage_current_60s += (*ptrMovMeanCur_new)/divider;
            divider = NMBR_AVERAGES_CUR_cfg;
            movMean_tab.movAverage_current_config += (*ptrMovMeanCur_new)/divider;

            /* Then, increment pointer and substract oldest value when respective window is filled with data */
            ptrMovMeanCur_new++;
            if ((curInit & 0x01) == 0x01) {
                divider = NMBR_AVERAGES_CUR_1s;
                movMean_tab.movAverage_current_1s -= ((*ptrMovMeanCur_1s)/divider);
                ptrMovMeanCur_1s++;
            } else {
                if (ptrMovMeanCur_new == &curValues[NMBR_AVERAGES_CUR_1s])
                    curInit |= 0x01;
            }
            if ((curInit & 0x02) == 0x02) {
                divider = NMBR_AVERAGES_CUR_5s;
                movMean_tab.movAverage_current_5s -= (*ptrMovMeanCur_5s)/divider;
                ptrMovMeanCur_5s++;
            } else {
                if (ptrMovMeanCur_new == &curValues[NMBR_AVERAGES_CUR_5s])
                    curInit |= 0x02;
            }
            if ((curInit & 0x04) == 0x04) {
                divider = NMBR_AVERAGES_CUR_10s;
                movMean_tab.movAverage_current_10s -= (*ptrMovMeanCur_10s)/divider;
                ptrMovMeanCur_10s++;
            } else {
                if (ptrMovMeanCur_new == &curValues[NMBR_AVERAGES_CUR_10s])
                    curInit |= 0x04;
            }
            if ((curInit & 0x08) == 0x08) {
                divider = NMBR_AVERAGES_CUR_30s;
                movMean_tab.movAverage_current_30s -= (*ptrMovMeanCur_30s)/divider;
                ptrMovMeanCur_30s++;
            } else {
                if (ptrMovMeanCur_new == &curValues[NMBR_AVERAGES_CUR_30s])
                    curInit |= 0x08;
            }
            if ((curInit & 0x10) == 0x10) {
                divider = NMBR_AVERAGES_CUR_60s;
                movMean_tab.movAverage_current_60s -= (*ptrMovMeanCur_60s)/divider;
                ptrMovMeanCur_60s++;
            } else {
                if (ptrMovMeanCur_new == &curValues[NMBR_AVERAGES_CUR_60s])
                    curInit |= 0x10;
            }
            if ((curInit & 0x20) == 0x20) {
                divider = NMBR_AVERAGES_CUR_cfg;
                movMean_tab.movAverage_current_config -= (*ptrMovMeanCur_cfg)/divider;
                ptrMovMeanCur_cfg++;
            } else {
                if (ptrMovMeanCur_new == &curValues[NMBR_AVERAGES_CUR_cfg])
                    curInit |= 0x20;
            }

            /* Check pointer for buffer overflow */
            if (ptrMovMeanCur_new > &curValues[movMeanCurLength-1])
                ptrMovMeanCur_new = &curValues[0];
            if (ptrMovMeanCur_1s > &curValues[movMeanCurLength-1])
                ptrMovMeanCur_1s = &curValues[0];
            if (ptrMovMeanCur_5s > &curValues[movMeanCurLength-1])
                ptrMovMeanCur_5s = &curValues[0];
            if (ptrMovMeanCur_10s > &curValues[movMeanCurLength-1])
                ptrMovMeanCur_10s = &curValues[0];
            if (ptrMovMeanCur_30s > &curValues[movMeanCurLength-1])
                ptrMovMeanCur_30s = &curValues[0];
            if (ptrMovMeanCur_60s > &curValues[movMeanCurLength-1])
                ptrMovMeanCur_60s = &curValues[0];
            if (ptrMovMeanCur_cfg > &curValues[movMeanCurLength-1])
                ptrMovMeanCur_cfg = &curValues[0];
        }
    }

    /* Check if new power value */
    if (powCounter != curPow_tab.newPower) {
        powCounter = curPow_tab.newPower;

        /* Check if valid value */
        if (curPow_tab.state_power == 0) {
            newValues = 1;

            /* Add value to array and calculate new moving mean values */
            *ptrMovMeanPow_new = curPow_tab.power;

            /* Calculate new moving means - first add new value */
            divider = NMBR_AVERAGES_POW_1s;
            movMean_tab.movAverage_power_1s += (*ptrMovMeanPow_new)/divider;
            divider = NMBR_AVERAGES_POW_5s;
            movMean_tab.movAverage_power_5s += (*ptrMovMeanPow_new)/divider;
            divider = NMBR_AVERAGES_POW_10s;
            movMean_tab.movAverage_power_10s += (*ptrMovMeanPow_new)/divider;
            divider = NMBR_AVERAGES_POW_30s;
            movMean_tab.movAverage_power_30s += (*ptrMovMeanPow_new)/divider;
            divider = NMBR_AVERAGES_POW_60s;
            movMean_tab.movAverage_power_60s += (*ptrMovMeanPow_new)/divider;
            divider = NMBR_AVERAGES_POW_cfg;
            movMean_tab.movAverage_power_config += (*ptrMovMeanPow_new)/divider;

            /* Then, increment pointer and substract oldest value when respective window is filled with data */
            ptrMovMeanPow_new++;
            if ((powInit & 0x01) == 0x01) {
                divider = NMBR_AVERAGES_POW_1s;
                movMean_tab.movAverage_power_1s -= ((*ptrMovMeanPow_1s)/divider);
                ptrMovMeanPow_1s++;
            } else {
                if (ptrMovMeanPow_new == &powValues[NMBR_AVERAGES_POW_1s])
                    powInit |= 0x01;
            }
            if ((powInit & 0x02) == 0x02) {
                divider = NMBR_AVERAGES_POW_5s;
                movMean_tab.movAverage_power_5s -= ((*ptrMovMeanPow_5s)/divider);
                ptrMovMeanPow_5s++;
            } else {
                if (ptrMovMeanPow_new == &powValues[NMBR_AVERAGES_POW_5s])
                    powInit |= 0x02;
            }
            if ((powInit & 0x04) == 0x04) {
                divider = NMBR_AVERAGES_POW_10s;
                movMean_tab.movAverage_power_10s -= ((*ptrMovMeanPow_10s)/divider);
                ptrMovMeanPow_10s++;
            } else {
                if (ptrMovMeanPow_new == &powValues[NMBR_AVERAGES_POW_10s])
                    powInit |= 0x04;
            }
            if ((powInit & 0x08) == 0x08) {
                divider = NMBR_AVERAGES_POW_30s;
                movMean_tab.movAverage_power_30s -= ((*ptrMovMeanPow_30s)/divider);
                ptrMovMeanPow_30s++;
            } else {
                if (ptrMovMeanPow_new == &powValues[NMBR_AVERAGES_POW_30s])
                    powInit |= 0x08;
            }
            if ((powInit & 0x10) == 0x10) {
                divider = NMBR_AVERAGES_POW_60s;
                movMean_tab.movAverage_power_60s -= ((*ptrMovMeanPow_60s)/divider);
                ptrMovMeanPow_60s++;
            } else {
                if (ptrMovMeanPow_new == &powValues[NMBR_AVERAGES_POW_60s])
                    powInit |= 0x10;
            }
            if ((powInit & 0x20) == 0x20) {
                divider = NMBR_AVERAGES_POW_cfg;
                movMean_tab.movAverage_power_config -= ((*ptrMovMeanPow_cfg)/divider);
                ptrMovMeanPow_cfg++;
            } else {
                if (ptrMovMeanPow_new == &powValues[NMBR_AVERAGES_POW_cfg])
                    powInit |= 0x20;
            }

            /* Check pointer for buffer overflow */
            if (ptrMovMeanPow_new > &powValues[movMeanPowLength-1])
                ptrMovMeanPow_new = &powValues[0];
            if (ptrMovMeanPow_1s > &powValues[movMeanPowLength-1])
                ptrMovMeanPow_1s = &powValues[0];
            if (ptrMovMeanPow_5s > &powValues[movMeanPowLength-1])
                ptrMovMeanPow_5s = &powValues[0];
            if (ptrMovMeanPow_10s > &powValues[movMeanPowLength-1])
                ptrMovMeanPow_10s = &powValues[0];
            if (ptrMovMeanPow_30s > &powValues[movMeanPowLength-1])
                ptrMovMeanPow_30s = &powValues[0];
            if (ptrMovMeanPow_60s > &powValues[movMeanPowLength-1])
                ptrMovMeanPow_60s = &powValues[0];
            if (ptrMovMeanPow_cfg > &powValues[movMeanPowLength-1])
                ptrMovMeanPow_cfg = &powValues[0];
        }
    }

    if (newValues == 1) {
        newValues = 0;

        DB_WriteBlock(&movMean_tab, DATA_BLOCK_ID_MOV_AVERAGE);
    }

    /* Only set task to ready state if it isn't blocked by the monitoring unit because of a runtime violation */
    if (algo_algorithms[algoIdx].state != ALGO_BLOCKED) {
        algo_algorithms[algoIdx].state = ALGO_READY;
    }
    return;
}
