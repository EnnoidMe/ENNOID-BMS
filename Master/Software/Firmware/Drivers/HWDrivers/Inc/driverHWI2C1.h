#ifndef __DRIVERHWI2C1_H
#define __DRIVERHWI2C1_H

#include "stm32f3xx_hal.h"
#include "stdbool.h"

#define NoOfI2C1Ports				2

typedef struct {
	GPIO_TypeDef* Port;
	uint32_t ClkRegister;
	uint32_t Pin;
	uint32_t Mode;
	uint32_t Pull;
	uint32_t Alternate;
} I2C1PortStruct;

void driverHWI2C1Init(void);
bool driverHWI2C1Write(uint16_t DevAddress, bool readWrite, uint8_t *pData, uint16_t Size);
bool driverHWI2C1Read(uint16_t DevAddress, uint8_t *pData, uint16_t Size);

#endif
