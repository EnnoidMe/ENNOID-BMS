#include "modDelay.h"

static uint32_t hmsCnt = 0;

void modDelayInit(void) {
	SystemCoreClockUpdate();
	
	//SystemCoreClock / 1000
	if(SysTick_Config(72000)){
		while(1); //Error setting SysTick.
	}
}

uint8_t modDelayTick1ms(uint32_t *last, uint32_t ticks) {
	if((uint32_t)(HAL_GetTick() - *last) >= ticks)
	{
		*last = HAL_GetTick();
		return true;
	}

	return false;
}

uint8_t modDelayTick100ms(uint32_t *last, uint32_t ticks) {
	static uint32_t msTicks = 0;

	if(modDelayTick1ms(&msTicks,99))
		hmsCnt++;

	if((uint32_t)(hmsCnt - *last) >= ticks)
	{
		*last = hmsCnt;
		return true;
	}

	return false;
}

uint8_t modDelayTick1msNoRST(uint32_t *last, uint32_t ticks) {
	if((uint32_t)(HAL_GetTick() - *last) >= ticks)
	{
		return true;
	}

	return false;
}

uint8_t modDelayTick100msNoRST(uint32_t *last, uint32_t ticks) {
	static uint32_t msTicks = 0;

	if(modDelayTick1msNoRST(&msTicks,99))
		hmsCnt++;

	if((uint32_t)(hmsCnt - *last) >= ticks)
	{
		*last = hmsCnt;
		return true;
	}

	return false;
}
