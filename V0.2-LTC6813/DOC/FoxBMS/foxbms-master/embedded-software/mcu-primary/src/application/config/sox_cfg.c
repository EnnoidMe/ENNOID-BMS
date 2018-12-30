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
 * @file    sox_cfg.c
 * @author  foxBMS Team
 * @date    16.11.2015 (date of creation)
 * @ingroup APPLICATION_CONF
 * @prefix  SOX
 *
 * @brief   Configuration for SOX
 *
 */

/*================== Includes =============================================*/
#include "sox_cfg.h"

/*================== Macros and Definitions ===============================*/


/*================== Constant and Variable Definitions ====================*/

const SOX_SOF_CONFIG_s sox_sof_config_maxAllowedCurrent = {
        .I_ChargeMax_Cont       = SOX_CURRENT_MAX_CONTINUOUS_CHARGE,
        .I_DischaMax_Cont       = SOX_CURRENT_MAX_CONTINUOUS_DISCHARGE,
        .I_Limphome             = SOX_CURRENT_LIMP_HOME,
        .Cutoff_TLow_Discha     = SOX_TEMP_LOW_CUTOFF_DISCHARGE,
        .Limit_TLow_Discha      = SOX_TEMP_LOW_LIMIT_DISCHARGE,
        .Cutoff_TLow_Charge     = SOX_TEMP_LOW_CUTOFF_CHARGE,
        .Limit_TLow_Charge      = SOX_TEMP_LOW_LIMIT_CHARGE,
        .Cutoff_THigh_Discha    = SOX_TEMP_HIGH_CUTOFF_DISCHARGE,
        .Limit_THigh_Discha     = SOX_TEMP_HIGH_LIMIT_DISCHARGE,
        .Cutoff_THigh_Charge    = SOX_TEMP_HIGH_CUTOFF_CHARGE,
        .Limit_THigh_Charge     = SOX_TEMP_HIGH_LIMIT_CHARGE,
        .Limit_Soc_Charge       = SOX_SOC_LIMIT_CHARGE,
        .Cutoff_Soc_Charge      = SOX_SOC_CUTOFF_CHARGE,
        .Limit_Soc_Discha       = SOX_SOC_LIMIT_DISCHARGE,
        .Cutoff_Soc_Discha      = SOX_SOC_CUTOFF_DISCHARGE,
        .Limit_Voltage_Charge   = SOX_VOLT_LIMIT_CHARGE,
        .Cutoff_Voltage_Charge  = SOX_VOLT_CUTOFF_CHARGE,
        .Limit_Voltage_Discha   = SOX_VOLT_LIMIT_DISCHARGE,
        .Cutoff_Voltage_Discha  = SOX_VOLT_CUTOFF_DISCHARGE
};

const SOX_SOF_CONFIG_s sox_sof_config_MOL = {
        .I_ChargeMax_Cont       = SOX_MOL_CURRENT_MAX_CONTINUOUS_CHARGE,
        .I_DischaMax_Cont       = SOX_MOL_CURRENT_MAX_CONTINUOUS_DISCHARGE,
        .I_Limphome             = SOX_MOL_CURRENT_LIMP_HOME,
        .Cutoff_TLow_Discha     = SOX_MOL_TEMP_LOW_CUTOFF_DISCHARGE,
        .Limit_TLow_Discha      = SOX_MOL_TEMP_LOW_LIMIT_DISCHARGE,
        .Cutoff_TLow_Charge     = SOX_MOL_TEMP_LOW_CUTOFF_CHARGE,
        .Limit_TLow_Charge      = SOX_MOL_TEMP_LOW_LIMIT_CHARGE,
        .Cutoff_THigh_Discha    = SOX_MOL_TEMP_HIGH_CUTOFF_DISCHARGE,
        .Limit_THigh_Discha     = SOX_MOL_TEMP_HIGH_LIMIT_DISCHARGE,
        .Cutoff_THigh_Charge    = SOX_MOL_TEMP_HIGH_CUTOFF_CHARGE,
        .Limit_THigh_Charge     = SOX_MOL_TEMP_HIGH_LIMIT_CHARGE,
        .Limit_Soc_Charge       = SOX_MOL_SOC_LIMIT_CHARGE,
        .Cutoff_Soc_Charge      = SOX_MOL_SOC_CUTOFF_CHARGE,
        .Limit_Soc_Discha       = SOX_MOL_SOC_LIMIT_DISCHARGE,
        .Cutoff_Soc_Discha      = SOX_MOL_SOC_CUTOFF_DISCHARGE,
        .Limit_Voltage_Charge   = SOX_MOL_VOLT_LIMIT_CHARGE,
        .Cutoff_Voltage_Charge  = SOX_MOL_VOLT_CUTOFF_CHARGE,
        .Limit_Voltage_Discha   = SOX_MOL_VOLT_LIMIT_DISCHARGE,
        .Cutoff_Voltage_Discha  = SOX_MOL_VOLT_CUTOFF_DISCHARGE
};


const SOX_SOF_CONFIG_s sox_sof_config_RSL = {
        .I_ChargeMax_Cont       = SOX_RSL_CURRENT_MAX_CONTINUOUS_CHARGE,
        .I_DischaMax_Cont       = SOX_RSL_CURRENT_MAX_CONTINUOUS_DISCHARGE,
        .I_Limphome             = SOX_RSL_CURRENT_LIMP_HOME,
        .Cutoff_TLow_Discha     = SOX_RSL_TEMP_LOW_CUTOFF_DISCHARGE,
        .Limit_TLow_Discha      = SOX_RSL_TEMP_LOW_LIMIT_DISCHARGE,
        .Cutoff_TLow_Charge     = SOX_RSL_TEMP_LOW_CUTOFF_CHARGE,
        .Limit_TLow_Charge      = SOX_RSL_TEMP_LOW_LIMIT_CHARGE,
        .Cutoff_THigh_Discha    = SOX_RSL_TEMP_HIGH_CUTOFF_DISCHARGE,
        .Limit_THigh_Discha     = SOX_RSL_TEMP_HIGH_LIMIT_DISCHARGE,
        .Cutoff_THigh_Charge    = SOX_RSL_TEMP_HIGH_CUTOFF_CHARGE,
        .Limit_THigh_Charge     = SOX_RSL_TEMP_HIGH_LIMIT_CHARGE,
        .Limit_Soc_Charge       = SOX_RSL_SOC_LIMIT_CHARGE,
        .Cutoff_Soc_Charge      = SOX_RSL_SOC_CUTOFF_CHARGE,
        .Limit_Soc_Discha       = SOX_RSL_SOC_LIMIT_DISCHARGE,
        .Cutoff_Soc_Discha      = SOX_RSL_SOC_CUTOFF_DISCHARGE,
        .Limit_Voltage_Charge   = SOX_RSL_VOLT_LIMIT_CHARGE,
        .Cutoff_Voltage_Charge  = SOX_RSL_VOLT_CUTOFF_CHARGE,
        .Limit_Voltage_Discha   = SOX_RSL_VOLT_LIMIT_DISCHARGE,
        .Cutoff_Voltage_Discha  = SOX_RSL_VOLT_CUTOFF_DISCHARGE
};

const SOX_SOF_CONFIG_s sox_sof_config_MSL = {
        .I_ChargeMax_Cont       = SOX_MSL_CURRENT_MAX_CONTINUOUS_CHARGE,
        .I_DischaMax_Cont       = SOX_MSL_CURRENT_MAX_CONTINUOUS_DISCHARGE,
        .I_Limphome             = SOX_MSL_CURRENT_LIMP_HOME,
        .Cutoff_TLow_Discha     = SOX_MSL_TEMP_LOW_CUTOFF_DISCHARGE,
        .Limit_TLow_Discha      = SOX_MSL_TEMP_LOW_LIMIT_DISCHARGE,
        .Cutoff_TLow_Charge     = SOX_MSL_TEMP_LOW_CUTOFF_CHARGE,
        .Limit_TLow_Charge      = SOX_MSL_TEMP_LOW_LIMIT_CHARGE,
        .Cutoff_THigh_Discha    = SOX_MSL_TEMP_HIGH_CUTOFF_DISCHARGE,
        .Limit_THigh_Discha     = SOX_MSL_TEMP_HIGH_LIMIT_DISCHARGE,
        .Cutoff_THigh_Charge    = SOX_MSL_TEMP_HIGH_CUTOFF_CHARGE,
        .Limit_THigh_Charge     = SOX_MSL_TEMP_HIGH_LIMIT_CHARGE,
        .Limit_Soc_Charge       = SOX_MSL_SOC_LIMIT_CHARGE,
        .Cutoff_Soc_Charge      = SOX_MSL_SOC_CUTOFF_CHARGE,
        .Limit_Soc_Discha       = SOX_MSL_SOC_LIMIT_DISCHARGE,
        .Cutoff_Soc_Discha      = SOX_MSL_SOC_CUTOFF_DISCHARGE,
        .Limit_Voltage_Charge   = SOX_MSL_VOLT_LIMIT_CHARGE,
        .Cutoff_Voltage_Charge  = SOX_MSL_VOLT_CUTOFF_CHARGE,
        .Limit_Voltage_Discha   = SOX_MSL_VOLT_LIMIT_DISCHARGE,
        .Cutoff_Voltage_Discha  = SOX_MSL_VOLT_CUTOFF_DISCHARGE
};


/*================== Function Prototypes ==================================*/


/*================== Function Implementations =============================*/
