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
 * @file    spi_cfg.c
 * @author  foxBMS Team
 * @date    02.10.2015 (date of creation)
 * @ingroup DRIVERS_CONF
 * @prefix  SPI
 *
 * @brief   Configuration for the serial peripheral interface module.
 *
 */

/*================== Includes =============================================*/
#include "spi_cfg.h"

#include "dma.h"

/*================== Macros and Definitions ===============================*/


/*================== Constant and Variable Definitions ====================*/
SPI_HandleType_s spi_devices[] = {

    {
        .Instance = SPI1,
        .Init.Mode = SPI_MODE_MASTER,
        .Init.Direction = SPI_DIRECTION_2LINES,
        .Init.DataSize = SPI_DATASIZE_8BIT,
        .Init.CLKPolarity = SPI_POLARITY_LOW,   /* CPOL = 0 */
        .Init.CLKPhase = SPI_PHASE_1EDGE,       /* CPHA = 0 */
        .Init.NSS = SPI_NSS_SOFT,
        .Init.FirstBit = SPI_FIRSTBIT_MSB,
        .Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128,  /* SPI clock = APB2_CLOCK / 128 = 90 / 128 = 703.125 kHz */
        .Init.TIMode = SPI_TIMODE_DISABLED,
        .Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED,
        .Init.CRCPolynomial = 1,
        .hdmarx = &dma_devices[0],
        .hdmatx = &dma_devices[1]
    },
    {
        .Instance = SPI6,
        .Init.Mode = SPI_MODE_MASTER,
        .Init.Direction = SPI_DIRECTION_2LINES,
        .Init.DataSize = SPI_DATASIZE_8BIT,
        .Init.CLKPolarity = SPI_POLARITY_LOW,
        .Init.CLKPhase = SPI_PHASE_1EDGE,
        .Init.NSS = SPI_NSS_SOFT,
        .Init.FirstBit = SPI_FIRSTBIT_MSB,
        .Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128,
        .Init.TIMode = SPI_TIMODE_DISABLED,
        .Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED,
        .Init.CRCPolynomial = 1,
    }
};

uint8_t spi_number_of_used_SPI_channels = sizeof(spi_devices)/sizeof(SPI_HandleType_s);


/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/
