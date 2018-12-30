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
 * @file    nvramhandler.c
 * @author  foxBMS Team
 * @date    17.09.2018 (date of creation)
 * @ingroup ENGINE
 * @prefix  NVRAM
 *
 * @brief   datahandler implementation to update nvram
 *
 */

/*================== Includes ===============================================*/
#include "nvramhandler.h"

#include "os.h"

/*================== Macros and Definitions =================================*/

/*================== Static Constant and Variable Definitions ===============*/

/*================== Extern Constant and Variable Definitions ===============*/

/*================== Static Function Prototypes =============================*/

/*================== Static Function Implementations ========================*/

/*================== Extern Function Implementations ========================*/

void NVRAM_dataHandlerInit(void) {

    /* Reset last update time */
    for (uint8_t i = 0; i < nvram_number_of_blocks; i++) {
        nvram_dataHandlerBlocks[i].lastUpdate = 0;
    }
}


void NVRAM_dataHandler(void) {

    static float timestamp = 0;
    STD_RETURN_TYPE_e retval = E_NOT_OK;

    /* Get new OS timestamp */
    timestamp = (float) OS_getOSSysTick();

    for (uint8_t i = 0; i < nvram_number_of_blocks; i++) {

        /* Check if cyclic nvram block needs to be updated */
        if (nvram_dataHandlerBlocks[i].mode == NVRAM_Cyclic) {
            /* If passed time > update cycle time: -> update channel OR additional asynchronous request has been made */
            if ((timestamp - (nvram_dataHandlerBlocks[i].lastUpdate + nvram_dataHandlerBlocks[i].phase_ms)) >
            nvram_dataHandlerBlocks[i].updateCycleTime_ms ||
            nvram_dataHandlerBlocks[i].state == NVRAM_write) {
                /* Update nvram block */

                if (nvram_dataHandlerBlocks[i].funcWR != NULL_PTR) {
                    retval = nvram_dataHandlerBlocks[i].funcWR();

                    if (retval == E_OK) {
                        /* Write request successful: set to wait state again */
                        nvram_dataHandlerBlocks[i].lastUpdate = timestamp;
                        nvram_dataHandlerBlocks[i].state = NVRAM_wait;
                    } else {
                        /* Try again next NVRAM_dataHandler() call */
                        ;
                    }
                } else {
                    /* Invalid pointer access -> do nothing */
                    nvram_dataHandlerBlocks[i].state = NVRAM_wait;
                }
            }
        } else if (nvram_dataHandlerBlocks[i].mode == NVRAM_Triggered) {
            /* Check if triggered nvram block needs to be updated */
            if (nvram_dataHandlerBlocks[i].state == NVRAM_write) {

                /* Update nvram block */
                if (nvram_dataHandlerBlocks[i].funcWR != NULL_PTR) {
                    retval = nvram_dataHandlerBlocks[i].funcWR();
                    if (retval == E_OK) {
                        /* Write request successful: set to wait state again */
                        nvram_dataHandlerBlocks[i].lastUpdate = timestamp;
                        nvram_dataHandlerBlocks[i].state = NVRAM_wait;
                    } else {
                        /* Try again next NVRAM_dataHandler() call */
                        ;
                    }
                } else {
                    /* Invalid pointer access -> do nothing */
                    nvram_dataHandlerBlocks[i].state = NVRAM_wait;
                }
            }
        }
        else if (nvram_dataHandlerBlocks[i].state == NVRAM_read) {
            /* Check if read is requested */

            /* Read nvram block */
            if (nvram_dataHandlerBlocks[i].funcRD != NULL_PTR) {
                retval = nvram_dataHandlerBlocks[i].funcRD();
                if (retval == E_OK) {
                    /* Read request successful: set to wait state again */
                    nvram_dataHandlerBlocks[i].state = NVRAM_wait;
                } else {
                    /* Try again next NVRAM_dataHandler() call */
                    ;
                }
            } else {
                /* Invalid pointer access -> do nothing */
                nvram_dataHandlerBlocks[i].state = NVRAM_wait;
            }
        }
    }
}


void NVRAM_setWriteRequest(NVRAM_BLOCK_ID_TYPE_e blockID) {
    if (blockID < nvram_number_of_blocks) {
        nvram_dataHandlerBlocks[blockID].state = NVRAM_write;
    }
}

void NVRAM_setReadRequest(NVRAM_BLOCK_ID_TYPE_e blockID) {
    if (blockID < nvram_number_of_blocks) {
        nvram_dataHandlerBlocks[blockID].state = NVRAM_read;
    }
}
