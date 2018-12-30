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
 * @file    sox.h
 * @author  foxBMS Team
 * @date    13.11.2015 (date of creation)
 * @ingroup APPLICATION
 * @prefix  SOX
 *
 * @brief   Header for SOX module, responsible for calculation of current derating and of SOC
 *
 */

#ifndef SOX_H_
#define SOX_H_

/*================== Includes =============================================*/
#include "sox_cfg.h"

/*================== Macros and Definitions ===============================*/
/**
 * This structure contains all the variables relevant for the SOX.
 *
 */
typedef struct {
    uint8_t sensor_cc_used;                 /*!< time in ms before the state machine processes the next state, e.g. in counts of 1ms    */
    float cc_scaling;                       /*!< scaling for the C-C value from sensor */
    uint8_t counter;                        /*!< general purpose counter */
} SOX_STATE_s;


/**
 * struct definition for 4 different values: in two current directions (charge, discharge) for two use cases (peak and continuous)
 */
typedef struct {
    float current_Charge_cont_max;  /*!< maximum current for continues charging     */
    float current_Charge_peak_max;  /*!< maximum current for peak charging          */
    float current_Discha_cont_max;  /*!< maximum current for continues discharging  */
    float current_Discha_peak_max;  /*!< maximum current for peak discharging       */
} SOX_SOF_s;

/**
 * state of charge (SOC). Since SOC is voltage dependent, three different values are used, min, max and mean
 * SOC defined as a float number between 0.0 and 100.0 (0% and 100%)
 */
typedef struct {
    float min;  /*!< minimum SOC    */
    float max;  /*!< maximum SOC    */
    float mean; /*!< mean SOC       */
    float reserved1;/*!< reserved for future use */
    float reserved2;/*!< reserved for future use */
    float reserved3;/*!< reserved for future use */
    float reserved4;/*!< reserved for future use */
} SOX_SOC_s;

/**
 * struct definition for calculating the linear SOF curve. The SOF curve is SOC,
 * voltage, temperature and charge/discharge dependent.
 */
typedef struct {
    float Slope_TLowDischa;
    float Offset_TLowDischa;
    float Slope_THighDischa;
    float Offset_THighDischa;

    float Slope_TLowCharge;
    float Offset_TLowCharge;
    float Slope_THighCharge;
    float Offset_THighCharge;

    float Slope_SocDischa;
    float Offset_SocDischa;
    float Slope_SocCharge;
    float Offset_SocCharge;

    float Slope_VoltageDischa;
    float Offset_VoltageDischa;
    float Slope_VoltageCharge;
    float Offset_VoltageCharge;
}SOF_curve_s;

/*================== Constant and Variable Definitions ====================*/


/*================== Function Prototypes ==================================*/

/**
 * @brief   initializes startup SOC-related values like lookup from nonvolatile ram at startup
 */
extern void SOC_Init(uint8_t cc_present);

/**
 * @brief   initializes the area for SOF (where derating starts and is fully active).
 *
 * Pseudocode for linear function parameter extraction with 2 points
 *  slope = (y2 - y1) / (x2-x1)
 *  offset = y1 - (slope) * x1
 *  function y= slope * x + offset
 */
extern void SOF_Init(void);

/**
 * @brief   sets SOC value with a parameter between 0.0 and 100.0.
 *
 * @param   soc_value_min   SOC min value to set
 * @param   soc_value_max   SOC maxn value to set
 * @param   soc_value_mean  SOC mean value to set
 */
extern void SOC_SetValue(float soc_value_min, float soc_value_max, float soc_value_mean);

/**
 * @brief   initializes the SOC values with lookup table (mean, min and max).
 */
extern void SOC_Set_Lookup_Table(void);

/**
 * @brief   integrates current over time to calculate SOC.
 */
extern void SOC_Calculation(void);

/**
 * @brief   triggers SOF calculation
 *
 * Calculation made with the function SOF_Calculate().
 */
extern void SOF_Calculation(void);


/*================== Function Implementations =============================*/

#endif /* SOX_H_ */
