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
 * @file    misc.h
 * @author  foxBMS Team
 * @date    23.12.2015 (date of creation)
 * @ingroup UTIL
 * @prefix  none
 *
 * @brief   Headers for the miscellaneous functions
 *
 */

#ifndef MISC_H_
#define MISC_H_

/*================== Includes =============================================*/
#include "general.h"

/*================== Macros and Definitions ===============================*/


/*================== Constant and Variable Definitions ====================*/


/*================== Function Prototypes ==================================*/
extern uint8_t *U8ToDecascii(uint8_t *destptr, uint8_t *dataptr, uint8_t minDigits);
extern uint8_t *hex2ascii(uint8_t *destptr, uint8_t *srcptr, uint8_t len);
extern uint8_t *U32ToHexascii(uint8_t *destptr, uint32_t *dataptr);
extern uint8_t *U16ToHexascii(uint8_t *destptr, uint16_t *dataptr);
extern uint8_t *U8ToHexascii(uint8_t *destptr, uint8_t *dataptr);
extern uint8_t *I32ToDecascii(uint8_t *destptr, int32_t *dataptr);
extern uint8_t *U16ToDecascii(uint8_t *destptr, uint16_t *dataptr, uint8_t minDigits);

/**
 * @brief   Converts one ASCII number to its corresponding integer value
 *
 * @param   number between 0-9 in ASCII format
 *
 * @return  uint8_t integer value
 */
extern uint8_t AsciiNumberToU8(uint8_t asciiChar);

/*================== Function Implementations =============================*/

#endif /* MISC_H_ */
