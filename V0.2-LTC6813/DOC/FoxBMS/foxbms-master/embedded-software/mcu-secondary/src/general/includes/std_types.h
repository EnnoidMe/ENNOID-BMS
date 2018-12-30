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
 * @file    std_types.h
 * @author  foxBMS Team
 * @date    20.12.2015 (date of creation)
 * @ingroup GENERAL_CONF
 * @prefix  STD
 *
 * @brief   Definition of foxBMS standard types
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_

/*================== Includes =============================================*/

/*================== Macros and Definitions ===============================*/

#define NULL_PTR (void*)(0)

#define TRUE    1
#define FALSE   0

/*================== Constant and Variable Definitions ====================*/

typedef uint8_t boolean;

typedef void (*VOID_FUNC_VOID_t)();

/**
 * enum for standard return type
 */
typedef enum {
    E_OK        = 0,    /*!< ok     */
    E_NOT_OK    = 1     /*!< not ok */
} STD_RETURN_TYPE_e;


/**
 * enum for error bits
 */
typedef enum {
    STD_ERR_BIT_0  = 0x00000001,
    STD_ERR_BIT_1  = 0x00000002,
    STD_ERR_BIT_2  = 0x00000004,
    STD_ERR_BIT_3  = 0x00000008,
    STD_ERR_BIT_4  = 0x00000010,
    STD_ERR_BIT_5  = 0x00000020,
    STD_ERR_BIT_6  = 0x00000040,
    STD_ERR_BIT_7  = 0x00000080,
    STD_ERR_BIT_8  = 0x00000100,
    STD_ERR_BIT_9  = 0x00000200,
    STD_ERR_BIT_10 = 0x00000400,
    STD_ERR_BIT_11 = 0x00000800,
    STD_ERR_BIT_12 = 0x00001000,
    STD_ERR_BIT_13 = 0x00002000,
    STD_ERR_BIT_14 = 0x00004000,
    STD_ERR_BIT_15 = 0x00008000,
    STD_ERR_BIT_16 = 0x00010000,
    STD_ERR_BIT_17 = 0x00020000,
    STD_ERR_BIT_18 = 0x00040000,
    STD_ERR_BIT_19 = 0x00080000,
    STD_ERR_BIT_20 = 0x00100000,
    STD_ERR_BIT_21 = 0x00200000,
    STD_ERR_BIT_22 = 0x00400000,
    STD_ERR_BIT_23 = 0x00800000,
    STD_ERR_BIT_24 = 0x01000000,
    STD_ERR_BIT_25 = 0x02000000,
    STD_ERR_BIT_26 = 0x04000000,
    STD_ERR_BIT_27 = 0x08000000,
    STD_ERR_BIT_28 = 0x10000000,
    STD_ERR_BIT_29 = 0x20000000,
    STD_ERR_BIT_30 = 0x40000000,
    STD_ERR_BIT_31 = 0x80000000,
} STD_ERROR_BIT_e;

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

#endif /* STD_TYPES_H_ */
