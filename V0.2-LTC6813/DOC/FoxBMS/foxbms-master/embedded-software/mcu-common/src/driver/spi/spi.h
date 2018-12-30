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
 * @file    spi.h
 * @author  foxBMS Team
 * @date    02.10.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  SPI
 *
 * @brief   Headers for the driver for the serial peripheral interface module
 * (encapsulation of some HAL functions).
 *
 */

#ifndef SPI_H_
#define SPI_H_

/*================== Includes =============================================*/
#include "spi_cfg.h"

/*================== Macros and Definitions ===============================*/

/**
 * This structure contains variables relevant for the SPI driver.
 *
 */
typedef struct {
    uint8_t transmit_ongoing;               /*!< time in ms before the state machine processes the next state, e.g. in counts of 1ms    */
    uint8_t counter;                        /*!< general purpose counter */
} SPI_STATE_s;

/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/

/**
 * @brief  initializes the SPI module.
 *
 * This function initializes the SPI channels according to the configuration given as parameter.
 *
 * @param   *hspi      pointer to the spi configuration
 */
extern void SPI_Init(SPI_HandleType_s *hspi);

/**
 * @brief  callback SPI transmit and receive complete from SPI-Interrupt
 *
 * @param  hspi: pointer to SPI hardware handle
 */
extern void HAL_SPI_TxRxCpltCallback(SPI_HandleType_s *hspi);

/**
 * @brief  callback SPI transmit complete from SPI-Interrupt
 *
 * @param  hspi:     pointer to SPI hardware handle
 */
extern void HAL_SPI_TxCpltCallback(SPI_HandleType_s *hspi);

/**
 * @brief   transmits through SPI without receiving data.
 *
 * @param   *hspi       pointer to SPI hardware handle
 * @param   *pData      data to be sent
 * @param   Size        size of the data to be sent
 *
 * @return  E_OK if SPI transmission is OK, E_NOT_OK otherwise
 */
extern STD_RETURN_TYPE_e SPI_Transmit(SPI_HandleType_s *hspi, uint8_t *pData, uint16_t Size);

/**
 * @brief   transmits and receives data through SPI.
 *
 * @param   *hspi         pointer to SPI hardware handle
 * @param   *pTxData      data to be sent
 * @param   *pRxData      data to be received
 * @param   Size          size of the data to be sent/received
 *
 * @return   E_OK if SPI transmission is OK, E_NOT_OK otherwise
 */
extern STD_RETURN_TYPE_e SPI_TransmitReceive(SPI_HandleType_s *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);

/**
 * @brief sets Chip Select low to start SPI transmission.
 *
 * This function sets CS low in case CS is driven by software.
 *
 * @param   busID      selects which CS pin has to be set low
 */
extern void SPI_SetCS(uint8_t busID);

/**
 * @brief   sets Chip Select high to end SPI transmission.
 *
 * This function sets CS high in case CS is driven by software.
 * It is typically called in the callback routine of SPI transmission.
 *
 * @param   busID      selects which CS pin has to be set high
 */
extern void SPI_UnsetCS(uint8_t busID);


/**
 * @brief   gets the SPI transmit status.
 *
 * @return  retval  TRUE if transmission still ongoing, FALSE otherwise
 *
 */
extern STD_RETURN_TYPE_e SPI_IsTransmitOngoing(void);


/**
 * @brief   sets the SPI transmit status.
 *
 */
extern void SPI_SetTransmitOngoing(void);



/*================== Function Implementations =============================*/


#endif /* SPI_H_ */
