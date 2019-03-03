#include "driverHWI2C2.h"

I2C_HandleTypeDef driverHWI2C2;

const I2C2PortStruct driverHWI2C2Ports[NoOfI2C2Ports] = 												// Hold all I2C pin configuration data
{
	{GPIOA,RCC_AHBENR_GPIOAEN,GPIO_PIN_8,GPIO_MODE_INPUT,GPIO_PULLUP,0x00},							    // ISL_INT
	{GPIOA,RCC_AHBENR_GPIOAEN,GPIO_PIN_9,GPIO_MODE_AF_OD,GPIO_PULLUP,GPIO_AF4_I2C2},				// ISL_SCL
	{GPIOA,RCC_AHBENR_GPIOAEN,GPIO_PIN_10,GPIO_MODE_AF_OD,GPIO_PULLUP,GPIO_AF4_I2C2}				// ISL_SDA
};

void driverHWI2C2Init(void) {
	GPIO_InitTypeDef PortInitHolder;
	uint8_t PortPointer;
	
	for(PortPointer = 0; PortPointer < NoOfI2C2Ports; PortPointer++) {
		RCC->AHBENR |= driverHWI2C2Ports[PortPointer].ClkRegister;								// Enable clock de desired port
		PortInitHolder.Mode = driverHWI2C2Ports[PortPointer].Mode;								// Push pull output
		PortInitHolder.Pin = driverHWI2C2Ports[PortPointer].Pin;									// Points to status pin
		PortInitHolder.Pull = driverHWI2C2Ports[PortPointer].Pull;								// No pullup
		PortInitHolder.Speed = GPIO_SPEED_HIGH;																		// GPIO clock speed
		PortInitHolder.Alternate = driverHWI2C2Ports[PortPointer].Alternate;			// Alternate function
		HAL_GPIO_Init(driverHWI2C2Ports[PortPointer].Port,&PortInitHolder);				// Perform the IO init 
	};
	
	__I2C2_CLK_ENABLE();																												// Enable clock source of I2C Master
	
  driverHWI2C2.Instance = I2C2;
  driverHWI2C2.Init.Timing = 0x00705CFF;																			// 200kHz SYSClock 72MHz
  driverHWI2C2.Init.OwnAddress1 = 0;
  driverHWI2C2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  driverHWI2C2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  driverHWI2C2.Init.OwnAddress2 = 0;
  driverHWI2C2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  driverHWI2C2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  driverHWI2C2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&driverHWI2C2) != HAL_OK)
  {
    while(true){
			// Handle error state
		};
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&driverHWI2C2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    while(true){
			// Handle error state
		};
  }
};

bool driverHWI2C2Write(uint16_t DevAddress, bool readWrite, uint8_t *pData, uint16_t Size) {
	uint16_t addresRW = (DevAddress << 1) | readWrite;
	
	HAL_I2C_Master_Transmit(&driverHWI2C2,addresRW,pData,Size,100);
	return false;
};

bool driverHWI2C2Read(uint16_t DevAddress, uint8_t *pData, uint16_t Size) {
	uint16_t addresRW = (DevAddress << 1) | 0x01; // Read bit high
	
	HAL_I2C_Master_Receive(&driverHWI2C2,addresRW,pData,Size,100);
	return false;
};


