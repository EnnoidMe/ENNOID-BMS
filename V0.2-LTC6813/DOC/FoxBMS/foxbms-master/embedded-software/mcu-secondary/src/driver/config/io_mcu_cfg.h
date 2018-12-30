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
 * @file    io_mcu_cfg.h
 * @author  foxBMS Team
 * @date    18.01.2016 (date of creation)
 * @ingroup DRIVERS_CONF
 * @prefix  IO_PIN_MCU_1
 *
 * @brief   Configuration for the I/O ports for MCU1 (secondary).
 *
 * This file describes the names of the signals connected
 * to the given hardware pin at the cpu.
 *
 */

#ifndef IO_MCU_CFG_H_
#define IO_MCU_CFG_H_

/*================== Includes =============================================*/
#include "io_package_cfg.h"

/*================== Macros and Definitions ===============================*/

/*
 *Trace
 */
#define IO_PIN_MCU_1_TRACECLK                        IO_PE_2
#define IO_PIN_MCU_1_TRACED0                         IO_PE_3
#define IO_PIN_MCU_1_TRACED1                         IO_PE_4
#define IO_PIN_MCU_1_TRACED2                         IO_PE_5
#define IO_PIN_MCU_1_TRACED3                         IO_PE_6

/*
 *Debug LEDs
 */
#define IO_PIN_MCU_1_DEBUG_LED_1                     IO_PC_2
#define IO_PIN_MCU_1_DEBUG_LED_0                     IO_PC_3

/*
 *Interfaces
 */
#define IO_PIN_MCU_1_BMS_INTERFACE_SPI_NSS           IO_PA_4
#define IO_PIN_MCU_1_BMS_INTERFACE_SPI_SCK           IO_PA_5
#define IO_PIN_MCU_1_BMS_INTERFACE_SPI_MISO          IO_PA_6
#define IO_PIN_MCU_1_BMS_INTERFACE_SPI_MOSI          IO_PA_7

#define IO_PIN_MCU_1_USART_TX                        IO_PB10
#define IO_PIN_MCU_1_USART_RX                        IO_PB11

#define IO_PIN_MCU_1_TO_MCU_0_INTERFACE_SPI_NSS      IO_PB_12
#define IO_PIN_MCU_1_TO_MCU_0_INTERFACE_SPI_SCK      IO_PD_3
#define IO_PIN_MCU_1_TO_MCU_0_INTERFACE_SPI_MOSI     IO_PB_14
#define IO_PIN_MCU_1_TO_MCU_0_INTERFACE_SPI_MISO     IO_PB_15




/*
 *Interlock and Feedback Pins
 */
#define IO_PIN_MCU_1_INTERLOCK_CONTROL               IO_PD_4
#define IO_PIN_MCU_1_INTERLOCK_FEEDBACK              IO_PD_5

/*
 *Interfaces
 */

#define IO_PIN_MCU_1_CAN_0_TRANS_STANDBY_CONTROL     IO_PC_12
#define IO_PIN_MCU_1_CAN_0_TX                        IO_PB_13
#define IO_PIN_MCU_1_CAN_0_RX                        IO_PB_5

#define IO_PIN_MCU_1_FTDI_TX                         IO_PA_11
#define IO_PIN_MCU_1_FTDI_RX                         IO_PA_12

#define IO_PIN_MCU_1_BMS_INTERFACE_0_GPIO_0          IO_PA_10
#define IO_PIN_MCU_1_BMS_INTERFACE_0_GPIO_1          IO_PA_11
#define IO_PIN_MCU_1_BMS_INTERFACE_0_GPIO_2          IO_PA_12
#define IO_PIN_MCU_1_BMS_INTERFACE_ISOSPI_DIRECTION  IO_PB_4

#define IO_PIN_MCU_1_GPIO_0                          IO_PD_2
#define IO_PIN_MCU_1_GPIO_1                          IO_PD_1
#define IO_PIN_MCU_1_GPIO_2                          IO_PD_0
#define IO_PIN_MCU_1_GPIO_3                          IO_PC_12
#define IO_PIN_MCU_1_GPIO_4                          IO_PC_11
#define IO_PIN_MCU_1_GPIO_5                          IO_PC_10



/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

#endif /* IO_MCU_CFG_H_ */
