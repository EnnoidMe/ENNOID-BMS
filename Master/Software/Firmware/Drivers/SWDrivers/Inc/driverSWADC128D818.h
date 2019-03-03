#include "stm32f3xx_hal.h"
#include "stdbool.h"
#include "math.h"
#include "modDelay.h"
#include "driverHWI2C1.h"

#define ADC128D818_ADDRES	0x1D

void driverSWADC128D818Init(void);
uint16_t driverSWADC128D818GetChannel(uint8_t adcChannel);
float driverSWADC128D818GetTemperature(uint32_t ntcNominal,uint32_t ntcSeriesResistance,uint16_t ntcBetaFactor, float ntcNominalTemp,uint8_t ntcChannel);
