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
 * @file    eepr_cfg.h
 * @author  foxBMS Team
 * @date    23.02.2016 (date of creation)
 * @ingroup DRIVERS_CONF,EXT_PER
 * @prefix  EEPR
 *
 * @brief   Header for the configuration for the driver for the storage in the EEPROM memory
 *
 */

#ifndef EEPR_CFG_H_
#define EEPR_CFG_H_


/*================== Includes =============================================*/
#include "general.h"

/*================== Macros and Definitions ===============================*/

/**
 * @ingroup CONFIG_EEPR
 * selects the used Eeprom Hardware (IC), which are supported by this driver
*/
/* #define EEPROM_VERSION_AT25128 */
#define EEPROM_VERSION_M95M02


#define EEPR_HW_PROTECTION_NONE       0x00  /*!< no protection          */
#define EEPR_HW_PROTECTION_1_4        0x04  /*!< 1/4 protected          */
#define EEPR_HW_PROTECTION_2_4        0x08  /*!< 1/2 protected          */
#define EEPR_HW_PROTECTION_ALL        0x0C  /*!< everything protected   */

/**
 * @ingroup CONFIG_EEPR
 * select the write/read Protection
*/
#define EEPR_HW_PROTECTION       EEPR_HW_PROTECTION_NONE
/* #define EEPR_HW_PROTECTION       EEPR_HW_PROTECTION_1_4 */
/* #define EEPR_HW_PROTECTION       EEPR_HW_PROTECTION_2_4 */
/* #define EEPR_HW_PROTECTION       EEPR_HW_PROTECTION_ALL */

/* controls if one eeprom version was selected */
#ifndef EEPROM_VERSION_AT25128
    #ifndef EEPROM_VERSION_M95M02
        #error no Eeprom Hardware (IC) defined
    #endif
#endif


/*
 * maximum numbers of channels
 */
#define EEPR_CHANNEL_MAX_NR                    10            /* maximum configured channels */
/*
 * maximum length of channels
 */
#define EEPR_CH_MAXLENGTH                      256           /* maximum  channel data volume in bytes */


#ifdef EEPROM_VERSION_AT25128
    #define EEPR_PageLength                    64
    #define EEPR_CMDBUF_OFFSET                 3             /* bytes needed to send address and commandbyte */
#else
    #ifdef EEPROM_VERSION_M95M02
        #define EEPR_PageLength                256
        #define EEPR_CMDBUF_OFFSET             4             /* bytes needed to send address and commandbyte */
     #else
        #error no Eeprom Hardware (IC) defined
    #endif
#endif

#define EEPR_TXBUF_LENGTH           (EEPR_CH_MAXLENGTH + EEPR_CMDBUF_OFFSET)  /* maximum data + command byte length */

/*
 * maximum time in ms which is needed to write or read one page
 */
#define EEPR_WRITETIME_PER_PAGE                70
#define EEPR_READTIME_PER_PAGE                 20

/**
 * symbolic names for EEPROM Channels
 */
typedef enum {
    EEPR_CHANNEL_1        = 0,
    EEPR_CHANNEL_2        = 1,
    EEPR_CHANNEL_3        = 2,
    EEPR_CHANNEL_4        = 3,
    EEPR_CHANNEL_5        = 4,
    EEPR_CHANNEL_6        = 5,
    EEPR_CHANNEL_7        = 6,
    EEPR_CHANNEL_8        = 7,
    EEPR_CHANNEL_9        = 8,

    EEPR_CHANNEL_MAX      = EEPR_CHANNEL_MAX_NR-1,
} EEPR_CHANNEL_ID_TYPE_e;

/* Channeldefinition */
#define EEPR_CH_HEADER            EEPR_CHANNEL_1
#define EEPR_CH_BOARD_INFO        EEPR_CHANNEL_2
#define EEPR_CH_HARDWARE_INFO     EEPR_CHANNEL_3
#define EEPR_CH_OPERATING_HOURS   EEPR_CHANNEL_4
#define EEPR_CH_NVSOC             EEPR_CHANNEL_5
#define EEPR_CH_CONTACTOR         EEPR_CHANNEL_6
#define EEPR_CH_STATISTICS        EEPR_CHANNEL_7
#define EEPR_CH_USER_DATA         EEPR_CHANNEL_8


/**
 * symbolic names for the EEPROM states
 */
typedef enum {
    EEPR_INITIALIZED                = 0,    /*!< Initializing-Sequence: EEPROM-driver initialized   */
    EEPR_INIT_INPROCESS             = 1,    /*!< Initializing-Sequence: EEPROM-driver initializing  */
    EEPR_IDLE                       = 2,    /*!< Idle-State                                         */
    EEPR_READMEMORY                 = 3,    /*!< Reading-Sequence: Initiate reading                 */
    EEPR_READMEMORY_INPROCESS       = 4,    /*!< Reading-Sequence: Currently reading                */
    EEPR_CHECK_DATA                 = 5,    /*!< Reading-Sequence: Data check                       */
    EEPR_WRITEMEMORY                = 6,    /*!< Writing-Sequence: Initiate writing                 */
    EEPR_WRITE_ENABLE_INPROCESS     = 7,    /*!< Writing-Sequence: Enable writing                   */
    EEPR_WRITEMEMORY_ENABLED        = 8,    /*!< Writing-Sequence: Writing enabled                  */
    EEPR_WRITEMEMORY_INPROCESS      = 9,    /*!< Writing-Sequence: Currently writing                */
    EEPR_INIT_PROTECTION_CFG        = 10,   /*!< Writeprotection: Initiate protecting               */
    EEPR_ENABLE_PROTECTION_CFG      = 11,   /*!< Writeprotection : Enable write statusregister      */
    EEPR_PROTECTION_CFG_ENABLED     = 12,   /*!< Writeprotection : writing status register enabled  */
    EEPR_PROTECTION_CFG_INPROCESS   = 13,   /*!< Writeprotection : set statusregister bits          */
    EEPR_NO_REQUEST                 = 0xF0, /*!< EEPROM-driver not initialized                      */
    EEPR_PROTECTION_FAILED          = 0xFA, /*!< Error-State: Protection failed                     */
    EEPR_WRITEFAILED                = 0xFB, /*!< Error-State: Write failed                          */
    EEPR_READFAILED                 = 0xFC, /*!< Error-State: Read failed                           */
    EEPR_INITFAILED                 = 0xFD, /*!< Error-State: Initialization failed                 */
    EEPR_UNINITIALIZED              = 0xFE, /*!< EEPROM-driver not initialized                      */
    EEPR_DISABLED                   = 0xFF, /*!< EEPROM-driver not active                           */
    EEPR_STATEREQFAILED             = 0x12, /*!< EEPROM-Requestfail                                 */
    EEPR_WRITEPROTECTED             = 0x11, /*!< EEPROM-Requestfail                                 */
} EEPR_STATE_e;

typedef enum {
    EEPR_REQ_TRANSITIONOK           = 0,    /*!< Transition allowed, request accepted                          */
    EEPR_REQ_WRITEPROTECTED         = 1,    /*!< Transition forbidden, channel is writeprotected               */
    EEPR_REQ_TRANSITIONFAIL         = 2,    /*!< Transition from current state to requested state not allowed  */
    EEPR_REQ_BUSY                   = 3,    /*!< Transition not possible, EEPROM already haandling a request   */
}EEPR_REQUEST_STATE_e;

/**
 * symbolic names for the EEPROM errors
 */
typedef enum {
    EEPR_NO_ERROR   = 0x00,
    EEPR_INIT_ERROR = 0x01,
    EEPR_ERR_WR     = 0x02,   /*!< Timeout Error while writing*/
    EEPR_ERR_RD     = 0x04,   /*!< Timeout Error while reading */
} EEPR_ERRORTYPES_e;



/** symbolic names for the EEPROM return types */
typedef enum {
    EEPR_OK      = 0,  /*!< error not occured or occured but threshold not reached    */
    EEPR_BUSY    = 1,  /*!< EEPROM driver busy */
    EEPR_ERROR   = 2,  /*!< error occurred */
    EEPR_REQUEST_PENDING = 3, /*!< request pending, no other request can be accepted */
    EEPR_WR_PROTECTED = 4,  /*!< request declined, channel is write protected */
} EEPR_RETURNTYPE_e;

/** symbolic names for SW Write Protection of eeprom channels */
typedef enum {
    EEPR_SW_WRITE_UNPROTECTED  = 0,  /*!< SW Write Protection off    */
    EEPR_SW_WRITE_PROTECTED    = 1,  /*!< SW Write Protection on     */
} EEPR_SW_WRITEPROTECTION_e;

typedef enum {
    EEPR_ACCESS_UNPROTECTED  = 0,  /*!< SW Write Protection off    */
    EEPR_ACCESS_PROTECTED    = 1,  /*!< SW Write Protection on     */
} EEPR_ACCESS_ROTECTION_e;

typedef struct {
    uint16_t eepromaddress;                    /*!<startaddress of the channel in EEPROM                                   */
    uint16_t length;                           /*!<length of the channel                                                   */
    EEPR_CHANNEL_ID_TYPE_e channelid;          /*!<the position of the channel in channelconfig                            */
    uint16_t chksumaddress;                    /*!<address of the checksum of the channel data                             */
    EEPR_SW_WRITEPROTECTION_e writeprotection; /*!<SW Write Protection                                                     */
    uint8_t *bkpsramptr;                       /*!<pointer to the location of the corresponding channel in the bkpsram     */
    EEPR_ERRORTYPES_e errorflag;               /*!<indicates an error during last write/read process                       */
    EEPR_ACCESS_ROTECTION_e accessprotection;  /*!<prevents parallel access of one channel by two processes                */
} EEPR_CH_CFG_s;


/* @FIXME comments missing */
typedef struct {
    uint16_t     u_cell_max;
    uint16_t     u_cell_min;
    uint16_t     u_temperature_max;
    uint16_t     u_temperature_min;
    float        current_max;
    float        current_min;
    uint32_t     dummy[3];        /* for future use */
    uint32_t checksum;            /* checksum of data block (must be last position) */
}EEPR_CALIB_STATISTICS_s;

/* @FIXME comments missing */
typedef struct {
    uint16_t  versionnumbermajor;     /*!<Versionnumber of major changes in EEPROM software    */
    uint16_t  versionnumberminor;     /*!<Versionnumber of minor changes in EEPROM software    */
    uint32_t eepr_headerpattern;      /*!<Versionnumber of minor changes in EEPROM software    */
    uint16_t dummy2;                  /*!<dummy for data alignment                             */
    uint16_t dummy3;                  /*!<dummy for data alignment                             */
    uint16_t reserved[2];             /*!<reserved for futore use                              */
    uint32_t reserved2[3];            /*!<reserved for futore use                              */
    uint32_t chksum;                  /*!<checksum of eeprom data                              */
}EEPR_HEADER_s;

/* @FIXME comments missing */
typedef struct {
    uint16_t network_nodeID;          /*!<Network Device ID              */
    uint16_t reserved_u16;            /*!<reserved for futore use                              */
    uint32_t reserved[8-2];           /*!<reserved for futore use                              */
    uint32_t chksum;                  /*!<checksum of eeprom data                              */
}EEPR_BOARD_INFO_s;

typedef enum{
    EEPR_STARTDIRTYCHECK = 0,
    EEPR_DATABACKUP = 1,
    EEPR_CHECKBACKUP = 2,
    EEPR_WRITEEEPROM = 3,
    EEPR_CHECKEEPROM = 4,
    EEPR_RECOVERYCHECK = 5,
    EEPR_READREQUEST = 6,
    EEPR_READCHECK = 7,
}EEPR_DATAHANDLING_STATE_e;

typedef enum {
    EEPR_DAT_IDLE   = 0x00,         /*!< no errors happened, eeprom is ready for usage  */
    EEPR_DAT_SUCCESS = 0x01,        /*!< writing data into eeprom was successful        */
    EEPR_DAT_RAM_ERROR = 0x02,      /*!< data in RAM were corrupt                       */
    EEPR_DAT_EEPROM_ERROR = 0x04,   /*!< data in EEPROM were corrupt                    */
    EEPR_DAT_FATAL_ERROR = 0x08,    /*!< writing data into eeprom was successful        */
} EEPR_DATAHANDLING_ERROR_e;

#define EEPR_DEFAULT_NETWORK_ID                 0xFFFF  /* Default NodeID of Primary Controller*/

/** Communication Interfaces: Data Receive */
#define EEPR_ReceiveData(bufferptr,length)   SPI_ReceiveData(bufferptr,length)
/** Communication Interfaces: Data Transmission */
#define EEPR_SendData(bufferptr,length,receiveoffset)   SPI_SendData(bufferptr,length,receiveoffset)

/*================== Constant and Variable Definitions ====================*/


extern uint8_t eepr_ch_dirtyflag[EEPR_CHANNEL_MAX_NR];
extern uint8_t eepr_ch_read_req_flag[EEPR_CHANNEL_MAX_NR];
extern uint8_t eepr_bkpsram_buffer[EEPR_CH_MAXLENGTH];

/**
 * configuration of eeprom channels
 *
 * Channels are memory slots mapped to a certain eeprom address area which can be assigned with attributes like
 * software protection. The channels can be linked to buffering SRAM areas (e.g. Backup-SRAM when available) for recovery and caching purposes.
 * The last quarter of memory space is hardware protected, which can be only unprotected by specific command sequences.
 *
 */
extern EEPR_CH_CFG_s eepr_ch_cfg[];


/**
 * versionnumber of EEPR Software
 */
extern const EEPR_HEADER_s eepr_header_default;

/**
 * foxBMS board and device information
 */
extern EEPR_BOARD_INFO_s eepr_board_info;

/**
 * default calibration statistics
 */

extern const EEPR_CALIB_STATISTICS_s defaultcalibstatistics;

/**
 * buffer to write or read data in eeprom
 */
extern uint8_t eepr_WR_RD_buffer[EEPR_CH_MAXLENGTH];

/**
 * checksum of the write/read buffer
 */
extern uint16_t eepr_WR_RD_Chksum;

/**
 * array length  for channels of the EEPROM definition
 */
extern const uint8_t eepr_nr_of_channels;

/*================== Function Prototypes ==================================*/

/**
 * @brief   sets a dirty flag to indicate that the data of a channel have been changed
 *
 * @param   eepr_channel   the channel of which the data have been changed
 * @return
*/
void EEPR_SetChDirtyFlag(EEPR_CHANNEL_ID_TYPE_e eepr_channel);

/**
 * @brief   removes the dirty flag to indicate that the eeprom was actualized
 *
 * @param   eepr_channel   the channel that has been updated
 * @return
*/
void EEPR_RemoveChDirtyFlag(EEPR_CHANNEL_ID_TYPE_e eepr_channel);

void EEPR_SetChReadReqFlag(EEPR_CHANNEL_ID_TYPE_e eepr_channel);
void EEPR_RemoveChReadReqFlag(EEPR_CHANNEL_ID_TYPE_e eepr_channel);

/**
 * @brief   Receives data from the EEPROM via SPI
 *
 * @param   *buffer pointer where the data should be stored to
 * @param   length  length of the buffer
 *
 * @return  EEPR_OK if OK, otherwise EEPR_ERROR
 */
extern EEPR_RETURNTYPE_e SPI_ReceiveData(uint8_t* buffer, uint16_t length);

/**
 * @brief   Stores the data in the EEPROM via SPI
 *
 * @param  *data            pointer where the data is stored
 * @param   length          length of data
 * @param   receiveoffset   receive offset
 *
 * @return  EEPR_OK if okay, otherwise EEPR_ERROR
 */
extern EEPR_RETURNTYPE_e SPI_SendData(uint8_t* data, uint16_t length, uint16_t receiveoffset);

/**
 * @brief   returns actual state of EEPROM state machine
 *
 * @return  state of EEPROM state machine
*/
extern EEPR_STATE_e EEPR_GetState(void);


/* FIXME bad confusing use of EEPR in EEPR module and BKPSRAM module. maybe move this checksum to utils or similar and use define here? */

/**
 * @fn      U16 EEPR_CalcChecksum(U8 *dataptr, U16 byte_len)
 * @brief   calculates the 16-bit sum-of-bytes checksum over byte_len of bytes by starting at *dataptr
 *
 * @param   dataptr   start address for calculation
 * @param   byte_len  number of bytes for calculation
 * @return  16 Bit Checksum
*/
extern uint16_t EEPR_CalcChecksum(uint8_t *dataptr, uint16_t byte_len);

/**
 * @brief  checks the validation of the bkpsram_nvsoc checksum by comparing Backup-SRAM with eeprom data
 *
 * @param  eepr_channel channel to be checked
 * @param  *ptr pointer to the startaddress of the data (in RAM), only necessary if no pointer is configured
 * @return 0 if checksum is OK, otherwise 0xFF
*/
extern STD_RETURN_TYPE_e  EEPR_BkpSramCheckChksum(EEPR_CHANNEL_ID_TYPE_e eepr_channel, uint8_t *ptr);

extern uint8_t EEPR_FormatCheck(void);

extern void EEPR_SetDefaultValue(EEPR_CHANNEL_ID_TYPE_e eepr_channel);

STD_RETURN_TYPE_e EEPR_CheckNodeID(uint8_t *dataptr);

/**
 * @brief  preparation for generic function, which can send channel data via can. Can be used like this for up to 8 byte of data.
*/
extern void EEPR_SendCanData(void);

/*================== Function Implementations =============================*/


#endif /* EEPR_CFG_H_ */
