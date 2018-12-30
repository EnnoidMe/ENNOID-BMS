/**
 *
 * @copyright &copy; 2010 - 2018, Fraunhofer-Gesellschaft zur Foerderung der
 *  angewandten Forschung e.V. All rights reserved.
 *
 * BSD 3-Clause License
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1.  Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of the copyright holder nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * We kindly request you to use one or more of the following phrases to refer
 * to foxBMS in your hardware, software, documentation or advertising
 * materials:
 *
 * &Prime;This product uses parts of foxBMS&reg;&Prime;
 *
 * &Prime;This product includes parts of foxBMS&reg;&Prime;
 *
 * &Prime;This product is derived from foxBMS&reg;&Prime;
 *
 */

/**
 * @file    eepr_cfg.c
 * @author  foxBMS Team
 * @date    23.02.2016 (date of creation)
 * @ingroup DRIVERS_CONF,EXT_PER
 * @prefix  EEPR
 *
 * @brief   Configuration for the driver for the storage in the EEPROM memory
 *
 */

/*================== Includes =============================================*/
#include "eepr_cfg.h"

#include "nvram_cfg.h"
#include "rtc.h"
#include "spi.h"
#include <string.h>

/*================== Macros and Definitions ===============================*/

/**
 * EEPR_VERSIONNUMBERMAYOR
 *
 * A change of major number usually indicates a change of memory layout in the eeprom
 * Any change of data alignment could lead to data loss if not handled by a dedicated function
 *
 * */
#define EEPR_VERSIONNUMBERMAYOR             0

#define EEPR_VERSIONNUMBERMINOR             1

#define EEPR_HEADERPATTERN                  0xAA551234

/*================== Constant and Variable Definitions ====================*/


/**
 * Version number of EEPR Software
 */
const EEPR_HEADER_s eepr_header_default = {
        .versionnumbermajor = EEPR_VERSIONNUMBERMAYOR,
        .versionnumberminor = EEPR_VERSIONNUMBERMINOR,
        .eepr_headerpattern = EEPR_HEADERPATTERN,
        .dummy2 = 0,
        .dummy3 = 0,
        .chksum = 0
};

EEPR_HEADER_s eepr_header;

const EEPR_CALIB_STATISTICS_s defaultcalibstatistics = {
        .u_cell_max = 0,
        .u_cell_min = 0,
        .u_temperature_max = 0,
        .u_temperature_min = 0,
        .current_max = 0,
        .current_min = 0,
};

/**
 * Version number of EEPR Software
 */
const EEPR_BOARD_INFO_s eepr_board_info_default = {
        .network_nodeID     = EEPR_DEFAULT_NETWORK_ID,            /* set default node ID (network address) */
        .chksum = 0
};

EEPR_BOARD_INFO_s eepr_board_info;

EEPR_CH_CFG_s eepr_ch_cfg[] = {
/* Hardware Protected Channels (last quarter of address area)  */
        /*  EEPROM CHANNELS: HW/SW-SYSTEM */
        {0x3000, sizeof(EEPR_HEADER_s),           EEPR_CH_HEADER,          0x3000 + sizeof(EEPR_HEADER_s) - 4,           EEPR_SW_WRITE_UNPROTECTED, (uint8_t*)&eepr_header},
/*       {0x3020, ...}, */
        {0x3F00, sizeof(EEPR_BOARD_INFO_s),       EEPR_CH_BOARD_INFO,      0x3F80 + sizeof(EEPR_BOARD_INFO_s) - 4,       EEPR_SW_WRITE_UNPROTECTED, (uint8_t*)&eepr_board_info},
        {0x3F80, 0x80,                            EEPR_CH_HARDWARE_INFO,      0x3F80 + 0x80 - 4,                         EEPR_SW_WRITE_PROTECTED,   (NULL_PTR)},
/* Channels placed in Hardware Protection disabled address area  */
        /*  EEPROM CHANNELS: APPLICATION */
/*       {0x0000, 00x54,    reserved to maintain previous formatted data,   0x0080 + sizeof(BKPSRAM_CH_NVSOC_s) - 4,      EEPR_SW_WRITE_UNPROTECTED, (uint8_t*)&bkpsram_nvsoc }, */
        {0x0080, sizeof(NVRAM_CH_OP_HOURS_s),   EEPR_CH_OPERATING_HOURS, 0x0080 + sizeof(NVRAM_CH_OP_HOURS_s) - 4,   EEPR_SW_WRITE_UNPROTECTED, (uint8_t*)&bkpsram_operating_hours},
        {0x0098, sizeof(NVRAM_CH_NVSOC_s),      EEPR_CH_NVSOC,           0x0098 + sizeof(NVRAM_CH_NVSOC_s) - 4,      EEPR_SW_WRITE_UNPROTECTED, (uint8_t*)&bkpsram_nvsoc },
        {0x00C0, sizeof(NVRRAM_CH_CONT_COUNT_s), EEPR_CH_CONTACTOR,       0x00C0 + sizeof(NVRRAM_CH_CONT_COUNT_s) - 4, EEPR_SW_WRITE_UNPROTECTED, (uint8_t*)&bkpsram_contactors_count},
/*         {0x0110, sizeof(EEPR_CALIB_STATISTICS_s), EEPR_CH_STATISTICS,      0x0100 + sizeof(EEPR_CALIB_STATISTICS_s) - 4, EEPR_SW_WRITE_UNPROTECTED, (NULL_PTR)}, */
        /*  FREE EEPRROMS CHANNELS (for future use) */
/*         {0x0130, 0x70,                            EEPR_CH_USER_DATA,       0x0120 + 0x70 - 4,                            EEPR_SW_WRITE_UNPROTECTED, (NULL_PTR)}, */
/*       {0x0180, ...}, */
};

/* In case of compile errors in the following dummy-declarations,
 *  - check configuration in eepr_ch_cfg[]
 *  - modify the following dummy-declarations according to data lenght of each channel
 *  - modify (increment) the eeprom version versionnumbermajor
 *  - backup and recover the data in eeprom
 *
 * Note: A modification of channel address, data length or data structure will lead to data loss if data transfer
 * is not handled (e.g. manually or automatically in EEPR_FormatCheck() )
 *
 * */
extern uint8_t compiler_throw_an_error_1[(sizeof(EEPR_HEADER_s) == 0x20)?1:-1];  /* EEPROM FORMAT ERROR! Change of data size. Please note comment above!!! */
extern uint8_t compiler_throw_an_error_2[(sizeof(EEPR_BOARD_INFO_s) == 0x20)?1:-1];  /* EEPROM FORMAT ERROR! Change of data size. Please note comment above!!! */
extern uint8_t compiler_throw_an_error_3[(0x80 == 0x80)?1:-1];  /* EEPROM FORMAT ERROR! Change of data size. Please note comment above!!! */
extern uint8_t compiler_throw_an_error_4[(sizeof(NVRAM_CH_OP_HOURS_s) == 0x18)?1:-1];  /* EEPROM FORMAT ERROR! Change of data size. Please note comment above!!! */
extern uint8_t compiler_throw_an_error_5[(sizeof(NVRAM_CH_NVSOC_s) == 0x28)?1:-1];  /* EEPROM FORMAT ERROR! Change of data size. Please note comment above!!! */
extern uint8_t compiler_throw_an_error_6[(sizeof(NVRRAM_CH_CONT_COUNT_s) == 0x48)?1:-1];  /* EEPROM FORMAT ERROR! Change of data size. Please note comment above!!! */
extern uint8_t compiler_throw_an_error_7[(sizeof(EEPR_CALIB_STATISTICS_s) == 0x20)?1:-1];  /* EEPROM FORMAT ERROR! Change of data size. Please note comment above!!! */
extern uint8_t compiler_throw_an_error_8[(0x70 == 0x70)?1:-1];  /* EEPROM FORMAT ERROR! Change of data size. Please note comment above!!! */


const uint8_t eepr_nr_of_channels = sizeof(eepr_ch_cfg)/sizeof(eepr_ch_cfg[0]);

/* write buffer for calibration data in eeprom */
uint8_t eepr_WR_RD_buffer[EEPR_CH_MAXLENGTH];


/* write buffer checksum */
uint16_t eepr_WR_RD_Chksum=0xFFFF;

uint8_t eepr_spi_rxbuf[EEPR_TXBUF_LENGTH];
uint16_t eepr_spi_rxoffset;

/** Dirty Flag indicates a modification of data in BKPSRAM for the corresponding channel. Thus Eeprom and BKPSRAM data are not consistent */
uint8_t MEM_BKP_SRAM eepr_ch_dirtyflag[EEPR_CHANNEL_MAX_NR];
uint8_t MEM_BKP_SRAM eepr_ch_read_req_flag[EEPR_CHANNEL_MAX_NR];

/** Buffer used for eeprom reads data when double-buffering with BKPSRAM is not used */
uint8_t MEM_BKP_SRAM eepr_bkpsram_buffer[EEPR_CH_MAXLENGTH];

/*================== Function Prototypes ==================================*/


/*================== Function Implementations =============================*/
void EEPR_delete_bkpsrambuffer(EEPR_CHANNEL_ID_TYPE_e eepr_channel){
    uint16_t counter;
    for(counter = 0; counter < eepr_ch_cfg[eepr_channel].length; counter++){
        eepr_bkpsram_buffer[counter] = 0xFF;
    }
}

void EEPR_SetChDirtyFlag(EEPR_CHANNEL_ID_TYPE_e eepr_channel) {
    eepr_ch_dirtyflag[eepr_channel] = 0;
}

void EEPR_SetChReadReqFlag(EEPR_CHANNEL_ID_TYPE_e eepr_channel) {
    eepr_ch_read_req_flag[eepr_channel] = 0;
}


void EEPR_RemoveChDirtyFlag(EEPR_CHANNEL_ID_TYPE_e eepr_channel) {
    eepr_ch_dirtyflag[eepr_channel] = 1;
}

void EEPR_RemoveChReadReqFlag(EEPR_CHANNEL_ID_TYPE_e eepr_channel) {
    eepr_ch_read_req_flag[eepr_channel] = 1;
}


EEPR_RETURNTYPE_e SPI_ReceiveData(uint8_t* buffer, uint16_t length) {
    EEPR_RETURNTYPE_e retVal = EEPR_ERROR;

    memcpy(buffer, eepr_spi_rxbuf + eepr_spi_rxoffset, length);
    if (spi_devices[1].State  ==  HAL_SPI_STATE_READY) {
        retVal = EEPR_OK;
    } else {
        retVal = EEPR_ERROR;
    }
    return retVal;
}


EEPR_RETURNTYPE_e SPI_SendData(uint8_t* data, uint16_t length, uint16_t receiveoffset) {

    EEPR_RETURNTYPE_e retVal = EEPR_ERROR;
    HAL_StatusTypeDef eepr_spi_halstate = HAL_ERROR;

    IO_WritePin(IO_PIN_MCU_0_DATA_STORAGE_EEPROM_SPI_NSS, IO_PIN_RESET);  /* FIXME use chip select/unselect functions fromSPI module! */

    eepr_spi_halstate = HAL_SPI_TransmitReceive_IT(&spi_devices[1], data, eepr_spi_rxbuf, length);
    eepr_spi_rxoffset = receiveoffset;

    if(eepr_spi_halstate == HAL_OK) {
        retVal = EEPR_OK;
    } else {
        retVal = EEPR_ERROR;
    }

    return retVal;
}


uint16_t EEPR_CalcChecksum(uint8_t*dataptr, uint16_t byte_len) {
    uint16_t u16_chksum = 0;

    for (;byte_len > 0;byte_len--) {
        u16_chksum += *dataptr++;
    }
    return (u16_chksum);
}


STD_RETURN_TYPE_e EEPR_BkpSramCheckChksum(EEPR_CHANNEL_ID_TYPE_e eepr_channel, uint8_t *ptr)
{
    STD_RETURN_TYPE_e retVal = E_NOT_OK;
    uint8_t*dataptr = eepr_ch_cfg[eepr_channel].bkpsramptr;
    uint16_t byte_len = eepr_ch_cfg[eepr_channel].length;

    if(dataptr == NULL_PTR) {
        if(ptr != NULL_PTR) {
            dataptr = ptr;
        } else {
            retVal = E_NOT_OK;
        }
    } else if (EEPR_CalcChecksum(dataptr, byte_len - 4) == *((uint32_t*)(dataptr + byte_len - 4))) {
        retVal = E_OK;  /* checksum ok */
    }
    return (retVal);
}


void EEPR_SetDefaultValue(EEPR_CHANNEL_ID_TYPE_e eepr_channel) {
    switch (eepr_channel) {

        case EEPR_CH_NVSOC:
            bkpsram_nvsoc.data = default_nvsoc.data;
            bkpsram_nvsoc.previous_timestamp = bkpsram_nvsoc.timestamp;
            bkpsram_nvsoc.timestamp = RTC_getUnixTime();
            bkpsram_nvsoc.checksum = EEPR_CalcChecksum((uint8_t*)(&bkpsram_nvsoc),sizeof(bkpsram_nvsoc)-4);
            EEPR_SetChDirtyFlag(EEPR_CH_NVSOC);
            break;

        case EEPR_CH_CONTACTOR:
            bkpsram_contactors_count.data = default_contactors_count.data;
            bkpsram_contactors_count.previous_timestamp = bkpsram_contactors_count.timestamp;
            bkpsram_contactors_count.timestamp = RTC_getUnixTime();
            bkpsram_contactors_count.checksum = EEPR_CalcChecksum((uint8_t*)(&bkpsram_contactors_count),sizeof(bkpsram_contactors_count)-4);
            EEPR_SetChDirtyFlag(EEPR_CH_CONTACTOR);
            break;

        case EEPR_CH_OPERATING_HOURS:
            bkpsram_operating_hours.data = default_operating_hours.data;
            bkpsram_operating_hours.previous_timestamp = bkpsram_operating_hours.timestamp;
            bkpsram_operating_hours.timestamp = RTC_getUnixTime();
            bkpsram_operating_hours.checksum = EEPR_CalcChecksum((uint8_t*)(&bkpsram_operating_hours),sizeof(bkpsram_operating_hours)-4);
            EEPR_SetChDirtyFlag(EEPR_CH_OPERATING_HOURS);
            break;

        case EEPR_CH_HEADER:
            eepr_header = eepr_header_default;
            eepr_header.chksum = EEPR_CalcChecksum((uint8_t*)&eepr_header_default, sizeof(eepr_header_default)-4);
            break;

        case EEPR_CH_BOARD_INFO:
            eepr_board_info = eepr_board_info_default;
            eepr_board_info.chksum = EEPR_CalcChecksum((uint8_t*)&eepr_board_info_default, sizeof(eepr_board_info_default)-4);
            break;

        default:
            break;
    }
}


uint8_t EEPR_FormatCheck(void) {
    uint8_t retval=1;
    /* EEPR_ERRORTYPES_e errtype; */

    /* check if application software supports the same data format of data which is stored in eeprom */
    if(eepr_header.eepr_headerpattern == eepr_header_default.eepr_headerpattern) {

        if(eepr_header.versionnumbermajor == eepr_header_default.versionnumbermajor) {
            retval = 0;
        } else {
            retval = 1;  /* FIXME: handle data transfer (update) in case of new version with different data format */
        }
    } else {
        retval = 1;
    }
    return retval;
}


STD_RETURN_TYPE_e EEPR_CheckNodeID(uint8_t *dataptr) {
    STD_RETURN_TYPE_e ret_val;

    /* TODO: check chksum of eepr_board_info struct */

    if (*(uint16_t*)(dataptr) == eepr_board_info.network_nodeID)
        ret_val = E_OK;
    else
        ret_val = E_NOT_OK;

    return ret_val;
}
