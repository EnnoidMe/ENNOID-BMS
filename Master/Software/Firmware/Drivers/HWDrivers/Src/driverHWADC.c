#include "driverHWADC.h"

// Example: http://visualgdb.com/tutorials/arm/stm32/adc/

ADC_HandleTypeDef hadc1;

const driverHWADCPortStruct driverHWADCPorts[NoOfADCPorts] = 								// Hold all I2C pin configuration data
{
	{GPIOA,RCC_AHBENR_GPIOAEN,GPIO_PIN_1,GPIO_MODE_ANALOG,GPIO_NOPULL,0x00},	// LoadVoltageSense analog pin
#ifdef HWVersion_0_4	
	{GPIOA,RCC_AHBENR_GPIOAEN,GPIO_PIN_0,GPIO_MODE_ANALOG,GPIO_PULLUP,0x00}		// NTC analog pin
#else  																																			// Any other previous version
	{GPIOA,RCC_AHBENR_GPIOCEN,GPIO_PIN_0,GPIO_MODE_INPUT,GPIO_PULLUP,0x00}		// P_STAT_CHARGE_DETECT
#endif
};

void driverHWADCInit(void) {
	GPIO_InitTypeDef PortInitHolder;
	uint8_t PortPointer;
	
	for(PortPointer = 0; PortPointer < NoOfADCPorts; PortPointer++) {
		RCC->AHBENR |= driverHWADCPorts[PortPointer].ClkRegister;								// Enable clock de desired port
		PortInitHolder.Mode = driverHWADCPorts[PortPointer].Mode;								// Push pull output
		PortInitHolder.Pin = driverHWADCPorts[PortPointer].Pin;									// Points to status pin
		PortInitHolder.Pull = driverHWADCPorts[PortPointer].Pull;								// No pullup
		PortInitHolder.Speed = GPIO_SPEED_HIGH;																	// GPIO clock speed
		PortInitHolder.Alternate = driverHWADCPorts[PortPointer].Alternate;			// Alternate function
		HAL_GPIO_Init(driverHWADCPorts[PortPointer].Port,&PortInitHolder);			// Perform the IO init 
	};
	
	__ADC1_CLK_ENABLE();																											// Enable clock to ADC1
	
	// ADC config	
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    while(true) {}; 																												// Error situation 
  }

	driverHWADCSetInputChannel(&hadc1,ADC_CHANNEL_2);
};

void driverHWADCSetInputChannel(ADC_HandleTypeDef* hadc, uint32_t inputChannel) {
  ADC_ChannelConfTypeDef sConfig;
	
  sConfig.Channel = inputChannel;
  sConfig.Rank = 1;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_181CYCLES_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
  {
    while(true) {}; 																												// Error situation 
  }
}

bool driverHWADCGetLoadVoltage(float *loCurrentLoadVoltage) {
	uint32_t driverHWADCAverageSum = 0;
	uint8_t	driverHWADCAverageCount = 0;
	
	driverHWADCSetInputChannel(&hadc1,ADC_CHANNEL_2);

	driverHWADCAverageSum = 0;
	for(driverHWADCAverageCount = 0; driverHWADCAverageCount < NoOfAverages; driverHWADCAverageCount++) {
		HAL_ADC_Start(&hadc1);
		if (HAL_ADC_PollForConversion(&hadc1, 1) == HAL_OK) {
			driverHWADCAverageSum += HAL_ADC_GetValue(&hadc1);
		};
	};
	
	uint16_t temp = driverHWADCAverageSum/NoOfAverages;
	*loCurrentLoadVoltage = temp*(3.3f/4096*17.4f);

	return false;
};

bool driverHWADCGetNTCValue(float *ntcValue, uint32_t ntcNominal, uint32_t ntcSeriesResistance, uint16_t ntcBetaFactor, float ntcNominalTemp) {
	uint32_t driverHWADCAverageSum;
	uint8_t	 driverHWADCAverageCount;
	uint16_t driverHWADCAverage;
	
#ifdef HWVersion_0_4
	driverHWADCSetInputChannel(&hadc1,ADC_CHANNEL_1);

	driverHWADCAverageSum = 0;
	for(driverHWADCAverageCount = 0; driverHWADCAverageCount < 5; driverHWADCAverageCount++) {
		HAL_ADC_Start(&hadc1);
		if (HAL_ADC_PollForConversion(&hadc1, 1) == HAL_OK) {
			driverHWADCAverageSum += HAL_ADC_GetValue(&hadc1);
		};
	};
	
	driverHWADCAverage = driverHWADCAverageSum/5;
	
	static float scalar;
	static float steinhart;
	
  scalar = 4095.0f / (float)driverHWADCAverage - 1.0f;
  scalar = (float)ntcSeriesResistance / scalar;
  steinhart = scalar / (float)ntcNominal;               // (R/Ro)
  steinhart = log(steinhart);                           // ln(R/Ro)
  steinhart /= (float)ntcBetaFactor;                    // 1/B * ln(R/Ro)
  steinhart += 1.0f / (ntcNominalTemp + 273.15f);       // + (1/To)
  steinhart = 1.0f / steinhart;                         // Invert
  *ntcValue = steinhart - 273.15f;                      // convert to degree
#else
	*ntcValue = 0.0f;
#endif

	return false;
};
