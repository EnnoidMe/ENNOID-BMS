#ifndef __DRIVERHWPOWERSTATE_H
#define __DRIVERHWPOWERSTATE_H

#include "generalDefines.h"
#include "stm32f3xx_hal.h"
#include "stdbool.h"

#define NoOfPowersSTATs				3

typedef struct {
	GPIO_TypeDef* Port;
	uint32_t ClkRegister;
	uint32_t Pin;
	uint32_t Mode;
	uint32_t Pull;
} PowerStatePortStruct;

extern const PowerStatePortStruct driverHWPorts[NoOfPowersSTATs];

typedef enum {
	P_STAT_POWER_ENABLE = 0,
	P_STAT_BUTTON_INPUT,
	P_STAT_CHARGE_DETECT
} PowerStateIDTypedef;

typedef enum {
	P_STAT_RESET = 0,
	P_STAT_SET
} PowerStateStateTypedef;

void driverHWPowerStateInit(void);
void driverHWPowerStateSetOutput(PowerStateIDTypedef inputPort, PowerStateStateTypedef newState);
bool driverHWPowerStateReadInput(PowerStateIDTypedef inputPort);

#endif
