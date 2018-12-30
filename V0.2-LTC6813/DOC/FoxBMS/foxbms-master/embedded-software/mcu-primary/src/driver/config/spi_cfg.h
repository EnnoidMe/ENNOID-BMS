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
 * @file    spi_cfg.h
 * @author  foxBMS Team
 * @date    02.10.2015 (date of creation)
 * @ingroup DRIVERS_CONF
 * @prefix  SPI
 *
 * @brief   Headers for the configuration for the serial peripheral interface module.
 *
 */

#ifndef SPI_CFG_H_
#define SPI_CFG_H_

/*================== Includes =============================================*/
#include "general.h"

#include "cpu_cfg.h"
#include "io.h"

/*================== Macros and Definitions ===============================*/

#define IO_PIN_DATA_STORAGE_EEPROM_SPI_NSS IO_PIN_MCU_0_DATA_STORAGE_EEPROM_SPI_NSS

#define SPI_TRANSMIT_WAKEUP         TRUE

/**
 * Wait time in [us] after sending dummy byte on SPI. Max wait time: 1000us
 */
#define SPI_DUMMY_BYTE_WAIT_TIME        20

/**
 * Wait time in [us] during initialization.
 */
#define SPI_WAKEUP_WAIT_TIME        0

#define SPI_NSS_PORT1   IO_PIN_MCU_0_BMS_INTERFACE_SPI_NSS

#define SPI_HASEEPROM

typedef SPI_HandleTypeDef       SPI_HandleType_s;

/*================== Constant and Variable Definitions ====================*/
extern SPI_HandleType_s spi_devices[];

/**
 * Number of SPI-channels that will be used
 * Maximum number of usable SPI-Channels is <= 6.
 */
extern uint8_t spi_number_of_used_SPI_channels;

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/



#endif /* SPI_CFG_H_ */
