#include "stm32f3xx_hal.h"
#include "stdbool.h"
#include "string.h"

#define RX_UART_BUFFER_SIZE					1024

void driverHWUART2Init(uint32_t baudRate);
void driverHWUART2SendChar(uint8_t character);
bool driverHWUART2GetChar(char *character);
