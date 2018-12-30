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
    {IO_PIN_MCU_1_TRACECLK,                        IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_LOW,       IO_ALTERNATE_AF0_TRACE,     IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_1_TRACED0,                         IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_LOW,       IO_ALTERNATE_AF0_TRACE,     IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_1_TRACED1,                         IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_LOW,       IO_ALTERNATE_AF0_TRACE,     IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_1_TRACED2,                         IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_LOW,       IO_ALTERNATE_AF0_TRACE,     IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_1_TRACED3,                         IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_LOW,       IO_ALTERNATE_AF0_TRACE,     IO_PIN_LOCK_ENABLE},

    /*
     * Debug LEDs
     */
    {IO_PIN_MCU_1_DEBUG_LED_1,                     IO_MODE_OUTPUT_PP,      0,                  0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_1_DEBUG_LED_0,                     IO_MODE_OUTPUT_PP,      0,                  0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},

    /*
     * Contactors' Controll and Feedback Pins
     */
    {IO_PIN_MCU_1_INTERLOCK_CONTROL,               IO_MODE_OUTPUT_PP,   0,                     0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_1_INTERLOCK_FEEDBACK,              IO_MODE_INPUT,       IO_PIN_PULLDOWN,       IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},

    /*
     * Interfaces
     */
    {IO_PIN_MCU_1_BMS_INTERFACE_SPI_NSS,           IO_MODE_OUTPUT_PP,      IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE,     IO_PIN_SET},
    {IO_PIN_MCU_1_BMS_INTERFACE_SPI_SCK,           IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF5_SPI1,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_1_BMS_INTERFACE_SPI_MISO,          IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF5_SPI1,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_1_BMS_INTERFACE_SPI_MOSI,          IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF5_SPI1,      IO_PIN_LOCK_ENABLE},

    {IO_PIN_MCU_1_TO_MCU_0_INTERFACE_SPI_NSS,      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF5_SPI1,      IO_PIN_LOCK_ENABLE,     IO_PIN_SET},
    {IO_PIN_MCU_1_TO_MCU_0_INTERFACE_SPI_SCK,      IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF5_SPI1,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_1_TO_MCU_0_INTERFACE_SPI_MISO,     IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF5_SPI1,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_1_TO_MCU_0_INTERFACE_SPI_MOSI,     IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF5_SPI1,      IO_PIN_LOCK_ENABLE},

    {IO_PIN_MCU_1_BMS_INTERFACE_0_GPIO_0,          IO_MODE_INPUT,          IO_PIN_PULLDOWN,    IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_1_BMS_INTERFACE_0_GPIO_1,          IO_MODE_INPUT,          IO_PIN_PULLDOWN,    IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_1_BMS_INTERFACE_0_GPIO_2,          IO_MODE_INPUT,          IO_PIN_PULLDOWN,    IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    /* Pin high -> forward mode, Pin low -> reverse mode */
    {IO_PIN_MCU_1_BMS_INTERFACE_ISOSPI_DIRECTION,  IO_MODE_OUTPUT_PP,      IO_PIN_NOPULL,      IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE,     IO_PIN_SET},

    {IO_PIN_MCU_1_GPIO_0,                          IO_MODE_INPUT,          IO_PIN_PULLDOWN,    IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_1_GPIO_1,                          IO_MODE_INPUT,          IO_PIN_PULLDOWN,    IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_1_GPIO_2,                          IO_MODE_INPUT,          IO_PIN_PULLDOWN,    IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_1_GPIO_3,                          IO_MODE_INPUT,          IO_PIN_PULLDOWN,    IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_1_GPIO_4,                          IO_MODE_INPUT,          IO_PIN_PULLDOWN,    IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_1_GPIO_5,                          IO_MODE_INPUT,          IO_PIN_PULLDOWN,    IO_SPEED_FAST,      IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},

    {IO_PIN_MCU_1_CAN_0_TRANS_STANDBY_CONTROL,     IO_MODE_OUTPUT_PP,      0,                  0,                  IO_ALTERNATE_NO_ALTERNATE,  IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_1_CAN_0_TX,                        IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF9_CAN0,      IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_1_CAN_0_RX,                        IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF9_CAN0,      IO_PIN_LOCK_ENABLE},

    {IO_PIN_MCU_1_FTDI_TX,                         IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF7_USART3,    IO_PIN_LOCK_ENABLE},
    {IO_PIN_MCU_1_FTDI_RX,                         IO_MODE_AF_PP,          IO_PIN_NOPULL,      IO_SPEED_HIGH,      IO_ALTERNATE_AF7_USART3,    IO_PIN_LOCK_ENABLE},

    };

const uint8_t io_cfg_length = sizeof(io_cfg)/sizeof(io_cfg[0]);

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/
