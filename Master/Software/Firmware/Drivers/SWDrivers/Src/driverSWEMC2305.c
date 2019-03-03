#include "driverSWEMC2305.h"

uint8_t driverSWEMC2305MinimalDutyCycle;
uint8_t driverSWEMC2305DutyCyle[4];

void driverSWEMC2305Init(uint8_t i2cAddres, uint8_t minimalDuty) {
	uint8_t configBytes[] = {0x2A,0x00,0x1F,0x0F,0x3F};																			// Init the FAN Driver
	driverHWI2C1Write(i2cAddres,false,configBytes,sizeof(configBytes));
	
	driverSWEMC2305MinimalDutyCycle = minimalDuty;
}

void driverSWEMC2305SetFANDutySingle(uint8_t i2cAddres, uint8_t FANDuty, uint8_t FANPointer){
	if((FANDuty < driverSWEMC2305MinimalDutyCycle) && FANDuty)
		FANDuty = driverSWEMC2305MinimalDutyCycle;
	
	if(FANPointer < 4){
		driverSWEMC2305DutyCyle[FANPointer] = FANDuty;
		
		uint8_t fanConfigBytes[] = {0x00,FANDuty,0x01};
		fanConfigBytes[0] = (FANPointer << 4) + 0x30;
		driverHWI2C1Write(i2cAddres,false,fanConfigBytes,sizeof(fanConfigBytes));
	}
}

void driverSWEMC2305SetFANDutyAll(uint8_t i2cAddres, uint8_t FANDuty){
	uint8_t FANPointer;
	
	for(FANPointer = 0; FANPointer < 4; FANPointer++){
		driverSWEMC2305SetFANDutySingle(i2cAddres, FANDuty,FANPointer);
	}
}

uint16_t driverEMC2305GetFANRPM(uint8_t i2cAddres, uint8_t FANPointer) {
	uint16_t returnRPM;
	
	uint8_t writeDataRegister[1] = {(FANPointer << 4) + 0x3E};												// FAN0 Tach reading register
	uint8_t tachoValue[2];

	driverHWI2C1Write(i2cAddres,false,writeDataRegister,1);
	driverHWI2C1Read(i2cAddres,tachoValue,2);
	
	returnRPM = (3932160*2)/((tachoValue[0] << 5) | tachoValue[1] >> 3);
	
	if(returnRPM < 1000)																															// Results below 1000 are invalid as per datasheet
		returnRPM = 0;
	
	return returnRPM;
}

driverSWEMC2305FanStatusTypeDef driverEMC2305GetFANStatus(uint8_t i2cAddres) {
	driverSWEMC2305FanStatusTypeDef FANStatus;
  uint8_t FANPointer;
	bool FANEnabled = false;
	bool FANError = false;
	
	for(FANPointer = 0; FANPointer < 4; FANPointer++){
		FANStatus.FANSpeedRPM[FANPointer] = driverEMC2305GetFANRPM(i2cAddres,FANPointer);
		
		if(driverSWEMC2305DutyCyle[FANPointer])
			FANEnabled |= true;
		
		if((FANStatus.FANSpeedRPM[FANPointer] == 0) && driverSWEMC2305DutyCyle[FANPointer])
			FANError |= true;			
	}

	FANStatus.FANEnabled = FANEnabled;
	FANStatus.FANError = FANError;
	
	if(FANEnabled && FANError)
	  FANStatus.FANOK = 0;
	else
		FANStatus.FANOK = 1;
	
	return FANStatus;
}
