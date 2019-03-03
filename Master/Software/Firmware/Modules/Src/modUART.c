#include "modUART.h"

uint32_t consoleStatusLastTick;

void modUARTInit(void) {	
	libPacketInit(modUARTSendPacket, modUARTProcessPacket, PACKET_HANDLER_UART);
	
	driverSWUART2Init(115200);																								// Configure the UART driver
};

void modUARTTask(void) {
	char inputChar;
	
	while(driverSWUART2Task()){};
		
	if(driverHWUART2GetChar(&inputChar))
		libPacketProcessByte(inputChar,PACKET_HANDLER_UART);
};

void modUARTSendPacketWrapper(unsigned char *data, unsigned int len) {
	libPacketSendPacket(data, len, PACKET_HANDLER_UART);
}

void modUARTProcessPacket(unsigned char *data, unsigned int len) {
	modCommandsSetSendFunction(modUARTSendPacketWrapper);
	modCommandsProcessPacket(data, len);
}

void modUARTSendPacket(unsigned char *data, unsigned int len) {
	uint32_t outputCharPointer;
	
	for(outputCharPointer = 0; outputCharPointer < len; outputCharPointer++) {
	  driverHWUART2SendChar(data[outputCharPointer]);
	}
}
