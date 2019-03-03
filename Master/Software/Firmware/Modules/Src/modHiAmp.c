#include "modHiAmp.h"

modPowerElectricsPackStateTypedef *modHiAmpPackStateHandle;
modConfigGeneralConfigStructTypedef *modHiAmpGeneralConfigHandle;

uint32_t modHiAmpShieldPresenceDetectLastTick;
uint32_t modHiAmpShieldSamplingLastTick;
uint32_t modHiAmpShieldRelayTimeoutLastTick;

relayControllerStateTypeDef modHiAmpShieldRelayControllerRelayEnabledState;
relayControllerStateTypeDef modHiAmpShieldRelayControllerRelayEnabledLastState;

bool modHiAmpShieldRelayControllerRelayEnabledDesiredLastState;

void modHiAmpInit(modPowerElectricsPackStateTypedef* packStateHandle, modConfigGeneralConfigStructTypedef *generalConfigPointer){
	modHiAmpPackStateHandle     = packStateHandle;																		// Store pack state pointer.
	modHiAmpGeneralConfigHandle = generalConfigPointer;
	
	driverHWI2C1Init();																																// Init the communication bus
	modHiAmpPackStateHandle->hiAmpShieldPresent = modHiAmpShieldPresentCheck();				// Check presence and store it
	
	// Initialisation variables
	modHiAmpShieldResetVariables();																										// Reset the hiAmp shield variables
	modHiAmpShieldRelayControllerRelayEnabledState = RELAY_CONTROLLER_OFF;
	modHiAmpShieldRelayControllerRelayEnabledLastState = RELAY_CONTROLLER_INIT;
	modHiAmpShieldRelayControllerRelayEnabledDesiredLastState = false;
	
	// Initialise slave board
	if(modHiAmpPackStateHandle->hiAmpShieldPresent){
		driverSWDCDCInit(modHiAmpPackStateHandle);																			// Init the DCDC converter enviroment
		driverSWDCDCSetEnabledState(true);																							// Enable the converter
		driverSWEMC2305Init(I2CADDRFANDriver,100);																			// Init the FANDriver with addres and minimal duty cycle
		modHiAmpShieldSetFANSpeedAll(0);																								// Disable all FANs
		modHiAmpShieldMainShuntMonitorInit();
		driverSWPCAL6416Init(0x07,0xF7,0x07,0xF7,0x07,0xF7);														// Init the IO Extender
		driverSWADC128D818Init();																												// Init the NTC ADC
		driverSWSHT21Init();																														// Init the Temperature / humidity sensor
	}else{
	  modHiAmpShieldResetSensors();
	}
}

void modHiAmpTask(void) {
	if(modDelayTick1ms(&modHiAmpShieldPresenceDetectLastTick,5000)){
		modHiAmpPackStateHandle->hiAmpShieldPresent = modHiAmpShieldPresentCheck();
	}
	
	if(modDelayTick1ms(&modHiAmpShieldSamplingLastTick,100)){
		if(modHiAmpPackStateHandle->hiAmpShieldPresent){
			// Determin whether discharge should be allowed
			bool dischargeHCEnable;
			
			if(modHiAmpGeneralConfigHandle->togglePowerModeDirectHCDelay || modHiAmpGeneralConfigHandle->pulseToggleButton){
				dischargeHCEnable = modHiAmpPackStateHandle->disChargeDesired && modHiAmpPackStateHandle->disChargeHCAllowed && modPowerElectronicsHCSafetyCANAndPowerButtonCheck();
			}else{
				dischargeHCEnable = modHiAmpPackStateHandle->disChargeDesired && modHiAmpPackStateHandle->disChargeHCAllowed && modHiAmpPackStateHandle->powerButtonActuated && modPowerElectronicsHCSafetyCANAndPowerButtonCheck();
			}

			// Update inputs
			modHiAmpPackStateHandle->FANStatus = driverEMC2305GetFANStatus(I2CADDRFANDriver);
			modHiAmpPackStateHandle->auxDCDCEnabled = driverSWDCDCGetEnabledState();
			modHiAmpPackStateHandle->auxVoltage = driverSWDCDCGetAuxVoltage();
			modHiAmpPackStateHandle->auxCurrent = driverSWDCDCGetAuxCurrent();
			modHiAmpPackStateHandle->auxPower = modHiAmpPackStateHandle->auxVoltage * modHiAmpPackStateHandle->auxCurrent;
			modHiAmpPackStateHandle->auxDCDCOutputOK = driverSWDCDCCheckVoltage(modHiAmpPackStateHandle->auxVoltage,12.0f,0.05f); // Nominal is 12V max error is 5%
			modHiAmpPackStateHandle->hiCurrentLoadVoltage = modHiAmpShieldShuntMonitorGetVoltage();
			modHiAmpPackStateHandle->hiCurrentLoadCurrent = modHiAmpShieldShuntMonitorGetCurrent();
			modHiAmpPackStateHandle->hiCurrentLoadPower = modHiAmpPackStateHandle->hiCurrentLoadVoltage * modHiAmpPackStateHandle->hiCurrentLoadCurrent;
			modHiAmpShieldTemperatureHumidityMeasureTask();
			
			// Update outputs
			modHiAmpShieldRelayControllerPassSampledInput(dischargeHCEnable,modHiAmpPackStateHandle->hiCurrentLoadVoltage,modHiAmpPackStateHandle->packVoltage);
		}else{
			modHiAmpShieldResetSensors();
	  }
	}
	

	driverSWDCDCEnableTask();
	modHiAmpShieldRelayControllerTask();
}

bool modHiAmpShieldPresentCheck(void) {
	uint8_t I2CWrite = 0;
	uint8_t PresenceDetect = 0;
	
	PresenceDetect |= driverHWI2C1Write(I2CADDRISLMain    ,false,&I2CWrite,0); // ISL Main
	PresenceDetect |= driverHWI2C1Write(I2CADDRISLAux     ,false,&I2CWrite,0); // ISL Aux
	PresenceDetect |= driverHWI2C1Write(I2CADDRIOExt      ,false,&I2CWrite,0); // IO Ext
	PresenceDetect |= driverHWI2C1Write(I2CADDRADC        ,false,&I2CWrite,0); // NTC ADC
	PresenceDetect |= driverHWI2C1Write(I2CADDRFANDriver  ,false,&I2CWrite,0); // FAN Driver
	
	if(PresenceDetect == HAL_OK)
		return true;
	else
		return false;
}

uint8_t modHiAmpShieldScanI2CDevices(void) {
	uint8_t I2CWrite = 0;
	uint8_t PresenceMask = 0;
	
	PresenceMask |= (driverHWI2C1Write(I2CADDRISLMain    ,false,&I2CWrite,0) == HAL_OK) ? (1 << 0) : false; // ISL Main
	PresenceMask |= (driverHWI2C1Write(I2CADDRISLAux     ,false,&I2CWrite,0) == HAL_OK) ? (1 << 1) : false; // ISL Aux
	PresenceMask |= (driverHWI2C1Write(I2CADDRSHT        ,false,&I2CWrite,0) == HAL_OK) ? (1 << 2) : false; // ISL Aux
	PresenceMask |= (driverHWI2C1Write(I2CADDRIOExt      ,false,&I2CWrite,0) == HAL_OK) ? (1 << 3) : false; // IO Ext
	PresenceMask |= (driverHWI2C1Write(I2CADDRADC        ,false,&I2CWrite,0) == HAL_OK) ? (1 << 4) : false; // NTC ADC
	PresenceMask |= (driverHWI2C1Write(I2CADDRFANDriver  ,false,&I2CWrite,0) == HAL_OK) ? (1 << 5) : false; // FAN Driver
	
  return PresenceMask;
}

void modHiAmpShieldResetVariables(void) {
	modHiAmpPackStateHandle->aux0EnableDesired            = false;
	modHiAmpPackStateHandle->aux0Enabled                  = false;
	modHiAmpPackStateHandle->aux1EnableDesired            = false;
	modHiAmpPackStateHandle->aux1Enabled                  = false;
	modHiAmpPackStateHandle->auxDCDCEnabled               = false;
	modHiAmpPackStateHandle->FANSpeedDutyDesired          = 0;
	modHiAmpPackStateHandle->FANStatus.FANEnabled         = false;
	
  modHiAmpShieldResetSensors();
}

void modHiAmpShieldMainShuntMonitorInit(void) {
	driverSWISL28022InitStruct ISLInitStruct;					 																								    // Init the bus voltage and current monitor. (MAIN)
	ISLInitStruct.ADCSetting = ADC_128_64010US;
	ISLInitStruct.busVoltageRange = BRNG_60V_1;
	ISLInitStruct.currentShuntGain = PGA_4_160MV;
	ISLInitStruct.Mode = MODE_SHUNTANDBUS_CONTINIOUS;
	driverSWISL28022Init(ISL28022_SHIELD_MAIN_ADDRES,ISL28022_SHIELD_MAIN_BUS,ISLInitStruct);
}

float modHiAmpShieldShuntMonitorGetVoltage(void) {
  float measuredVoltage;
	driverSWISL28022GetBusVoltage(ISL28022_SHIELD_MAIN_ADDRES,ISL28022_SHIELD_MAIN_BUS,&measuredVoltage,0.004f);
	return measuredVoltage;
}

float modHiAmpShieldShuntMonitorGetCurrent(void) {
  float measuredCurrent;
	driverSWISL28022GetBusCurrent(ISL28022_SHIELD_MAIN_ADDRES,ISL28022_SHIELD_MAIN_BUS,&measuredCurrent,modHiAmpGeneralConfigHandle->shuntHCOffset,modHiAmpGeneralConfigHandle->shuntHCFactor);
	return measuredCurrent;
}

void modHiAmpShieldSetFANSpeedAll(uint8_t newFANSpeed) {
	modHiAmpPackStateHandle->FANSpeedDutyDesired = newFANSpeed;
	driverSWEMC2305SetFANDutyAll(I2CADDRFANDriver,newFANSpeed);
}

void modHiAmpShieldRelayControllerPassSampledInput(uint8_t relayStateDesired, float mainBusVoltage, float batteryVoltage) {
	if(modHiAmpShieldRelayControllerRelayEnabledDesiredLastState != relayStateDesired) {
		if(relayStateDesired && modHiAmpGeneralConfigHandle->HCUseRelay){
			if(modHiAmpGeneralConfigHandle->HCUsePrecharge)
				modHiAmpShieldRelayControllerRelayEnabledState = RELAY_CONTROLLER_PRECHARGING;
			else
				modHiAmpShieldRelayControllerRelayEnabledState = RELAY_CONTROLLER_ENABLED;
		}else{
			modHiAmpShieldRelayControllerRelayEnabledState = RELAY_CONTROLLER_OFF;
		}
		
		modHiAmpShieldRelayControllerRelayEnabledDesiredLastState = relayStateDesired;
	}
	
	// process new samples and go to precharged state if ok
	if(modHiAmpShieldRelayControllerRelayEnabledState == RELAY_CONTROLLER_PRECHARGING){
		if(mainBusVoltage >= batteryVoltage*PRECHARGE_PERCENTAGE)
			modHiAmpShieldRelayControllerRelayEnabledState = RELAY_CONTROLLER_PRECHARGED;
	}
	
	// process new samples and go to timeout state if voltage drops
	if(modHiAmpShieldRelayControllerRelayEnabledState == RELAY_CONTROLLER_ENABLED){
		if((mainBusVoltage <= batteryVoltage*PRECHARGE_PERCENTAGE) && modHiAmpGeneralConfigHandle->HCUsePrecharge)
			modHiAmpShieldRelayControllerRelayEnabledState = RELAY_CONTROLLER_TIMOUT;
	}
}

void modHiAmpShieldRelayControllerTask(void) {
	if(modHiAmpShieldRelayControllerRelayEnabledState != modHiAmpShieldRelayControllerRelayEnabledLastState){		// Only on change
		switch(modHiAmpShieldRelayControllerRelayEnabledState) {
			case RELAY_CONTROLLER_INIT:
				modHiAmpShieldRelayControllerRelayEnabledState = RELAY_CONTROLLER_OFF;
			case RELAY_CONTROLLER_OFF:
				modHiAmpShieldRelayControllerSetRelayOutputState(false,false);
			  modHiAmpPackStateHandle->hiLoadEnabled = false;
			  modHiAmpPackStateHandle->hiLoadPreChargeEnabled = false;
			  modHiAmpPackStateHandle->hiLoadPreChargeError = false;
				break;
			case RELAY_CONTROLLER_PRECHARGING:
			  modHiAmpShieldRelayControllerSetRelayOutputState(false,true);
			  modHiAmpPackStateHandle->hiLoadEnabled = false;
			  modHiAmpPackStateHandle->hiLoadPreChargeEnabled = true;
			  modHiAmpPackStateHandle->hiLoadPreChargeError = false;
			  modHiAmpShieldRelayTimeoutLastTick = HAL_GetTick();
			  break;
			case RELAY_CONTROLLER_PRECHARGED:
				modHiAmpShieldRelayControllerSetRelayOutputState(true,true);
			  modHiAmpPackStateHandle->hiLoadEnabled = true;
			  modHiAmpPackStateHandle->hiLoadPreChargeEnabled = true;
			  modHiAmpPackStateHandle->hiLoadPreChargeError = false;
			  modHiAmpShieldRelayTimeoutLastTick = HAL_GetTick();
				break;
			case RELAY_CONTROLLER_TIMOUT:
        modHiAmpShieldRelayControllerSetRelayOutputState(false,false);
			  modHiAmpPackStateHandle->hiLoadEnabled = false;
			  modHiAmpPackStateHandle->hiLoadPreChargeEnabled = false;
			  modHiAmpPackStateHandle->hiLoadPreChargeError = true;
			  modHiAmpShieldRelayTimeoutLastTick = HAL_GetTick();
				break;
			case RELAY_CONTROLLER_ENABLED:
				modHiAmpShieldRelayControllerSetRelayOutputState(true,false);
			  modHiAmpPackStateHandle->hiLoadEnabled = true;
			  modHiAmpPackStateHandle->hiLoadPreChargeEnabled = false;
			  modHiAmpPackStateHandle->hiLoadPreChargeError = false;
				break;
			default:
				modHiAmpShieldRelayControllerRelayEnabledState = RELAY_CONTROLLER_OFF;
				break;
		}
		modHiAmpShieldRelayControllerRelayEnabledLastState = modHiAmpShieldRelayControllerRelayEnabledState;
	}
	
	if(modHiAmpShieldRelayControllerRelayEnabledState == RELAY_CONTROLLER_PRECHARGING){
		// check delay and go to RELAY_CONTROLLER_TIMOUT if triggered
		if(modDelayTick1ms(&modHiAmpShieldRelayTimeoutLastTick,modHiAmpGeneralConfigHandle->timeoutHCPreCharge))
			modHiAmpShieldRelayControllerRelayEnabledState = RELAY_CONTROLLER_TIMOUT;
	}
	
	if(modHiAmpShieldRelayControllerRelayEnabledState == RELAY_CONTROLLER_PRECHARGED){
		// wait for main relay to enable and then disable pre charge
		if(modDelayTick1ms(&modHiAmpShieldRelayTimeoutLastTick,modHiAmpGeneralConfigHandle->timeoutHCRelayOverlap))
			modHiAmpShieldRelayControllerRelayEnabledState = RELAY_CONTROLLER_ENABLED;
	}	
	
	if(modHiAmpShieldRelayControllerRelayEnabledState == RELAY_CONTROLLER_TIMOUT){
		// check delay and go to RELAY_CONTROLLER_PRECHARGING if triggered
		if(modDelayTick1ms(&modHiAmpShieldRelayTimeoutLastTick,modHiAmpGeneralConfigHandle->timeoutHCPreChargeRetryInterval)){
			if(modHiAmpGeneralConfigHandle->HCUsePrecharge)
				modHiAmpShieldRelayControllerRelayEnabledState = RELAY_CONTROLLER_PRECHARGING;
			else
				modHiAmpShieldRelayControllerRelayEnabledState = RELAY_CONTROLLER_ENABLED;
		}
	}
}

void modHiAmpShieldRelayControllerSetRelayOutputState(bool newStateRelay, bool newStatePreCharge) {
	driverSWPCAL6416SetOutput(0,7,newStatePreCharge,false);																											// Set new state precharge relay
	driverSWPCAL6416SetOutput(0,6,newStateRelay,false);																												  // Set new state relay hold
	driverSWPCAL6416SetOutput(0,5,newStateRelay,true);																													// Set new state relay main and write to chip
}

void  modHiAmpShieldTemperatureHumidityMeasureTask(void) {
	// Static variable for SHT measure timer
	static uint32_t measureSHTStartLastTick          = 0;
	static driverSWSHT21MeasureType lastMeasuredType = TEMP;
	
	// Measure pack NTC's	(external)
	modHiAmpPackStateHandle->temperatures[4] = driverSWADC128D818GetTemperature(modHiAmpGeneralConfigHandle->NTC25DegResistance[modConfigNTCGroupHiAmpExt],modHiAmpGeneralConfigHandle->NTCTopResistor[modConfigNTCGroupHiAmpExt],modHiAmpGeneralConfigHandle->NTCBetaFactor[modConfigNTCGroupHiAmpExt],25.0f,0);
	modHiAmpPackStateHandle->temperatures[5] = driverSWADC128D818GetTemperature(modHiAmpGeneralConfigHandle->NTC25DegResistance[modConfigNTCGroupHiAmpExt],modHiAmpGeneralConfigHandle->NTCTopResistor[modConfigNTCGroupHiAmpExt],modHiAmpGeneralConfigHandle->NTCBetaFactor[modConfigNTCGroupHiAmpExt],25.0f,1);
	modHiAmpPackStateHandle->temperatures[6] = driverSWADC128D818GetTemperature(modHiAmpGeneralConfigHandle->NTC25DegResistance[modConfigNTCGroupHiAmpExt],modHiAmpGeneralConfigHandle->NTCTopResistor[modConfigNTCGroupHiAmpExt],modHiAmpGeneralConfigHandle->NTCBetaFactor[modConfigNTCGroupHiAmpExt],25.0f,2);
	modHiAmpPackStateHandle->temperatures[7] = driverSWADC128D818GetTemperature(modHiAmpGeneralConfigHandle->NTC25DegResistance[modConfigNTCGroupHiAmpExt],modHiAmpGeneralConfigHandle->NTCTopResistor[modConfigNTCGroupHiAmpExt],modHiAmpGeneralConfigHandle->NTCBetaFactor[modConfigNTCGroupHiAmpExt],25.0f,3);
	modHiAmpPackStateHandle->temperatures[8] = driverSWADC128D818GetTemperature(modHiAmpGeneralConfigHandle->NTC25DegResistance[modConfigNTCGroupHiAmpExt],modHiAmpGeneralConfigHandle->NTCTopResistor[modConfigNTCGroupHiAmpExt],modHiAmpGeneralConfigHandle->NTCBetaFactor[modConfigNTCGroupHiAmpExt],25.0f,4);
	modHiAmpPackStateHandle->temperatures[9] = driverSWADC128D818GetTemperature(modHiAmpGeneralConfigHandle->NTC25DegResistance[modConfigNTCGroupHiAmpExt],modHiAmpGeneralConfigHandle->NTCTopResistor[modConfigNTCGroupHiAmpExt],modHiAmpGeneralConfigHandle->NTCBetaFactor[modConfigNTCGroupHiAmpExt],25.0f,5);

	// Measure HiAmpshield NTC's (on the PCB)
	modHiAmpPackStateHandle->temperatures[10] = driverSWADC128D818GetTemperature(modHiAmpGeneralConfigHandle->NTC25DegResistance[modConfigNTCGroupHiAmpPCB],modHiAmpGeneralConfigHandle->NTCTopResistor[modConfigNTCGroupHiAmpPCB],modHiAmpGeneralConfigHandle->NTCBetaFactor[modConfigNTCGroupHiAmpPCB],25.0f,6);
	modHiAmpPackStateHandle->temperatures[11] = driverSWADC128D818GetTemperature(modHiAmpGeneralConfigHandle->NTC25DegResistance[modConfigNTCGroupHiAmpPCB],modHiAmpGeneralConfigHandle->NTCTopResistor[modConfigNTCGroupHiAmpPCB],modHiAmpGeneralConfigHandle->NTCBetaFactor[modConfigNTCGroupHiAmpPCB],25.0f,7);
		
	// Read Temp and Humidity from SHT21 when ready
	if(driverSWSHT21PollMeasureReady()){
		modHiAmpPackStateHandle->temperatures[12] = driverSWSHT21GetTemperature();
		modHiAmpPackStateHandle->humidity         = driverSWSHT21GetHumidity();
	}
	
	if(modDelayTick1ms(&measureSHTStartLastTick,500)){
		driverSWSHT21StartMeasurement(lastMeasuredType);
		
		if(lastMeasuredType == TEMP)																																							// Toggle between SHT21 sensor modes
			lastMeasuredType = HUMIDITY;
		else
			lastMeasuredType = TEMP;
	}
}

void  modHiAmpShieldResetSensors(void) {	
	modHiAmpPackStateHandle->hiCurrentLoadVoltage         = 0.0f;
	modHiAmpPackStateHandle->hiCurrentLoadCurrent         = 0.0f;
	modHiAmpPackStateHandle->hiCurrentLoadPower           = 0.0f;
	modHiAmpPackStateHandle->auxVoltage                   = 0.0f;
	modHiAmpPackStateHandle->auxCurrent                   = 0.0f;
	modHiAmpPackStateHandle->auxPower                     = 0.0f;
	modHiAmpPackStateHandle->aux0LoadIncorrect            = false;
	modHiAmpPackStateHandle->aux1LoadIncorrect            = false;
	modHiAmpPackStateHandle->auxDCDCOutputOK              = false;
	modHiAmpPackStateHandle->temperatures[4]              = 200.0f;
	modHiAmpPackStateHandle->temperatures[5]              = 200.0f;
	modHiAmpPackStateHandle->temperatures[6]              = 200.0f;
	modHiAmpPackStateHandle->temperatures[7]              = 200.0f;
	modHiAmpPackStateHandle->temperatures[8]              = 200.0f;
	modHiAmpPackStateHandle->temperatures[9]              = 200.0f;
	modHiAmpPackStateHandle->temperatures[10]             = 200.0f;
	modHiAmpPackStateHandle->temperatures[11]             = 200.0f;
	modHiAmpPackStateHandle->temperatures[12]             = 200.0f;
	modHiAmpPackStateHandle->humidity                     = 0.0f;
	modHiAmpPackStateHandle->hiLoadEnabled                = false;
	modHiAmpPackStateHandle->hiLoadPreChargeEnabled       = false;
	modHiAmpPackStateHandle->hiLoadPreChargeError         = false;
	modHiAmpPackStateHandle->IOIN1                        = false;
	modHiAmpPackStateHandle->IOOUT0                       = false;
	modHiAmpPackStateHandle->FANStatus.FANError           = false;
	modHiAmpPackStateHandle->FANStatus.FANOK              = false;
	modHiAmpPackStateHandle->FANStatus.FANSpeedRPM[0]     = 0;
	modHiAmpPackStateHandle->FANStatus.FANSpeedRPM[1]     = 0;
	modHiAmpPackStateHandle->FANStatus.FANSpeedRPM[2]     = 0;
	modHiAmpPackStateHandle->FANStatus.FANSpeedRPM[3]     = 0;
}

