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
 * @file    batterysystem_cfg.h
 * @author  foxBMS Team
 * @date    20.02.2017 (date of creation)
 * @ingroup BATTER_SYSTEM_CONF
 * @prefix  BS
 *
 * @brief   Configuration of the battery system (e.g., number of battery modules, battery cells, temperature sensors)
 *
 * This files contains basic macros of the battery system in order to derive needed inputs
 * in other parts of the software. These macros are all depended on the hardware.
 *
 */

#ifndef BATTERYSYSTEM_CFG_H_
#define BATTERYSYSTEM_CFG_H_

/*================== Includes =============================================*/
#include "general.h"

/*================== Macros and Definitions ===============================*/

/**
 * Secondary goes in error state in case of SOA violation.
 * When set to FALSE, secondary remains in error state when the SOA has been violated.
 * When set to TRUE, goes back to stand-by when the violation disappeared.
 *
 */
#define SECONDARY_OUT_OF_ERROR_STATE     FALSE

/**
 * @ingroup CONFIG_BATTERYSYSTEM
 * number of modules in battery pack
 * \par Type:
 * int
 * \par Default:
 * 2
*/
#define BS_NR_OF_MODULES                           1

/**
 * @ingroup CONFIG_BATTERYSYSTEM
 * number of battery cells per battery module (parallel cells are counted as one)
 * \par Type:
 * int
 * \par Default:
 * 12
*/
#define BS_NR_OF_BAT_CELLS_PER_MODULE               12

#if BS_NR_OF_BAT_CELLS_PER_MODULE<=12
    #define BS_MAX_SUPPORTED_CELLS         12
#elif BS_NR_OF_BAT_CELLS_PER_MODULE<=15
    #define BS_MAX_SUPPORTED_CELLS         15
#elif BS_NR_OF_BAT_CELLS_PER_MODULE<=18
    #define BS_MAX_SUPPORTED_CELLS         18
#else
    #error "Unsupported number of cells per module, higher than 18"
#endif

/* Number of GPIOs on the LTC IC
 * 5 for 12 cell version
 * 9 for 18 cell version
 */
#if BS_MAX_SUPPORTED_CELLS==12
    #define BS_NR_OF_GPIOS_PER_MODULE               5
#else
    #define BS_NR_OF_GPIOS_PER_MODULE               9
#endif

/*
 * specify the type of LTC connected to the battery module
 * @var LTC 6804 used
 * @type select(2)
 * @default  0
 * @level    user
 * @group    GENERAL
 */
/* #define BS_LTC6804_USED      TRUE */
#define BS_LTC6804_USED      FALSE

/*
 * specify the type of LTC connected to the battery module
 * @var LTC 6810 used
 * @type select(2)
 * @default  0
 * @level    user
 * @group    GENERAL
 */
/* #define BS_LTC6810_USED      TRUE */
#define BS_LTC6810_USED      FALSE

/*
 * specify the type of LTC connected to the battery module
 * @var LTC 6811 used
 * @type select(2)
 * @default  1
 * @level    user
 * @group    GENERAL
 */
#define BS_LTC6811_USED      TRUE
/* #define BS_LTC6811_USED      FALSE */

/*
 * specify the type of LTC connected to the battery module
 * @var LTC 6812 used
 * @type select(2)
 * @default  0
 * @level    user
 * @group    GENERAL
 */
/* #define BS_LTC6812_USED      TRUE */
#define BS_LTC6812_USED      FALSE

/*
 * specify the type of LTC connected to the battery module
 * @var LTC 6813 used
 * @type select(2)
 * @default  0
 * @level    user
 * @group    GENERAL
 */
/* #define BS_LTC6813_USED      TRUE */
#define BS_LTC6813_USED      FALSE

/**
 * @ingroup CONFIG_BATTERYSYSTEM
 * number of temperature sensors per battery module
 * \par Type:
 * int
 * \par Default:
 * 6
*/
#define BS_NR_OF_TEMP_SENSORS_PER_MODULE            8


#define BS_NR_OF_BAT_CELLS                         (BS_NR_OF_MODULES * BS_NR_OF_BAT_CELLS_PER_MODULE)
#define BS_NR_OF_TEMP_SENSORS                      (BS_NR_OF_MODULES * BS_NR_OF_TEMP_SENSORS_PER_MODULE)

/**
 * @ingroup CONFIG_BATTERYSYSTEM
 * number of pack voltage inputs measured by current sensors (like IVT-MOD)
 * \par Type:
 * int
 * \par Default:
 * 3
*/
#define BS_NR_OF_VOLTAGES_FROM_CURRENT_SENSOR      3

/**
 * @ingroup CONFIG_BATTERYSYSTEM
 * number of contactors
 * On the foxBMS Basic Board 6 contactors are supported. On the foxBMS
 * Basic Extension board one can use 3 more contactors.
 * \par Type:
 * int
 * \par Default:
 * 3
*/
#define BS_NR_OF_CONTACTORS                        6

/**
 * @ingroup CONFIG_BATTERYSYSTEM
 * separation of charge and discharge power line
 * \par Type:
 * select(2)
 * \par Default:
 * 1
*/
/* #define BS_SEPARATE_POWERLINES 0 */
#define BS_SEPARATE_POWERLINES 1

/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

#endif /* BATTERYSYSTEM_CFG_H_ */
