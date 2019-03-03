#include "modFlash.h"

static const uint32_t flash_addr[FLASH_PAGES] = {
	ADDR_FLASH_PAGE_0,
	ADDR_FLASH_PAGE_1,
	ADDR_FLASH_PAGE_2,
	ADDR_FLASH_PAGE_3,
	ADDR_FLASH_PAGE_4,
	ADDR_FLASH_PAGE_5,
	ADDR_FLASH_PAGE_6,
	ADDR_FLASH_PAGE_7,
	ADDR_FLASH_PAGE_8,
	ADDR_FLASH_PAGE_9,
	ADDR_FLASH_PAGE_10,
	ADDR_FLASH_PAGE_11,
	ADDR_FLASH_PAGE_12,
	ADDR_FLASH_PAGE_13,
	ADDR_FLASH_PAGE_14,
	ADDR_FLASH_PAGE_15,
	ADDR_FLASH_PAGE_16,
	ADDR_FLASH_PAGE_17,
	ADDR_FLASH_PAGE_18,
	ADDR_FLASH_PAGE_19,
	ADDR_FLASH_PAGE_20,
	ADDR_FLASH_PAGE_21,
	ADDR_FLASH_PAGE_22,
	ADDR_FLASH_PAGE_23,
	ADDR_FLASH_PAGE_24,
	ADDR_FLASH_PAGE_25,
	ADDR_FLASH_PAGE_26,
	ADDR_FLASH_PAGE_27,
	ADDR_FLASH_PAGE_28,
	ADDR_FLASH_PAGE_29,
	ADDR_FLASH_PAGE_30,
	ADDR_FLASH_PAGE_31,
	ADDR_FLASH_PAGE_32,
	ADDR_FLASH_PAGE_33,
	ADDR_FLASH_PAGE_34,
	ADDR_FLASH_PAGE_35,
	ADDR_FLASH_PAGE_36,
	ADDR_FLASH_PAGE_37,
	ADDR_FLASH_PAGE_38,
	ADDR_FLASH_PAGE_39,
	ADDR_FLASH_PAGE_40,
	ADDR_FLASH_PAGE_41,
	ADDR_FLASH_PAGE_42,
	ADDR_FLASH_PAGE_43,
	ADDR_FLASH_PAGE_44,
	ADDR_FLASH_PAGE_45,
	ADDR_FLASH_PAGE_46,
	ADDR_FLASH_PAGE_47,
	ADDR_FLASH_PAGE_48,
	ADDR_FLASH_PAGE_49,
	ADDR_FLASH_PAGE_50,
	ADDR_FLASH_PAGE_51,
	ADDR_FLASH_PAGE_52,
	ADDR_FLASH_PAGE_53,
	ADDR_FLASH_PAGE_54,
	ADDR_FLASH_PAGE_55,
	ADDR_FLASH_PAGE_56,
	ADDR_FLASH_PAGE_57,
	ADDR_FLASH_PAGE_58,
	ADDR_FLASH_PAGE_59,
	ADDR_FLASH_PAGE_60,
	ADDR_FLASH_PAGE_61,
	ADDR_FLASH_PAGE_62,
	ADDR_FLASH_PAGE_63,
	ADDR_FLASH_PAGE_64,
	ADDR_FLASH_PAGE_65,
	ADDR_FLASH_PAGE_66,
	ADDR_FLASH_PAGE_67,
	ADDR_FLASH_PAGE_68,
	ADDR_FLASH_PAGE_69,
	ADDR_FLASH_PAGE_70,
	ADDR_FLASH_PAGE_71,
	ADDR_FLASH_PAGE_72,
	ADDR_FLASH_PAGE_73,
	ADDR_FLASH_PAGE_74,
	ADDR_FLASH_PAGE_75,
	ADDR_FLASH_PAGE_76,
	ADDR_FLASH_PAGE_77,
	ADDR_FLASH_PAGE_78,
	ADDR_FLASH_PAGE_79,
	ADDR_FLASH_PAGE_80,
	ADDR_FLASH_PAGE_81,
	ADDR_FLASH_PAGE_82,
	ADDR_FLASH_PAGE_83,
	ADDR_FLASH_PAGE_84,
	ADDR_FLASH_PAGE_85,
	ADDR_FLASH_PAGE_86,
	ADDR_FLASH_PAGE_87,
	ADDR_FLASH_PAGE_88,
	ADDR_FLASH_PAGE_89,
	ADDR_FLASH_PAGE_90,
	ADDR_FLASH_PAGE_91,
	ADDR_FLASH_PAGE_92,
	ADDR_FLASH_PAGE_93,
	ADDR_FLASH_PAGE_94,
	ADDR_FLASH_PAGE_95,
	ADDR_FLASH_PAGE_96,
	ADDR_FLASH_PAGE_97,
	ADDR_FLASH_PAGE_98,
	ADDR_FLASH_PAGE_99
};

uint16_t modFlashEraseNewAppData(uint32_t new_app_size) {	
	uint32_t page_error = 0;
	
	new_app_size += flash_addr[NEW_APP_BASE];
	
	FLASH_EraseInitTypeDef flashEraseInit;
	flashEraseInit.NbPages     = 1;
	flashEraseInit.PageAddress = flash_addr[NEW_APP_BASE];
	flashEraseInit.TypeErase   = FLASH_TYPEERASE_PAGES;

	for (int i = 0;i < NEW_APP_SECTORS;i++) {
		if (new_app_size > flash_addr[NEW_APP_BASE + i]) {
			flashEraseInit.PageAddress = flash_addr[NEW_APP_BASE + i];
			uint16_t res = HAL_FLASHEx_Erase(&flashEraseInit,&page_error);
			if (res != HAL_OK) {
				return res;
			}
		} else {
			break;
		}
	}
	
	return HAL_OK;
}

uint16_t modFlashEraseMainAppData(uint32_t new_app_size) {
	uint32_t page_error = 0;
	new_app_size += flash_addr[MAIN_APP_BASE];
	
	FLASH_EraseInitTypeDef flashEraseInit;
	flashEraseInit.NbPages     = 1;
	flashEraseInit.PageAddress = flash_addr[MAIN_APP_BASE];
	flashEraseInit.TypeErase   = FLASH_TYPEERASE_PAGES;

	for (int i = 0;i < NEW_APP_SECTORS;i++) {
		if (new_app_size > flash_addr[MAIN_APP_BASE + i]) {
			flashEraseInit.PageAddress = flash_addr[NEW_APP_BASE + i];
			uint16_t res = HAL_FLASHEx_Erase(&flashEraseInit,&page_error);
			
			if (res != HAL_OK) {
				return res;
			}
		} else {
			break;
		}
	}
	
	return HAL_OK;
}

uint16_t modFlashWriteByte(uint32_t offset, uint8_t data, bool lastByte) {
	static bool highLowByte;
	static bool newStoredData;
	static uint32_t newAddressOffset;
	static uint32_t newData;
	uint16_t returnValue = HAL_OK;
	
	if(offset != 0){
		highLowByte = (offset & 0x01) ? true : false;
		newAddressOffset = (offset & 0xFFFFFFFE);
		
		if(!highLowByte)
			newData = data;
		else
			newData |= (data << 8);
		
		newStoredData = true;
	}
	
	if((highLowByte || lastByte) && newStoredData) {
		returnValue = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,newAddressOffset,newData);
		newStoredData = false;
	}
	
	return returnValue;
}

uint16_t modFlashWriteNewAppData(uint32_t offset, uint8_t *data, uint32_t len) {
	uint16_t returnVal = HAL_OK;

	for (uint32_t i = 0;i < len;i++) {
		uint16_t res = modFlashWriteByte(flash_addr[NEW_APP_BASE] + offset + i, data[i],false);
		if (res != HAL_OK) {
			return res;
		}
	}

	return returnVal;
}

uint16_t modFlashCopyNewAppToMainApp(uint32_t offset, uint8_t *data, uint32_t len) {
	for (uint32_t i = 0;i < len;i++) {
		uint16_t res = modFlashWriteByte(flash_addr[NEW_APP_BASE] + offset + i, data[1],false);
		if (res != HAL_OK) {
			return res;
		}
	}
	
	modFlashWriteByte(0,0,true);

	return HAL_OK;
}

void modFlashJumpToBootloader(void) {
	typedef void (*pFunction)(void);
	
	modFlashWriteByte(0,0,true);
	
	__HAL_RCC_CAN1_FORCE_RESET();
	HAL_Delay(5);
	__HAL_RCC_CAN1_RELEASE_RESET();
	HAL_Delay(5);

	__HAL_RCC_USART2_FORCE_RESET();
	HAL_Delay(5);
	__HAL_RCC_USART2_RELEASE_RESET();
	HAL_Delay(5);
	
	HAL_RCC_DeInit();
	
	pFunction jump_to_bootloader;

	// Variable that will be loaded with the start address of the application
	volatile uint32_t* jump_address;
	const volatile uint32_t* bootloader_address = (volatile uint32_t*)ADDR_FLASH_PAGE_100;

	// Get jump address from application vector table
	jump_address = (volatile uint32_t*) bootloader_address[1];

	// Load this address into function pointer
	jump_to_bootloader = (pFunction) jump_address;
	
	// Clear pending interrupts
	SCB->ICSR = SCB_ICSR_PENDSVCLR_Msk;

	// Disable all interrupts
	for(int i = 0;i < 8;i++) {
		NVIC->ICER[i] = NVIC->IABR[i];
	}

	// Set stack pointer
	__set_MSP((uint32_t) (bootloader_address[0]));

	// Jump to the bootloader
	jump_to_bootloader();
}

void modFlashJumpToMainApplication(void) {
	NVIC_SystemReset();
}




