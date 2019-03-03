#include "driverHWSPI1.h"

SPI_HandleTypeDef driverHWSPI1Handle;

void driverHWSPI1Init(GPIO_TypeDef* GPIOCSPort, uint16_t GPIO_CSPin) {
  driverHWSPI1Handle.Instance = SPI1;
  driverHWSPI1Handle.Init.Mode = SPI_MODE_MASTER;
  driverHWSPI1Handle.Init.Direction = SPI_DIRECTION_2LINES;
  driverHWSPI1Handle.Init.DataSize = SPI_DATASIZE_8BIT;
  driverHWSPI1Handle.Init.CLKPolarity = SPI_POLARITY_HIGH;
  driverHWSPI1Handle.Init.CLKPhase = SPI_PHASE_2EDGE;
  driverHWSPI1Handle.Init.NSS = SPI_NSS_HARD_OUTPUT;
  driverHWSPI1Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  driverHWSPI1Handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
  driverHWSPI1Handle.Init.TIMode = SPI_TIMODE_DISABLE;
  driverHWSPI1Handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  driverHWSPI1Handle.Init.CRCPolynomial = 7;
  driverHWSPI1Handle.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  driverHWSPI1Handle.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&driverHWSPI1Handle) != HAL_OK)
  {
    while(true);
  }
	
	HAL_GPIO_WritePin(GPIOCSPort,GPIO_CSPin,GPIO_PIN_SET);
};

bool driverHWSPI1Write(uint8_t *writeBuffer, uint8_t noOfBytesToWrite, GPIO_TypeDef* GPIOCSPort, uint16_t GPIO_CSPin) {
	uint8_t *readBuffer;																																					// Make fake buffer holder
	HAL_StatusTypeDef halReturnStatus;																														// Make holder for HAL state
	readBuffer = malloc(noOfBytesToWrite);																												// Make fake buffer for
	
	HAL_GPIO_WritePin(GPIOCSPort,GPIO_CSPin,GPIO_PIN_RESET);																// Make CS low
	halReturnStatus = HAL_SPI_TransmitReceive(&driverHWSPI1Handle,writeBuffer,readBuffer,noOfBytesToWrite,driverHWSPI1DefaultTimeout);	// Write desired data to slave and store the received data in readBuffer
	while( driverHWSPI1Handle.State == HAL_SPI_STATE_BUSY );  																		// Wait until transmission is complete
	HAL_GPIO_WritePin(GPIOCSPort,GPIO_CSPin,GPIO_PIN_SET);																	// Make CS High
	
	free(readBuffer);																																							// Dump de fake buffer
	
	return (halReturnStatus == HAL_OK);																														// Return true if all went OK
};

bool driverHWSPI1WriteRead(uint8_t *writeBuffer, uint8_t noOfBytesToWrite, uint8_t *readBuffer, uint8_t noOfBytesToRead, GPIO_TypeDef* GPIOCSPort, uint16_t GPIO_CSPin) {
	uint8_t *writeArray, *readArray;
	HAL_StatusTypeDef halReturnStatus;																														// Make holder for HAL state
	
	writeArray = malloc(sizeof(uint8_t)*(noOfBytesToWrite+noOfBytesToRead));
	readArray = malloc(sizeof(uint8_t)*(noOfBytesToWrite+noOfBytesToRead));	
	
	memset(writeArray,0xFF,noOfBytesToWrite+noOfBytesToRead);
	memcpy(writeArray,writeBuffer,noOfBytesToWrite);
	
	HAL_GPIO_WritePin(GPIOCSPort,GPIO_CSPin,GPIO_PIN_RESET);
	halReturnStatus = HAL_SPI_TransmitReceive(&driverHWSPI1Handle,writeArray,readArray,noOfBytesToWrite+noOfBytesToRead,driverHWSPI1DefaultTimeout);
	while( driverHWSPI1Handle.State == HAL_SPI_STATE_BUSY );  // wait xmission complete
	HAL_GPIO_WritePin(GPIOCSPort,GPIO_CSPin,GPIO_PIN_SET);
	
	memcpy(readBuffer,readArray+noOfBytesToWrite,noOfBytesToRead);
		
	free(writeArray);
	free(readArray);
	
	return (halReturnStatus == HAL_OK);																														// Return true if all went OK
};




