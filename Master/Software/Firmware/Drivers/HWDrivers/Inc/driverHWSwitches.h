#include "stm32f3xx_hal.h"
#include "stdbool.h"

#define NoOfSwitches				4

typedef struct {
	GPIO_TypeDef* Port;
	uint32_t ClkRegister;
	uint32_t Pin;
	uint32_t Mode;
	uint32_t Pull;
} driverHWSwitchesPortStruct;

extern const driverHWSwitchesPortStruct driverHWSwitchesPorts[NoOfSwitches];

typedef enum {
	SWITCH_DRIVER = 0,
	SWITCH_CHARGE,
	SWITCH_PRECHARGE,
	SWITCH_DISCHARGE
} driverHWSwitchesIDTypedef;

typedef enum {
	SWITCH_RESET = 0,
	SWITCH_SET,
} driverHWSwitchesStateTypedef;

void driverHWSwitchesInit(void);
void driverHWSwitchesSetSwitchState(driverHWSwitchesIDTypedef switchID, driverHWSwitchesStateTypedef newState);
void driverHWSwitchesDisableAll(void);
bool driverHWSwitchesGetMonitorEnabledState(void);
bool driverHWSwitchesGetSwitchState(driverHWSwitchesIDTypedef switchID);
