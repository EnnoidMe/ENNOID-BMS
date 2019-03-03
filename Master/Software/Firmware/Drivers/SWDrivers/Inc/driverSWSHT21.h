#ifndef __DRIVERSWSHT21_H
#define __DRIVERSWSHT21_H

#include "stdbool.h"
#include "modDelay.h"
#include "driverHWI2C1.h"

#define DRIVERSHT21_ADDR   0x40
#define DRIVERSHT21_TRIGGER_T_MEASUREMENT_HM 0XE3   // command trig. temp meas. hold master
#define DRIVERSHT21_TRIGGER_RH_MEASUREMENT_HM 0XE5  // command trig. hum. meas. hold master
#define DRIVERSHT21_TRIGGER_T_MEASUREMENT_NHM 0XF3  // command trig. temp meas. no hold master
#define DRIVERSHT21_TRIGGER_RH_MEASUREMENT_NHM 0XF5 // command trig. hum. meas. no hold master
#define DRIVERSHT21_USER_REGISTER_W 0XE6		        // command writing user register
#define DRIVERSHT21_USER_REGISTER_R 0XE7            // command reading user register
#define DRIVERSHT21_SOFT_RESET 0XFE                 // command soft reset

// measurement signal selection
typedef enum{
  HUMIDITY,
  TEMP,
	NONE
} driverSWSHT21MeasureType;

void driverSWSHT21Init(void);
void driverSWSHT21StartMeasurement(driverSWSHT21MeasureType measureType);
bool driverSWSHT21PollMeasureReady(void);
float driverSWSHT21CalculateTemperature(uint16_t rawTemperature);
float driverSWSHT21GetTemperature(void);
float driverSWSHT21CalculateHumidity(uint16_t rawHumidity);
float driverSWSHT21GetHumidity(void);
uint8_t driverSWSHT21CalculateCRC(uint8_t data[], uint8_t no_of_bytes, uint8_t checksum);

#endif
