#include "stdbool.h"
#include "mainDataTypes.h"
#include "modDelay.h"
#include "modPowerState.h"
#include "modEffect.h"
#include "modPowerElectronics.h"
#include "modDisplay.h"
#include "modConfig.h"
#include "modStateOfCharge.h"
#include "math.h"

#ifndef MODOPERATIONALSTATE_H_
#define MODOPERATIONALSTATE_H_

void modOperationalStateInit(modPowerElectricsPackStateTypedef *packState, modConfigGeneralConfigStructTypedef *generalConfigPointer,modStateOfChargeStructTypeDef *generalStateOfCharge);
void modOperationalStateTask(void);
void modOperationalStateUpdateStates(void);
void modOperationalStateSetAllStates(OperationalStateTypedef newState);
void modOperationalStateSetNewState(OperationalStateTypedef newState);
void modOperationalStateHandleChargerDisconnect(OperationalStateTypedef newState);
void modOperationalStateTerminateOperation(void);
bool modOperationalStateDelayedDisable(bool delayedPowerDownDesired);

#endif
