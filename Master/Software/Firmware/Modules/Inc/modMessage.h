#ifndef __MODMESSAGE_H
#define __MODMESSAGE_H

#include "stdint.h"
#include "stdbool.h"
#include "driverSWUART2.h"
#include "modDelay.h"
#include "modConfig.h"
#include "modPowerElectronics.h"

typedef enum {
	MESSAGE_DEBUG = 0,
	MESSAGE_ERROR,
	MESSAGE_NORMAL,
} modMessageMessageTypeDef;

void modMessageInit(modPowerElectricsPackStateTypedef *packState, modConfigGeneralConfigStructTypedef *generalConfigPointer);
void modMessageTask(void);
void modMessageQueMessage(modMessageMessageTypeDef messageType, const char* format, ...);

#endif
