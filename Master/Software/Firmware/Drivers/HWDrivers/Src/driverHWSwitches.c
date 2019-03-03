#include "driverHWSwitches.h"

const driverHWSwitchesPortStruct driverHWSwitchesPorts[NoOfSwitches] =			// Hold all status configuration data
{
	{GPIOB,RCC_AHBENR_GPIOAEN,GPIO_PIN_2,GPIO_MODE_OUTPUT_PP,GPIO_NOPULL},		// SWITCH_DRIVER
	{GPIOB,RCC_AHBENR_GPIOCEN,GPIO_PIN_0,GPIO_MODE_OUTPUT_PP,GPIO_NOPULL},		// SWITCH_CHARGE
	{GPIOB,RCC_AHBENR_GPIOCEN,GPIO_PIN_11,GPIO_MODE_OUTPUT_PP,GPIO_NOPULL},		// SWITCH_PRECHARGE
	{GPIOB,RCC_AHBENR_GPIOCEN,GPIO_PIN_10,GPIO_MODE_OUTPUT_PP,GPIO_NOPULL}		// SWITCH_DISCHARGE
};

void driverHWSwitchesInit(void) {
	GPIO_InitTypeDef switchPortHolder;
	uint8_t SwitchPointer;
	
	for(SwitchPointer = 0; SwitchPointer < NoOfSwitches; SwitchPointer++) {
		RCC->AHBENR |= driverHWSwitchesPorts[SwitchPointer].ClkRegister;				// Enable clock de desired port
		switchPortHolder.Mode = driverHWSwitchesPorts[SwitchPointer].Mode;			// Push pull output
		switchPortHolder.Pin = driverHWSwitchesPorts[SwitchPointer].Pin;				// Points to status pin
		switchPortHolder.Pull = driverHWSwitchesPorts[SwitchPointer].Pull;			// Pullup
		switchPortHolder.Speed = GPIO_SPEED_HIGH;																// GPIO clock speed
		HAL_GPIO_Init(driverHWSwitchesPorts[SwitchPointer].Port,&switchPortHolder);// Perform the IO init 
	};
};

void driverHWSwitchesSetSwitchState(driverHWSwitchesIDTypedef switchID, driverHWSwitchesStateTypedef newState) {
	HAL_GPIO_WritePin(driverHWSwitchesPorts[switchID].Port,driverHWSwitchesPorts[switchID].Pin,(GPIO_PinState)newState); // Set desired pin to desired state 
};

void driverHWSwitchesDisableAll(void) {
	uint8_t SwitchPointer;
	for(SwitchPointer = 0; SwitchPointer < NoOfSwitches; SwitchPointer++) {
		HAL_GPIO_WritePin(driverHWSwitchesPorts[SwitchPointer].Port,driverHWSwitchesPorts[SwitchPointer].Pin,(GPIO_PinState)SWITCH_RESET); // Set desired pin to desired state 
	};
};

bool driverHWSwitchesGetMonitorEnabledState(void) {
	return (bool) HAL_GPIO_ReadPin(driverHWSwitchesPorts[SWITCH_DRIVER].Port,driverHWSwitchesPorts[SWITCH_DRIVER].Pin);
};

bool driverHWSwitchesGetSwitchState(driverHWSwitchesIDTypedef switchID) {
	return (bool) HAL_GPIO_ReadPin(driverHWSwitchesPorts[switchID].Port,driverHWSwitchesPorts[switchID].Pin); // Set desired pin to desired state 
};


