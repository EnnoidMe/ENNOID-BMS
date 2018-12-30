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
 * @file    diag.c
 * @author  foxBMS Team
 * @date    09.11.2015 (date of creation)
 * @ingroup ENGINE
 * @prefix  DIAG
 *
 * @brief   Diag driver implementation
 *
 * This diagnose module is responsible for error handling and reporting.
 * Reported errors are logged into the global database and can be reviewed
 * on user request.
 */

/*================== Includes =============================================*/
#include "diag.h"

#include "contactor.h"
#include "com.h"
#include "os.h"
#include "misc.h"
#include "nvramhandler.h"
#include "rtc.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/
static DIAG_s diag;
static DIAG_DEV_s  *diag_devptr;
static uint32_t diagsysmonTimestamp = 0;
static uint8_t diag_locked = 0;

/* FIXME unused */
/* FIXME do you really want to have global variables? */
DIAG_CODE_s diag_err;
DIAG_CODE_s diag_warn;
DIAG_OCCURRENCE_COUNTERS_s diag_occurrence_counters;

/* uint32_t diag_error; */

DIAG_SYSMON_NOTIFICATION_s diag_sysmon[DIAG_SYSMON_MODULE_ID_MAX];
DIAG_SYSMON_NOTIFICATION_s diag_sysmon_last[DIAG_SYSMON_MODULE_ID_MAX];

uint32_t diag_sysmon_cnt[DIAG_SYSMON_MODULE_ID_MAX];

DIAG_ERROR_ENTRY_s MEM_BKP_SRAM diag_memory[DIAG_FAIL_ENTRY_LENGTH];
DIAG_ERROR_ENTRY_s MEM_BKP_SRAM *diag_entry_wrptr;
DIAG_ERROR_ENTRY_s MEM_BKP_SRAM *diag_entry_rdptr;

DIAG_CONTACTOR_ERROR_ENTRY_s MEM_BKP_SRAM diagContactorErrorMemory[DIAG_FAIL_ENTRY_CONTACTOR_LENGTH];
DIAG_CONTACTOR_ERROR_ENTRY_s MEM_BKP_SRAM *diagContactorError_entry_wrptr;
DIAG_CONTACTOR_ERROR_ENTRY_s MEM_BKP_SRAM *diagContactorError_entry_rdptr;

DIAG_FAILURECODE_s diag_fc;

/*================== Function Prototypes ==================================*/
static void DIAG_Reset(void);
static uint8_t DIAG_EntryWrite(uint8_t eventID, DIAG_EVENT_e event, uint32_t item_nr);
static DIAG_RETURNTYPE_e DIAG_GeneralHandler(DIAG_CH_ID_e diag_ch_id, DIAG_EVENT_e event, uint32_t item_nr);
static DIAG_RETURNTYPE_e DIAG_ContHandler(DIAG_CH_ID_e eventID, uint32_t cont_nr, float* openingCur);

/*================== Function Implementations =============================*/

/**
 * @brief   DIAG_Reset resets/initalizes all needed strcutures/buffers.
 *
 * This function gets called during initialization of the diagnose module.
 * It clears memory and counters used by diag later on.
 */

static void DIAG_Reset(void)
{
    uint32_t i;
    uint32_t *u32ptr = (uint32_t*)(&diag_memory[0]);

    diag_locked = 1;

    /* Delete memory */
    for(i = 0; i < (sizeof(diag_memory))/4; i++)
        *u32ptr++ = 0;

    /* Reset counter */
    for(i = 0; i < sizeof(diag.entry_cnt); i++)
        diag.entry_cnt[i] = 0;

    /* Set pointer to beginning of buffer */
    diag_entry_wrptr = diag_entry_rdptr = &diag_memory[0];
    diag.errcnttotal = 0;

    /* Set pointer to beginning of buffer */
    u32ptr = (uint32_t*)(&diagContactorErrorMemory[0]);

    /* Delete memory */
    for(i = 0; i < (sizeof(diagContactorErrorMemory))/4; i++)
        *u32ptr++ = 0;

    /* Set pointer to beginning of buffer */
    diagContactorError_entry_wrptr = diagContactorError_entry_rdptr = &diagContactorErrorMemory[0];
    diag_locked=0;
}


STD_RETURN_TYPE_e DIAG_Init(DIAG_DEV_s *diag_dev_pointer, STD_RETURN_TYPE_e bkpramValid) {

    STD_RETURN_TYPE_e retval = E_OK;
    uint8_t c = 0;
    uint8_t id_nr = DIAG_ID_MAX;
    uint32_t tmperr_Check[(DIAG_ID_MAX+31)/32];

    diag_devptr = diag_dev_pointer;

    diag.state = DIAG_STATE_UNINITIALIZED;
    uint16_t checkfail = 0;

    if( (diag_entry_rdptr<&diag_memory[0]) || (diag_entry_rdptr >= &diag_memory[DIAG_FAIL_ENTRY_LENGTH]))
        checkfail |= 0x01;

    if( (diag_entry_wrptr<&diag_memory[0]) || (diag_entry_wrptr >= &diag_memory[DIAG_FAIL_ENTRY_LENGTH]))
        checkfail |= 0x02;

    if(bkpramValid == E_NOT_OK)
        checkfail |= 0x04;

    if( (diagContactorError_entry_rdptr < &diagContactorErrorMemory[0]) ||
            (diagContactorError_entry_rdptr >= &diagContactorErrorMemory[DIAG_FAIL_ENTRY_CONTACTOR_LENGTH]) )
        checkfail |= 0x08;

    if( (diagContactorError_entry_wrptr < &diagContactorErrorMemory[0]) ||
            (diagContactorError_entry_wrptr >= &diagContactorErrorMemory[DIAG_FAIL_ENTRY_CONTACTOR_LENGTH]))
        checkfail |= 0x10;


    if(checkfail) {
        DIAG_Reset();
    }

    /* Fill lookup table id2ch */
    for(c = 0; c < diag_dev_pointer->nr_of_ch; c++) {

        id_nr = diag_dev_pointer->ch_cfg[c].id;
        if(id_nr < DIAG_ID_MAX) {
            diag.id2ch[id_nr] = c;      /* e.g. diag.id2ch[DIAG_ID_90] = configured channel index */
        } else {
            /* Configuration error -> set retval to E_NOT_OK */
            checkfail |= 0x20;
            retval = E_NOT_OK;
        }
    }

    for(int i = 0; i < (DIAG_ID_MAX+31)/32; i++)
        tmperr_Check[i] = 0;

    /* Fill enable array err_enableflag */
    for(int i = 0; i < diag_dev_pointer->nr_of_ch; i++) {
        if(diag_dev_pointer->ch_cfg[i].state == DIAG_DISABLED) {

            /* Disable diagnosis entry */
            tmperr_Check[diag_dev_pointer->ch_cfg[i].id/32] |= 1 << (diag_dev_pointer->ch_cfg[i].id % 32);
        }
    }

    /* take over configured error enable masks*/
    for(c = 0; c < (DIAG_ID_MAX+31)/32; c++) {
        diag.err_enableflag[c] = ~tmperr_Check[c];
    }

    diag.state = DIAG_STATE_INITIALIZED;

    if(checkfail) {
        /* make first entry after DIAG_Reset() */
        (void)(DIAG_Handler(DIAG_CH_BKPDIAG_FAILURE, DIAG_EVENT_NOK, checkfail, NULL));
    }
    return retval;
}


void DIAG_PrintErrors(void) {

    /* FIXME if read once, rdptr is on writeptr, therefore in the next call the errors aren't
     * printed again. Maybe tmp save rdptr and set again at end of function. But when is the
     * diag memory cleared then? */

    uint8_t buf[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    if(diag_entry_rdptr == diag_entry_wrptr)
    {
        DEBUG_PRINTF((const uint8_t * )"no new entries in DIAG");
        DEBUG_PRINTF((const uint8_t * )"\r\n");
    }
    else
    {
        DEBUG_PRINTF((const uint8_t * )"DIAG error entries:");
        DEBUG_PRINTF((const uint8_t * )"\r\n");
        DEBUG_PRINTF((const uint8_t * )"Date and Time:      Error Code/Item   Status     Description");
        DEBUG_PRINTF((const uint8_t * )"\r\n");
    }
    uint8_t c = 0;
    while(diag_entry_rdptr != diag_entry_wrptr && c < 7)
    {

        if( diag_entry_rdptr >= &diag_memory[DIAG_FAIL_ENTRY_LENGTH]) {
            diag_entry_rdptr = &diag_memory[0];
        }

        DEBUG_PRINTF(U8ToDecascii(buf, &diag_entry_rdptr->DD, 2));
        DEBUG_PRINTF((const uint8_t * )".");
        DEBUG_PRINTF(U8ToDecascii(buf, &diag_entry_rdptr->MM, 2));
        DEBUG_PRINTF((const uint8_t * )".");
        DEBUG_PRINTF(U8ToDecascii(buf, &diag_entry_rdptr->YY, 2));
        DEBUG_PRINTF((const uint8_t * )" - ");
        DEBUG_PRINTF(U8ToDecascii(buf, &diag_entry_rdptr->hh, 2));
        DEBUG_PRINTF((const uint8_t * )":");
        DEBUG_PRINTF(U8ToDecascii(buf, &diag_entry_rdptr->mm, 2));
        DEBUG_PRINTF((const uint8_t * )":");
        DEBUG_PRINTF(U8ToDecascii(buf, &diag_entry_rdptr->ss, 2));
        DEBUG_PRINTF((const uint8_t * )"     ");
        DEBUG_PRINTF(U8ToDecascii(buf, (uint8_t*)(&diag_entry_rdptr->event_id), 2));

        DEBUG_PRINTF((const uint8_t * )" / 0x");
        DEBUG_PRINTF(U32ToHexascii(buf, &diag_entry_rdptr->item));
        DEBUG_PRINTF((const uint8_t * )"      ");

        if(diag_entry_rdptr->event == DIAG_EVENT_OK)
            DEBUG_PRINTF((const uint8_t * )"cleared     ");
        else if(diag_entry_rdptr->event == DIAG_EVENT_NOK)
            DEBUG_PRINTF((const uint8_t * )"occurred    ");
        else
            DEBUG_PRINTF((const uint8_t * )"reset       ");

        for(uint8_t i = 0; i < 24; i++)
            buf[i] = diag_devptr->ch_cfg[diag.id2ch[diag_entry_rdptr->event_id]].description[i];

        DEBUG_PRINTF((const uint8_t *)buf);
        DEBUG_PRINTF((const uint8_t * )"\r\n");

        diag_entry_rdptr++;
        c++;

    }

    /* More entries in diag buffer */
    if(diag_entry_rdptr != diag_entry_wrptr)
        DEBUG_PRINTF((const uint8_t * )"Please repeat command. Additional error entries in DIAG buffer available!\r\n");

}

void DIAG_PrintContactorInfo(void)
{
    /* FIXME if read once, rdptr is on writeptr, therefore in the next call the errors aren't
     * printed again. Maybe tmp save rdptr and set again at end of function. But when is the
     * diag memory cleared then? */

    uint8_t buf[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int32_t tmp = 0;
    DIAG_CONTACTOR_s diagContactor;

    NVM_Get_contactorcnt(&diagContactor);

    DEBUG_PRINTF((const uint8_t * )"Contactor switching entries:");
    DEBUG_PRINTF((const uint8_t * )"\r\n");

    for(uint8_t i = 0; i < BS_NR_OF_CONTACTORS; i++) {
        DEBUG_PRINTF((const uint8_t * )"Contactor ");
        DEBUG_PRINTF(U8ToDecascii(buf, &i, 2));
        DEBUG_PRINTF((const uint8_t * )"\r\nOpening switches: ");
        DEBUG_PRINTF(U16ToHexascii(buf, &diagContactor.cont_switch_opened[i]));
        DEBUG_PRINTF((const uint8_t * )"\r\nClosing switches: ");
        DEBUG_PRINTF(U16ToHexascii(buf, &diagContactor.cont_switch_closed[i]));
        DEBUG_PRINTF((const uint8_t * )"\r\nOpening switches hard at current: ");
        DEBUG_PRINTF(U16ToHexascii(buf, &diagContactor.cont_switch_opened_hard_at_current[i]));
        DEBUG_PRINTF((const uint8_t * )"\r\n\n");
        DEBUG_PRINTF((const uint8_t * )"\r\n");
    }

    DEBUG_PRINTF((const uint8_t * )"\r\n");
    if(diagContactorError_entry_rdptr == diagContactorError_entry_wrptr)
    {
        DEBUG_PRINTF((const uint8_t * )"no entries in Contactor");
        DEBUG_PRINTF((const uint8_t * )"\r\n");
    }
    else
    {
        DEBUG_PRINTF((const uint8_t * )"Contactor error entries:");
        DEBUG_PRINTF((const uint8_t * )"\r\n");
        DEBUG_PRINTF((const uint8_t * )"Date and Time       Contactor     Opening current");
        DEBUG_PRINTF((const uint8_t * )"\r\n");
    }



    while(diagContactorError_entry_rdptr != diagContactorError_entry_wrptr)
    {
        if( diagContactorError_entry_rdptr >= &diagContactorErrorMemory[DIAG_FAIL_ENTRY_CONTACTOR_LENGTH]) {
            diagContactorError_entry_rdptr = &diagContactorErrorMemory[0];
        }

        DEBUG_PRINTF(U8ToDecascii(buf, &diagContactorError_entry_rdptr->DD, 2));
        DEBUG_PRINTF((const uint8_t * )".");
        DEBUG_PRINTF(U8ToDecascii(buf, &diagContactorError_entry_rdptr->MM, 2));
        DEBUG_PRINTF((const uint8_t * )".");
        DEBUG_PRINTF(U8ToDecascii(buf, &diagContactorError_entry_rdptr->YY, 2));
        DEBUG_PRINTF((const uint8_t * )" - ");
        DEBUG_PRINTF(U8ToDecascii(buf, &diagContactorError_entry_rdptr->hh, 2));
        DEBUG_PRINTF((const uint8_t * )":");
        DEBUG_PRINTF(U8ToDecascii(buf, &diagContactorError_entry_rdptr->mm, 2));
        DEBUG_PRINTF((const uint8_t * )":");
        DEBUG_PRINTF(U8ToDecascii(buf, &diagContactorError_entry_rdptr->ss, 2));
        DEBUG_PRINTF((const uint8_t * )"     ");

        DEBUG_PRINTF(U8ToDecascii(buf, (uint8_t*)(&diagContactorError_entry_rdptr->contactor), 2));
        DEBUG_PRINTF((const uint8_t * )"        ");
        tmp = (int32_t)diagContactorError_entry_rdptr->openingCurrent;
        DEBUG_PRINTF(I32ToDecascii(buf, &tmp));
        DEBUG_PRINTF((const uint8_t * )"mA\r\n");

        diagContactorError_entry_rdptr++;
    }

    /* Reset counter for open errors */
    diagContactor.errcntreported = 0;

}


/**
 * @brief DIAG_EntryWrite adds an error entry.
 *
 * This function adds an entry to the error buffer.
 * It provides some functionality to prevent duplicates from being logged.
 * Multiple occurring error doesn't get logged anymore after they reached a
 * pre-defined error count.
 *
 * @param  eventID:   ID of entry
 * @param  event:     OK, NOK or RESET
 * @param  item_nr:   item number of event
 *
 * @return 0xFF if event is logged, otherwise 0
 */
static uint8_t DIAG_EntryWrite(uint8_t eventID, DIAG_EVENT_e event, uint32_t item_nr) {

    uint8_t ret_val = 0;
    uint8_t c;
    RTC_Time_s currTime;
    RTC_Date_s currDate;
    uint8_t buf[25] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; /* max. description length = 24 + 1 to identify end of array */

    if(diag_locked)
        return ret_val;    /* only locked when clearing the diagnosis memory */

    if(diag.entry_event[eventID] == event)
        return ret_val;        /* same event of same error type already recorded before -> ignore until event toggles */
    if((diag.entry_event[eventID] == DIAG_EVENT_OK) && (event ==  DIAG_EVENT_RESET))
        return ret_val;     /* do record DIAG_EVENT_RESET-event only if last event was an error (re-initialization) */
                            /* meaning: DIAG_EVENT_RESET-event at first time call or after DIAG_EVENT_OK-event will not be recorded */

    if(++diag.entry_cnt[eventID] > DIAG_MAX_ENTRIES_OF_ERROR){
        diag.entry_cnt[eventID] = DIAG_MAX_ENTRIES_OF_ERROR;
        return ret_val;        /* this type of error has been recorded too many times -> ignore to avoid filling buffer with same failurecodes */
    }

    if( diag_entry_wrptr >= &diag_memory[DIAG_FAIL_ENTRY_LENGTH] ) {
        diag_entry_wrptr = &diag_memory[0];
    }

    /* now record failurecode */
    ret_val=0xFF;
    RTC_getTime(&currTime);
    RTC_getDate(&currDate);

    diag_entry_wrptr->YY = currDate.Year;
    diag_entry_wrptr->MM = currDate.Month;
    diag_entry_wrptr->DD = currDate.Date;
    diag_entry_wrptr->hh = currTime.Hours;
    diag_entry_wrptr->mm = currTime.Minutes;
    diag_entry_wrptr->ss = currTime.Seconds;

    diag_entry_wrptr->event_id = eventID;        /* Error Code 0... 4x32-1 */
    diag_entry_wrptr->item    = item_nr;               /*  */
    diag_entry_wrptr->event   = (uint8_t)event;        /* DIAG_EVENT_OK, DIAG_EVENT_NOK, DIAG_EVENT_RESET */

    diag_entry_wrptr->Val0 = diag_fc.Val0;
    diag_entry_wrptr->Val1 = diag_fc.Val1;
    diag_entry_wrptr->Val2 = diag_fc.Val2;
    diag_entry_wrptr->Val3 = diag_fc.Val3;
    ++diag_entry_wrptr;

    ++diag.errcntreported;         /* counts of (new) diagnosis entry records which is still not been read by external Tool */
                                   /* which will reset this value to 0 after having read all new entries which means <acknowledged by user> */
    ++diag.errcnttotal;            /* total counts of diagnosis entry records */

    diag.entry_event[eventID] = event;
    DEBUG_PRINTF((const uint8_t * )"New Error entry! (");
    c = (uint8_t) diag.errcntreported;
    DEBUG_PRINTF(U8ToDecascii(buf, &c,3));
    DEBUG_PRINTF((const uint8_t * )"): Error Code/Item ");
    DEBUG_PRINTF(U8ToDecascii(buf, &eventID, 3));
    DEBUG_PRINTF((const uint8_t * )"/0x");
    DEBUG_PRINTF(U32ToHexascii(buf, &item_nr));
    DEBUG_PRINTF((const uint8_t * )" ");

    /* Copy error description  in buffer, maximum description length = 24 characters */
    for(uint8_t i = 0; i < 24; i++)
        buf[i] = diag_devptr->ch_cfg[diag.id2ch[eventID]].description[i];

    DEBUG_PRINTF((const uint8_t *)buf);

    if(event==DIAG_EVENT_OK)
        DEBUG_PRINTF((const uint8_t * )" cleared");
    else if (event==DIAG_EVENT_NOK)
        DEBUG_PRINTF((const uint8_t * )" occurred");
    else /* DIAG_EVENT_RESET */
        DEBUG_PRINTF((const uint8_t * )" reset");

    DEBUG_PRINTF((const uint8_t * )"\r\n");

    return ret_val;
}



DIAG_RETURNTYPE_e DIAG_Handler(DIAG_CH_ID_e diag_ch_id, DIAG_EVENT_e event, uint32_t item_nr, void* data) {

    DIAG_RETURNTYPE_e retVal = DIAG_HANDLER_RETURN_UNKNOWN;

    /* Get diagnosis type */
    DIAG_TYPE_e diagType = diag_dev.ch_cfg[diag.id2ch[diag_ch_id]].type;

    switch(diagType) {
    /* Call handler function depending on diagnosis type */

        case DIAG_GENERAL_TYPE:
            retVal = DIAG_GeneralHandler(diag_ch_id, event, item_nr);
            break;

        case DIAG_CELLMON_TYPE:
            break;

        case DIAG_COM_TYPE:
            break;

        case DIAG_ADC_TYPE:
            break;

        case DIAG_CONT_TYPE:
            /* item_nr is contactor number */
            retVal = DIAG_ContHandler(diag_ch_id, item_nr, (float*) data);
            break;

        default:
            break;

    }

    return retVal;
}


/**
 * @brief DIAG_GeneralHandler provides generic error handling, based on configuration.
 *
 * This function does all the handling based on the user defined configuration.
 * According to its return value further treatment is left to the calling module itself.
 * @param   diag_ch_id: event ID of the event that has occurred
 * @param   event:      event that occurred (OK, NOK, RESET)
 * @param   item_nr:    item nr of event, to distinguish between different calling locations of the event
 *
 * @return   DIAG_STATE_UNINITIALIZED if diag module still uninitialized,\n
 *           DIAG_HANDLER_RETURN_WRONG_ID if invalid diag id,\n
 *           DIAG_HANDLER_INVALID_TYPE if diag id doesn't correspond to diag type,\n
 *           DIAG_HANDLER_RETURN_OK if error/warning occurred but no threshold reached or event = DIAG_EVENT_OK/DIAG_EVENT_RESET,\n
 *           DIAG_HANDLER_RETURN_ERR_OCCURRED if error threshold reached,\n
 *           DIAG_HANDLER_RETURN_WARNING_OCCURRED if warning threshold reached,\n
 */
static DIAG_RETURNTYPE_e DIAG_GeneralHandler(DIAG_CH_ID_e diag_ch_id, DIAG_EVENT_e event, uint32_t item_nr) {
    uint32_t ret_val = DIAG_HANDLER_RETURN_UNKNOWN;
    uint32_t *u32ptr_errCodemsk, *u32ptr_warnCodemsk;
    uint16_t  *u16ptr_threshcounter;
    uint16_t cfg_threshold;
    uint16_t err_enable_idx;
    uint32_t err_enable_bitmask;

    DIAG_TYPE_RECORDING_e recordingenabled;

    if(diag.state == DIAG_STATE_UNINITIALIZED) {
        return (DIAG_HANDLER_RETURN_NOT_READY);
    }

    if(diag_ch_id >= DIAG_ID_MAX) {
        return (DIAG_HANDLER_RETURN_WRONG_ID);
    }

    if((diag_ch_id == DIAG_CH_CONTACTOR_DAMAGED) || (diag_ch_id == DIAG_CH_CONTACTOR_OPENING) ||
            (diag_ch_id == DIAG_CH_CONTACTOR_CLOSING)) {
        return (DIAG_HANDLER_INVALID_TYPE);
    }
    err_enable_idx      = diag_ch_id/32;        /* array index of diag.err_enableflag[..] */
    err_enable_bitmask  = 1<<(diag_ch_id%32);   /* bit number (mask) of diag.err_enableflag[idx] */


    u32ptr_errCodemsk   = &diag.errflag[err_enable_idx];
    u32ptr_warnCodemsk  = &diag.warnflag[err_enable_idx];
    u16ptr_threshcounter = &diag.occurrence_cnt[diag_ch_id];
    cfg_threshold       = diag_devptr->ch_cfg[diag.id2ch[diag_ch_id]].thresholds;
    recordingenabled    = diag_devptr->ch_cfg[diag.id2ch[diag_ch_id]].enablerecording;

    if(event == DIAG_EVENT_OK)
    {
        if(diag.err_enableflag[err_enable_idx] & err_enable_bitmask)
        {
            /* if (((*u16ptr_threshcounter) == 0) && (*u32ptr_errCodemsk == 0)) */
            if (((*u16ptr_threshcounter) == 0))
            {
                ;   /* everything ok, nothing to be handled */
            }
            else if ((*u16ptr_threshcounter) > 1 )
            {
                (*u16ptr_threshcounter)--;   /* Error did not occur, decrement Error-Counter */
            }
            /* else if ((*u16ptr_threshcounter) <= 1) */
            else if ((*u16ptr_threshcounter) == 1)
            {   /*  Error did not occur, now decrement to zero and clear Error- or Warning-Flag and make recording if enabled */
                *u32ptr_errCodemsk &= ~err_enable_bitmask;      /* ERROR:   clear corresponding bit in errflag[idx] */
                *u32ptr_warnCodemsk &= ~err_enable_bitmask;     /* WARNING: clear corresponding bit in warnflag[idx] */
                (*u16ptr_threshcounter) = 0;
                /* Make entry in error-memory (error disappeared) */
                if(recordingenabled == DIAG_RECORDING_ENABLED)
                    DIAG_EntryWrite(diag_ch_id, event, item_nr);

                /* Call callback function and reset error */
                diag_ch_cfg[diag.id2ch[diag_ch_id]].callbackfunc(diag_ch_id, DIAG_EVENT_RESET);
            }
        }
        ret_val = DIAG_HANDLER_RETURN_OK; /* Function does not return an error-message! */
    }
    else if(event == DIAG_EVENT_NOK)
    {
        if( diag.err_enableflag[err_enable_idx] & err_enable_bitmask )
        {
            if ((*u16ptr_threshcounter) < cfg_threshold )
            {
                (*u16ptr_threshcounter)++;   /* error-threshold not exceeded yet, increment Error-Counter */
                ret_val = DIAG_HANDLER_RETURN_OK; /* Function does not return an error-message! */
            }
            else if ((*u16ptr_threshcounter) == cfg_threshold )
            {
                /* Error occured AND error-threshold exceeded */
                (*u16ptr_threshcounter)++;
                *u32ptr_errCodemsk |= err_enable_bitmask;      /* ERROR:   set corresponding bit in errflag[idx] */
                *u32ptr_warnCodemsk &= ~err_enable_bitmask;    /* WARNING: clear corresponding bit in warnflag[idx] */

                /* Make entry in error-memory (error occurred) */
                if(recordingenabled == DIAG_RECORDING_ENABLED)
                    DIAG_EntryWrite(diag_ch_id, event, item_nr);

                /* Call callback function and set error */
                diag_ch_cfg[diag.id2ch[diag_ch_id]].callbackfunc(diag_ch_id, DIAG_EVENT_NOK);
                /* Function returns an error-message! */
                ret_val = DIAG_HANDLER_RETURN_ERR_OCCURRED;
            }
            else if (((*u16ptr_threshcounter) > cfg_threshold))
            {
                ;   /* error-threshold already exceeded, nothing to be handled */
            }
        }
        else
        {
            /* Error occured BUT NOT enabled by mask */
            *u32ptr_errCodemsk &= ~err_enable_bitmask;        /* ERROR:   clear corresponding bit in errflag[idx] */
            *u32ptr_warnCodemsk |= err_enable_bitmask;        /* WARNING: set corresponding bit in warnflag[idx] */
            ret_val = DIAG_HANDLER_RETURN_WARNING_OCCURRED;   /* Function returns an error-message! */
        }
    }
    else if(event == DIAG_EVENT_RESET)
    {
        if(diag.err_enableflag[err_enable_idx] & err_enable_bitmask)
        {    /* clear counter, Error-, Warning-Flag and make recording if enabled */
            *u32ptr_errCodemsk &= ~err_enable_bitmask;      /* ERROR:   clear corresponding bit in errflag[idx] */
            *u32ptr_warnCodemsk &= ~err_enable_bitmask;     /* WARNING: clear corresponding bit in warnflag[idx] */
            (*u16ptr_threshcounter) = 0;
            if(recordingenabled==DIAG_RECORDING_ENABLED)
                DIAG_EntryWrite(diag_ch_id, event, item_nr);      /* Make entry in error-memory (error disappeared) if error was recorded before */
        }
        ret_val = DIAG_HANDLER_RETURN_OK; /* Function does not return an error-message! */
    }


    return (ret_val);
}


/**
 * @brief DIAG_ContHandler provides generic contactor switching handling, based on configuration.
 *
 * This function does all the handling based on the user defined configuration.
 * It needs to get called in every occurrence where a contactor is either opened or closed.
 * According to its return value further treatment is left to the calling module itself.
 *
 * @param eventID     switching event that occurred
 * @param cont_nr     contactor that switches
 * @param openingCur  current flow during contactor opening
 *
 * @return  DIAG_HANDLER_RETURN_ERR_OCCURRED if hard opening threshold is reached,\n
 *          DIAG_HANDLER_RETURN_OK if normal opening/closing of contactor occurred or threshold not reached,\n
 *          DIAG_HANDLER_INVALID_TYPE if called with wrong eventID,\n
 *          DIAG_HANDLER_INVALID_DATA if no opening current is passed in case of hard opening
 */
static DIAG_RETURNTYPE_e DIAG_ContHandler(DIAG_CH_ID_e eventID, uint32_t cont_nr, float* openingCur) {

    DIAG_RETURNTYPE_e retVal = DIAG_HANDLER_RETURN_UNKNOWN;
    uint8_t updateNVRAM = 0;

    if(diag_locked)
        return retVal;    /*  only locked when clearing the diagnosis memory */
    DIAG_CONTACTOR_s diagContactor;
    NVM_Get_contactorcnt(&diagContactor);
    if(eventID == DIAG_CH_CONTACTOR_OPENING) {
        diagContactor.cont_switch_opened[cont_nr]++;
        retVal = DIAG_HANDLER_RETURN_OK;
    }
    else if(eventID == DIAG_CH_CONTACTOR_CLOSING) {
        diagContactor.cont_switch_closed[cont_nr]++;
        retVal = DIAG_HANDLER_RETURN_OK;
    }
    else if(eventID == DIAG_CH_CONTACTOR_DAMAGED) {
        if (NULL_PTR == openingCur) {
            retVal = DIAG_HANDLER_INVALID_DATA;
        }
        else {
            RTC_Time_s currTime;
            RTC_Date_s currDate;
            uint8_t buf[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

            updateNVRAM = 1;

            diagContactor.cont_switch_opened_hard_at_current[cont_nr]++;

            if(diagContactor.cont_switch_opened_hard_at_current[cont_nr] >= CONT_NUMBER_OF_BAD_COUNTINGS)
                retVal = DIAG_HANDLER_RETURN_ERR_OCCURRED;
            else
                retVal = DIAG_HANDLER_RETURN_OK;

            if( diagContactorError_entry_wrptr >= &diagContactorErrorMemory[DIAG_FAIL_ENTRY_CONTACTOR_LENGTH]) {
                diagContactorError_entry_wrptr = &diagContactorErrorMemory[0];
            }

            /* Write error entry */

            /* Get time and date */
            RTC_getTime(&currTime);
            RTC_getDate(&currDate);

            /* Set time and date */
            diagContactorError_entry_wrptr->YY = currDate.Year;
            diagContactorError_entry_wrptr->MM = currDate.Month;
            diagContactorError_entry_wrptr->DD = currDate.Date;
            diagContactorError_entry_wrptr->hh = currTime.Hours;
            diagContactorError_entry_wrptr->mm = currTime.Minutes;
            diagContactorError_entry_wrptr->ss = currTime.Seconds;

            diagContactorError_entry_wrptr->contactor = (uint8_t)cont_nr;
            diagContactorError_entry_wrptr->openingCurrent =  *openingCur;

            diagContactorError_entry_wrptr++;

            DEBUG_PRINTF((const uint8_t * )"new Contactor error entry! currently ");
            uint8_t tmp = (uint8_t)diagContactor.errcntreported;
            DEBUG_PRINTF(U8ToDecascii(buf, &tmp, 2));
            DEBUG_PRINTF((const uint8_t * )" error entrys");
            DEBUG_PRINTF((const uint8_t * )"\r\n");
        }
    }
    else {
        retVal = DIAG_HANDLER_INVALID_TYPE;
    }
    if ((DIAG_HANDLER_RETURN_ERR_OCCURRED == retVal) || (DIAG_HANDLER_RETURN_OK == retVal)) {
        /* Write new value in nvram buffer */
        NVM_Set_contactorcnt(&diagContactor);
        if (updateNVRAM == 1) {
            /* Update NVRAM only if contactor opened hard at curren */
            NVRAM_setWriteRequest(NVRAM_BLOCK_ID_CONT_COUNTER);
        }
    }
    return retVal;
}


/**
 * @brief overall system monitoring
 *
 * checks notifications (state and timestamps) of all system-relevant tasks or functions
 * all checks should be customized corresponding to its timing and state requirements
 */
void DIAG_SysMon(void) {
    DIAG_SYSMON_MODULE_ID_e module_id;
    uint32_t localTimer = OS_getOSSysTick();
    if (diagsysmonTimestamp == localTimer) {
        return;
    }
    diagsysmonTimestamp = localTimer;

    /* check modules */
    for (module_id = 0; module_id < DIAG_SYSMON_MODULE_ID_MAX; module_id++) {

        if ((diag_sysmon_ch_cfg[module_id].type == DIAG_SYSMON_CYCLICTASK) &&
           (diag_sysmon_ch_cfg[module_id].state == DIAG_ENABLED)) {

            if(diag_sysmon[module_id].timestamp -  diag_sysmon_last[module_id].timestamp <  1) {
                /* module not running */
                if(++diag_sysmon_cnt[module_id] >= diag_sysmon_ch_cfg[module_id].threshold) {
                    /* @todo configurable timeouts ! */
                    if(diag_sysmon_ch_cfg[module_id].enablerecording == DIAG_RECORDING_ENABLED) {
                        DIAG_Handler(DIAG_CH_SYSTEMMONITORING_TIMEOUT,DIAG_EVENT_NOK,module_id, NULL);
                    }
#if BUILD_MODULE_ENABLE_CONTACTOR == 1
                    if(diag_sysmon_ch_cfg[module_id].handlingtype == DIAG_SYSMON_HANDLING_SWITCHOFFCONTACTOR) {
                        /* system not working trustfully, switch off contactors! */
                        CONT_SwitchAllContactorsOff();
                    }
#endif
                    diag_sysmon_cnt[module_id] = 0;

                    /* @todo: call callback function if error occurred */
                    diag_sysmon_ch_cfg[module_id].callbackfunc(module_id);
                }
            } else {
                /*  module running */
                diag_sysmon_cnt[module_id] = 0;

                if(diag_sysmon[module_id].state != 0) {
                    /* check state of module */
                    /* @todo: do something now! */
                }
            }
        } else {
            /* if Sysmon type != cyclic task (not used at the moment) */
        }
        diag_sysmon_last[module_id] = diag_sysmon[module_id];      /*save last values for next check*/
    }
}


void DIAG_SysMonNotify(DIAG_SYSMON_MODULE_ID_e module_id, uint32_t state) {

    if (module_id < DIAG_SYSMON_MODULE_ID_MAX) {

        taskENTER_CRITICAL();
        diag_sysmon[module_id].timestamp = OS_getOSSysTick();
        diag_sysmon[module_id].state = state;
        taskEXIT_CRITICAL();
    }
}


void DIAG_configASSERT(void) {

#ifdef STM32F4
    uint32_t lr_register;
    uint32_t sp_register;

    __ASM volatile ("mov %0, r14" : "=r" (lr_register) );
    __ASM volatile ("mov %0, r13" : "=r" (sp_register) );

    lr_register = lr_register & 0xFFFFFFFE;     /* mask out LSB as this only is indicates thumb instruction */
    diag_fc.Val0 = sp_register;                 /* actual stack pointer */
    diag_fc.Val1 = lr_register;                 /* report instruction address where this function has been called */
    diag_fc.Val2 = *(uint32_t*)(sp_register + 0x1C);        /* return address of callers context (one above caller) */
    DIAG_Handler(DIAG_CH_CONFIGASSERT,DIAG_EVENT_NOK,0, NULL);
#endif

    while (1) {
        ;
    }
}
