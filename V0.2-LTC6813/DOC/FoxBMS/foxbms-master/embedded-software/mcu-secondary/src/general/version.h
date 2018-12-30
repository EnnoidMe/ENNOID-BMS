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
 * @file    version.h
 * @author  foxBMS Team
 * @date    21.09.2015 (date of creation)
 * @ingroup GENERAL
 * @prefix  VER
 *
 * @brief   Functions for setting a version number, headers
 *
 */

#ifndef VERSION_H_
#define VERSION_H_

/*================== Includes =============================================*/
#include "general.h"

/*================== Macros and Definitions ===============================*/
extern uint32_t _sflash1[];
extern uint32_t _eflash1[];
extern uint32_t _jumpadressvectable[];

/**
 * @brief   Software Version Number
 *
 * Use Case:
 * uint8_t  Version[16] example:     "V0.4.0 "   ( 15 char + '\0' )
 */
#define VER_SW_VERSION              BUILD_VERSION_SECONDARY

/**
 * @brief   Software Project Name
 * Use Case:
 * uint8_t  Version[16] example:     " foxBMS"   ( 15 char + '\0' )
 */
#define VER_PROJECT                 BUILD_APPNAME_SECONDARY

/**
 * @brief   Application Valid Marker in Flash header
 *
 */
#define VER_FLASH_APPL_VALID_MARKER     0x1122334455667788

/**
 * @brief   Application Invalid Marker in Flash header
 *
 */
#define VER_FLASH_APPL_INVALID_MARKER    0x8877665544332211

/**
 * @brief   Application Not Invalid Marker in Flash header (Default Value of erased Flash)
 *
 */
#define VER_FLASH_APPL_NOT_INVALID_MARKER     0xFFFFFFFFFFFFFFFF


typedef enum {
  VER_BUILDVARIANT_UNKNOWN      = 0,
  VER_BUILDVARIANT_LABOR        = 1,
  VER_BUILDVARIANT_TESTBENCH    = 2,
} VER_BUILDVARIANT_e;


/**
 * Adress-Range of Checksum-Test (SW-Startup) will depend on following data which
 * is place at the end of the flash memory (see SECTION FLASH_HEADER)
 * */
typedef struct
{
/*0x00*/    uint64_t Valid_u64;                 /* not used (Application-Validation marker) */
/*0x08*/    uint64_t Invalid_u64;               /* not used (Application-Invalidation marker)*/
/*0x10*/    uint32_t Checksum_u32;              /* checksum for validating the Application SW */
/*0x14*/    uint32_t dummy_14;
/*0x18*/    uint8_t  Version[16];               /*Version: "V0.4.0"*/
/*0x28*/    uint8_t  Project[16];               /*Project: "foxBMS"*/
/*0x38*/    VER_BUILDVARIANT_e  BuildVariant;   /*BuildVariant*/
/*0x39*/    uint8_t  dummy39[3];
/*0x3C*/    uint8_t  enableFlashChecksum;
/*0x3D*/    uint8_t  dummy_3D;
/*0x3E*/    uint16_t dummy_3E;
/*0x40*/    uint32_t dummy_40;
/*0x44*/    uint32_t dummy_44;       /* number of memory ranges for checksum calculation, addresses in the following */
/*0x48*/    uint32_t startaddress;  /* the address at which Checksum-Test starts (Start-Address of Application SW)  */
/*0x4C*/    uint32_t endaddress;    /* end address of first memory range */
/*0x50*/    uint32_t jumpadressvectable;
            uint32_t dummy_00b;
/*0x58*/    uint32_t dummy_01a;
            uint32_t dummy_01b;
/*0x60*/    uint32_t dummy_02a;
            uint32_t dummy_02b;
/*0x68*/    uint32_t dummy_03a;
            uint32_t dummy_03b;
/*0x70*/    uint32_t dummy_04a;
            uint32_t dummy_04b;
/*0x78*/    uint32_t dummy_05a;
            uint32_t dummy_05b;
/*0x80*/    uint32_t dummy_06a;
            uint32_t dummy_06b;
/*0x88*/    uint32_t dummy_07a;
            uint32_t dummy_07b;
/*0x90*/    uint32_t dummy_08a;
            uint32_t dummy_08b;
/*0x98*/    uint32_t dummy_09a;
            uint32_t dummy_09b;
/*0xA0*/    uint8_t date[12];
/*0xAC*/    uint8_t time[12];
/*0xB8*/    uint8_t dummy_u64[0x100 - 0xB8];
} VER_ValidStruct_s;



/*================== Constant and Variable Definitions ====================*/
extern const VER_ValidStruct_s ver_sw_validation;


/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

#endif /* VERSION_H_ */
