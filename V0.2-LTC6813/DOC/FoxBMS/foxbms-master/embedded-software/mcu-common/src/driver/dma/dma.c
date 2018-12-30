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
 * @file    dma.c
 * @author  foxBMS Team
 * @date    02.10.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  DMA
 *
 * @brief   Driver for the DMA module (encapsulation of the init function).
 *
 */

/*================== Includes =============================================*/
#include "dma.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

void DMA_Init(DMA_HandleTypeDef *hdma) {

    uint8_t i = 0;

    if(hdma != NULL) {

        for (i = 0; i < dma_number_of_used_streams; i++) {

            if ( (hdma[i].Instance == DMA1_Stream0)
                ||(hdma[i].Instance == DMA1_Stream1)
                ||(hdma[i].Instance == DMA1_Stream2)
                ||(hdma[i].Instance == DMA1_Stream3)
                ||(hdma[i].Instance == DMA1_Stream4)
                ||(hdma[i].Instance == DMA1_Stream5)
                ||(hdma[i].Instance == DMA1_Stream6)
                ||(hdma[i].Instance == DMA1_Stream7)
                ) {
                __HAL_RCC_DMA1_CLK_ENABLE();
            }

            if ( (hdma[i].Instance == DMA2_Stream0)
                ||(hdma[i].Instance == DMA2_Stream1)
                ||(hdma[i].Instance == DMA2_Stream2)
                ||(hdma[i].Instance == DMA2_Stream3)
                ||(hdma[i].Instance == DMA2_Stream4)
                ||(hdma[i].Instance == DMA2_Stream5)
                ||(hdma[i].Instance == DMA2_Stream6)
                ||(hdma[i].Instance == DMA2_Stream7)
                ) {
                __HAL_RCC_DMA2_CLK_ENABLE();
            }

            HAL_DMA_Init(&hdma[i]);

        }
    }
}
