#include "driverSWPCAL6416.h"

void driverSWPCAL6416Init(uint8_t DDR0, uint8_t DDR1,uint8_t PUER0,uint8_t PUER1,uint8_t PUDR0,uint8_t PUDR1){
	driverHWI2C1Init();																																										// Init the I2C1 Bus
	
	// Init the CHIP
	uint8_t configBytes[] = {0x02,0x00,0x00};																															// Reset all outputs to 0
	driverHWI2C1Write(PCAL6461_ADDRES,false,configBytes,sizeof(configBytes));
	
	configBytes[0] = 0x06; 																																								// Config port0 - data direction register.
	configBytes[1] = DDR0; 																																								// 1 = input, 0 = Ouput
	configBytes[2] = DDR1;
	driverHWI2C1Write(PCAL6461_ADDRES,false,configBytes,sizeof(configBytes));
	
	configBytes[0] = 0x46; 																																								// Pullup/down enable register port0.
	configBytes[1] = PUER0; 																																							// 1 = enable, 0 = disable
	configBytes[2] = PUER1;
	driverHWI2C1Write(PCAL6461_ADDRES,false,configBytes,sizeof(configBytes));
	
	configBytes[0] = 0x48; 																																								// Pullup/down direction register port0.
	configBytes[1] = PUDR0; 																																							// 1 = up, 0 = down
	configBytes[2] = PUDR1;
	driverHWI2C1Write(PCAL6461_ADDRES,false,configBytes,sizeof(configBytes));	
};

void driverSWPCAL6416SetOutput(uint8_t port, uint8_t pin, bool newState, bool writeToChip){
	static uint8_t dataBytes[3] = {0x02,0x00,0x00};																												// Write to output register with init values 0/low
	
	if((port > 1) || (pin > 7))
		return;
	
	if(newState)
		dataBytes[port+1] |= (1<<pin);
	else
		dataBytes[port+1] &= ~(1<<pin);
	
	if(writeToChip) {
		dataBytes[0] = 0x02;																																								// Make sure writing is done to the Output port register
		driverHWI2C1Write(PCAL6461_ADDRES,false,dataBytes,sizeof(dataBytes));
	}
};

bool driverSWPCAL6416GetInput(uint8_t port, uint8_t pin, bool readFromChip){
	return false;
};
