#include "stm32f3xx_hal.h"
#include "stdbool.h"
#include "driverHWI2C1.h"

#define PCAL6461_ADDRES 0x20

void driverSWPCAL6416Init(uint8_t DDR0, uint8_t DDR1,uint8_t PUER0,uint8_t PUER1,uint8_t PUDR0,uint8_t PUDR1);
void driverSWPCAL6416SetOutput(uint8_t port, uint8_t pin, bool newState, bool writeToChip);
bool driverSWPCAL6416GetInput(uint8_t port, uint8_t pin, bool readFromChip);
