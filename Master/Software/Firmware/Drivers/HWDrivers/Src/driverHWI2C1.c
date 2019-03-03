#include "driverHWI2C1.h"

I2C_HandleTypeDef driverHWI2C1;

const I2C1PortStruct driverHWI2C1Ports[NoOfI2C1Ports] = 											// Hold all I2C pin configuration data
{
	{GPIOB,RCC_AHBENR_GPIOBEN,GPIO_PIN_6,GPIO_MODE_AF_OD,GPIO_PULLUP,GPIO_AF4_I2C1},			// OLED_SCL
	{GPIOB,RCC_AHBENR_GPIOBEN,GPIO_PIN_7,GPIO_MODE_AF_OD,GPIO_PULLUP,GPIO_AF4_I2C1}				// OLED_SDA
};

void driverHWI2C1Init(void) {
	GPIO_InitTypeDef PortInitHolder;
	uint8_t PortPointer;
	
	for(PortPointer = 0; PortPointer < NoOfI2C1Ports; PortPointer++) {
		RCC->AHBENR |= driverHWI2C1Ports[PortPointer].ClkRegister;								// Enable clock de desired port
		PortInitHolder.Mode = driverHWI2C1Ports[PortPointer].Mode;								// Push pull output
		PortInitHolder.Pin = driverHWI2C1Ports[PortPointer].Pin;									// Points to status pin
		PortInitHolder.Pull = driverHWI2C1Ports[PortPointer].Pull;								// No pullup
		PortInitHolder.Speed = GPIO_SPEED_HIGH;																		// GPIO clock speed
		PortInitHolder.Alternate = driverHWI2C1Ports[PortPointer].Alternate;			// Alternate function
		HAL_GPIO_Init(driverHWI2C1Ports[PortPointer].Port,&PortInitHolder);				// Perform the IO init 
	};
	
	__I2C1_CLK_ENABLE();																												// Enable clock source of I2C Master
	
  driverHWI2C1.Instance = I2C1;
  driverHWI2C1.Init.Timing = 0x10808DD3;																			// 200kHz: 0x00705CFF, 110khz: 10808DD3 SYSClock 72MHz
  driverHWI2C1.Init.OwnAddress1 = 0;
  driverHWI2C1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  driverHWI2C1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  driverHWI2C1.Init.OwnAddress2 = 0;
  driverHWI2C1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  driverHWI2C1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  driverHWI2C1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  if (HAL_I2C_Init(&driverHWI2C1) != HAL_OK)
  {
    while(true){
			// Handle error state
		};
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&driverHWI2C1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    while(true){
			// Handle error state
		};
  }
};

bool driverHWI2C1Write(uint16_t DevAddress, bool readWrite, uint8_t *pData, uint16_t Size) {
	uint16_t addresRW = (DevAddress << 1) | readWrite;
	
	return HAL_I2C_Master_Transmit(&driverHWI2C1,addresRW,pData,Size,5);
};

bool driverHWI2C1Read(uint16_t DevAddress, uint8_t *pData, uint16_t Size) {
	uint16_t addresRW = (DevAddress << 1) | 0x01; // Read bit high
	
	return HAL_I2C_Master_Receive(&driverHWI2C1,addresRW,pData,Size,5);
};


