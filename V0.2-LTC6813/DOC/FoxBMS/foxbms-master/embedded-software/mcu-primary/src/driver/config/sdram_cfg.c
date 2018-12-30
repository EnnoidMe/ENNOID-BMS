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
 * @file    sdram_cfg.c
 * @author  foxBMS Team
 * @date    24.05.2016 (date of creation)
 * @ingroup DRIVERS_CONF
 * @prefix  extsd
 *
 * @brief   Configuration and Data Definitons of external SDRAM
 *
 */


/*================== Includes =============================================*/
#include "sdram_cfg.h"
#include "cpu_cfg.h"

#ifdef HAL_SDRAM_MODULE_ENABLED
/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/
#if 0
uint32_t extsd_testbuffer[100]={0x10,0xf,0xe,0xd,0xc,0xb,0xa,0x9,0x8,7,6,5,4,3,2,1,0xaa};
volatile uint32_t extsd_test2;
#endif
/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

void SDRAM_c_init(void)
{
    uint32_t *extRAMptr = (uint32_t*)(&_s_extdata[0]);       /* start address of external SDRAM  .data-section */
    uint32_t *flashptr  = (uint32_t*)(&_sidata_ext[0]);      /* start address of Flash related to .data-section */

    while( extRAMptr < (uint32_t*)(&_e_extdata[0]) )         /* compare to end address of external SDRAM  .data-section */
    {
       *extRAMptr++ =  *flashptr++;
    }
}
#if 0
void SDRAM_testram(void)
{
    uint32_t i=0;
    for(i=0;i<(sizeof(extsd_testbuffer)/sizeof(uint32_t));i++)
    {
        extsd_testbuffer[i]=i;
    }
    extsd_test2++;
}
#endif


#endif
