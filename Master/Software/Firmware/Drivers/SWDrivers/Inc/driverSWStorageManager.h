#ifndef __DRIVERSWSTORAGEMANAGER_H
#define __DRIVERSWSTORAGEMANAGER_H

#include "stdint.h"
#include "stdbool.h"
#include "driverHWEEPROM.h"

extern bool driverSWStorageManagerConfigEmpty;
extern bool driverSWStorageManagerStateOfChargeEmpty;
extern uint16_t driverSWStorageManagerConfigStructSize;
extern uint16_t driverSWStorageManagerStateOfChargeStructSize;

typedef enum {
	STORAGE_CONFIG = 0,
	STORAGE_STATEOFCHARGE,
} StorageLocationTypedef;

void driverSWStorageManagerInit(void);
bool driverSWStorageManagerEraseData(void);
bool driverSWStorageManagerStoreStruct(void *configStruct, StorageLocationTypedef storageLocation);
bool driverSWStorageManagerGetStruct(void *configStruct, StorageLocationTypedef storageLocation);
uint16_t driverSWStorageManagerGetOffsetFromLocation(StorageLocationTypedef storageLocation);
uint16_t driverSWStorageManagerGetStructSize(StorageLocationTypedef storageLocation);

#endif
