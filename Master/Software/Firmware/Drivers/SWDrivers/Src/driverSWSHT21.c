#include "driverSWSHT21.h"

const uint16_t driverSWSHT21PolyNomial = 0x131;  // P(x)=x^8+x^5+x^4+1 = 100110001

uint32_t driverSWSHT21LastMeasureStart;
driverSWSHT21MeasureType driverSWSHT21LastMeasureType;
float driverSWSHT21LastTemperature;
float driverSWSHT21LastHumidity;

void driverSWSHT21Init(void) {
	// Init measure
	driverSWSHT21LastMeasureType = NONE;
	driverSWSHT21LastMeasureStart = HAL_GetTick();
	
	driverSWSHT21LastTemperature = 0.0f;
	driverSWSHT21LastHumidity = 0.0f;
}

void driverSWSHT21StartMeasurement(driverSWSHT21MeasureType measureType) {
  driverSWSHT21LastMeasureStart = HAL_GetTick();
	
	uint8_t commandBytes[] = {DRIVERSHT21_TRIGGER_T_MEASUREMENT_NHM};									// make command package, init with temp but that will be changed later
	
	if(driverSWSHT21LastMeasureType == NONE){																					// Only start conversion when there is no conversion
		switch(measureType){
			case TEMP:
				commandBytes[0] = DRIVERSHT21_TRIGGER_T_MEASUREMENT_NHM;										// start a no hold master Temperature conversion
				driverHWI2C1Write(DRIVERSHT21_ADDR,false,commandBytes,sizeof(commandBytes));
				break;
			case HUMIDITY:
				commandBytes[0] = DRIVERSHT21_TRIGGER_RH_MEASUREMENT_NHM;										// start a no hold master Humidity conversion
				driverHWI2C1Write(DRIVERSHT21_ADDR,false,commandBytes,sizeof(commandBytes));
				break;
			default:
				// No action
				break;
		}
		
		driverSWSHT21LastMeasureType = measureType;
	}
}

bool driverSWSHT21PollMeasureReady(void) {
	uint16_t returnedSensorValue;
	uint8_t returnedData[3];
	bool returnValue = false;
	
	if(driverSWSHT21LastMeasureType != NONE) {																				// Check wether there is a conversion in progress
		if(driverHWI2C1Read(DRIVERSHT21_ADDR,returnedData,3) == HAL_OK){								// Try to read data from sensor
			if(driverSWSHT21CalculateCRC(returnedData,2,returnedData[2])){								// When read is a succes check checksum
				returnedSensorValue = (returnedData[0] << 8) | returnedData[1];							// Compose ADC value
				
				switch(driverSWSHT21LastMeasureType){																				// Switch on type 
					case TEMP:
						driverSWSHT21LastTemperature = driverSWSHT21CalculateTemperature(returnedSensorValue); // Calculate temperature
					  returnValue = true;
						break;
					case HUMIDITY:
						driverSWSHT21LastHumidity = driverSWSHT21CalculateHumidity(returnedSensorValue); // Calculate humidity
					  returnValue = true;
						break;
					default:
						break;
				}
				
				driverSWSHT21LastMeasureType = NONE;																				// Reset conversion progress to NONE
			}
		}
	}
	
	if(modDelayTick1ms(&driverSWSHT21LastMeasureStart,500))														// If no result within 500ms reset converstion progress
		driverSWSHT21LastMeasureType = NONE;
	
	return returnValue;
}

float driverSWSHT21CalculateTemperature(uint16_t rawTemperature) {
  float temperatureC;                                                  // variable for result
  rawTemperature &= ~0x0003;                                           // clear bits [1..0] (status bits)
  temperatureC= -46.85 + 175.72/65536 *(float)rawTemperature;          //T= -46.85 + 175.72 * ST/2^16
  return temperatureC;
}

float driverSWSHT21GetTemperature(void) {
	return driverSWSHT21LastTemperature;
}

float driverSWSHT21CalculateHumidity(uint16_t rawHumidity) {
  float humidityRH;                                                    // variable for result
  rawHumidity &= ~0x0003;                                              // clear bits [1..0] (status bits)
  humidityRH = -6.0 + 125.0/65536 * (float)rawHumidity;                // RH= -6 + 125 * SRH/2^16
  return humidityRH;
}

float driverSWSHT21GetHumidity(void) {
	return driverSWSHT21LastHumidity;
}

uint8_t driverSWSHT21CalculateCRC(uint8_t data[], uint8_t no_of_bytes, uint8_t checksum) {
	uint8_t crc = 0;	
	uint8_t byteCtr;

	//calculates 8-Bit checksum with given polynomial
	for(byteCtr = 0; byteCtr < no_of_bytes; ++byteCtr) {
	  crc ^= (data[byteCtr]);
	  
		for(uint8_t bit = 8; bit > 0; --bit) {
			if(crc & 0x80)
				crc = (crc << 1) ^ driverSWSHT21PolyNomial;
		  else 
			  crc = (crc << 1);
	  }
	}
	
	if (crc == checksum)
		return 1;
	else
		return 0;
}
