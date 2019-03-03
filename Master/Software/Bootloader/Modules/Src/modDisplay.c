#include "modDisplay.h"

modDisplayInfoType modDisplayCurrentState;
uint32_t modDisplayLastRefresh;
uint32_t modDisplayStartupDelay;

extern modDisplayDataTypedef modDisplayData;

void modDisplayInit(void) {
	modDisplayStartupDelay = HAL_GetTick();
	libGraphicsInit(SSD1306_LCDWIDTH,SSD1306_LCDHEIGHT);
	while(!modDelayTick1ms(&modDisplayStartupDelay,STARTUPDELAY)) {};
	driverSWSSD1306Init(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
			
	modDisplayCurrentState = DISP_MODE_OFF;												//  Default content is nothing
	modDisplayLastRefresh = HAL_GetTick();
};

void modDisplayShowInfo(modDisplayInfoType newState, modDisplayDataTypedef modDisplayData) {
	static modDisplayDataTypedef modDisplayDataLast;
	
	if((modDisplayCurrentState != newState) || memcmp(&modDisplayDataLast,&modDisplayData,sizeof(modDisplayDataTypedef))) {											// Different state than last state?
		memcpy(&modDisplayDataLast,&modDisplayData,sizeof(modDisplayDataTypedef));
		switch(newState) {
			case DISP_MODE_OFF:
				driverSWSSD1306ClearDisplay();
				break;
			case DISP_MODE_BOOTLOADER:
				driverSWSSD1306ClearDisplay();
				libGraphicsSetTextSize(1);
				libGraphicsSetTextColor_0(WHITE);
				libGraphicsSetCursor(0,0);
			
				libGraphicsWrite('B');  
				libGraphicsWrite('o');  
				libGraphicsWrite('o');  
				libGraphicsWrite('t');  
				libGraphicsWrite('l');  
				libGraphicsWrite('o');  
				libGraphicsWrite('a');  
				libGraphicsWrite('d');  
				libGraphicsWrite('e');  
				libGraphicsWrite('r');   
				libGraphicsWrite(' ');   
				libGraphicsWrite('V');   
				libGraphicsWrite('0');   
				libGraphicsWrite('.');   
				libGraphicsWrite('1');   
				break;
				
			default:
				break;
		};
	}
	
	modDisplayCurrentState = newState;
};

void modDisplayTask(void) {
	driverSWSSD1306DisplayAsyncEfficient();
};
