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
 * @file    chksum.c
 * @author  foxBMS Team
 * @date    28.08.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  CHK
 *
 * @brief   Driver for the checksum calculation
 *
 * This module provides some checksum functionality using either
 * modulo32bitaddition as fast and easy algorithm or hardware supported
 * crc32.
 *

 *
 */



/*================== Includes =============================================*/
#include "chksum.h"

#include "cpu_cfg.h"

/*================== Macros and Definitions ===============================*/


/*================== Constant and Variable Definitions ====================*/
CHK_STATUS_s chk_status;

/*================== Function Prototypes ==================================*/
uint32_t CHK_modulo32addition(uint8_t* data, uint32_t len);

/*================== Function Implementations =============================*/

/**
 * @brief CHK_Flashchecksum provides validation for the currently used firmware.
 *
 * This function gets called during startup and validates the currently used firmware
 * by doing a checksum over pre-defined memory areas and compares result with a pre-calculated
 * checksum stored in the flashheader
 *
 * @return (type: uint8_t)
 */
STD_RETURN_TYPE_e CHK_Flashchecksum(const VER_ValidStruct_s *valid_struct) {
    STD_RETURN_TYPE_e retVal=E_NOT_OK;
    uint32_t length = valid_struct->endaddress - valid_struct->startaddress + 1;
    uint32_t start = valid_struct->startaddress;
    uint32_t cs = CHK_crc32((uint8_t*)start, length);
    if(cs == valid_struct->Checksum_u32)
        retVal=E_OK;
    else
        retVal=E_NOT_OK;

    return retVal;
}

/**
 * @brief CHK_modulo32addition provides modulo32bitaddition algorithm.
 *
 * This function provides some basic checksum functionality using module32bitaddition algorithm.
 * It has no hardware dependencies.
 *
 * @return (type: uint32_t)
 */

uint32_t CHK_modulo32addition(uint8_t* data, uint32_t len) {
       uint32_t *pBuffer = (uint32_t*) data;
       uint32_t BufferLength = len / 4;
       uint32_t chksum = 0;

       while (BufferLength > 0) {
             chksum += *pBuffer;
             pBuffer++;
             BufferLength--;
       }

       return chksum;
}


uint32_t CHK_crc32(uint8_t* data, uint32_t len) {

    uint32_t* pBuffer = (uint32_t*) data;
    uint32_t BufferLength = len/4;
    uint32_t index = 0;
    __HAL_RCC_CRC_CLK_ENABLE();
    CRC->CR = CRC_CR_RESET;

    for(index = 0; index < BufferLength; index++)
    {
        CRC->DR = __RBIT(pBuffer[index]);
    }

    return __RBIT(CRC->DR) ^ 0xFFFFFFFF;
}
