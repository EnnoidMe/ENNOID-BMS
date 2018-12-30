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
 * @file    rcc_cfg.h
 * @author  foxBMS Team
 * @date    02.10.2015 (date of creation)
 * @ingroup DRIVERS_CONF
 * @prefix  RCC
 *
 * @brief   Headers for the configuration for the clock system.
 *
 * The system Clock is configured as follows:
 *
 * Description                      | Value
 * ---------------------------------|------------------------------------------------------------
 * System Clock source              | PLL (HSE)
 * SYSCLK(Hz)                       | 168000000
 * HCLK(Hz)                         | 168000000
 * AHB Prescaler                    | 1
 * APB1 Prescaler                   | 4
 * APB2 Prescaler                   | 2
 * HSE Frequency(Hz)                | 8000000 define given by compiler flag -DHSEVALUE=
 * PLL_M                            | 8
 * PLL_N                            | 336
 * PLL_P                            | 2
 * PLL_Q                            | 7
 * VDD(V)                           | 3.3
 * Main regulator output voltage    | Scale1 mode
 * Flash Latency(WS)                | 5
 */

#ifndef RCC_CFG_H_
#define RCC_CFG_H_

/*================== Includes =============================================*/
#include "general.h"

#include "cpu_cfg.h"

/*================== Macros and Definitions ===============================*/

/**
 * SYSCLOCK = ((HSE/RCC_PLL_M) * RCC_PLL_N) / RCC_PLL_P_NUM
 *
 * HCLK     = SYSCLK / AHB Prescaler
 *
 * APB1 peripheral clock = HCLK / APB1 Prescaler (45MHz max.)
 *
 * APB1 timer clock = 2 * APB1 peripheral clock
 *
 * APB2 peripheral clock = HCLK / APB2 Prescaler (90MHz max.)
 *
 * APB2 timer clock = 2 * APB2 peripheral clock
 */

/**
 * Divides output of the PLL SOURCE MUX(HSI or HSE clock) by the factor RCC_PLL_M. The dived
 * clock signal is then used for Main PLL, PLLI2S (currently not used) and PLLSAI (currently
 * not used).
 *
 * Possible values: 2-63
 */
#define RCC_PLL_M    4

/**
 * Multiplies the input of Main PLL with the factor RCC_PLL_N. This clock frequency
 * divided by the factor RCC_PLL_P_NUM results in the SysClock frequency.
 *
 * Possible values for RCC_PLL_N:       192 - 432
 * Possible values for RCC_PLL_P_NUM:   2, 4, 6, 8
 */
#define RCC_PLL_N       180

#define RCC_PLL_P_NUM   2

/** Divides the multiplied clock frequency (after RCC_PLL_N) by the factor RCC_PLL_Q.
 * This frequency supplies the I2S clock (currently not used)
 */
#define RCC_PLL_Q    8

/**
 * AHB Prescaler for HCLK. Divides the SYSCLK frequency by factor RCC_AHBCLKDivider_NUM
 *
 * Possible values: 1, 2, 4, 8, 16, 64, 128, 256, 512
 */
#define RCC_AHBCLKDivider_NUM       1

/**
 * APB1 Prescaler for APB1 Clock. Divides the HCLK frequency by factor RCC_APB1CLKDivider_NUM.
 *
 * Possible values: 1, 2, 4, 8, 16
 */
#define RCC_APB1CLKDivider_NUM      4

/**
 * APB2 Prescaler for APB2 Clock. Divides the HCLK frequency by factor RCC_APB1CLKDivider_NUM.
 *
 * Possible values: 1, 2, 4, 8, 16
 */
#define RCC_APB2CLKDivider_NUM      2

/**
 * Calculates the different clock frequencies in [Hz]
 */
#define RCC_SYSTEM_CLOCK    (HSE_VALUE/RCC_PLL_M/RCC_PLL_P_NUM*RCC_PLL_N)
#define RCC_AHB_CLOCK       (RCC_SYSTEM_CLOCK/RCC_AHBCLKDivider_NUM)
#define RCC_APB1_CLOCK      (RCC_AHB_CLOCK/RCC_APB1CLKDivider_NUM)
#define RCC_APB2_CLOCK      (RCC_AHB_CLOCK/RCC_APB2CLKDivider_NUM)


#if RCC_PLL_P_NUM == 2
    #define RCC_PLL_P    RCC_PLLP_DIV2
#elif RCC_PLL_P_NUM == 4
    #define RCC_PLL_P    RCC_PLLP_DIV2
#elif RCC_PLL_P_NUM == 6
    #define RCC_PLL_P    RCC_PLLP_DIV2
#elif RCC_PLL_P_NUM == 8
    #define RCC_PLL_P    RCC_PLLP_DIV2
#else
    #error RCC_PLL_P_NUM
#endif

#if RCC_AHBCLKDivider_NUM == 1
    #define RCC_AHBCLKDivider    RCC_SYSCLK_DIV1
#elif RCC_AHBCLKDivider_NUM == 2
    #define RCC_AHBCLKDivider    RCC_SYSCLK_DIV2
#elif RCC_AHBCLKDivider_NUM == 4
    #define RCC_AHBCLKDivider    RCC_SYSCLK_DIV4
#elif RCC_AHBCLKDivider_NUM == 8
    #define RCC_AHBCLKDivider    RCC_SYSCLK_DIV8
#elif RCC_AHBCLKDivider_NUM == 16
    #define RCC_AHBCLKDivider    RCC_SYSCLK_DIV16
#elif RCC_AHBCLKDivider_NUM == 64
    #define RCC_AHBCLKDivider    RCC_SYSCLK_DIV64
#elif RCC_AHBCLKDivider_NUM == 128
    #define RCC_AHBCLKDivider    RCC_SYSCLK_DIV128
#elif RCC_AHBCLKDivider_NUM == 256
    #define RCC_AHBCLKDivider    RCC_SYSCLK_DIV256
#elif RCC_AHBCLKDivider_NUM == 512
    #define RCC_AHBCLKDivider    RCC_SYSCLK_DIV512
#else
    #error RCC_AHBCLKDivider_NUM
#endif


#if RCC_APB1CLKDivider_NUM == 1
    #define RCC_APB1CLKDivider    RCC_HCLK_DIV1
#elif RCC_APB1CLKDivider_NUM == 2
    #define RCC_APB1CLKDivider    RCC_HCLK_DIV2
#elif RCC_APB1CLKDivider_NUM == 4
    #define RCC_APB1CLKDivider    RCC_HCLK_DIV4
#elif RCC_APB1CLKDivider_NUM == 8
    #define RCC_APB1CLKDivider    RCC_HCLK_DIV8
#elif RCC_APB1CLKDivider_NUM == 16
    #define RCC_APB1CLKDivider    RCC_HCLK_DIV16
#else
    #error RCC_APB1CLKDivider_NUM
#endif


#if RCC_APB2CLKDivider_NUM == 1
    #define RCC_APB2CLKDivider    RCC_HCLK_DIV1
#elif RCC_APB2CLKDivider_NUM == 2
    #define RCC_APB2CLKDivider    RCC_HCLK_DIV2
#elif RCC_APB2CLKDivider_NUM == 4
    #define RCC_APB2CLKDivider    RCC_HCLK_DIV4
#elif RCC_APB2CLKDivider_NUM == 8
    #define RCC_APB2CLKDivider    RCC_HCLK_DIV8
#elif RCC_APB2CLKDivider_NUM == 16
    #define RCC_APB2CLKDivider    RCC_HCLK_DIV16
#else
    #error RCC_APB2CLKDivider_NUM
#endif


/*================== Constant and Variable Definitions ====================*/
extern RCC_OscInitTypeDef RCC_OscInitStruct;
extern RCC_ClkInitTypeDef RCC_ClkInitStruct;

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

#endif /* RCC_CFG_H_ */
