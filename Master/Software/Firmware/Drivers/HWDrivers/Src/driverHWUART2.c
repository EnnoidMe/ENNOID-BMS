#include "driverHWUART2.h"

UART_HandleTypeDef driverHWUART2Handle;
DMA_HandleTypeDef driverHWUART2HDMAHandleRX;
uint8_t driverHWUART2ReceivedChar;

uint8_t driverHWUART2ReceiveBuffer[RX_UART_BUFFER_SIZE];															// Buffer that stores received chars

void driverHWUART2Init(uint32_t baudRate) {
	memset(driverHWUART2ReceiveBuffer,0,sizeof(driverHWUART2ReceiveBuffer)/sizeof(uint8_t));
	
	/* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
	
	driverHWUART2HDMAHandleRX.Instance = DMA1_Channel6;														// Will handle transfer of received char to buffer
	driverHWUART2HDMAHandleRX.Init.Direction = DMA_PERIPH_TO_MEMORY;
	driverHWUART2HDMAHandleRX.Init.PeriphInc = DMA_PINC_DISABLE;
	driverHWUART2HDMAHandleRX.Init.MemInc = DMA_MINC_ENABLE;
	driverHWUART2HDMAHandleRX.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	driverHWUART2HDMAHandleRX.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	driverHWUART2HDMAHandleRX.Init.Mode = DMA_CIRCULAR;
	driverHWUART2HDMAHandleRX.Init.Priority = DMA_PRIORITY_MEDIUM;
	
	if (HAL_DMA_Init(&driverHWUART2HDMAHandleRX) != HAL_OK)
	{
		while(true);
	}

  __HAL_LINKDMA(&driverHWUART2Handle,hdmarx,driverHWUART2HDMAHandleRX);
	
  driverHWUART2Handle.Instance = USART2;
  driverHWUART2Handle.Init.BaudRate = baudRate;
  driverHWUART2Handle.Init.WordLength = UART_WORDLENGTH_8B;
  driverHWUART2Handle.Init.StopBits = UART_STOPBITS_1;
  driverHWUART2Handle.Init.Parity = UART_PARITY_NONE;
  driverHWUART2Handle.Init.Mode = UART_MODE_TX_RX;
  driverHWUART2Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  driverHWUART2Handle.Init.OverSampling = UART_OVERSAMPLING_16;
  driverHWUART2Handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  driverHWUART2Handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	
  if (HAL_UART_Init(&driverHWUART2Handle) != HAL_OK) {
    while(true);
  }
	
	volatile HAL_StatusTypeDef temp = HAL_UART_Receive_DMA(&driverHWUART2Handle,driverHWUART2ReceiveBuffer,sizeof(driverHWUART2ReceiveBuffer)/sizeof(uint8_t)); // Start receive to DMA transfer
	
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);		// Will realise interrupt on half and full DMA transfer
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
};

void driverHWUART2SendChar(uint8_t character) {
	HAL_UART_Transmit(&driverHWUART2Handle,&character,sizeof(character)/sizeof(uint8_t),10);	// Send single char over serial port
};

bool driverHWUART2GetChar(char *character) {
	static uint32_t bufferTail = 0;																								// Initiate tail to 0
	uint32_t bufferHead = sizeof(driverHWUART2ReceiveBuffer)/sizeof(uint8_t) - driverHWUART2HDMAHandleRX.Instance->CNDTR; // Tranform to be transferred bytes to bytes transferred.
	
	if(bufferTail != bufferHead){																									// If head and tail differs there is something in the buffer
		*character = driverHWUART2ReceiveBuffer[bufferTail];												// Get char from tail of buffer
		bufferTail++;																																// Move the tail forward
		bufferTail %= (sizeof(driverHWUART2ReceiveBuffer)/sizeof(uint8_t));					// Wrap it around the end
		return true;																																// Indicate there is a char in buffer
	}else
		return false;																																// Indicate there is no char in buffer
};

void DMA1_Channel6_IRQHandler(void) {		 																				// Will trigger on halve and full
  HAL_DMA_IRQHandler(&driverHWUART2HDMAHandleRX);
}
