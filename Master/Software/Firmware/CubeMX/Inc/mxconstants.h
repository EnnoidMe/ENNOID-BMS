/**
  ******************************************************************************
  * File Name          : mxconstants.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
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
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MXCONSTANT_H
#define __MXCONSTANT_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define SAFETY_ENA_Pin GPIO_PIN_13
#define SAFETY_ENA_GPIO_Port GPIOC
#define ChargeDetect_Pin GPIO_PIN_14
#define ChargeDetect_GPIO_Port GPIOC
#define Wireless_GD0_Pin GPIO_PIN_15
#define Wireless_GD0_GPIO_Port GPIOC
#define NTC_SWITCH_Pin GPIO_PIN_0
#define NTC_SWITCH_GPIO_Port GPIOA
#define VoutSense_Pin GPIO_PIN_1
#define VoutSense_GPIO_Port GPIOA
#define BOOTLOADER_TX_Pin GPIO_PIN_2
#define BOOTLOADER_TX_GPIO_Port GPIOA
#define BOOTLOADER_RX_Pin GPIO_PIN_3
#define BOOTLOADER_RX_GPIO_Port GPIOA
#define LTC_CS_Pin GPIO_PIN_4
#define LTC_CS_GPIO_Port GPIOA
#define LTC_SCK_Pin GPIO_PIN_5
#define LTC_SCK_GPIO_Port GPIOA
#define LTC_MISO_Pin GPIO_PIN_6
#define LTC_MISO_GPIO_Port GPIOA
#define LTC_MOSI_Pin GPIO_PIN_7
#define LTC_MOSI_GPIO_Port GPIOA
#define ChargeEnable_Pin GPIO_PIN_0
#define ChargeEnable_GPIO_Port GPIOB
#define StatusLED_Pin GPIO_PIN_1
#define StatusLED_GPIO_Port GPIOB
#define SwitchEnable_Pin GPIO_PIN_2
#define SwitchEnable_GPIO_Port GPIOB
#define DischargeEnable_Pin GPIO_PIN_10
#define DischargeEnable_GPIO_Port GPIOB
#define PreChargeEnable_Pin GPIO_PIN_11
#define PreChargeEnable_GPIO_Port GPIOB
#define SDC_CS_Pin GPIO_PIN_12
#define SDC_CS_GPIO_Port GPIOB
#define SDC_SCK_Pin GPIO_PIN_13
#define SDC_SCK_GPIO_Port GPIOB
#define SDC_MISO_Pin GPIO_PIN_14
#define SDC_MISO_GPIO_Port GPIOB
#define SDC_MOSI_Pin GPIO_PIN_15
#define SDC_MOSI_GPIO_Port GPIOB
#define ISL_INT_Pin GPIO_PIN_8
#define ISL_INT_GPIO_Port GPIOA
#define ISL_SCL_Pin GPIO_PIN_9
#define ISL_SCL_GPIO_Port GPIOA
#define ISL_SDA_Pin GPIO_PIN_10
#define ISL_SDA_GPIO_Port GPIOA
#define Buzzer_Pin GPIO_PIN_15
#define Buzzer_GPIO_Port GPIOA
#define PowerLED_Pin GPIO_PIN_3
#define PowerLED_GPIO_Port GPIOB
#define PowerButton_Pin GPIO_PIN_4
#define PowerButton_GPIO_Port GPIOB
#define PowerEnable_Pin GPIO_PIN_5
#define PowerEnable_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_6
#define OLED_SCL_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_7
#define OLED_SDA_GPIO_Port GPIOB
#define OLED_RST_Pin GPIO_PIN_8
#define OLED_RST_GPIO_Port GPIOB
#define SDC_DETECT_Pin GPIO_PIN_9
#define SDC_DETECT_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MXCONSTANT_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
