#include "driverHWSPI1.h"
#include "stdlib.h"
#include "math.h"

#define PEC_POLY 												0x07
#define PEC_SEED 												0x41
#define LTC6803_BASEADDRES							0x80

typedef enum {
	LTC6803WriteConfigurationRegisterGroup = 0x01,
	LTC6803ReadConfigurationRegisterGroup = 0x02,
	LTC6803ReadAllCellVoltageGroup  = 0x04,																									// Read the measured cell voltages
	LTC6803ReadCellVoltage1to4 = 0x06,
	LTC6803ReadCellVoltage5to8 = 0x08,
	LTC6803ReadCellVoltage9to12 = 0x0A,
	LTC6803ReadFlagRegisterGroup = 0x0C,
	LTC6803ReadTemperatureRegisterGroup = 0x0E,																							// Read the measured temperature voltages
	LTC6803StartCellVoltageADCConversionAll = 0x10,																					// Start normal cellvoltage to digital conversion. Discharge resistors are switched of during measurement.
	LTC6803StartCellVoltageADCConversionCell01 = 0x11,
	LTC6803StartCellVoltageADCConversionCell02 = 0x12,
	LTC6803StartCellVoltageADCConversionCell03 = 0x12,
	LTC6803StartCellVoltageADCConversionCell04 = 0x14,
	LTC6803StartCellVoltageADCConversionCell05 = 0x15,
	LTC6803StartCellVoltageADCConversionCell06 = 0x16,
	LTC6803StartCellVoltageADCConversionCell07 = 0x17,
	LTC6803StartCellVoltageADCConversionCell08 = 0x18,
	LTC6803StartCellVoltageADCConversionCell09 = 0x19,
	LTC6803StartCellVoltageADCConversionCell10 = 0x1A,
	LTC6803StartCellVoltageADCConversionCell11 = 0x1B,
	LTC6803StartCellVoltageADCConversionCell12 = 0x1C,
	LTC6803StartCellVoltageADCConversionClear = 0x1D,
	LTC6803StartCellVoltageADCConversionSelfTest1 = 0x1E,
	LTC6803StartCellVoltageADCConversionSelfTest2 = 0x1F,
	LTC6803StartOpenWireADCConversionAll = 0x20,																						// Start cellvoltage conversion with extra 100uA load. Discharge resistors are switched of during measurement.
	LTC6803StartOpenWireADCConversionCell01 = 0x21,
	LTC6803StartOpenWireADCConversionCell02 = 0x22,
	LTC6803StartOpenWireADCConversionCell03 = 0x23,
	LTC6803StartOpenWireADCConversionCell04 = 0x24,
	LTC6803StartOpenWireADCConversionCell05 = 0x25,
	LTC6803StartOpenWireADCConversionCell06 = 0x26,
	LTC6803StartOpenWireADCConversionCell07 = 0x27,
	LTC6803StartOpenWireADCConversionCell08 = 0x28,
	LTC6803StartOpenWireADCConversionCell09 = 0x29,
	LTC6803StartOpenWireADCConversionCell10 = 0x2A,
	LTC6803StartOpenWireADCConversionCell11 = 0x2B,
	LTC6803StartOpenWireADCConversionCell12 = 0x2C,
	LTC6803StartTemperatureADCConversionAll = 0x30,																					// Start temperature input analog to digital conversion.
	LTC6803StartTemperatureADCConversionExternal1 = 0x31,
	LTC6803StartTemperatureADCConversionExternal2 = 0x32,
	LTC6803StartTemperatureADCConversionInternal = 0x33,
	LTC6803StartTemperatureADCConversionSelfTest1 = 0x3E,
	LTC6803StartTemperatureADCConversionSelfTest2 = 0x3F,
	LTC6803PollADCConverterStatus = 0x40,
	LTC6803PollInterruptStatus = 0x50,
	LTC6803StartDiagnose = 0x52,
  LTC6803ReadDiagnosticRegister = 0x54,
	LTC6803StartCellVoltageADCConversionDischargePermittedAll = 0x60,												// Start normal cellvoltage to digital conversion. Load resistors stay on during measurement.
	LTC6803StartCellVoltageADCConversionDischargePermittedCell01 = 0x61,
	LTC6803StartCellVoltageADCConversionDischargePermittedCell02 = 0x62,
	LTC6803StartCellVoltageADCConversionDischargePermittedCell03 = 0x63,
	LTC6803StartCellVoltageADCConversionDischargePermittedCell04 = 0x64,
	LTC6803StartCellVoltageADCConversionDischargePermittedCell05 = 0x65,
	LTC6803StartCellVoltageADCConversionDischargePermittedCell06 = 0x66,
	LTC6803StartCellVoltageADCConversionDischargePermittedCell07 = 0x67,
	LTC6803StartCellVoltageADCConversionDischargePermittedCell08 = 0x68,
	LTC6803StartCellVoltageADCConversionDischargePermittedCell09 = 0x69,
	LTC6803StartCellVoltageADCConversionDischargePermittedCell10 = 0x6A,
	LTC6803StartCellVoltageADCConversionDischargePermittedCell11 = 0x6B,
	LTC6803StartCellVoltageADCConversionDischargePermittedCell12 = 0x6C,
  LTC6803StartOpenWireADCConversionDischargePermittedAll = 0x70,													// Start cellvoltage to digital conversion with 100uA extra load. Load resistors stay on during measurement.
  LTC6803StartOpenWireADCConversionDischargePermittedCell01 = 0x71,
  LTC6803StartOpenWireADCConversionDischargePermittedCell02 = 0x72,
  LTC6803StartOpenWireADCConversionDischargePermittedCell03 = 0x73,
  LTC6803StartOpenWireADCConversionDischargePermittedCell04 = 0x74,
  LTC6803StartOpenWireADCConversionDischargePermittedCell05 = 0x75,
  LTC6803StartOpenWireADCConversionDischargePermittedCell06 = 0x76,
  LTC6803StartOpenWireADCConversionDischargePermittedCell07 = 0x77,
  LTC6803StartOpenWireADCConversionDischargePermittedCell08 = 0x78,
  LTC6803StartOpenWireADCConversionDischargePermittedCell09 = 0x79,
  LTC6803StartOpenWireADCConversionDischargePermittedCell10 = 0x7A,
  LTC6803StartOpenWireADCConversionDischargePermittedCell11 = 0x7B,
  LTC6803StartOpenWireADCConversionDischargePermittedCell12 = 0x7C
} driverSWLTC6803Registers;

typedef struct {
	float cellVoltage;
	uint8_t cellNumber;
} driverLTC6803CellsTypedef;

typedef struct {
	bool WatchDogFlag;																																			// Read the watchdog timer io pin, this pin is open drain and internally controlled by HW 0=WDT timeout
	bool GPIO1;																																							// Read/Write opendrain GPIO0
	bool GPIO2;																																							// Read/Write opendrain GPIO1
	bool LevelPolling;																																			// Look at page 20 of 680324fa.pdf (LTC6803-2 datasheet), for now make this bit high by default.
	uint8_t CDCMode;																																				// Comparator Duty cycle mode
	uint16_t DisChargeEnableMask;																														// Set enable state of discharge, 1=EnableDischarge, 0=DisableDischarge
	uint8_t noOfCells;																																			// Number of cells to monitor (that can cause interrupt)
	uint8_t CellVoltageConversionMode;																											// Cell voltage conversion mode
	float CellUnderVoltageLimit;																														// Undervoltage level, cell voltages under this limit will cause interrupt
	float CellOverVoltageLimit;																															// Over voltage limit, cell voltages over this limit will cause interrupt
} driverLTC6803ConfigStructTypedef;

void driverSWLTC6803Init(driverLTC6803ConfigStructTypedef configStruct, uint8_t totalNumberOfLTCs);
void driverSWLTC6803ReInit(void);
void driverSWLTC6803ReadInit(driverLTC6803ConfigStructTypedef *configStruct, uint8_t totalNumberOfLTCs);
void driverSWLTC6803StartCellVoltageConversion(void);
void driverSWLTC6803StartTemperatureVoltageConversion(void);
void driverSWLTC6803ResetCellVoltageRegisters(void);
bool driverSWLTC6803ReadCellVoltages(driverLTC6803CellsTypedef cellVoltages[12]);
bool driverSWLTC6803ReadTempVoltages(uint16_t tempVoltages[3]);
void driverSWLTC6803WriteConfigRegisters(uint8_t total_ic, uint8_t config[][6]);
void driverSWLTC6803WriteConfig(driverLTC6803ConfigStructTypedef configStruct);
bool driverSWLTC6803ReadConfigRegisters(uint8_t total_ic, uint8_t r_config[][7]);
bool driverSWLTC6803ReadConfig(driverLTC6803ConfigStructTypedef *configStruct);
bool driverSWLTC6803ReadFlagRegisters(uint8_t total_ic, uint8_t flagRegisters[][4]);
bool driverSWLTC6803ReadVoltageFlags(uint16_t *underVoltageFlags, uint16_t *overVoltageFlags);
void driverSWLTC6803EnableBalanceResistors(uint16_t balanceEnableMask);

// Calculating CRC for communication
uint8_t driverSWLTC6803CalcPEC(uint8_t len, uint8_t *data	);

// Writing and reading to the chip
void driverSWLTC6803SendCommand(driverSWLTC6803Registers command);
void driverSWLTC6803Write(uint8_t *writeBytes, uint8_t writeLength);
void driverSWLTC6803WriteRead(uint8_t *writeBytes, uint8_t writeLength, uint8_t *readBytes, uint8_t readLength);

// Temperature calculation
float driverSWLTC6803ConvertTemperatureExt(uint16_t inputValue,uint32_t ntcNominal,uint32_t ntcSeriesResistance,uint16_t ntcBetaFactor, float ntcNominalTemp);
float driverSWLTC6803ConvertTemperatureInt(uint16_t inputValue);
