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
 * @file    nvramhandler_cfg.h
 * @author  foxBMS Team
 * @date    17.09.2018 (date of creation)
 * @ingroup ENGINE_CONF
 * @prefix  NVRAM
 *
 * @brief   Datahandler configuration header
 *
 * Provides interfaces to datahandler configuration
 *
 */

#ifndef NVRAMHANDLER_CFG_H_
#define NVRAMHANDLER_CFG_H_

/*================== Includes =============================================*/
#include "general.h"

#include "nvram_cfg.h"

/*================== Macros and Definitions ===============================*/
typedef enum NVRAM_UpdateType {
    NVRAM_Cyclic,
    NVRAM_Triggered,
} NVRAM_UpdateType_e;

typedef enum NVRAM_state {
    NVRAM_wait    = 0,
    NVRAM_write   = 1,
    NVRAM_read    = 2,
} NVRAM_state_e;

typedef struct NVRAM_BLOCK {
    NVRAM_state_e state;                /*!< state of datahandler block          */
    uint32_t lastUpdate;                /*!< time of last nvram update           */
    NVRAM_UpdateType_e mode;            /*!< update mode (cyclic or triggered)   */
    uint32_t updateCycleTime_ms;        /*!< cycle time of algorithm             */
    uint32_t phase_ms;                  /*!< start time when executing algorithm */
    STD_RETURN_TYPE_e (*funcRD)(void);  /*!< read callback function              */
    STD_RETURN_TYPE_e (*funcWR)(void);  /*!< write callback function             */
} NVRAM_BLOCK_s;

/**
 * @brief data block identification number
 */
typedef enum {
    NVRAM_BLOCK_00 =  0,
    NVRAM_BLOCK_01 =  1,
    NVRAM_BLOCK_02 =  2,
    NVRAM_BLOCK_03 =  3,
    NVRAM_BLOCK_04 =  4,
    NVRAM_BLOCK_05 =  5,
    NVRAM_BLOCK_06 =  6,
    NVRAM_BLOCK_07 =  7,
    NVRAM_BLOCK_08 =  8,
    NVRAM_BLOCK_09 =  9,
    NVRAM_BLOCK_10 = 10,
    NVRAM_BLOCK_11 = 11,
    NVRAM_BLOCK_12 = 12,
    NVRAM_BLOCK_13 = 13,
    NVRAM_BLOCK_14 = 14,
    NVRAM_BLOCK_15 = 15,
    NVRAM_BLOCK_16 = 16,
} NVRAM_BLOCK_ID_TYPE_e;

/*================== Macros and Definitions [USER CONFIGURATION] =============*/
#define NVRAM_BLOCK_ID_OPERATING_HOURS         NVRAM_BLOCK_00
#define NVRAM_BLOCK_ID_CELLTEMPERATURE         NVRAM_BLOCK_01
#define NVRAM_BLOCK_ID_CONT_COUNTER            NVRAM_BLOCK_02

/*================== Constant and Variable Definitions ====================*/
/*
 * Array with pointer to the different algorithms
 */
extern NVRAM_BLOCK_s nvram_dataHandlerBlocks[];

/**
 * number of nvram datablocks
 */
extern const uint16_t nvram_number_of_blocks;

/*================== Function Prototypes ==================================*/

/** Interface functions writing to/ reading from non-volatile memory (NVRAM) */

/**
 * @brief   saves operating hours data into the non-volatile memory (NVM)
 *
 * @return  E_OK if successful, otherwise E_NOT_OK
 */
extern STD_RETURN_TYPE_e NVM_operatingHoursUpdateNVRAM(void);

/**
 * @brief   reads operating hours data from the non-volatile and writes to the volatile memory (RAM)
 *
 * @return  E_OK if successful, otherwise E_NOT_OK
 */
extern STD_RETURN_TYPE_e NVM_operatingHoursUpdateRAM(void);

/**
 * @brief   saves SOC data into the non-volatile memory (NVM)
 *
 * @return  E_OK if successful, otherwise E_NOT_OK
 */
extern STD_RETURN_TYPE_e NVM_socUpdateNVRAM(void);

/**
 * @brief   reads SOC data from the non-volatile and writes to the volatile memory (RAM)
 *
 * @return  E_OK if successful, otherwise E_NOT_OK
 */
extern STD_RETURN_TYPE_e NVM_socUpdateRAM(void);

/**
 * @brief   saves contactor counting data into the non-volatile memory (NVM)
 *
 * @return  E_OK if successful, otherwise E_NOT_OK
 */
extern STD_RETURN_TYPE_e NVM_contactorcountUpdateNVRAM(void);

/**
 * @brief   reads contactor counting data from the non-volatile and writes to the volatile memory (RAM)
 *
 * @return  E_OK if successful, otherwise E_NOT_OK
 */
extern STD_RETURN_TYPE_e NVM_contactorcountUpdateRAM(void);


/** Interface functions writting to/ reading from volatile memory (RAM/BKPSRAM) */

/**
 * @brief  Gets the operating hours from the volatile memory (RAM)
 *
 * @param  ptr pointer to destination buffer where the stored operating hours are copied
 * @return STD_RETURN_TYPE_e
*/
extern STD_RETURN_TYPE_e NVM_getOperatingHours(NVRAM_OPERATING_HOURS_s *dest_ptr);

/**
 * @brief  Increments the volatile memory (RAM)
 *
*/
extern STD_RETURN_TYPE_e NVM_setOperatingHours(NVRAM_OPERATING_HOURS_s *timer);

/**
 * @brief  Gets the number of opening/closing events of the contactors saved in the non-volatile RAM
 *
 * @param  ptr pointer where the contactor event data should be stored to
 *
 * @return E_OK if successful, otherwise E_NOT_OK
*/
extern STD_RETURN_TYPE_e NVM_Get_contactorcnt(DIAG_CONTACTOR_s *ptr);

/**
 * @brief  Sets the number of opening/closing events of the contactors saved in the non-volatile RAM
 *
 * @param  ptr pointer where the contactor events are stored
 *
 * @return E_OK if successful, otherwise E_NOT_OK
*/
extern STD_RETURN_TYPE_e NVM_Set_contactorcnt(DIAG_CONTACTOR_s *ptr);

/**
 * @brief  Gets the SOC data saved in the non-volatile RAM
 *
 * @return E_OK if successful, otherwise E_NOT_OK
*/
extern STD_RETURN_TYPE_e NVM_getSOC(SOX_SOC_s *dest_ptr);

/**
 * @brief  Sets the SOC data saved in the non-volatile RAM
 *
 * @param  ptr pointer where the soc data is stored
 *
 * @return E_OK if successful, otherwise E_NOT_OK
*/
extern STD_RETURN_TYPE_e NVM_setSOC(SOX_SOC_s* ptr);

/*================== Function Implementations =============================*/

#endif /* NVRAMHANDLER_CFG_H_ */
