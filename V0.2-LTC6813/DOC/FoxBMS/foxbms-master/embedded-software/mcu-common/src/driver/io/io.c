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
 * @file    io.c
 * @author  foxBMS Team
 * @date    26.08.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  IO
 *
 * @brief   Driver for the I/O ports (pins).
 *
 */

/*================== Includes =============================================*/
#include "io.h"

/*================== Macros and Definitions ===============================*/
#define IO_GET_GPIOx(_N) ((GPIO_TypeDef *)(GPIOA_BASE + (GPIOB_BASE-GPIOA_BASE)*(_N)))

#define GPIO_MODE             ((uint32_t)0x00000003U)
#define EXTI_MODE             ((uint32_t)0x10000000U)
#define GPIO_MODE_IT          ((uint32_t)0x00010000U)
#define GPIO_MODE_EVT         ((uint32_t)0x00020000U)
#define RISING_EDGE           ((uint32_t)0x00100000U)
#define FALLING_EDGE          ((uint32_t)0x00200000U)
#define GPIO_OUTPUT_TYPE      ((uint32_t)0x00000010U)

#define GPIO_NUMBER           ((uint32_t)16U)

/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/
static STD_RETURN_TYPE_e IO_ClkInit(void);
#ifdef IO_PIN_LOCKING
static IO_HAL_STATUS_e IO_LockPin(IO_PORTS_e pin);
#endif
static STD_RETURN_TYPE_e GPIO_Check(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init);

/*================== Function Implementations =============================*/

/*================== Public functions =====================================*/

STD_RETURN_TYPE_e IO_Init(const IO_PIN_CFG_s *io_cfg) {

    if (NULL_PTR == io_cfg) {
        return E_NOT_OK; /* configuration error */
    }

    STD_RETURN_TYPE_e retVal = E_NOT_OK;
    STD_RETURN_TYPE_e clk_ok = E_NOT_OK;
    STD_RETURN_TYPE_e config_ok = E_OK;
    GPIO_InitTypeDef GPIO_InitStructure;

    clk_ok = IO_ClkInit();

    if (E_OK == clk_ok) {
        for (uint8_t i = 0; i < io_cfg_length; i++)
        {
            GPIO_InitStructure.Pin = (uint16_t) (1<< ((io_cfg[i].pin) % IO_NR_OF_PINS_PER_PORT));
            GPIO_InitStructure.Mode = io_cfg[i].mode;
            GPIO_InitStructure.Pull = io_cfg[i].pinpull;
            GPIO_InitStructure.Speed = io_cfg[i].speed;

            if (IO_ALTERNATE_NO_ALTERNATE != io_cfg[i].alternate) {
                GPIO_InitStructure.Alternate = io_cfg[i].alternate;
            }
            HAL_GPIO_Init(IO_GET_GPIOx(io_cfg[i].pin/IO_NR_OF_PINS_PER_PORT), &GPIO_InitStructure);

            if (IO_PIN_SET  ==  io_cfg[i].initvalue) {
                if((IO_MODE_OUTPUT_PP  ==  io_cfg[i].mode)||(IO_MODE_OUTPUT_OD  ==  io_cfg[i].mode)||
                        (IO_MODE_AF_PP  ==  io_cfg[i].mode)||(IO_MODE_AF_OD  ==  io_cfg[i].mode)
                ) {
                    IO_WritePin(io_cfg[i].pin, IO_PIN_SET);
                }
            }
            else {
                IO_WritePin(io_cfg[i].pin, IO_PIN_RESET);
            };

            config_ok = (GPIO_Check(IO_GET_GPIOx(io_cfg[i].pin/IO_NR_OF_PINS_PER_PORT), &GPIO_InitStructure) == E_OK ? config_ok : E_NOT_OK);
        }
    }


#ifdef IO_PIN_LOCKING
    STD_RETURN_TYPE_e pinLocking_ok = E_NOT_OK;
    if (E_OK == config_ok) {
        IO_HAL_STATUS_e pinLockingState = IO_HAL_STATUS_ERROR;
        uint8_t requestedLocks = 0;
        uint8_t successfulLocks = 0;
        for (uint8_t i = 0; i < io_cfg_length; i++) {
            if (IO_PIN_LOCK_ENABLE  ==  io_cfg[i].pinlock) {
                requestedLocks++;
                pinLockingState = IO_LockPin(io_cfg[i].pin);
            }
            if (IO_HAL_STATUS_OK == pinLockingState) {
                successfulLocks++;
            }
            else {
                break;
            }
            pinLockingState = IO_HAL_STATUS_ERROR;
        }
        if (requestedLocks == successfulLocks) {
            pinLocking_ok = E_OK;
        }
    }
#endif

    if ((E_OK == clk_ok) &&
        (E_OK == config_ok)
#ifdef IO_PIN_LOCKING
        && (E_OK == pinLocking_ok)
#endif
         ) {
        retVal = E_OK;
    }
    return retVal;
}

IO_PIN_STATE_e IO_ReadPin(IO_PORTS_e pin) {
    IO_PIN_STATE_e currentPinState;
    uint16_t getPin = (uint16_t) (1<<(pin%IO_NR_OF_PINS_PER_PORT));
    currentPinState = HAL_GPIO_ReadPin(IO_GET_GPIOx(pin/IO_NR_OF_PINS_PER_PORT), getPin);
    return currentPinState;
}

void IO_WritePin(IO_PORTS_e pin, IO_PIN_STATE_e requestedPinState) {
    uint16_t setPin = (uint16_t) (1<<(pin%IO_NR_OF_PINS_PER_PORT));
    HAL_GPIO_WritePin(IO_GET_GPIOx(pin/IO_NR_OF_PINS_PER_PORT), setPin, requestedPinState);
}

void IO_TogglePin(IO_PORTS_e pin) {
    uint16_t setPin = (uint16_t) (1<<(pin%IO_NR_OF_PINS_PER_PORT));
    HAL_GPIO_TogglePin(IO_GET_GPIOx(pin/IO_NR_OF_PINS_PER_PORT), setPin);
}

void IO_EXTI_IRQHandler(IO_PORTS_e pin) {
    HAL_GPIO_EXTI_IRQHandler((uint16_t) (1<<(pin%IO_NR_OF_PINS_PER_PORT)));
}

void IO_EXTI_Callback(IO_PORTS_e pin) {
    HAL_GPIO_EXTI_Callback((uint16_t) (1<<(pin%IO_NR_OF_PINS_PER_PORT)));
}

/*================== Static functions =====================================*/
/**
 * @brief   IO_ClkInit() activates the port clocks of the used
 *          package.
 *
 *  IO_ClkInit is automatically called in IO_Init(). The clocks of each port
 *  available at the used package is enabled.
 *
 * @return  retVal (type: STD_RETURN_TYPE_e) returns E_OK when finished
 */
static STD_RETURN_TYPE_e IO_ClkInit(void) {
    STD_RETURN_TYPE_e retVal = E_NOT_OK;

#if defined(IO_PACKAGE_LQFP100) || defined(IO_PACKAGE_LQFP144) || defined(IO_PACKAGE_LQFP176)
    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    __GPIOD_CLK_ENABLE();
    __GPIOE_CLK_ENABLE();
#endif /* IO_PACKAGE_LQFP100 */

#if defined(IO_PACKAGE_LQFP144) || defined(IO_PACKAGE_LQFP176)
    __GPIOF_CLK_ENABLE();
    __GPIOG_CLK_ENABLE();
    __GPIOH_CLK_ENABLE();
#endif /* IO_PACKAGE_LQFP144 */

#if defined(IO_PACKAGE_LQFP176)
    __GPIOI_CLK_ENABLE();
#endif /* IO_PACKAGE_LQFP176 */
    retVal = E_OK;
    return retVal;
}

#ifdef IO_PIN_LOCKING
/**
 * @brief   Locks the configuration of a pin.
 *
 * IO_LockPin(IO_PORTS_e) gets called in IO_Init(IO_PIN_CFG_s)
 * when the IO_PIN_LOCKING macro is defined. If IO_PIN_LOCK_ENABLE is set
 * in IO_PIN_CFG_s io_cfg[] the following registers are locked:
 *  - GPIOx_MODER,
 *  - GPIOx_OTYPER,
 *  - GPIOx_OSPEEDR,
 *  - GPIOx_PUPDR,
 *  - GPIOx_AFRL and
 *  - GPIOx_AFRH
 *
 * @param   pin (type: IO_PORTS_e)
 *
 * @return  currentPinStatus (type: IO_HAL_STATUS_e) returns
 *          IO_HAL_STATUS_OK when pin locking of the requested pin
 *          was successful.
 */
static IO_HAL_STATUS_e IO_LockPin(IO_PORTS_e pin) {
    IO_HAL_STATUS_e currentPinStatus;
    uint16_t setPin = (uint16_t) (1<<(pin%IO_NR_OF_PINS_PER_PORT));
    currentPinStatus = HAL_GPIO_LockPin(IO_GET_GPIOx(pin/IO_NR_OF_PINS_PER_PORT), setPin);
    return currentPinStatus;
}
#endif

/**
  * @brief  Checks the GPIOx peripheral according to the specified parameters in the GPIO_Init.
  *
  * <hr>
  * @attention  This function is based on the function 'HAL_GPIO_Init' from stm32f4xx_hal_gpio.c.
  *             The license of stm32f4xx_hal_gpio.c is as follows:
  *
  ******************************************************************************
  *
  * @attention <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  *
  * <hr>
  *
  * @param  GPIOx: where x can be (A..K) to select the GPIO peripheral for STM32F429X device or
  *                      x can be (A..I) to select the GPIO peripheral for STM32F40XX and STM32F427X devices.
  * @param  GPIO_Init: pointer to a GPIO_InitTypeDef structure that contains
  *         the configuration information for the specified GPIO peripheral.
  * @retval E_OK on success, E_NOT_OK on failure
  */
STD_RETURN_TYPE_e GPIO_Check(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{

  uint32_t position;
  uint32_t ioposition = 0x00U;
  uint32_t iocurrent = 0x00U;
  uint32_t temp = 0x00U;

  /* Check the parameters */
  assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Init->Pin));
  assert_param(IS_GPIO_MODE(GPIO_Init->Mode));
  assert_param(IS_GPIO_PULL(GPIO_Init->Pull));

  /* Check the port pins */
  for(position = 0U; position < GPIO_NUMBER; position++)
  {
    /* Get the IO position */
    ioposition = ((uint32_t)0x01U) << position;
    /* Get the current IO position */
    iocurrent = (uint32_t)(GPIO_Init->Pin) & ioposition;

    if(iocurrent == ioposition)
    {
      /*--------------------- GPIO Mode Checking ------------------------*/
      /* In case of Alternate function mode selection */
      if((GPIO_Init->Mode == GPIO_MODE_AF_PP) || (GPIO_Init->Mode == GPIO_MODE_AF_OD))
      {
        /* Check Alternate function mapped with the current IO */
        temp = GPIOx->AFR[position >> 3U];
        temp &= ~((uint32_t)0xFU << ((uint32_t)(position & (uint32_t)0x07U) * 4U)) ;
        temp |= ((uint32_t)(GPIO_Init->Alternate) << (((uint32_t)position & (uint32_t)0x07U) * 4U));
        if(GPIOx->AFR[position >> 3U] != temp) return E_NOT_OK;
      }

      /* Check IO Direction mode (Input, Output, Alternate or Analog) */
      temp = GPIOx->MODER;
      temp &= ~(GPIO_MODER_MODER0 << (position * 2U));
      temp |= ((GPIO_Init->Mode & GPIO_MODE) << (position * 2U));
      if(GPIOx->MODER != temp) return E_NOT_OK;

      /* In case of Output or Alternate function mode selection */
      if((GPIO_Init->Mode == GPIO_MODE_OUTPUT_PP) || (GPIO_Init->Mode == GPIO_MODE_AF_PP) ||
         (GPIO_Init->Mode == GPIO_MODE_OUTPUT_OD) || (GPIO_Init->Mode == GPIO_MODE_AF_OD))
      {
        /* Check the IO Speed */
        temp = GPIOx->OSPEEDR;
        temp &= ~(GPIO_OSPEEDER_OSPEEDR0 << (position * 2U));
        temp |= (GPIO_Init->Speed << (position * 2U));
        if(GPIOx->OSPEEDR != temp) return E_NOT_OK;

        /* Check the IO Output Type */
        temp = GPIOx->OTYPER;
        temp &= ~(GPIO_OTYPER_OT_0 << position) ;
        temp |= (((GPIO_Init->Mode & GPIO_OUTPUT_TYPE) >> 4U) << position);
        if(GPIOx->OTYPER != temp) return E_NOT_OK;
      }

      /* Activate the Pull-up or Pull down resistor for the current IO */
      temp = GPIOx->PUPDR;
      temp &= ~(GPIO_PUPDR_PUPDR0 << (position * 2U));
      temp |= ((GPIO_Init->Pull) << (position * 2U));
      if(GPIOx->PUPDR != temp) return E_NOT_OK;

      /*--------------------- EXTI Mode Configuration ------------------------*/
      /* Check the External Interrupt or event for the current IO */
      if((GPIO_Init->Mode & EXTI_MODE) == EXTI_MODE)
      {
        temp = SYSCFG->EXTICR[position >> 2U];
        temp &= ~(((uint32_t)0x0FU) << (4U * (position & 0x03U)));
        temp |= ((uint32_t)(GPIO_GET_INDEX(GPIOx)) << (4U * (position & 0x03U)));
        if(SYSCFG->EXTICR[position >> 2U] != temp) return E_NOT_OK;

        /* Check EXTI line configuration */
        temp = EXTI->IMR;
        temp &= ~((uint32_t)iocurrent);
        if((GPIO_Init->Mode & GPIO_MODE_IT) == GPIO_MODE_IT)
        {
          temp |= iocurrent;
        }
        if(EXTI->IMR != temp) return E_NOT_OK;

        temp = EXTI->EMR;
        temp &= ~((uint32_t)iocurrent);
        if((GPIO_Init->Mode & GPIO_MODE_EVT) == GPIO_MODE_EVT)
        {
          temp |= iocurrent;
        }
        if(EXTI->EMR != temp) return E_NOT_OK;

        /* Check Rising Falling edge configuration */
        temp = EXTI->RTSR;
        temp &= ~((uint32_t)iocurrent);
        if((GPIO_Init->Mode & RISING_EDGE) == RISING_EDGE)
        {
          temp |= iocurrent;
        }
        if(EXTI->RTSR != temp) return E_NOT_OK;

        temp = EXTI->FTSR;
        temp &= ~((uint32_t)iocurrent);
        if((GPIO_Init->Mode & FALLING_EDGE) == FALLING_EDGE)
        {
          temp |= iocurrent;
        }
        if(EXTI->FTSR != temp) return E_NOT_OK;
      }
    }
  }
  return E_OK;
}
