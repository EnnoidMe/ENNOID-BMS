/*
	Copyright 2016 - 2017 Benjamin Vedder	benjamin@vedder.se

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

#include "modTerminal.h"

// Private types
typedef struct _terminal_callback_struct {
	const char *command;
	const char *help;
	const char *arg_names;
	void(*cbf)(int argc, const char **argv);
} terminal_callback_struct;

// Private variables
static terminal_callback_struct callbacks[CALLBACK_LEN];
static int callback_write = 0;

extern modConfigGeneralConfigStructTypedef *generalConfig;
extern modStateOfChargeStructTypeDef *generalStateOfCharge;
extern modPowerElectricsPackStateTypedef packState;
extern OperationalStateTypedef modOperationalStateCurrentState;

void terminal_process_string(char *str) {
	enum { kMaxArgs = 64 };
	int argc = 0;
	char *argv[kMaxArgs];

	char *p2 = strtok(str, " ");
	while (p2 && argc < kMaxArgs) {
		argv[argc++] = p2;
		p2 = strtok(0, " ");
	}

	if (argc == 0) {
		modCommandsPrintf("No command received\n");
		return;
	}

	if (strcmp(argv[0], "ping") == 0) {
		modCommandsPrintf("pong\n");
	} else if (strcmp(argv[0], "status") == 0) {
		bool disChargeEnabled = packState.disChargeDesired && packState.disChargeLCAllowed;
		bool chargeEnabled = packState.chargeDesired && packState.chargeAllowed;
		 
		modCommandsPrintf("-----Battery Pack Status-----");		
		modCommandsPrintf("Pack voltage          : %.2fV",packState.packVoltage);
		modCommandsPrintf("Pack current          : %.2fA",packState.packCurrent);
		modCommandsPrintf("Low  current          : %.2fA",packState.loCurrentLoadCurrent);
		modCommandsPrintf("High current          : %.2fA",packState.hiCurrentLoadCurrent);		
		modCommandsPrintf("State of charge       : %.1f%%",generalStateOfCharge->generalStateOfCharge);
		modCommandsPrintf("Remaining capacity    : %.2fAh",generalStateOfCharge->remainingCapacityAh);
		
		switch(modOperationalStateCurrentState) {
			case OP_STATE_CHARGING:
				modCommandsPrintf("Operational state     : %s","Charging");
				break;
			case OP_STATE_LOAD_ENABLED:
				modCommandsPrintf("Operational state     : %s","Load enabled");
				break;
			case OP_STATE_CHARGED:
				modCommandsPrintf("Operational state     : %s","Charged");
				break;
			case OP_STATE_BALANCING:
				modCommandsPrintf("Operational state     : %s","Balancing");
				break;
			case OP_STATE_ERROR_PRECHARGE:
				modCommandsPrintf("Operational state     : %s","Pre charge error");
				break;	
			case OP_STATE_ERROR:
				modCommandsPrintf("Operational state     : %s","Error");
				break;			
			case OP_STATE_FORCEON:
				modCommandsPrintf("Operational state     : %s","Forced on");
				break;
			case OP_STATE_POWER_DOWN:
				modCommandsPrintf("Operational state     : %s","Power down");
				break;
			case OP_STATE_EXTERNAL:
				modCommandsPrintf("Operational state     : %s","External (USB or CAN)");
				break;
			default:
				modCommandsPrintf("Operational state     : %s","Unknown");
				break;
		}
		modCommandsPrintf("Load voltage          : %.2fV",packState.loCurrentLoadVoltage);
		modCommandsPrintf("Cell voltage high     : %.3fV",packState.cellVoltageHigh);
		modCommandsPrintf("Cell voltage low      : %.3fV",packState.cellVoltageLow);
		modCommandsPrintf("Cell voltage average  : %.3fV",packState.cellVoltageAverage);
		modCommandsPrintf("Cell voltage mismatch : %.3fV",packState.cellVoltageMisMatch);
		modCommandsPrintf("Discharge enabled     : %s",disChargeEnabled ? "True" : "False");
		modCommandsPrintf("Charge enabled        : %s",chargeEnabled ? "True" : "False");	
    modCommandsPrintf("Power button pressed  : %s",packState.powerButtonActuated ? "True" : "False");		
		modCommandsPrintf("---End Battery Pack Status---");
		modCommandsPrintf(" ");
		
	} else if (strcmp(argv[0], "sens") == 0) {		
		modCommandsPrintf("-----       Sensors         -----");
		
		// print temperatures
		modCommandsPrintf("Sensor[0]  : %.1f C - E - 'LTC NTC0'",packState.temperatures[0]);
		modCommandsPrintf("Sensor[1]  : %.1f C - E - 'LTC NTC1'",packState.temperatures[1]);
		modCommandsPrintf("Sensor[2]  : %.1f C - I - 'LTC Internal'",packState.temperatures[2]);
		modCommandsPrintf("Sensor[3]  : %.1f C - I - 'STM NTC'",packState.temperatures[3]);
		modCommandsPrintf("Sensor[4]  : %.1f C - E - 'ADC NTC0'",packState.temperatures[4]);
		modCommandsPrintf("Sensor[5]  : %.1f C - E - 'ADC NTC1'",packState.temperatures[5]);
		modCommandsPrintf("Sensor[6]  : %.1f C - E - 'ADC NTC2'",packState.temperatures[6]);
		modCommandsPrintf("Sensor[7]  : %.1f C - E - 'ADC NTC3'",packState.temperatures[7]);
		modCommandsPrintf("Sensor[8]  : %.1f C - E - 'ADC NTC4'",packState.temperatures[8]);
		modCommandsPrintf("Sensor[9]  : %.1f C - E - 'ADC NTC5'",packState.temperatures[9]);
		modCommandsPrintf("Sensor[10] : %.1f C - I - 'ADC NTC6'",packState.temperatures[10]);
		modCommandsPrintf("Sensor[11] : %.1f C - I - 'ADC NTC7'",packState.temperatures[11]);
		modCommandsPrintf("Sensor[12] : %.1f C - I - 'SHT'",packState.temperatures[12]);
		modCommandsPrintf("Sensor[13] : %.1f %% - I - 'Humidity'",packState.humidity);		
		modCommandsPrintf("----- E=External I=Internal -----");
		modCommandsPrintf("-----     End sensors       -----");
		modCommandsPrintf(" ");
		
	} else if (strcmp(argv[0], "cells") == 0) {
		uint8_t cellPointer = 0;
		
		modCommandsPrintf("-----   Cell voltages   -----");				
		for(cellPointer = 0 ; cellPointer < generalConfig->noOfCells ; cellPointer++) {
			modCommandsPrintf("Cell voltage%2d             : %.3fV",cellPointer,packState.cellVoltagesIndividual[cellPointer].cellVoltage);
		}
		modCommandsPrintf("Cell voltage high          : %.3fV",packState.cellVoltageHigh);
		modCommandsPrintf("Cell voltage low           : %.3fV",packState.cellVoltageLow);
		modCommandsPrintf("Cell voltage average       : %.3fV",packState.cellVoltageAverage);
		modCommandsPrintf("Cell voltage mismatch      : %.3fV",packState.cellVoltageMisMatch);
		modCommandsPrintf("----- End Cell voltages -----");	
		modCommandsPrintf(" ");
		
	} else if (strcmp(argv[0], "config") == 0) {
		modCommandsPrintf("---   BMS Configuration   ---");
		modCommandsPrintf("NoOfCells                  : %u",generalConfig->noOfCells);
		modCommandsPrintf("batteryCapacity            : %.2fAh",generalConfig->batteryCapacity);
		modCommandsPrintf("cellHardUnderVoltage       : %.3fV",generalConfig->cellHardUnderVoltage);
		modCommandsPrintf("cellHardOverVoltage        : %.3fV",generalConfig->cellHardOverVoltage);
		modCommandsPrintf("cellLCSoftUnderVoltage     : %.3fV",generalConfig->cellLCSoftUnderVoltage);
		modCommandsPrintf("cellSoftOverVoltage        : %.3fV",generalConfig->cellSoftOverVoltage);
		modCommandsPrintf("cellBalanceStart           : %.3fV",generalConfig->cellBalanceStart);
		modCommandsPrintf("cellBalanceDiffThreshold   : %.3fV",generalConfig->cellBalanceDifferenceThreshold);
		modCommandsPrintf("CAN ID                     : %u",generalConfig->CANID);
		modCommandsPrintf("--- End BMS Configuration ---");
		modCommandsPrintf(" ");
		
	} else if (strcmp(argv[0], "config_default") == 0) {
		modCommandsPrintf("--Restoring default config--");
		if(modConfigStoreDefaultConfig())
			modCommandsPrintf("Succesfully restored config, new config wil be used on powercycle (or use config_read to apply it now).");
		else
			modCommandsPrintf("Error restored config.");
		modCommandsPrintf(" ");
		
	} else if (strcmp(argv[0], "config_write") == 0) {
		modCommandsPrintf("---    Writing config    ---");
		if(modConfigStoreConfig())
			modCommandsPrintf("Succesfully written config.");
		else
			modCommandsPrintf("Error writing config.");
		modCommandsPrintf(" ");
		
	} else if (strcmp(argv[0], "config_read") == 0) {
		modCommandsPrintf("---    Reading config    ---");
		if(modConfigLoadConfig())
			modCommandsPrintf("Succesfully read config.");
		else
			modCommandsPrintf("Error reading config.");
		modCommandsPrintf(" ");
		
	} else if (strcmp(argv[0], "config_set_cells") == 0) {
		modCommandsPrintf("---Setting new cell count---");
		if (argc == 2) {
			uint32_t newNumberOfCells = 0;
			sscanf(argv[1], "%u", &newNumberOfCells);
			if(newNumberOfCells < 13 && newNumberOfCells > 2) {
				modCommandsPrintf("Number of cells is set to: %u.",newNumberOfCells);
				generalConfig->noOfCells = newNumberOfCells;
			} else {
				modCommandsPrintf("Invalid number of cells (should be anything from 3 to 12).");
			}
		} else {
			modCommandsPrintf("This command requires one argument.");
		}
		modCommandsPrintf(" ");

	} else if (strcmp(argv[0], "hwinfo") == 0) {
		modCommandsPrintf("-------    BMS Info   -------");		
		modCommandsPrintf("Firmware: %s", FW_REAL_VERSION);
		modCommandsPrintf("Name    : %s", HW_NAME);
		modCommandsPrintf("UUID: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
				STM32_UUID_8[0], STM32_UUID_8[1], STM32_UUID_8[2], STM32_UUID_8[3],
				STM32_UUID_8[4], STM32_UUID_8[5], STM32_UUID_8[6], STM32_UUID_8[7],
				STM32_UUID_8[8], STM32_UUID_8[9], STM32_UUID_8[10], STM32_UUID_8[11]);
		modCommandsPrintf("------- End BMS Info  -------");
		modCommandsPrintf(" ");
		
	} else if (strcmp(argv[0], "reboot") == 0) {
		modCommandsPrintf("------  Rebooting BMS  ------");
		NVIC_SystemReset();
		
	} else if (strcmp(argv[0], "bootloader_erase") == 0) {
		modCommandsPrintf("------  erasing new app space  ------");
		if(modFlashEraseNewAppData(0x00002000) == HAL_OK)
			modCommandsPrintf("--Erase done.");
		else
			modCommandsPrintf("--Erase error.");
		
	} else if (strcmp(argv[0], "bootloader_jump") == 0) {
		modFlashJumpToBootloader();
		
	} else if (strcmp(argv[0], "slave_scan") == 0) {
		uint8_t bitPointer;
		char    outputString[9];
		uint8_t presence = modHiAmpShieldScanI2CDevices();
		
		modCommandsPrintf("------  Slave BMS I2C scan  ------");
		
		for(bitPointer = 0; bitPointer < 8; bitPointer++){
		  if(presence & (1 << bitPointer))
				outputString[7-bitPointer] = '1';
			else
				outputString[7-bitPointer] = '0';
		}

		outputString[8] = 0;
		
		modCommandsPrintf("Presence: 0b%s",outputString);
		modCommandsPrintf("Bit order: 0(MSB) - 0 - FANDriver - NTCADC - IOExt - SHT - ISLAux - ISLMain(LSB). ");
		modCommandsPrintf("SHT does not respond when it is doing a conversion.");
		modCommandsPrintf("------  Slave BMS I2C scan end  ------");
		
	} else if (strcmp(argv[0], "help") == 0) {
		modCommandsPrintf("------- Start of help -------");
		modCommandsPrintf("Valid commands for the DieBieMS are:");
		modCommandsPrintf("help");
		modCommandsPrintf("  Show this help.");
		modCommandsPrintf("ping");
		modCommandsPrintf("  Print pong here to see if the reply works.");
		modCommandsPrintf("slave_scan");
		modCommandsPrintf("  Scan the I2C devices on the slave.");
		modCommandsPrintf("status");
		modCommandsPrintf("  Print battery measurements summary.");
		modCommandsPrintf("sens");
		modCommandsPrintf("  Print all sensor values.");
		modCommandsPrintf("cells");
		modCommandsPrintf("  Print cell voltage measurements.");
		modCommandsPrintf("config");
		modCommandsPrintf("  Print BMS configuration.");
		modCommandsPrintf("config_default");
		modCommandsPrintf("  Load default BMS configuration.");
		modCommandsPrintf("config_write");
		modCommandsPrintf("  Store current BMS configuration to EEPROM.");
		modCommandsPrintf("config_read");
		modCommandsPrintf("  Read BMS configuration from EEPROM.");
		modCommandsPrintf("hwinfo");
		modCommandsPrintf("  Print some hardware information.");
		modCommandsPrintf(" ");
		modCommandsPrintf("---More functionallity to come...--");

		for (int i = 0;i < callback_write;i++) {
			if (callbacks[i].arg_names) {
				modCommandsPrintf("%s %s", callbacks[i].command, callbacks[i].arg_names);
			} else {
				modCommandsPrintf(callbacks[i].command);
			}

			if (callbacks[i].help) {
				modCommandsPrintf("  %s", callbacks[i].help);
			} else {
				modCommandsPrintf("  There is no help available for this command.");
			}
		}

		modCommandsPrintf(" ");
	} else {
		bool found = false;
		for (int i = 0;i < callback_write;i++) {
			if (strcmp(argv[0], callbacks[i].command) == 0) {
				callbacks[i].cbf(argc, (const char**)argv);
				found = true;
				break;
			}
		}

		if (!found) {
			modCommandsPrintf("Invalid command: %s\n type help to list all available commands\n", argv[0]);
		}
	}
}

/**
 * Register a custom command  callback to the terminal. If the command
 * is already registered the old command callback will be replaced.
 *
 * @param command
 * The command name.
 *
 * @param help
 * A help text for the command. Can be NULL.
 *
 * @param arg_names
 * The argument names for the command, e.g. [arg_a] [arg_b]
 * Can be NULL.
 *
 * @param cbf
 * The callback function for the command.
 */
void terminal_register_command_callback(
		const char* command,
		const char *help,
		const char *arg_names,
		void(*cbf)(int argc, const char **argv)) {

	int callback_num = callback_write;

	for (int i = 0;i < callback_write;i++) {
		// First check the address in case the same callback is registered more than once.
		if (callbacks[i].command == command) {
			callback_num = i;
			break;
		}

		// Check by string comparison.
		if (strcmp(callbacks[i].command, command) == 0) {
			callback_num = i;
			break;
		}
	}

	callbacks[callback_num].command = command;
	callbacks[callback_num].help = help;
	callbacks[callback_num].arg_names = arg_names;
	callbacks[callback_num].cbf = cbf;

	if (callback_num == callback_write) {
		callback_write++;
		if (callback_write >= CALLBACK_LEN) {
			callback_write = 0;
		}
	}
}
		

/*
	defaultConfig.cellBalanceDifferenceThreshold						=	0.01f;												// Start balancing @ 5mV difference, stop if below
*/
