#include "stm32f3xx_hal.h"
#include "generalDefines.h"
#include "driverHWPowerState.h"
#include "libBuffer.h"
#include "libCRC.h"
#include "modEffect.h"
#include "modDelay.h"
#include "moddisplay.h"
#include "modFlash.h"
#include "stdbool.h"


void SystemClock_Config(void);
void Error_Handler(void);
static void MX_GPIO_Init(void);
void jumpToMainApplication(void);

modDisplayDataTypedef emptyData;
uint8_t   bootloaderStateCurrent;
uint8_t   bootloaderStateNext;
uint32_t  bootloaderDelayLastTick;
uint32_t  bootloaderDelayTime;
uint8_t*  newAppAdress;
uint32_t  newAppSize;
uint32_t  newAppCRC;
int32_t   indexPointer;
uint8_t   yAxisOffset;

typedef enum {
	BOOT_INIT = 0,
	BOOT_DELAY,
	BOOT_SIZE_CHECK,
	BOOT_SIZE_ZERO,
	BOOT_SIZE_WRONG,
  BOOT_SIZE_OK,	
	BOOT_CRC_CHECK,
	BOOT_CRC_OK,
	BOOT_CRC_ERROR,
	BOOT_ERASE,
	BOOT_ERASE_ERROR,
	BOOT_ERASE_SUCCES,
	BOOT_COPYAPP,
	BOOT_COPYAPP_ERROR,
	BOOT_COPYAPP_SUCCES,
	BOOT_DONE,
	BOOT_REBOOT,
	BOOT_ERROR
} bootLoaderState;

int main(void) {
  HAL_Init();
	SCB->VTOR = ADDR_FLASH_PAGE_100;	
  SystemClock_Config();
  MX_GPIO_Init();
	
	driverHWPowerStateInit();
	driverHWPowerStateSetOutput(P_STAT_POWER_ENABLE,P_STAT_SET);

	modEffectInit();
	modDisplayInit();
	
	yAxisOffset						 = 0;
	newAppAdress           = (uint8_t*)ADDR_FLASH_PAGE_50;
	indexPointer           = 0;
	newAppSize             = buffer_get_uint32(newAppAdress, &indexPointer);
	newAppCRC              = buffer_get_uint16(newAppAdress, &indexPointer);
	bootloaderStateCurrent = BOOT_INIT;
	bootloaderStateNext    = BOOT_INIT;
	bootloaderDelayTime    = 100;
	
	emptyData.Empty = 0.0f;
	
	bootloaderDelayLastTick = HAL_GetTick();

  while (true) {
		modEffectTask();
		modDisplayTask();
		
		switch(bootloaderStateCurrent) {
			case BOOT_INIT:
				modDisplayShowInfo(DISP_MODE_BOOTLOADER,emptyData);
			  
			  bootloaderDelayTime    = 100;
			  bootloaderStateCurrent = BOOT_DELAY;
			  bootloaderStateNext    = BOOT_SIZE_CHECK;
				break;
			case BOOT_DELAY:
				if(modDelayTick1ms(&bootloaderDelayLastTick,bootloaderDelayTime))
					bootloaderStateCurrent = bootloaderStateNext;
				break;
			case BOOT_SIZE_CHECK:
				yAxisOffset += 8;
				libGraphicsSetCursor(0,yAxisOffset);
				libGraphicsWrite('S');  
				libGraphicsWrite('i');  
				libGraphicsWrite('z');  
				libGraphicsWrite('e'); 
				libGraphicsWrite('.'); 
				libGraphicsWrite('.'); 
				libGraphicsWrite('.'); 			
			
				if(newAppSize == 0)
					bootloaderStateNext = BOOT_SIZE_ZERO;
				else if(newAppSize > NEW_APP_MAX_SIZE)
					bootloaderStateNext = BOOT_SIZE_WRONG;
				else
					bootloaderStateNext = BOOT_SIZE_OK;
			
			  bootloaderDelayTime    = 100;
			  bootloaderStateCurrent = BOOT_DELAY;
				break;
			case BOOT_SIZE_ZERO:
				libGraphicsWrite('E');  
				libGraphicsWrite('r');
				libGraphicsWrite('r');  
				libGraphicsWrite('o');
				libGraphicsWrite('r'); 
				libGraphicsWrite('-'); 
				libGraphicsWrite('z'); 
				libGraphicsWrite('e'); 
				libGraphicsWrite('r'); 
				libGraphicsWrite('o'); 
			
				bootloaderStateNext    = BOOT_ERROR;
			
			  bootloaderDelayTime    = 100;
			  bootloaderStateCurrent = BOOT_DELAY;
				break;
			case BOOT_SIZE_WRONG:
				libGraphicsWrite('E');  
				libGraphicsWrite('r');
				libGraphicsWrite('r');  
				libGraphicsWrite('o');
				libGraphicsWrite('r'); 
				libGraphicsWrite('-'); 
				libGraphicsWrite('w'); 
				libGraphicsWrite('r'); 
				libGraphicsWrite('o'); 
				libGraphicsWrite('n'); 
				libGraphicsWrite('g'); 
			
				bootloaderStateNext    = BOOT_ERROR;
			
				bootloaderDelayTime    = 100;
			  bootloaderStateCurrent = BOOT_DELAY;
				break;
			case BOOT_SIZE_OK:
				libGraphicsWrite('O');  
				libGraphicsWrite('K');
			
				bootloaderStateNext    = BOOT_CRC_CHECK;

				bootloaderDelayTime    = 100;
			  bootloaderStateCurrent = BOOT_DELAY;
				break;
			case BOOT_CRC_CHECK:
				yAxisOffset += 8;
				libGraphicsSetCursor(0,yAxisOffset);
				libGraphicsWrite('C');
				libGraphicsWrite('R');
				libGraphicsWrite('C');
				libGraphicsWrite('.');
				libGraphicsWrite('.');
				libGraphicsWrite('.');
				libGraphicsWrite('.');
			
			  if(newAppCRC != libCRCCalcCRC16(newAppAdress + indexPointer, newAppSize))
					bootloaderStateNext = BOOT_CRC_ERROR;
				else
					bootloaderStateNext = BOOT_CRC_OK;
			
				bootloaderDelayTime    = 100;
			  bootloaderStateCurrent = BOOT_DELAY;
				break;
			case BOOT_CRC_OK:
				libGraphicsWrite('O');  
				libGraphicsWrite('K');

				bootloaderStateNext    = BOOT_ERASE;
			
				bootloaderDelayTime    = 100;
			  bootloaderStateCurrent = BOOT_DELAY;
				break;
			case BOOT_CRC_ERROR:
				libGraphicsWrite('E');  
				libGraphicsWrite('r');
				libGraphicsWrite('r');  
				libGraphicsWrite('o');
				libGraphicsWrite('r');

				bootloaderStateNext    = BOOT_ERROR;

				bootloaderDelayTime    = 100;
			  bootloaderStateCurrent = BOOT_DELAY;
				break;
			case BOOT_ERASE:
				yAxisOffset += 8;
				libGraphicsSetCursor(0,yAxisOffset);
				libGraphicsWrite('E');
				libGraphicsWrite('r');
				libGraphicsWrite('a');
				libGraphicsWrite('s');
				libGraphicsWrite('e');
				libGraphicsWrite('.'); 
				libGraphicsWrite('.');
			
				// Code to erasing main flash
			  if(modFlashEraseMainAppData(newAppSize) == HAL_OK)
					bootloaderStateNext    = BOOT_ERASE_SUCCES;
				else
					bootloaderStateNext    = BOOT_ERASE_ERROR;
			
				bootloaderDelayTime    = 100;
			  bootloaderStateCurrent = BOOT_DELAY;
				break;
			case BOOT_ERASE_ERROR:
				libGraphicsWrite('E');  
				libGraphicsWrite('r');
				libGraphicsWrite('r');  
				libGraphicsWrite('o');
				libGraphicsWrite('r');

				bootloaderStateNext    = BOOT_ERROR;

				bootloaderDelayTime    = 100;
			  bootloaderStateCurrent = BOOT_DELAY;
				break;
			case BOOT_ERASE_SUCCES:
				libGraphicsWrite('O');  
				libGraphicsWrite('K');

				bootloaderStateNext    = BOOT_COPYAPP;

				bootloaderDelayTime    = 100;
			  bootloaderStateCurrent = BOOT_DELAY;
				break;
			case BOOT_COPYAPP:
				yAxisOffset += 8;
				libGraphicsSetCursor(0,yAxisOffset);
				libGraphicsWrite('C');
				libGraphicsWrite('o');
				libGraphicsWrite('p');
				libGraphicsWrite('y');
				libGraphicsWrite('.');
				libGraphicsWrite('.');
				libGraphicsWrite('.');
			
				// Code to copy new app to main flash
				if(modFlashCopyNewAppToMainApp(0, newAppAdress + indexPointer, newAppSize) == HAL_OK)
					bootloaderStateNext    = BOOT_COPYAPP_SUCCES;
				else
					bootloaderStateNext    = BOOT_COPYAPP_ERROR;
			
				bootloaderDelayTime    = 100;
			  bootloaderStateCurrent = BOOT_DELAY;
				break;
			case BOOT_COPYAPP_ERROR:
				libGraphicsWrite('E');  
				libGraphicsWrite('r');
				libGraphicsWrite('r');  
				libGraphicsWrite('o');
				libGraphicsWrite('r');

				bootloaderStateNext    = BOOT_ERROR;

				bootloaderDelayTime    = 100;
			  bootloaderStateCurrent = BOOT_DELAY;
				break;
			case BOOT_COPYAPP_SUCCES:
				libGraphicsWrite('O');  
				libGraphicsWrite('K');

				bootloaderStateNext    = BOOT_DONE;

				bootloaderDelayTime    = 100;
			  bootloaderStateCurrent = BOOT_DELAY;
				break;			
			case BOOT_DONE:
				yAxisOffset += 8;
				libGraphicsSetCursor(0,yAxisOffset);
				libGraphicsWrite('D');
				libGraphicsWrite('o');
				libGraphicsWrite('n');
				libGraphicsWrite('e');
				libGraphicsWrite('!');
			
				bootloaderStateNext    = BOOT_REBOOT;
			
				bootloaderDelayTime    = 2000;
			  bootloaderStateCurrent = BOOT_DELAY;
				break;
      case BOOT_REBOOT:
				jumpToMainApplication();
				break;
			case BOOT_ERROR:
				yAxisOffset += 8;
				libGraphicsSetCursor(0,yAxisOffset);
				libGraphicsWrite('E');
				libGraphicsWrite('R');
				libGraphicsWrite('R');
				libGraphicsWrite('O');
				libGraphicsWrite('R');
				libGraphicsWrite('!');

				// Do nothing but keep showing error.
				bootloaderStateNext    = BOOT_REBOOT;
				bootloaderDelayTime    = 5000;
			  bootloaderStateCurrent = BOOT_DELAY;
				break;
			default:
				break;
		}
	};
}

void jumpToMainApplication(void) {
	NVIC_SystemReset();
}

void SystemClock_Config(void) {

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, StatusLED_Pin|PowerLED_Pin|PowerEnable_Pin|OLED_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : StatusLED_Pin PowerLED_Pin PowerEnable_Pin OLED_RST_Pin */
  GPIO_InitStruct.Pin = StatusLED_Pin|PowerLED_Pin|PowerEnable_Pin|OLED_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : Buzzer_Pin */
  GPIO_InitStruct.Pin = Buzzer_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(Buzzer_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PowerButton_Pin */
  GPIO_InitStruct.Pin = PowerButton_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PowerButton_GPIO_Port, &GPIO_InitStruct);

}

void Error_Handler(void) {
  while(1){};
}

