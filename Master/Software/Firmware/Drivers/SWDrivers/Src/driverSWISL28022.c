#include "driverSWISL28022.h"

void driverSWISL28022Init(uint8_t i2cAddres, uint8_t i2cBus, driverSWISL28022InitStruct initStruct){
	// Implementing the fancy driverSWISL28022InitStruct is still on my todo list :).
	uint8_t writeData[3] = {0x00,0x77,0xFF};
	
	if(i2cBus == 1){
		driverHWI2C1Init();
		driverHWI2C1Write(i2cAddres,false,writeData,3);
	}else if(i2cBus == 2){
		driverHWI2C2Init();
		driverHWI2C2Write(i2cAddres,false,writeData,3);
	}
};

bool driverSWISL28022GetBusCurrent(uint8_t i2cAddres, uint8_t i2cBus, float *busCurrent, int16_t offset, float scalar) {
	// ToDo make register to current conversion register dependent
	uint8_t writeDataC[1] = {REG_SHUNTVOLTAGE};
	uint8_t readDataC[2];
	static int16_t busCurrentInt;

	if(i2cBus == 1){
		driverHWI2C1Write(i2cAddres,false,writeDataC,1);
		driverHWI2C1Read(i2cAddres,readDataC,2);
	}else if(i2cBus == 2){
		driverHWI2C2Write(i2cAddres,false,writeDataC,1);
		driverHWI2C2Read(i2cAddres,readDataC,2);
	}
	
	busCurrentInt = (readDataC[0] << 9) | (readDataC[1] << 1);
	*busCurrent = scalar*(busCurrentInt+offset);
	
	return false;
};

bool driverSWISL28022GetBusVoltage(uint8_t i2cAddres, uint8_t i2cBus, float *busVoltage, float scalar){
	uint8_t writeDataV[1] = {REG_BUSVOLTAGE};
	uint8_t readDataV[2];
	uint16_t busVoltageInt;
	
	if(i2cBus == 1){
	  driverHWI2C1Write(i2cAddres,false,writeDataV,1);
	  driverHWI2C1Read(i2cAddres,readDataV,2);
	}else if(i2cBus == 2){
	  driverHWI2C2Write(i2cAddres,false,writeDataV,1);
	  driverHWI2C2Read(i2cAddres,readDataV,2);
	}

	busVoltageInt = (readDataV[0] << 6) | (readDataV[1] >> 2);
	*busVoltage = scalar*busVoltageInt;		
	
	return false;
};
