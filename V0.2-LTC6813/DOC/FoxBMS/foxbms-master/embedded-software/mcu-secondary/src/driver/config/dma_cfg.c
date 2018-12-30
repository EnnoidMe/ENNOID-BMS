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
 * @file    dma_cfg.c
 * @author  foxBMS Team
 * @date    02.10.2015 (date of creation)
 * @ingroup DRIVERS_CONF
 * @prefix  DMA
 *
 * @brief   Configuration for the DMA module
 *
 */

/*================== Includes =============================================*/
#include "dma_cfg.h"

#include "spi.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/
DMA_HandleTypeDef dma_devices[] = {
/* SPI1 RX */
    {
        .Instance = DMA2_Stream2,
        .Init.Channel = DMA_CHANNEL_3,
        .Init.Direction = DMA_PERIPH_TO_MEMORY,
        .Init.PeriphInc = DMA_PINC_DISABLE,
        .Init.MemInc = DMA_MINC_ENABLE,
        .Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
        .Init.MemDataAlignment = DMA_MDATAALIGN_BYTE,
        .Init.Mode = DMA_NORMAL,
        .Init.Priority = DMA_PRIORITY_LOW,
        .Init.FIFOMode = DMA_FIFOMODE_DISABLE,
        .Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL,
        .Init.MemBurst = DMA_MBURST_SINGLE,
        .Init.PeriphBurst = DMA_PBURST_SINGLE,
        .Parent = &spi_devices[0]
    },
/* SPI1 TX */
    {
        .Instance = DMA2_Stream3,
        .Init.Channel = DMA_CHANNEL_3,
        .Init.Direction = DMA_MEMORY_TO_PERIPH,
        .Init.PeriphInc = DMA_PINC_DISABLE,
        .Init.MemInc = DMA_MINC_ENABLE,
        .Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE,
        .Init.MemDataAlignment = DMA_MDATAALIGN_BYTE,
        .Init.Mode = DMA_NORMAL,
        .Init.Priority = DMA_PRIORITY_LOW,
        .Init.FIFOMode = DMA_FIFOMODE_DISABLE,
        .Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL,
        .Init.MemBurst = DMA_MBURST_SINGLE,
        .Init.PeriphBurst = DMA_PBURST_SINGLE,
        .Parent = &spi_devices[0]
    }
};

const uint8_t dma_number_of_used_streams = sizeof(dma_devices)/sizeof(DMA_HandleTypeDef);

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/
