#include "modPowerState.h"

modConfigGeneralConfigStructTypedef *modPowerStateGeneralConfigHandle;

bool modPowerStatePulsePowerDownDesired;
bool modPowerStateForceOnDesired;
bool modPowerStateButtonPressedVar;
bool modPowerStateLastButtonPressedVar;
bool modPowerStateLastButtonFirstPress;
bool modPowerStateButtonPulsToggleMode;
bool modPowerStatePowerModeDirectHCDelay;
uint32_t modPowerStateButtonPressedDuration;
uint32_t modPowerStateButtonPressedTimeStamp;
uint32_t modPowerStateStartupDelay;
uint32_t modPowerStatePowerDownTimeout;

void modPowerStateInit(PowerStateStateTypedef desiredPowerState) {
	uint32_t startupDelay = HAL_GetTick();
	modPowerStateStartupDelay = HAL_GetTick();
	modPowerStatePowerDownTimeout = HAL_GetTick();
	modPowerStatePulsePowerDownDesired = false;
	modPowerStateForceOnDesired  = false;
	modPowerStateButtonPressedVar = true;
	modPowerStateButtonPulsToggleMode = true;
	modPowerStateButtonPressedDuration = 0;
	modPowerStateButtonPressedTimeStamp = 0;
	
	driverHWPowerStateInit();
	
	while(!modDelayTick1ms(&modPowerStateStartupDelay,20));										// Needed for power button signal to reach uC
	modPowerStateLastButtonFirstPress = modPowerStateLastButtonPressedVar = driverHWPowerStateReadInput(P_STAT_BUTTON_INPUT);
	
	if(!modPowerStateLastButtonFirstPress) {
		while(!modDelayTick1ms(&modPowerStateStartupDelay,100));
	}

	modPowerStateSetState(desiredPowerState);
};



void modPowerStateSetConfigHandle(modConfigGeneralConfigStructTypedef *generalConfigPointer) {
	modPowerStateGeneralConfigHandle = generalConfigPointer;
	
	modPowerStateButtonPulsToggleMode   = modPowerStateGeneralConfigHandle->pulseToggleButton;
	modPowerStatePowerModeDirectHCDelay = modPowerStateGeneralConfigHandle->togglePowerModeDirectHCDelay;
}

void modPowerStateTask(void) {
	bool tempButtonPressed = driverHWPowerStateReadInput(P_STAT_BUTTON_INPUT);
	
	if(modPowerStateLastButtonPressedVar != tempButtonPressed) {
		if(modPowerStateLastButtonPressedVar){ 																	// If is was high and now low (actuated)
			modPowerStateLastButtonFirstPress = false;
		}else{ 																																	// If is was low and now high (non actuated)
			modPowerStateButtonPressedTimeStamp = HAL_GetTick();
		}
		modPowerStateLastButtonPressedVar = tempButtonPressed;
	}
	
	if(tempButtonPressed) {
		modPowerStateButtonPressedDuration = HAL_GetTick() - modPowerStateButtonPressedTimeStamp;
	
		if((modPowerStateButtonPressedDuration >= POWERBUTTON_POWERDOWN_THRESHOLD_TIME) && (modPowerStateLastButtonFirstPress == false) && modPowerStateGeneralConfigHandle->pulseToggleButton) {
			modPowerStatePulsePowerDownDesired = true;
			modPowerStateButtonPressedDuration = 0;
		}
		
		if((modPowerStateButtonPressedDuration >= POWERBUTTON_FORCEON_THRESHOLD_TIME) && (modPowerStateLastButtonFirstPress == true) && modPowerStateGeneralConfigHandle->allowForceOn && modPowerStateGeneralConfigHandle->pulseToggleButton) {
			modPowerStateForceOnDesired = true;
			modPowerStateButtonPressedDuration = 0;
		}
		
		if((modPowerStateButtonPressedDuration > POWERBUTTON_DEBOUNCE_TIME) && (modPowerStateLastButtonFirstPress == false))
			modPowerStateButtonPressedVar = true;
		
		modPowerStatePowerDownTimeout = HAL_GetTick();
	}else{
		if(modDelayTick1ms(&modPowerStatePowerDownTimeout,500))
			modPowerStateButtonPressedVar = false;
	}
};

bool modPowerStateGetButtonPressedState(void) {
	return modPowerStateButtonPressedVar;
};

bool modPowerStateChargerDetected(void) {
	static bool chargeDetect = false;
	chargeDetect = driverHWPowerStateReadInput(P_STAT_CHARGE_DETECT);
	return chargeDetect;
};

bool modPowerStatePowerdownRequest(void) {
	bool returnValue = false;
	
	if(modPowerStateGeneralConfigHandle->pulseToggleButton){
		returnValue = modPowerStatePulsePowerDownDesired;
	}else{
		if(modPowerStateGeneralConfigHandle->togglePowerModeDirectHCDelay){
			returnValue = !modPowerStateButtonPressedVar;
		}else{
			returnValue = false;
		}
	}
	
	return returnValue;
};

bool modPowerStateForceOnRequest(void) {
	static bool firstTrigger = true;
	
	if(modPowerStateForceOnDesired && firstTrigger){
		modPowerStateForceOnDesired = false;
		firstTrigger = false;
		return true;
	}else{
		return false;
	}
};

void modPowerStateSetState(PowerStateStateTypedef newState) {
	driverHWPowerStateSetOutput(P_STAT_POWER_ENABLE,newState);
};

bool modPowerStateButtonPressedOnTurnon(void) {
	return modPowerStateLastButtonFirstPress;
};
