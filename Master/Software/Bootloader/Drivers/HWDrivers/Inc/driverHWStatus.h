#ifndef __DRIVERHWSTATUS_H
#define __DRIVERHWSTATUS_H

#include "stm32f3xx_hal.h"
#include "stdbool.h"
#include "modDelay.h"

#define NoOfSTATs				3
#define MAX_BLINKSHORT_CYCLES	20
#define MAX_BLINKLONG_CYCLES	5

typedef struct {
	GPIO_TypeDef* Port;
	uint32_t ClkRegister;
	uint32_t Pin;
} STATPortStruct;

extern const STATPortStruct STATPorts[NoOfSTATs];

typedef enum {
	STAT_LED_DEBUG = 0,
	STAT_LED_POWER,
	STAT_BUZZER
} STATIDTypedef;

typedef enum
{
	STAT_RESET = 0,
	STAT_SET,
	STAT_FLASH,
	STAT_FLASH_FAST,
	STAT_FLASH_VERY_FAST,
	STAT_BLINKSHORT,
	STAT_BLINKLONG,	
	STAT_BLINKSHORTLONG_100_20,
	STAT_BLINKSHORTLONG_1000_4,	
	STAT_ON,
	STAT_OFF
} STATStateTypedef;

typedef struct {
	STATStateTypedef 	State;
	uint32_t					Count;
} STATStatusStruct;


// Public functions
void driverHWStatusInit(void);
void driverHWSetOutput(STATIDTypedef LEDType,STATStateTypedef State);

#endif
