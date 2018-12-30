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
 * @file    sox.c
 * @author  foxBMS Team
 * @date    13.11.2015 (date of creation)
 * @ingroup APPLICATION
 * @prefix  SOX
 *
 * @brief   SOX module responsible for calculation of current derating and of SOC
 *
 */



/*================== Includes =============================================*/
#include "sox.h"

#include "database.h"
#include "batterycell_cfg.h"
#include "batterysystem_cfg.h"
#include "nvramhandler.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/
static SOX_STATE_s sox_state = {
    .sensor_cc_used         = 0,
    .cc_scaling               = 0.0,
    .counter                = 0,
};

static DATA_BLOCK_CURRENT_SENSOR_s sox_current_tab;
static DATA_BLOCK_MINMAX_s cellminmax;
static DATA_BLOCK_SOX_s sox;
static DATA_BLOCK_SOF_s sof;
static DATA_BLOCK_CONTFEEDBACK_s contfeedbacktab;

static uint32_t soc_previous_current_timestamp = 0;
static uint32_t soc_previous_current_timestamp_cc = 0;


/** @{
 * module-local static Variables that are calculated at startup and used later to avoid divisions at runtime
 */
static SOF_curve_s sofCurveRecOperatingCurrent;
static SOF_curve_s sofCurve_MOL;
static SOF_curve_s sofCurve_RSL;
static SOF_curve_s sofCurve_MSL;

static SOX_SOF_s sof_recOperatingCurrent;
static SOX_SOF_s sof_mol_Level;
static SOX_SOF_s sof_rsl_Level;
static SOX_SOF_s sof_msl_Level;

/*================== Function Prototypes ==================================*/
static void SOF_CalculateCurves(const SOX_SOF_CONFIG_s *configLimitValues, SOF_curve_s* calcCurveValues);
static void SOF_Calculate(int16_t maxtemp, int16_t mintemp, uint16_t maxvolt, uint16_t minvolt, uint16_t maxsoc, uint16_t minsoc);
static void SOF_CalculateVoltageBased(float MinVoltage,float MaxVoltage, SOX_SOF_s *ResultValues, const SOX_SOF_CONFIG_s *configLimitValues, SOF_curve_s* calcCurveValues);
static void SOF_CalculateSocBased(float MinSoc,float MaxSoc, SOX_SOF_s *ResultValues, const SOX_SOF_CONFIG_s *configLimitValues, SOF_curve_s* calcCurveValues);
static void SOF_CalculateTemperatureBased(float MinTemp,float MaxTemp, SOX_SOF_s *ResultValues, const SOX_SOF_CONFIG_s *configLimitValues, SOF_curve_s* calcCurveValues);
static void SOF_MinimumOfThreeSofValues(SOX_SOF_s Ubased, SOX_SOF_s Sbased, SOX_SOF_s Tbased, SOX_SOF_s *resultValues);
static float SOF_MinimumOfThreeValues (float value1,float value2, float value3);
static float SOC_GetFromVoltage(float voltage);

/*================== Function Implementations =============================*/

void SOC_Init(uint8_t cc_present) {
    SOX_SOC_s soc = {50.0, 50.0, 50.0};
    DATA_BLOCK_ERRORSTATE_s error_flags;


    DB_ReadBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
    DB_ReadBlock(&sox_current_tab, DATA_BLOCK_ID_CURRENT_SENSOR);
    NVM_getSOC(&soc);

    if (cc_present == TRUE) {
        soc_previous_current_timestamp_cc = sox_current_tab.timestamp_cc;
        error_flags.can_cc_used = 1;
        sox_state.sensor_cc_used = TRUE;

        if (POSITIVE_DISCHARGE_CURRENT == TRUE) {
            sox_state.cc_scaling = soc.mean + 100.0*sox_current_tab.current_counter/(3600.0*(SOX_CELL_CAPACITY/1000.0));
        } else {
            sox_state.cc_scaling = soc.mean - 100.0*sox_current_tab.current_counter/(3600.0*(SOX_CELL_CAPACITY/1000.0));
        }


        sox.soc_mean = soc.mean;
        sox.soc_min = soc.min;
        sox.soc_max = soc.max;
        if (sox.soc_mean > 100.0) { sox.soc_mean = 100.0; }
        if (sox.soc_mean < 0.0)   { sox.soc_mean = 0.0;   }
        if (sox.soc_min > 100.0)  { sox.soc_min = 100.0;  }
        if (sox.soc_min < 0.0)    { sox.soc_min = 0.0;    }
        if (sox.soc_max > 100.0)  { sox.soc_max = 100.0;  }
        if (sox.soc_max < 0.0)    { sox.soc_max = 0.0;    }
        /* Alternatively, SOC can be initialized with {V,SOC} lookup table if available */
        /* with the function SOC_Init_Lookup_Table() */
        sox.state = 0;
        sox.timestamp = 0;
        sox.previous_timestamp = 0;
    }
    else {
        soc_previous_current_timestamp = sox_current_tab.timestamp_cur;
        error_flags.can_cc_used = 0;
        sox_state.sensor_cc_used = FALSE;

    }
    DB_WriteBlock(&error_flags, DATA_BLOCK_ID_ERRORSTATE);
    DB_WriteBlock(&sox, DATA_BLOCK_ID_SOX);
}

void SOC_SetValue(float soc_value_min, float soc_value_max, float soc_value_mean) {
    SOX_SOC_s soc = {50.0, 50.0, 50.0};

    if (soc_value_min < 0.0) {
        soc_value_min = 0.0;
    }
    if (soc_value_min > 100.0) {
        soc_value_min = 100.0;
    }

    if (soc_value_max < 0.0) {
        soc_value_max = 0.0;
    }
    if (soc_value_max > 100.0) {
        soc_value_max = 100.0;
    }

    if (soc_value_mean < 0.0) {
        soc_value_mean = 0.0;
    }
    if (soc_value_mean > 100.0) {
        soc_value_mean = 100.0;
    }

    if (sox_state.sensor_cc_used == FALSE) {
        soc.mean = soc_value_mean;
        soc.min = soc_value_min;
        soc.max = soc_value_max;
        NVM_setSOC(&soc);

        sox.soc_mean = soc.mean;
        sox.soc_min = soc.min;
        sox.soc_max = soc.max;
        sox.state = 0;
        sox.timestamp = 0;
        sox.previous_timestamp = 0;

        DB_WriteBlock(&sox,DATA_BLOCK_ID_SOX);

    }
    else {
        DB_ReadBlock(&sox_current_tab, DATA_BLOCK_ID_CURRENT_SENSOR);
        soc.mean = soc_value_mean;
        soc.min = soc_value_min;
        soc.max = soc_value_max;

        if (POSITIVE_DISCHARGE_CURRENT == TRUE) {
            sox_state.cc_scaling = soc.mean + 100.0*sox_current_tab.current_counter/(3600.0*(SOX_CELL_CAPACITY/1000.0));
        } else {
            sox_state.cc_scaling = soc.mean - 100.0*sox_current_tab.current_counter/(3600.0*(SOX_CELL_CAPACITY/1000.0));
        }

        sox.soc_mean = soc.mean;
        sox.soc_min = sox.soc_mean;
        sox.soc_max = sox.soc_mean;
        if (sox.soc_mean > 100.0) { sox.soc_mean = 100.0; }
        if (sox.soc_mean < 0.0)   { sox.soc_mean = 0.0;   }
        if (sox.soc_min > 100.0)  { sox.soc_min = 100.0;  }
        if (sox.soc_min < 0.0)    { sox.soc_min = 0.0;    }
        if (sox.soc_max > 100.0)  { sox.soc_max = 100.0;  }
        if (sox.soc_max < 0.0)    { sox.soc_max = 0.0;    }
        NVM_setSOC(&soc);
        DB_WriteBlock(&sox,DATA_BLOCK_ID_SOX);
    }
}


void SOC_Set_Lookup_Table(void) {
    float soc_min = 50.0;
    float soc_max = 50.0;
    float soc_mean = 50.0;

    DB_ReadBlock(&cellminmax, DATA_BLOCK_ID_MINMAX);
    DB_ReadBlock(&sox_current_tab, DATA_BLOCK_ID_CURRENT_SENSOR);

    soc_mean = SOC_GetFromVoltage((float)(cellminmax.voltage_mean));
    soc_min = SOC_GetFromVoltage((float)(cellminmax.voltage_min));
    soc_max = SOC_GetFromVoltage((float)(cellminmax.voltage_max));

    SOC_SetValue(soc_min, soc_max, soc_mean);

}


void SOC_Calculation(void) {
    uint32_t timestamp = 0;
    uint32_t previous_timestamp = 0;

    uint32_t timestamp_cc = 0;
    uint32_t previous_timestamp_cc = 0;

    uint32_t timestep = 0;

    DATA_BLOCK_CURRENT_SENSOR_s cans_current_tab;
    SOX_SOC_s soc = {50.0, 50.0, 50.0};
    float deltaSOC = 0.0;

    if (sox_state.sensor_cc_used == FALSE) {
        DB_ReadBlock(&sox_current_tab, DATA_BLOCK_ID_CURRENT_SENSOR);

        timestamp = sox_current_tab.timestamp_cur;
        previous_timestamp = sox_current_tab.previous_timestamp_cur;

        if (soc_previous_current_timestamp != timestamp) { /* check if current measurement has been updated */
            timestep = timestamp - previous_timestamp;
            if (timestep > 0) {

                NVM_getSOC(&soc);
                /* Current in charge direction negative means SOC increasing --> BAT naming, not ROB */
                /* soc_mean = soc_mean - (sox_current_tab.current *mA* /(float)SOX_CELL_CAPACITY (*mAh*)) * (float)(timestep) * (10.0/3600.0); /*milliseconds*/

                if (POSITIVE_DISCHARGE_CURRENT == TRUE) {
                    deltaSOC = (((sox_current_tab.current)*(float)(timestep)/10))/(3600.0*SOX_CELL_CAPACITY); /* ((mA *ms *(1s/1000ms)) / (3600(s/h) *mAh)) *100% */
                } else {
                    deltaSOC = -(((sox_current_tab.current)*(float)(timestep)/10))/(3600.0*SOX_CELL_CAPACITY); /* ((mA *ms *(1s/1000ms)) / (3600(s/h) *mAh)) *100% */
                }
                soc.mean = soc.mean - deltaSOC;
                soc.min = soc.min - deltaSOC;
                soc.max = soc.max - deltaSOC;
                if (soc.mean > 100.0) { soc.mean = 100.0; }
                if (soc.mean < 0.0)   { soc.mean = 0.0;   }
                if (soc.min > 100.0)  { soc.min = 100.0;  }
                if (soc.min < 0.0)    { soc.min = 0.0;    }
                if (soc.max > 100.0)  { soc.max = 100.0;  }
                if (soc.max < 0.0)    { soc.max = 0.0;    }

                sox.soc_mean = soc.mean;
                sox.soc_min = soc.min;
                sox.soc_max = soc.max;

                NVM_setSOC(&soc);
                sox.state++;
                DB_WriteBlock(&sox, DATA_BLOCK_ID_SOX);

            }
        } /* end check if current measurement has been updated */
        /* update the variable for the next check */
        soc_previous_current_timestamp = sox_current_tab.timestamp;

    }
    else {

        DB_ReadBlock(&sox_current_tab, DATA_BLOCK_ID_CURRENT_SENSOR);

        timestamp_cc = sox_current_tab.timestamp_cc;
        previous_timestamp_cc = sox_current_tab.previous_timestamp_cc;

        if (previous_timestamp_cc != timestamp_cc) { /* check if cc measurement has been updated */
            DB_ReadBlock(&cans_current_tab,DATA_BLOCK_ID_CURRENT_SENSOR);

            if (POSITIVE_DISCHARGE_CURRENT == TRUE) {
                sox.soc_mean = sox_state.cc_scaling - 100.0*cans_current_tab.current_counter/(3600.0*(SOX_CELL_CAPACITY/1000.0));
            } else {
                sox.soc_mean = sox_state.cc_scaling + 100.0*cans_current_tab.current_counter/(3600.0*(SOX_CELL_CAPACITY/1000.0));
            }

            sox.soc_min = sox.soc_mean;
            sox.soc_max = sox.soc_mean;
            soc.mean = sox.soc_mean;
            soc.min = sox.soc_min;
            soc.max = sox.soc_max;
            if (sox.soc_mean > 100.0) { sox.soc_mean = 100.0; }
            if (sox.soc_mean < 0.0)   { sox.soc_mean = 0.0;   }
            if (sox.soc_min > 100.0)  { sox.soc_min = 100.0;  }
            if (sox.soc_min < 0.0)    { sox.soc_min = 0.0;    }
            if (sox.soc_max > 100.0)  { sox.soc_max = 100.0;  }
            if (sox.soc_max < 0.0)    { sox.soc_max = 0.0;    }
            NVM_setSOC(&soc);
            sox.state++;
            DB_WriteBlock(&sox, DATA_BLOCK_ID_SOX);
        }
        soc_previous_current_timestamp_cc = sox_current_tab.timestamp_cc;
    }


}

void SOF_Init(void) {

    /* Calculating SOF curve for the recommended operating current */
    SOF_CalculateCurves(&sox_sof_config_maxAllowedCurrent, &sofCurveRecOperatingCurrent);

#if BMS_TEST_CELL_SOF_LIMITS == TRUE
    /* Calculating SOF curve for maximum operating limit */
    SOF_CalculateCurves(&sox_sof_config_MOL, &sofCurve_MOL);

    /* Calculating SOF curve for recommended safety limit */
    SOF_CalculateCurves(&sox_sof_config_RSL, &sofCurve_RSL);

    /* Calculating SOF curve for maximum safety limit */
    SOF_CalculateCurves(&sox_sof_config_MSL, &sofCurve_MSL);
#endif
}

static void SOF_CalculateCurves(const SOX_SOF_CONFIG_s *configLimitValues, SOF_curve_s* calcCurveValues) {

    /* Calculating SOF curve for the maximum allowed current for MOL/RSL/MSL */
    calcCurveValues->Slope_TLowDischa = (configLimitValues->I_DischaMax_Cont - configLimitValues->I_Limphome) / (configLimitValues->Cutoff_TLow_Discha - configLimitValues->Limit_TLow_Discha);
    calcCurveValues->Offset_TLowDischa = configLimitValues->I_Limphome - (calcCurveValues->Slope_TLowDischa * configLimitValues->Limit_TLow_Discha);

    calcCurveValues->Slope_THighDischa = (0 - configLimitValues->I_DischaMax_Cont) / (configLimitValues->Limit_THigh_Discha - configLimitValues->Cutoff_THigh_Discha);
    calcCurveValues->Offset_THighDischa = 0 - calcCurveValues->Slope_THighDischa * configLimitValues->Limit_THigh_Discha;

    calcCurveValues->Slope_TLowCharge =  (configLimitValues->I_ChargeMax_Cont - 0) / (configLimitValues->Cutoff_TLow_Charge - configLimitValues->Limit_TLow_Charge);
    calcCurveValues->Offset_TLowCharge = 0 - (calcCurveValues->Slope_TLowCharge * configLimitValues->Limit_TLow_Charge);

    calcCurveValues->Slope_THighCharge = (0 - configLimitValues->I_ChargeMax_Cont) / (configLimitValues->Limit_THigh_Charge - configLimitValues->Cutoff_THigh_Charge);
    calcCurveValues->Offset_THighCharge = 0 - calcCurveValues->Slope_THighCharge * configLimitValues->Limit_THigh_Charge;

    calcCurveValues->Slope_SocDischa = (configLimitValues->I_DischaMax_Cont - configLimitValues->I_Limphome) / (configLimitValues->Cutoff_Soc_Discha - configLimitValues->Limit_Soc_Discha);
    calcCurveValues->Offset_SocDischa = configLimitValues->I_Limphome - (calcCurveValues->Slope_SocDischa * configLimitValues->Limit_Soc_Discha);

    calcCurveValues->Slope_SocCharge = (configLimitValues->I_ChargeMax_Cont - 0.0) / (configLimitValues->Cutoff_Soc_Charge - configLimitValues->Limit_Soc_Charge);
    calcCurveValues->Offset_SocCharge = 0 - calcCurveValues->Slope_SocCharge * configLimitValues->Limit_Soc_Charge;

    calcCurveValues->Slope_VoltageDischa = (configLimitValues->I_DischaMax_Cont - 0) / (configLimitValues->Cutoff_Voltage_Discha - configLimitValues->Limit_Voltage_Discha);
    calcCurveValues->Offset_VoltageDischa = 0 - calcCurveValues->Slope_SocDischa * configLimitValues->Limit_Soc_Discha;

    calcCurveValues->Slope_VoltageCharge = (configLimitValues->I_ChargeMax_Cont - 0) / (configLimitValues->Cutoff_Voltage_Charge - configLimitValues->Limit_Voltage_Charge);
    calcCurveValues->Offset_VoltageCharge = 0 - calcCurveValues->Slope_VoltageCharge * configLimitValues->Limit_Soc_Discha;
}


void SOF_Calculation(void) {
    DB_ReadBlock(&cellminmax, DATA_BLOCK_ID_MINMAX);
    DB_ReadBlock(&sox,DATA_BLOCK_ID_SOX);
    DB_ReadBlock(&sof,DATA_BLOCK_ID_SOF);
    DB_ReadBlock(&contfeedbacktab, DATA_BLOCK_ID_CONTFEEDBACK);

    /* Calculate SOF limits */
    SOF_Calculate(cellminmax.temperature_max, cellminmax.temperature_min, cellminmax.voltage_max, cellminmax.voltage_min, (uint16_t)(100.0*sox.soc_max), (uint16_t)(100.0*sox.soc_min));

    /* Write MOL level */
    sof.continuous_charge_MOL = sof_mol_Level.current_Charge_cont_max;
    sof.continuous_discharge_MOL = sof_mol_Level.current_Discha_cont_max;
    /* Write RSL level */
    sof.continuous_charge_RSL = sof_rsl_Level.current_Charge_cont_max;
    sof.continuous_discharge_RSL = sof_rsl_Level.current_Discha_cont_max;
    /* Write MSL level */
    sof.continuous_charge_MSL = sof_msl_Level.current_Charge_cont_max;
    sof.continuous_discharge_MSL = sof_msl_Level.current_Discha_cont_max;

    /* if Contactor MainPlus and MainMinus are not closed (when they are closed, state_feedback is 0x0C) */
#if BS_SEPARATE_POWERLINES == 1
    if ( ! ( (contfeedbacktab.contactor_feedback & 0x3F) == 0x28 || (contfeedbacktab.contactor_feedback & 0x3F) == 0x05) ){
#else
    if ( (contfeedbacktab.contactor_feedback & 0x3F) != 0x05) {
#endif /* BS_SEPARATE_POWERLINES == 1 */
        sof.recommended_continuous_charge = 0.0;
        sof.recommended_continuous_discharge = 0.0;
        sof.recommended_peak_charge = 0.0;
        sof.recommended_peak_discharge = 0.0;
    } else {
        sof.recommended_continuous_charge = sof_recOperatingCurrent.current_Charge_cont_max;
        sof.recommended_continuous_discharge = sof_recOperatingCurrent.current_Discha_cont_max;
        sof.recommended_peak_charge = sof_recOperatingCurrent.current_Charge_peak_max;
        sof.recommended_peak_discharge = sof_recOperatingCurrent.current_Discha_peak_max;
    }
    DB_WriteBlock(&sof, DATA_BLOCK_ID_SOF);
}

/**
 * @brief   look-up table for SOC initialization (mean, min and max).
 *
 * @param   voltage: voltage of battery cell
 *
 * @return  SOC value
 */

float SOC_GetFromVoltage(float voltage) {

    float SOC = 0.50;

    return SOC;
}


/**
 * @brief   calculates State of function which means how much current can be delivered by battery to stay in safe operating area.
 *
 * @param   maxtemp       maximum temperature in system in �C
 * @param   mintemp       minimum temperature in system in �C
 * @param   maxvolt       maximum voltage in system in mV
 * @param   minvolt       minimum voltage in system in mV
 * @param   maxsoc        maximum soc in system with resolution 0.01% (0..10000)
 * @param   minsoc        minimum soc in system with resolution 0.01% (0..10000)
 */
static void SOF_Calculate(int16_t maxtemp, int16_t mintemp, uint16_t maxvolt, uint16_t minvolt, uint16_t maxsoc, uint16_t minsoc) {
    SOX_SOF_s UbasedSof = {0.0, 0.0, 0.0};
    SOX_SOF_s SbasedSof = {0.0, 0.0, 0.0};
    SOX_SOF_s TbasedSof = {0.0, 0.0, 0.0};

    /* Calculate maximum allowed current depending on current values */
    SOF_CalculateVoltageBased((float)minvolt,(float)maxvolt,&UbasedSof, &sox_sof_config_maxAllowedCurrent, &sofCurveRecOperatingCurrent);
    SOF_CalculateSocBased((float)minsoc,(float)maxsoc,&SbasedSof, &sox_sof_config_maxAllowedCurrent, &sofCurveRecOperatingCurrent);
    SOF_CalculateTemperatureBased((float)mintemp,(float)maxtemp,&TbasedSof, &sox_sof_config_maxAllowedCurrent, &sofCurveRecOperatingCurrent);
    SOF_MinimumOfThreeSofValues(UbasedSof, SbasedSof, TbasedSof, &sof_recOperatingCurrent);

#if BMS_TEST_CELL_SOF_LIMITS == TRUE
    /* Calculate maximum allowed current MOL level */
    SOF_CalculateVoltageBased((float)minvolt,(float)maxvolt,&UbasedSof, &sox_sof_config_MOL, &sofCurve_MOL);
    SOF_CalculateSocBased((float)minsoc,(float)maxsoc,&SbasedSof, &sox_sof_config_MOL, &sofCurve_MOL);
    SOF_CalculateTemperatureBased((float)mintemp,(float)maxtemp,&TbasedSof, &sox_sof_config_MOL, &sofCurve_MOL);
    SOF_MinimumOfThreeSofValues(UbasedSof, SbasedSof, TbasedSof, &sof_mol_Level);

    /* Calculate maximum allowed current RSL level */
    SOF_CalculateVoltageBased((float)minvolt,(float)maxvolt,&UbasedSof, &sox_sof_config_RSL, &sofCurve_RSL);
    SOF_CalculateSocBased((float)minsoc,(float)maxsoc,&SbasedSof, &sox_sof_config_RSL, &sofCurve_RSL);
    SOF_CalculateTemperatureBased((float)mintemp,(float)maxtemp,&TbasedSof, &sox_sof_config_RSL, &sofCurve_RSL);
    SOF_MinimumOfThreeSofValues(UbasedSof, SbasedSof, TbasedSof, &sof_rsl_Level);

    /* Calculate maximum allowed current MSL level */
    SOF_CalculateVoltageBased((float)minvolt,(float)maxvolt,&UbasedSof, &sox_sof_config_MSL, &sofCurve_MSL);
    SOF_CalculateSocBased((float)minsoc,(float)maxsoc,&SbasedSof, &sox_sof_config_MSL, &sofCurve_MSL);
    SOF_CalculateTemperatureBased((float)mintemp,(float)maxtemp,&TbasedSof, &sox_sof_config_MSL, &sofCurve_MSL);
    SOF_MinimumOfThreeSofValues(UbasedSof, SbasedSof, TbasedSof, &sof_msl_Level);
#else
    sof_mol_Level.current_Charge_cont_max = BC_CURRENTMAX_CHARGE_MOL;
    sof_mol_Level.current_Discha_cont_max = BC_CURRENTMAX_DISCHARGE_MOL;
    sof_rsl_Level.current_Charge_cont_max = BC_CURRENTMAX_CHARGE_MOL;
    sof_rsl_Level.current_Discha_cont_max = BC_CURRENTMAX_DISCHARGE_MOL;
    sof_msl_Level.current_Charge_cont_max = BC_CURRENTMAX_CHARGE_MOL;
    sof_msl_Level.current_Discha_cont_max = BC_CURRENTMAX_DISCHARGE_MOL;
#endif

}

/**
 *  @brief  calculates the SoF from voltage data (i.e., minimum and maximum voltage)
 *
 *  @param  MinVoltage minimum sell voltage
 *  @param  MaxVoltage maximum cell voltage
 *  @param  ResultValues Voltage-based SOF
 */
static void SOF_CalculateVoltageBased(float MinVoltage,float MaxVoltage, SOX_SOF_s *ResultValues, const SOX_SOF_CONFIG_s *configLimitValues, SOF_curve_s* calcCurveValues) {
    /* min voltage issues */
    if (MinVoltage <= configLimitValues->Limit_Voltage_Discha) {
        ResultValues->current_Discha_cont_max = 0;
        ResultValues->current_Discha_peak_max = 0;
    }
    else {
        if (MinVoltage <= configLimitValues->Cutoff_Voltage_Discha) {
            ResultValues->current_Discha_cont_max = (calcCurveValues->Slope_VoltageDischa * (MinVoltage-configLimitValues->Limit_Voltage_Discha));
            ResultValues->current_Discha_peak_max = ResultValues->current_Discha_cont_max;
        }
        else {
            ResultValues->current_Discha_cont_max = configLimitValues->I_DischaMax_Cont;
            ResultValues->current_Discha_peak_max = configLimitValues->I_DischaMax_Cont;
        }
    }
    /* max voltage issues */
    if (MaxVoltage >= configLimitValues->Limit_Voltage_Charge) {
        ResultValues->current_Charge_cont_max = 0;
        ResultValues->current_Charge_peak_max = 0;
    }
    else {
        if (MaxVoltage >= configLimitValues->Cutoff_Voltage_Charge) {
            ResultValues->current_Charge_cont_max =  (calcCurveValues->Slope_VoltageCharge * (MaxVoltage- configLimitValues->Limit_Voltage_Charge));
            ResultValues->current_Charge_peak_max = ResultValues->current_Charge_cont_max;
        }
        else {
            ResultValues->current_Charge_cont_max = configLimitValues->I_ChargeMax_Cont;
            ResultValues->current_Charge_peak_max = configLimitValues->I_ChargeMax_Cont;
        }
    }
}

/**
 * @brief   calculates the SoF from SoC data (i.e., minimum and maximum SoC of cells, both values are equal in case of Ah-counter)
 *
 * @param   MinSoc minimum State of Charge
 * @param   MaxSoc maximum State of Charge
 * @param   ResultValues pointer where to store the results
 */
static void SOF_CalculateSocBased(float MinSoc,float MaxSoc, SOX_SOF_s *ResultValues, const SOX_SOF_CONFIG_s *configLimitValues, SOF_curve_s* calcCurveValues) {
    /* min voltage issues */
    if (MinSoc <= configLimitValues->Limit_Soc_Discha){
        ResultValues->current_Discha_cont_max = configLimitValues->I_Limphome;
        ResultValues->current_Discha_peak_max = configLimitValues->I_Limphome;
    }
    else {
        if (MinSoc <= configLimitValues->Cutoff_Soc_Discha){
            ResultValues->current_Discha_cont_max =  calcCurveValues->Slope_SocDischa * MinSoc + calcCurveValues->Offset_SocDischa;
            ResultValues->current_Discha_peak_max =  ResultValues->current_Discha_cont_max;
        }
        else{
            ResultValues->current_Discha_cont_max =  configLimitValues->I_DischaMax_Cont;
            ResultValues->current_Discha_peak_max =  configLimitValues->I_DischaMax_Cont;
        }
    }

    /* max voltage issues */
    if (MaxSoc >= configLimitValues->Limit_Soc_Charge){
        ResultValues->current_Charge_cont_max = 0;
        ResultValues->current_Charge_peak_max = 0;
    }
    else {
        if (MaxSoc >= configLimitValues->Cutoff_Soc_Charge){
            ResultValues->current_Charge_cont_max =  calcCurveValues->Slope_SocCharge * MaxSoc + calcCurveValues->Offset_SocCharge;
            ResultValues->current_Charge_peak_max = ResultValues->current_Charge_cont_max;
        }
        else{
            ResultValues->current_Charge_cont_max = configLimitValues->I_ChargeMax_Cont;
            ResultValues->current_Charge_peak_max = configLimitValues->I_ChargeMax_Cont;
        }
    }
}

/**
 * @brief   calculates the SoF from temperature data (i.e., minimum and maximum temperature of cells)
 *
 * @param   MinTemp minimum temperature of cells
 * @param   MaxTemp maximum temperature of cells
 * @param   ResultValues pointer where to store the results
 */
static void  SOF_CalculateTemperatureBased(float MinTemp,float MaxTemp, SOX_SOF_s *ResultValues, const SOX_SOF_CONFIG_s *configLimitValues, SOF_curve_s* calcCurveValues) {

    SOX_SOF_s dummyResultValues = {0.0, 0.0, 0.0};
    /* Temperature low Discharge */
    if (MinTemp <= configLimitValues->Limit_TLow_Discha) {
        ResultValues->current_Discha_cont_max = configLimitValues->I_Limphome;
        ResultValues->current_Discha_peak_max = configLimitValues->I_Limphome;
    }
    else {
        if (MinTemp <= configLimitValues->Cutoff_TLow_Discha) {
            ResultValues->current_Discha_cont_max = calcCurveValues->Slope_TLowDischa * MinTemp + calcCurveValues->Offset_TLowDischa;
            ResultValues->current_Discha_peak_max = ResultValues->current_Discha_cont_max;
        }
        else {
            ResultValues->current_Discha_cont_max = configLimitValues->I_DischaMax_Cont;
            ResultValues->current_Discha_peak_max = configLimitValues->I_DischaMax_Cont;
        }
    }
    /* Temperature low charge */
    if (MinTemp <= configLimitValues->Limit_TLow_Charge) {
        ResultValues->current_Charge_cont_max = 0;
        ResultValues->current_Charge_peak_max = 0;
    }
    else {
        if (MinTemp <= configLimitValues->Cutoff_TLow_Charge) {
            ResultValues->current_Charge_cont_max = calcCurveValues->Slope_TLowCharge * MinTemp + calcCurveValues->Offset_TLowCharge;
            ResultValues->current_Charge_peak_max = ResultValues->current_Charge_cont_max;
        }
        else{
            ResultValues->current_Charge_cont_max =  configLimitValues->I_ChargeMax_Cont;
            ResultValues->current_Charge_peak_max =  configLimitValues->I_ChargeMax_Cont;
        }
    }
    /* Temperature high discharge */
    if (MaxTemp >= configLimitValues->Limit_THigh_Discha) {
        ResultValues->current_Discha_cont_max = 0;
        ResultValues->current_Discha_peak_max = 0;
        dummyResultValues.current_Discha_cont_max = 0;
        dummyResultValues.current_Discha_peak_max = 0;
    }
    else {
        if (MaxTemp >= configLimitValues->Cutoff_THigh_Discha) {
            dummyResultValues.current_Discha_cont_max = (calcCurveValues->Slope_THighDischa * MaxTemp + calcCurveValues->Offset_THighDischa);
            dummyResultValues.current_Discha_peak_max = dummyResultValues.current_Discha_cont_max;
        }
        else {
            /* do nothing because this situation is handled with MinTemp */
            dummyResultValues.current_Discha_cont_max = configLimitValues->I_DischaMax_Cont;
            dummyResultValues.current_Discha_peak_max = configLimitValues->I_DischaMax_Cont;

        }
        /* take the smaller current as limit */
        if (dummyResultValues.current_Discha_cont_max < ResultValues->current_Discha_cont_max)
            ResultValues->current_Discha_cont_max = dummyResultValues.current_Discha_cont_max;
        if (dummyResultValues.current_Discha_peak_max < ResultValues->current_Discha_peak_max)
            ResultValues->current_Discha_peak_max = dummyResultValues.current_Discha_peak_max;
    }
    /* Temperature high Charge */
    if (MaxTemp >= configLimitValues->Limit_THigh_Charge) {
        ResultValues->current_Charge_cont_max = 0;
        ResultValues->current_Charge_peak_max = 0;
        dummyResultValues.current_Charge_cont_max = 0;
        dummyResultValues.current_Charge_peak_max = 0;
    }
    else {
        if (MaxTemp >= configLimitValues->Cutoff_THigh_Charge){
            dummyResultValues.current_Charge_cont_max = (calcCurveValues->Slope_THighCharge * MaxTemp + calcCurveValues->Offset_THighCharge);
            dummyResultValues.current_Charge_peak_max = dummyResultValues.current_Charge_cont_max;
        }
        else {
            /* do nothing because this situation is handled with MinTemp */
            dummyResultValues.current_Charge_cont_max = configLimitValues->I_ChargeMax_Cont;
            dummyResultValues.current_Charge_peak_max = configLimitValues->I_ChargeMax_Cont;
        }
        /* take the smaller current as limit */
        if (dummyResultValues.current_Charge_cont_max < ResultValues->current_Charge_cont_max)
            ResultValues->current_Charge_cont_max = dummyResultValues.current_Charge_cont_max;
        if (dummyResultValues.current_Charge_peak_max < ResultValues->current_Charge_peak_max)
            ResultValues->current_Charge_peak_max = dummyResultValues.current_Charge_peak_max;
    }
}

/**
 * @brief   get the minimum current values of all variants of SoF calculation
 *
 * @param   Ubased voltage constrained current derating values
 * @param   Sbased SOC constrained current derating values
 * @param   Tbased temperature constrained current derating values
 * @param   resultValues minimum SoF current values
 */
static void SOF_MinimumOfThreeSofValues(SOX_SOF_s Ubased, SOX_SOF_s Sbased, SOX_SOF_s Tbased, SOX_SOF_s *resultValues) {
    resultValues->current_Charge_cont_max = SOF_MinimumOfThreeValues(Ubased.current_Charge_cont_max, Tbased.current_Charge_cont_max, Sbased.current_Charge_cont_max);
    resultValues->current_Charge_peak_max = SOF_MinimumOfThreeValues(Ubased.current_Charge_peak_max, Tbased.current_Charge_peak_max, Sbased.current_Charge_peak_max);
    resultValues->current_Discha_cont_max = SOF_MinimumOfThreeValues(Ubased.current_Discha_cont_max, Tbased.current_Discha_cont_max, Sbased.current_Discha_cont_max);
    resultValues->current_Discha_peak_max = SOF_MinimumOfThreeValues(Ubased.current_Discha_peak_max, Tbased.current_Discha_peak_max, Sbased.current_Discha_peak_max);
}

/**
 * @brief   calculates minimum of three values
 *
 * @param   value1
 * @param   value2
 * @param   value3
 *
 * @return minimum of the 3 parameters
 */
static float SOF_MinimumOfThreeValues (float value1,float value2, float value3) {
    float result = 0.0;
    if (value1 <= value2) {
        if (value3 <= value1){
            result = value3;
        }
        else {
            result = value1;
        }
    }
    else {
        if (value3 <= value2){
            result = value3;
        }
        else{
            result = value2;
        }
    }
    return result;
}
