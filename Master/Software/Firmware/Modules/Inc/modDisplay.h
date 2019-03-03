#include "driverSWSSD1306.h"
#include "modDelay.h"
#include "libGraphics.h"
#include "libLogos.h"

#define REFRESHTIMOUT	5000																	// Interval between complete sync between desired and actual display content					
#define STARTUPDELAY	5																		// Startup delay, wait this amount of time before talking to the display

#ifndef MODDISPLAY_H_
#define MODDISPLAY_H_

typedef struct {
	float StateOfCharge;
	float LowestCellVoltage;
	float HighestCellVoltage;
	float AverageCellVoltage;
} modDisplayDataTypedef;

typedef enum {
	DISP_MODE_OFF = 0,
	DISP_MODE_SPLASH,	
	DISP_MODE_LOAD,
	DISP_MODE_CHARGE,
	DISP_MODE_POWEROFF,
	DISP_MODE_ERROR,
	DISP_MODE_ERROR_PRECHARGE,
	DISP_MODE_EXTERNAL,
	DISP_MODE_BATTERY_DEAD,
	DISP_MODE_BALANCING,
	DISP_MODE_CHARGED,
	DISP_MODE_FORCED_ON
} modDisplayInfoType;

void modDisplayInit(void);
void modDisplayShowInfo(modDisplayInfoType newState,modDisplayDataTypedef modDisplayData);
void modDisplayTask(void);

#endif
