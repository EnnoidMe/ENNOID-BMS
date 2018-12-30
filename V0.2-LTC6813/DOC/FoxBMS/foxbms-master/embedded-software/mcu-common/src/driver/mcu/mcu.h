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
 * @file    mcu.h
 * @author  foxBMS Team
 * @date    22.11.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  MCU
 *
 * @brief   Headers for the functions for the MCU to manage time and interrupts
 *
 * Header file provides interfaces to set boot information, to disable/enable all interrupts,
 * to wait a designated time and and to get the a timestamp based on the os systick.
 *
 */

#ifndef MCU_H_
#define MCU_H_

/*================== Includes =============================================*/
#include "general.h"

/*================== Macros and Definitions ===============================*/
typedef struct {
    uint32_t off0;
    uint32_t off32;
    uint32_t off64;
    uint32_t crc;
} MCU_DeviceID_s;

/*================== Constant and Variable Definitions ====================*/
extern MCU_DeviceID_s mcu_unique_deviceID;

/*================== Function Prototypes ==================================*/

/**
 * @brief   simple wait-loop to wait short periods of time
 *
 * @param   time    number of microseconds to wait, max. 1000us
 */
extern void MCU_Wait_us(uint32_t time);

/**
 * @brief   disables interrupts and returns prior state (enabled or disabled)
 *
 * this function is used for synchronization of real-time interrupts when bypassing FreeRTOS context and
 * could be used (called) from 'interrupt blocked' or 'interrupt unblocked' context
 *
 * @return  current priority mask register, store this value and use it to enable interrupts again with CPU_RestoreINT()
 */
extern unsigned int MCU_DisableINT(void);

/**
 * @brief   restores interrupts to given state (enable or disable)
 *
 * this function is used for synchronization of real-time interrupts when bypassing FreeRTOS context
 *
 * @param   primask_reg value for priority mask register, for use in case of nesting calling
 */
extern void MCU_RestoreINT(unsigned int primask_reg);

/**
 * @brief   Gets system reset status
 *
 * @return  0: no unexpected reset detected, otherwise unexpected reset detected
 */
extern uint32_t MCU_SystemResetStatus(uint32_t* regValue);

/**
 * @brief   gets systick value which is a 24-bit downcounter, auto refreshed with 71999
 *
 * @return  time    current systick value
 */
extern uint32_t MCU_GetTimeBase(void);

/**
 * @brief   Get unique device ID
 */
extern void MCU_GetDeviceID(MCU_DeviceID_s * deviceID);

/**
 * @brief   Check if passed crc == unique device ID crc
 */
STD_RETURN_TYPE_e MCU_CheckCRCUniqueDeviceID(uint8_t *dataptr);

/*================== Function Implementations =============================*/
#endif /* MCU_H_ */
