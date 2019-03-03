#ifndef __DRIVERSWEMS2305_H
#define __DRIVERSWEMS2305_H

#include "stdbool.h"
#include "modDelay.h"
#include "driverHWI2C1.h"

#define I2CADDRFANDriver     0x2E

typedef struct {
  uint16_t FANSpeedRPM[4];
	uint8_t  FANEnabled;
	uint8_t	 FANError;
	uint8_t  FANOK;
} driverSWEMC2305FanStatusTypeDef;

void driverSWEMC2305Init(uint8_t i2cAddres, uint8_t minimalDuty);
void driverSWEMC2305SetFANDutySingle(uint8_t i2cAddres, uint8_t FANDuty, uint8_t FANPointer);
void driverSWEMC2305SetFANDutyAll(uint8_t i2cAddres, uint8_t FANDuty);
uint16_t driverEMC2305GetFANRPM(uint8_t i2cAddres, uint8_t FANPointer);
driverSWEMC2305FanStatusTypeDef driverEMC2305GetFANStatus(uint8_t i2cAddres);

#endif
