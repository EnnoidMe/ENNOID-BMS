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
 * @file    diag_cfg.c
 * @author  foxBMS Team
 * @date    09.11.2015 (date of creation)
 * @ingroup ENGINE_CONF
 * @prefix  DIAG
 *
 * @brief   Diagnostic module configuration
 *
 * The configuration of the different diagnosis events defined in diag_cfg.h is set in the array
 * diag_ch_cfg[], e.g. initialization errors or runtime errors.
 *
 * Every entry of the diag_ch_cfg[] array consists of
 *  - name of the diagnosis event (defined in diag_cfg.h)
 *  - type of diagnosis event
 *  - diagnosis sensitivity (after how many occurrences event is counted as error)
 *  - enabling of the recording for diagnosis event
 *  - enabling of the diagnosis event
 *  - callback function for diagnosis event if wished, otherwise dummyfu
 *
 * The system monitoring configuration defined in diag_cfg.h is set in the array
 * diag_sysmon_ch_cfg[]. The system monitoring is at the moment only used for
 * supervising the cyclic/periodic tasks.
 *
 * Every entry of the diag_sysmon_ch_cfg[] consists of
 *  - enum of monitored object
 *  - type of monitored object (at the moment only DIAG_SYSMON_CYCLICTASK is supported)
 *  - maximum delay in [ms] in which the object needs to call the DIAG_SysMonNotify function defined in diag.c
 *  - enabling of the recording for system monitoring
 *  - enabling of the system monitoring for the monitored object
 *  - callback function if system monitoring notices an error if wished, otherwise dummyfu2
 */

/*================== Includes =============================================*/
#include "diag_cfg.h"

#include "database.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/

/**
 * Enable and Disable of Error Checks for Testing Purposes
 *
 * Each Bit enables or disables the diagnosis (checking of) the corresponding failure code
 * FIXME struct isn't used anywhere in the code at the moment.
 * FIXME delete if not needed
*/
DIAG_CODE_s diag_mask = {
        .GENERALmsk=0xFFFFFFFF,
        .CELLMONmsk=0xFFFFFFFF,
        .COMmsk=0xFFFFFFFF,
        .ADCmsk=0xFFFFFFFF,
};

/**
 * Callback function of diagnosis error events
 *
*/

static void dummyfu(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);

static void DIAG_MSL_overvoltage(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_RSL_overvoltage(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_MOL_overvoltage(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);

static void DIAG_MSL_undervoltage(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_RSL_undervoltage(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_MOL_undervoltage(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);

static void DIAG_MSL_overtemperaturecharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_RSL_overtemperaturecharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_MOL_overtemperaturecharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);

static void DIAG_MSL_overtemperaturedischarge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_RSL_overtemperaturedischarge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_MOL_overtemperaturedischarge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);

static void DIAG_MSL_undertemperaturecharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_RSL_undertemperaturecharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_MOL_undertemperaturecharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);

static void DIAG_MSL_undertemperaturedischarge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_RSL_undertemperaturedischarge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_MOL_undertemperaturedischarge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);

static void DIAG_MSL_overcurrentcharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_RSL_overcurrentcharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_MOL_overcurrentcharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);

static void DIAG_MSL_overcurrentdischarge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_RSL_overcurrentdischarge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_MOL_overcurrentdischarge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);

static void DIAG_error_cantiming(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_error_cantiming_cc(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);

static void DIAG_error_cancurrentsensor(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);

static void DIAG_error_ltcpec(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_error_ltcmux(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_error_ltcspi(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);

static void DIAG_error_contactormainplus(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_error_contactormainminus(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);

static void DIAG_error_contactorprecharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_error_contactorchargemainplus(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);

static void DIAG_error_contactorchargemainminus(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_error_contactorchargeprecharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);
static void DIAG_error_interlock(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);

static void DIAG_error_insulation(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event);

static void dummyfu2(DIAG_SYSMON_MODULE_ID_e ch_id);

void dummyfu(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    ;
}

void DIAG_MSL_overvoltage(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_MSL_FLAG_s msl_flags;
    DB_ReadBlock(&msl_flags, DATA_BLOCK_ID_MSL);
    if (event == DIAG_EVENT_RESET) {
        msl_flags.over_voltage = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        msl_flags.over_voltage = 1;
    }
    DB_WriteBlock(&msl_flags, DATA_BLOCK_ID_ERRORSTATE);
}

void DIAG_RSL_overvoltage(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_RSL_FLAG_s rsl_flags;
    DB_ReadBlock(&rsl_flags, DATA_BLOCK_ID_RSL);
    if (event == DIAG_EVENT_RESET) {
        rsl_flags.over_voltage = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        rsl_flags.over_voltage = 1;
    }
    DB_WriteBlock(&rsl_flags, DATA_BLOCK_ID_RSL);
}

void DIAG_MOL_overvoltage(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_MOL_FLAG_s mol_flags;
    DB_ReadBlock(&mol_flags, DATA_BLOCK_ID_MOL);
    if (event == DIAG_EVENT_RESET) {
        mol_flags.over_voltage = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        mol_flags.over_voltage = 1;
    }
    DB_WriteBlock(&mol_flags, DATA_BLOCK_ID_MOL);
}

void DIAG_MSL_undervoltage(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_MSL_FLAG_s msl_flags;
    DB_ReadBlock(&msl_flags, DATA_BLOCK_ID_MSL);
    if (event == DIAG_EVENT_RESET) {
        msl_flags.under_voltage = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        msl_flags.under_voltage = 1;
    }
    DB_WriteBlock(&msl_flags, DATA_BLOCK_ID_MSL);
}

void DIAG_RSL_undervoltage(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_RSL_FLAG_s rsl_flags;
    DB_ReadBlock(&rsl_flags, DATA_BLOCK_ID_RSL);
    if (event == DIAG_EVENT_RESET) {
        rsl_flags.under_voltage = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        rsl_flags.under_voltage = 1;
    }
    DB_WriteBlock(&rsl_flags, DATA_BLOCK_ID_RSL);
}

void DIAG_MOL_undervoltage(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_MOL_FLAG_s mol_flags;
    DB_ReadBlock(&mol_flags, DATA_BLOCK_ID_MOL);
    if (event == DIAG_EVENT_RESET) {
        mol_flags.under_voltage = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        mol_flags.under_voltage = 1;
    }
    DB_WriteBlock(&mol_flags, DATA_BLOCK_ID_MOL);
}

void DIAG_MSL_overtemperaturecharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_MSL_FLAG_s msl_flags;
    DB_ReadBlock(&msl_flags, DATA_BLOCK_ID_MSL);
    if (event == DIAG_EVENT_RESET) {
        msl_flags.over_temperature_charge = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        msl_flags.over_temperature_charge = 1;
    }
    DB_WriteBlock(&msl_flags, DATA_BLOCK_ID_MSL);
}

void DIAG_RSL_overtemperaturecharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_RSL_FLAG_s rsl_flags;
    DB_ReadBlock(&rsl_flags, DATA_BLOCK_ID_RSL);
    if (event == DIAG_EVENT_RESET) {
        rsl_flags.over_temperature_charge = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        rsl_flags.over_temperature_charge = 1;
    }
    DB_WriteBlock(&rsl_flags, DATA_BLOCK_ID_RSL);
}

void DIAG_MOL_overtemperaturecharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_MOL_FLAG_s mol_flags;
    DB_ReadBlock(&mol_flags, DATA_BLOCK_ID_MOL);
    if (event == DIAG_EVENT_RESET) {
        mol_flags.over_temperature_charge = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        mol_flags.over_temperature_charge = 1;
    }
    DB_WriteBlock(&mol_flags, DATA_BLOCK_ID_MOL);
}

void DIAG_MSL_overtemperaturedischarge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_MSL_FLAG_s msl_flags;
    DB_ReadBlock(&msl_flags, DATA_BLOCK_ID_MSL);
    if (event == DIAG_EVENT_RESET) {
        msl_flags.over_temperature_discharge = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        msl_flags.over_temperature_discharge = 1;
    }
    DB_WriteBlock(&msl_flags, DATA_BLOCK_ID_MSL);
}

void DIAG_RSL_overtemperaturedischarge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_RSL_FLAG_s rsl_flags;
    DB_ReadBlock(&rsl_flags, DATA_BLOCK_ID_RSL);
    if (event == DIAG_EVENT_RESET) {
        rsl_flags.over_temperature_discharge = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        rsl_flags.over_temperature_discharge = 1;
    }
    DB_WriteBlock(&rsl_flags, DATA_BLOCK_ID_RSL);
}

void DIAG_MOL_overtemperaturedischarge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_MOL_FLAG_s mol_flags;
    DB_ReadBlock(&mol_flags, DATA_BLOCK_ID_MOL);
    if (event == DIAG_EVENT_RESET) {
        mol_flags.over_temperature_discharge = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        mol_flags.over_temperature_discharge = 1;
    }
    DB_WriteBlock(&mol_flags, DATA_BLOCK_ID_MOL);

}

void DIAG_MSL_undertemperaturecharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_MSL_FLAG_s msl_flags;
    DB_ReadBlock(&msl_flags, DATA_BLOCK_ID_MSL);
    if (event == DIAG_EVENT_RESET) {
        msl_flags.under_temperature_charge = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        msl_flags.under_temperature_charge = 1;
    }
    DB_WriteBlock(&msl_flags, DATA_BLOCK_ID_MSL);
}

void DIAG_RSL_undertemperaturecharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_RSL_FLAG_s rsl_flags;
    DB_ReadBlock(&rsl_flags, DATA_BLOCK_ID_RSL);
    if (event == DIAG_EVENT_RESET) {
        rsl_flags.under_temperature_charge = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        rsl_flags.under_temperature_charge = 1;
    }
    DB_WriteBlock(&rsl_flags, DATA_BLOCK_ID_RSL);
}

void DIAG_MOL_undertemperaturecharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_MOL_FLAG_s mol_flags;
    DB_ReadBlock(&mol_flags, DATA_BLOCK_ID_MOL);
    if (event == DIAG_EVENT_RESET) {
        mol_flags.under_temperature_charge = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        mol_flags.under_temperature_charge = 1;
    }
    DB_WriteBlock(&mol_flags, DATA_BLOCK_ID_MOL);

}

void DIAG_MSL_undertemperaturedischarge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_MSL_FLAG_s msl_flags;
    DB_ReadBlock(&msl_flags, DATA_BLOCK_ID_MSL);
    if (event == DIAG_EVENT_RESET) {
        msl_flags.under_temperature_discharge = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        msl_flags.under_temperature_discharge = 1;
    }
    DB_WriteBlock(&msl_flags, DATA_BLOCK_ID_MSL);
}

void DIAG_RSL_undertemperaturedischarge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_RSL_FLAG_s rsl_flags;
    DB_ReadBlock(&rsl_flags, DATA_BLOCK_ID_RSL);
    if (event == DIAG_EVENT_RESET) {
        rsl_flags.under_temperature_discharge = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        rsl_flags.under_temperature_discharge = 1;
    }
    DB_WriteBlock(&rsl_flags, DATA_BLOCK_ID_RSL);
}

void DIAG_MOL_undertemperaturedischarge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_MOL_FLAG_s mol_flags;
    DB_ReadBlock(&mol_flags, DATA_BLOCK_ID_MOL);
    if (event == DIAG_EVENT_RESET) {
        mol_flags.under_temperature_discharge = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        mol_flags.under_temperature_discharge = 1;
    }
    DB_WriteBlock(&mol_flags, DATA_BLOCK_ID_MOL);
}

void DIAG_MSL_overcurrentcharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_MSL_FLAG_s msl_flags;
    DB_ReadBlock(&msl_flags, DATA_BLOCK_ID_MSL);
    if (event == DIAG_EVENT_RESET) {
        msl_flags.over_current_charge = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        msl_flags.over_current_charge = 1;
    }
    DB_WriteBlock(&msl_flags, DATA_BLOCK_ID_MSL);
}

void DIAG_RSL_overcurrentcharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_RSL_FLAG_s rsl_flags;
    DB_ReadBlock(&rsl_flags, DATA_BLOCK_ID_RSL);
    if (event == DIAG_EVENT_RESET) {
        rsl_flags.over_current_charge = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        rsl_flags.over_current_charge = 1;
    }
    DB_WriteBlock(&rsl_flags, DATA_BLOCK_ID_RSL);
}

void DIAG_MOL_overcurrentcharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_MOL_FLAG_s mol_flags;
    DB_ReadBlock(&mol_flags, DATA_BLOCK_ID_MOL);
    if (event == DIAG_EVENT_RESET) {
        mol_flags.over_current_charge = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        mol_flags.over_current_charge = 1;
    }
    DB_WriteBlock(&mol_flags, DATA_BLOCK_ID_MOL);
}

void DIAG_MSL_overcurrentdischarge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_MSL_FLAG_s msl_flags;
    DB_ReadBlock(&msl_flags, DATA_BLOCK_ID_MSL);
    if (event == DIAG_EVENT_RESET) {
        msl_flags.over_current_discharge = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        msl_flags.over_current_discharge = 1;
    }
    DB_WriteBlock(&msl_flags, DATA_BLOCK_ID_MSL);
}

void DIAG_RSL_overcurrentdischarge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_RSL_FLAG_s rsl_flags;
    DB_ReadBlock(&rsl_flags, DATA_BLOCK_ID_RSL);
    if (event == DIAG_EVENT_RESET) {
        rsl_flags.over_current_discharge = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        rsl_flags.over_current_discharge = 1;
    }
    DB_WriteBlock(&rsl_flags, DATA_BLOCK_ID_RSL);
}

void DIAG_MOL_overcurrentdischarge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_MOL_FLAG_s mol_flags;
    DB_ReadBlock(&mol_flags, DATA_BLOCK_ID_MOL);
    if (event == DIAG_EVENT_RESET) {
        mol_flags.over_current_discharge = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        mol_flags.over_current_discharge = 1;
    }
    DB_WriteBlock(&mol_flags, DATA_BLOCK_ID_MOL);
}

void DIAG_error_cantiming(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_ERRORSTATE_s error_flags;
    DB_ReadBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
    if (event == DIAG_EVENT_RESET) {
        error_flags.can_timing = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        error_flags.can_timing = 1;
    }
    DB_WriteBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
}

void DIAG_error_cantiming_cc(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_ERRORSTATE_s error_flags;
    DB_ReadBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
    if (event == DIAG_EVENT_RESET) {
        error_flags.can_timing_cc = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        error_flags.can_timing_cc = 1;
    }
    DB_WriteBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
}

void DIAG_error_cancurrentsensor(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_ERRORSTATE_s error_flags;
    DB_ReadBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
    if (event == DIAG_EVENT_RESET) {
        error_flags.currentsensorresponding = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        error_flags.currentsensorresponding = 1;
    }
    DB_WriteBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
}

void DIAG_error_ltcpec(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_ERRORSTATE_s error_flags;
    DB_ReadBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
    if (event == DIAG_EVENT_RESET) {
        error_flags.crc_error = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        error_flags.crc_error = 1;
    }
    DB_WriteBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
}

void DIAG_error_ltcmux(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_ERRORSTATE_s error_flags;
    DB_ReadBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
    if (event == DIAG_EVENT_RESET) {
        error_flags.mux_error = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        error_flags.mux_error = 1;
    }
    DB_WriteBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
}

void DIAG_error_ltcspi(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_ERRORSTATE_s error_flags;
    DB_ReadBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
    if (event == DIAG_EVENT_RESET) {
        error_flags.spi_error = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        error_flags.spi_error = 1;
    }
    DB_WriteBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
}

void DIAG_error_contactormainplus(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_ERRORSTATE_s error_flags;
    DB_ReadBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
    if (event == DIAG_EVENT_RESET) {
        error_flags.main_plus = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        error_flags.main_plus = 1;
    }
    DB_WriteBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
}

void DIAG_error_contactormainminus(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_ERRORSTATE_s error_flags;
    DB_ReadBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
    if (event == DIAG_EVENT_RESET) {
        error_flags.main_minus = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        error_flags.main_minus = 1;
    }
    DB_WriteBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
}

void DIAG_error_contactorprecharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_ERRORSTATE_s error_flags;
    DB_ReadBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
    if (event == DIAG_EVENT_RESET) {
        error_flags.precharge = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        error_flags.precharge = 1;
    }
    DB_WriteBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
}

void DIAG_error_contactorchargemainplus(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_ERRORSTATE_s error_flags;
    DB_ReadBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
    if (event == DIAG_EVENT_RESET) {
        error_flags.charge_main_plus = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        error_flags.charge_main_plus = 1;
    }
    DB_WriteBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
}

void DIAG_error_contactorchargemainminus(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_ERRORSTATE_s error_flags;
    DB_ReadBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
    if (event == DIAG_EVENT_RESET) {
        error_flags.charge_main_minus = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        error_flags.charge_main_minus = 1;
    }
    DB_WriteBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
}

void DIAG_error_contactorchargeprecharge(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_ERRORSTATE_s error_flags;
    DB_ReadBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
    if (event == DIAG_EVENT_RESET) {
        error_flags.charge_precharge = 0;
    }
    if (event == DIAG_EVENT_NOK) {
        error_flags.charge_precharge = 1;
    }
    DB_WriteBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
}

void DIAG_error_interlock(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_ERRORSTATE_s error_flags;
    DB_ReadBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
    if (event  ==  DIAG_EVENT_RESET) {
        error_flags.interlock = 0;
    }
    if (event==DIAG_EVENT_NOK) {
        error_flags.interlock = 1;
    }
    DB_WriteBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
}

void DIAG_error_insulation(DIAG_CH_ID_e ch_id, DIAG_EVENT_e event) {
    DATA_BLOCK_ERRORSTATE_s error_flags;
    DB_ReadBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
    if (event  ==  DIAG_EVENT_RESET) {
        error_flags.insulation_error = 0;
    }
    if (event==DIAG_EVENT_NOK) {
        error_flags.insulation_error = 1;
    }
    DB_WriteBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
}

/**
 * Callback function of system monitoring error events
 *
*/
void dummyfu2(DIAG_SYSMON_MODULE_ID_e ch_id) {
    ;
}


DIAG_CH_CFG_s  diag_ch_cfg[] = {

    /* OS-Framework and startup events */
    {DIAG_CH_FLASHCHECKSUM,                             "FLASHCHECKSUM",                        DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_BKPDIAG_FAILURE,                           "BKPDIAG",                              DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_WATCHDOGRESET_FAILURE,                     "WATCHDOGRESET",                        DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_POSTOSINIT_FAILURE,                        "POSTOSINIT",                           DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_CALIB_EEPR_FAILURE,                        "CALIB_EEPR",                           DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_CAN_INIT_FAILURE,                          "CAN_INIT",                             DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_VIC_INIT_FAILURE,                          "VIC_INIT",                             DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},

    /* HW-/SW-Runtime events */
    {DIAG_CH_DIV_BY_ZERO_FAILURE,                       "DIV_BY_ZERO",                          DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_UNDEF_INSTRUCTION_FAILURE,                 "UNDEF_INSTRUCTION",                    DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_DATA_BUS_FAILURE,                          "DATA_BUS_FAILURE",                     DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_INSTRUCTION_BUS_FAILURE,                   "INSTRUCTION_BUS",                      DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_HARDFAULT_NOTHANDLED,                      "HARDFAULT_NOTHANDLED",                 DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},

    {DIAG_CH_CONFIGASSERT,                              "CONFIGASSERT",                         DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_SYSTEMMONITORING_TIMEOUT,                  "SYSTEMMONITORING_TIMEOUT",             DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},


    /* Measurement events */
    {DIAG_CH_CANS_MAX_VALUE_VIOLATE,                    "CANS_MAX_VALUE_VIOLATE",               DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_CANS_MIN_VALUE_VIOLATE,                    "CANS_MIN_VALUE_VIOLATE",               DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_CANS_CAN_MOD_FAILURE,                      "CANS_CAN_MOD_FAILURE",                 DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},

#if BUILD_MODULE_ENABLE_ISOGUARD == 1
    {DIAG_CH_ISOMETER_TIM_ERROR,                        "ISOMETER_TIM_ERROR",                   DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_MID,               DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_ISOMETER_GROUNDERROR,                      "ISOMETER_GROUNDERROR",                 DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_ISOMETER_ERROR,                            "ISOMETER_ERROR",                       DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_MID,               DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_ISOMETER_MEAS_INVALID,                     "ISOMETER_MEAS_INVALID",                DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_INSULATION_ERROR,                          "INSULATION_ERROR",                     DIAG_GENERAL_TYPE, DIAG_ERROR_INSULATION_SENSITIVITY,        DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_error_insulation},
#else
    {DIAG_CH_ISOMETER_TIM_ERROR,                        "ISOMETER_TIM_ERROR",                   DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_MID,               DIAG_RECORDING_DISABLED, DIAG_DISABLED, dummyfu},
    {DIAG_CH_ISOMETER_GROUNDERROR,                      "ISOMETER_GROUNDERROR",                 DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_DISABLED, DIAG_DISABLED, dummyfu},
    {DIAG_CH_ISOMETER_ERROR,                            "ISOMETER_ERROR",                       DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_MID,               DIAG_RECORDING_DISABLED, DIAG_DISABLED, dummyfu},
    {DIAG_CH_ISOMETER_MEAS_INVALID,                     "ISOMETER_MEAS_INVALID",                DIAG_GENERAL_TYPE, DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_DISABLED, DIAG_DISABLED, dummyfu},
    {DIAG_CH_INSULATION_ERROR,                          "INSULATION_ERROR",                     DIAG_GENERAL_TYPE, DIAG_ERROR_INSULATION_SENSITIVITY,        DIAG_RECORDING_DISABLED, DIAG_DISABLED, dummyfu},
    #endif

    /* Under and over temperature, voltage and current at cell level */
    {DIAG_CH_CELLVOLTAGE_OVERVOLTAGE_MSL,             "CELLVOLT_OVERVOLT_MSL",            DIAG_GENERAL_TYPE, DIAG_ERROR_VOLTAGE_SENSITIVITY_MSL,           DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_MSL_overvoltage},
    {DIAG_CH_CELLVOLTAGE_OVERVOLTAGE_RSL,             "CELLVOLT_OVERVOLT_RSL",            DIAG_GENERAL_TYPE, DIAG_ERROR_VOLTAGE_SENSITIVITY_RSL,           DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_RSL_overvoltage},
    {DIAG_CH_CELLVOLTAGE_OVERVOLTAGE_MOL,           "CELLVOLT_OVERVOLT_MOL",              DIAG_GENERAL_TYPE, DIAG_ERROR_VOLTAGE_SENSITIVITY_MOL,           DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_MOL_overvoltage},

    {DIAG_CH_CELLVOLTAGE_UNDERVOLTAGE_MSL,            "CELLVOLT_UNDERVOLT_MSL",           DIAG_GENERAL_TYPE, DIAG_ERROR_VOLTAGE_SENSITIVITY_MSL,           DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_MSL_undervoltage},
    {DIAG_CH_CELLVOLTAGE_UNDERVOLTAGE_RSL,            "CELLVOLT_UNDERVOLT_RSL",           DIAG_GENERAL_TYPE, DIAG_ERROR_VOLTAGE_SENSITIVITY_RSL,           DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_RSL_undervoltage},
    {DIAG_CH_CELLVOLTAGE_UNDERVOLTAGE_MOL,          "CELLVOLT_UNDERVOLT_MOL",             DIAG_GENERAL_TYPE, DIAG_ERROR_VOLTAGE_SENSITIVITY_MOL,           DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_MOL_undervoltage},

    {DIAG_CH_TEMP_OVERTEMPERATURE_CHARGE_MSL,         "OVERTEMP_CHARGE_MSL",             DIAG_GENERAL_TYPE, DIAG_ERROR_TEMPERATURE_SENSITIVITY_MSL,       DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_MSL_overtemperaturecharge},
    {DIAG_CH_TEMP_OVERTEMPERATURE_CHARGE_RSL,         "OVERTEMP_CHARGE_RSL",             DIAG_GENERAL_TYPE, DIAG_ERROR_TEMPERATURE_SENSITIVITY_RSL,       DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_RSL_overtemperaturecharge},
    {DIAG_CH_TEMP_OVERTEMPERATURE_CHARGE_MOL,       "OVERTEMP_CHARGE_MOL",               DIAG_GENERAL_TYPE, DIAG_ERROR_TEMPERATURE_SENSITIVITY_MOL,       DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_MOL_overtemperaturecharge},

    {DIAG_CH_TEMP_OVERTEMPERATURE_DISCHARGE_MSL,      "OVERTEMP_DISCHARGE_MSL",          DIAG_GENERAL_TYPE, DIAG_ERROR_TEMPERATURE_SENSITIVITY_MSL,       DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_MSL_overtemperaturedischarge},
    {DIAG_CH_TEMP_OVERTEMPERATURE_DISCHARGE_RSL,      "OVERTEMP_DISCHARGE_RSL",          DIAG_GENERAL_TYPE, DIAG_ERROR_TEMPERATURE_SENSITIVITY_MSL,       DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_RSL_overtemperaturedischarge},
    {DIAG_CH_TEMP_OVERTEMPERATURE_DISCHARGE_MOL,    "OVERTEMP_DISCHARGE_MOL",            DIAG_GENERAL_TYPE, DIAG_ERROR_TEMPERATURE_SENSITIVITY_MSL,       DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_MOL_overtemperaturedischarge},

    {DIAG_CH_TEMP_UNDERTEMPERATURE_CHARGE_MSL,        "UNDERTEMP_CHARGE_MSL",            DIAG_GENERAL_TYPE, DIAG_ERROR_TEMPERATURE_SENSITIVITY_MSL,       DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_MSL_undertemperaturecharge},
    {DIAG_CH_TEMP_UNDERTEMPERATURE_CHARGE_RSL,        "UNDERTEMP_CHARGE_RSL",            DIAG_GENERAL_TYPE, DIAG_ERROR_TEMPERATURE_SENSITIVITY_MSL,       DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_RSL_undertemperaturecharge},
    {DIAG_CH_TEMP_UNDERTEMPERATURE_CHARGE_MOL,      "UNDERTEMP_CHARGE_MOL",              DIAG_GENERAL_TYPE, DIAG_ERROR_TEMPERATURE_SENSITIVITY_MSL,       DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_MOL_undertemperaturecharge},

    {DIAG_CH_TEMP_UNDERTEMPERATURE_DISCHARGE_MSL,     "UNDERTEMP_DISCHARGE_MSL",         DIAG_GENERAL_TYPE, DIAG_ERROR_TEMPERATURE_SENSITIVITY_MSL,       DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_MSL_undertemperaturedischarge},
    {DIAG_CH_TEMP_UNDERTEMPERATURE_DISCHARGE_RSL,     "UNDERTEMP_DISCHARGE_RSL",         DIAG_GENERAL_TYPE, DIAG_ERROR_TEMPERATURE_SENSITIVITY_MSL,       DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_RSL_undertemperaturedischarge},
    {DIAG_CH_TEMP_UNDERTEMPERATURE_DISCHARGE_MOL,   "UNDERTEMP_DISCHARGE_MOL",           DIAG_GENERAL_TYPE, DIAG_ERROR_TEMPERATURE_SENSITIVITY_MSL,       DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_MOL_undertemperaturedischarge},

    {DIAG_CH_OVERCURRENT_CHARGE_MSL,                  "OVERCUR_CHARGE_MSL",                 DIAG_GENERAL_TYPE, DIAG_ERROR_CURRENT_SENSITIVITY_MSL,           DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_MSL_overcurrentcharge},
    {DIAG_CH_OVERCURRENT_CHARGE_RSL,                  "OVERCUR_CHARGE_RSL",                 DIAG_GENERAL_TYPE, DIAG_ERROR_CURRENT_SENSITIVITY_MSL,           DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_RSL_overcurrentcharge},
    {DIAG_CH_OVERCURRENT_CHARGE_MOL,                "OVERCUR_CHARGE_MOL",                   DIAG_GENERAL_TYPE, DIAG_ERROR_CURRENT_SENSITIVITY_MSL,           DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_MOL_overcurrentcharge},

    {DIAG_CH_OVERCURRENT_DISCHARGE_MSL,               "OVERCUR_DISCHARGE_MSL",              DIAG_GENERAL_TYPE, DIAG_ERROR_CURRENT_SENSITIVITY_MSL,           DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_MSL_overcurrentdischarge},
    {DIAG_CH_OVERCURRENT_DISCHARGE_RSL,               "OVERCUR_DISCHARGE_RSL",              DIAG_GENERAL_TYPE, DIAG_ERROR_CURRENT_SENSITIVITY_MSL,           DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_RSL_overcurrentdischarge},
    {DIAG_CH_OVERCURRENT_DISCHARGE_MOL,             "OVERCUR_DISCHARGE_MOL",                DIAG_GENERAL_TYPE, DIAG_ERROR_CURRENT_SENSITIVITY_MSL,           DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_MOL_overcurrentdischarge},

    {DIAG_CH_LTC_SPI,                                   "LTC_SPI",                              DIAG_GENERAL_TYPE, DIAG_ERROR_LTC_SPI_SENSITIVITY,           DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_error_ltcspi},
    {DIAG_CH_LTC_PEC,                                   "LTC_PEC",                              DIAG_GENERAL_TYPE, DIAG_ERROR_LTC_PEC_SENSITIVITY,           DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_error_ltcpec},
    {DIAG_CH_LTC_MUX,                                   "LTC_MUX",                              DIAG_GENERAL_TYPE, DIAG_ERROR_LTC_MUX_SENSITIVITY,           DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_error_ltcmux},

    /* Communication events */
    {DIAG_CH_CAN_TIMING,                                "CAN_TIMING",                           DIAG_GENERAL_TYPE, DIAG_ERROR_CAN_TIMING_SENSITIVITY,        DIAG_RECORDING_ENABLED, DIAG_CAN_TIMING, DIAG_error_cantiming},
    {DIAG_CH_CAN_CC_RESPONDING,                         "CAN_CC_RESPONDING",                    DIAG_GENERAL_TYPE, DIAG_ERROR_CAN_TIMING_CC_SENSITIVITY,     DIAG_RECORDING_ENABLED, DIAG_CAN_SENSOR_PRESENT, DIAG_error_cantiming_cc},
    {DIAG_CH_CURRENT_SENSOR_RESPONDING,                 "CURRENT_SENSOR_RESPONDING",            DIAG_GENERAL_TYPE, DIAG_ERROR_CAN_SENSOR_SENSITIVITY,        DIAG_RECORDING_ENABLED, DIAG_CAN_SENSOR_PRESENT, DIAG_error_cancurrentsensor},

#if BUILD_MODULE_ENABLE_CONTACTOR == 1
    /* Contactor Damage Error */
    {DIAG_CH_CONTACTOR_DAMAGED,                         "CONTACTOR_DAMAGED",                    DIAG_CONT_TYPE,     DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_CONTACTOR_OPENING,                         "CONTACTOR_OPENING",                    DIAG_CONT_TYPE,     DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_CONTACTOR_CLOSING,                         "CONTACTOR_CLOSING",                    DIAG_CONT_TYPE,     DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},

    /* Contactor Feedback Error */
    {DIAG_CH_CONTACTOR_MAIN_PLUS_FEEDBACK,              "CONT_MAIN_PLUS_FEED",         DIAG_GENERAL_TYPE,  DIAG_ERROR_MAIN_PLUS_SENSITIVITY,      DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_error_contactormainplus},
    {DIAG_CH_CONTACTOR_MAIN_MINUS_FEEDBACK,             "CONT_MAIN_MINUS_FEED",        DIAG_GENERAL_TYPE,  DIAG_ERROR_MAIN_MINUS_SENSITIVITY,     DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_error_contactormainminus},
    {DIAG_CH_CONTACTOR_PRECHARGE_FEEDBACK,              "CONT_PRECHARGE_FEED",         DIAG_GENERAL_TYPE,  DIAG_ERROR_PRECHARGE_SENSITIVITY,      DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_error_contactorprecharge},
    {DIAG_CH_CONTACTOR_CHARGE_MAIN_PLUS_FEEDBACK,       "CONT_CHRGE_MAIN_PLUS_FEED",  DIAG_GENERAL_TYPE,  DIAG_ERROR_MAIN_PLUS_SENSITIVITY,      DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_error_contactorchargemainplus},
    {DIAG_CH_CONTACTOR_CHARGE_MAIN_MINUS_FEEDBACK,      "CONT_CHRGE_MAIN_MINUS_FEED", DIAG_GENERAL_TYPE,  DIAG_ERROR_MAIN_MINUS_SENSITIVITY,     DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_error_contactorchargemainminus},
    {DIAG_CH_CONTACTOR_CHARGE_PRECHARGE_FEEDBACK,       "CONT_CHRGE_PRECHARGE_FEED",  DIAG_GENERAL_TYPE,  DIAG_ERROR_PRECHARGE_SENSITIVITY,      DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_error_contactorchargeprecharge},
#else
    /* Contactor Damage Error */
    {DIAG_CH_CONTACTOR_DAMAGED,                         "CONTACTOR_DAMAGED",                    DIAG_CONT_TYPE,     DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_DISABLED, DIAG_DISABLED, dummyfu},
    {DIAG_CH_CONTACTOR_OPENING,                         "CONTACTOR_OPENING",                    DIAG_CONT_TYPE,     DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_DISABLED, DIAG_DISABLED, dummyfu},
    {DIAG_CH_CONTACTOR_CLOSING,                         "CONTACTOR_CLOSING",                    DIAG_CONT_TYPE,     DIAG_ERROR_SENSITIVITY_HIGH,              DIAG_RECORDING_DISABLED, DIAG_DISABLED, dummyfu},

    /* Contactor Feedback Error */
    {DIAG_CH_CONTACTOR_MAIN_PLUS_FEEDBACK,              "CONT_MAIN_PLUS_FEED",         DIAG_GENERAL_TYPE,  DIAG_ERROR_MAIN_PLUS_SENSITIVITY,      DIAG_RECORDING_DISABLED, DIAG_DISABLED, DIAG_error_contactormainplus},
    {DIAG_CH_CONTACTOR_MAIN_MINUS_FEEDBACK,             "CONT_MAIN_MINUS_FEED",        DIAG_GENERAL_TYPE,  DIAG_ERROR_MAIN_MINUS_SENSITIVITY,     DIAG_RECORDING_DISABLED, DIAG_DISABLED, DIAG_error_contactormainminus},
    {DIAG_CH_CONTACTOR_PRECHARGE_FEEDBACK,              "CONT_PRECHARGE_FEED",         DIAG_GENERAL_TYPE,  DIAG_ERROR_PRECHARGE_SENSITIVITY,      DIAG_RECORDING_DISABLED, DIAG_DISABLED, DIAG_error_contactorprecharge},
    {DIAG_CH_CONTACTOR_CHARGE_MAIN_PLUS_FEEDBACK,       "CONT_CHRGE_MAIN_PLUS_FEED",  DIAG_GENERAL_TYPE,  DIAG_ERROR_MAIN_PLUS_SENSITIVITY,      DIAG_RECORDING_DISABLED, DIAG_DISABLED, DIAG_error_contactorchargemainplus},
    {DIAG_CH_CONTACTOR_CHARGE_MAIN_MINUS_FEEDBACK,      "CONT_CHRGE_MAIN_MINUS_FEED", DIAG_GENERAL_TYPE,  DIAG_ERROR_MAIN_MINUS_SENSITIVITY,     DIAG_RECORDING_DISABLED, DIAG_DISABLED, DIAG_error_contactorchargemainminus},
    {DIAG_CH_CONTACTOR_CHARGE_PRECHARGE_FEEDBACK,       "CONT_CHRGE_PRECHARGE_FEED",  DIAG_GENERAL_TYPE,  DIAG_ERROR_PRECHARGE_SENSITIVITY,      DIAG_RECORDING_DISABLED, DIAG_DISABLED, DIAG_error_contactorchargeprecharge},
#endif

#if BUILD_MODULE_ENABLE_ILCK == 1
    /* Interlock Feedback Error */
    {DIAG_CH_INTERLOCK_FEEDBACK,                        "INTERLOCK_FEEDBACK",                   DIAG_GENERAL_TYPE,  DIAG_ERROR_INTERLOCK_SENSITIVITY,       DIAG_RECORDING_ENABLED, DIAG_ENABLED, DIAG_error_interlock},
#else
    {DIAG_CH_INTERLOCK_FEEDBACK,                        "INTERLOCK_FEEDBACK",                   DIAG_GENERAL_TYPE,  DIAG_ERROR_INTERLOCK_SENSITIVITY,       DIAG_RECORDING_DISABLED, DIAG_DISABLED, DIAG_error_interlock},
#endif

    /* Slave PCB temperature errors for under and over temperature */
    {DIAG_CH_SLAVE_PCB_UNDERTEMPERATURE_MSL,          "SLAVE_PCB_UNDERTEMP_MSL",     DIAG_GENERAL_TYPE,    DIAG_ERROR_SLAVE_TEMP_SENSITIVITY_MSL,   DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_SLAVE_PCB_UNDERTEMPERATURE_RSL,          "SLAVE_PCB_UNDERTEMP_RSL",     DIAG_GENERAL_TYPE,    DIAG_ERROR_SLAVE_TEMP_SENSITIVITY_RSL,   DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_SLAVE_PCB_UNDERTEMPERATURE_MOL,        "SLAVE_PCB_UNDERTEMP_MOL",       DIAG_GENERAL_TYPE,  DIAG_ERROR_SLAVE_TEMP_SENSITIVITY_MOL, DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},

    {DIAG_CH_SLAVE_PCB_OVERTEMPERATURE_MSL,           "SLAVE_PCB_OVERTEMP_MSL",      DIAG_GENERAL_TYPE,    DIAG_ERROR_SLAVE_TEMP_SENSITIVITY_MSL,   DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_SLAVE_PCB_OVERTEMPERATURE_RSL,           "SLAVE_PCB_OVERTEMP_RSL",      DIAG_GENERAL_TYPE,    DIAG_ERROR_SLAVE_TEMP_SENSITIVITY_RSL,   DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},
    {DIAG_CH_SLAVE_PCB_OVERTEMPERATURE_MOL,         "SLAVE_PCB_OVERTEMP_MOL",        DIAG_GENERAL_TYPE,  DIAG_ERROR_SLAVE_TEMP_SENSITIVITY_MOL, DIAG_RECORDING_ENABLED, DIAG_ENABLED, dummyfu},

};


DIAG_SYSMON_CH_CFG_s diag_sysmon_ch_cfg[] = {
    {DIAG_SYSMON_DATABASE_ID,       DIAG_SYSMON_CYCLICTASK,  10, DIAG_RECORDING_ENABLED, DIAG_SYSMON_HANDLING_SWITCHOFFCONTACTOR, DIAG_ENABLED, dummyfu2},
    {DIAG_SYSMON_SYS_ID,            DIAG_SYSMON_CYCLICTASK,  20, DIAG_RECORDING_ENABLED, DIAG_SYSMON_HANDLING_SWITCHOFFCONTACTOR, DIAG_ENABLED, dummyfu2},
    {DIAG_SYSMON_BMS_ID,            DIAG_SYSMON_CYCLICTASK,  20, DIAG_RECORDING_ENABLED, DIAG_SYSMON_HANDLING_SWITCHOFFCONTACTOR, DIAG_ENABLED, dummyfu2},

#if BUILD_MODULE_ENABLE_CONTACTOR == 1
    {DIAG_SYSMON_CONT_ID,           DIAG_SYSMON_CYCLICTASK,  20, DIAG_RECORDING_ENABLED, DIAG_SYSMON_HANDLING_SWITCHOFFCONTACTOR, DIAG_ENABLED, dummyfu2},
#else
    {DIAG_SYSMON_CONT_ID,           DIAG_SYSMON_CYCLICTASK,  20, DIAG_RECORDING_DISABLED, DIAG_SYSMON_HANDLING_SWITCHOFFCONTACTOR, DIAG_DISABLED, dummyfu2},
#endif

#if BUILD_MODULE_ENABLE_ILCK == 1
    {DIAG_SYSMON_ILCK_ID,           DIAG_SYSMON_CYCLICTASK,  20, DIAG_RECORDING_ENABLED, DIAG_SYSMON_HANDLING_SWITCHOFFCONTACTOR, DIAG_ENABLED, dummyfu2},
#else
    {DIAG_SYSMON_ILCK_ID,           DIAG_SYSMON_CYCLICTASK,  20, DIAG_RECORDING_DISABLED, DIAG_SYSMON_HANDLING_SWITCHOFFCONTACTOR, DIAG_DISABLED, dummyfu2},
#endif
    {DIAG_SYSMON_LTC_ID,            DIAG_SYSMON_CYCLICTASK,   5, DIAG_RECORDING_ENABLED, DIAG_SYSMON_HANDLING_SWITCHOFFCONTACTOR, DIAG_ENABLED, dummyfu2},

#if BUILD_MODULE_ENABLE_ISOGUARD == 1
    {DIAG_SYSMON_ISOGUARD_ID,       DIAG_SYSMON_CYCLICTASK, 400, DIAG_RECORDING_ENABLED, DIAG_SYSMON_HANDLING_SWITCHOFFCONTACTOR, DIAG_ENABLED, dummyfu2},
#else
    {DIAG_SYSMON_ISOGUARD_ID,       DIAG_SYSMON_CYCLICTASK, 400, DIAG_RECORDING_DISABLED, DIAG_SYSMON_HANDLING_SWITCHOFFCONTACTOR, DIAG_DISABLED, dummyfu2},
#endif

    {DIAG_SYSMON_CANS_ID,           DIAG_SYSMON_CYCLICTASK,  20, DIAG_RECORDING_ENABLED, DIAG_SYSMON_HANDLING_SWITCHOFFCONTACTOR, DIAG_ENABLED, dummyfu2},
    {DIAG_SYSMON_APPL_CYCLIC_1ms,   DIAG_SYSMON_CYCLICTASK,  20, DIAG_RECORDING_ENABLED, DIAG_SYSMON_HANDLING_SWITCHOFFCONTACTOR, DIAG_ENABLED, dummyfu2},
    {DIAG_SYSMON_APPL_CYCLIC_10ms,  DIAG_SYSMON_CYCLICTASK,  20, DIAG_RECORDING_ENABLED, DIAG_SYSMON_HANDLING_SWITCHOFFCONTACTOR, DIAG_ENABLED, dummyfu2},
    {DIAG_SYSMON_APPL_CYCLIC_100ms, DIAG_SYSMON_CYCLICTASK, 200, DIAG_RECORDING_ENABLED, DIAG_SYSMON_HANDLING_SWITCHOFFCONTACTOR, DIAG_ENABLED, dummyfu2},
};


DIAG_DEV_s  diag_dev = {
    .nr_of_ch   = sizeof(diag_ch_cfg)/sizeof(DIAG_CH_CFG_s),
    .ch_cfg     = &diag_ch_cfg[0],
};

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/
