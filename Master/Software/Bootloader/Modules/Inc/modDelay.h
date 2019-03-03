#include "stm32f3xx_hal.h"
#include "stdbool.h"

void modDelayInit(void);
uint8_t modDelayTick1ms(uint32_t *last, uint32_t ticks);
uint8_t modDelayTick100ms(uint32_t *last, uint32_t ticks);
