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
 * @prefix  IO_PIN_MCU_0
 *
 * @brief   Configuration for the I/O ports for MCU0 (primary)
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
#define IO_PIN_MCU_0_TRACECLK                        IO_PE_2     /* package pin:1 */
#define IO_PIN_MCU_0_TRACED0                         IO_PE_3     /* package pin:2 */
#define IO_PIN_MCU_0_TRACED1                         IO_PE_4     /* package pin:3 */
#define IO_PIN_MCU_0_TRACED2                         IO_PE_5     /* package pin:4 */
#define IO_PIN_MCU_0_TRACED3                         IO_PE_6     /* package pin:5 */

/*
 *Data Storage SPI and Supply
 */
#define IO_PIN_MCU_0_DATA_STORAGE_EEPROM_SPI_NSS        IO_PI_8     /* package pin:7 */
#define IO_PIN_MCU_0_DATA_STORAGE_MEMORY_CARD_SPI_NSS   IO_PI_9     /* package pin:11 */
#define IO_PIN_MCU_0_DATA_STORAGE_SPI_SCK               IO_PG_13    /* package pin:156 */
#define IO_PIN_MCU_0_DATA_STORAGE_SPI_MISO              IO_PG_12    /* package pin:155 */
#define IO_PIN_MCU_0_DATA_STORAGE_SPI_MOSI              IO_PG_14    /* package pin:157 */
#define IO_PIN_MCU_0_MEMORY_CARD_SUPPLY_CONTROL         IO_PC_1     /* package pin:33 */

/*
 *Flexible Memory Controller
 */
#define IO_PIN_MCU_0_FMC_RAM_A0                      IO_PF_0     /* package pin:16 */
#define IO_PIN_MCU_0_FMC_RAM_A1                      IO_PF_1     /* package pin:17 */
#define IO_PIN_MCU_0_FMC_RAM_A2                      IO_PF_2     /* package pin:18 */
#define IO_PIN_MCU_0_FMC_RAM_A3                      IO_PF_3     /* package pin:19 */
#define IO_PIN_MCU_0_FMC_RAM_A4                      IO_PF_4     /* package pin:20 */
#define IO_PIN_MCU_0_FMC_RAM_A5                      IO_PF_5     /* package pin:21 */
#define IO_PIN_MCU_0_FMC_RAM_A6                      IO_PF_12    /* package pin:60 */
#define IO_PIN_MCU_0_FMC_RAM_A7                      IO_PF_13    /* package pin:63 */
#define IO_PIN_MCU_0_FMC_RAM_A8                      IO_PF_14    /* package pin:64 */
#define IO_PIN_MCU_0_FMC_RAM_A9                      IO_PF_15    /* package pin:65 */
#define IO_PIN_MCU_0_FMC_RAM_A10                     IO_PG_0     /* package pin:66 */
#define IO_PIN_MCU_0_FMC_RAM_A11                     IO_PG_1     /* package pin:67 */
#define IO_PIN_MCU_0_FMC_RAM_D0                      IO_PD_14    /* package pin:104 */
#define IO_PIN_MCU_0_FMC_RAM_D1                      IO_PD_15    /* package pin:105 */
#define IO_PIN_MCU_0_FMC_RAM_D2                      IO_PD_0     /* package pin:142 */
#define IO_PIN_MCU_0_FMC_RAM_D3                      IO_PD_1     /* package pin:143 */
#define IO_PIN_MCU_0_FMC_RAM_D4                      IO_PE_7     /* package pin:68 */
#define IO_PIN_MCU_0_FMC_RAM_D5                      IO_PE_8     /* package pin:69 */
#define IO_PIN_MCU_0_FMC_RAM_D6                      IO_PE_9     /* package pin:70 */
#define IO_PIN_MCU_0_FMC_RAM_D7                      IO_PE_10    /* package pin:73 */
#define IO_PIN_MCU_0_FMC_RAM_D8                      IO_PE_11    /* package pin:74 */
#define IO_PIN_MCU_0_FMC_RAM_D9                      IO_PE_12    /* package pin:75 */
#define IO_PIN_MCU_0_FMC_RAM_D10                     IO_PE_13    /* package pin:76 */
#define IO_PIN_MCU_0_FMC_RAM_D11                     IO_PE_14    /* package pin:77 */
#define IO_PIN_MCU_0_FMC_RAM_D12                     IO_PE_15    /* package pin:78 */
#define IO_PIN_MCU_0_FMC_RAM_D13                     IO_PD_8     /* package pin:96 */
#define IO_PIN_MCU_0_FMC_RAM_D14                     IO_PD_9     /* package pin:97 */
#define IO_PIN_MCU_0_FMC_RAM_D15                     IO_PD_10    /* package pin:98 */
#define IO_PIN_MCU_0_FMC_RAM_SDNWE                   IO_PC_0     /* package pin:32 */
#define IO_PIN_MCU_0_FMC_RAM_SDNRAS                  IO_PF_11    /* package pin:59 */
#define IO_PIN_MCU_0_FMC_RAM_SDNE1                   IO_PH_6     /* package pin:83 */
#define IO_PIN_MCU_0_FMC_RAM_SDCKE1                  IO_PH_7     /* package pin:84 */
#define IO_PIN_MCU_0_FMC_RAM_BA0                     IO_PG_4     /* package pin:108 */
#define IO_PIN_MCU_0_FMC_RAM_BA1                     IO_PG_5     /* package pin:109 */
#define IO_PIN_MCU_0_FMC_RAM_SDCLK                   IO_PG_8     /* package pin:112 */
#define IO_PIN_MCU_0_FMC_RAM_SDNCAS                  IO_PG_15    /* package pin:160 */
#define IO_PIN_MCU_0_FMC_RAM_NBL0                    IO_PE_0     /* package pin:169 */
#define IO_PIN_MCU_0_FMC_RAM_NBL1                    IO_PE_1     /* package pin:170 */

/*
 *Debug LEDs
 */
#define IO_PIN_MCU_0_DEBUG_LED_1                     IO_PC_2     /* package pin:34 */
#define IO_PIN_MCU_0_DEBUG_LED_0                     IO_PC_3     /* package pin:35 */

/*
 *ADCs
 */
#define IO_PIN_MCU_0_ADC_CH_0                        IO_PC_4     /* package pin:54 */
#define IO_PIN_MCU_0_ADC_CH_1                        IO_PC_5     /* package pin:55 */
#define IO_PIN_MCU_0_ADC_CH_2                        IO_PB_0     /* package pin:56 */
#define IO_PIN_MCU_0_ADC_CH_3                        IO_PB_1     /* package pin:57 */
#define IO_PIN_MCU_0_ADC_CH_4                        IO_PA_1     /* package pin:41 */

/*
 *Bender
 */
#define IO_PIN_MCU_0_BENDER_SUPPLY_ENABLE            IO_PH_2     /* package pin:43 */
#define IO_PIN_MCU_0_BENDER_OK                       IO_PH_5     /* package pin:46 */
#define IO_PIN_MCU_0_BENDER_PWM                      IO_PA_3     /* package pin:47 */

/*
 *Contactors' Controll and Feedback Pins
 */
#define IO_PIN_MCU_0_INTERLOCK_CONTROL               IO_PD_4     /* package pin:146 */
#define IO_PIN_MCU_0_INTERLOCK_FEEDBACK              IO_PD_5     /* package pin:147 */
#define IO_PIN_MCU_0_CONTACTOR_0_CONTROL             IO_PI_2     /* package pin:133 */
#define IO_PIN_MCU_0_CONTACTOR_0_FEEDBACK            IO_PH_13    /* package pin:128 */
#define IO_PIN_MCU_0_CONTACTOR_1_CONTROL             IO_PI_1     /* package pin:132 */
#define IO_PIN_MCU_0_CONTACTOR_1_FEEDBACK            IO_PH_14    /* package pin:129 */
#define IO_PIN_MCU_0_CONTACTOR_2_CONTROL             IO_PI_0     /* package pin:131 */
#define IO_PIN_MCU_0_CONTACTOR_2_FEEDBACK            IO_PH_15    /* package pin:130 */
#define IO_PIN_MCU_0_CONTACTOR_3_CONTROL             IO_PA_9     /* package pin:120 */
#define IO_PIN_MCU_0_CONTACTOR_3_FEEDBACK            IO_PC_6     /* package pin:115 */
#define IO_PIN_MCU_0_CONTACTOR_4_CONTROL             IO_PA_8     /* package pin:119 */
#define IO_PIN_MCU_0_CONTACTOR_4_FEEDBACK            IO_PC_7     /* package pin:116 */
#define IO_PIN_MCU_0_CONTACTOR_5_CONTROL             IO_PC_9     /* package pin:118 */
#define IO_PIN_MCU_0_CONTACTOR_5_FEEDBACK            IO_PC_8     /* package pin:117 */
#define IO_PIN_MCU_0_CONTACTOR_6_CONTROL             IO_PC_10    /* package pin:139 */
#define IO_PIN_MCU_0_CONTACTOR_6_FEEDBACK            IO_PC_11    /* package pin:140 */
#define IO_PIN_MCU_0_CONTACTOR_7_CONTROL             IO_PD_2     /* package pin:144 */
#define IO_PIN_MCU_0_CONTACTOR_7_FEEDBACK            IO_PB_6     /* package pin:164 */
#define IO_PIN_MCU_0_CONTACTOR_8_CONTROL             IO_PH_3     /* package pin:44 */
#define IO_PIN_MCU_0_CONTACTOR_8_FEEDBACK            IO_PH_4     /* package pin:45 */

/*
 *Interfaces
 */
#define IO_PIN_MCU_0_BMS_INTERFACE_SPI_NSS           IO_PA_4     /* package pin:50 */
#define IO_PIN_MCU_0_BMS_INTERFACE_SPI_SCK           IO_PA_5     /* package pin:51 */
#define IO_PIN_MCU_0_BMS_INTERFACE_SPI_MISO          IO_PA_6     /* package pin:52 */
#define IO_PIN_MCU_0_BMS_INTERFACE_SPI_MOSI          IO_PA_7     /* package pin:53 */
#define IO_PIN_MCU_0_TO_MCU_1_INTERFACE_SPI_NSS      IO_PB_12    /* package pin:92 */
#define IO_PIN_MCU_0_TO_MCU_1_INTERFACE_SPI_SCK      IO_PD_3     /* package pin:145 */
#define IO_PIN_MCU_0_TO_MCU_1_INTERFACE_SPI_MISO     IO_PB_14    /* package pin:94 */
#define IO_PIN_MCU_0_TO_MCU_1_INTERFACE_SPI_MOSI     IO_PB_15    /* package pin:95 */
#define IO_PIN_MCU_0_TO_FPGA_INTERFACE_SPI_NSS       IO_PF_6     /* package pin:24 */
#define IO_PIN_MCU_0_TO_FPGA_INTERFACE_SPI_SCK       IO_PF_7     /* package pin:25 */
#define IO_PIN_MCU_0_TO_FPGA_INTERFACE_SPI_MISO      IO_PF_8     /* package pin:26 */
#define IO_PIN_MCU_0_TO_FPGA_INTERFACE_SPI_MOSI      IO_PF_9     /* package pin:27 */
#define IO_PIN_MCU_0_CAN_0_TRANS_STANDBY_CONTROL     IO_PC_12    /* package pin:141 */
#define IO_PIN_MCU_0_CAN_0_TX                        IO_PB_13    /* package pin:93 */
#define IO_PIN_MCU_0_CAN_0_RX                        IO_PB_5     /* package pin:163 */
#define IO_PIN_MCU_0_CAN_1_TRANS_STANDBY_CONTROL     IO_PB_7     /* package pin:165 */
#define IO_PIN_MCU_0_CAN_1_RX                        IO_PB_8     /* package pin:167 */
#define IO_PIN_MCU_0_CAN_1_TX                        IO_PB_9     /* package pin:168 */
#define IO_PIN_MCU_0_FTDI_TX                         IO_PB_10    /* package pin:79 */
#define IO_PIN_MCU_0_FTDI_RX                         IO_PB_11    /* package pin:80 */
#define IO_PIN_MCU_0_RS485_NRE                       IO_PG_2     /* package pin:106 */
#define IO_PIN_MCU_0_RS485_DE                        IO_PG_3     /* package pin:107 */
#define IO_PIN_MCU_0_RS485_TX                        IO_PA_2     /* package pin:42 */
#define IO_PIN_MCU_0_RS485_RX                        IO_PD_6

#define IO_PIN_MCU_0_BMS_INTERFACE_0_GPIO_0          IO_PA_10    /* package pin:121 */
#define IO_PIN_MCU_0_BMS_INTERFACE_0_GPIO_1          IO_PA_11    /* package pin:122 */
#define IO_PIN_MCU_0_BMS_INTERFACE_0_GPIO_2          IO_PA_12    /* package pin:123 */
#define IO_PIN_MCU_0_BMS_INTERFACE_ISOSPI_DIRECTION  IO_PB_4     /* package pin:162 */

/*
 *Isolated In- and Outputs
 */
#define IO_PIN_MCU_0_ISO_GPIO_IN_0                   IO_PD_12    /* package pin:152 */
#define IO_PIN_MCU_0_ISO_GPIO_OUT_0                  IO_PG_9     /* package pin:100 */
#define IO_PIN_MCU_0_ISO_GPIO_IN_1                   IO_PD_13    /* package pin:153 */
#define IO_PIN_MCU_0_ISO_GPIO_OUT_1                  IO_PG_10    /* package pin:101 */
#define IO_PIN_MCU_0_ISO_GPIO_IN_2                   IO_PG_11    /* package pin:174 */
#define IO_PIN_MCU_0_ISO_GPIO_OUT_2                  IO_PI_5     /* package pin:154 */
#define IO_PIN_MCU_0_ISO_GPIO_IN_3                   IO_PI_4     /* package pin:175 */
#define IO_PIN_MCU_0_ISO_GPIO_OUT_3                  IO_PI_6     /* package pin:173 */
#define IO_PIN_MCU_0_ISONOC_0_CONTROL                IO_PH_8     /* package pin:85 */
#define IO_PIN_MCU_0_ISONOC_1_CONTROL                IO_PH_9     /* package pin:86 */
#define IO_PIN_MCU_0_ISONOC_2_CONTROL                IO_PH_10    /* package pin:87 */
#define IO_PIN_MCU_0_ISONOC_3_CONTROL                IO_PH_11    /* package pin:88 */
#define IO_PIN_MCU_0_ISONOC_4_CONTROL                IO_PH_12    /* package pin:89 */
#define IO_PIN_MCU_0_ISONOC_5_CONTROL                IO_PD_11    /* package pin:99 */

/*
 *Special Pins
 */
/* VBATT                                                      package pin:6 */
/* RTC_XTAL                                       IO_PC_14    package pin:9 */
/* RTC_XTAL                                       IO_PC_15    package pin:10 */
/* XTAL                                           IO_PH_0     package pin:29 */
/* XTAL                                           IO_PH_1     package pin:30 */
/* NRST                                                       package pin:31 */
/* GNDA                                                       package pin:37 */
/* VREF+                                                      package pin:38 */
/* VCCA                                                       package pin:39 */
/* Wakeup                                         IO_PA_0     package pin:40 */
/* BOOT1                                          IO_PB_2     package pin:58 */
/* BYPASS_REG                                                 package pin:48 */
/* VCAP_1                                                     package pin:81 */
/* SWDIO                                          IO_PA_13    package pin:124 */
/* VCAP_2                                                     package pin:125 */
/* SWCLK                                          IO_PA_14    package pin:137 */
/* TDI                                            IO_PA_15    package pin:138 */
/* SWO                                            IO_PB_3     package pin:161 */
/* BOOT0                                                      package pin:166 */
/* PDR_ON                                                     package pin:171 */

/*
 *Ground Pins
 */
/* GND                                                        package pin:14 */
/* GND                                                        package pin:22 */
/* GND                                                        package pin:61 */
/* GND                                                        package pin:71 */
/* GND                                                        package pin:90 */
/* GND                                                        package pin:102 */
/* GND                                                        package pin:113 */
/* GND                                                        package pin:126 */
/* GND                                                        package pin:135 */
/* GND                                                        package pin:148 */
/* GND                                                        package pin:158 */

/*
 *Supply Pins
 */
/* VCC                                                        package pin:15 */
/* VCC                                                        package pin:23 */
/* VCC                                                        package pin:36 */
/* VCC                                                        package pin:49 */
/* VCC                                                        package pin:62 */
/* VCC                                                        package pin:72 */
/* VCC                                                        package pin:82 */
/* VCC                                                        package pin:91 */
/* VCC                                                        package pin:103 */
/* VCC                                                        package pin:114 */
/* VCC                                                        package pin:127 */
/* VCC                                                        package pin:136 */
/* VCC                                                        package pin:149 */
/* VCC                                                        package pin:159 */
/* VCC                                                        package pin:172 */

/*
 *Pins free to be used
 */
/* FREE FOR USAGE                                 IO_PC_13    package pin:8 */
/* FREE FOR USAGE                                 IO_PI_10    package pin:12 */
/* FREE FOR USAGE                                 IO_PI_11    package pin:13 */
/* FREE FOR USAGE                                 IO_PF_10    package pin:28 */
/* FREE FOR USAGE                                 IO_PG_6     package pin:110 */
/* FREE FOR USAGE                                 IO_PG_7     package pin:111 */
/* FREE FOR USAGE                                 IO_PI_3     package pin:134 */
/* FREE FOR USAGE                                 IO_PD_7     package pin:151 */
/* FREE FOR USAGE                                 IO_PI_7     package pin:176 */


/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

#endif /* IO_MCU_CFG_H_ */
