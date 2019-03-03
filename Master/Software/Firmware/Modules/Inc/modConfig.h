#ifndef __MODCONFIG_H
#define __MODCONFIG_H

#include "stdint.h"
#include "stdbool.h"
#include "mainDataTypes.h"
#include "driverSWStorageManager.h"
#include "generalDefines.h"

#define modConfigNoOfNTCTypes 4
#define modConfigNTCGroupHiAmpExt  0
#define modConfigNTCGroupHiAmpPCB  1
#define modConfigNTCGroupLTCExt    2
#define modConfigNTCGroupMasterPCB 3


typedef struct {
	// Master BMS
	uint8_t  noOfCells;																														// Number of cells in series in pack
	float    batteryCapacity;																											// Battery capacity in Ah
	float    cellHardUnderVoltage;																								// If the lowest cell is under this voltage -> Error situation, turn all off and power down
	float    cellHardOverVoltage;																									// If the upper cell is above this voltage -> Error situation, turn all off and power down
	float    cellLCSoftUnderVoltage;																							// If the lowest cell is under this voltage -> disable low current load
	float    cellHCSoftUnderVoltage;																							// If the lowest cell is under this voltage -> disable high current load
	float    cellSoftOverVoltage;																								  // If the upper cell is above this voltage -> disable charging, but keep bms enabled
	float    cellBalanceDifferenceThreshold;																			// If the upper cell is more than this voltage away from the average -> start discharging this cell
	float    cellBalanceStart;																										// If an upper cell is above this voltage and higher than the cellBalanceDifferenceThreshold voltage then average, start discharging 
	float    cellThrottleUpperStart;																							// Charge throttle range
	float    cellThrottleLowerStart;																							// Discharge throttle rande
	float    cellThrottleUpperMargin;																							// Margin from the upper cell voltage extremes
  float    cellThrottleLowerMargin;                                             // Margin from the lower cell voltage extremes
	float    shuntLCFactor;                                                       // Shunt multiplication factor Low current
	uint16_t shuntLCOffset;                                                       // Shunt low current offset
	float    shuntHCFactor;                                                       // Shunt multiplication factor High current	
  uint16_t shuntHCOffset;                                                       // Shunt high current offset	
	uint8_t  throttleChargeIncreaseRate;																					// The rate of charge throttle percentage increase per 100ms
	uint8_t  throttleDisChargeIncreaseRate;                                       // The rate of discharge throttle percentage instrease per 100ms
	uint32_t cellBalanceUpdateInterval;																						// Amount of time that the balance resistor enable mask is kept
	uint8_t  maxSimultaneousDischargingCells;																			// Set the maximum amount of discharging cells. This is to limit dissepated power (and thus board temperature)
	uint32_t timeoutDischargeRetry;																								// If soft lower threshold limit was tripped wait this amount of time to re-enable load if cell is within threshold
	float    hysteresisDischarge;																									// If the lowest cell voltage rises this amount of mV re enable output
	uint32_t timeoutChargeRetry;																									// If soft higher threshold limit was tripped and cell is within acceptable limits wait this amount of time before re-enabling charge input
	float    hysteresisCharge;																										// If the highest cell voltage loweres this amount of mW re enable charge input
	uint32_t timeoutChargeCompleted;																							// If tricklecharging > this threshold timer declare the pack charged but keep balancing if nessesary
	uint32_t timeoutChargingCompletedMinimalMismatch;															// If charger is disabled and cellvoltagemismatch is under threshold determin charged after this timeout time
	float    maxMismatchThreshold;
	float    chargerEnabledThreshold;																							// Minimal current to stay in charge mode
	uint32_t timeoutChargerDisconnected;																					// Timeout for charger disconnect detection
	float    minimalPrechargePercentage;																					// Output voltage threshold for precharging
	uint32_t timeoutLCPreCharge;																									// If threshold is not reached within this time in ms goto error state
	float    maxAllowedCurrent;																										// Max allowed current passing trough BMS, if limit is exceded disable output
	uint32_t displayTimeoutBatteryDead;																						// Duration of displaying battery dead symbol
	uint32_t displayTimeoutBatteryError;																					// Duration of displaying error symbol
	uint32_t displayTimeoutBatteryErrorPreCharge;																	// Duration of displaying error symbol
	uint32_t displayTimeoutSplashScreen;																					// Duration of displaying splash screen + First few samples of ADC's
	uint8_t  maxUnderAndOverVoltageErrorCount;																		// Threshold that defines max amount of hard over / under voltage errors
	float    notUsedCurrentThreshold;																							// Threshold that defines whether or not pack is in use.
	uint32_t notUsedTimeout;																											// Delay time that defines max amount of no operation on-time. When absolute battery curren < notUsedCurrentThreshold for longer than this amount of time -> the system is disabled
	uint32_t stateOfChargeStoreInterval;																					// Interval to store state of charge information.
	uint8_t  CANID;																																// Stores the CAN ID of the device.
	uint8_t  CANIDStyle;                                                          // Store the ID style used to communicate over CAN
	uint8_t  emitStatusOverCAN;                                                   // Enable or disable sending of status over CAN
	uint16_t tempEnableMaskBMS;																								    // Stores the mask to select what temperature sensor is enabled for the BMS.
	uint16_t tempEnableMaskBattery;																								// Stores the mask to select what temperature sensor is enabled for the battery.
	uint8_t  LCUseDischarge;                                                      // Enable or disable switch output.
	uint8_t  LCUsePrecharge;                                                      // choice whether to precharge or not.
	uint8_t  allowChargingDuringDischarge;																				// Allow charging during discharge.
	uint8_t  allowForceOn;                                                        // Enable or disable the option to be forced on.
	uint8_t  pulseToggleButton;                                                   // Pulse or Toggle power button.
	uint8_t  togglePowerModeDirectHCDelay;                                        // Toggle switch controls onstate directly or controls HC output with delayed turn off.
	uint8_t  useCANSafetyInput;                                                   // Use the safe input state received from the CAN bus.
	uint8_t  useCANDelayedPowerDown;                                              // Config that holds the preference whethet to used delayes power down
	
	// Slave - HiAmp Config
	uint32_t NTCTopResistor[modConfigNoOfNTCTypes];                               // NTC Pullup resistor value
	uint32_t NTC25DegResistance[modConfigNoOfNTCTypes];                           // NTC resistance at 25 degree
	uint16_t NTCBetaFactor[modConfigNoOfNTCTypes];                                // NTC Beta factor
	uint8_t  HCUseRelay;                                                          // Enable or disable relay output
	uint8_t  HCUsePrecharge;                                                      // choice whether to precharge or not
	uint32_t timeoutHCPreCharge;																									// If threshold is not reached within this time in ms goto error state
	uint32_t timeoutHCPreChargeRetryInterval;                                     // When pre charge failed, wait this long
	uint32_t timeoutHCRelayOverlap;																								// When pre charge succeeded turn on main relay and wait this long before disabling precharge
} modConfigGeneralConfigStructTypedef;

modConfigGeneralConfigStructTypedef* modConfigInit(void);
bool modConfigStoreAndLoadDefaultConfig(void);
bool modConfigStoreConfig(void);
bool modConfigLoadConfig(void);
bool modConfigStoreDefaultConfig(void);
void modConfigLoadDefaultConfig(modConfigGeneralConfigStructTypedef *configLocation);

#endif
