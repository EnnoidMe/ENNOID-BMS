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
 * @file    misc.c
 * @author  foxBMS Team
 * @date    23.12.2015 (date of creation)
 * @ingroup UTIL
 * @prefix  none
 *
 * @brief   Miscellaneous functions
 *
 *
 */

/*================== Includes =============================================*/
#include "misc.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

/**
 * @brief reverse is a helper function which reverses strings.
 *
 * This function reverses the character order of a string.
 *
 * @return (type: uint8_t)
 */

void reverseString(uint8_t str[], uint8_t length) {
    uint8_t start = 0;
    uint8_t end = length-1;
    while(start < end) {
        uint8_t a = *(str+start);
        uint8_t b = *(str+end);
        *(str+end) = a;
        *(str+start) = b;
        start++;
        end--;
    }
}

/**
 * @brief unsignedIntegerToString is a helper function which converts unsigned Integers to ASCII.
 *
 * This function does some basic conversion stuff to be able to print out numbers/values as their corresponding ASCII.
 *
 * @return (type: uint8_t)
 */

uint8_t* unsignedIntegerToString(uint8_t *destptr, uint32_t *dataptr, uint8_t minDigits, uint8_t base) {
    uint32_t num = *dataptr;
    uint8_t* str = destptr;

    if(minDigits  < 1) {
        minDigits = 1;
    }

    int i = 0;
    /* Process individual digits */
    while (num != 0)
    {
        int rem = num % base;
        if(rem > 9) {
            str[i++] = (rem-10) + 'a';  /* we are in hex */
        } else {
            str[i++] =  rem + '0';
        }
        num = num/base;
    }

    while(i < minDigits) {
       str[i++] = '0';
    }

    str[i] = '\0';

    reverseString(str, i);

    return str;
}

/**
 * @brief integerToString is a helper function which converts signed Integers to ASCII.
 *
 * This function does some basic conversion stuff to be able to print out numbers/values as their corresponding ASCII.
 *
 * @return (type: uint8_t)
 */

uint8_t* integerToString(uint8_t *destptr, int32_t *dataptr, uint8_t minDigits, uint8_t base) {
    uint32_t num = *dataptr;
    uint8_t* str = destptr;
    uint8_t negative = 0;

    if(minDigits  < 1) {
        minDigits = 1;
    }
    if(num < 0) {
        negative = 1;
        num = -num;
    }

    int i = 0;
    /* Process individual digits */
    while (num != 0)
    {
        int rem = num % base;
        if(rem > 9) {
            str[i++] = (rem-10) + 'a';  /* we are in hex */
        } else {
            str[i++] =  rem + '0';
        }
        num = num/base;
    }

    while(i < minDigits) {
       str[i++] = '0';
    }
    if(negative) {
        str[i++] = '-';
    }

    str[i] = '\0';

    reverseString(str, i);

    return str;
}

/**
 * @brief U16ToDecascii is a helper function which converts U16 to ASCII.
 *
 * This function does some basic conversion stuff to be able to print out numbers/values as their corresponding ASCII.
 *
 * @return (type: uint8_t)
 */

uint8_t *U16ToDecascii(uint8_t *destptr, uint16_t *dataptr, uint8_t minDigits)
{
    uint32_t tmp = *dataptr;
    return unsignedIntegerToString(destptr, &tmp, minDigits, 10);
}



/**
 * @brief U8ToDecascii is a helper function which converts U8 to ASCII.
 *
 * This function does some basic conversion stuff to be able to print out numbers/values as their corresponding ASCII.
 *
 * @return (type: uint8_t)
 */

uint8_t *U8ToDecascii(uint8_t *destptr, uint8_t *dataptr, uint8_t minDigits)
{
    uint32_t tmp = *dataptr;
    return unsignedIntegerToString(destptr, &tmp, minDigits, 10);
}

/**
 * @brief hex2ascii is a helper function which converts hex input to ASCII.
 *
 * This function does some basic conversion stuff to be able to print out numbers/values as their corresponding ASCII.
 *
 * @return (type: uint8_t)
 */

uint8_t *hex2ascii(uint8_t *destptr, uint8_t *srcptr, uint8_t len)
{
    uint32_t tmp = *srcptr;
    return unsignedIntegerToString(destptr, &tmp, len, 16);
}

/**
 * @brief U32ToHexascii is a helper function which converts U32 to hex.
 *
 * This function is just a wrapper for hex2ascii.
 *
 * @return (type: uint8_t)
 */

uint8_t *U32ToHexascii(uint8_t *destptr, uint32_t *dataptr)
{
   /*  return(hex2ascii(destptr,(uint8_t*)(dataptr), 4)); */
    uint32_t tmp = *dataptr;
    return unsignedIntegerToString(destptr, &tmp, 8, 16);
}

/**
 * @brief U16ToHexascii is a helper function which converts U16 to hex.
 *
 * This function is just a wrapper for hex2ascii.
 *
 * @return (type: uint8_t)
 */

uint8_t *U16ToHexascii(uint8_t *destptr, uint16_t *dataptr)
{
    uint32_t tmp = *dataptr;
    /* return(hex2ascii(destptr,(uint8_t*)(dataptr), 2)); */
    return unsignedIntegerToString(destptr, &tmp, 4, 16);
}


/**
 * @brief U8ToHexascii is a helper function which converts U16 to hex.
 *
 * This function is just a wrapper for hex2ascii.
 *
 * @return (type: uint8_t)
 */

uint8_t *U8ToHexascii(uint8_t *destptr, uint8_t *dataptr)
{
    uint32_t tmp = *dataptr;
   /*  return(hex2ascii(destptr,(uint8_t*)(dataptr), 1)); */
    return unsignedIntegerToString(destptr, &tmp, 2, 16);
}


/**
 * @brief I32ToDecascii is a helper function which converts I32 to ASCII.
 *
 * This function does some basic conversion stuff to be able to print out numbers/values as their corresponding ASCII.
 *
 * Required buffer: at least uint8_t[12]
 *
 * @return (type: uint8_t)
 */
uint8_t *I32ToDecascii(uint8_t *destptr, int32_t *dataptr) {

    int32_t tmp = *dataptr;
    return integerToString(destptr, &tmp, 1, 10);
}


uint8_t AsciiNumberToU8(uint8_t asciiChar) {

    return (asciiChar - '0');
}
