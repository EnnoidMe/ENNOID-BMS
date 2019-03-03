#include "driverSWUART2.h"

libRingBufferTypedef *driverSWUART2OutputBuffer;

void driverSWUART2Init(uint32_t baudRate) {
	driverSWUART2OutputBuffer = libRingBufferNew(sizeof(uint8_t),RINGBUFFERSIZE);	// Make new output buffer	
	
	if(!driverSWUART2OutputBuffer)																								// Check if buffer pointer is generated
		while(true);																																// Out of memory error
	
	driverHWUART2Init(baudRate);																									// Initialize serial port and pass function that should be called when a byte is received
};

char driverSWUART2PutCharInOutputBuffer(char character, FILE *stream) {
	// TODO: If buffer is full, first send a character out, then place new char. This should however never happen
	if(!driverSWUART2OutputBuffer->isFull(driverSWUART2OutputBuffer)) {
		driverSWUART2OutputBuffer->add(driverSWUART2OutputBuffer,&character);
	}else{
		driverSWUART2Task();
		driverSWUART2OutputBuffer->add(driverSWUART2OutputBuffer,&character);
	}
	return 0;
};

bool driverSWUART2Task(void) {
	char outputChar;
	
	if(!driverSWUART2OutputBuffer->isEmpty(driverSWUART2OutputBuffer)){						// Check if there is data in the ouput buffer
		driverSWUART2OutputBuffer->pull(driverSWUART2OutputBuffer,&outputChar);			// Pull the data from ouput buffer
		driverHWUART2SendChar(outputChar);																					// And send it to the uart
	}
	
	return !driverSWUART2OutputBuffer->isEmpty(driverSWUART2OutputBuffer);
};
