#include "driverSWSSD1306.h"
#include "modDelay.h"
#include "libGraphics.h"

#define REFRESHTIMOUT	5000																	// Interval between complete sync between desired and actual display content					
#define STARTUPDELAY	5																		// Startup delay, wait this amount of time before talking to the display

#ifndef MODDISPLAY_H_
#define MODDISPLAY_H_

typedef struct {
	float Empty;
} modDisplayDataTypedef;

typedef enum {
	DISP_MODE_OFF = 0,
	DISP_MODE_BOOTLOADER
} modDisplayInfoType;

void modDisplayInit(void);
void modDisplayShowInfo(modDisplayInfoType newState,modDisplayDataTypedef modDisplayData);
void modDisplayTask(void);

#endif
