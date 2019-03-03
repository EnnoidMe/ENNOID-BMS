#include "driverSWADC128D818.h"

uint32_t driverSWADC128StartupDelayLastTick;

void driverSWADC128D818Init(void){
	uint32_t maxDelayIntervals = 20;
	bool adcBusy = true;
	driverSWADC128StartupDelayLastTick = HAL_GetTick();
	
	uint8_t registerPointer = 0x0C;
	uint8_t readData = 0x00;
	
	// Check busy register
	while(maxDelayIntervals && adcBusy) {
		if(modDelayTick1ms(&driverSWADC128StartupDelayLastTick,10) && maxDelayIntervals){
			maxDelayIntervals--;
			
			driverHWI2C1Write(ADC128D818_ADDRES,false,&registerPointer,1);
			driverHWI2C1Read(ADC128D818_ADDRES,&readData,1);
			
			if(!(readData & 0x01))
				adcBusy = false;
		}
	}
	
	uint8_t configBytes[] = {0x00,0x08};																															// Configuration register -> disable ADC conversion and reset registers
	driverHWI2C1Write(ADC128D818_ADDRES,false,configBytes,sizeof(configBytes));
		
	configBytes[0] = 0x0B;																																						// Advanced config register Mode 1 + ext ref
	configBytes[1] = 0x03;
	driverHWI2C1Write(ADC128D818_ADDRES,false,configBytes,sizeof(configBytes));
	
	configBytes[0] = 0x07;																																						// Configuration rate register -> continious conversion
	configBytes[1] = 0x01;
	driverHWI2C1Write(ADC128D818_ADDRES,false,configBytes,sizeof(configBytes));
	
	configBytes[0] = 0x08;																																						// Channel disable register -> disable channel 6
	configBytes[1] = 0x00;
	driverHWI2C1Write(ADC128D818_ADDRES,false,configBytes,sizeof(configBytes));
	
	configBytes[0] = 0x03;																																						// Interrupt enable register -> disable all interrupts
	configBytes[1] = 0xFF;
	driverHWI2C1Write(ADC128D818_ADDRES,false,configBytes,sizeof(configBytes));
	
	configBytes[0] = 0x00;																																						// Configuration register -> enable ADC conversion
	configBytes[1] = 0x01;
	driverHWI2C1Write(ADC128D818_ADDRES,false,configBytes,sizeof(configBytes));
	
	
	// Now ready to read samples
}

uint16_t driverSWADC128D818GetChannel(uint8_t adcChannel){	
	if(adcChannel > 7)
		return 0;
	
	uint8_t registerPointer = 0x20 + adcChannel;
	uint8_t readBytes[2];
	driverHWI2C1Write(ADC128D818_ADDRES,false,&registerPointer,1);
	driverHWI2C1Read(ADC128D818_ADDRES,readBytes,2);
	
	return (readBytes[0] << 4) | (readBytes[1] >> 4);
}

float driverSWADC128D818GetTemperature(uint32_t ntcNominal, uint32_t ntcSeriesResistance,uint16_t ntcBetaFactor, float ntcNominalTemp,uint8_t ntcChannel){
	static uint32_t adcValue = 0;
	static float scalar;
	static float steinhart;
	
	adcValue = driverSWADC128D818GetChannel(ntcChannel);

  scalar = 4095.0f / (float)adcValue - 1.0f;
  scalar = (float)ntcSeriesResistance / scalar;
  steinhart = scalar / (float)ntcNominal;               // (R/Ro)
  steinhart = log(steinhart);                           // ln(R/Ro)
  steinhart /= (float)ntcBetaFactor;                    // 1/B * ln(R/Ro)
  steinhart += 1.0f / (ntcNominalTemp + 273.15f);       // + (1/To)
  steinhart = 1.0f / steinhart;                         // Invert
  steinhart -= 273.15f;                                 // convert to degree
	
	if(steinhart < -100.0f)
		steinhart = 200;
	
  return steinhart;
}
