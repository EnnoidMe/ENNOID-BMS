#include "stm32f3xx_hal.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include "driverHWUART2.h"
#include "libRingBuffer.h"
#include "libFileStream.h"

#define RINGBUFFERSIZE									1024

void driverSWUART2Init(uint32_t baudRate);
char driverSWUART2PutCharInOutputBuffer(char character, FILE *stream);
bool driverSWUART2Task(void);
