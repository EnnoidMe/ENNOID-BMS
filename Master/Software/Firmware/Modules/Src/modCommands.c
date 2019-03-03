#include "modCommands.h"

// Private variables
static uint8_t modCommandsSendBuffer[PACKET_MAX_PL_LEN];
static void(*modCommandsSendFunction)(unsigned char *data, unsigned int len) = 0;
bool jumpBootloaderTrue;
modConfigGeneralConfigStructTypedef *modCommandsGeneralConfig;
modConfigGeneralConfigStructTypedef *modCommandsToBeSendConfig;
modConfigGeneralConfigStructTypedef modCommandsConfigStorage;
modPowerElectricsPackStateTypedef   *modCommandsGeneralState;

void modCommandsInit(modPowerElectricsPackStateTypedef   *generalState,modConfigGeneralConfigStructTypedef *configPointer) {
	modCommandsGeneralConfig = configPointer;
	modCommandsGeneralState  = generalState;
	jumpBootloaderTrue = false;
}

void modCommandsSetSendFunction(void(*func)(unsigned char *data, unsigned int len)) {
	modCommandsSendFunction = func;
}

void modCommandsSendPacket(unsigned char *data, unsigned int len) {
	if (modCommandsSendFunction) {
		modCommandsSendFunction(data, len);
	}
}

void modCommandsProcessPacket(unsigned char *data, unsigned int len) {
	if (!len) {
		return;
	}

	COMM_PACKET_ID packet_id;
	int32_t ind = 0;
	uint16_t flash_res;
	uint32_t new_app_offset;
	uint32_t delayTick;
	uint8_t cellPointer;

	packet_id = (COMM_PACKET_ID) data[0];
	data++;
	len--;

	switch (packet_id) {
		case COMM_FW_VERSION:
			ind = 0;
			modCommandsSendBuffer[ind++] = COMM_FW_VERSION;
			modCommandsSendBuffer[ind++] = FW_VERSION_MAJOR;
			modCommandsSendBuffer[ind++] = FW_VERSION_MINOR;
			strcpy((char*)(modCommandsSendBuffer + ind), HW_NAME);
			ind += strlen(HW_NAME) + 1;
			memcpy(modCommandsSendBuffer + ind, STM32_UUID_8, 12);
			ind += 12;

			modCommandsSendPacket(modCommandsSendBuffer, ind);
			break;
		case COMM_JUMP_TO_BOOTLOADER:
			jumpBootloaderTrue = true;
			delayTick = HAL_GetTick();
			break;
		case COMM_ERASE_NEW_APP:
			ind = 0;
			flash_res = modFlashEraseNewAppData(libBufferGet_uint32(data, &ind));

			ind = 0;
			modCommandsSendBuffer[ind++] = COMM_ERASE_NEW_APP;
			modCommandsSendBuffer[ind++] = flash_res == HAL_OK ? true : false;
			modCommandsSendPacket(modCommandsSendBuffer, ind);
			break;
		case COMM_WRITE_NEW_APP_DATA:
			ind = 0;
			new_app_offset = libBufferGet_uint32(data, &ind);
			flash_res = modFlashWriteNewAppData(new_app_offset, data + ind, len - ind);

			ind = 0;
			modCommandsSendBuffer[ind++] = COMM_WRITE_NEW_APP_DATA;
			modCommandsSendBuffer[ind++] = flash_res == HAL_OK ? 1 : 0;
			modCommandsSendPacket(modCommandsSendBuffer, ind);
			break;
		case COMM_GET_VALUES:
			ind = 0;
			modCommandsSendBuffer[ind++] = COMM_GET_VALUES;
		
		  libBufferAppend_float32(modCommandsSendBuffer, modCommandsGeneralState->packVoltage, 1e3, &ind);
		  libBufferAppend_float32(modCommandsSendBuffer, modCommandsGeneralState->packCurrent, 1e3, &ind);
		
		  libBufferAppend_uint8(modCommandsSendBuffer, (uint8_t)round(modCommandsGeneralState->SoC), &ind);
		
		  libBufferAppend_float32(modCommandsSendBuffer, modCommandsGeneralState->cellVoltageHigh, 1e3, &ind);
		  libBufferAppend_float32(modCommandsSendBuffer, modCommandsGeneralState->cellVoltageAverage, 1e3, &ind);
		  libBufferAppend_float32(modCommandsSendBuffer, modCommandsGeneralState->cellVoltageLow, 1e3, &ind);
		  libBufferAppend_float32(modCommandsSendBuffer, modCommandsGeneralState->cellVoltageMisMatch, 1e3, &ind);
		
		  libBufferAppend_float16(modCommandsSendBuffer, modCommandsGeneralState->loCurrentLoadVoltage, 1e2, &ind);
		  libBufferAppend_float16(modCommandsSendBuffer, modCommandsGeneralState->loCurrentLoadCurrent, 1e2, &ind);
		  libBufferAppend_float16(modCommandsSendBuffer, modCommandsGeneralState->hiCurrentLoadVoltage, 1e2, &ind);
		  libBufferAppend_float16(modCommandsSendBuffer, modCommandsGeneralState->hiCurrentLoadCurrent, 1e2, &ind);
		  libBufferAppend_float16(modCommandsSendBuffer, modCommandsGeneralState->auxVoltage, 1e2, &ind);
		  libBufferAppend_float16(modCommandsSendBuffer, modCommandsGeneralState->auxCurrent, 1e2, &ind);
		
			libBufferAppend_float16(modCommandsSendBuffer, modCommandsGeneralState->tempBatteryHigh, 1e1, &ind);
			libBufferAppend_float16(modCommandsSendBuffer, modCommandsGeneralState->tempBatteryAverage, 1e1, &ind);
			libBufferAppend_float16(modCommandsSendBuffer, modCommandsGeneralState->tempBMSHigh, 1e1, &ind);
			libBufferAppend_float16(modCommandsSendBuffer, modCommandsGeneralState->tempBMSAverage, 1e1, &ind);
			
			libBufferAppend_uint8(modCommandsSendBuffer, (uint8_t)modCommandsGeneralState->operationalState, &ind);
			libBufferAppend_uint8(modCommandsSendBuffer, (uint8_t)modCommandsGeneralState->chargeBalanceActive, &ind);  // Indicator for charging
			
			libBufferAppend_uint8(modCommandsSendBuffer, 0, &ind); // Future faultstate
		
			modCommandsSendBuffer[ind++] = modCommandsGeneralConfig->CANID;
			modCommandsSendPacket(modCommandsSendBuffer, ind);
		
			break;
    case COMM_GET_BMS_CELLS:
			ind = 0;
			modCommandsSendBuffer[ind++] = COMM_GET_BMS_CELLS;
		
		  libBufferAppend_uint8(modCommandsSendBuffer, modCommandsGeneralConfig->noOfCells, &ind);                // Cell count
		  for(cellPointer = 0; cellPointer < modCommandsGeneralConfig->noOfCells; cellPointer++){
				if(modCommandsGeneralState->cellBalanceResistorEnableMask & (1 << cellPointer))
				  libBufferAppend_float16(modCommandsSendBuffer, modCommandsGeneralState->cellVoltagesIndividual[cellPointer].cellVoltage*-1.0f, 1e3, &ind);    // Individual cells
				else
					libBufferAppend_float16(modCommandsSendBuffer, modCommandsGeneralState->cellVoltagesIndividual[cellPointer].cellVoltage, 1e3, &ind);          // Individual cells
			}
		
			modCommandsSendBuffer[ind++] = modCommandsGeneralConfig->CANID;
			modCommandsSendPacket(modCommandsSendBuffer, ind);
			break;
		case COMM_SET_MCCONF:
			ind = 0;
		  modCommandsGeneralConfig->noOfCells                      = libBufferGet_uint8(data,&ind);
			modCommandsGeneralConfig->batteryCapacity                = libBufferGet_float32_auto(data,&ind);
			modCommandsGeneralConfig->cellHardUnderVoltage           = libBufferGet_float32_auto(data,&ind);
			modCommandsGeneralConfig->cellHardOverVoltage            = libBufferGet_float32_auto(data,&ind);
			modCommandsGeneralConfig->cellLCSoftUnderVoltage         = libBufferGet_float32_auto(data,&ind);
			modCommandsGeneralConfig->cellHCSoftUnderVoltage         = libBufferGet_float32_auto(data,&ind);
			modCommandsGeneralConfig->cellSoftOverVoltage            = libBufferGet_float32_auto(data,&ind);
			modCommandsGeneralConfig->cellBalanceDifferenceThreshold = libBufferGet_float32_auto(data,&ind);
			modCommandsGeneralConfig->cellBalanceStart               = libBufferGet_float32_auto(data,&ind);
			modCommandsGeneralConfig->cellThrottleUpperStart         = libBufferGet_float32_auto(data,&ind);
			modCommandsGeneralConfig->cellThrottleLowerStart         = libBufferGet_float32_auto(data,&ind);
			modCommandsGeneralConfig->cellThrottleUpperMargin        = libBufferGet_float32_auto(data,&ind);
			modCommandsGeneralConfig->cellThrottleLowerMargin        = libBufferGet_float32_auto(data,&ind);
			modCommandsGeneralConfig->shuntLCFactor                  = libBufferGet_float32_auto(data,&ind);
			modCommandsGeneralConfig->shuntLCOffset                  = libBufferGet_int16(data,&ind);
			modCommandsGeneralConfig->shuntHCFactor	                 = libBufferGet_float32_auto(data,&ind);
			modCommandsGeneralConfig->shuntHCOffset                  = libBufferGet_int16(data,&ind);
		  modCommandsGeneralConfig->throttleChargeIncreaseRate     = libBufferGet_uint8(data,&ind);
		  modCommandsGeneralConfig->throttleDisChargeIncreaseRate  = libBufferGet_uint8(data,&ind);
		  modCommandsGeneralConfig->cellBalanceUpdateInterval      = libBufferGet_uint32(data,&ind);
		  modCommandsGeneralConfig->maxSimultaneousDischargingCells = libBufferGet_uint8(data,&ind);
		  modCommandsGeneralConfig->timeoutDischargeRetry          = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->hysteresisDischarge            = libBufferGet_float32_auto(data,&ind);
		  modCommandsGeneralConfig->timeoutChargeRetry             = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->hysteresisCharge               = libBufferGet_float32_auto(data,&ind);
			modCommandsGeneralConfig->timeoutChargeCompleted         = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->timeoutChargingCompletedMinimalMismatch = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->maxMismatchThreshold           = libBufferGet_float32_auto(data,&ind);
			modCommandsGeneralConfig->chargerEnabledThreshold        = libBufferGet_float32_auto(data,&ind);
			modCommandsGeneralConfig->timeoutChargerDisconnected     = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->minimalPrechargePercentage     = libBufferGet_float32_auto(data,&ind);
			modCommandsGeneralConfig->timeoutLCPreCharge             = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->maxAllowedCurrent              = libBufferGet_float32_auto(data,&ind);
			modCommandsGeneralConfig->displayTimeoutBatteryDead      = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->displayTimeoutBatteryError     = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->displayTimeoutBatteryErrorPreCharge = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->displayTimeoutSplashScreen     = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->maxUnderAndOverVoltageErrorCount = libBufferGet_uint8(data,&ind);
			modCommandsGeneralConfig->notUsedCurrentThreshold        = libBufferGet_float32_auto(data,&ind);
			modCommandsGeneralConfig->notUsedTimeout                 = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->stateOfChargeStoreInterval     = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->CANID                          = libBufferGet_uint8(data,&ind);
			modCommandsGeneralConfig->CANIDStyle                     = libBufferGet_uint8(data,&ind);
			modCommandsGeneralConfig->emitStatusOverCAN              = libBufferGet_uint8(data,&ind);
			modCommandsGeneralConfig->tempEnableMaskBMS              = libBufferGet_uint16(data,&ind);
			modCommandsGeneralConfig->tempEnableMaskBattery          = libBufferGet_uint16(data,&ind);
		  modCommandsGeneralConfig->LCUseDischarge                 = libBufferGet_uint8(data,&ind);
			modCommandsGeneralConfig->LCUsePrecharge                 = libBufferGet_uint8(data,&ind);
			modCommandsGeneralConfig->allowChargingDuringDischarge   = libBufferGet_uint8(data,&ind);
			modCommandsGeneralConfig->allowForceOn                   = libBufferGet_uint8(data,&ind);
			modCommandsGeneralConfig->pulseToggleButton              = libBufferGet_uint8(data,&ind);
			modCommandsGeneralConfig->togglePowerModeDirectHCDelay   = libBufferGet_uint8(data,&ind);
			modCommandsGeneralConfig->useCANSafetyInput              = libBufferGet_uint8(data,&ind);
			modCommandsGeneralConfig->useCANDelayedPowerDown         = libBufferGet_uint8(data,&ind);
			modCommandsGeneralConfig->HCUseRelay                     = libBufferGet_uint8(data,&ind);
			modCommandsGeneralConfig->HCUsePrecharge                 = libBufferGet_uint8(data,&ind);
			modCommandsGeneralConfig->timeoutHCPreCharge             = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->timeoutHCPreChargeRetryInterval= libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->timeoutHCRelayOverlap          = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->NTCTopResistor[modConfigNTCGroupLTCExt]         = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->NTC25DegResistance[modConfigNTCGroupLTCExt]     = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->NTCBetaFactor[modConfigNTCGroupLTCExt]          = libBufferGet_uint16(data,&ind);
			modCommandsGeneralConfig->NTCTopResistor[modConfigNTCGroupMasterPCB]      = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->NTC25DegResistance[modConfigNTCGroupMasterPCB]  = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->NTCBetaFactor[modConfigNTCGroupMasterPCB]       = libBufferGet_uint16(data,&ind);
			modCommandsGeneralConfig->NTCTopResistor[modConfigNTCGroupHiAmpExt]       = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->NTC25DegResistance[modConfigNTCGroupHiAmpExt]   = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->NTCBetaFactor[modConfigNTCGroupHiAmpExt]        = libBufferGet_uint16(data,&ind);
			modCommandsGeneralConfig->NTCTopResistor[modConfigNTCGroupHiAmpPCB]       = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->NTC25DegResistance[modConfigNTCGroupHiAmpPCB]   = libBufferGet_uint32(data,&ind);
			modCommandsGeneralConfig->NTCBetaFactor[modConfigNTCGroupHiAmpPCB]        = libBufferGet_uint16(data,&ind);
		
			ind = 0;
			modCommandsSendBuffer[ind++] = packet_id;
			modCommandsSendPacket(modCommandsSendBuffer, ind);
		
			break;
		case COMM_GET_MCCONF:
		case COMM_GET_MCCONF_DEFAULT:
      if(packet_id == COMM_GET_MCCONF_DEFAULT){
				modConfigLoadDefaultConfig(&modCommandsConfigStorage);
				modCommandsToBeSendConfig = &modCommandsConfigStorage;
			}else{
				modCommandsToBeSendConfig = modCommandsGeneralConfig;
			}
		
      ind = 0;
		  modCommandsSendBuffer[ind++] = packet_id;
		  
		  libBufferAppend_uint8(modCommandsSendBuffer,modCommandsToBeSendConfig->noOfCells,&ind);
		  libBufferAppend_float32_auto(modCommandsSendBuffer,modCommandsToBeSendConfig->batteryCapacity,&ind);
		  libBufferAppend_float32_auto(modCommandsSendBuffer,modCommandsToBeSendConfig->cellHardUnderVoltage,&ind);
		  libBufferAppend_float32_auto(modCommandsSendBuffer,modCommandsToBeSendConfig->cellHardOverVoltage,&ind);
		  libBufferAppend_float32_auto(modCommandsSendBuffer,modCommandsToBeSendConfig->cellLCSoftUnderVoltage,&ind);
		  libBufferAppend_float32_auto(modCommandsSendBuffer,modCommandsToBeSendConfig->cellHCSoftUnderVoltage,&ind);
		  libBufferAppend_float32_auto(modCommandsSendBuffer,modCommandsToBeSendConfig->cellSoftOverVoltage,&ind);
		  libBufferAppend_float32_auto(modCommandsSendBuffer,modCommandsToBeSendConfig->cellBalanceDifferenceThreshold,&ind);
		  libBufferAppend_float32_auto(modCommandsSendBuffer,modCommandsToBeSendConfig->cellBalanceStart,&ind);
		  libBufferAppend_float32_auto(modCommandsSendBuffer,modCommandsToBeSendConfig->cellThrottleUpperStart,&ind);
		  libBufferAppend_float32_auto(modCommandsSendBuffer,modCommandsToBeSendConfig->cellThrottleLowerStart,&ind);
		  libBufferAppend_float32_auto(modCommandsSendBuffer,modCommandsToBeSendConfig->cellThrottleUpperMargin,&ind);
		  libBufferAppend_float32_auto(modCommandsSendBuffer,modCommandsToBeSendConfig->cellThrottleLowerMargin,&ind);
			
			libBufferAppend_float32_auto(modCommandsSendBuffer,modCommandsGeneralConfig->shuntLCFactor,&ind);
			libBufferAppend_int16(modCommandsSendBuffer,modCommandsGeneralConfig->shuntLCOffset,&ind);
			libBufferAppend_float32_auto(modCommandsSendBuffer,modCommandsGeneralConfig->shuntHCFactor,&ind);
			libBufferAppend_int16(modCommandsSendBuffer,modCommandsGeneralConfig->shuntHCOffset,&ind);
			
		  libBufferAppend_uint8(modCommandsSendBuffer,modCommandsToBeSendConfig->throttleChargeIncreaseRate,&ind);
		  libBufferAppend_uint8(modCommandsSendBuffer,modCommandsToBeSendConfig->throttleDisChargeIncreaseRate,&ind);
		  libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->cellBalanceUpdateInterval,&ind);
		  libBufferAppend_uint8(modCommandsSendBuffer,modCommandsToBeSendConfig->maxSimultaneousDischargingCells,&ind);
		  libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->timeoutDischargeRetry,&ind);
			libBufferAppend_float32_auto(modCommandsSendBuffer,modCommandsToBeSendConfig->hysteresisDischarge,&ind);
		  libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->timeoutChargeRetry,&ind);
			libBufferAppend_float32_auto(modCommandsSendBuffer,modCommandsToBeSendConfig->hysteresisCharge,&ind);
			libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->timeoutChargeCompleted,&ind);
			libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->timeoutChargingCompletedMinimalMismatch,&ind);
			libBufferAppend_float32_auto(modCommandsSendBuffer,modCommandsToBeSendConfig->maxMismatchThreshold,&ind);
			libBufferAppend_float32_auto(modCommandsSendBuffer,modCommandsToBeSendConfig->chargerEnabledThreshold,&ind);
			libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->timeoutChargerDisconnected,&ind);
			libBufferAppend_float32_auto(modCommandsSendBuffer,modCommandsToBeSendConfig->minimalPrechargePercentage,&ind);
			libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->timeoutLCPreCharge,&ind);
			libBufferAppend_float32_auto(modCommandsSendBuffer,modCommandsToBeSendConfig->maxAllowedCurrent,&ind);
			libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->displayTimeoutBatteryDead,&ind);
			libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->displayTimeoutBatteryError,&ind);
			libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->displayTimeoutBatteryErrorPreCharge,&ind);
			libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->displayTimeoutSplashScreen,&ind);
			libBufferAppend_uint8(modCommandsSendBuffer,modCommandsToBeSendConfig->maxUnderAndOverVoltageErrorCount,&ind);
			libBufferAppend_float32_auto(modCommandsSendBuffer,modCommandsToBeSendConfig->notUsedCurrentThreshold,&ind);
			libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->notUsedTimeout,&ind);
			libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->stateOfChargeStoreInterval,&ind);
			libBufferAppend_uint8(modCommandsSendBuffer,modCommandsToBeSendConfig->CANID,&ind);
			libBufferAppend_uint8(modCommandsSendBuffer,modCommandsToBeSendConfig->CANIDStyle,&ind);
			libBufferAppend_uint8(modCommandsSendBuffer,modCommandsGeneralConfig->emitStatusOverCAN,&ind);
			libBufferAppend_uint16(modCommandsSendBuffer,modCommandsToBeSendConfig->tempEnableMaskBMS,&ind);
			libBufferAppend_uint16(modCommandsSendBuffer,modCommandsToBeSendConfig->tempEnableMaskBattery,&ind);
		  libBufferAppend_uint8(modCommandsSendBuffer,modCommandsToBeSendConfig->LCUseDischarge,&ind);
			libBufferAppend_uint8(modCommandsSendBuffer,modCommandsToBeSendConfig->LCUsePrecharge,&ind);
			libBufferAppend_uint8(modCommandsSendBuffer,modCommandsToBeSendConfig->allowChargingDuringDischarge,&ind);
			libBufferAppend_uint8(modCommandsSendBuffer,modCommandsToBeSendConfig->allowForceOn,&ind);
			libBufferAppend_uint8(modCommandsSendBuffer,modCommandsToBeSendConfig->pulseToggleButton,&ind);
			libBufferAppend_uint8(modCommandsSendBuffer,modCommandsToBeSendConfig->togglePowerModeDirectHCDelay,&ind);
			libBufferAppend_uint8(modCommandsSendBuffer,modCommandsToBeSendConfig->useCANSafetyInput,&ind);
			libBufferAppend_uint8(modCommandsSendBuffer,modCommandsToBeSendConfig->useCANDelayedPowerDown,&ind);
			libBufferAppend_uint8(modCommandsSendBuffer,modCommandsToBeSendConfig->HCUseRelay,&ind);
			libBufferAppend_uint8(modCommandsSendBuffer,modCommandsToBeSendConfig->HCUsePrecharge,&ind);
			libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->timeoutHCPreCharge,&ind);
			libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->timeoutHCPreChargeRetryInterval,&ind);
			libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->timeoutHCRelayOverlap,&ind);
			libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->NTCTopResistor[modConfigNTCGroupLTCExt],&ind);
			libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->NTC25DegResistance[modConfigNTCGroupLTCExt],&ind);
			libBufferAppend_uint16(modCommandsSendBuffer,modCommandsToBeSendConfig->NTCBetaFactor[modConfigNTCGroupLTCExt],&ind);
			libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->NTCTopResistor[modConfigNTCGroupMasterPCB],&ind);
			libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->NTC25DegResistance[modConfigNTCGroupMasterPCB],&ind);
			libBufferAppend_uint16(modCommandsSendBuffer,modCommandsToBeSendConfig->NTCBetaFactor[modConfigNTCGroupMasterPCB],&ind);
			libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->NTCTopResistor[modConfigNTCGroupHiAmpExt],&ind);
			libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->NTC25DegResistance[modConfigNTCGroupHiAmpExt],&ind);
			libBufferAppend_uint16(modCommandsSendBuffer,modCommandsToBeSendConfig->NTCBetaFactor[modConfigNTCGroupHiAmpExt],&ind);
			libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->NTCTopResistor[modConfigNTCGroupHiAmpPCB],&ind);
			libBufferAppend_uint32(modCommandsSendBuffer,modCommandsToBeSendConfig->NTC25DegResistance[modConfigNTCGroupHiAmpPCB],&ind);
			libBufferAppend_uint16(modCommandsSendBuffer,modCommandsToBeSendConfig->NTCBetaFactor[modConfigNTCGroupHiAmpPCB],&ind);

		  modCommandsSendPacket(modCommandsSendBuffer, ind);
		
			break;
		case COMM_TERMINAL_CMD:
		  data[len] = '\0';
		  terminal_process_string((char*)data);
			break;
		case COMM_REBOOT:
			modCommandsJumpToMainApplication();
			break;
		case COMM_ALIVE:
			break;
		case COMM_FORWARD_CAN:
			modCANSendBuffer(data[0], data + 1, len - 1, false);
			break;
		case COMM_STORE_BMS_CONF:
			modConfigStoreConfig();
		
			ind = 0;
			modCommandsSendBuffer[ind++] = packet_id;
			modCommandsSendPacket(modCommandsSendBuffer, ind);
			break;
		default:
			break;
	}
	
	if(modDelayTick1ms(&delayTick,1000) && jumpBootloaderTrue)
		modFlashJumpToBootloader();
}

void modCommandsPrintf(const char* format, ...) {
	va_list arg;
	va_start (arg, format);
	int len;
	static char print_buffer[255];

	print_buffer[0] = COMM_PRINT;
	len = vsnprintf(print_buffer+1, 254, format, arg);
	va_end (arg);

	if(len > 0) {
		modCommandsSendPacket((unsigned char*)print_buffer, (len<254)? len+1: 255);
	}
}


void modCommandsJumpToMainApplication(void) {
	NVIC_SystemReset();
}
