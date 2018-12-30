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
 * @file    database.c
 * @author  foxBMS Team
 * @date    18.08.2015 (date of creation)
 * @ingroup ENGINE
 * @prefix  DATA
 *
 * @brief   Database module implementation
 *
 * Implementation of database module
 */

/*================== Includes =============================================*/
#include "database.h"

#include "diag.h"
#include <string.h>

/*================== Macros and Definitions ===============================*/
/**
 * Maximum queue timeout time in milliseconds
 */
#define DATA_QUEUE_TIMEOUT_MS   10

/*================== Constant and Variable Definitions ====================*/
/* FIXME Some uninitialized variables */
static DATA_BLOCK_ACCESS_s data_block_access[DATA_MAX_BLOCK_NR];
static SemaphoreHandle_t data_base_mutex[DATA_MAX_BLOCK_NR];
QueueHandle_t data_queue;

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

/*================== Public functions =====================================*/
void DATA_Init(void) {
    if(sizeof(data_base_dev) == 0) {
        /* todo fatal error! */

        while (1) {
            ;
        }
    }

    /* FIXME What is mymessage structures? */
    /* Create a queue capable of containing 10 pointers to mymessage structures.
       These should be passed by pointer as they contain a lot of data.
       data_queueID = xQueueCreate( devptr->nr_of_blockheader, sizeof( DATA_QUEUE_MESSAGE_s) ); */    /* FIXME number of queues=1 should be enough */

/*     if(data_queueID  ==  NULL_PTR) {
             Failed to create the queue
           ;   @ TODO Error Handling
       }
*/

    /* Iterate over database and set respective read/write pointer for each database entry */
    for(uint16_t i = 0; i < data_base_dev.nr_of_blockheader; i++) {

        /* Set write pointer to database entry */
        data_block_access[i].WRptr = (void*)*(uint32_t*)(data_base_dev.blockheaderptr + i);

        /* Set read pointer */
        if((data_base_dev.blockheaderptr + i)->buffertype  ==  DOUBLE_BUFFERING) {

            /* If database entry is double buffered -> set read pointer to
             * second section. After writing first database entry, pointer are
             * swapped and read pointer then points to written entry. */
            data_block_access[i].RDptr = (void*)((uint32_t)data_block_access[i].WRptr + (data_base_dev.blockheaderptr + i)->datalength);

        } else {
            /* Single buffering -> read = write pointer */
            data_block_access[i].RDptr = data_block_access[i].WRptr;
        }
        /* Create a mutex for each database entry */
        data_base_mutex[i] = xSemaphoreCreateMutex();
    }

    /* Create queue to transfer data to/from database */

    /* Create a queue capable of containing a pointer of type DATA_QUEUE_MESSAGE_s
    Data of Messages are passed by pointer as they contain a lot of data. */
    data_queue = xQueueCreate( 1, sizeof( DATA_QUEUE_MESSAGE_s) );

    if (data_queue == NULL_PTR) {
        /* Failed to create the queue */
        /* @ TODO Error Handling */
        while (1) {
            ;
        }
    }
}


void DB_WriteBlock(void *dataptrfromSender, DATA_BLOCK_ID_TYPE_e  blockID) {

    /* dataptrfromSender is a pointer to data of caller function
       dataptr_toptr_fromSender is a pointer to this pointer
       this is used for passing message variable by reference
       note: xQueueSend() always takes message variable by value */
    DATA_QUEUE_MESSAGE_s data_send_msg;
    TickType_t queuetimeout;

    if( vPortCheckCriticalSection() ) {
        configASSERT(0);
    }

    queuetimeout = DATA_QUEUE_TIMEOUT_MS / portTICK_RATE_MS;
    if (queuetimeout == 0) {
        queuetimeout = 1;
    }

    /* prepare send message with attributes of data block */
    data_send_msg.blockID = blockID;
    data_send_msg.value.voidptr = dataptrfromSender;
    data_send_msg.accesstype = WRITE_ACCESS;
    /* Send a pointer to a message object and
       maximum block time: queuetimeout */
    xQueueSend( data_queue, (void *) &data_send_msg, queuetimeout);
 }


void DATA_Task(void) {
    DATA_QUEUE_MESSAGE_s receive_msg;
    void *srcdataptr;
    void *dstdataptr;
    DATA_BLOCK_ID_TYPE_e blockID;
    DATA_BLOCK_ACCESS_TYPE_e    accesstype; /* read or write access type */
    uint16_t datalength;
    DATA_BLOCK_BUFFER_TYPE_e buffertype;

    if(data_queue != NULL_PTR) {

        if( xQueueReceive(data_queue,(&receive_msg),(TickType_t ) 1) ) {  /* scan queue and wait for a message up to a maximum amount of 1ms (block time) */

            /* ptrrcvmessage now points to message of sender which contains data pointer and data block ID */
            blockID = receive_msg.blockID;
            srcdataptr = receive_msg.value.voidptr;
            accesstype = receive_msg.accesstype;
            if((blockID < DATA_MAX_BLOCK_NR) && (srcdataptr != NULL_PTR)) {   /* plausibility check */
                /* get entries of blockheader and write pointer */
                if(accesstype == WRITE_ACCESS) {
                    /* write access to data blocks */
                    datalength = (data_base_dev.blockheaderptr + blockID)->datalength;
                    buffertype = (data_base_dev.blockheaderptr + blockID)->buffertype;
                    dstdataptr=data_block_access[blockID].WRptr;

                    /* Check if there any read accesses taking place (in tasks with lower priorities)*/
                    if(xSemaphoreTake(data_base_mutex[blockID], 0)  ==  TRUE) {
                        uint32_t *previousTimestampptr = NULL_PTR;
                        uint32_t *timestampptr = NULL_PTR;

                        /* Set timestamp pointer */
                        timestampptr = (uint32_t *)srcdataptr;
                        /* Set previous timestampptr */
                        previousTimestampptr = (uint32_t *)srcdataptr;
                        previousTimestampptr++;

                        /* Write previous timestamp */
                        *previousTimestampptr = *timestampptr;
                        /* Write timestamp */
                        *(uint32_t *)srcdataptr = OS_getOSSysTick();

                        memcpy(dstdataptr, srcdataptr, datalength);
                        xSemaphoreGive(data_base_mutex[blockID]);
                        if(buffertype  ==  DOUBLE_BUFFERING) {
                            /* swap the WR and RD pointers:
                               WRptr always points to buffer to be written next time and changed afterwards
                               RDptr always points to buffer to be read next time */
                            data_block_access[blockID].WRptr=data_block_access[blockID].RDptr;
                            data_block_access[blockID].RDptr=dstdataptr;
                        }
                    } else {
                        dstdataptr=data_block_access[blockID].WRptr;
                    }
                } else if(accesstype == READ_ACCESS) {
                    /* Read access to data blocks */
                    datalength = (data_base_dev.blockheaderptr + blockID)->datalength;
                    buffertype = (data_base_dev.blockheaderptr + blockID)->buffertype;
                    dstdataptr = srcdataptr;

                    if(buffertype  ==  DOUBLE_BUFFERING) {

                        if(xSemaphoreTake(data_base_mutex[blockID], 0)  ==  TRUE) {

                            srcdataptr = data_block_access[blockID].RDptr;
                            if(srcdataptr != NULL_PTR) {

                                memcpy(dstdataptr, srcdataptr, datalength);
                                xSemaphoreGive(data_base_mutex[blockID]);
                            }
                        }
                    } else if(buffertype  ==  SINGLE_BUFFERING) {
                            srcdataptr = data_block_access[blockID].RDptr;
                            if(srcdataptr != NULL_PTR) {
                                memcpy(dstdataptr, srcdataptr, datalength);
                            }
                    }
                } else {
                    ;
                }
            }
        }
        DIAG_SysMonNotify(DIAG_SYSMON_DATABASE_ID, 0);        /* task is running, state = ok */
    }
}


STD_RETURN_TYPE_e DB_ReadBlock(void *dataptrtoReceiver, DATA_BLOCK_ID_TYPE_e  blockID) {

    DATA_QUEUE_MESSAGE_s data_send_msg;
    TickType_t queuetimeout;

    if(vPortCheckCriticalSection()) {
        configASSERT(0);
    }

    queuetimeout = DATA_QUEUE_TIMEOUT_MS / portTICK_RATE_MS;
    if (queuetimeout  ==  0) {
        queuetimeout = 1;
    }

    /* prepare send message with attributes of data block */
    data_send_msg.blockID = blockID;
    data_send_msg.value.voidptr = dataptrtoReceiver;
    data_send_msg.accesstype = READ_ACCESS;

    /* Send a pointer to a message object and */
    /* maximum block time: queuetimeout */
    xQueueSend( data_queue, (void *) &data_send_msg, queuetimeout);

    return E_OK;
}

/* FIXME not used  currently - delete? */
void * DATA_GetTablePtrBeginCritical(DATA_BLOCK_ID_TYPE_e  blockID) {
    /* FIXME block with semaphore */
    return data_block_access[blockID].RDptr;
}

/*================== Static functions =====================================*/
