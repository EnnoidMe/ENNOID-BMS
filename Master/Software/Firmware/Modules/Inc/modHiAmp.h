#ifndef __MODHIAMP_H
#define __MODHIAMP_H

#include "stdbool.h"
#include "modDelay.h"
#include "modPowerElectronics.h"
#include "modConfig.h"
#include "driverHWI2C1.h"
#include "driverSWISL28022.h"
#include "driverSWDCDC.h"
#include "driverSWPCAL6416.h"
#include "driverSWADC128D818.h"
#include "driverSWEMC2305.h"
#include "driverSWSHT21.h"

#define I2CADDRISLMain       0x44
#define I2CADDRISLAux        0x45
#define I2CADDRSHT           0x40
#define I2CADDRIOExt         0x20
#define I2CADDRADC           0x1D
#define I2CADDRFANDriver     0x2E

#define PRECHARGE_PERCENTAGE 		0.75f

typedef enum {
	RELAY_CONTROLLER_INIT = 0,
	RELAY_CONTROLLER_OFF,
	RELAY_CONTROLLER_PRECHARGING,
  RELAY_CONTROLLER_PRECHARGED,
	RELAY_CONTROLLER_TIMOUT,
	RELAY_CONTROLLER_ENABLED
	
} relayControllerStateTypeDef;

void  modHiAmpInit(modPowerElectricsPackStateTypedef* packStateHandle, modConfigGeneralConfigStructTypedef *generalConfigPointer);
void  modHiAmpTask(void);
bool  modHiAmpShieldPresentCheck(void);
uint8_t modHiAmpShieldScanI2CDevices(void);
void  modHiAmpShieldResetVariables(void);
void  modHiAmpShieldMainShuntMonitorInit(void);
float modHiAmpShieldShuntMonitorGetVoltage(void);
float modHiAmpShieldShuntMonitorGetCurrent(void);
void  modHiAmpShieldSetFANSpeedAll(uint8_t newFANSpeed);
void  modHiAmpShieldRelayControllerPassSampledInput(uint8_t relayStateDesired, float mainBusVoltage, float batteryVoltage);
void  modHiAmpShieldRelayControllerTask(void);
void  modHiAmpShieldRelayControllerSetRelayOutputState(bool newStateRelay, bool newStatePreCharge);
void  modHiAmpShieldTemperatureHumidityMeasureTask(void);
void  modHiAmpShieldResetSensors(void);

#endif
