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
 * @file    io_cfg.h
 * @author  foxBMS Team
 * @date    26.08.2015 (date of creation)
 * @ingroup DRIVERS_CONF
 * @prefix  IO
 *
 * @brief   Headers for the configuration for the driver for the I/O ports (pins).
 *
 * ==============================================================================
 *                  Configuration of the GPIOs
 * ==============================================================================
 *
 * Supported packages of the STM32F429
 * ===================================
 * In io_cfg.h the users defines the used package of the STM32F429 by a macro.
 * The following table shows the package and its corresponding macro:
 * Package | Macro
 * --------| ------------------
 * LQFP100 | IO_PACKAGE_LQFP100
 * LQFP144 | IO_PACKAGE_LQFP144
 * LQFP176 | IO_PACKAGE_LQFP176
 *
 * The global enable for locking of pin configuration is set (Macro IO_PIN_LOCKING).
 * Enabling this macro allows to specify in a locking variable in io_cfg[] array in
 * io_cfg.c.
 *
 *
 */

#ifndef IO_CFG_H_
#define IO_CFG_H_

/*================== Includes =============================================*/
#include "general.h"

#include "cpu_cfg.h"
#include "io_mcu_cfg.h"

/*================== Macros and Definitions ===============================*/

/**
 * @ingroup CONFIG_IO
 * description if port locking is enabled, one can specify in the
 * io_cfg[] array each pin configuration as locked/unlocked.
 * \par Type:
 * toggle
 * \par Default:
 * True
*/

/**
 * Enables pin locking globally. If this macro is not defined, pin locking
 * can not be activated.
 */
/* #define IO_PIN_LOCKING */

/*================== Constant and Variable Definitions ====================*/
/**
 * symbolic names for possible modes of the pins
 */
typedef enum {
    IO_MODE_INPUT               = GPIO_MODE_INPUT,              /*!< The pin is used as input                               */
    IO_MODE_OUTPUT_PP           = GPIO_MODE_OUTPUT_PP,          /*!< The pin is used as output with push-pull               */
    IO_MODE_OUTPUT_OD           = GPIO_MODE_OUTPUT_OD,          /*!< The pin is used as output with open-drain              */
    IO_MODE_AF_PP               = GPIO_MODE_AF_PP,              /*!< The pin is used as alternate function with push-pull   */
    IO_MODE_AF_OD               = GPIO_MODE_AF_OD,              /*!< The pin is used as alternate function with open-drain  */

    IO_MODE_ANALOG              = GPIO_MODE_ANALOG,             /*!< The pin is used as analog pin                          */

    IO_MODE_IT_RISING           = GPIO_MODE_IT_RISING,          /*!< The pin is used as interrupt at rising edge            */
    IO_MODE_IT_FALLING          = GPIO_MODE_IT_FALLING,         /*!< The pin is used as interrupt at falling edge           */
    IO_MODE_IT_RISING_FALLING   = GPIO_MODE_IT_RISING_FALLING,  /*!< The pin is used as interrupt at rising/falling edge    */

    IO_MODE_EVT_RISING          = GPIO_MODE_EVT_RISING,         /*!< The pin is used as event mode at rising edge           */
    IO_MODE_EVT_FALLLING        = GPIO_MODE_EVT_FALLING,        /*!< The pin is used as event mode at falling edge          */
    IO_MODE_EVT_RISING_FALLING  = GPIO_MODE_EVT_RISING_FALLING  /*!< The pin is used as event mode at rising/falling edge   */
} IO_PIN_MODES_e;

/**
 * symbolic names for possible speeds of the pins
 */
typedef enum {
    IO_SPEED_LOW    = GPIO_SPEED_LOW,       /*!< The pin speed is low       */
    IO_SPEED_MEDIUM = GPIO_SPEED_MEDIUM,    /*!< The pin speed is medium    */
    IO_SPEED_FAST   = GPIO_SPEED_FAST,      /*!< The pin speed is fast      */
    IO_SPEED_HIGH   = GPIO_SPEED_HIGH       /*!< The pin speed is high      */
} IO_PIN_SPEEDS_e;

/**
 * symbolic names for possible pull-up or pull-downs of the pins
 */
typedef enum {
    IO_PIN_NOPULL   = GPIO_NOPULL,      /*!< Whether pull-up nor pull-down is activated */
    IO_PIN_PULLUP   = GPIO_PULLUP,      /*!< Pull-up is activated                       */
    IO_PIN_PULLDOWN = GPIO_PULLDOWN     /*!< Pull-down is activated                     */

} IO_PIN_PULL_e;

/**
 * symbolic names for possible alternate functions of the pins
 */
typedef enum {
    IO_ALTERNATE_AF0_RTC_50Hz   = GPIO_AF0_RTC_50Hz,    /*!< Real-time Clock 50Hz                                       */
    IO_ALTERNATE_AF0_MCO        = GPIO_AF0_MCO,         /*!< Microcontroller clock output, (MCO1 and MCO2)              */
    IO_ALTERNATE_AF0_SWJ        = GPIO_AF0_SWJ,         /*!< Serial Wire Debug or JTAG                                  */
    IO_ALTERNATE_AF0_TRACE      = GPIO_AF0_TRACE,       /*!< Trace                                                      */

    IO_ALTERNATE_AF1_TIM1       = GPIO_AF1_TIM1,        /*!< TIM1                                                       */
    IO_ALTERNATE_AF1_TIM2       = GPIO_AF1_TIM2,        /*!< TIM2                                                       */

    IO_ALTERNATE_AF2_TIM3       = GPIO_AF2_TIM3,        /*!< TIM3                                                       */
    IO_ALTERNATE_AF2_TIM4       = GPIO_AF2_TIM4,        /*!< TIM4                                                       */
    IO_ALTERNATE_AF2_TIM5       = GPIO_AF2_TIM5,        /*!< TIM5                                                       */

    IO_ALTERNATE_AF3_TIM8       = GPIO_AF3_TIM8,        /*!< TIM8                                                       */
    IO_ALTERNATE_AF3_TIM9       = GPIO_AF3_TIM9,        /*!< TIM9                                                       */
    IO_ALTERNATE_AF3_TIM10      = GPIO_AF3_TIM10,       /*!< TIM10                                                      */
    IO_ALTERNATE_AF3_TIM11      = GPIO_AF3_TIM11,       /*!< TIM11                                                      */

    IO_ALTERNATE_AF4_I2C1       = GPIO_AF4_I2C1,        /*!< Inter-Integrated Circuit Bus 1                             */
    IO_ALTERNATE_AF4_I2C2       = GPIO_AF4_I2C2,        /*!< Inter-Integrated Circuit Bus 2                             */
    IO_ALTERNATE_AF4_I2C3       = GPIO_AF4_I2C3,        /*!< Inter-Integrated Circuit Bus 3                             */

    IO_ALTERNATE_AF5_SPI1       = GPIO_AF5_SPI1,        /*!< Serial Peripheral Interface Bus 1                          */
    IO_ALTERNATE_AF5_SPI2       = GPIO_AF5_SPI2,        /*!< Serial Peripheral Interface Bus 2                          */
    IO_ALTERNATE_AF5_SPI3       = GPIO_AF5_SPI3,        /*!< Serial Peripheral Interface Bus 3                          */
    IO_ALTERNATE_AF5_SPI4       = GPIO_AF5_SPI4,        /*!< Serial Peripheral Interface Bus 4                          */
    IO_ALTERNATE_AF5_SPI5       = GPIO_AF5_SPI5,        /*!< Serial Peripheral Interface Bus 5                          */
    IO_ALTERNATE_AF5_SPI6       = GPIO_AF5_SPI6,        /*!< Serial Peripheral Interface Bus 6                          */

    IO_ALTERNATE_AF6_SPI3       = GPIO_AF6_SPI3,        /*!< Serial Peripheral Interface Bus 3                          */
    IO_ALTERNATE_AF6_SAI1       = GPIO_AF6_SAI1,        /*!< Serial Audio Interface 1                                   */

    IO_ALTERNATE_AF7_USART1     = GPIO_AF7_USART1,      /*!< Universal Synchronous/Asynchronous Receiver Transmitter 1  */
    IO_ALTERNATE_AF7_USART2     = GPIO_AF7_USART2,      /*!< Universal Synchronous/Asynchronous Receiver Transmitter 2  */
    IO_ALTERNATE_AF7_USART3     = GPIO_AF7_USART3,      /*!< Universal Synchronous/Asynchronous Receiver Transmitter 3  */

    IO_ALTERNATE_AF8_UART4      = GPIO_AF8_UART4,       /*!< Universal Asynchronous Receiver Transmitter 4              */
    IO_ALTERNATE_AF8_UART5      = GPIO_AF8_UART5,       /*!< Universal Asynchronous Receiver Transmitter 5              */
    IO_ALTERNATE_AF8_USART6     = GPIO_AF8_USART6,      /*!< Universal Synchronous/Asynchronous Receiver Transmitter 6  */
    IO_ALTERNATE_AF8_UART7      = GPIO_AF8_UART7,       /*!< Universal Asynchronous Receiver Transmitter 3              */
    IO_ALTERNATE_AF8_UART8      = GPIO_AF8_UART8,       /*!< Universal Asynchronous Receiver Transmitter 3              */

    IO_ALTERNATE_AF9_CAN1       = GPIO_AF9_CAN1,        /*!< Controller Area Network Bus 1                              */
    IO_ALTERNATE_AF9_CAN0       = GPIO_AF9_CAN2,        /*!< Controller Area Network Bus 2                              */
    IO_ALTERNATE_AF9_TIM12      = GPIO_AF9_TIM12,       /*!< TIM12                                                      */
    IO_ALTERNATE_AF9_TIM13      = GPIO_AF9_TIM13,       /*!< TIM13                                                      */
    IO_ALTERNATE_AF9_TIM14      = GPIO_AF9_TIM14,       /*!< TIM14                                                      */

    IO_ALTERNATE_AF10_OTG_FS    = GPIO_AF10_OTG_FS,     /*!< USB On-the-go FS                                           */
    IO_ALTERNATE_AF10_OTG_HS    = GPIO_AF10_OTG_HS,     /*!< USB On-the-go HS                                           */

    IO_ALTERNATE_AF11_ETH       = GPIO_AF11_ETH,        /*!< Ethernet                                                   */

    IO_ALTERNATE_AF12_FMC       = GPIO_AF12_FMC,        /*!< Flexible Memory Controller                                 */
    IO_ALTERNATE_AF12_OTG_HS_FS = GPIO_AF12_OTG_HS_FS,  /*!< USB On-the-go HS/FS                                        */
    IO_ALTERNATE_AF12_SDIO      = GPIO_AF12_SDIO,       /*!< Secure Digital Memory Card                                 */

    IO_ALTERNATE_AF13_DCMI      = GPIO_AF13_DCMI,       /*!< Digital Camera Memory Interface                            */


    IO_ALTERNATE_AF15_EVENTOUT  = GPIO_AF15_EVENTOUT,   /*!< Eventout                                                   */

    IO_ALTERNATE_NO_ALTERNATE   = 0xFF                  /*!< No alternate function                                      */
} IO_PIN_ALTERNATE_e;

/**
 * symbolic names for enabling/disabling pin locking at certain pin
 */
typedef enum {
    IO_PIN_LOCK_DISABLE = 0,    /*!< Disable configuration locking  */
    IO_PIN_LOCK_ENABLE  = 1,    /*!< Enable configuration locking   */
} IO_PIN_LOCK_e;

/**
 * symbolic names for pin state, where reset means low, and set means high
 */
typedef enum {
  IO_PIN_RESET  = GPIO_PIN_RESET,   /*!< Pin is set to low/0    */
  IO_PIN_SET    = GPIO_PIN_SET      /*!< Pin is set to high/1   */
} IO_PIN_STATE_e;

/**
 * symbolic names for status returned by the HAL
 */
typedef enum {
  IO_HAL_STATUS_OK      = HAL_OK,       /*!< HAL ok         */
  IO_HAL_STATUS_ERROR   = HAL_ERROR,    /*!< HAL error      */
  IO_HAL_STATUS_BUSY    = HAL_BUSY,     /*!< HAL busy       */
  IO_HAL_STATUS_TIMEOUT = HAL_TIMEOUT   /*!< HAL timeout    */
} IO_HAL_STATUS_e;

/**
 * struct for the initialization configuration of a pin
 */
typedef struct {
    IO_PORTS_e pin;                     /*!< The pin to be configured defined by its symbolic name                  */
    IO_PIN_MODES_e mode;                /*!< The mode in which the pin will be used defined by its symbolic name    */
    IO_PIN_PULL_e pinpull;              /*!< The speed at which the pin will be driven defined by its symbolic name */
    IO_PIN_SPEEDS_e speed;              /*!< The speed at which the pin will be driven defined by its symbolic name */
    IO_PIN_ALTERNATE_e alternate;       /*!< The alternate function the pin uses defined by its symbolic name       */
    IO_PIN_LOCK_e pinlock;              /*!< Enable or disable pin locking. The macro IO_PIN_LOCKING has to be set  */
    IO_PIN_STATE_e initvalue;           /*!< Set the pin high or low at the pin initialization. Default is low.     */
} IO_PIN_CFG_s;

/**
 * array for the initialization of the pins of the microcontroller
 */
extern const IO_PIN_CFG_s io_cfg[];

/**
 * length of the array for the initialization of the pins of the
 * microcontroller
 */
extern const uint8_t io_cfg_length;

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

#endif /* IO_CFG_H_ */
