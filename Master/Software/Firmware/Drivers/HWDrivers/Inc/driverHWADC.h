#include "stm32f3xx_hal.h"
#include "stdbool.h"
#include "math.h"
#include "generalDefines.h"

#define NoOfADCPorts 2
#define NoOfAverages 10

typedef struct {
	GPIO_TypeDef* Port;
	uint32_t ClkRegister;
	uint32_t Pin;
	uint32_t Mode;
	uint32_t Pull;
	uint32_t Alternate;
} driverHWADCPortStruct;

void driverHWADCInit(void);
void driverHWADCSetInputChannel(ADC_HandleTypeDef* hadc, uint32_t inputChannel);
bool driverHWADCGetLoadVoltage(float *loCurrentLoadVoltage);
bool driverHWADCGetNTCValue(float *ntcValue, uint32_t ntcNominal, uint32_t ntcSeriesResistance, uint16_t ntcBetaFactor, float ntcNominalTemp);
