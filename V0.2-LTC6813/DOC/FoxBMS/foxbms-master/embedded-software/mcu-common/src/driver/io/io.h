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
 * @file    io.h
 * @author  foxBMS Team
 * @date    26.08.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  IO
 *
 * @brief   Header for the driver for the I/O ports (pins)..
 *
 * The io-module allows a neat configuration and initialization of the microcontroller
 * pins of the foxbms hardware. The io-module enables an easy read and write access
 * to the microcontroller pins.
 *
 * The configuration of the GPIOs is performed through the following parts of the io-module:
 * - config\io_package_cfg.h:
 *   - If the foxBMS board is used, no changes have to be made my the user.
 * - Configuration file config\io_foxbms_mcu0_cfg.h respectively config.h.h:
 *   - Defines the names of the signals connected to the pins of the STM32F429. The macro used
 *     for identification of the pin-port assignment are taken from io_package_cfg.h
 * - config\io_cfg.h:
 *   - Defines the package type of the STM32F29 used on the board. The list of supported packages
 *     of the STM32F429 can be found in io_package_cfg.h.
 *   - Defines if the pin configuration locking of the is enabled.
 *   - Includes the pin configuration file io_foxbms_mcu0_cfg.h.
 * - config\io_cfg.c
 *   - Determines the configuration of each used pin. The configuration is in the io_cfg[] array.
 *     The possible configurations of each GPIO are given in the official STM32F29
 *     <a href"http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00071990.pdf">datasheet</a>.
 *     The io-driver DOES NOT check whether your GPIO configuration is valid.
 */

#ifndef IO_H_
#define IO_H_

/*================== Includes =============================================*/
#include "io_cfg.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/

/**
 * @brief   Initializes all ports and pins of the microcontroller
 *
 * The pins of the microcontroller package are initialized
 * by calling IO_Init(). The pins are identified by a signal
 * name given in io_cfg_foxbms_mcu0_generated.h. The
 * function gets the configuration of the pins by the array
 * *io_cfg, which is defined is done in io_cfg.c.
 * The io_cfg array allows you to set the functionality of
 * the pin in the following order of the IO_PIN_CFG_s struct:
 * - pin
 *  - signal name
 * - mode
 *  -input
 *  - output
 *  - push-pull
 *  - open drain
 *  - analog
 *  - interrupt
 *   - rising edge
 *   - falling edge
 *   - rising falling edge
 *  - event
 *   - rising edge
 *   - falling edge
 *   - rising falling edge
 * - logic level
 *  - no pull
 *  - pull up
 *  - pull down
 * - speed
 *  - low
 *  - medium
 *  - fast
 *  - high
 * - alternate functions
 *  - RTC_50Hz
 *  - MCO
 *  - TAMPER
 *  - SWJ (SWD and JTAG)
 *  - TRACE
 *  - TIM
 *  - I2C
 *  - SPI
 *  - I2S
 *  - SAI
 *  - USART
 *  - UART
 *  - CAN
 *  - LCD-TFT
 *  - OTG
 * - pin configuration locking
 *  - disable
 *  - enable
 * - default value of the pin
 *  - reset/low
 *  - set/high
 *
 * If the define IO_PIN_LOCKING is set in file io_cfg.h the pin configuration locking is
 * performed as defined in file io_cfg.c at the array IO_PIN_CFG_s io_cfg.
 *
 * The default value of the pin/signal is optional. If the not given the signal is set low.
 *
 * @param   *io_cfg (type: IO_PIN_CFG_s)
 *
 * @return  retVal (type: STD_RETURN_TYPE_e)
 */
extern STD_RETURN_TYPE_e IO_Init(const IO_PIN_CFG_s *io_cfg);

/**
 * @brief   Returns the state of a pin
 *
 * Reads the state/output of the given pin, which is given by its
 * symbolic name.
 *
 * @param   pin (type: IO_PORTS_e)
 *
 * @return  currentPinState (type: IO_PIN_STATE_e) returns
 *          currentPinState  | physical pin out
 *          ---------------- | -----------------
 *          GPIO_PIN_SET     | high
 *          GPIO_PIN_RESET   | low
 */
extern IO_PIN_STATE_e IO_ReadPin(IO_PORTS_e pin);

/**
 * @brief   Sets the state of a pin
 *
 * Sets the state/output of the given pin, which is given by its
 * symbolic name.
 *
 * @param   pin (type: IO_PORTS_e) requested pin to be set
 *
 * @param   requestedPinState (type: IO_PIN_STATE_e) requests
 *          requestedPinState   | physical pin out
 *          ------------------- | -----------------
 *          GPIO_PIN_SET        | high
 *          GPIO_PIN_RESET      | low
 */
extern void IO_WritePin(IO_PORTS_e pin, IO_PIN_STATE_e requestedPinState);

/**
 * @brief   Toggles the state of a pin
 *
 * @param   pin (type: IO_PORTS_e)
 */
extern void IO_TogglePin(IO_PORTS_e pin);

/**
 * @brief   Calls the EXTI_IRQHandler of the given pin
 *
 * @param   pin (type: IO_PORTS_e)
 */
extern void IO_EXTI_IRQHandler(IO_PORTS_e pin);

/**
 * @brief   Calls the EXTI_Callback of the given pin
 *
 * @param   pin (type: IO_PORTS_e)
 */
extern void IO_EXTI_Callback(IO_PORTS_e pin);

/*================== Function Implementations =============================*/

#endif /* IO_H_ */
