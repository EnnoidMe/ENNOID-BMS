#include "driverSWLTC6803.h"

uint8_t driverSWLTC6803TotalNumerOfICs = 0;
driverLTC6803ConfigStructTypedef driverSWLTC6803ConfigStruct;

void driverSWLTC6803Init(driverLTC6803ConfigStructTypedef configStruct, uint8_t totalNumberOfLTCs) {
	driverSWLTC6803TotalNumerOfICs = totalNumberOfLTCs;
	driverSWLTC6803ConfigStruct = configStruct;
	
	driverHWSPI1Init(LTC_CS_GPIO_Port,LTC_CS_Pin);
	
	driverSWLTC6803WriteConfig(driverSWLTC6803ConfigStruct);
};

void driverSWLTC6803ReInit(void) {
		driverSWLTC6803WriteConfig(driverSWLTC6803ConfigStruct);
};

void driverSWLTC6803ReadInit(driverLTC6803ConfigStructTypedef *configStruct, uint8_t totalNumberOfLTCs) {	
	//driverSWLTC6803ReadConfig(driverSWLTC6803TotalNumerOfICs,configPointer);
};

void driverSWLTC6803StartCellVoltageConversion(void) {
	driverSWLTC6803SendCommand((driverSWLTC6803Registers)driverSWLTC6803ConfigStruct.CellVoltageConversionMode);
};

void driverSWLTC6803StartTemperatureVoltageConversion(void) {
	driverSWLTC6803SendCommand(LTC6803StartTemperatureADCConversionAll);
};

void driverSWLTC6803ResetCellVoltageRegisters(void) {
	driverSWLTC6803SendCommand(LTC6803StartCellVoltageADCConversionClear);
};

bool driverSWLTC6803ReadCellVoltages(driverLTC6803CellsTypedef cellVoltages[12]) {
	uint16_t cellRegisters[12]; 
  int data_counter = 0;
  bool dataValid = true;
  uint8_t data_pec = 0;
  uint8_t received_pec = 0;
  uint8_t rx_data[19];
  uint8_t cmd[4];
	
	cmd[0] = LTC6803_BASEADDRES;
	cmd[1] = driverSWLTC6803CalcPEC(1,cmd);
	cmd[2] = LTC6803ReadAllCellVoltageGroup;
	cmd[3] = driverSWLTC6803CalcPEC(1,cmd+2);
	
	driverSWLTC6803WriteRead(cmd, 4,rx_data,19);

	received_pec =  rx_data[18];
	data_pec = driverSWLTC6803CalcPEC(18, &rx_data[0]);
	if (received_pec != data_pec) {
		dataValid = false;
	}
	
	data_counter = 0;
	uint16_t temp,temp2;

	for (int k = 0; k<12; k=k+2) {
		temp = rx_data[data_counter++];
		temp2 = (uint16_t)(rx_data[data_counter]&0x0F)<<8;
		cellRegisters[k] = temp + temp2 -512;
		temp2 = (rx_data[data_counter++])>>4;
		temp =  (rx_data[data_counter++])<<4;
		cellRegisters[k+1] = temp+temp2 -512;
	}
	
	if(dataValid){
		for(uint8_t j=0; j<12 ; j++) {		
			if(cellRegisters[j]*0.0015 < 5.0f)
				cellVoltages[j].cellVoltage = cellRegisters[j]*0.0015;
			else
				dataValid = false;
			cellVoltages[j].cellNumber = j;
		}
	}
	
  return dataValid;
};

bool driverSWLTC6803ReadTempVoltages(uint16_t tempVoltages[3]) {
  int data_counter = 0;
  bool pec_error = true;
  uint8_t data_pec = 0;
  uint8_t received_pec = 0;
  uint8_t cmd[4];
  uint8_t rx_data[7];
	
	cmd[0] = LTC6803_BASEADDRES;
	cmd[1] = driverSWLTC6803CalcPEC(1,cmd);
	cmd[2] = LTC6803ReadTemperatureRegisterGroup;
	cmd[3] = driverSWLTC6803CalcPEC(1,cmd+2);
	
	driverSWLTC6803WriteRead(cmd, 4,rx_data,6);

	received_pec =  rx_data[5];
	data_pec = driverSWLTC6803CalcPEC(5, &rx_data[0]);
	if (received_pec != data_pec) {
		pec_error = false;
	}

	int temp,temp2;
	temp = rx_data[data_counter++];
	temp2 = (rx_data[data_counter]& 0x0F)<<8;
	tempVoltages[0] = (temp + temp2 -512)*2;
	temp2 = (rx_data[data_counter++])>>4;
	temp =  (rx_data[data_counter++])<<4;
	tempVoltages[1] = (temp+temp2 -512)*2;
	temp2 = (rx_data[data_counter++]);
	temp =  (rx_data[data_counter++]& 0x0F)<<8;
	tempVoltages[2] = temp+temp2 -512;
	
  return pec_error;
};

void driverSWLTC6803WriteConfigRegisters(uint8_t total_ic, uint8_t config[][6]) {
  uint8_t BYTES_IN_REG = 6;
  uint8_t CMD_LEN = 4+7;
  uint8_t *cmd;
  uint16_t cfg_pec;
  uint8_t cmd_index; //command counter

  cmd = (uint8_t *)malloc(CMD_LEN*sizeof(uint8_t));
	
  for (uint8_t current_ic = 0; current_ic < total_ic; current_ic++) {
    cmd[0] = LTC6803_BASEADDRES + current_ic;
    cmd[1] = driverSWLTC6803CalcPEC(1,cmd);

    cmd[2] = LTC6803WriteConfigurationRegisterGroup;
    cmd[3] = driverSWLTC6803CalcPEC(sizeof(uint8_t),cmd+2);

    cmd_index = 4;

    for (uint8_t current_byte = 0; current_byte < BYTES_IN_REG; current_byte++) {
      cmd[cmd_index] = config[current_ic][current_byte];
      cmd_index = cmd_index + 1;
    }

    cfg_pec = driverSWLTC6803CalcPEC(BYTES_IN_REG, &config[current_ic][0]);    // calculating the PEC for each ICs configuration register data
    cmd[cmd_index ] = (uint8_t)cfg_pec;
    cmd_index = cmd_index + 1;

		driverSWLTC6803Write(cmd,CMD_LEN);
  }
	
  free(cmd);
};

void driverSWLTC6803WriteConfig(driverLTC6803ConfigStructTypedef configStruct) {
	uint8_t configPointer[1][6];
	uint16_t maskCellHolder = 0x0FFF;	
	
	for(uint8_t i = 0; i < driverSWLTC6803ConfigStruct.noOfCells; i++)
		maskCellHolder &= ~(1 << i);																								// Clear the bit nrs to 0 in order to check that cell 1=ignore cell 0=check cell
	
	configPointer[0][0] = (uint8_t) (driverSWLTC6803ConfigStruct.WatchDogFlag << 7) | (driverSWLTC6803ConfigStruct.GPIO1 << 6) | (driverSWLTC6803ConfigStruct.GPIO2 << 5) |  (driverSWLTC6803ConfigStruct.LevelPolling << 4) | (driverSWLTC6803ConfigStruct.CDCMode & 0x07);
	configPointer[0][1] = (uint8_t) (driverSWLTC6803ConfigStruct.DisChargeEnableMask & 0x00FF) ; 
	configPointer[0][2] = (uint8_t) ((maskCellHolder & 0x000F) << 4) | (driverSWLTC6803ConfigStruct.DisChargeEnableMask >> 8);
	configPointer[0][3] = (uint8_t) (maskCellHolder >> 4); 
	configPointer[0][4] = (uint8_t) (driverSWLTC6803ConfigStruct.CellUnderVoltageLimit/(16*0.0015))+31;
	configPointer[0][5] = (uint8_t) (driverSWLTC6803ConfigStruct.CellOverVoltageLimit/(16*0.0015))+32;

	driverSWLTC6803WriteConfigRegisters(driverSWLTC6803TotalNumerOfICs,configPointer);
};

bool driverSWLTC6803ReadConfigRegisters(uint8_t total_ic, uint8_t r_config[][7]) {
  uint8_t BYTES_IN_REG = 7;																											// 6 Register bytes + 1 PEC
  uint8_t cmd[4];
  uint8_t *rx_data;
  bool pec_error = true;
  static uint8_t data_pec;
  static uint8_t received_pec;

  rx_data = (uint8_t *) malloc((BYTES_IN_REG*total_ic)*sizeof(uint8_t));

  for (uint8_t current_ic = 0; current_ic < total_ic; current_ic++) {       //executes for each LTC6803 in the daisy chain and packs the data
    //into the r_config array as well as check the received Config data
    //for any bit errors

    cmd[0] = LTC6803_BASEADDRES + current_ic;
    cmd[1] = driverSWLTC6803CalcPEC(1,cmd);
    cmd[2] = LTC6803ReadConfigurationRegisterGroup;
    cmd[3] = driverSWLTC6803CalcPEC(sizeof(uint8_t),cmd+2);

    driverSWLTC6803WriteRead(cmd, 4, rx_data, (BYTES_IN_REG*total_ic));

    for (uint8_t current_byte = 0; current_byte < BYTES_IN_REG; current_byte++) {
      r_config[current_ic][current_byte] = rx_data[current_byte];
    }

    received_pec =  r_config[current_ic][6];
    data_pec = driverSWLTC6803CalcPEC(6, &r_config[current_ic][0]);
    if (received_pec != data_pec) {
      pec_error = false;
    }
  }

  free(rx_data);
	
  return pec_error;
};

bool driverSWLTC6803ReadConfig(driverLTC6803ConfigStructTypedef *configStruct) {
	uint8_t configPointer[1][7];
	bool returnval;
	
	returnval = driverSWLTC6803ReadConfigRegisters(driverSWLTC6803TotalNumerOfICs,configPointer);
	
	configStruct->WatchDogFlag = (configPointer[0][0] & 0x80) >> 7;
	configStruct->GPIO1 = (configPointer[0][0] & 0x20) >> 5;
	configStruct->GPIO2 = (configPointer[0][0] & 0x40) >> 6;
	configStruct->LevelPolling = (configPointer[0][0] & 0x10) >> 4;
	configStruct->CDCMode = (configPointer[0][0] & 0x07);
	configStruct->DisChargeEnableMask = ((configPointer[0][2] & 0x0F) << 8) | configPointer[0][1];
	configStruct->noOfCells = 0;
	configStruct->CellVoltageConversionMode = 0;
	configStruct->CellUnderVoltageLimit = ((configPointer[0][4]-31)*16*0.0015);
	configStruct->CellOverVoltageLimit = ((configPointer[0][5]-32)*16*0.0015);

	return returnval;
}

bool driverSWLTC6803ReadFlagRegisters(uint8_t total_ic, uint8_t flagRegisters[][4]) {
  uint8_t BYTES_IN_REG = 4;																									// 3 Register bytes + 1 PEC
  uint8_t cmd[4];
  uint8_t *rx_data;
  bool pec_error = true;
  static uint8_t data_pec;
  static uint8_t received_pec;

  rx_data = (uint8_t *) malloc((BYTES_IN_REG*total_ic)*sizeof(uint8_t));

  for (uint8_t current_ic = 0; current_ic < total_ic; current_ic++) {       //executes for each LTC6803 in the daisy chain and packs the data
    cmd[0] = LTC6803_BASEADDRES + current_ic;
    cmd[1] = driverSWLTC6803CalcPEC(1,cmd);
    cmd[2] = LTC6803ReadFlagRegisterGroup;
    cmd[3] = driverSWLTC6803CalcPEC(sizeof(uint8_t),cmd+2);

    driverSWLTC6803WriteRead(cmd, 4, rx_data, (BYTES_IN_REG*total_ic));

    for (uint8_t current_byte = 0; current_byte < BYTES_IN_REG; current_byte++) {
      flagRegisters[current_ic][current_byte] = rx_data[current_byte];
    }

    received_pec =  flagRegisters[current_ic][3];
    data_pec = driverSWLTC6803CalcPEC(6, &flagRegisters[current_ic][0]);
    if (received_pec != data_pec) {
      pec_error = false;
    }
  }

  free(rx_data);
	
  return pec_error;
};

bool driverSWLTC6803ReadVoltageFlags(uint16_t *underVoltageFlags, uint16_t *overVoltageFlags) {
	bool returnVal = false;
	uint8_t flagRegisters[1][4];
	uint32_t registersCombined;
	uint32_t registersCombinedTemp;
	*underVoltageFlags = 0;
	*overVoltageFlags = 0;
	
	returnVal = driverSWLTC6803ReadFlagRegisters(1,flagRegisters);
	
	registersCombined = (flagRegisters[0][2] << 16) | (flagRegisters[0][1] << 8) | flagRegisters[0][0];	// Combine all registers in one variable
	registersCombinedTemp = registersCombined & 0x00555555;																							// Filter out only the undervoltage bits
	
	for(int bitPointer = 0; bitPointer < driverSWLTC6803ConfigStruct.noOfCells; bitPointer++)
		*underVoltageFlags |= (registersCombinedTemp & (1 << bitPointer*2)) ? (1 << bitPointer) : 0;			// Shift undervoltage bits closer together and stote them in *underVoltageFlags
	
	registersCombinedTemp = registersCombined & 0x00AAAAAA;																							// Filter out only the overvoltage bits
	registersCombinedTemp = registersCombinedTemp >> 1;																									// Move everything one bit to the right

	for(int bitPointer = 0; bitPointer < driverSWLTC6803ConfigStruct.noOfCells; bitPointer++)
		*overVoltageFlags |= (registersCombinedTemp & (1 << bitPointer*2)) ? (1 << bitPointer) : 0;				// And do the same for the overvoltage bits
	
	return returnVal;
};

void driverSWLTC6803EnableBalanceResistors(uint16_t balanceEnableMask) {
	driverSWLTC6803ConfigStruct.DisChargeEnableMask = balanceEnableMask;
	driverSWLTC6803WriteConfig(driverSWLTC6803ConfigStruct);
};

uint8_t driverSWLTC6803CalcPEC(uint8_t len, uint8_t *data	) {
  uint8_t  remainder = PEC_SEED;																					//  PEC statrt byte PEC_SEED;

  for (uint8_t byte = 0; byte < len; ++byte) {														//	Perform modulo-2 division, a byte at a time.
    remainder ^= data[byte];																							//  Bring the next byte into the remainder.
    for (uint8_t bit = 8; bit > 0; --bit)	{																//  Perform modulo-2 division, a bit at a time.
      if (remainder & 128) {																							//  Try to divide the current data bit.
        remainder = (remainder << 1) ^ PEC_POLY;
      } else {
        remainder = (remainder << 1);
      }
    }
  }
	
  return remainder;																												// Remainder is the PEC result
};

void driverSWLTC6803SendCommand(driverSWLTC6803Registers command) {
	uint8_t cmd[2];																													// Sending single commands take only 2 bytes (command + PEC)
	cmd[0] = (uint8_t) command;
	cmd[1] = driverSWLTC6803CalcPEC(sizeof(cmd)-1,(uint8_t *)&command);			// Calculate command PEC and put it in the array
	driverSWLTC6803Write(cmd,sizeof(cmd));																	// Send both bytes over spi
};

// Coupling of drivers
void driverSWLTC6803Write(uint8_t *writeBytes, uint8_t writeLength) {
	driverHWSPI1Write(writeBytes,writeLength,LTC_CS_GPIO_Port,LTC_CS_Pin);
};

// Coupling of drivers
void driverSWLTC6803WriteRead(uint8_t *writeBytes, uint8_t writeLength, uint8_t *readBytes, uint8_t readLength) {
	driverHWSPI1WriteRead(writeBytes,writeLength,readBytes,readLength,LTC_CS_GPIO_Port,LTC_CS_Pin);
};

float driverSWLTC6803ConvertTemperatureExt(uint16_t inputValue,uint32_t ntcNominal,uint32_t ntcSeriesResistance,uint16_t ntcBetaFactor, float ntcNominalTemp) {
	static float scalar;
	static float steinhart;
	
  scalar = 4095.0f / (float)inputValue - 1.0f;
  scalar = (float)ntcSeriesResistance / scalar;
  steinhart = scalar / (float)ntcNominal;               // (R/Ro)
  steinhart = log(steinhart);                           // ln(R/Ro)
  steinhart /= (float)ntcBetaFactor;                    // 1/B * ln(R/Ro)
  steinhart += 1.0f / (ntcNominalTemp + 273.15f);       // + (1/To)
  steinhart = 1.0f / steinhart;                         // Invert
  steinhart -= 273.15f;                                 // convert to degree
	
	if(steinhart < -30.0f)
		steinhart = 200;
	
  return steinhart;
}

float driverSWLTC6803ConvertTemperatureInt(uint16_t inputValue) {
 return (float)inputValue*1.5f/8.0f - 273.15f;
}

