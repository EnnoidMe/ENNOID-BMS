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
 * @file    nvramhandler_cfg.c
 * @author  foxBMS Team
 * @date    17.09.2018 (date of creation)
 * @ingroup ENGINE_CONF
 * @prefix  NVRAM
 *
 * @brief   Datahandler configuration
 *
 * Configuration of datahandler module
 *
 */

/*================== Includes =============================================*/
#include "nvramhandler_cfg.h"

#include "eepr.h"
#include "mcu.h"
#include "rtc.h"


/*================== Macros and Definitions ===============================*/


/*================== Constant and Variable Definitions ====================*/
#ifdef BKP_SRAM_ENABLE
NVRAM_CH_NVSOC_s MEM_BKP_SRAM bkpsram_nvsoc;
NVRRAM_CH_CONT_COUNT_s MEM_BKP_SRAM bkpsram_contactors_count;
NVRAM_CH_OP_HOURS_s MEM_BKP_SRAM bkpsram_operating_hours;
NVRAM_OPERATING_HOURS_s MEM_BKP_SRAM bkpsram_op_hours;
#else
NVRAM_CH_NVSOC_s bkpsram_nvsoc;
NVRRAM_CH_CONT_COUNT_s bkpsram_contactors_count;
NVRAM_CH_OP_HOURS_s bkpsram_operating_hours;
NVRAM_OPERATING_HOURS_s bkpsram_op_hours;
#endif

NVRAM_BLOCK_s nvram_dataHandlerBlocks[] = {
        { NVRAM_wait, 0, NVRAM_Cyclic, 30000, 100, &NVM_operatingHoursUpdateRAM, &NVM_operatingHoursUpdateNVRAM },
        { NVRAM_wait, 0, NVRAM_Cyclic, 60000, 1000, &NVM_socUpdateRAM, &NVM_socUpdateNVRAM },
        { NVRAM_wait, 0, NVRAM_Triggered, 0, 0, &NVM_contactorcountUpdateRAM, &NVM_contactorcountUpdateNVRAM },
};

const uint16_t nvram_number_of_blocks = sizeof(nvram_dataHandlerBlocks)/sizeof(nvram_dataHandlerBlocks[0]);

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

STD_RETURN_TYPE_e NVM_setSOC(SOX_SOC_s* ptr) {

    STD_RETURN_TYPE_e retval = E_OK;

    if (ptr != NULL_PTR) {
        uint32_t interrupt_status = 0;

        /* Disable interrupts */
        interrupt_status = MCU_DisableINT();

        bkpsram_nvsoc.data = *ptr;
        bkpsram_nvsoc.previous_timestamp = bkpsram_nvsoc.timestamp;
        bkpsram_nvsoc.timestamp = RTC_getUnixTime();
        /* calculate checksum*/
        bkpsram_nvsoc.checksum = EEPR_CalcChecksum((uint8_t*)&bkpsram_nvsoc, sizeof(bkpsram_nvsoc) - 4);

        /* Enable interrupts */
        MCU_RestoreINT(interrupt_status);
    } else {
        retval = E_NOT_OK;
    }

    return retval;
}


STD_RETURN_TYPE_e NVM_getSOC(SOX_SOC_s *dest_ptr) {
    STD_RETURN_TYPE_e ret_val;

    if(EEPR_CalcChecksum((uint8_t*)&bkpsram_nvsoc, sizeof(bkpsram_nvsoc)-4) == bkpsram_nvsoc.checksum){
        /* data valid */
        *dest_ptr = bkpsram_nvsoc.data;
        ret_val = E_OK;
    } else {
        ret_val = E_NOT_OK;
    }
    return ret_val;
}


STD_RETURN_TYPE_e NVM_Set_contactorcnt(DIAG_CONTACTOR_s *ptr) {

    STD_RETURN_TYPE_e retval = E_OK;

    if (ptr != NULL_PTR) {
        uint32_t interrupt_status = 0;

        /* Disable interrupts */
        interrupt_status = MCU_DisableINT();

        /* update bkpsram values */
        /* @FIXME: check pointer (nullpointer) */
        bkpsram_contactors_count.data = *ptr;

        bkpsram_contactors_count.previous_timestamp = bkpsram_contactors_count.timestamp;
        bkpsram_contactors_count.timestamp = RTC_getUnixTime();
        /* calculate checksum*/
        bkpsram_contactors_count.checksum = EEPR_CalcChecksum((uint8_t*)&bkpsram_contactors_count, sizeof(bkpsram_contactors_count)-4);

        /* Enable interrupts */
        MCU_RestoreINT(interrupt_status);
    } else {
        retval = E_NOT_OK;
    }

    return retval;
}


STD_RETURN_TYPE_e NVM_Get_contactorcnt(DIAG_CONTACTOR_s *dest_ptr) {
    STD_RETURN_TYPE_e retval;

    if(EEPR_CalcChecksum((uint8_t*)&bkpsram_contactors_count, sizeof(bkpsram_contactors_count)-4) == bkpsram_contactors_count.checksum){
        /* data valid */
        /* @FIXME: check pointer (nullpointer) */
        *dest_ptr = bkpsram_contactors_count.data;
        retval = E_OK;
    } else {
        /* data invalid */
        retval = E_NOT_OK;
    }
    return retval;

}

STD_RETURN_TYPE_e NVM_setOperatingHours(NVRAM_OPERATING_HOURS_s *timer) {

    STD_RETURN_TYPE_e retval = E_OK;

    if (timer != NULL_PTR) {
        if(++timer->Timer_1ms > 9 ) {
            /* 10ms */
            timer->Timer_1ms = 0;

            if(++timer->Timer_10ms > 9) {
            /* 100ms */
                timer->Timer_10ms = 0;

                if(++timer->Timer_100ms > 9) {
                /* 1s */
                    timer->Timer_100ms = 0;

                    if(++timer->Timer_sec > 59) {
                    /* 1min */
                        timer->Timer_sec = 0;

                        if(++timer->Timer_min > 59) {
                        /* 1h */
                            timer->Timer_min=0;

                            if(++timer->Timer_h > 23) {
                            /* 1d */
                                timer->Timer_h=0;
                                ++timer->Timer_d;
                            }
                        }
                    }
                }
            }
        }
    } else {
        retval = E_NOT_OK;
    }
    return retval;
}


STD_RETURN_TYPE_e NVM_getOperatingHours(NVRAM_OPERATING_HOURS_s *dest_ptr) {
    STD_RETURN_TYPE_e ret_val;

    if(EEPR_CalcChecksum((uint8_t*)&bkpsram_operating_hours, sizeof(bkpsram_operating_hours)-4) == bkpsram_operating_hours.checksum){
        /* data valid */
        /* @FIXME: check pointer (nullpointer) */
        *dest_ptr = bkpsram_operating_hours.data;
        ret_val = E_OK;
    } else {
        /* data invalid */
        ret_val = E_NOT_OK;
    }
    return ret_val;
}


STD_RETURN_TYPE_e NVM_operatingHoursUpdateNVRAM(void) {

    STD_RETURN_TYPE_e retval = E_OK;

    uint32_t interrupt_status = 0;

    /* timestamp calculation done here because trigger function is called
     * in engine 1ms task. To reduce cpuload only calculate timestamps and
     * checksum before writing to NVRAM
     */
    /* Disable interrupts */
    interrupt_status = MCU_DisableINT();

    /* Set data value */
    bkpsram_operating_hours.data = bkpsram_op_hours;

    /* Set timestamp bkpsram values */
    bkpsram_operating_hours.previous_timestamp = bkpsram_operating_hours.timestamp;
    bkpsram_operating_hours.timestamp = RTC_getUnixTime();

    /* calculate checksum*/
    bkpsram_operating_hours.checksum = EEPR_CalcChecksum((uint8_t *)(&bkpsram_operating_hours),sizeof(bkpsram_operating_hours) - 4);

    /* Enable interrupts */
    MCU_RestoreINT(interrupt_status);

    EEPR_SetChDirtyFlag(EEPR_CH_OPERATING_HOURS);

    return retval;
}


STD_RETURN_TYPE_e NVM_operatingHoursUpdateRAM(void) {
    STD_RETURN_TYPE_e retval = E_OK;
    EEPR_SetChReadReqFlag(EEPR_CH_OPERATING_HOURS);
    return retval;
}


STD_RETURN_TYPE_e NVM_socUpdateNVRAM(void) {
    STD_RETURN_TYPE_e retval = E_OK;
    EEPR_SetChDirtyFlag(EEPR_CH_NVSOC);
    return retval;
}


STD_RETURN_TYPE_e NVM_socUpdateRAM(void) {
    STD_RETURN_TYPE_e retval = E_OK;
    EEPR_SetChReadReqFlag(EEPR_CH_NVSOC);
    return retval;
}


STD_RETURN_TYPE_e NVM_contactorcountUpdateNVRAM(void) {
    STD_RETURN_TYPE_e retval = E_OK;
    EEPR_SetChDirtyFlag(EEPR_CH_CONTACTOR);
    return retval;
}


STD_RETURN_TYPE_e NVM_contactorcountUpdateRAM(void) {
    STD_RETURN_TYPE_e retval = E_OK;
    EEPR_SetChReadReqFlag(EEPR_CH_CONTACTOR);
    return retval;
}
