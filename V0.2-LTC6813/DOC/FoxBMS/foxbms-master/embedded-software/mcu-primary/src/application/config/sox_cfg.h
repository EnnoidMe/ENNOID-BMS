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
 * @file    sox_cfg.h
 * @author  foxBMS Team
 * @date    18.08.2015 (date of creation)
 * @ingroup APPLICATION_CONF
 * @prefix  SOX
 *
 * @brief   Configuration header for SOX
 *
 */

#ifndef SOX_CFG_H_
#define SOX_CFG_H_

/*================== Includes =============================================*/
#include "general.h"

/*================== Macros and Definitions ===============================*/
/**
 * @ingroup CONFIG_SOX
 * when initializing SOC from SOC-Voltage lookup table, current must be below
 * this value in order to ensure low disturbance and equilibrium in chemical
 * reactions (relaxation effects)
 * \par Type:
 * int
 * \par Unit:
 * mA
 * \par Default:
 * 100
*/
#define SOX_SOC_INIT_CURRENT_LIMIT      100

/**
 * @ingroup CONFIG_SOX
 * when initializing SOC from SOC-Voltage lookup table, the difference between
 * two consecutive voltage measurements of the cell with minimum voltage must be below
 * this value in order to ensure low disturbance and equilibrium in chemical
 * reactions (relaxation effects)
 * \par Type:
 * int
 * \par Unit:
 * mV
 * \par Default:
 * 10
*/
#define SOX_DELTA_MIN_LIMIT             10

/**
 * @ingroup CONFIG_SOX
 * when initializing SOC from SOC-Voltage lookup table, the difference between
 * two consecutive voltage measurements of the cell with maximum voltage must be below
 * this value in order to ensure low disturbance and equilibrium in chemical
 * reactions (relaxation effects)
 * \par Type:
 * int
 * \par Unit:
 * mV
 * \par Default:
 * 10
*/
#define SOX_DELTA_MAX_LIMIT             10

/**
 * @ingroup CONFIG_SOX
 * the cell capacity used for SOC calculation, in this case Ah counting
 * Specified once according to data sheet of cell usually.
 * \par Type:
 * float
 * \par Unit:
 * mAh
 * \par Default:
 * 20000.0
*/
#define SOX_CELL_CAPACITY               20000.0

/**
 * @ingroup CONFIG_SOX
 * the maximum current in charge direction that the battery pack can sustain.
 * Normally set once for the specific battery cell from datasheet
 * \par Type:
 * float
 * \par Unit:
 * A
 * \par Default:
 * 120.0
*/
#define SOX_CURRENT_MAX_CONTINUOUS_CHARGE           120.00

/**
 * Different alarm levels for the maximum continuous charge current.
 */
#define SOX_MOL_CURRENT_MAX_CONTINUOUS_CHARGE       100.00
#define SOX_RSL_CURRENT_MAX_CONTINUOUS_CHARGE       115.00
#define SOX_MSL_CURRENT_MAX_CONTINUOUS_CHARGE       122.00

/**
 * @ingroup CONFIG_SOX
 * the maximum current in discharge direction that the battery pack can deliver.
 * Normally set once for the specific battery cell from datasheet
 *
 * \par Type:
 * float
 * \par Unit:
 * A
 * \par Default:
 * 120.0
*/
#define SOX_CURRENT_MAX_CONTINUOUS_DISCHARGE 120.00

/**
 * Different alarm levels for the maximum continuous discharge current.
 */
#define SOX_MOL_CURRENT_MAX_CONTINUOUS_DISCHARGE       100.00
#define SOX_RSL_CURRENT_MAX_CONTINUOUS_DISCHARGE       115.00
#define SOX_MSL_CURRENT_MAX_CONTINUOUS_DISCHARGE       122.00

/**
 * @ingroup CONFIG_SOX
 * the current that the battery pack should be able to discharge when in LIMPHOME mode,
 * i.e., something noncritical went wrong but it should be able to drive home.
 * The value is system engineer's choice.
 *
 * \par Type:
 * float
 * \par Unit:
 * A
 * \par Range:
 * [1,40]
 * \par Default:
 * 20.0
*/
#define SOX_CURRENT_LIMP_HOME 20.00

/**
 * Different alarm levels for the limp home current
 */
#define SOX_MOL_CURRENT_LIMP_HOME           20.00
#define SOX_RSL_CURRENT_LIMP_HOME           20.00
#define SOX_MSL_CURRENT_LIMP_HOME           20.00

/**
 * @ingroup CONFIG_SOX
 * the cold temperature where the derating of maximum discharge current starts,
 * i.e., below this temperature battery pack should not deliver full discharge current
 * \par Type:
 * float
 * \par Range:
 * -40.0<x<80.0
 * \par Unit:
 * &deg;C
 * \par Default:
 * 0.0
*/
#define SOX_TEMP_LOW_CUTOFF_DISCHARGE 0.0

/**
 * Different alarm levels for the cold temperature where the derating of
 * maximum discharge current starts, i.e., below this temperature battery pack
 * should not deliver full discharge current
 */
#define SOX_MOL_TEMP_LOW_CUTOFF_DISCHARGE       0.0
#define SOX_RSL_TEMP_LOW_CUTOFF_DISCHARGE       0.0
#define SOX_MSL_TEMP_LOW_CUTOFF_DISCHARGE       0.0

/**
 * @ingroup CONFIG_SOX
 * the cold temperature where the derating of maximum discharge current is fully applied,
 * i.e., below this temperature battery pack should not deliver any current in discharge direction
 * \par Type:
 * float
 * \par Range:
 * -40.0<x<80.0
 * \par Unit:
 * &deg;C
 * \par Default:
 * -10.0
*/
#define SOX_TEMP_LOW_LIMIT_DISCHARGE -10.0

/**
 * Different alarm levels for the cold temperature where the derating of
 * maximum discharge current is fully applied. Below this temperature the
 * battery pack should not deliver any current in discharge direction.
 */
#define SOX_MOL_TEMP_LOW_LIMIT_DISCHARGE       -10.0
#define SOX_RSL_TEMP_LOW_LIMIT_DISCHARGE       -10.0
#define SOX_MSL_TEMP_LOW_LIMIT_DISCHARGE       -10.0

/**
 * @ingroup CONFIG_SOX
 * the cold temperature where the derating of maximum charge current starts,
 * i.e., below this temperature battery pack should not deliver full charge current
 * \par Type:
 * float
 * \par Range:
 * -40.0<x<80.0
 * \par Unit:
 * &deg;C
 * \par Default:
 * 0.0
*/
#define SOX_TEMP_LOW_CUTOFF_CHARGE 0.0

/**
 * Different alarm levels for the cold temperature where the derating of
 * maximum charge current starts, i.e., below this temperature battery pack
 * should not deliver full charge current
 */
#define SOX_MOL_TEMP_LOW_CUTOFF_CHARGE       0.0
#define SOX_RSL_TEMP_LOW_CUTOFF_CHARGE       0.0
#define SOX_MSL_TEMP_LOW_CUTOFF_CHARGE       0.0

/**
 * @ingroup CONFIG_SOX
 * the cold temperature where the derating of maximum charge current is fully applied,
 * i.e., below this temperature battery pack should not deliver any current in charge direction
 * \par Type:
 * float
 * \par Range:
 * -40.0<x<80.0
 * \par Unit:
 * &deg;C
 * \par Default:
 * -10.0
*/
#define SOX_TEMP_LOW_LIMIT_CHARGE -10.0

/**
 * Different alarm levels for the cold temperature where the derating of
 * maximum charge current is fully applied. Below this temperature the
 * battery pack should not deliver any current in charge direction.
 */
#define SOX_MOL_TEMP_LOW_LIMIT_CHARGE          -10.0
#define SOX_RSL_TEMP_LOW_LIMIT_CHARGE          -10.0
#define SOX_MSL_TEMP_LOW_LIMIT_CHARGE          -10.0



/**
 * @ingroup CONFIG_SOX
 * the hot temperature where the derating of maximum discharge current starts,
 * i.e., above this temperature battery pack should not deliver full discharge current
 * \par Type:
 * float
 * \par Range:
 * -40.0<x<80.0
 * \par Unit:
 * &deg;C
 * \par Default:
 * 45.0
*/
#define SOX_TEMP_HIGH_CUTOFF_DISCHARGE 45.0

/**
 * Different alarm levels for the high temperature where the derating of
 * maximum discharge current starts, i.e., above this temperature battery pack
 * should not deliver full discharge current
 */
#define SOX_MOL_TEMP_HIGH_CUTOFF_DISCHARGE       45.0
#define SOX_RSL_TEMP_HIGH_CUTOFF_DISCHARGE       45.0
#define SOX_MSL_TEMP_HIGH_CUTOFF_DISCHARGE       45.0

/**
 * @ingroup CONFIG_SOX
 * the hot temperature where the derating of maximum discharge current is fully applied,
 * i.e., above this temperature battery pack should not deliver any current in discharge direction
 * \par Type:
 * float
 * \par Range:
 * -40.0<x<80.0
 * \par Unit:
 * &deg;C
 * \par Default:
 * 55.0
*/
#define SOX_TEMP_HIGH_LIMIT_DISCHARGE 55.0

/**
 * Different alarm levels for the high temperature where the derating of
 * maximum discharge current is fully applied. Above this temperature the
 * battery pack should not deliver any current in discharge direction.
 */
#define SOX_MOL_TEMP_HIGH_LIMIT_DISCHARGE          55.0
#define SOX_RSL_TEMP_HIGH_LIMIT_DISCHARGE          55.0
#define SOX_MSL_TEMP_HIGH_LIMIT_DISCHARGE          55.0


/**
 * @ingroup CONFIG_SOX
 * the hot temperature where the derating of maximum charge current starts,
 * i.e., above this temperature battery pack should not deliver full charge current
 * \par Type:
 * float
 * \par Range:
 * -40.0<x<80.0
 * \par Unit:
 * &deg;C
 * \par Default:
 * 45.0
*/
#define SOX_TEMP_HIGH_CUTOFF_CHARGE 45.0

/**
 * Different alarm levels for the high temperature where the derating of
 * maximum charge current starts, i.e., above this temperature battery pack
 * should not deliver full charge current
 */
#define SOX_MOL_TEMP_HIGH_CUTOFF_CHARGE          45.0
#define SOX_RSL_TEMP_HIGH_CUTOFF_CHARGE          45.0
#define SOX_MSL_TEMP_HIGH_CUTOFF_CHARGE          45.0

/**
 * @ingroup CONFIG_SOX
 * the hot temperature where the derating of maximum charge current is fully applied,
 * i.e., above this temperature battery pack should not deliver any current in charge direction
 * \par Type:
 * float
 * \par Range:
 * -40.0<x<80.0
 * \par Unit:
 * &deg;C
 * \par Default:
 * 55.0
*/
#define SOX_TEMP_HIGH_LIMIT_CHARGE 55.0

/**
 * Different alarm levels for the high temperature where the derating of
 * maximum charge current is fully applied. Above this temperature the
 * battery pack should not deliver any current in charge direction.
 */
#define SOX_MOL_TEMP_HIGH_LIMIT_CHARGE             55.0
#define SOX_RSL_TEMP_HIGH_LIMIT_CHARGE             55.0
#define SOX_MSL_TEMP_HIGH_LIMIT_CHARGE             55.0

/**
 * @ingroup CONFIG_SOX
 * above this SOC value battery pack should not be exposed to full current in charge direction
 * \par Type:
 * int
 * \par Range:
 * 0<=x<=10000
 * \par Unit:
 * 0.01%
 * \par Default:
 * 8500
*/
#define SOX_SOC_CUTOFF_CHARGE 8500

/**
 * Different alarm levels for the upper SOC limit where the derating of
 * maximum charge current starts. Above this value the battery pack should
 * not deliver full current in charge direction.
 */
#define SOX_MOL_SOC_CUTOFF_CHARGE               8500
#define SOX_RSL_SOC_CUTOFF_CHARGE               8500
#define SOX_MSL_SOC_CUTOFF_CHARGE               8500

/**
 * @ingroup CONFIG_SOX
 * above this SOC value battery pack should not be exposed to any current in charge direction
 * \par Type:
 * int
 * \par Range:
 * 0<=x<=10000
 * \par Unit:
 * 0.01%
 * \par Default:
 * 9500
*/
#define SOX_SOC_LIMIT_CHARGE 9500

/**
 * Different alarm levels for the upper SOC limit where the derating of
 * maximum charge current is fully applied. Above this value the battery pack
 * should not deliver full current in charge direction.
 */
#define SOX_MOL_SOC_LIMIT_CHARGE               9500
#define SOX_RSL_SOC_LIMIT_CHARGE               9500
#define SOX_MSL_SOC_LIMIT_CHARGE               9500
/**
 * @ingroup CONFIG_SOX
 * below this SOC value battery pack should not deliver full current in discharge direction
 * \par Type:
 * int
 * \par Range:
 * 0<=x<=10000
 * \par Unit:
 * 0.01%
 * \par Default:
 * 1500
*/
#define SOX_SOC_CUTOFF_DISCHARGE 1500

/**
 * Different alarm levels for the lower SOC limit where the derating of
 * maximum discharge current starts. Below this value the battery pack should
 * not deliver full current in discharge direction.
 */
#define SOX_MOL_SOC_CUTOFF_DISCHARGE               1500
#define SOX_RSL_SOC_CUTOFF_DISCHARGE               1500
#define SOX_MSL_SOC_CUTOFF_DISCHARGE               1500

/**
 * @ingroup CONFIG_SOX
 * below this SOC value battery pack should not deliver any current in discharge direction
 * \par Type:
 * int
 * \par Range:
 * 0<=x<=10000
 * \par Unit:
 * 0.01%
 * \par Default:
 * 500
*/
#define SOX_SOC_LIMIT_DISCHARGE 500

/**
 * Different alarm levels for the lower SOC limit where the derating of
 * maximum discharge is fully applied. Below this value the battery pack
 * should not deliver any current in discharge direction.
 */
#define SOX_MOL_SOC_LIMIT_DISCHARGE                500
#define SOX_RSL_SOC_LIMIT_DISCHARGE                500
#define SOX_MSL_SOC_LIMIT_DISCHARGE                500

/**
 * @ingroup CONFIG_SOX
 * above this voltage value battery pack should not be exposed to full current in charge direction
 * \par Type:
 * int
 * \par Unit:
 * mV
 * \par Range:
 * [2000,2500]
 * \par Default:
 * 2400
*/
#define SOX_VOLT_CUTOFF_CHARGE 2400

/**
 * Different alarm levels for the upper voltage limit where the derating of
 * maximum charge current starts. Above this value the battery pack
 * should not deliver full current in charge direction.
 */
#define SOX_MOL_VOLT_CUTOFF_CHARGE                2400
#define SOX_RSL_VOLT_CUTOFF_CHARGE                2400
#define SOX_MSL_VOLT_CUTOFF_CHARGE                2400

/**
 * @ingroup CONFIG_SOX
 * above this voltage value battery pack should not be exposed to any current in charge direction
 * \par Type:
 * int
 * \par Unit:
 * mV
 * \par Range:
 * [2500,3000]
 * \par Default:
 * 2550
*/
#define SOX_VOLT_LIMIT_CHARGE 2550

/**
 * Different alarm levels for the upper voltage limit where the derating of
 * maximum charge current is fully applied. Above this value the battery pack
 * should not deliver any current in charge direction.
 */
#define SOX_MOL_VOLT_LIMIT_CHARGE                 2550
#define SOX_RSL_VOLT_LIMIT_CHARGE                 2550
#define SOX_MSL_VOLT_LIMIT_CHARGE                 2550

/**
 * @ingroup CONFIG_SOX
 * below this voltage value battery pack should not deliver full current in discharge direction
 * \par Type:
 * int
 * \par Unit:
 * mV
 * \par Range:
 * [1900,2200]
 * \par Default:
 * 2000
*/
#define SOX_VOLT_CUTOFF_DISCHARGE 2000

/**
 * Different alarm levels for the lower voltage limit where the derating of
 * maximum discharge current starts. Below this value the battery pack
 * should not deliver full current in discharge direction.
 */
#define SOX_MOL_VOLT_CUTOFF_DISCHARGE                 2000
#define SOX_RSL_VOLT_CUTOFF_DISCHARGE                 2000
#define SOX_MSL_VOLT_CUTOFF_DISCHARGE                 2000

/**
 * @ingroup CONFIG_SOX
 * below this voltage value battery pack should not deliver any current in discharge direction
 * \par Type:
 * int
 * \par Unit:
 * mV
 * \par Range:
 * [1700,1900]
 * \par Default:
 * 1750
*/
#define SOX_VOLT_LIMIT_DISCHARGE 1750

/**
 * Different alarm levels for the lower voltage limit where the derating of
 * maximum discharge is fully applied. Below this value the battery pack
 * should not deliver any current in discharge direction.
 */
#define SOX_MOL_VOLT_LIMIT_DISCHARGE                 1750
#define SOX_RSL_VOLT_LIMIT_DISCHARGE                 1750
#define SOX_MSL_VOLT_LIMIT_DISCHARGE                 1750

/*================== Constant and Variable Definitions ====================*/

/**
 * structure for configuration of SoF Calculation
 */
typedef struct {
    float I_DischaMax_Cont;
    float I_ChargeMax_Cont;
    float I_Limphome;

    float Cutoff_TLow_Discha;
    float Limit_TLow_Discha;

    float Cutoff_TLow_Charge;
    float Limit_TLow_Charge;

    float Cutoff_THigh_Discha;
    float Limit_THigh_Discha;

    float Cutoff_THigh_Charge;
    float Limit_THigh_Charge;

    float Cutoff_Soc_Charge;
    float Limit_Soc_Charge;

    float Cutoff_Soc_Discha;
    float Limit_Soc_Discha;

    float Cutoff_Voltage_Charge;
    float Limit_Voltage_Charge;

    float Cutoff_Voltage_Discha;
    float Limit_Voltage_Discha;
} SOX_SOF_CONFIG_s;

extern const SOX_SOF_CONFIG_s sox_sof_config_maxAllowedCurrent;
extern const SOX_SOF_CONFIG_s sox_sof_config_MOL;
extern const SOX_SOF_CONFIG_s sox_sof_config_RSL;
extern const SOX_SOF_CONFIG_s sox_sof_config_MSL;

/*================== Function Prototypes ==================================*/


/*================== Function Implementations =============================*/

#endif /* SOX_CFG_H_ */
