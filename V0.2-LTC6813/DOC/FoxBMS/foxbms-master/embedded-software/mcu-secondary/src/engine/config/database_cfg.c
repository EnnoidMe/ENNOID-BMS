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
 * @file    database_cfg.c
 * @author  foxBMS Team
 * @date    18.08.2015 (date of creation)
 * @ingroup ENGINE_CONF
 * @prefix  DATA
 *
 * @brief   Database configuration
 *
 * Configuration of database module
 *
 */

/*================== Includes =============================================*/
#include "database_cfg.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/

/**
 * data block: cell voltage
 */
DATA_BLOCK_CELLVOLTAGE_s data_block_cellvoltage[DOUBLE_BUFFERING];

/**
 * data block: cell temperature
 */
DATA_BLOCK_CELLTEMPERATURE_s data_block_celltemperature[DOUBLE_BUFFERING];

/**
 * data block: sox
 */
DATA_BLOCK_SOX_s data_block_sox[SINGLE_BUFFERING];

/**
 * data block: sof
 */
DATA_BLOCK_SOF_s data_block_sof[SINGLE_BUFFERING];

/**
 * data block: balancing control
 */
DATA_BLOCK_BALANCING_CONTROL_s data_block_control_balancing[DOUBLE_BUFFERING];

/**
 * data block: balancing feedback
 */
DATA_BLOCK_BALANCING_FEEDBACK_s data_block_feedback_balancing[DOUBLE_BUFFERING];

/**
 * data block: current measurement
 */
DATA_BLOCK_CURRENT_SENSOR_s data_block_curr_sensor[DOUBLE_BUFFERING];

/**
 * data block: ADC
 */
DATA_BLOCK_HW_INFO_s data_block_adc[SINGLE_BUFFERING];

/**
 * data block: can state request
 */
DATA_BLOCK_STATEREQUEST_s data_block_staterequest[SINGLE_BUFFERING];

/**
 * data block: LTC minimum and maximum values
 */
DATA_BLOCK_MINMAX_s data_block_minmax[DOUBLE_BUFFERING];

/**
 * data block: isometer measurement
 */
DATA_BLOCK_ISOMETER_s data_block_isometer[SINGLE_BUFFERING];

/**
 * data block: error flags
 */
DATA_BLOCK_ERRORSTATE_s data_block_errors[DOUBLE_BUFFERING];

/**
 * data block: maximum safety limit violations
 */
DATA_BLOCK_MSL_FLAG_s data_block_MSL[SINGLE_BUFFERING];

/**
 * data block: recommended safety limit violations
 */
DATA_BLOCK_RSL_FLAG_s data_block_RSL[SINGLE_BUFFERING];

/**
 * data block: maximum operating limit violations
 */
DATA_BLOCK_MOL_FLAG_s data_block_MOL[SINGLE_BUFFERING];

/**
 * data block: moving mean current and power
 */
DATA_BLOCK_MOVING_AVERAGE_s data_block_mov_average[DOUBLE_BUFFERING];

/**
 * data block: contactor feedback
 */
DATA_BLOCK_CONTFEEDBACK_s data_block_contfeedback[SINGLE_BUFFERING];

/**
 * data block: interlock feedback
 */
DATA_BLOCK_ILCKFEEDBACK_s data_block_ilckfeedback[SINGLE_BUFFERING];

/**
 * data block: slave control
 */
DATA_BLOCK_SLAVE_CONTROL_s data_block_slave_control[SINGLE_BUFFERING];

/**
 * data block: system state
 */
DATA_BLOCK_SYSTEMSTATE_s data_block_systemstate[SINGLE_BUFFERING];

/**
 * data block: open wire check
 */
DATA_BLOCK_OPENWIRE_s data_block_open_wire[DOUBLE_BUFFERING];

/**
 * data block: LTC diagnosis values
 */
DATA_BLOCK_LTC_DEVICE_PARAMETER_s data_block_ltc_diagnosis[SINGLE_BUFFERING];


/**
 * data block: LTC ADC accuracy verification
 */
DATA_BLOCK_LTC_ADC_ACCURACY_s data_block_ltc_adc_accuracy[SINGLE_BUFFERING];

/**
 * data block: LTC ADC accuracy verification
 */
DATA_BLOCK_ALLGPIOVOLTAGE_s data_block_ltc_allgpiovoltages[DOUBLE_BUFFERING];

/**
 * @brief channel configuration of database (data blocks)
 *
 * all data block managed by database are listed here (address,size,consistency type)
 *
 */
DATA_BASE_HEADER_s  data_base_header[] = {
    {
            (void*)(&data_block_cellvoltage[0]),
            sizeof(DATA_BLOCK_CELLVOLTAGE_s),
            DOUBLE_BUFFERING,
    },
    {
            (void*)(&data_block_celltemperature[0]),
            sizeof(DATA_BLOCK_CELLTEMPERATURE_s),
            DOUBLE_BUFFERING,
    },
    {
            (void*)(&data_block_sox[0]),
            sizeof(DATA_BLOCK_SOX_s),
            SINGLE_BUFFERING,
    },
    {
            (void*)(&data_block_control_balancing[0]),
            sizeof(DATA_BLOCK_BALANCING_CONTROL_s),
            DOUBLE_BUFFERING,
    },
    {
            (void*)(&data_block_feedback_balancing[0]),
            sizeof(DATA_BLOCK_BALANCING_FEEDBACK_s),
            DOUBLE_BUFFERING,
    },
    {
            (void*)(&data_block_curr_sensor[0]),
            sizeof(DATA_BLOCK_CURRENT_SENSOR_s),
            DOUBLE_BUFFERING,
    },
    {
            (void*)(&data_block_adc[0]),
            sizeof(DATA_BLOCK_HW_INFO_s),
            SINGLE_BUFFERING,
    },
    {
            (void*)(&data_block_staterequest[0]),
            sizeof(DATA_BLOCK_STATEREQUEST_s),
            SINGLE_BUFFERING,
    },
    {
            (void*)(&data_block_minmax[0]),
            sizeof(DATA_BLOCK_MINMAX_s),
            DOUBLE_BUFFERING,
    },
    {
            (void*)(&data_block_isometer[0]),
            sizeof(DATA_BLOCK_ISOMETER_s),
            SINGLE_BUFFERING,
    },
    {
            (void*)(&data_block_slave_control[0]),
            sizeof(DATA_BLOCK_SLAVE_CONTROL_s),
            SINGLE_BUFFERING,
    },
    {
            (void*)(&data_block_open_wire[0]),
            sizeof(DATA_BLOCK_OPENWIRE_s),
            DOUBLE_BUFFERING,
    },
    {
            (void*)(&data_block_ltc_diagnosis[0]),
            sizeof(DATA_BLOCK_LTC_DEVICE_PARAMETER_s),
            SINGLE_BUFFERING,
    },
    {
            (void*)(&data_block_ltc_adc_accuracy[0]),
            sizeof(DATA_BLOCK_LTC_ADC_ACCURACY_s),
            SINGLE_BUFFERING,
    },
    {
            (void*)(&data_block_errors[0]),
            sizeof(DATA_BLOCK_ERRORSTATE_s),
            DOUBLE_BUFFERING,
    },
    {
            (void*)(&data_block_MSL[0]),
            sizeof(DATA_BLOCK_MSL_FLAG_s),
            SINGLE_BUFFERING,
    },
    {
            (void*)(&data_block_RSL[0]),
            sizeof(DATA_BLOCK_RSL_FLAG_s),
            SINGLE_BUFFERING,
    },
    {
            (void*)(&data_block_MOL[0]),
            sizeof(DATA_BLOCK_MOL_FLAG_s),
            SINGLE_BUFFERING,
    },
    {
            (void*)(&data_block_mov_average[0]),
            sizeof(DATA_BLOCK_MOVING_AVERAGE_s),
            DOUBLE_BUFFERING,
    },
    {
            (void*)(&data_block_contfeedback[0]),
            sizeof(DATA_BLOCK_CONTFEEDBACK_s),
            SINGLE_BUFFERING,
    },
    {
            (void*)(&data_block_ilckfeedback[0]),
            sizeof(DATA_BLOCK_ILCKFEEDBACK_s),
            SINGLE_BUFFERING,
    },
    {
            (void*)(&data_block_systemstate[0]),
            sizeof(DATA_BLOCK_SYSTEMSTATE_s),
            SINGLE_BUFFERING,
    },
    {
            (void*)(&data_block_sof[0]),
            sizeof(DATA_BLOCK_SOF_s),
            SINGLE_BUFFERING,
    },
    {
            (void*)(&data_block_ltc_allgpiovoltages[0]),
            sizeof(DATA_BLOCK_ALLGPIOVOLTAGE_s),
            DOUBLE_BUFFERING,
    },
};

/**
 * @brief device configuration of database
 *
 * all attributes of device configuration are listed here (pointer to channel list, number of channels)
 */
const DATA_BASE_HEADER_DEV_s data_base_dev = {
    .nr_of_blockheader  = sizeof(data_base_header)/sizeof(DATA_BASE_HEADER_s),    /* number of blocks (and block headers) */
    .blockheaderptr     = &data_base_header[0],
};

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/
