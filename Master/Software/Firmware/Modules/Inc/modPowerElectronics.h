#ifndef __MODPOWERELECTRONICS_H
#define __MODPOWERELECTRONICS_H

#include "driverSWISL28022.h"
#include "driverHWADC.h"
#include "driverSWLTC6803.h"
#include "driverHWSwitches.h"
#include "driverSWEMC2305.h"
#include "modDelay.h"
#include "modConfig.h"
#include "modPowerState.h"
#include "stdbool.h"
#include "math.h"

#define NoOfCellsPossibleOnChip	12
#define NoOfTempSensors         13
#define PRECHARGE_PERCENTAGE 		0.75f
#define TotalLTCICs							1
#define ISLErrorThreshold       10

typedef enum {
	TEMP_EXT_LTC_NTC0 = 0,									// EXT on master BMS on LTC
	TEMP_EXT_LTC_NTC1,											// EXT on master BMS on LTC
	TEMP_INT_LTC_CHIP,											// Int on master BMS inside LTC Chip
	TEMP_INT_STM_NTC,												// Int on master BMS outside STM Chip
	TEMP_EXT_ADC_NTC0,											// Ext on slave BMS
	TEMP_EXT_ADC_NTC1,											// Ext on slave BMS
	TEMP_EXT_ADC_NTC2,											// Ext on slave BMS
	TEMP_EXT_ADC_NTC3,											// Ext on slave BMS
	TEMP_EXT_ADC_NTC4,											// Ext on slave BMS
	TEMP_EXT_ADC_NTC5,											// Ext on slave BMS
	TEMP_INT_ADC_NTC6,											// Int on slave BMS
	TEMP_INT_ADC_NTC7,											// Int on slave BMS
	TEMP_INT_SHT														// Int on slave BMS
} modPowerElectronicsTemperatureSensorMapping;

typedef enum {
	PACK_STATE_ERROR_HARD_CELLVOLTAGE = 0,
	PACK_STATE_ERROR_SOFT_CELLVOLTAGE,
	PACK_STATE_ERROR_OVER_CURRENT,
	PACK_STATE_NORMAL,
} modPowerElectronicsPackOperationalCellStatesTypedef;

typedef struct {
	// Master BMS
	uint8_t  throttleDutyCharge;
	uint8_t  throttleDutyDischarge;
	float    SoC;
	float    SoCCapacityAh;
	OperationalStateTypedef operationalState;
	float    packVoltage;
	float    packCurrent;
	float    packPower;
	float    loCurrentLoadCurrent;
	float    loCurrentLoadVoltage;
	float    cellVoltageHigh;
	float    cellVoltageLow;
	float    cellVoltageAverage;
	float    cellVoltageMisMatch;
	uint16_t cellBalanceResistorEnableMask;
	float    temperatures[NoOfTempSensors];
	float    tempBatteryHigh;
	float    tempBatteryLow;
	float    tempBatteryAverage;
	float    tempBMSHigh;
	float    tempBMSLow;
	float    tempBMSAverage;
	uint8_t  preChargeDesired;
	uint8_t  disChargeDesired;
	uint8_t  disChargeLCAllowed;
	uint8_t  disChargeHCAllowed;
	uint8_t  chargeDesired;
	uint8_t  chargeAllowed;
	uint8_t  safetyOverCANHCSafeNSafe;
	uint8_t  chargeCurrentDetected;
	uint8_t  chargeBalanceActive;
	uint8_t  powerButtonActuated;
	uint8_t  packInSOA;
	uint8_t  watchDogTime;
	driverLTC6803CellsTypedef cellVoltagesIndividual[NoOfCellsPossibleOnChip];
	modPowerElectronicsPackOperationalCellStatesTypedef packOperationalCellState;
	
	// Slave BMS
	uint8_t  hiAmpShieldPresent;
	float    hiCurrentLoadVoltage;
	float    hiCurrentLoadCurrent;
	float		 hiCurrentLoadPower;
	float    auxVoltage;
	float    auxCurrent;
	float		 auxPower;
	uint8_t  aux0EnableDesired;
	uint8_t  aux0Enabled;
	uint8_t  aux0LoadIncorrect;
	uint8_t  aux1EnableDesired;
	uint8_t  aux1Enabled;
	uint8_t  aux1LoadIncorrect;
	uint8_t  auxDCDCEnabled;
	uint8_t  auxDCDCOutputOK;
	float    humidity;
	uint8_t  hiLoadEnabled;
	uint8_t  hiLoadPreChargeEnabled;
	uint8_t  hiLoadPreChargeError;
	uint8_t	 IOIN1;
	uint8_t  IOOUT0;
	uint8_t  FANSpeedDutyDesired;
	driverSWEMC2305FanStatusTypeDef FANStatus;
} modPowerElectricsPackStateTypedef;

void modPowerElectronicsInit(modPowerElectricsPackStateTypedef *packState, modConfigGeneralConfigStructTypedef *generalConfig);
bool modPowerElectronicsTask(void);
void modPowerElectronicsAllowForcedOn(bool allowedState);
void modPowerElectronicsSetPreCharge(bool newState);
bool modPowerElectronicsSetDisCharge(bool newState);
void modPowerElectronicsSetCharge(bool newState);
void modPowerElectronicsDisableAll(void);
void modPowerElectronicsCalculateCellStats(void);
void modPowerElectronicsSubTaskBalaning(void);
void modPowerElectronicsSubTaskVoltageWatch(void);
void modPowerElectronicsUpdateSwitches(void);
void modPowerElectronicsSortCells(driverLTC6803CellsTypedef *cells, uint8_t cellCount);
void modPowerElectronicsCalcTempStats(void);
void modPowerElectronicsCalcThrottle(void);
int32_t modPowerElectronicsMapVariableInt(int32_t inputVariable, int32_t inputLowerLimit, int32_t inputUpperLimit, int32_t outputLowerLimit, int32_t outputUpperLimit);
float modPowerElectronicsMapVariableFloat(float inputVariable, float inputLowerLimit, float inputUpperLimit, float outputLowerLimit, float outputUpperLimit);
void modPowerElectronicsInitISL(void);
bool modPowerElectronicsHCSafetyCANAndPowerButtonCheck(void);
void modPowerElectronicsResetBalanceModeActiveTimeout(void);

#endif
