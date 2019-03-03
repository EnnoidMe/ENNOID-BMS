#include "stm32f3xx_hal.h"
#include "stdbool.h"
#include "modPowerElectronics.h"

void driverSWDCDCInit(modPowerElectricsPackStateTypedef* packStateHandle);
void driverSWDCDCSetEnabledState(bool newEnabledState);
bool driverSWDCDCGetEnabledState(void);
bool driverSWDCDCGetOKState(void);
void driverSWDCDCEnableTask(void);
float driverSWDCDCGetAuxVoltage(void);
float driverSWDCDCGetAuxCurrent(void);
bool driverSWDCDCCheckVoltage(float voltageActual, float voltageNominal, float accuracy);
