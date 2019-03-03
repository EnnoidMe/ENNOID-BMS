/*
	Copyright 2016 Benjamin Vedder	benjamin@vedder.se

	This file is part of the VESC firmware.

	The VESC firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The VESC firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include "mainDataTypes.h"
#include "libPacket.h"
#include "generalDefines.h"
#include "libBuffer.h"
#include "modTerminal.h"
#include "modCAN.h"
#include "modFlash.h"
#include "modConfig.h"
#include "modPowerElectronics.h"
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

// Functions
void modCommandsInit(modPowerElectricsPackStateTypedef   *generalState,modConfigGeneralConfigStructTypedef *configPointer);
void modCommandsSetSendFunction(void(*func)(unsigned char *data, unsigned int len));
void modCommandsSendPacket(unsigned char *data, unsigned int len);
void modCommandsProcessPacket(unsigned char *data, unsigned int len);
void modCommandsPrintf(const char* format, ...);
void modCommandsJumpToMainApplication(void);

#endif /* COMMANDS_H_ */
