#ifndef __MODMESSAGE_H
#define __MODMESSAGE_H

#include "stdint.h"
#include "stdbool.h"
#include "driverSWUART2.h"
#include "modDelay.h"
#include "modConfig.h"
#include "modPowerElectronics.h"

#include "libPacket.h"
#include "modCommands.h"

#define PACKET_HANDLER_UART		1

typedef enum {
	MESSAGE_DEBUG = 0,
	MESSAGE_ERROR,
	MESSAGE_NORMAL,
} modMessageMessageTypeDef;

void modUARTInit(void);
void modUARTTask(void);
void modUARTSendPacketWrapper(unsigned char *data, unsigned int len);
void modUARTProcessPacket(unsigned char *data, unsigned int len);
void modUARTSendPacket(unsigned char *data, unsigned int len);

#endif
