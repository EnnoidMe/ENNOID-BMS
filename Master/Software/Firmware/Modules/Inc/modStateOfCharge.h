#ifndef __MODSTATEOFCHARGE_H
#define __MODSTATEOFCHARGE_H

#include "stdint.h"
#include "stdbool.h"
#include "modPowerElectronics.h"
#include "modConfig.h"

typedef struct {
	float generalStateOfCharge;
	float generalStateOfHealth;
	float remainingCapacityAh;
	float remainingCapacityWh;
} modStateOfChargeStructTypeDef;

typedef enum {
	EVENT_FULL = 0,
	EVENT_EMPTY,
} modStateOfChargeVoltageEventTypeDef;

modStateOfChargeStructTypeDef* modStateOfChargeInit(modPowerElectricsPackStateTypedef *packState, modConfigGeneralConfigStructTypedef *generalConfigPointer);
void modStateOfChargeProcess(void);
bool modStateOfChargeStoreAndLoadDefaultStateOfCharge(void);
bool modStateOfChargeStoreStateOfCharge(void);
bool modStateOfChargeLoadStateOfCharge(void);
bool modStateOfChargePowerDownSave(void);
void modStateOfChargeVoltageEvent(modStateOfChargeVoltageEventTypeDef eventType);

#endif
