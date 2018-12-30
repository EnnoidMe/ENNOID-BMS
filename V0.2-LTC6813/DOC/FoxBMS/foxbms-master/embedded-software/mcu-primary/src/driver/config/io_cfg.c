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
 * @file    io_cfg.c
 * @author  foxBMS Team
 * @date    26.08.2015 (date of creation)
 * @ingroup DRIVERS_CONF
 * @prefix  IO
 *
 * @brief   Configuration for the driver for the I/O ports
 *
 * ==============================================================================
 *                  Detailed Configuration of the GPIOs
 * ==============================================================================
 *
 * The functionality of the signal defined in io_foxbms_mcu0_cfg.h  is set in the array
 * io_cfg[], e.g. digital I/O or alternate functions like SPI, CAN, ...
 *
 * An incorrect definition of an signal may disturb one or more other
 * signal functional definitions of pins
 *
 * Every entry of the io_cfg[] array consists of
 *  - name of the signal/pin (defined in io_foxbms_mcu0_cfg.h)
 *  - mode of signal/pin (input, output, ...)
 *  - push/pull
 *  - pin speed
 *  - alternate function (SPI, CAN, ...)
 *  - locking of the pin configuration (has to enabled in io_cfg.h)
 * All available possibilities for each pin are found in the official<a href"http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00071990.pdf">datasheet</a>.
 *
 */

/*================== Includes =============================================*/
#include "io_cfg.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/

const IO_PIN_CFG_s io_cfg[] = {
    /*
     * Trace
     */
    {IO_PIN_MCU_0_TRACECLK,                        IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_LOW,       IO_ALTERNATE_AF0_TRACE,     IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_TRACED0,                         IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_LOW,       IO_ALTERNATE_AF0_TRACE,     IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_TRACED1,                         IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_LOW,       IO_ALTERNATE_AF0_TRACE,     IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_TRACED2,                         IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_LOW,       IO_ALTERNATE_AF0_TRACE,     IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_TRACED3,                         IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_LOW,       IO_ALTERNATE_AF0_TRACE,     IO_PIN_LOCK_ENABLE},

    /*
     * Data Storage SPI and Supply
     */
    {IO_PIN_MCU_0_DATA_STORAGE_EEPROM_SPI_NSS,     IO_MODE_OUTPUT_PP,      IO_PIN_NOPULL,      IO_SPEED_LOW,       IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE,     IO_PIN_SET},
    {IO_PIN_MCU_0_DATA_STORAGE_MEMORY_CARD_SPI_NSS,     IO_MODE_OUTPUT_PP,      IO_PIN_NOPULL,      IO_SPEED_LOW,       IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE,     IO_PIN_SET},
    {IO_PIN_MCU_0_DATA_STORAGE_SPI_MISO,           IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF5_SPI6,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_DATA_STORAGE_SPI_SCK,            IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF5_SPI6,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_DATA_STORAGE_SPI_MOSI,           IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF5_SPI6,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_MEMORY_CARD_SUPPLY_CONTROL,           IO_MODE_OUTPUT_PP,      0,                  0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},

    /*
     * Flexible Memory Controller
     */
    {IO_PIN_MCU_0_FMC_RAM_A0,                      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_A1,                      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_A2,                      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_A3,                      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_A4,                      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_A5,                      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_A6,                      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_A7,                      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_A8,                      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_A9,                      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_A10,                     IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_A11,                     IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},

    {IO_PIN_MCU_0_FMC_RAM_D0,                      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_D1,                      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_D2,                      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_D3,                      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_D4,                      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_D5,                      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_D6,                      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_D7,                      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_D8,                      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_D9,                      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_D10,                     IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_D11,                     IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_D12,                     IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_D13,                     IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_D14,                     IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_D15,                     IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_SDNWE,                   IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_SDNRAS,                  IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_SDNE1,                   IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_SDCKE1,                  IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_BA0,                     IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_BA1,                     IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_SDCLK,                   IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_NBL0,                    IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_SDNCAS,                  IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FMC_RAM_NBL1,                    IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF12_FMC,      IO_PIN_LOCK_ENABLE},

    /*
     * Debug LEDs
     */
    {IO_PIN_MCU_0_DEBUG_LED_1,                     IO_MODE_OUTPUT_PP,      0,                  0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_DEBUG_LED_0,                     IO_MODE_OUTPUT_PP,      0,                  0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},

    /*
     * ADCs
     */
    {IO_PIN_MCU_0_ADC_CH_0,                        IO_MODE_ANALOG,         0,                     0,               IO_ALTERNATE_NO_ALTERNATE,  0,                },
    {IO_PIN_MCU_0_ADC_CH_1,                        IO_MODE_ANALOG,         0,                     0,               IO_ALTERNATE_NO_ALTERNATE,  0,                },
    {IO_PIN_MCU_0_ADC_CH_2,                        IO_MODE_ANALOG,         0,                     0,               IO_ALTERNATE_NO_ALTERNATE,  0,                },
    {IO_PIN_MCU_0_ADC_CH_3,                        IO_MODE_ANALOG,         0,                     0,               IO_ALTERNATE_NO_ALTERNATE,  0,                },
    {IO_PIN_MCU_0_ADC_CH_4,                        IO_MODE_ANALOG,         0,                  0,                  IO_ALTERNATE_NO_ALTERNATE,  0,                },

    /*
     * Bender
     */
    {IO_PIN_MCU_0_BENDER_SUPPLY_ENABLE,            IO_MODE_OUTPUT_PP,      0,                  0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_BENDER_OK,                       IO_MODE_INPUT,          IO_PIN_PULLDOWN,    IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_BENDER_PWM,                      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF3_TIM9,      IO_PIN_LOCK_ENABLE},

    /*
     * Contactors' Controll and Feedback Pins
     */
    {IO_PIN_MCU_0_INTERLOCK_CONTROL,               IO_MODE_OUTPUT_PP,   0,                     0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_INTERLOCK_FEEDBACK,              IO_MODE_INPUT,       IO_PIN_PULLDOWN,       IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CONTACTOR_0_CONTROL,             IO_MODE_OUTPUT_PP,   0,                     0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CONTACTOR_0_FEEDBACK,            IO_MODE_INPUT,       IO_PIN_PULLDOWN,       IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CONTACTOR_1_CONTROL,             IO_MODE_OUTPUT_PP,   0,                     0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CONTACTOR_1_FEEDBACK,            IO_MODE_INPUT,       IO_PIN_PULLDOWN,       IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CONTACTOR_2_CONTROL,             IO_MODE_OUTPUT_PP,   0,                     0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CONTACTOR_2_FEEDBACK,            IO_MODE_INPUT,       IO_PIN_PULLDOWN,       IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CONTACTOR_3_CONTROL,             IO_MODE_OUTPUT_PP,   0,                     0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CONTACTOR_3_FEEDBACK,            IO_MODE_INPUT,       IO_PIN_PULLDOWN,       IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CONTACTOR_4_CONTROL,             IO_MODE_OUTPUT_PP,   0,                     0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CONTACTOR_4_FEEDBACK,            IO_MODE_INPUT,       IO_PIN_PULLDOWN,       IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CONTACTOR_5_CONTROL,             IO_MODE_OUTPUT_PP,   0,                     0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CONTACTOR_5_FEEDBACK,            IO_MODE_INPUT,       IO_PIN_PULLDOWN,       IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CONTACTOR_6_CONTROL,             IO_MODE_OUTPUT_PP,   0,                     0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CONTACTOR_6_FEEDBACK,            IO_MODE_INPUT,       IO_PIN_PULLDOWN,       IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CONTACTOR_7_CONTROL,             IO_MODE_OUTPUT_PP,   0,                     0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CONTACTOR_7_FEEDBACK,            IO_MODE_INPUT,       IO_PIN_PULLDOWN,       IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CONTACTOR_8_CONTROL,             IO_MODE_OUTPUT_PP,   0,                     0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CONTACTOR_8_FEEDBACK,            IO_MODE_INPUT,       IO_PIN_PULLDOWN,       IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},

    /*
     * Interfaces
     */
    {IO_PIN_MCU_0_BMS_INTERFACE_SPI_NSS,           IO_MODE_OUTPUT_PP,      IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE,     IO_PIN_SET},
    {IO_PIN_MCU_0_BMS_INTERFACE_SPI_SCK,           IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF5_SPI1,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_BMS_INTERFACE_SPI_MISO,          IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF5_SPI1,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_BMS_INTERFACE_SPI_MOSI,          IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF5_SPI1,      IO_PIN_LOCK_ENABLE},

    {IO_PIN_MCU_0_TO_MCU_1_INTERFACE_SPI_NSS,      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF5_SPI1,      IO_PIN_LOCK_ENABLE,     IO_PIN_SET},
    {IO_PIN_MCU_0_TO_MCU_1_INTERFACE_SPI_SCK,      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF5_SPI1,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_TO_MCU_1_INTERFACE_SPI_MISO,     IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF5_SPI1,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_TO_MCU_1_INTERFACE_SPI_MOSI,     IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF5_SPI1,      IO_PIN_LOCK_ENABLE},

    {IO_PIN_MCU_0_TO_FPGA_INTERFACE_SPI_NSS,       IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF5_SPI5,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_TO_FPGA_INTERFACE_SPI_SCK,       IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF5_SPI5,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_TO_FPGA_INTERFACE_SPI_MISO,      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF5_SPI5,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_TO_FPGA_INTERFACE_SPI_MOSI,      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF5_SPI5,      IO_PIN_LOCK_ENABLE},

    {IO_PIN_MCU_0_BMS_INTERFACE_0_GPIO_0,          IO_MODE_INPUT,          IO_PIN_PULLDOWN,    IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_BMS_INTERFACE_0_GPIO_1,          IO_MODE_INPUT,          IO_PIN_PULLDOWN,    IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_BMS_INTERFACE_0_GPIO_2,          IO_MODE_INPUT,          IO_PIN_PULLDOWN,    IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    /* Pin high -> forward mode, Pin low -> reverse mode */
    {IO_PIN_MCU_0_BMS_INTERFACE_ISOSPI_DIRECTION,  IO_MODE_OUTPUT_PP,      IO_PIN_NOPULL,      IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE,     IO_PIN_SET},

    {IO_PIN_MCU_0_CAN_0_TRANS_STANDBY_CONTROL,     IO_MODE_OUTPUT_PP,      0,                  0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CAN_0_TX,                        IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF9_CAN0,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CAN_0_RX,                        IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF9_CAN0,      IO_PIN_LOCK_ENABLE},

    {IO_PIN_MCU_0_CAN_1_TRANS_STANDBY_CONTROL,     IO_MODE_OUTPUT_PP,      0,                  0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CAN_1_TX,                        IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF9_CAN1,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_CAN_1_RX,                        IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF9_CAN1,      IO_PIN_LOCK_ENABLE},

    {IO_PIN_MCU_0_FTDI_TX,                         IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF7_USART3,    IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_FTDI_RX,                         IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF7_USART3,    IO_PIN_LOCK_ENABLE},

    {IO_PIN_MCU_0_RS485_TX,                        IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF7_USART2,    IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_RS485_RX,                        IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF7_USART2,    IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_RS485_NRE,                       IO_MODE_OUTPUT_PP,      0,                  0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE, IO_PIN_RESET},
    {IO_PIN_MCU_0_RS485_DE,                        IO_MODE_OUTPUT_PP,      0,                  0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE, IO_PIN_SET},

    /*
     * Isolated In- and Outputs
     */
    {IO_PIN_MCU_0_ISO_GPIO_IN_0,                   IO_MODE_INPUT,          IO_PIN_PULLDOWN,       IO_SPEED_FAST,   IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_ISO_GPIO_OUT_0,                  IO_MODE_OUTPUT_PP,      0,                     0,               IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_ISO_GPIO_IN_1,                   IO_MODE_INPUT,          IO_PIN_PULLDOWN,       IO_SPEED_FAST,   IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_ISO_GPIO_OUT_1,                  IO_MODE_OUTPUT_PP,      0,                     0,               IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_ISO_GPIO_IN_2,                   IO_MODE_INPUT,          IO_PIN_PULLDOWN,       IO_SPEED_FAST,   IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_ISO_GPIO_OUT_2,                  IO_MODE_OUTPUT_PP,      0,                     0,               IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_ISO_GPIO_IN_3,                   IO_MODE_INPUT,          IO_PIN_PULLDOWN,       IO_SPEED_FAST,   IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_ISO_GPIO_OUT_3,                  IO_MODE_OUTPUT_PP,      0,                     0,               IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},

    {IO_PIN_MCU_0_ISONOC_0_CONTROL,                IO_MODE_OUTPUT_PP,      0,                     0,               IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_ISONOC_1_CONTROL,                IO_MODE_OUTPUT_PP,      0,                     0,               IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_ISONOC_2_CONTROL,                IO_MODE_OUTPUT_PP,      0,                     0,               IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_ISONOC_3_CONTROL,                IO_MODE_OUTPUT_PP,      0,                     0,               IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_ISONOC_4_CONTROL,                IO_MODE_OUTPUT_PP,      0,                     0,               IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_0_ISONOC_5_CONTROL,                IO_MODE_OUTPUT_PP,      0,                     0,               IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},


    };

const uint8_t io_cfg_length = sizeof(io_cfg)/sizeof(io_cfg[0]);

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/
