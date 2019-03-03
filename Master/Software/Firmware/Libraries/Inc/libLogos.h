#ifndef _LIBLOGOS_H
#define _LIBLOGOS_H

#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"

#define LOGO_WIDTH                  128
#define LOGO_HEIGHT                 64

#define NoOfLogos 									7

extern const uint8_t libLogos[NoOfLogos][LOGO_HEIGHT * LOGO_WIDTH / 8];

typedef enum {
	LOGO_DIEBIE = 0,
	LOGO_CHARGING,
	LOGO_LOAD,
	LOGO_BATTERY_DEAD,
	LOGO_ERROR,
	LOGO_BATTERY_FULL,
	LOGO_FORCEON
} libLogo;

#endif
