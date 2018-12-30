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
 * @file    eepr.c
 * @author  foxBMS Team
 * @date    07.10.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  EEPR
 *
 * @brief   Driver for the storage in the EEPROM memory.
 *
 * driver for external EEPROM device AT25128 (16kByte)
 * /HOLD and /WP are permanently kept at Hi-Level, thus HW-Write-Protection (WP) is disabled.
 * SW-Protection(Block Write Protection) could be done by Sending Command EEPR_CMD_WREN if disabled before by EEPR_CMD_WRDI.
 * At startup the device is unprotected. Actual state will be provided by EEPR_CMD_RDSR (Read Status Register).
 * A Block Write Protection can be controlled by Status Register Bit BP1 and BP0 in different address ranges within
 * the whole range: Byte Address 0x0000..0x3FFF
 * \n
 *  | Protected Address  |    BP1  |  BP0    |
 *  | :----              |:----    | :----   |
 *  | None               |    0    |    0    |
 *  | 0x3000..0x3FFFF    |    0    |    1    |
 *  | 0x2000..0x3FFFF    |    1    |    0    |
 *  | 0x0000..0x3FFFF    |    1    |    1    |
 */

/*================== Includes =============================================*/
#include "eepr.h"

#include "diag.h"
#include "io.h"
#include "os.h"
#include "rtc.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/

uint8_t eepr_cmd_read[EEPR_TXBUF_LENGTH] = { EEPR_CMD_READ, 0, 0 };
uint8_t eepr_cmd_write[EEPR_TXBUF_LENGTH] = { EEPR_CMD_WRITE, 0, 0 };
uint8_t eepr_cmd_WREN[1] = { EEPR_CMD_WREN };
uint8_t eepr_cmdbuf[2];  /* buffer for sending commands via SPI */
uint8_t lastDirtyChannel;
uint8_t lastReadChannel;
uint16_t byte_len;
uint8_t eepr_backupbuffer[EEPR_CH_MAXLENGTH];
EEPR_DATAHANDLING_ERROR_e EEPR_ch_dataerrors[EEPR_CHANNEL_MAX_NR];
uint8_t cnt;    /* counter variable */


uint16_t timeout_cnt = 0;
uint8_t tmpbuf[EEPR_TXBUF_LENGTH];
uint8_t protectionbuf[2];

EEPR_REQ_HW_PROTECTION_s protection_request = {
        .SR.u8 = 0xFF,
};


/* EEPR_STATUSREGISTER_s epr_state; */
EEPR_STATUS_s eepr_state = {
    .RSR.u8         = 0xFF,
    .hwprotection   = 0,
    .protectionstartadd = 1,
    .protectionendadd = 0,
    .state          = EEPR_DISABLED,
    .stateend       = EEPR_DISABLED,
    .statestart     = EEPR_DISABLED,
    .statereq       = EEPR_NO_REQUEST,
    .substate       = 0,
    .triggerentry   = 0,
    .repeat         = 2,
    .timer          = 0,
    .eepromaddress1  = 0,
    .eepromaddress2 = 0,
    .currentpagelength     = 0,
    .nr_of_pages    = 1,   /* will be re-set at initialization to sizeof(struct_EEPR_CALIB_FRAME)+(EEPR_PageLength-1)/EEPR_PageLength */
    .readtime       = EEPR_READTIME_PER_PAGE,   /* max. readtime of one page */
    .writetime      = EEPR_WRITETIME_PER_PAGE,  /* max. writetime of one page */
    .page_nr        = 0,
    .write_len      = EEPR_PageLength,
    .read_len       = EEPR_PageLength,
    .ramaddress = 0,
    .currentchannel = EEPR_CH_HEADER,
};

EEPR_DATA_HANDLING_s data_handler = {
        .dirtyChannel = 0xFF,
        .repeat = 2,
        .state = 0,
        .recovery_active = 0,
};

/*================== Function Prototypes ==================================*/
static void EEPR_StateFailed(EEPR_STATE_e state);
static void EEPR_ReEnterStateInit(void);
static EEPR_STATE_e EEPR_CheckStateRequest(EEPR_STATE_e state_req, EEPR_CHANNEL_ID_TYPE_e channel);
static void EEPR_TransferStateRequest(void);
void EEPR_ClearErrorEvent(void);
static void EEPR_delete_bkpsrambuffer(EEPR_CHANNEL_ID_TYPE_e eepr_channel);
static EEPR_ERRORTYPES_e EEPR_BackupChannelData2Eepr(EEPR_CHANNEL_ID_TYPE_e eepr_channel);
static EEPR_ERRORTYPES_e EEPR_ReadChannelData(EEPR_CHANNEL_ID_TYPE_e eepr_channel);
static EEPR_ERRORTYPES_e EEPR_RefreshChannelData(EEPR_CHANNEL_ID_TYPE_e eepr_channel);
STD_RETURN_TYPE_e EEPR_CheckDirtyFlags();
uint8_t EEPR_GetNextDirtyChannel();


/*================== Function Implementations =============================*/

/* FIXME if config changes (for example eepr_WR_RD_buffer) the main driver files have to be changed as well. Maintainability? */
/**
 * @brief   initialization of external Serial EEPROM device (AT25128N)
 *
 * @return  DIAG_EEPR_OK if initialization OK, otherwise DIAG_EEPR_ERROR
*/
EEPR_RETURNTYPE_e EEPR_StartInitialization(void) {

    EEPR_RETURNTYPE_e retVal = EEPR_ERROR;
    eepr_cmdbuf[0]=(EEPR_CMD_RDSR);    /* first byte is command, second one is dummy (for data receive) */
    if(EEPR_SendData(eepr_cmdbuf, 2, 1)  ==  EEPR_OK) {   /* send data, data length, receive data offset(leading dummy bytes) */
        retVal = EEPR_OK;     /* EEPROM Initialization: command sending process has been started */
    }
    else {
        retVal = EEPR_ERROR;  /* EEPROM Initialization failed due to communication error */
    }

    return retVal;
}

uint16_t EEPR_GetChReadTime(EEPR_CHANNEL_ID_TYPE_e channel) {
    uint16_t number_of_pages;
    if(eepr_ch_cfg[channel].eepromaddress % EEPR_PageLength == 0)
        number_of_pages = (eepr_ch_cfg[channel].length+(EEPR_PageLength-1))/EEPR_PageLength;
    else
        number_of_pages = (((eepr_ch_cfg[channel].length - (eepr_ch_cfg[channel].eepromaddress % EEPR_PageLength))+(EEPR_PageLength-1))/EEPR_PageLength)+1;
    return number_of_pages * EEPR_READTIME_PER_PAGE;
}

uint16_t EEPR_GetChWriteTime(EEPR_CHANNEL_ID_TYPE_e channel) {
    uint16_t number_of_pages;
    if(eepr_ch_cfg[channel].eepromaddress % EEPR_PageLength == 0)
        number_of_pages = (eepr_ch_cfg[channel].length+(EEPR_PageLength-1))/EEPR_PageLength;
    else
        number_of_pages = (((eepr_ch_cfg[channel].length - (eepr_ch_cfg[channel].eepromaddress % EEPR_PageLength))+(EEPR_PageLength-1))/EEPR_PageLength)+1;
    return number_of_pages * EEPR_WRITETIME_PER_PAGE;
}

uint16_t EEPR_GetCurrentChReadTime(void){
    return eepr_state.nr_of_pages * EEPR_READTIME_PER_PAGE;
}

uint16_t EEPR_GetCurrentChWriteTime(void){
    return eepr_state.nr_of_pages * EEPR_WRITETIME_PER_PAGE;
}

uint16_t EEPR_GetRestReadTime(void){
    return (eepr_state.nr_of_pages - eepr_state.page_nr) * EEPR_READTIME_PER_PAGE;
}

uint16_t EEPR_GetRestWriteTime(void){
    return (eepr_state.nr_of_pages - eepr_state.page_nr) * EEPR_WRITETIME_PER_PAGE;
}

uint8_t EEPR_ReadMemory(uint32_t EeprAddr, uint16_t byte_len) {

#ifdef EEPROM_VERSION_AT25128
    eepr_cmd_read[1]=(uint8_t)(EeprAddr>>8);    /* Highbyte of Address */
    eepr_cmd_read[2]=(uint8_t)(EeprAddr);       /* Lowbyte of Address */
#endif
#ifdef EEPROM_VERSION_M95M02
    eepr_cmd_read[1]=(uint8_t)(EeprAddr>>16);   /* Highbyte of Address */
    eepr_cmd_read[2]=(uint8_t)(EeprAddr>>8);    /* Lowbyte of Address */
    eepr_cmd_read[3]=(uint8_t)(EeprAddr);       /* Lowbyte of Address */
#endif
    if(byte_len > EEPR_CH_MAXLENGTH)
        return 0xFE;

    if(EEPR_SendData(&eepr_cmd_read[0], byte_len+EEPR_CMDBUF_OFFSET, EEPR_CMDBUF_OFFSET)  ==  0) {
    /* Send Read Command has been started */
        return 0;
    }
    else
        return 0xFF;
}

/**
 * @fn     U8 EEPR_WriteMemory(U32 EeprAddr,U8 *srcptr ,U16 byte_len)
 * @brief  writes "byte_len" of bytes from EEPROM memory starting at "address" and reading data from "srcptr"
 *
 * @param  EeprAddr  starting address of EEPROM where to write data
 * @param  srcptr    pointer where the data is stored
 * @param  byte_len  length of bytes that are to be written
 * @return 0 if writing successful, otherwise 0xFF
*/
uint8_t EEPR_WriteMemory(uint32_t EeprAddr, uint8_t *srcptr, uint16_t byte_len) {

    uint8_t cnt = 0;

    if(srcptr == NULL)
        return 0xFF;

    if(!byte_len)
        return 0xFF;
    else if(byte_len > EEPR_CH_MAXLENGTH)
        return 0xFE;

#ifdef EEPROM_VERSION_AT25128
    tmpbuf[0]=EEPR_CMD_WRITE;
    tmpbuf[1]=(uint8_t)(EeprAddr>>8);    /* Highbyte of Address */
    tmpbuf[2]=(uint8_t)(EeprAddr);       /* Lowbyte of Address */
#endif
#ifdef EEPROM_VERSION_M95M02
    tmpbuf[0]=EEPR_CMD_WRITE;
    tmpbuf[1]=(uint8_t)(EeprAddr>>16);   /* Highbyte of Address */
    tmpbuf[2]=(uint8_t)(EeprAddr>>8);    /* Lowbyte of Address */
    tmpbuf[3]=(uint8_t)(EeprAddr);       /* Lowbyte of Address */
#endif
    for(cnt=0;cnt<byte_len;cnt++) {
        tmpbuf[EEPR_CMDBUF_OFFSET+cnt]=*srcptr++;
    }

    if(EEPR_SendData(&tmpbuf[0], byte_len+EEPR_CMDBUF_OFFSET, EEPR_CMDBUF_OFFSET)  ==  EEPR_OK) {
        /* Send Write Command has been started */
        return 0;
    }
    else
        return 0xFF;
}


/**
 * @brief  writes the protection bytes BP0 and BP1 into EEPROM
 *
 * @return 0 if writing successful, otherwise 1
*/
uint8_t EEPR_SetHwProtection(void) {
    protectionbuf[0] = EEPR_CMD_WRSR;
    protectionbuf[1] = (uint8_t)EEPR_HW_PROTECTION;
    if (EEPR_SendData(&protectionbuf[0], 2, 0)  ==  EEPR_OK) {
        return 0;
    } else {
        return 1;
    }
}

/**
 * @brief  sets the startaddress and the endaddress of the hardware-protected area
*/
void setProtectedAddresses(void) {
    if(eepr_state.hwprotection == 0x00) {
        /* no protection on, set values 1 and 0, so ne values are between start and end */
        eepr_state.protectionstartadd = 0x4000;  /* @FIXME: check and write some comments */
        eepr_state.protectionendadd = 0x3FFF;
    } else if(eepr_state.hwprotection == 0x04) {
        eepr_state.protectionstartadd = 0x3000;
        eepr_state.protectionendadd = 0x3FFF;
    } else if(eepr_state.hwprotection == 0x08) {
        eepr_state.protectionstartadd = 0x2000;
        eepr_state.protectionendadd = 0x3FFF;
    } else if(eepr_state.hwprotection == 0x0C) {
        eepr_state.protectionstartadd = 0x0000;
        eepr_state.protectionendadd = 0x3FFF;
    }
}

/**
 * @fn     EEPR_STATE_e EEPR_GetState(void)
 * @brief
 * @return returns the current state
*/
EEPR_STATE_e EEPR_GetState(void) {
    return (eepr_state.state);
}

uint8_t EEPR_IsInFinalState(void) {
    EEPR_STATE_e tempstate;
    tempstate = EEPR_GetState();
    if(tempstate == EEPR_IDLE ||
       (tempstate == EEPR_PROTECTION_FAILED && eepr_state.substate == 1) ||
       (tempstate == EEPR_WRITEFAILED && eepr_state.substate == 1)       ||
       (tempstate == EEPR_READFAILED && eepr_state.substate == 1)        ||
       (tempstate == EEPR_INITFAILED && eepr_state.substate == 1)        ||
       tempstate == EEPR_DISABLED                                        ||
       (tempstate == EEPR_STATEREQFAILED && eepr_state.substate == 1)    ||
       tempstate == EEPR_WRITEPROTECTED)
    {
        /* eeprom is in a final state, where it doesn´t need SPI or any other resources, so return true. */
        return 1;
    } else {
        return 0;
    }
}

/**
 * @fn     EEPR_STATE_e EEPR_CheckStateRequest(EEPR_STATE_e state_req, EEPR_CHANNEL_ID_TYPE_e channel)
 * @brief  checks if the transition from one state to another one is allowed
 *
 * @param  state_req    requested state
 * @param  channel      eepr_channel to be written or read from
 * @return the requested state if accepted or EEPR_STATEREQFAILED if denied
*/
EEPR_STATE_e EEPR_CheckStateRequest(EEPR_STATE_e state_req, EEPR_CHANNEL_ID_TYPE_e channel) {
    /* Checks if transition from current state to state_req is allowed */
    EEPR_STATE_e retVal = EEPR_STATEREQFAILED;

    /* when in EEPR_IDLE, EEPR_READFAILED or in EEPR_WRITEFAILED */
    /* only accept requests for: EEPR_WRITEMEMORY , EEPR_READMEMORY or EEPR_IDLE */

    if (eepr_state.state  ==  EEPR_IDLE || eepr_state.state  ==  EEPR_READFAILED
                    || eepr_state.state  ==  EEPR_WRITEFAILED) {
        if(state_req  ==  EEPR_READMEMORY || state_req == EEPR_IDLE) {
            retVal = state_req;
        } else if(state_req  ==  EEPR_WRITEMEMORY) {
            if(eepr_ch_cfg[channel].writeprotection == 0) /* check if software writeprotection is on */
                /* check if channel is located in hardware write protected area */
                if(((eepr_ch_cfg[channel].eepromaddress >= eepr_state.protectionstartadd) && (eepr_ch_cfg[channel].eepromaddress <= eepr_state.protectionendadd))
                        || (((eepr_ch_cfg[channel].eepromaddress + eepr_ch_cfg[channel].length) >= eepr_state.protectionstartadd)
                                && ((eepr_ch_cfg[channel].eepromaddress + eepr_ch_cfg[channel].length) <= eepr_state.protectionendadd)))
                {
                    /* channel is write protected, transition is not allowed */
                    retVal = EEPR_WRITEPROTECTED;
                } else {
                    retVal = state_req;
                }
            else
                retVal = EEPR_WRITEPROTECTED;
        }
        else
            retVal = EEPR_STATEREQFAILED;
    }

    /* when in EEPR_INITFAILED, UNINITIALIZED or DISABLED */
    /* only accept requests for: EEPR_IDLE or EEPR_UNINITIALIZED */

    else if (eepr_state.state == EEPR_UNINITIALIZED || eepr_state.state == EEPR_INITFAILED || eepr_state.state == EEPR_DISABLED) {

        if (state_req  ==  EEPR_IDLE || state_req  ==  EEPR_UNINITIALIZED){
            retVal = EEPR_UNINITIALIZED;
        }
        else
            retVal = EEPR_STATEREQFAILED;
    }
    else
        retVal = EEPR_STATEREQFAILED;


    return retVal;
}


EEPR_RETURNTYPE_e EEPR_SetStateRequest(EEPR_STATE_e state_req, EEPR_CHANNEL_ID_TYPE_e channel, uint8_t* ramaddress) {
    EEPR_RETURNTYPE_e retVal = EEPR_OK;
    EEPR_STATE_e tempstate;

    OS_TaskEnter_Critical();

    if(eepr_state.triggerentry) {
        retVal = EEPR_BUSY;   /* return as "BUSY" because EEPR_Trigger() is actually running */
    }
    else {
        if(eepr_state.statereq != EEPR_NO_REQUEST) {
            retVal = EEPR_REQUEST_PENDING; /* return as "REQUEST PENDING" because another request is waiting in Queue */
        }
    }



    if(retVal == EEPR_OK) {
        tempstate = EEPR_CheckStateRequest(state_req, channel);

        if(tempstate == EEPR_STATEREQFAILED || tempstate == EEPR_WRITEPROTECTED) { /* Requested state declined */
            if(tempstate == EEPR_WRITEPROTECTED)
                retVal = EEPR_WR_PROTECTED;
            else
                retVal = EEPR_ERROR;
        }
        else
        {
            /* transition is allowed */
            eepr_state.statereq = tempstate;
            eepr_state.ramaddress = ramaddress;
            eepr_state.currentchannel = channel;
            if(state_req == EEPR_UNINITIALIZED) {
                eepr_state.stateend = EEPR_IDLE; /* statemachine will automatically go to Idle after Initialization */
            } else {
                eepr_state.stateend = state_req;
            }
        }
    }
    OS_TaskExit_Critical();

    return (retVal);
}

/**
 * @fn     void EEPR_TransferStateRequest(void)
 * @brief  updates the current state after checking once again if the transition is allowed and sets all the variables needed
*/
void EEPR_TransferStateRequest(void) {  /* uses the request to update the current state, if allowed */
    uint16_t i;
    uint16_t datalength;

    /* checks if transition is still allowed */
    if((EEPR_CheckStateRequest(eepr_state.statereq, eepr_state.currentchannel) != EEPR_STATEREQFAILED) && (EEPR_CheckStateRequest(eepr_state.statereq, eepr_state.currentchannel) != EEPR_WRITEPROTECTED))
    { /* start transition */
        EEPR_ClearErrorEvent(); /*  clears error events */
        eepr_state.statestart = eepr_state.state;   /* save starting point */
        eepr_state.state = eepr_state.statereq;
        eepr_state.statereq = EEPR_NO_REQUEST;

        if(eepr_state.stateend == EEPR_WRITEMEMORY) {

            OS_TaskEnter_Critical();
            datalength=eepr_ch_cfg[eepr_state.currentchannel].length;
            for(i = 0; i < datalength; i++) {
                eepr_WR_RD_buffer[i] = *(eepr_state.ramaddress + i);
            }
            eepr_WR_RD_Chksum = *(uint32_t*)(eepr_state.ramaddress + (datalength -4));
            if(EEPR_CalcChecksum(eepr_WR_RD_buffer, datalength-4) != eepr_WR_RD_Chksum){
                /* the data in the WR_RD-Buffer aren´t valid, therefore don´t write them into eeprom. */
                eepr_state.state = EEPR_WRITEFAILED;
                OS_TaskExit_Critical();
                return;
            }
            OS_TaskExit_Critical();
        }

        if (eepr_state.stateend == EEPR_WRITEMEMORY || eepr_state.stateend == EEPR_READMEMORY) {
            if(eepr_ch_cfg[eepr_state.currentchannel].eepromaddress % EEPR_PageLength == 0) {
                eepr_state.eepromaddress1 = eepr_ch_cfg[eepr_state.currentchannel].eepromaddress;
                eepr_state.eepromaddress2 = eepr_state.eepromaddress1 + EEPR_PageLength;
                eepr_state.nr_of_pages = (eepr_ch_cfg[eepr_state.currentchannel].length+(EEPR_PageLength-1))/EEPR_PageLength; /*  will be used for EEPROM write in blocks of EEPR_PageLength Byte pages */
            } else {
                eepr_state.eepromaddress1 = eepr_ch_cfg[eepr_state.currentchannel].eepromaddress;
                eepr_state.eepromaddress2 = eepr_state.eepromaddress1 + EEPR_PageLength - (eepr_ch_cfg[eepr_state.currentchannel].eepromaddress % EEPR_PageLength);
                eepr_state.nr_of_pages = (((eepr_ch_cfg[eepr_state.currentchannel].length - (EEPR_PageLength-(eepr_ch_cfg[eepr_state.currentchannel].eepromaddress % EEPR_PageLength)))+(EEPR_PageLength-1))/EEPR_PageLength)+1;/*  will be used for EEPROM write in blocks of EEPR_PageLength Byte pages */
                /* will be used for EEPROM write in blocks of EEPR_PageLength Byte pages */
                /* calculation: first page is from startaddress to next multiple of EEPR_PageLength, following pages are always EEPR_PageLength bytes */
            }
            eepr_state.readtime =  (eepr_state.nr_of_pages) * EEPR_READTIME_PER_PAGE;         /* max.  EEPR_READTIME_PER_PAGE ms time for reading for each page (EEPR_PageLength Byte) */
            eepr_state.writetime = (eepr_state.nr_of_pages) * EEPR_WRITETIME_PER_PAGE;        /* max. EEPR_WRITETIME_PER_PAGE ms time for writing for each page */
            eepr_state.page_nr = 0;
            eepr_state.read_len = eepr_ch_cfg[eepr_state.currentchannel].length;
            eepr_state.write_len = eepr_ch_cfg[eepr_state.currentchannel].length;
        }

    }
    else {
        eepr_state.statereq = EEPR_NO_REQUEST; /* delete Request if declined */
    }
}


/**
 * @fn     void EEPR_ClearErrorEvent(void)
 * @brief  sends error message and clears the error event
*/

void EEPR_ClearErrorEvent(void){

    switch (eepr_state.statereq) {
        case EEPR_WRITEMEMORY:
            (void)(DIAG_Handler(DIAG_CH_CALIB_EEPR_FAILURE,DIAG_EVENT_RESET,0, NULL));  /* clear error event */
            break;

        case EEPR_READMEMORY:
            (void)(DIAG_Handler(DIAG_CH_CALIB_EEPR_FAILURE,DIAG_EVENT_RESET,1, NULL));  /* clear error event */
            break;

        case EEPR_IDLE:
            if(eepr_state.state == EEPR_IDLE || eepr_state.state  ==  EEPR_READFAILED || eepr_state.state  ==  EEPR_WRITEFAILED){
                (void)(DIAG_Handler(DIAG_CH_CALIB_EEPR_FAILURE,DIAG_EVENT_RESET,2, NULL));  /* clear error event */
            }
            else if(eepr_state.state == EEPR_UNINITIALIZED || eepr_state.state == EEPR_INITFAILED || eepr_state.state == EEPR_DISABLED){
                (void)(DIAG_Handler(DIAG_CH_CALIB_EEPR_FAILURE,DIAG_EVENT_RESET,3, NULL));  /* clear error event */
            }
            break;

        case EEPR_UNINITIALIZED:
            (void)(DIAG_Handler(DIAG_CH_CALIB_EEPR_FAILURE,DIAG_EVENT_RESET,4, NULL));  /* clear error event */
            break;

        default:
            break;

    }
}


/**
 * @brief   transition function for error states
 *
 * @param   state error state
 */
static void EEPR_StateFailed(EEPR_STATE_e state) {
    eepr_state.state    = state;
    eepr_state.substate = 0;
    eepr_state.repeat = 2;    /* reset re-try-timer for next usage */
}


/**
 * @brief   EEPROM state is reset to state after initialization
 */
static void EEPR_ReEnterStateInit(void) {
    eepr_state.page_nr=0;
   /* eepr_state.eepromaddress1 = EEPR_DATA_STARTADDR; */
    eepr_state.repeat = 2;    /* reset re-try-timer for next usage */
}





void EEPR_Trigger(void) {

    uint16_t i; /* running variable for loop */
    EEPR_RETURNTYPE_e ret_val;
    EEPR_STATUSREGISTER_s tmp_eepr_stat;
    uint16_t c_idx;
    uint32_t chksum;
    EEPR_STATE_e statereq;
    /* Check re-entrance of function */

    taskENTER_CRITICAL();
    if(!eepr_state.triggerentry) {

        eepr_state.triggerentry++;
        ret_val = EEPR_OK;
    }
    else
        ret_val = EEPR_ERROR;    /* multiple calls of function */
    statereq=eepr_state.statereq;
    taskEXIT_CRITICAL();

    if (ret_val == EEPR_ERROR)
        return;

    if(eepr_state.timer) {

        --eepr_state.timer;
        eepr_state.triggerentry--;
        return;    /* handle state machine only when timer has elapsed */
    }

    if(statereq != EEPR_NO_REQUEST)
        EEPR_TransferStateRequest(); /* update current state when there is a request */


    switch(eepr_state.state) {

        case EEPR_DISABLED:
            break;

        case EEPR_UNINITIALIZED:

            ret_val=EEPR_StartInitialization();        /* initialize EEPROM */
            if(ret_val == EEPR_OK) {

                eepr_state.timer  = 5;
                eepr_state.state  = EEPR_INIT_INPROCESS;
                eepr_state.repeat = 2;    /* reset repeat counter for next usage */
            } else {

                if(--eepr_state.repeat) {

                    eepr_state.timer  = 5;    /* retry in 5ms */
                } else {

                    eepr_state.state  = EEPR_INITFAILED;
                    eepr_state.repeat = 2;    /* reset repeat counter for next usage */
                }
            }
            break;

        case EEPR_INIT_INPROCESS:

            /* wait for response */
            ret_val = EEPR_ReceiveData((uint8_t*)(&tmp_eepr_stat),1);

            if(ret_val != EEPR_OK) {
            /* no data available */
                if(--eepr_state.repeat) {

                    eepr_state.timer  = 2;    /* retry Reading in 1ms */
                } else {

                    EEPR_StateFailed(EEPR_INITFAILED);
                }
                break;
            } else {

                eepr_state.repeat = 2;    /* reset repeat counter for next usage */
            }

            /* response received */
            if(tmp_eepr_stat.B.nRDY) {
            /* /RDY-Flag is set -> EEPROM is busy (write cycle progress) */
                if(--eepr_state.repeat) {

                    eepr_state.timer    = 10;    /* restart initialization */
                    eepr_state.state    = EEPR_UNINITIALIZED;
                } else {

                    EEPR_StateFailed(EEPR_INITFAILED);
                }

            } else {
                /* nRDY = 0, device is ready */
                eepr_state.RSR = tmp_eepr_stat;    /* overtake status information */
                /* initialization succeeded */
                eepr_state.timer    = 0;
                eepr_state.state    = EEPR_IDLE;
#if 0
                /* check write protection */
                protection_request.SR = (EEPR_STATUSREGISTER_s)(uint8_t)EEPR_HW_PROTECTION;
                if((eepr_state.RSR.B.BP0 == protection_request.SR.B.BP0) && (eepr_state.RSR.B.BP1 == protection_request.SR.B.BP1)){
                    /* correct write protection is set, initialization successful */
                    eepr_state.hwprotection = EEPR_HW_PROTECTION;
                    setProtectedAddresses();
                    eepr_state.state    = EEPR_IDLE;
                } else {
                    /* request and set protection */
                    eepr_state.state = EEPR_INIT_PROTECTION_CFG;
                }
#endif
            }
            break;

#if 0
        case EEPR_INIT_PROTECTION_CFG:
            /* do first Write-Enable (WEN) */
            if(EEPR_SendData(&eepr_cmd_WREN[0], 1 ,0)  ==  EEPR_OK) {   /* send data, data length, receive data offset(leading dummy bytes) */
            /* Sending Command has been started */
                if(--eepr_state.repeat) {

                   eepr_state.timer    = 10;
                   eepr_state.state    = EEPR_ENABLE_PROTECTION_CFG;
                   eepr_state.substate = 0;
                   eepr_state.repeat=2;
                } else {

                    EEPR_StateFailed(EEPR_PROTECTION_FAILED);
                }
            } else {

                eepr_state.timer    = 1;
            }
            break;

        case EEPR_ENABLE_PROTECTION_CFG:

            if(eepr_state.substate  ==  0)
            {

                ret_val = EEPR_ReceiveData((uint8_t*)(&tmp_eepr_stat),1);

                if(ret_val != EEPR_OK)
                    break;

                /*Write-Enable (WEN) has been send !*/
                /*Now, verify status if WREN was accepted*/
                eepr_cmdbuf[0]=(EEPR_CMD_RDSR);    /* first byte is command, second one is dummy (for data receive) */
                if(EEPR_SendData(eepr_cmdbuf, 2, 1)  ==  EEPR_OK)
                {   /* send data, data length, receive data offset(leading dummy bytes) */

                    if(--eepr_state.repeat) {

                        eepr_state.timer    = 1;
                        eepr_state.substate = 1;
                        eepr_state.repeat=2;
                        /* stay in state EEPR_WRITE_ENABLE_INPROCESS */
                    } else {

                       EEPR_StateFailed(EEPR_PROTECTION_FAILED);
                    }
                } else {

                    eepr_state.timer    = 1;    /* retry Sending */
                }
            }
            else if(eepr_state.substate  ==  1)
            {

                /* check Status Register of EEPROM (RSR) */
                ret_val = EEPR_ReceiveData((uint8_t*)(&tmp_eepr_stat),1);
                if(ret_val != EEPR_OK) {

                    if(--eepr_state.repeat) {

                    /* no data available */
                        eepr_state.timer    = 1;    /* retry Receiving */
                    } else {

                        EEPR_StateFailed(EEPR_PROTECTION_FAILED);
                    }
                    break;
                }

                /* response received */
                if(!tmp_eepr_stat.B.nRDY) {

                /* /RDY-Flag is cleared -> EEPROM is not busy (write cycle not in process) */
                    eepr_state.RSR = tmp_eepr_stat;    /*  overtake status information */
                    /* initialization succeeded */

                    if(eepr_state.RSR.B.WEN) {

                    /* Write-Enable (WEN) has been accepted !*/
                        eepr_state.state = EEPR_PROTECTION_CFG_ENABLED;
                        break;
                    }
                }
                /* Write Not Enabled OR EEPROM is busy */
                /* retry Write Enabling */
                if(--eepr_state.repeat) {

                    eepr_state.state    = EEPR_INIT_PROTECTION_CFG;    /* retry to enable the write statusregister process */
                    eepr_state.substate = 0;
                    eepr_state.timer    = 5;
                } else {

                   EEPR_StateFailed(EEPR_PROTECTION_FAILED);
                }
            }
            break;

        case EEPR_PROTECTION_CFG_ENABLED:
            ret_val = EEPR_SetHwProtection();
            if(ret_val != EEPR_OK) {

                if(--eepr_state.repeat) {

                    eepr_state.timer    = 2;    /* retry Start Command of EEPROM SRWrite */
                } else {

                    EEPR_StateFailed(EEPR_PROTECTION_FAILED);
                }
            } else {
                eepr_state.timer    = 5;
                eepr_state.substate = 0;
                eepr_state.state    = EEPR_PROTECTION_CFG_INPROCESS;
            }
            break;

        case EEPR_PROTECTION_CFG_INPROCESS:
            ret_val = EEPR_ReceiveData(&protectionbuf[0], 2);

            if(ret_val != EEPR_OK)
                break;
            if(eepr_state.substate == 0){
                eepr_cmdbuf[0]=(EEPR_CMD_RDSR);    /* first byte is command, second one is dummy (for data receive) */
                if(EEPR_SendData(eepr_cmdbuf, 2, 1)  ==  EEPR_OK)
                {
                    if(--eepr_state.repeat) {
                        eepr_state.timer    = 1;
                        eepr_state.substate = 1;
                        eepr_state.repeat=2;
                        /* stay in state EEPR_PROTECTION_CFG_INPROCESS */
                    } else {

                       EEPR_StateFailed(EEPR_PROTECTION_FAILED);
                    }
                } else {

                    eepr_state.timer    = 1;    /* retry Sending */
                }
            } else {
                ret_val = EEPR_ReceiveData(&protectionbuf[0], 1);
                if(ret_val  ==  EEPR_OK) {

                    if(--eepr_state.repeat) {

                        eepr_state.RSR = (EEPR_STATUSREGISTER_s)protectionbuf[0];
                        if((eepr_state.RSR.B.BP0 == protection_request.SR.B.BP0) && (eepr_state.RSR.B.BP1 == protection_request.SR.B.BP1))
                        {
                            /* protection bits have been set, continue to Idle */
                            EEPR_ReEnterStateInit();
                            eepr_state.hwprotection = EEPR_HW_PROTECTION;
                            setProtectedAddresses();
                            eepr_state.state    = EEPR_IDLE;
                        }else{
                            EEPR_StateFailed(EEPR_PROTECTION_FAILED);
                        }

                    } else {

                        EEPR_StateFailed(EEPR_PROTECTION_FAILED);
                    }
                }
            }
            break;
#endif
        case EEPR_READMEMORY:
            /* at first, set the new read_len by subtracting the number of bytes already read. */
            if(eepr_state.page_nr == 1)
                eepr_state.read_len = eepr_state.read_len - (EEPR_PageLength - (eepr_ch_cfg[eepr_state.currentchannel].eepromaddress % EEPR_PageLength));
            if(eepr_state.page_nr > 1)
                eepr_state.read_len = eepr_state.read_len - EEPR_PageLength;



            if(eepr_state.page_nr == 0) {
                if(eepr_state.read_len > (EEPR_PageLength - (eepr_ch_cfg[eepr_state.currentchannel].eepromaddress % EEPR_PageLength)))
                    eepr_state.currentpagelength = EEPR_PageLength - (eepr_ch_cfg[eepr_state.currentchannel].eepromaddress % EEPR_PageLength);
                else
                    eepr_state.currentpagelength = eepr_state.read_len;
                ret_val = EEPR_ReadMemory(eepr_state.eepromaddress1,eepr_state.currentpagelength);
            } else {
                if(eepr_state.read_len > EEPR_PageLength)
                    eepr_state.currentpagelength = EEPR_PageLength;
                else
                    eepr_state.currentpagelength = eepr_state.read_len;
                ret_val = EEPR_ReadMemory(eepr_state.eepromaddress2,eepr_state.currentpagelength);
            }

            if(ret_val != EEPR_OK) {

                if(--eepr_state.repeat) {
                    eepr_state.timer    = 2;    /* retry Start of EEPROM Reading */
                } else {
                    EEPR_StateFailed(EEPR_READFAILED);
                }
            } else {
                eepr_state.timer    = 2;
                eepr_state.state    = EEPR_READMEMORY_INPROCESS;
            }
            break;

        case EEPR_READMEMORY_INPROCESS:

            ret_val = EEPR_ReceiveData(&eepr_WR_RD_buffer[eepr_ch_cfg[eepr_state.currentchannel].length - eepr_state.read_len], eepr_state.currentpagelength);

            if(ret_val  ==  EEPR_OK) {

                if(++eepr_state.page_nr < eepr_state.nr_of_pages) {
                    if(eepr_state.page_nr != 1){
                        eepr_state.eepromaddress2 += EEPR_PageLength;
                    }
                    eepr_state.state    = EEPR_READMEMORY;    /* read next page */
                } else {
                    eepr_state.state    = EEPR_CHECK_DATA;
                }
            } else {
                if(--eepr_state.repeat) {
                    eepr_state.timer    = 2;    /* retry receiving data */
                } else {
                    EEPR_StateFailed(EEPR_READFAILED);
                }
            }
            break;

        case EEPR_CHECK_DATA:

            c_idx = eepr_ch_cfg[eepr_state.currentchannel].length;
            chksum =* (uint32_t*)(&eepr_WR_RD_buffer[c_idx-4]);

            if(chksum != EEPR_CalcChecksum(eepr_WR_RD_buffer, c_idx - 4)) {   /* Verify Checksum */
            /* data corrupt */
                if(--eepr_state.repeat) {

                    /* @FIXME: there is no repetition implemented */
                    EEPR_StateFailed(EEPR_READFAILED);
                    /* EEPR_ReEnterStateInit(); */
                } else {
                    EEPR_StateFailed(EEPR_READFAILED);
                    /********************************************/
                    /* KEEP CALBRATION DATA at last valid NVRAM data (CALIB_eNVMDATA)
                     * or at default CALIB_defaultDATA*/
                    /********************************************/
                }
            } else {
                for(i = 0; i < eepr_ch_cfg[eepr_state.currentchannel].length; i++)
                    *(eepr_state.ramaddress+i) = (uint8_t) eepr_WR_RD_buffer[i];

                eepr_ch_cfg[eepr_state.currentchannel].errorflag = EEPR_NO_ERROR;
                eepr_state.state    = EEPR_IDLE;
                EEPR_ReEnterStateInit();
            }
            break;

        case EEPR_IDLE:

            /* wait for State Change Request */
            EEPR_ReEnterStateInit();
            break;

        case EEPR_WRITEMEMORY:

            /* do first Write-Enable (WEN) */
            if(EEPR_SendData(&eepr_cmd_WREN[0], 1 ,0)  ==  EEPR_OK) {   /*  send data, data length, receive data offset(leading dummy bytes) */
            /* Sending Command has been started */
                eepr_state.timer    = 10;
                eepr_state.state    = EEPR_WRITE_ENABLE_INPROCESS;
                eepr_state.substate = 0;
                eepr_state.repeat=2;
            } else {
                if(--eepr_state.repeat) {
                    eepr_state.timer    = 1;
                } else {
                    EEPR_StateFailed(EEPR_WRITEFAILED);
                }
            }
            break;

        case EEPR_WRITE_ENABLE_INPROCESS:

            if(eepr_state.substate  ==  0) {

                ret_val = EEPR_ReceiveData((uint8_t*)(&tmp_eepr_stat),1);

                if(ret_val != EEPR_OK)
                    break;

                /*Write-Enable (WEN) has been send !*/
                /*Now, verify status if WREN was accepted*/
                eepr_cmdbuf[0]=(EEPR_CMD_RDSR);    /* first byte is command, second one is dummy (for data receive) */
                if(EEPR_SendData(eepr_cmdbuf, 2, 1)  ==  EEPR_OK) {   /* send data, data length, receive data offset(leading dummy bytes) */
                    eepr_state.timer    = 1;
                    eepr_state.substate = 1;
                    eepr_state.repeat=2;
                        /* stay in state EEPR_WRITE_ENABLE_INPROCESS */
                } else {
                    if(--eepr_state.repeat) {
                        eepr_state.timer    = 1;    /*  retry Sending */
                    } else {
                        EEPR_StateFailed(EEPR_WRITEFAILED);
                    }
                    break;
                }
            }
            else if(eepr_state.substate  ==  1) {

            /* check Status Register of EEPROM (RSR) */
                ret_val = EEPR_ReceiveData((uint8_t*)(&tmp_eepr_stat),1);
                if(ret_val != EEPR_OK) {

                    if(--eepr_state.repeat) {

                    /* no data available */
                        eepr_state.timer    = 1;    /*  retry Receiving */
                    } else {
                        EEPR_StateFailed(EEPR_WRITEFAILED);
                    }
                    break;
                }

                /* response received */
                if(!tmp_eepr_stat.B.nRDY) {

                /* /RDY-Flag is cleared -> EEPROM is not busy (write cycle not in process) */
                    eepr_state.RSR = tmp_eepr_stat;    /*  overtake status information */
                    /*  initialization succeeded */

                    if(eepr_state.RSR.B.WEN) {

                    /* Write-Enable (WEN) has been accepted !*/
                        eepr_state.state = EEPR_WRITEMEMORY_ENABLED;
                        break;
                    }
                }
                /* Write Not Enabled OR EEPROM is busy */
                /* retry Write Enabling */
                if(--eepr_state.repeat) {

                    eepr_state.state    = EEPR_WRITEMEMORY;    /* retry reading of EEPROM */
                    eepr_state.timer    = 5;
                } else {

                   EEPR_StateFailed(EEPR_WRITEFAILED);
                }
            }
            break;

        case EEPR_WRITEMEMORY_ENABLED:

            if(eepr_state.page_nr == 1) {
                eepr_state.write_len = eepr_state.write_len - (EEPR_PageLength - (eepr_ch_cfg[eepr_state.currentchannel].eepromaddress % EEPR_PageLength));
            }
            if(eepr_state.page_nr > 1) {
                eepr_state.write_len = eepr_state.write_len - EEPR_PageLength;
            }

            if(eepr_state.page_nr == 0) {
                if(eepr_state.write_len > (EEPR_PageLength - (eepr_ch_cfg[eepr_state.currentchannel].eepromaddress % EEPR_PageLength))) {
                    eepr_state.currentpagelength = EEPR_PageLength - (eepr_ch_cfg[eepr_state.currentchannel].eepromaddress % EEPR_PageLength);
                } else {
                    eepr_state.currentpagelength = eepr_state.write_len;
                }
                ret_val = EEPR_WriteMemory(eepr_state.eepromaddress1,(uint8_t*)((uint32_t)(&eepr_WR_RD_buffer)+eepr_ch_cfg[eepr_state.currentchannel].length - eepr_state.write_len),eepr_state.currentpagelength);
                /* calculation of srcptr: startaddress+(totalwrtlngth - yet to read length) */
            } else {
                if(eepr_state.write_len > EEPR_PageLength) {
                    eepr_state.currentpagelength = EEPR_PageLength;
                } else {
                    eepr_state.currentpagelength = eepr_state.write_len;
                }
                ret_val = EEPR_WriteMemory(eepr_state.eepromaddress2, (uint8_t*)((uint32_t)(&eepr_WR_RD_buffer) +
                        eepr_ch_cfg[eepr_state.currentchannel].length - eepr_state.write_len), eepr_state.currentpagelength);
                /* calculation of srcptr: startaddress+(totalwrtlngth - yet to read length) */
            }

            if(ret_val != EEPR_OK) {

                if(--eepr_state.repeat) {
                    eepr_state.timer    = 2;    /* retry Start Command of EEPROM Write */
                } else {
                    EEPR_StateFailed(EEPR_WRITEFAILED);
                }
            } else {
                eepr_state.timer    = 10;    /* max time: 10ms/EEPR_PageLength-byte page /* todo: timer: unterschiedlich neu/alt */
                eepr_state.state    = EEPR_WRITEMEMORY_INPROCESS;
            }
            break;

        case EEPR_WRITEMEMORY_INPROCESS:

            ret_val = EEPR_ReceiveData(&eepr_WR_RD_buffer[eepr_ch_cfg[eepr_state.currentchannel].length - eepr_state.write_len],eepr_state.currentpagelength);
            if(ret_val  ==  EEPR_OK) {
                if(++eepr_state.page_nr < eepr_state.nr_of_pages) {
                    if(eepr_state.page_nr != 1) {
                        eepr_state.eepromaddress2 += EEPR_PageLength;
                    }
                    eepr_state.state    = EEPR_WRITEMEMORY;    /* write next page */
                    eepr_state.repeat=2;
                } else {

                    eepr_state.eepromaddress2 = eepr_state.eepromaddress1 + EEPR_PageLength - (eepr_ch_cfg[eepr_state.currentchannel].eepromaddress % EEPR_PageLength);
                    /*now, do verify*/
                    EEPR_ReEnterStateInit();
                    eepr_state.state    = EEPR_READMEMORY;
                }
            } else {
                EEPR_StateFailed(EEPR_WRITEFAILED);
            }
            /* note: automatically write disable state at the completion of a write cycle ! */
            /* make verify/checksum test ? */
            break;

        case EEPR_INITFAILED:
            if (eepr_state.substate == 0) {
                (void)(DIAG_Handler(DIAG_CH_CALIB_EEPR_FAILURE,DIAG_EVENT_NOK,2, NULL));
                EEPR_ReEnterStateInit();
                eepr_state.substate = 1;
            }

            break;

        case EEPR_READFAILED:
            if (eepr_state.substate == 0) {
                eepr_ch_cfg[eepr_state.currentchannel].errorflag = EEPR_ERR_RD;
                (void)(DIAG_Handler(DIAG_CH_CALIB_EEPR_FAILURE,DIAG_EVENT_NOK,1, NULL));
                IO_WritePin(IO_PIN_MCU_0_TO_FPGA_INTERFACE_SPI_NSS, IO_PIN_SET);
                EEPR_ReEnterStateInit();
                eepr_state.substate = 1;
            }
            break;

        case EEPR_WRITEFAILED:
            if (eepr_state.substate == 0) {
                eepr_ch_cfg[eepr_state.currentchannel].errorflag = EEPR_ERR_WR;
                (void)(DIAG_Handler(DIAG_CH_CALIB_EEPR_FAILURE,DIAG_EVENT_NOK,0, NULL));
                EEPR_ReEnterStateInit();
                eepr_state.substate = 1;
            }
            break;

        case EEPR_PROTECTION_FAILED:
            /* todo */
            if (eepr_state.substate == 0) {
                EEPR_ReEnterStateInit();
            }

            break;

        default:
            break;
    }
    eepr_state.triggerentry--;
}


/*==================INTERFACE FUNCTIONS===================*/

void EEPR_delete_bkpsrambuffer(EEPR_CHANNEL_ID_TYPE_e eepr_channel) {
    uint16_t counter;
    for(counter = 0; counter < eepr_ch_cfg[eepr_channel].length; counter++) {
        eepr_bkpsram_buffer[counter] = 0xFF;
    }
}

/**
 * @brief  copys the data of one channel from bkpsram into the eeprom
 *
 * @param  eepr_channel the channel that will be copied
 * @return 0 if everything worked correctly, error bit if something failed
*/
EEPR_ERRORTYPES_e EEPR_BackupChannelData2Eepr(EEPR_CHANNEL_ID_TYPE_e eepr_channel) {
    uint8_t*dataptr = eepr_ch_cfg[eepr_channel].bkpsramptr;
    EEPR_ERRORTYPES_e retval = EEPR_NO_ERROR;
    timeout_cnt = 0;
    uint16_t maxtime = EEPR_GetChWriteTime(eepr_channel);

    /* @FIXME check dataptr== NULL_PTR */
    if(EEPR_SetStateRequest(EEPR_WRITEMEMORY, eepr_channel, dataptr) == EEPR_OK) {
        EEPR_Trigger();
        while (EEPR_GetState() != EEPR_IDLE) {
            if (timeout_cnt > maxtime) {
                retval |= EEPR_ERR_WR;    /* possible reasons: SPI busy, eeprom hardware in busy state or defect */
                break;
            }
            timeout_cnt++;
            EEPR_Trigger();
            OS_taskDelay(1);             /* wait 1ms for next call of eeprom trigger */
        }
    } else {
        retval = EEPR_ERR_WR;
    }
    if(retval == EEPR_NO_ERROR) {
        EEPR_RemoveChDirtyFlag(eepr_channel);
    }
   return retval;
}

/**
 * @brief  copys the data of one channel from eeprom into the bkpsrams
 *
 * @param  eepr_channel the channel that will be copied
 * @return 0 if everything worked correctly, error bit if something failed
*/
EEPR_ERRORTYPES_e EEPR_ReadChannelData(EEPR_CHANNEL_ID_TYPE_e eepr_channel) {
    timeout_cnt = 0;
    EEPR_ERRORTYPES_e retval = EEPR_NO_ERROR;
    uint8_t*dataptr = eepr_ch_cfg[eepr_channel].bkpsramptr;
    uint16_t maxtime = EEPR_GetChReadTime(eepr_channel);

    /* @FIXME check dataptr== NULL_PTR */
    if(EEPR_SetStateRequest(EEPR_READMEMORY,eepr_channel, dataptr) == EEPR_OK) {
        EEPR_Trigger();
        while (EEPR_GetState() != EEPR_IDLE){
            if (timeout_cnt > maxtime) {
                retval = EEPR_ERR_RD;     /* possible reasons: SPI busy, eeprom hardware in busy state or defect */
                break;
            }
            timeout_cnt++;
            EEPR_Trigger();
            OS_taskDelay(1);             /* wait 1ms for next call of eeprom trigger */
        }
    } else {
        retval = EEPR_ERR_RD;
    }
    if(retval == EEPR_NO_ERROR) {
        EEPR_RemoveChDirtyFlag(eepr_channel);
    }
    return retval;
}


/**
 * @brief  handles the data exchange between eeprom and bkpsram
 *          checks if the bkpsram data are valid.
 *          If so, and a dirty flag is set, the function saves the data into eeprom
 *          If not, the function recovers the BKPSRAM data backed up from the eeprom
 *
 * @param  eepr_channel pointer where the channel data are stored
 * @return none
*/
EEPR_ERRORTYPES_e EEPR_RefreshChannelData(EEPR_CHANNEL_ID_TYPE_e eepr_channel) {
    EEPR_ERRORTYPES_e retval = 0;
    uint8_t*dataptr = eepr_ch_cfg[eepr_channel].bkpsramptr;
    uint16_t byte_len = eepr_ch_cfg[eepr_channel].length;

    /* if no RAM pointer defined in channel, use a temporary buffer */
    if(dataptr==NULL_PTR) {
        dataptr = eepr_bkpsram_buffer;
    }
    /* verify the checksum of the RAM or Backup-SRAM instance */
    if(EEPR_CalcChecksum(dataptr, byte_len - 4) == *((uint32_t*)(dataptr + byte_len - 4))) {
        /*  checksum ok, if dirty flag is set actualize eeprom data (copy channel data RAM -> eeprom ) */
        if(eepr_ch_dirtyflag[eepr_channel] == 0) {
            retval = EEPR_BackupChannelData2Eepr(eepr_channel);
        }
    } else {
        /*  checksum corrupt, recover RAM instance by eeprom data (copy channel data eeprom -> RAM or BKPSRAM) */
        retval = EEPR_ReadChannelData(eepr_channel);
    }

    if(dataptr == eepr_bkpsram_buffer) { /* clear temporary buffer for next usage (invalidate checksum) */
        EEPR_delete_bkpsrambuffer(eepr_channel);
    }
    return retval;
}


EEPR_ERRORTYPES_e EEPR_Init(void) {
    timeout_cnt = 0;
    EEPR_ERRORTYPES_e retval;
    retval = EEPR_NO_ERROR;
    uint16_t maxtime = EEPR_WRITETIME_PER_PAGE;
    if(EEPR_SetStateRequest(EEPR_IDLE, 0, 0) == EEPR_OK) {
        while (EEPR_GetState() != EEPR_IDLE) {
            if (timeout_cnt > maxtime) {   /* timeout: 50ms (normal needed time: 5ms) */
               /* eeprom not usable, no eeprom data available */
                retval = EEPR_INIT_ERROR;     /* possible reasons: SPI busy, eeprom hardware in busy state or defect */
                break;
            }
            timeout_cnt++;
            EEPR_Trigger();
            OS_taskDelay(1);             /* wait 1ms for next call of eeprom trigger */
        }
    }

    if(retval == EEPR_NO_ERROR) {
        /* initialization successful */
        /* intialize flags */
        for(cnt = 0; cnt < eepr_nr_of_channels; cnt++) {
            EEPR_RemoveChReadReqFlag(cnt);
        }
        retval = EEPR_InitChannelData();      /* do update channel data */
    } else {
        EEPR_SetDefaultValue(EEPR_CH_HEADER);
        EEPR_SetDefaultValue(EEPR_CH_BOARD_INFO);
    }
    return retval;
}

EEPR_ERRORTYPES_e EEPR_InitChannelData(void) {
    EEPR_ERRORTYPES_e retval = EEPR_NO_ERROR;
    EEPR_ERRORTYPES_e errtype = EEPR_NO_ERROR;

    /* Read eeprom header check data format */
    errtype = EEPR_ReadChannelData(EEPR_CH_HEADER);
    if(errtype == EEPR_NO_ERROR) {
        EEPR_FormatCheck();  /* handles data transfer (update) in case of new eeprom driver version with different data format */
    } else {
        /* try to read a second time */
        errtype = EEPR_ReadChannelData(EEPR_CH_HEADER);
        if(errtype == EEPR_NO_ERROR) {
            EEPR_FormatCheck();  /* handles data transfer (update) in case of new eeprom driver version with different data format */
        } else {
            /* seems to be the very first time of eeprom initialization */
            EEPR_SetDefaultValue(EEPR_CH_HEADER);
            errtype = EEPR_BackupChannelData2Eepr(EEPR_CH_HEADER);
            if(errtype != EEPR_NO_ERROR) {
                /* try to write a second time */
                EEPR_SetDefaultValue(EEPR_CH_HEADER);
                errtype = EEPR_BackupChannelData2Eepr(EEPR_CH_HEADER);
            }
            return errtype;
        }
    }

    /* Read board and device info */
    errtype = EEPR_ReadChannelData(EEPR_CH_BOARD_INFO);
    if(errtype != EEPR_NO_ERROR) {
        /* try to read a second time */
        errtype = EEPR_ReadChannelData(EEPR_CH_BOARD_INFO);
        if(errtype != EEPR_NO_ERROR) {
            /* when reading of board info fails */
            EEPR_SetDefaultValue(EEPR_CH_BOARD_INFO);
#if 0   /* @FIXME do not overwrite board info forever if somehow reading fails one time !!! */
            errtype = EEPR_BackupChannelData2Eepr(EEPR_CH_BOARD_INFO);
            if(errtype != EEPR_NO_ERROR) {
                /* try to write a second time */
                EEPR_SetDefaultValue(EEPR_CH_BOARD_INFO);
                errtype = EEPR_BackupChannelData2Eepr(EEPR_CH_BOARD_INFO);
            }
#endif
        }
    }

    /* Reset error because EEPR_Header is separate section in EEPROM */
    errtype = EEPR_NO_ERROR;

    if(RTC_NVMRAM_DATAVALID_VARIABLE == 0) {
        /* NVM data corrupt, so set all the dirty flags and take backup (if valid) or default values */
        uint8_t cnt;    /* counter variable */
        for(cnt = 0; cnt < eepr_nr_of_channels; cnt++) {
            EEPR_SetChDirtyFlag(cnt);
        }
        errtype |= EEPR_ReadChannelData(EEPR_CH_NVSOC);
        retval |= errtype;
        if (errtype != EEPR_NO_ERROR) {
            errtype = EEPR_NO_ERROR;
            EEPR_SetDefaultValue(EEPR_CH_NVSOC);
        }
        errtype |= EEPR_ReadChannelData(EEPR_CH_CONTACTOR);
        retval |= errtype;
        if (errtype != EEPR_NO_ERROR) {
            errtype = EEPR_NO_ERROR;
            EEPR_SetDefaultValue(EEPR_CH_CONTACTOR);
        }
        errtype |= EEPR_ReadChannelData(EEPR_CH_OPERATING_HOURS);
        retval |= errtype;
        if (errtype != EEPR_NO_ERROR) {
            errtype = EEPR_NO_ERROR;
            EEPR_SetDefaultValue(EEPR_CH_OPERATING_HOURS);
        }
        RTC_NVMRAM_DATAVALID_VARIABLE = 1;      /* validate NVNRAM data */
    } else {
        /* @FIXME do set dirty flags for not double buffered channel (not in bkpsram) unless the ram is not cleared (warm reset) */
        errtype |= EEPR_RefreshChannelData(EEPR_CH_NVSOC);
        retval |= errtype;
        if (errtype == EEPR_ERR_RD || errtype == (EEPR_ERR_RD | EEPR_ERR_WR)) {
            /* read error can only occur if checksum of bkpsram channel is corrupt -> set default values */
            /* ignore possible write error because we definitely want to try writing to EEPROM */
            errtype = EEPR_NO_ERROR;
            EEPR_SetDefaultValue(EEPR_CH_NVSOC);
        }

        errtype |= EEPR_RefreshChannelData(EEPR_CH_CONTACTOR);
        retval |= errtype;
        if (errtype == EEPR_ERR_RD || errtype == (EEPR_ERR_RD | EEPR_ERR_WR)) {
            /* read error can only occur if checksum of bkpsram channel is corrupt -> set default values */
            /* ignore possible write error because we definitely want to try writing to EEPROM */
            errtype = EEPR_NO_ERROR;
            EEPR_SetDefaultValue(EEPR_CH_CONTACTOR);
        }

        errtype |= EEPR_RefreshChannelData(EEPR_CH_OPERATING_HOURS);
        retval |= errtype;
        if (errtype == EEPR_ERR_RD || errtype == (EEPR_ERR_RD | EEPR_ERR_WR)) {
            /* read error can only occur if checksum of bkpsram channel is corrupt -> set default values */
            /* ignore possible write error because we definitely want to try writing to EEPROM */
            errtype = EEPR_NO_ERROR;
            EEPR_SetDefaultValue(EEPR_CH_OPERATING_HOURS);
        }
    }
    return retval;
}

EEPR_RETURNTYPE_e EEPR_GetChannelData(EEPR_CHANNEL_ID_TYPE_e eepr_channel, uint8_t *dest_ptr) {
    EEPR_RETURNTYPE_e ret_val = EEPR_OK;
    uint8_t *dataptr;
    dataptr = dest_ptr;

    if(dataptr == NULL_PTR) {
        if(dest_ptr != NULL_PTR){
            dataptr = eepr_ch_cfg[eepr_channel].bkpsramptr;
        } else {
            ret_val = EEPR_ERROR;
        }
    }
    if(ret_val == EEPR_OK) {
        ret_val = EEPR_SetStateRequest(EEPR_READMEMORY,eepr_channel, dataptr);
    }

    return ret_val;
}


EEPR_RETURNTYPE_e EEPR_SetChannelData(EEPR_CHANNEL_ID_TYPE_e eepr_channel, uint8_t *src_ptr) {
    EEPR_RETURNTYPE_e ret_val;
    uint8_t *dataptr;
    dataptr=src_ptr;
    uint16_t byte_len = eepr_ch_cfg[eepr_channel].length;

    if(dataptr == NULL_PTR) {
        dataptr = eepr_ch_cfg[eepr_channel].bkpsramptr;
    }
    /* checksumtest */
    if(EEPR_CalcChecksum(dataptr, byte_len - 4) == *((uint32_t*)(dataptr + byte_len - 4))) {
        ret_val = EEPR_SetStateRequest(EEPR_WRITEMEMORY,eepr_channel, dataptr);
    } else {
        ret_val = EEPR_ERROR;
    }
    if(ret_val == EEPR_OK) {
        EEPR_RemoveChDirtyFlag(eepr_channel);
    }
    return ret_val;
}

STD_RETURN_TYPE_e EEPR_CheckDirtyFlags(){
    STD_RETURN_TYPE_e ret_val = 0;
    cnt = 0;
    for(cnt = 0; cnt < (eepr_nr_of_channels); cnt++){
        if( eepr_ch_dirtyflag[cnt] == 0 ){
            ret_val = 1;
        }
    }
    return ret_val;
}

uint8_t EEPR_GetNextReadReq(){
    uint8_t ret_val = 0xFF;
    uint8_t channel;
    cnt = 0;
    for(cnt = 0; cnt < (eepr_nr_of_channels + 1); cnt++){
        channel = (cnt + lastReadChannel + 1)%eepr_nr_of_channels;
        if(eepr_ch_read_req_flag[channel] == 0 ){
            lastReadChannel = channel;
            ret_val = channel;
            break;
        }
    }
    return ret_val;
}

uint8_t EEPR_GetNextDirtyChannel(){
    uint8_t ret_val = 0xFF;
    uint8_t channel;
    cnt = 0;
    for(cnt = 0; cnt < (eepr_nr_of_channels + 1); cnt++){
        channel = (cnt + lastDirtyChannel + 1)%eepr_nr_of_channels;
        if( eepr_ch_dirtyflag[channel] == 0 ){
            lastDirtyChannel = channel;
            ret_val = channel;
            break;
        }
    }
    return ret_val;
}

void EEPR_DataHandler(){
    uint16_t length = 0xFFFF;
    uint16_t chksum = 0xFFFF;
    uint16_t i = 0;
    /* check, if eeprom is busy */
    if(!EEPR_IsInFinalState()){
        return;
    }

    switch(data_handler.state){
        case EEPR_STARTDIRTYCHECK:
            /* Read requests have priority */
            data_handler.dirtyChannel = EEPR_GetNextReadReq();
            if(data_handler.dirtyChannel == 0xFF){
                /* nothing to read */
            }else{
                /* there is at least one channel to update */
                data_handler.repeat = 2;
                data_handler.state = EEPR_READREQUEST;
                EEPR_ch_dataerrors[data_handler.dirtyChannel] = EEPR_DAT_IDLE;
                eepr_ch_cfg[data_handler.dirtyChannel].accessprotection = EEPR_ACCESS_PROTECTED;
                return;
            }
            data_handler.dirtyChannel = EEPR_GetNextDirtyChannel();
            if(data_handler.dirtyChannel == 0xFF){
                /* nothing to update */
                return;
            }else{
                /* there is at least one channel to update */
                data_handler.repeat = 2;
                data_handler.state = EEPR_DATABACKUP;
                EEPR_ch_dataerrors[data_handler.dirtyChannel] = EEPR_DAT_IDLE;
                eepr_ch_cfg[data_handler.dirtyChannel].accessprotection = EEPR_ACCESS_PROTECTED;
                return;
            }
            break;

        case EEPR_DATABACKUP:
            /* first, delete old buffer */
            for(i = 0; i < EEPR_CH_MAXLENGTH; i++){
                eepr_backupbuffer[i] = 0xFF;
            }
            /* safe EEPROM-Data to Backupbuffer */
            /* todo: check if request was accepted */
            EEPR_GetChannelData(data_handler.dirtyChannel, (uint8_t*) &eepr_backupbuffer);
            data_handler.state = EEPR_CHECKBACKUP;
            break;

        case EEPR_CHECKBACKUP:
            /* check if safed eeprom data are valid */
            length=eepr_ch_cfg[data_handler.dirtyChannel].length;
            chksum=*(uint16_t*)(&eepr_backupbuffer[length-4]);
            if(chksum != EEPR_CalcChecksum(eepr_backupbuffer, length - 4)) {
                /* checksum corrupt */
                if(--data_handler.repeat != 0){
                    /* retry saving data */
                    data_handler.state = EEPR_DATABACKUP;
                }else{
                    /* backup error */
                    EEPR_ch_dataerrors[data_handler.dirtyChannel] |= EEPR_DAT_EEPROM_ERROR;
                    data_handler.repeat = 2;
                    data_handler.state = EEPR_WRITEEEPROM;
                }
            }else{
                data_handler.repeat = 2;
                data_handler.state = EEPR_WRITEEEPROM;
            }
            break;

        case EEPR_WRITEEEPROM:
            /* write new data into eeprom */
            byte_len = eepr_ch_cfg[data_handler.dirtyChannel].length;
            if(EEPR_CalcChecksum(eepr_ch_cfg[data_handler.dirtyChannel].bkpsramptr, byte_len - 4) ==
                    *((uint32_t*)(eepr_ch_cfg[data_handler.dirtyChannel].bkpsramptr + byte_len - 4))){
                /* checksum ok */
                if(EEPR_SetChannelData(data_handler.dirtyChannel, NULL_PTR) != EEPR_OK){
                    /* eeprom is busy, try again or restart if already repeated. */
                    if(--data_handler.repeat == 0){
                        EEPR_ch_dataerrors[data_handler.dirtyChannel] |= EEPR_DAT_EEPROM_ERROR;
                        data_handler.repeat = 2;
                        data_handler.state = EEPR_STARTDIRTYCHECK;
                    }
                } else {
                    data_handler.state = EEPR_CHECKEEPROM;
                }
            }else{
                /* Checksum of new data corrupt, use backup */
                length=eepr_ch_cfg[data_handler.dirtyChannel].length;
                chksum=*(uint16_t*)(&eepr_backupbuffer[length-4]);
                if(chksum != EEPR_CalcChecksum(eepr_backupbuffer, length - 4)) {
                    /* checksum of backed up data corrupt, try to read eeprom directly into ram */
                    EEPR_GetChannelData(data_handler.dirtyChannel, NULL_PTR);
                    data_handler.repeat = 2;
                    data_handler.state = EEPR_RECOVERYCHECK;
                }else{
                    /* backed up data valid */
                    for(i = 0; i < eepr_ch_cfg[data_handler.dirtyChannel].length; i++){
                        *(eepr_ch_cfg[data_handler.dirtyChannel].bkpsramptr+i) = (uint8_t) eepr_backupbuffer[i];
                    }
                    /* data are restored, handle next channel */
                    EEPR_ch_dataerrors[data_handler.dirtyChannel] |= EEPR_DAT_RAM_ERROR;
                    eepr_ch_cfg[data_handler.dirtyChannel].accessprotection = EEPR_ACCESS_UNPROTECTED;
                    data_handler.repeat = 2;
                    data_handler.state = EEPR_STARTDIRTYCHECK;
                }
            }
            break;

        case EEPR_CHECKEEPROM:
            if(eepr_ch_cfg[data_handler.dirtyChannel].errorflag == EEPR_ERR_WR || eepr_ch_cfg[data_handler.dirtyChannel].errorflag == EEPR_ERR_RD){
                /* writing into eeprom unsuccessful */
                length=eepr_ch_cfg[data_handler.dirtyChannel].length;
                chksum=*((uint32_t*)(eepr_ch_cfg[data_handler.dirtyChannel].bkpsramptr + length - 4));
                if(chksum  == EEPR_CalcChecksum(eepr_ch_cfg[data_handler.dirtyChannel].bkpsramptr, length - 4)){
                    /* new data are still valid */
                    if(--data_handler.repeat != 0){
                        /* eeprom error, retry writing into eeprom */
                        EEPR_ch_dataerrors[data_handler.dirtyChannel] |= EEPR_DAT_EEPROM_ERROR;
                        data_handler.state = EEPR_WRITEEEPROM;
                    }else{
                        /* eeprom error, ram data are valid. */
                        EEPR_ch_dataerrors[data_handler.dirtyChannel] |= EEPR_DAT_EEPROM_ERROR;
                        eepr_ch_cfg[data_handler.dirtyChannel].accessprotection = EEPR_ACCESS_UNPROTECTED;
                        data_handler.repeat = 2;
                        data_handler.state = EEPR_STARTDIRTYCHECK;
                    }

                }else{
                    /* new data and eeprom data are corrupt */
                    /* use backed up data to restore */
                    length=eepr_ch_cfg[data_handler.dirtyChannel].length;
                    chksum=*(uint16_t*)(&eepr_backupbuffer[length-4]);
                    if(chksum != EEPR_CalcChecksum(eepr_backupbuffer, length - 4)) {
                        /* fatal error, all data are corrupt */
                        EEPR_ch_dataerrors[data_handler.dirtyChannel] |= EEPR_DAT_FATAL_ERROR;
                        eepr_ch_cfg[data_handler.dirtyChannel].accessprotection = EEPR_ACCESS_UNPROTECTED;
                        data_handler.state = EEPR_STARTDIRTYCHECK;
                    }else{
                        /* backed up data are valid */
                        for(i = 0; i < eepr_ch_cfg[data_handler.dirtyChannel].length; i++){
                            *(eepr_ch_cfg[data_handler.dirtyChannel].bkpsramptr+i) = (uint8_t) eepr_backupbuffer[i];
                        }
                        if(--data_handler.repeat != 0){
                            /* retry writing into eeprom */
                            EEPR_ch_dataerrors[data_handler.dirtyChannel] |= EEPR_DAT_EEPROM_ERROR;
                            data_handler.state = EEPR_WRITEEEPROM;

                        }else{
                            /* eeprom error */
                            EEPR_ch_dataerrors[data_handler.dirtyChannel] |= EEPR_DAT_EEPROM_ERROR;
                            eepr_ch_cfg[data_handler.dirtyChannel].accessprotection = EEPR_ACCESS_UNPROTECTED;
                            data_handler.state = EEPR_STARTDIRTYCHECK;
                        }

                    }
                }

            }else{
                /* Idle, everything worked */
                EEPR_ch_dataerrors[data_handler.dirtyChannel] = EEPR_DAT_SUCCESS;
                eepr_ch_cfg[data_handler.dirtyChannel].accessprotection = EEPR_ACCESS_UNPROTECTED;
                EEPR_RemoveChDirtyFlag(data_handler.dirtyChannel);
                data_handler.state = EEPR_STARTDIRTYCHECK;
            }
            break;

        case EEPR_RECOVERYCHECK:
            length=eepr_ch_cfg[data_handler.dirtyChannel].length;
            chksum=*(uint16_t*)((uint8_t*)eepr_ch_cfg[data_handler.dirtyChannel].bkpsramptr + length -4);
            if(chksum != EEPR_CalcChecksum(eepr_ch_cfg[data_handler.dirtyChannel].bkpsramptr, length - 4)){
                /* recovery failed, fatal error */
                EEPR_ch_dataerrors[data_handler.dirtyChannel] |= EEPR_DAT_FATAL_ERROR;
                eepr_ch_cfg[data_handler.dirtyChannel].accessprotection = EEPR_ACCESS_UNPROTECTED;
                data_handler.state = EEPR_STARTDIRTYCHECK;
            }else{
                /* recovery successful */
                EEPR_ch_dataerrors[data_handler.dirtyChannel] |= EEPR_DAT_RAM_ERROR;
                EEPR_RemoveChDirtyFlag(data_handler.dirtyChannel);
                eepr_ch_cfg[data_handler.dirtyChannel].accessprotection = EEPR_ACCESS_UNPROTECTED;
                data_handler.state = EEPR_STARTDIRTYCHECK;
            }
            break;
        case EEPR_READREQUEST:
            if(EEPR_GetChannelData(data_handler.dirtyChannel, (uint8_t*) &eepr_backupbuffer) == EEPR_OK){
                /*  request was accepted */
                data_handler.state = EEPR_READCHECK;
            } else {
                if(--data_handler.repeat != 0) {
                    /* do nothing, try again */
                } else {
                    /* eeprom error */
                    EEPR_ch_dataerrors[data_handler.dirtyChannel] |= EEPR_DAT_EEPROM_ERROR;
                    data_handler.state = EEPR_STARTDIRTYCHECK;
                }
            }
            break;

        case EEPR_READCHECK:
            if(eepr_ch_cfg[data_handler.dirtyChannel].errorflag == EEPR_ERR_RD){
                /* Reading was unsuccessful */
                if(--data_handler.repeat != 0){
                    /* try to read again */
                    data_handler.state = EEPR_READREQUEST;
                } else {
                    /* eeprom error */
                    EEPR_ch_dataerrors[data_handler.dirtyChannel] |= EEPR_DAT_EEPROM_ERROR;
                    data_handler.state = EEPR_STARTDIRTYCHECK;
                }
            } else {
                /* check if safed eeprom data are valid */
                length=eepr_ch_cfg[data_handler.dirtyChannel].length;
                chksum=*(uint16_t*)(&eepr_backupbuffer[length-4]);
                if(chksum != EEPR_CalcChecksum(eepr_backupbuffer, length - 4)) {
                    /* checksum corrupt */
                    if(--data_handler.repeat != 0){
                        /* retry reading data */
                        data_handler.state = EEPR_READREQUEST;
                    }else{
                        /* backup error */
                        EEPR_ch_dataerrors[data_handler.dirtyChannel] |= EEPR_DAT_EEPROM_ERROR;
                        data_handler.repeat = 2;
                        data_handler.state = EEPR_STARTDIRTYCHECK;
                    }
                }else{
                    /* reading was successful, overtake data and start cycle from beginning */
                    for(i = 0; i < eepr_ch_cfg[data_handler.dirtyChannel].length; i++){
                        *(eepr_ch_cfg[data_handler.dirtyChannel].bkpsramptr+i) = (uint8_t) eepr_backupbuffer[i];
                    }
                    data_handler.repeat = 2;
                    EEPR_ch_dataerrors[data_handler.dirtyChannel] = EEPR_DAT_SUCCESS;
                    EEPR_RemoveChReadReqFlag(data_handler.dirtyChannel);
                    data_handler.state = EEPR_STARTDIRTYCHECK;
                }
            }
            break;
        default:
            break;

    }
    return;
}
