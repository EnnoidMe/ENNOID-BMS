#include "driverSWCC1101.h"

uint32_t registerReadInterval;

volatile uint8_t registerValues[3] = {0x00,0x00,0x00};

void driverSWCC1101Init(void) {
	driverHWSPI1Init(GPIOA,GPIO_PIN_8);
	
	registerReadInterval = HAL_GetTick();
}

void driverSWCC1101Task(void) {
	if(modDelayTick1ms(&registerReadInterval,100)) {
		registerValues[0] = driverSWCC1101ReadSingleRegister(CC1101_PARTNUM,CC1101_STATUS_REGISTER);
		registerValues[1] = driverSWCC1101ReadSingleRegister(CC1101_VERSION,CC1101_STATUS_REGISTER);
		registerValues[2] = driverSWCC1101ReadSingleRegister(CC1101_MARCSTATE,CC1101_STATUS_REGISTER);
	};
}

// Coupling of drivers
void driverSWCC1101Write(uint8_t *writeBytes, uint8_t writeLength) {
	driverHWSPI1Write(writeBytes,writeLength,GPIOA,GPIO_PIN_8);
};

// Coupling of drivers
void driverSWCC1101WriteRead(uint8_t *writeBytes, uint8_t writeLength, uint8_t *readBytes, uint8_t readLength) {
	driverHWSPI1WriteRead(writeBytes,writeLength,readBytes,readLength,GPIOA,GPIO_PIN_8);
};

uint8_t driverSWCC1101ReadSingleRegister(uint8_t regAddr, uint8_t regType) {
  uint8_t addr[1];
	uint8_t val;
	uint8_t readData[1];
  addr[0] = regAddr | regType;
	
	driverSWCC1101WriteRead(addr, 1, readData,1);
	
	return val;
}




