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
 * @file    eepr.h
 * @author  foxBMS Team
 * @date    27.11.2012 (date of creation)
 * @ingroup DRIVERS
 * @prefix  EEPR
 *
 * @brief   Headers for the driver for the storage in the EEPROM memory
 *
 * Header file driver of external EEPROM device
 *
 */

#ifndef EEPR_H_
#define EEPR_H_


/*================== Includes =============================================*/
#include "eepr_cfg.h"

/*================== Macros and Definitions ===============================*/

/** Instruction Set for Serial EEPROM AT25128 (16k x 8 Bit): <br> - REN */
#define EEPR_CMD_WREN    0x06
/** Instruction Set for Serial EEPROM AT25128 (16k x 8 Bit): <br> - RDI */
#define EEPR_CMD_WRDI    0x04
/** Instruction Set for Serial EEPROM AT25128 (16k x 8 Bit): <br> - DSR */
#define EEPR_CMD_RDSR    0x05
/** Instruction Set for Serial EEPROM AT25128 (16k x 8 Bit): <br> - RSR */
#define EEPR_CMD_WRSR    0x01
/** Instruction Set for Serial EEPROM AT25128 (16k x 8 Bit): <br> - Read */
#define EEPR_CMD_READ    0x03
/** Instruction Set for Serial EEPROM AT25128 (16k x 8 Bit): <br> - Write */
#define EEPR_CMD_WRITE   0x02

/**
 * Control and Status Information of EEPROM Driver
 * must be a multiple of a pagelength, e.g. multiple of 64 (AT25128) or 256 (M95M02)
 */
#define EEPR_DATA_STARTADDR            0x00



/**
 * Bitfield of EEPROM Status Register RSR
 */
typedef volatile union {
    struct {
        unsigned int nRDY           : 1;    /*!< RDY=0 indicates device is ready                */
        unsigned int WEN            : 1;    /*!< WEN=0 indicates device is not write enabled    */
        unsigned int BP0            : 1;    /*!< BP0 Block Protection Bit 0                     */
        unsigned int BP1            : 1;    /*!< BP1 Block Protection Bit 1                     */
        unsigned int                : 3;    /*!< dummy                                          */
        unsigned int WPEN           : 1;    /*!< WPEN                                           */

    } B;
    uint8_t u8;
} EEPR_STATUSREGISTER_s;


/**
 * structure to safe the requested hardware protection
 */
typedef struct {
    EEPR_STATUSREGISTER_s SR;
} EEPR_REQ_HW_PROTECTION_s;

/**
 * Control and Status Information of EEPROM Driver
 */
typedef struct {
    EEPR_STATUSREGISTER_s RSR;  /*!< EEPROM status register                                             */
    uint8_t hwprotection;       /*!< the selected writeprotection                                       */
    uint32_t protectionstartadd;/*!< the startaddress of the protected area                             */
    uint32_t protectionendadd;  /*!< the endaddress of the protected area                               */
    EEPR_STATE_e state;         /*!< state of EEPROM Driver State Machine                               */
    EEPR_STATE_e statereq;      /*!< current state request                                              */
    EEPR_STATE_e statestart;    /*!< starting state of transition                                       */
    EEPR_STATE_e stateend;      /*!< target state of transition                                         */
    uint8_t substate;           /*!< sub-state of state                                                 */
    uint8_t triggerentry;       /*!< re-entrance protection (function running flag)                     */
    uint8_t repeat;             /*!<                                                                    */
    uint32_t timer;             /*!< in counts of 1ms                                                   */
    uint32_t eepromaddress1;    /*!< actually accessed EEPROM address for first page                    */
    uint32_t eepromaddress2;    /*!< EEPROM address for other page(absolute, 64 Byte page aligned)      */
    uint16_t currentpagelength;  /*!< length of the current page (maximum of 64 Bytes)                   */
    uint8_t* ramaddress;        /*!<  source or destination                                             */
    EEPR_CHANNEL_ID_TYPE_e currentchannel;  /*!<  Channel to be written in or read from                 */
    uint16_t nr_of_pages;       /*!< (sizeof(EEPR_STATUS_s)+PageLength-/PageLength)                     */
    uint16_t page_nr;           /*!< actually accessed EEPROM page                                      */
    uint16_t readtime;          /*!< time for read of all pages of struct_CALIB_FRAME                   */
    uint16_t writetime;         /*!< time for write of all pages of struct_CALIB_FRAME                  */
    uint16_t write_len;          /*!< write page in EEPROM  < PageLength                                 */
    uint16_t read_len;           /*!< depends only on SPI buffer                                         */
} EEPR_STATUS_s;

typedef struct {
    uint8_t repeat;
    uint8_t dirtyChannel;
    EEPR_DATAHANDLING_STATE_e state;
    uint8_t recovery_active;
}EEPR_DATA_HANDLING_s;


/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/
/**
 * @brief   returns 1 if the eeprom is in a final state in which it stays unless a staterequest is coming.
 *
 * @return  uint8_t 1 if true 0 if false
*/
extern uint8_t EEPR_IsInFinalState(void);

/**
 * @brief   cyclic trigger of EEPROM State Machine, trigger cycle: 1ms
 *          trigger timer in units of 1ms
*/
extern void EEPR_Trigger(void);


/**
 * @brief   Clears the error event and sends an errormessage
 *
 * @param   state_req   state request that caused the error
 *
*/
extern void EEPR_ClearErrorEvent(void);




/* FIXME function not called, is it important? throw out? */
/**
 * @brief   control of EEPROM Data management
*/
extern void EEPR_DataCtrl(void);

/**
 * @brief   reads "byte_len" of bytes from EEPROM memory starting at "address" and saving data in "eepr_spi_rxbuf"
 *
 * @param   EeprAddr  starting address of EEPROM where to read data
 * @param   byte_len  length of bytes that are to be read
 * @return  0 if reading successful, otherwise 0xFF
*/
extern uint8_t EEPR_ReadMemory(uint32_t EeprAddr, uint16_t byte_len);

/**
 * @brief   requests a state to be handled by the statetrigger
 *
 * @param   state_req   the requested state
 * @param   channel     channel to be written or read from
 * @param   ramaddress  sourceaddress for writing, targetaddress for reading
 * @return  0 if request was accepted, 1 if eeprom is busy, 2 if request was denied
*/
extern EEPR_RETURNTYPE_e EEPR_SetStateRequest(EEPR_STATE_e state_req, EEPR_CHANNEL_ID_TYPE_e channel, uint8_t* ramaddress);

/* FIXME brief section is confusing */
/**
 * @brief   max. time for reading of eeprom data of the current channel
 *
 * @return  readtime in ms
*/
extern uint16_t EEPR_GetCurrentChReadTime(void);

/* FIXME brief section is confusing */
/**
 * @brief   max. time for writing of eeprom data of the current channel
 *
 * @return  readtime in ms
*/
extern uint16_t EEPR_GetCurrentChWriteTime(void);

/**
 * @brief   calculates the maximum rest writetime for the current channel, by substracting the writetime taken to
 * read the current amount of pages
 * @return  writetime in ms
*/
extern uint16_t EEPR_GetRestWriteTime(void);

/**
 * @brief   calculates the maximum rest readtime for the current channel, by substracting the readtime taken to
 * read the current amount of pages
 *
 * @return  writetime in ms
*/
extern uint16_t EEPR_GetRestReadTime(void);

/**
 * @brief   calculates the maximum readtime for a specific channel
 *
 * @return  writetime in ms
*/
extern uint16_t EEPR_GetChWriteTime(EEPR_CHANNEL_ID_TYPE_e channel);

/**
 * @brief   calculates the maximum writetime for a specific channel
 *
 * @return  writetime in ms
*/
extern uint16_t EEPR_GetChReadTime(EEPR_CHANNEL_ID_TYPE_e channel);

/**
 * @brief   requests the initialization of the eeprom
 *          and triggers the state machine until eeprom is in IDLE-Mode or timer has elapsed
 *
 * @return  0 if successful, error-bit if not
*/
EEPR_ERRORTYPES_e EEPR_Init(void);

/**
 * @brief  Updates the EEPROM data if bkpsram data are valid, otherwise uses the valid EEPROM data to update Bkpsram
 *         If both data are invalid, writes default values into corrupt channels
 *
 * @return none
*/
extern EEPR_ERRORTYPES_e EEPR_InitChannelData(void);

/**
 * @brief  Reads the Data of one eeprom channel
 *         function is usable as soon as the timer tasks have been initialized
 *
 * @param  eepr_channel     the channel to be read from
 * @param  *dest_ptr        pointer to the destination of the read data
 * @return state of epprom, returns EEPROM_OK if request was accepted, EEPR_BUSY, EEPR_ERROR or EEPR_REQUEST_PENDING if not
*/
extern EEPR_RETURNTYPE_e EEPR_GetChannelData(EEPR_CHANNEL_ID_TYPE_e eepr_channel, uint8_t *dest_ptr);

/**
 * @brief  Writes data into one eeprom channel
 *         function is usable as soon as the timer tasks have been initialized
 *
 * @param  eepr_channel     the channel to be read from
 * @param  *src_ptr         ptr to the src of the data that should be written into the eeprom. Only necessary if no bkpsram channel is defined in eeprom channel config
 * @return state of epprom, returns EEPROM_OK if request was accepted, EEPR_BUSY, EEPR_ERROR or EEPR_REQUEST_PENDING if not
*/
extern EEPR_RETURNTYPE_e EEPR_SetChannelData(EEPR_CHANNEL_ID_TYPE_e eepr_channel, uint8_t *src_ptr);

/**
 * @brief handles Data, checks validity, provides automatic backup in case of data corruption, dependent on dirtyflags
 *
 * The DataHandler provides an automatic read- and writeprocess if a dirtyflag is set in eepr_ch_dirtyflag[].
 * If the dirtyflag is set, it checks the validity of the data. If they are invalid it recovers valid data from the eeprom.
 * If the data from the eeprom are also invalid, the default data are used. If the data are valid, they will be written into the eeprom
*/
extern void EEPR_DataHandler();

/*================== Function Implementations =============================*/

#endif /* EEPR_H_ */
