#include "modMessage.h"

uint32_t consoleStatusLastTick;
modPowerElectricsPackStateTypedef *modMessagePackStateHandle;
modConfigGeneralConfigStructTypedef *modPowerMessageGeneralConfigHandle;

void modMessageInit(modPowerElectricsPackStateTypedef *packState, modConfigGeneralConfigStructTypedef *generalConfigPointer) {
	modMessagePackStateHandle = packState;
	modPowerMessageGeneralConfigHandle = generalConfigPointer;
	
	driverSWUART2Init(115200);																								// Configure the UART driver
};

void modMessageTask(void) {
	while(driverSWUART2Task()){};
		
	if(modDelayTick1ms(&consoleStatusLastTick,2000))
		modMessageQueMessage(MESSAGE_DEBUG,"CVLow: %1.3fV, CVHigh: %1.3fV, VPack: %.3fV\r\n",modMessagePackStateHandle->cellVoltageLow,modMessagePackStateHandle->cellVoltageHigh,modMessagePackStateHandle->packVoltage);
};

void modMessageQueMessage(modMessageMessageTypeDef messageType, const char* format, ...) {
	uint32_t timeStamp = HAL_GetTick();
	switch(messageType) {
		case MESSAGE_DEBUG:
			fprintf(&driverSWUART2IOStream,"[%10.03f] [DEBUG] ",((float)timeStamp)/1000);
			break;
		case MESSAGE_ERROR:
			fprintf(&driverSWUART2IOStream,"[%10.03f] [ERROR] ",((float)timeStamp)/1000);
			break;
		case MESSAGE_NORMAL:
			fprintf(&driverSWUART2IOStream,"[%10.03f] ",((float)timeStamp)/1000);
			break;
	};
	
	va_list argptr;
	va_start(argptr, format);
	vfprintf(&driverSWUART2IOStream, format, argptr);
	va_end(argptr);
	
	while(driverSWUART2Task());
};
