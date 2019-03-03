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

#ifndef TERMINAL_H_
#define TERMINAL_H_

#include "mainDataTypes.h"
#include "modCommands.h"
#include "generalDefines.h"
#include "modStateOfCharge.h"
#include "modPowerElectronics.h"
#include "modOperationalState.h"
#include "modHiAmp.h"
#include "modConfig.h"

#include <string.h>
#include <stdio.h>
#include <math.h>

// Settings
#define FAULT_VEC_LEN						25
#define CALLBACK_LEN						25

// Functions
void terminal_process_string(char *str);
void terminal_register_command_callback(
		const char* command,
		const char *help,
		const char *arg_names,
		void(*cbf)(int argc, const char **argv));

#endif /* TERMINAL_H_ */
