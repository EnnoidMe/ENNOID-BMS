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
 * @file    mcu.c
 * @author  foxBMS Team
 * @date    22.11.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  MCU
 *
 * @brief   Functions for the MCU to manage time and interrupts
 *
 * Source file implements the interfaces to set boot information, to disable/enable all interrupts,
 * to wait a designated time and and to get the a timestamp based on the os systick.
 */

/*================== Includes =============================================*/
#include "mcu.h"

#include "cpu_cfg.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/
MCU_DeviceID_s mcu_unique_deviceID;

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

/*================== Public functions =====================================*/
void MCU_Wait_us(uint32_t time) {
    uint32_t start = (SysTick->VAL);
    uint32_t t_end = start;
    int32_t dt = 0;

    if (time > 1000) {
        time = 1000;
    }

    dt = start - (time * 72);

    if (dt > 0) {
        t_end = dt;

        while (1) {
            uint32_t act_time = (uint32_t) (SysTick->VAL);
            if (act_time <= t_end) {
                break;
            }
        }
    }
    else {
        t_end = 71999 + (dt);

        while (1) {
            uint32_t act_time = (uint32_t) (SysTick->VAL);
            if (act_time < t_end && act_time > start) {
                break;
            }
        }
    }
}

unsigned int MCU_DisableINT(void) {
    unsigned int primask_reg;
    primask_reg = __get_PRIMASK();
    __disable_irq();        /* set primask register to disable global interrupts */
    /* CPSID instruction is self synchronizing, thus instruction-sync __ISB() not needed */
    return (primask_reg);
}

void MCU_RestoreINT(unsigned int primask_reg) {
    if((primask_reg & 0x01) == 0) {
        /* enable global interrupts*/
        __DMB();               /* do not execute any instructions with memory access when prior memory accesses not finished */
        __enable_irq();        /* CPSIE instruction is not self synchronizing, */
    }
    /* else ... restore of disabled interrupts throughout enabled state not supported */
}

uint32_t MCU_GetTimeBase(void) {
     uint32_t time = SysTick->VAL;   /* Current Value of SysTick Base Register, Downcounter 24-bit, Startvalue 71999 */
     return (time);
}

uint32_t MCU_SystemResetStatus(uint32_t* regValue) {
    uint32_t errCode = 0;
    uint32_t csr;
    csr = RCC->CSR;

    if(csr & RCC_CSR_LPWRRSTF) {
        /* Low-power management(!) reset flag, note: this is not low-power reset! */
/*         DIAG_Handler(DIAG_CH_WATCHDOGRESET_FAILURE,DIAG_EVENT_NOK,0);   actually not supported by software */
    }
    if (csr & RCC_CSR_WWDGRSTF) {
        /* Window watchdog reset */
        errCode |= STD_ERR_BIT_0;  /*  unexcepted watchdog reset occured */
    }
    if(csr & RCC_CSR_WDGRSTF) {
       /* Independent watchdog reset flag */
        errCode |= STD_ERR_BIT_1;  /* unexcepted watchdog reset occured */
    }
    if(csr & RCC_CSR_SFTRSTF) {     /* actually not evaluated by software */
       /* Software reset flag */
        /* check if sw reset requested */
        /* DIAG_Handler(DIAG_CH_....,DIAG_EVENT_NOK,0);   unexcepted software reset occured */
    }
    if(csr & RCC_CSR_PORRSTF) {     /* actually not evaluated by software */
       /* POR/PDR reset flag */
        /* check if POR reset requested */
        /* DIAG_Handler(DIAG_CH_....,DIAG_EVENT_NOK,0);   unexcepted ... */
    }
    if(csr & RCC_CSR_PADRSTF) {     /* actually not evaluated by software */
       /* PIN reset flag (NRST) */
       /* DIAG_Handler(DIAG_CH_....,DIAG_EVENT_NOK,0);   unexcepted ... */
    }
    if(csr & RCC_CSR_BORRSTF) {     /* actually not evaluated by software */
       /* BOR reset flag (POR/PDR or BOR) */
        /* DIAG_Handler(DIAG_CH_....,DIAG_EVENT_NOK,0);   unexcepted ... */
    }
    RCC->CSR = RCC_CSR_RMVF;        /* Clear all (sticky) reset flags */

    *regValue = csr;
    return errCode;
}


void MCU_GetDeviceID(MCU_DeviceID_s * deviceID) {

    /* Base address of device ID register  0x1FFF 7A10 */
    deviceID->off0 = (*(volatile uint32_t *)(0x1FFF7A10 + 0x0));
    deviceID->off32 = (*(volatile uint32_t *)(0x1FFF7A10 + 0x4));
    deviceID->off64 = (*(volatile uint32_t *)(0x1FFF7A10 + 0x8));

    return;
}


STD_RETURN_TYPE_e MCU_ChecCRCkUniqueDeviceID(uint8_t *dataptr) {
    STD_RETURN_TYPE_e ret_val;

    if (*(uint32_t*)(dataptr) == mcu_unique_deviceID.crc)
        ret_val = E_OK;
    else
        ret_val = E_NOT_OK;

    return ret_val;

}

/*================== Static functions =====================================*/
