#include "modConfig.h"

modConfigGeneralConfigStructTypedef modConfigGeneralConfig;

modConfigGeneralConfigStructTypedef* modConfigInit(void) {
	driverSWStorageManagerConfigStructSize = (sizeof(modConfigGeneralConfigStructTypedef)/sizeof(uint16_t)); // Calculate the space needed for the config struct in EEPROM
	return &modConfigGeneralConfig;
};

bool modConfigStoreAndLoadDefaultConfig(void) {
	bool returnVal = false;
	if(driverSWStorageManagerConfigEmpty) {
		returnVal = modConfigStoreDefaultConfig();
	}
	
	modConfigLoadConfig();
	return returnVal;
};

bool modConfigStoreConfig(void) {
	return driverSWStorageManagerStoreStruct(&modConfigGeneralConfig,STORAGE_CONFIG);
	// TODO_EEPROM
};

bool modConfigLoadConfig(void) {
	return driverSWStorageManagerGetStruct(&modConfigGeneralConfig,STORAGE_CONFIG);
};

bool modConfigStoreDefaultConfig(void) {
	// VALUES WILL ONLY AUTIMATICALLY UPDATE AFTER FLASH ERASE!
	modConfigGeneralConfigStructTypedef defaultConfig;
  modConfigLoadDefaultConfig(&defaultConfig);
	
	driverSWStorageManagerConfigEmpty = false;
	return driverSWStorageManagerStoreStruct(&defaultConfig,STORAGE_CONFIG);
}

void modConfigLoadDefaultConfig(modConfigGeneralConfigStructTypedef *configLocation) {
#ifdef EFoilNew
	configLocation->noOfCells 																     = 12;											// X Cells in series
	configLocation->batteryCapacity														     = 40.00f;									// XXAh battery
	configLocation->cellHardUnderVoltage											     = 2.30f;										// Worst case X.XXV as lowest cell voltage
	configLocation->cellHardOverVoltage												     = 4.25f;										// Worst case X.XXV as highest cell voltage
	configLocation->cellLCSoftUnderVoltage											   = 2.90f;										// Low current lowest cell voltage X.XXV.
  configLocation->cellHCSoftUnderVoltage                         = 3.25f;                   // High current lowest cell voltage X.XXV.
	configLocation->cellSoftOverVoltage												     = 4.15f;										// Normal highest cell voltage X.XXV.
	configLocation->cellBalanceDifferenceThreshold                 = 0.01f;										// Start balancing @ XmV difference, stop if below.
	configLocation->cellBalanceStart													     = 3.80f;										// Start balancing above X.XXV.
	configLocation->cellThrottleUpperStart										     = 0.03f;										// Upper range of cell voltage for charge throttling.
	configLocation->cellThrottleLowerStart										     = 0.20f;									  // Lower range of cell voltage for discharge throttling.
	configLocation->cellThrottleUpperMargin										     = 0.01f;										// Margin of throttle from upper soft limits.
	configLocation->cellThrottleLowerMargin										     = 0.50f;									  // Margin of throttle from lower soft limits.
  configLocation->shuntLCFactor                                  = -0.004494f;              // Shunt factor low current
	configLocation->shuntLCOffset                                  = 0;                       // Shunt offset low current
  configLocation->shuntHCFactor	                                 = -0.038f;                 // Shunt factor high current
	configLocation->shuntHCOffset                                  = 4;                       // Shunt offset high current
	configLocation->throttleChargeIncreaseRate                     = 1;                       // Percentage charge throttle increase rate per 100ms (cell voltage loop time)  
	configLocation->throttleDisChargeIncreaseRate                  = 2;                       // Percentage discharge throttle increase rate per 100ms (cell voltage loop time)  	
	configLocation->cellBalanceUpdateInterval									     = 4*1000;									// Keep calculated resistors enabled for this amount of time in miliseconds.
	configLocation->maxSimultaneousDischargingCells						     = 5;												// Allow a maximum of X cells simultinous discharging trough bleeding resistors.
	configLocation->timeoutDischargeRetry											     = 4*1000;									// Wait for X seconds before retrying to enable load.
	configLocation->hysteresisDischarge 											     = 0.02f;										// Lowest cell should rise XXmV before output is re enabled.
	configLocation->timeoutChargeRetry												     = 30*1000;									// Wait for XX seconds before retrying to enable charger.
	configLocation->hysteresisCharge													     = 0.01f;										// Highest cell should lower XXmV before charger is re enabled.
	configLocation->timeoutChargeCompleted										     = 30*60*1000;							// Wait for XX minutes before setting charge state to charged.
	configLocation->timeoutChargingCompletedMinimalMismatch 	     = 6*1000;									// If cell mismatch is under threshold and (charging is not allowed) wait this delay time to set "charged" state.
	configLocation->maxMismatchThreshold											     = 0.020f;									// If mismatch is under this threshold for timeoutChargingCompletedMinimalMismatch determin fully charged.
	configLocation->chargerEnabledThreshold										     = 0.5f;										// If charge current > X.XA stay in charging mode and dont power off.
	configLocation->timeoutChargerDisconnected								     = 2000;										// Wait for X seconds to respond to charger disconnect.
	configLocation->minimalPrechargePercentage								     = 0.80f;										// output should be at a minimal of 80% of input voltage.
	configLocation->timeoutLCPreCharge												     = 300;											// Precharge error timeout, allow 300ms pre-charge time before declaring load error.
	configLocation->maxAllowedCurrent													     = 120.0f;									// Allow max XXXA trough BMS.
	configLocation->displayTimeoutBatteryDead									     = 5000;										// Show battery dead symbol X seconds before going to powerdown in cell voltage error state.
	configLocation->displayTimeoutBatteryError								     = 2000;										// Show error symbol for X seconds before going to powerdown in general error state.
	configLocation->displayTimeoutBatteryErrorPreCharge				     = 10000;										// Show pre charge error for XX seconds.
	configLocation->displayTimeoutSplashScreen								     = 1000;										// Display / INIT splash screen time.
	configLocation->maxUnderAndOverVoltageErrorCount 					     = 5;												// Max count of hard cell voltage errors.
	configLocation->notUsedCurrentThreshold										     = 1.0f;										// If abs(packcurrent) < X.XA consider pack as not used.
	configLocation->notUsedTimeout														     = 60*60*1000;							// If pack is not used for longer than XX minutes disable bms.
	configLocation->stateOfChargeStoreInterval								     = 60*1000;									// Interval in ms to store state of charge information.
	configLocation->CANID																			     = 10;											// CAN ID for CAN communication.
	configLocation->CANIDStyle                                     = CANIDStyleFoiler;        // CAN ID default Style
	configLocation->emitStatusOverCAN                              = true;                    // Send status over can.
	configLocation->tempEnableMaskBMS                              = 0x1C0F;									// Bitwise select what sensor to enable for the BMS (internal sensors).
	configLocation->tempEnableMaskBattery                          = 0x03F0;									// Bitwise select what sensor to enable for the battery (external sensors).
  configLocation->LCUseDischarge                                 = true;                    // Enable or disable the solid state output
	configLocation->LCUsePrecharge                                 = true;                    // Use precharge before enabling main output
	configLocation->NTCTopResistor[modConfigNTCGroupLTCExt]        = 100000;                  // NTC Pullup resistor value
	configLocation->NTCTopResistor[modConfigNTCGroupMasterPCB]     = 100000;                  // NTC Pullup resistor value
	configLocation->NTC25DegResistance[modConfigNTCGroupLTCExt]    = 100000;                  // NTC resistance at 25 degree
	configLocation->NTC25DegResistance[modConfigNTCGroupMasterPCB] = 100000;                  // NTC resistance at 25 degree
	configLocation->NTCBetaFactor[modConfigNTCGroupLTCExt]         = 4390;                    // NTC Beta factor
	configLocation->NTCBetaFactor[modConfigNTCGroupMasterPCB]      = 3590;                    // NTC Beta factor
	configLocation->allowChargingDuringDischarge                   = true;                    // Allow the battery to be charged in normal mode
	configLocation->allowForceOn                                   = false;                   // Allow the BMS to be forced ON by long actuation of the power button
	configLocation->pulseToggleButton                              = false;                   // Select either pulse or toggle power button
	configLocation->togglePowerModeDirectHCDelay                   = true;                    // Select either direct power state control or HC output control with delayed turn off.
	configLocation->useCANSafetyInput                              = true;                    // Use the safety input status from CAN
	configLocation->useCANDelayedPowerDown                         = true;                    // Use delayed power down
	
	// Slave / HiAmp Config
	configLocation->NTCTopResistor[modConfigNTCGroupHiAmpExt]      = 100000;                  // NTC Pullup resistor value
	configLocation->NTCTopResistor[modConfigNTCGroupHiAmpPCB]      = 100000;                  // NTC Pullup resistor value
	configLocation->NTC25DegResistance[modConfigNTCGroupHiAmpExt]  = 100000;                  // NTC resistance at 25 degree
	configLocation->NTC25DegResistance[modConfigNTCGroupHiAmpPCB]  = 100000;                  // NTC resistance at 25 degree
	configLocation->NTCBetaFactor[modConfigNTCGroupHiAmpExt]       = 4390;                    // NTC Beta factor
	configLocation->NTCBetaFactor[modConfigNTCGroupHiAmpPCB]       = 3590;                    // NTC Beta factor
	configLocation->HCUseRelay                                     = false;                    // Enable or disable the relay output, when false will also disable HC pre charge.
	configLocation->HCUsePrecharge                                 = true;                    // choice whether to precharge or not, will only work when HCUseRelay = true.
	configLocation->timeoutHCPreCharge													   = 300;											// Precharge error timeout, allow xxxms pre-charge time before declaring load error.
	configLocation->timeoutHCPreChargeRetryInterval						     = 20000;										// When pre charge failes wait this long in ms
	configLocation->timeoutHCRelayOverlap											     = 1000;										// When precharge succeeds enable both relay and precharge combined for this time, then go to relay only.
#endif

#ifdef EFoilOld
	configLocation->noOfCells 																     = 12;											// X Cells in series
	configLocation->batteryCapacity														     = 40.00f;									// XXAh battery
	configLocation->cellHardUnderVoltage											     = 2.30f;										// Worst case X.XXV as lowest cell voltage
	configLocation->cellHardOverVoltage												     = 4.25f;										// Worst case X.XXV as highest cell voltage
	configLocation->cellLCSoftUnderVoltage											   = 2.90f;										// Low current lowest cell voltage X.XXV.
  configLocation->cellHCSoftUnderVoltage                         = 3.25f;                   // High current lowest cell voltage X.XXV.
	configLocation->cellSoftOverVoltage												     = 4.15f;										// Normal highest cell voltage X.XXV.
	configLocation->cellBalanceDifferenceThreshold                 = 0.01f;										// Start balancing @ XmV difference, stop if below.
	configLocation->cellBalanceStart													     = 3.80f;										// Start balancing above X.XXV.
	configLocation->cellThrottleUpperStart										     = 0.03f;										// Upper range of cell voltage for charge throttling.
	configLocation->cellThrottleLowerStart										     = 0.20f;									  // Lower range of cell voltage for discharge throttling.
	configLocation->cellThrottleUpperMargin										     = 0.01f;										// Margin of throttle from upper soft limits.
	configLocation->cellThrottleLowerMargin										     = 0.50f;									  // Margin of throttle from lower soft limits.	
  configLocation->shuntLCFactor                                  = -0.004494f;              // Shunt factor low current
	configLocation->shuntLCOffset                                  = 0;                       // Shunt offset low current
  configLocation->shuntHCFactor	                                 = -0.038f;                 // Shunt factor high current
	configLocation->shuntHCOffset                                  = 4;                       // Shunt offset high current
	configLocation->throttleChargeIncreaseRate                     = 1;                       // Percentage charge throttle increase rate per 100ms (cell voltage loop time)  
	configLocation->throttleDisChargeIncreaseRate                  = 2;                       // Percentage discharge throttle increase rate per 100ms (cell voltage loop time)  	
	configLocation->cellBalanceUpdateInterval									     = 4*1000;									// Keep calculated resistors enabled for this amount of time in miliseconds.
	configLocation->maxSimultaneousDischargingCells						     = 5;												// Allow a maximum of X cells simultinous discharging trough bleeding resistors.
	configLocation->timeoutDischargeRetry											     = 4*1000;									// Wait for X seconds before retrying to enable load.
	configLocation->hysteresisDischarge 											     = 0.02f;										// Lowest cell should rise XXmV before output is re enabled.
	configLocation->timeoutChargeRetry												     = 30*1000;									// Wait for XX seconds before retrying to enable charger.
	configLocation->hysteresisCharge													     = 0.01f;										// Highest cell should lower XXmV before charger is re enabled.
	configLocation->timeoutChargeCompleted										     = 30*60*1000;							// Wait for XX minutes before setting charge state to charged.
	configLocation->timeoutChargingCompletedMinimalMismatch 	     = 6*1000;									// If cell mismatch is under threshold and (charging is not allowed) wait this delay time to set "charged" state.
	configLocation->maxMismatchThreshold											     = 0.020f;									// If mismatch is under this threshold for timeoutChargingCompletedMinimalMismatch determin fully charged.
	configLocation->chargerEnabledThreshold										     = 0.5f;										// If charge current > X.XA stay in charging mode and dont power off.
	configLocation->timeoutChargerDisconnected								     = 2000;										// Wait for X seconds to respond to charger disconnect.
	configLocation->minimalPrechargePercentage								     = 0.80f;										// output should be at a minimal of 80% of input voltage.
	configLocation->timeoutLCPreCharge												     = 300;											// Precharge error timeout, allow 300ms pre-charge time before declaring load error.
	configLocation->maxAllowedCurrent													     = 120.0f;									// Allow max XXXA trough BMS.
	configLocation->displayTimeoutBatteryDead									     = 5000;										// Show battery dead symbol X seconds before going to powerdown in cell voltage error state.
	configLocation->displayTimeoutBatteryError								     = 2000;										// Show error symbol for X seconds before going to powerdown in general error state.
	configLocation->displayTimeoutBatteryErrorPreCharge				     = 10000;										// Show pre charge error for XX seconds.
	configLocation->displayTimeoutSplashScreen								     = 1000;										// Display / INIT splash screen time.
	configLocation->maxUnderAndOverVoltageErrorCount 					     = 5;												// Max count of hard cell voltage errors.
	configLocation->notUsedCurrentThreshold										     = 1.0f;										// If abs(packcurrent) < X.XA consider pack as not used.
	configLocation->notUsedTimeout														     = 60*60*1000;							// If pack is not used for longer than XX minutes disable bms.
	configLocation->stateOfChargeStoreInterval								     = 60*1000;									// Interval in ms to store state of charge information.
	configLocation->CANID																			     = 10;											// CAN ID for CAN communication.
	configLocation->CANIDStyle                                     = CANIDStyleFoiler;        // CAN ID default Style
	configLocation->emitStatusOverCAN                              = true;                    // Send status over can.
	configLocation->tempEnableMaskBMS                              = 0x1C0F;									// Bitwise select what sensor to enable for the BMS (internal sensors).
	configLocation->tempEnableMaskBattery                          = 0x03F0;									// Bitwise select what sensor to enable for the battery (external sensors).
  configLocation->LCUseDischarge                                 = true;                    // Enable or disable the solid state output
	configLocation->LCUsePrecharge                                 = true;                    // Use precharge before enabling main output
	configLocation->NTCTopResistor[modConfigNTCGroupLTCExt]        = 100000;                  // NTC Pullup resistor value
	configLocation->NTCTopResistor[modConfigNTCGroupMasterPCB]     = 100000;                  // NTC Pullup resistor value
	configLocation->NTC25DegResistance[modConfigNTCGroupLTCExt]    = 100000;                  // NTC resistance at 25 degree
	configLocation->NTC25DegResistance[modConfigNTCGroupMasterPCB] = 100000;                  // NTC resistance at 25 degree
	configLocation->NTCBetaFactor[modConfigNTCGroupLTCExt]         = 4390;                    // NTC Beta factor
	configLocation->NTCBetaFactor[modConfigNTCGroupMasterPCB]      = 3590;                    // NTC Beta factor
	configLocation->allowChargingDuringDischarge                   = true;                    // Allow the battery to be charged in normal mode
	configLocation->allowForceOn                                   = false;                   // Allow the BMS to be forced ON by long actuation of the power button
	configLocation->pulseToggleButton                              = true;                    // Select either pulse or toggle power button
	configLocation->togglePowerModeDirectHCDelay                   = true;                    // Select either direct power state control or HC output control with delayed turn off.
	configLocation->useCANSafetyInput                              = false;                   // Use the safety input status from CAN
	configLocation->useCANDelayedPowerDown                         = false;                   // Use delayed power down
	
	// Slave / HiAmp Config
	configLocation->NTCTopResistor[modConfigNTCGroupHiAmpExt]      = 100000;                  // NTC Pullup resistor value
	configLocation->NTCTopResistor[modConfigNTCGroupHiAmpPCB]      = 100000;                  // NTC Pullup resistor value
	configLocation->NTC25DegResistance[modConfigNTCGroupHiAmpExt]  = 100000;                  // NTC resistance at 25 degree
	configLocation->NTC25DegResistance[modConfigNTCGroupHiAmpPCB]  = 100000;                  // NTC resistance at 25 degree
	configLocation->NTCBetaFactor[modConfigNTCGroupHiAmpExt]       = 4390;                    // NTC Beta factor
	configLocation->NTCBetaFactor[modConfigNTCGroupHiAmpPCB]       = 3590;                    // NTC Beta factor
	configLocation->HCUseRelay                                     = false;                    // Enable or disable the relay output, when false will also disable HC pre charge.
	configLocation->HCUsePrecharge                                 = true;                    // choice whether to precharge or not, will only work when HCUseRelay = true.
	configLocation->timeoutHCPreCharge													   = 300;											// Precharge error timeout, allow xxxms pre-charge time before declaring load error.
	configLocation->timeoutHCPreChargeRetryInterval						     = 20000;										// When pre charge failes wait this long in ms
	configLocation->timeoutHCRelayOverlap											     = 1000;										// When precharge succeeds enable both relay and precharge combined for this time, then go to relay only.
#endif

#ifdef ESK8
	configLocation->noOfCells 																     = 10;											// X Cells in series
	configLocation->batteryCapacity														     = 12.00f;									// XXAh battery
	configLocation->cellHardUnderVoltage											     = 2.30f;										// Worst case X.XXV as lowest cell voltage
	configLocation->cellHardOverVoltage												     = 4.35f;										// Worst case X.XXV as highest cell voltage
	configLocation->cellLCSoftUnderVoltage											   = 2.70f;										// Low current lowest cell voltage X.XXV.
  configLocation->cellHCSoftUnderVoltage                         = 3.25f;                   // High current lowest cell voltage X.XXV.
	configLocation->cellSoftOverVoltage												     = 4.20f;										// Normal highest cell voltage X.XXV.
	configLocation->cellBalanceDifferenceThreshold                 = 0.01f;										// Start balancing @ XmV difference, stop if below.
	configLocation->cellBalanceStart													     = 3.80f;										// Start balancing above X.XXV.
	configLocation->cellThrottleUpperStart										     = 0.03f;										// Upper range of cell voltage for charge throttling.
	configLocation->cellThrottleLowerStart										     = 0.20f;									  // Lower range of cell voltage for discharge throttling.
	configLocation->cellThrottleUpperMargin										     = 0.01f;										// Margin of throttle from upper soft limits.
	configLocation->cellThrottleLowerMargin										     = 0.50f;									  // Margin of throttle from lower soft limits.	
  configLocation->shuntLCFactor                                  = -0.004494f;              // Shunt factor low current
	configLocation->shuntLCOffset                                  = 0;                       // Shunt offset low current
  configLocation->shuntHCFactor	                                 = -0.038f;                 // Shunt factor high current
	configLocation->shuntHCOffset                                  = 4;                       // Shunt offset high current
	configLocation->throttleChargeIncreaseRate                     = 1;                       // Percentage charge throttle increase rate per 100ms (cell voltage loop time)  
	configLocation->throttleDisChargeIncreaseRate                  = 2;                       // Percentage discharge throttle increase rate per 100ms (cell voltage loop time)  	
	configLocation->cellBalanceUpdateInterval									     = 4*1000;									// Keep calculated resistors enabled for this amount of time in miliseconds.
	configLocation->maxSimultaneousDischargingCells						     = 5;												// Allow a maximum of X cells simultinous discharging trough bleeding resistors.
	configLocation->timeoutDischargeRetry											     = 4*1000;									// Wait for X seconds before retrying to enable load.
	configLocation->hysteresisDischarge 											     = 0.02f;										// Lowest cell should rise XXmV before output is re enabled.
	configLocation->timeoutChargeRetry												     = 30*1000;									// Wait for XX seconds before retrying to enable charger.
	configLocation->hysteresisCharge													     = 0.01f;										// Highest cell should lower XXmV before charger is re enabled.
	configLocation->timeoutChargeCompleted										     = 30*60*1000;							// Wait for XX minutes before setting charge state to charged.
	configLocation->timeoutChargingCompletedMinimalMismatch 	     = 6*1000;									// If cell mismatch is under threshold and (charging is not allowed) wait this delay time to set "charged" state.
	configLocation->maxMismatchThreshold											     = 0.020f;									// If mismatch is under this threshold for timeoutChargingCompletedMinimalMismatch determin fully charged.
	configLocation->chargerEnabledThreshold										     = 0.5f;										// If charge current > X.XA stay in charging mode and dont power off.
	configLocation->timeoutChargerDisconnected								     = 2000;										// Wait for X seconds to respond to charger disconnect.
	configLocation->minimalPrechargePercentage								     = 0.80f;										// output should be at a minimal of 80% of input voltage.
	configLocation->timeoutLCPreCharge												     = 300;											// Precharge error timeout, allow 300ms pre-charge time before declaring load error.
	configLocation->maxAllowedCurrent													     = 120.0f;									// Allow max XXXA trough BMS.
	configLocation->displayTimeoutBatteryDead									     = 5000;										// Show battery dead symbol X seconds before going to powerdown in cell voltage error state.
	configLocation->displayTimeoutBatteryError								     = 2000;										// Show error symbol for X seconds before going to powerdown in general error state.
	configLocation->displayTimeoutBatteryErrorPreCharge				     = 10000;										// Show pre charge error for XX seconds.
	configLocation->displayTimeoutSplashScreen								     = 1000;										// Display / INIT splash screen time.
	configLocation->maxUnderAndOverVoltageErrorCount 					     = 5;												// Max count of hard cell voltage errors.
	configLocation->notUsedCurrentThreshold										     = 1.0f;										// If abs(packcurrent) < X.XA consider pack as not used.
	configLocation->notUsedTimeout														     = 20*60*1000;							// If pack is not used for longer than XX minutes disable bms.
	configLocation->stateOfChargeStoreInterval								     = 60*1000;									// Interval in ms to store state of charge information.
	configLocation->CANID																			     = 10;											// CAN ID for CAN communication.
	configLocation->CANIDStyle                                     = CANIDStyleVESC;          // CAN ID default Style.
	configLocation->emitStatusOverCAN                              = false;                    // Send status over can.
	configLocation->tempEnableMaskBMS                              = 0x0004;									// Bitwise select what sensor to enable for the BMS (internal sensors).
	configLocation->tempEnableMaskBattery                          = 0x0000;									// Bitwise select what sensor to enable for the battery (external sensors).
  configLocation->LCUseDischarge                                 = true;                    // Enable or disable the solid state output
	configLocation->LCUsePrecharge                                 = true;                    // Use precharge before enabling main output
	configLocation->NTCTopResistor[modConfigNTCGroupLTCExt]        = 100000;                  // NTC Pullup resistor value
	configLocation->NTCTopResistor[modConfigNTCGroupMasterPCB]     = 100000;                  // NTC Pullup resistor value
	configLocation->NTC25DegResistance[modConfigNTCGroupLTCExt]    = 100000;                  // NTC resistance at 25 degree
	configLocation->NTC25DegResistance[modConfigNTCGroupMasterPCB] = 100000;                  // NTC resistance at 25 degree
	configLocation->NTCBetaFactor[modConfigNTCGroupLTCExt]         = 4390;                    // NTC Beta factor
	configLocation->NTCBetaFactor[modConfigNTCGroupMasterPCB]      = 3590;                    // NTC Beta factor
	configLocation->allowChargingDuringDischarge                   = true;                    // Allow the battery to be charged in normal mode
	configLocation->allowForceOn                                   = false;                   // Allow the BMS to be forced ON by long actuation of the power button
	configLocation->pulseToggleButton                              = true;                    // Select either pulse or toggle power button
	configLocation->togglePowerModeDirectHCDelay                   = true;                    // Select either direct power state control or HC output control with delayed turn off.
	configLocation->useCANSafetyInput                              = false;                   // Use the safety input status from CAN
	configLocation->useCANDelayedPowerDown                         = false;                   // Use delayed power down
	
	// Slave / HiAmp Config
	configLocation->NTCTopResistor[modConfigNTCGroupHiAmpExt]      = 100000;                  // NTC Pullup resistor value
	configLocation->NTCTopResistor[modConfigNTCGroupHiAmpPCB]      = 100000;                  // NTC Pullup resistor value
	configLocation->NTC25DegResistance[modConfigNTCGroupHiAmpExt]  = 100000;                  // NTC resistance at 25 degree
	configLocation->NTC25DegResistance[modConfigNTCGroupHiAmpPCB]  = 100000;                  // NTC resistance at 25 degree
	configLocation->NTCBetaFactor[modConfigNTCGroupHiAmpExt]       = 4390;                    // NTC Beta factor
	configLocation->NTCBetaFactor[modConfigNTCGroupHiAmpPCB]       = 3590;                    // NTC Beta factor
	configLocation->HCUseRelay                                     = false;                    // Enable or disable the relay output, when false will also disable HC pre charge.
	configLocation->HCUsePrecharge                                 = true;                    // choice whether to precharge or not, will only work when HCUseRelay = true.
	configLocation->timeoutHCPreCharge													   = 300;											// Precharge error timeout, allow xxxms pre-charge time before declaring load error.
	configLocation->timeoutHCPreChargeRetryInterval						     = 20000;										// When pre charge failes wait this long in ms
	configLocation->timeoutHCRelayOverlap											     = 1000;										// When precharge succeeds enable both relay and precharge combined for this time, then go to relay only.
#endif
}
