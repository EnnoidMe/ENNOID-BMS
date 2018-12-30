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
 * @file    spi.c
 * @author  foxBMS Team
 * @date    02.10.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  SPI
 *
 * @brief   Driver for the serial peripheral interface module (encapsulation of some HAL functions)
 *
 */

/*================== Includes =============================================*/
#include "spi.h"

#include "mcu.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/

/**
 * contains variables used by the SPI driver
 *
 */
static SPI_STATE_s spi_state = {
    .transmit_ongoing       = FALSE,
    .counter                = 0,
};

const uint8_t spi_cmdDummy[1]={0x00};

/*================== Function Prototypes ==================================*/
void SPI_Wait(void);
STD_RETURN_TYPE_e SPI_SendDummyByte(uint8_t busID, SPI_HandleType_s *hspi);

/*================== Function Implementations =============================*/

void SPI_Init(SPI_HandleType_s *hspi) {

    uint8_t i = 0;

    for (i=0; i < spi_number_of_used_SPI_channels; i++) {

        if(hspi != NULL && hspi[i].State  ==  HAL_SPI_STATE_RESET) {

            if(hspi[i].Instance == SPI1) {

                  /* SPI1 peripheral clock enable */
                  __HAL_RCC_SPI1_CLK_ENABLE();
            }

            if(hspi[i].Instance == SPI2) {

                  /* SPI2 peripheral clock enable */
                  __HAL_RCC_SPI2_CLK_ENABLE();
            }

            if(hspi[i].Instance == SPI3) {

                  /* SPI3 peripheral clock enable */
                  __HAL_RCC_SPI3_CLK_ENABLE();
            }

            if(hspi[i].Instance == SPI4) {

                  /* SPI4 peripheral clock enable */
                  __HAL_RCC_SPI4_CLK_ENABLE();
            }

            if(hspi[i].Instance == SPI5) {

                  /* SPI5 peripheral clock enable */
                  __HAL_RCC_SPI5_CLK_ENABLE();
            }

            if(hspi[i].Instance == SPI6) {

                  /* SPI6 peripheral clock enable */
                  __HAL_RCC_SPI6_CLK_ENABLE();
            }
        }
        HAL_SPI_Init(&hspi[i]);
    }
}


void HAL_SPI_TxRxCpltCallback(SPI_HandleType_s *hspi) {
    if (hspi  ==  &spi_devices[0])        /* Iso-SPI Main */
    {
        SPI_UnsetCS(1);
        spi_state.transmit_ongoing = FALSE;
    }
    if (hspi  ==  &spi_devices[1])        /* Eeprom */
    {
#ifdef SPI_HASEEPROM
        IO_WritePin(IO_PIN_DATA_STORAGE_EEPROM_SPI_NSS, IO_PIN_SET);
#endif
    }

}


void HAL_SPI_TxCpltCallback(SPI_HandleType_s *hspi)
{
    if (hspi  ==  &spi_devices[0])        /* Iso-SPI Main */
    {
        SPI_UnsetCS(1);
        spi_state.transmit_ongoing = FALSE;
    }

    if (hspi  ==  &spi_devices[1])        /* Eeprom */
    {
#ifdef SPI_HASEEPROM
        IO_WritePin(IO_PIN_DATA_STORAGE_EEPROM_SPI_NSS, IO_PIN_SET);
#endif
    }
}


void SPI_SetCS(uint8_t busID) {

    switch(busID) {
        case 1:
#ifdef SPI_NSS_PORT1
            IO_WritePin(SPI_NSS_PORT1, IO_PIN_RESET);
#endif
            break;
        case 2:
#ifdef SPI_NSS_PORT2
            IO_WritePin(SPI_NSS_PORT2, IO_PIN_RESET);
#endif
            break;
        case 3:
#ifdef SPI_NSS_PORT3
            IO_WritePin(SPI_NSS_PORT3, IO_PIN_RESET);
#endif
            break;
        case 4:
#ifdef SPI_NSS_PORT4
            IO_WritePin(SPI_NSS_PORT4, IO_PIN_RESET);
#endif
            break;
        case 5:
#ifdef SPI_NSS_PORT5
            IO_WritePin(SPI_NSS_PORT5, IO_PIN_RESET);
#endif
            break;
        case 6:
#ifdef SPI_NSS_PORT6
            IO_WritePin(SPI_NSS_PORT6, IO_PIN_RESET);
#endif
            break;
    }

}


void SPI_UnsetCS(uint8_t busID) {

    switch(busID) {
        case 1:
#ifdef SPI_NSS_PORT1
            IO_WritePin(SPI_NSS_PORT1, IO_PIN_SET);
#endif
            break;
        case 2:
#ifdef SPI_NSS_PORT2
            IO_WritePin(SPI_NSS_PORT2, IO_PIN_SET);
#endif
            break;
        case 3:
#ifdef SPI_NSS_PORT3
            IO_WritePin(SPI_NSS_PORT3, IO_PIN_SET);
#endif
            break;
        case 4:
#ifdef SPI_NSS_PORT4
            IO_WritePin(SPI_NSS_PORT4, IO_PIN_SET);
#endif
            break;
        case 5:
#ifdef SPI_NSS_PORT5
            IO_WritePin(SPI_NSS_PORT5, IO_PIN_SET);
#endif
            break;
        case 6:
#ifdef SPI_NSS_PORT6
            IO_WritePin(SPI_NSS_PORT6, IO_PIN_SET);
#endif
            break;
    }
}


STD_RETURN_TYPE_e SPI_Transmit(SPI_HandleType_s *hspi, uint8_t *pData, uint16_t Size) {

    HAL_StatusTypeDef statusSPI = HAL_ERROR;
    STD_RETURN_TYPE_e retVal = E_OK;

#if SPI_TRANSMIT_WAKEUP  ==  TRUE
    SPI_SetCS(1);
    retVal = SPI_SendDummyByte(0x01, hspi);
    if (retVal != E_OK) {
        return E_NOT_OK;
    }
    SPI_Wait();
#endif

    SPI_SetCS(1);
    statusSPI = HAL_SPI_Transmit_DMA(hspi, pData, Size);
    if(statusSPI != HAL_OK) {
        retVal = E_NOT_OK;
    }
    return retVal;
}


STD_RETURN_TYPE_e SPI_TransmitReceive(SPI_HandleType_s *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size) {

    HAL_StatusTypeDef statusSPI = HAL_ERROR;
    STD_RETURN_TYPE_e retVal = E_OK;

#if SPI_TRANSMIT_WAKEUP  ==  TRUE
    SPI_SetCS(1);
    retVal = SPI_SendDummyByte(0x01, hspi);
    if (retVal != E_OK) {
        return E_NOT_OK;
    }
    SPI_Wait();
#endif

    SPI_SetCS(1);
    statusSPI = HAL_SPI_TransmitReceive_DMA(hspi, pTxData, pRxData, Size);
    if(statusSPI != HAL_OK) {
        retVal = E_NOT_OK;
    }

    return retVal;
}

/**
 * sends a dummy byte on SPI.
 *
 * @param   busID      selects which CS pin has to be set high
 * @param   *hspi      pointer to SPI hardware handle
 *
 * @return             E_OK if SPI transmission is OK, E_NOT_OK otherwise
 */
STD_RETURN_TYPE_e SPI_SendDummyByte(uint8_t busID, SPI_HandleType_s *hspi)
{
    HAL_StatusTypeDef statusSPI;
    STD_RETURN_TYPE_e retVal=E_OK;

    statusSPI = HAL_SPI_Transmit_DMA(hspi, (uint8_t *)spi_cmdDummy, 1);
    if(statusSPI != HAL_OK)
        retVal = E_NOT_OK;

    return retVal;
}


/**
 * waits a defined time.
 *
 */
void SPI_Wait(void)
{
    MCU_Wait_us(SPI_DUMMY_BYTE_WAIT_TIME);
}

extern STD_RETURN_TYPE_e SPI_IsTransmitOngoing(void) {
    STD_RETURN_TYPE_e retval = FALSE;

    retval    =  spi_state.transmit_ongoing;

    return (retval);
}

extern void SPI_SetTransmitOngoing(void) {

    spi_state.transmit_ongoing = TRUE;

}
