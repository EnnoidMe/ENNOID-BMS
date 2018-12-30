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
 * @file    diag_id_cfg.h
 * @author  foxBMS Team
 * @date    13.11.2015 (date of creation)
 * @ingroup ENGINE
 * @prefix  DIAG
 *
 * @brief   This file contains basic definitions in order to use an abstracted diagnosis
 */

#ifndef DIAG_ID_CFG_H_
#define DIAG_ID_CFG_H_

/*================== Includes =============================================*/

/*================== Macros and Definitions ===============================*/
/**
 * diagnosis check result (event)
 */
typedef enum {
    DIAG_EVENT_OK       = 0x00, /*!< diag channel event OK */
    DIAG_EVENT_NOK      = 0x01, /*!< diag channel event NOK */
    DIAG_EVENT_RESET    = 0x02, /*!< reset diag channel eventcounter to 0 */
} DIAG_EVENT_e;

/**
 * symbolic IDs for possible diagnosis entries
 */
typedef enum {
    DIAG_ID_0       = 0,
    DIAG_ID_1       = 1,
    DIAG_ID_2       = 2,
    DIAG_ID_3       = 3,
    DIAG_ID_4       = 4,
    DIAG_ID_5       = 5,
    DIAG_ID_6       = 6,
    DIAG_ID_7       = 7,
    DIAG_ID_8       = 8,
    DIAG_ID_9       = 9,
    DIAG_ID_10      = 10,
    DIAG_ID_11      = 11,
    DIAG_ID_12      = 12,
    DIAG_ID_13      = 13,
    DIAG_ID_14      = 14,
    DIAG_ID_15      = 15,

    DIAG_ID_16      = 16,
    DIAG_ID_17      = 17,
    DIAG_ID_18      = 18,
    DIAG_ID_19      = 19,
    DIAG_ID_20      = 20,
    DIAG_ID_21      = 21,
    DIAG_ID_22      = 22,
    DIAG_ID_23      = 23,
    DIAG_ID_24      = 24,
    DIAG_ID_25      = 25,
    DIAG_ID_26      = 26,
    DIAG_ID_27      = 27,
    DIAG_ID_28      = 28,
    DIAG_ID_29      = 29,
    DIAG_ID_30      = 30,
    DIAG_ID_31      = 31,

    DIAG_ID_32      = 32,
    DIAG_ID_33      = 33,
    DIAG_ID_34      = 34,
    DIAG_ID_35      = 35,
    DIAG_ID_36      = 36,
    DIAG_ID_37      = 37,
    DIAG_ID_38      = 38,
    DIAG_ID_39      = 39,
    DIAG_ID_40      = 40,
    DIAG_ID_41      = 41,
    DIAG_ID_42      = 42,
    DIAG_ID_43      = 43,
    DIAG_ID_44      = 44,
    DIAG_ID_45      = 45,
    DIAG_ID_46      = 46,
    DIAG_ID_47      = 47,

    DIAG_ID_48      = 48,
    DIAG_ID_49      = 49,
    DIAG_ID_50      = 50,
    DIAG_ID_51      = 51,
    DIAG_ID_52      = 52,
    DIAG_ID_53      = 53,
    DIAG_ID_54      = 54,
    DIAG_ID_55      = 55,
    DIAG_ID_56      = 56,
    DIAG_ID_57      = 57,
    DIAG_ID_58      = 58,
    DIAG_ID_59      = 59,
    DIAG_ID_60      = 60,
    DIAG_ID_61      = 61,
    DIAG_ID_62      = 62,
    DIAG_ID_63      = 63,

    DIAG_ID_64      = 64,
    DIAG_ID_65      = 65,
    DIAG_ID_66      = 66,
    DIAG_ID_67      = 67,
    DIAG_ID_68      = 68,
    DIAG_ID_69      = 69,
    DIAG_ID_70      = 70,
    DIAG_ID_71      = 71,
    DIAG_ID_72      = 72,
    DIAG_ID_73      = 73,
    DIAG_ID_74      = 74,
    DIAG_ID_75      = 75,
    DIAG_ID_76      = 76,
    DIAG_ID_77      = 77,
    DIAG_ID_78      = 78,
    DIAG_ID_79      = 79,

    DIAG_ID_80      = 80,
    DIAG_ID_81      = 81,
    DIAG_ID_82      = 82,
    DIAG_ID_83      = 83,
    DIAG_ID_84      = 84,
    DIAG_ID_85      = 85,
    DIAG_ID_86      = 86,
    DIAG_ID_87      = 87,
    DIAG_ID_88      = 88,
    DIAG_ID_89      = 89,
    DIAG_ID_90      = 90,
    DIAG_ID_91      = 91,
    DIAG_ID_92      = 92,
    DIAG_ID_93      = 93,
    DIAG_ID_94      = 94,
    DIAG_ID_95      = 95,

    DIAG_ID_96      = 96,
    DIAG_ID_97      = 97,
    DIAG_ID_98      = 98,
    DIAG_ID_99      = 99,
    DIAG_ID_100     = 100,
    DIAG_ID_101     = 101,
    DIAG_ID_102     = 102,
    DIAG_ID_103     = 103,
    DIAG_ID_104     = 104,
    DIAG_ID_105     = 105,
    DIAG_ID_106     = 106,
    DIAG_ID_107     = 107,
    DIAG_ID_108     = 108,
    DIAG_ID_109     = 109,
    DIAG_ID_110     = 110,
    DIAG_ID_111     = 111,

    DIAG_ID_MAX     = 112,

} DIAG_CH_ID_e;

/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

#endif /* DIAG_ID_CFG_H_ */
