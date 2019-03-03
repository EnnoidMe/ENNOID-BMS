#include "stm32f3xx_hal.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"

#define driverHWSPI1DefaultTimeout										100

void driverHWSPI1Init(GPIO_TypeDef* GPIOCSPort, uint16_t GPIO_CSPin);
bool driverHWSPI1Write(uint8_t *writeBuffer, uint8_t noOfBytesToWrite,GPIO_TypeDef* GPIOCSPort, uint16_t GPIO_CSPin);
bool driverHWSPI1WriteRead(uint8_t *writeBuffer, uint8_t noOfBytesToWrite, uint8_t *readBuffer, uint8_t noOfBytesToRead,GPIO_TypeDef* GPIOCSPort, uint16_t GPIO_CSPin);
