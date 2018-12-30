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
 * @file    uart.c
 * @author  foxBMS Team
 * @date    23.09.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  UART
 *
 * @brief   Driver for the UART
 *
 * This uart module handles sending/receiving requests using UART/RS232.
 * In its current implementation it's interrupt driven, may change in the future
 * in order to support DMA.
 *
 */

/*================== Includes =============================================*/
#include "uart.h"

/*================== Macros and Definitions ===============================*/

#define MSG_NOT_BUSY    0
#define MSG_BUSY        1


/*================== Constant and Variable Definitions ====================*/

volatile unsigned char Msg0SendBusy=0;

uint8_t rxbuf[RXBUF_LENGTH];
uint8_t txbuf[TXBUF_LENGTH];

uint8_t *wrpoi_rxbuf=&rxbuf[0];
uint8_t *rdpoi_rxbuf=&rxbuf[0];

uint8_t *wrpoi_txbuf=&txbuf[0];
uint8_t *rdpoi_txbuf=&txbuf[0];


/*================== Constant and Variable Definitions ====================*/

char uart_com_receivedbyte[UART_COM_RECEIVEBUFFER_LENGTH];
uint8_t uart_com_receive_slot;

/*================== Function Prototypes ==================================*/

static void UART_IntRx(void);
static void UART_IntTx(void);
static void UART_IntRxComp(void);
static uint8_t *UART_txbuf_copy (uint8_t *ptrb, uint8_t *ptra);
static uint8_t *UART_txbuf_copy_length (uint8_t *ptrb, uint8_t *ptra, uint16_t length);

/*================== Function Implementations =============================*/


void UART_Init(/*UART_HandleTypeDef *uart_cfg*/ void)
{

    for (int i = 0; i < uart_cfg_length; i++) {

        HAL_UART_Init(&uart_cfg[i]);

        /* Enable the UART Parity Error Interrupt */
        SET_BIT(uart_cfg[i].Instance->CR1, USART_CR1_PEIE);

        /* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
        SET_BIT(uart_cfg[i].Instance->CR3, USART_CR3_EIE);

        /* Enable the UART Data Register not empty Interrupt */
        SET_BIT(uart_cfg[i].Instance->CR1, USART_CR1_RXNEIE);
    }
}


void HAL_UART_CustomIRQHandler(UART_HandleTypeDef *huart)
{
    uint32_t isrflags   = READ_REG(huart->Instance->SR);
    uint32_t cr1its     = READ_REG(huart->Instance->CR1);
    uint32_t cr3its     = READ_REG(huart->Instance->CR3);
    uint32_t errorflags = 0x00U;
    uint32_t dmarequest = 0x00U;

   /* If no error occurs */
   errorflags = (isrflags & (uint32_t)(USART_SR_PE | USART_SR_FE | USART_SR_ORE | USART_SR_NE));
   if(errorflags == RESET)
   {
     /* UART in mode Receiver -------------------------------------------------*/
     if(((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
     {
       UART_IntRx(); /* @todo use general handler according to HAL-Interface:  UART_Receive_IT(huart); */
       return;
     }
   }

   /* If some errors occur */
   if((errorflags != RESET) && ((cr3its & (USART_CR3_EIE | USART_CR1_PEIE)) != RESET))
   {
     /* UART parity error interrupt occurred ----------------------------------*/
     if(((isrflags & USART_SR_PE) != RESET) && ((cr1its & USART_CR1_PEIE) != RESET))
     {
       huart->ErrorCode |= HAL_UART_ERROR_PE;
     }

     /* UART noise error interrupt occurred -----------------------------------*/
     if(((isrflags & USART_SR_NE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
     {
       huart->ErrorCode |= HAL_UART_ERROR_NE;
     }

     /* UART frame error interrupt occurred -----------------------------------*/
     if(((isrflags & USART_SR_FE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
     {
       huart->ErrorCode |= HAL_UART_ERROR_FE;
     }

     /* UART Over-Run interrupt occurred --------------------------------------*/
     if(((isrflags & USART_SR_ORE) != RESET) && ((cr3its & USART_CR3_EIE) != RESET))
     {
       huart->ErrorCode |= HAL_UART_ERROR_ORE;
     }

     /* Call UART Error Call back function if need be --------------------------*/
     if(huart->ErrorCode != HAL_UART_ERROR_NONE)
     {
       /* UART in mode Receiver -----------------------------------------------*/
       if(((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
       {
           UART_IntRx(); /* @todo use general handler according to HAL-Interface:  UART_Receive_IT(huart); */
       }

       /* If Overrun error occurs, or if any error occurs in DMA mode reception,
          consider error as blocking */
       dmarequest = HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR);
       if(((huart->ErrorCode & HAL_UART_ERROR_ORE) != RESET) || dmarequest)
       {
         /* Blocking error : transfer is aborted
            Set the UART state ready to be able to start again the process,
            Disable Rx Interrupts, and disable Rx DMA request, if ongoing */
/* @todo         UART_EndRxTransfer(huart); */

         /* Disable the UART DMA Rx request if enabled */
         if(HAL_IS_BIT_SET(huart->Instance->CR3, USART_CR3_DMAR))
         {
           CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);

           /* Abort the UART DMA Rx channel */
           if(huart->hdmarx != NULL)
           {
             /* Set the UART DMA Abort callback :
                will lead to call HAL_UART_ErrorCallback() at end of DMA abort procedure */
/* @todo      huart->hdmarx->XferAbortCallback = UART_DMAAbortOnError; */
             if(HAL_DMA_Abort_IT(huart->hdmarx) != HAL_OK)
             {
               /* Call Directly XferAbortCallback function in case of error */
/* @todo        huart->hdmarx->XferAbortCallback(huart->hdmarx); */
                 UART_HandleTypeDef* huartptr = ( UART_HandleTypeDef* )((DMA_HandleTypeDef* )(huart->hdmarx))->Parent;
                 huartptr->RxXferCount = 0;
                 huartptr->TxXferCount = 0;
                 HAL_UART_ErrorCallback(huartptr);
             }
           }
           else
           {
             /* Call user error callback */
             HAL_UART_ErrorCallback(huart);
           }
         }
         else
         {
           /* Call user error callback */
           HAL_UART_ErrorCallback(huart);
         }
       }
       else
       {
         /* Non Blocking error : transfer could go on.
            Error is notified to user through user error callback */
         HAL_UART_ErrorCallback(huart);
         huart->ErrorCode = HAL_UART_ERROR_NONE;
       }
     }
     return;
   } /* End if some error occurs */

   /* UART in mode Transmitter ------------------------------------------------*/
   if(((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
   {
     UART_IntTx(); /* @todo use general handler according to HAL-Interface: UART_Transmit_IT(huart); */
     return;
   }

   /* UART in mode Transmitter end --------------------------------------------*/
   if(((isrflags & USART_SR_TC) != RESET) && ((cr1its & USART_CR1_TCIE) != RESET))
   {
     UART_IntRxComp(); /* @todo use general handler according to HAL-Interface:  UART_EndTransmit_IT(huart); */
     return;
   }
}

/**
 * @brief UART_IntRx is responsible for handling receive requests.
 *
 * This is invoked from with the interrupt handler and processes
 * userdata on receive event.
 * It copies data from data register into a ringbuffer,
 * provides some very basic application example.
 */
static void UART_IntRx(void)
{
/*   *asc0_wrpoi_rxbuf=(ASC0_RBUF.U & 0xFF);       read 8-Bit receive buffer register (like ASC0_usGetData(void) ) */
    *wrpoi_rxbuf = (uint8_t)(uart_cfg[0].Instance->DR/* & huart3.Mask*/);
    /*pointer handling of ringbuffer*/
    if(++wrpoi_rxbuf >= &rxbuf[RXBUF_LENGTH])
        wrpoi_rxbuf = &rxbuf[0];
    /* check overwrite of read pointer */

    /* todo: this is just a very simple debug/demonstration implementation for decoder */
    if(uart_com_receive_slot < UART_COM_RECEIVEBUFFER_LENGTH-1)
    {
    *(uart_com_receivedbyte + uart_com_receive_slot) = *rdpoi_rxbuf;
    uart_com_receive_slot++;
    }

    /*pointer handling of ringbuffer*/
    if(++rdpoi_rxbuf >= &rxbuf[RXBUF_LENGTH])
       rdpoi_rxbuf = &rxbuf[0];
}

/**
 * @brief UART_IntTx is responsible for handling send requests.
 *
 * This is invoked from with the interrupt handler and processes
 * userdata on send event.
 * It copies data from a ringbuffer into the data register
 * as long as the read pointer doesn't match the write pointer
 * (which means there's still unsent data in the ringbuffer)
 */
static void UART_IntTx(void)
{
    if(rdpoi_txbuf >= &txbuf[TXBUF_LENGTH])
        rdpoi_txbuf = &txbuf[0];

    if(rdpoi_txbuf != wrpoi_txbuf)
    {
        Msg0SendBusy=MSG_BUSY;
/*       ASC0_vSendData(*asc0_rdpoi_txbuf++);   send when pointers unequal (values in buffer) */
        uart_cfg[0].Instance->DR = (uint16_t) (*rdpoi_txbuf++);
    }
    else
    {
        /* Disable the UART Transmit Data Register Empty Interrupt */
        CLEAR_BIT(uart_cfg[0].Instance->CR1, USART_CR1_TXEIE);

        /* Enable the UART Transmit Complete Interrupt */
        SET_BIT(uart_cfg[0].Instance->CR1, USART_CR1_TCIE);

        Msg0SendBusy=MSG_NOT_BUSY;
    }
}

/**
 * Disables UART transmit complete interrupt
 */
static void UART_IntRxComp(void)
{
  /* Disable the UART Transmit Complete Interrupt */
  CLEAR_BIT(uart_cfg[0].Instance->CR1, USART_CR1_TCIE);

  /* optional: call TX complete function */
}


void UART_vWrite(const uint8_t *source)
{

    wrpoi_txbuf=UART_txbuf_copy (wrpoi_txbuf,(uint8_t*) source);
    if (Msg0SendBusy==MSG_NOT_BUSY)
    {
        /* Enable the UART Transmit Data Register Empty Interrupt */
        SET_BIT(uart_cfg[0].Instance->CR1, USART_CR1_TXEIE);
    }

}

void UART_vWrite_intbuf(const uint8_t *source, uint16_t length)
{

    wrpoi_txbuf=UART_txbuf_copy_length (wrpoi_txbuf,(uint8_t*) source, length);
    if (Msg0SendBusy==MSG_NOT_BUSY)
    {
        /* Enable the UART Transmit Data Register Empty Interrupt */
        SET_BIT(uart_cfg[0].Instance->CR1, USART_CR1_TXEIE);
    }
}

/**
 * @brief UART_txbuf_copy is a helper function.
 *
 * This function just copies content from buffer a to ringbuffer b.
 *
 * @return (type: uint8_t)
 */
static uint8_t *UART_txbuf_copy (uint8_t *ptrb, uint8_t *ptra)
{
    uint8_t *tmpPtr = ptra;

    while(*ptra)
    {
        *ptrb++=*ptra++;
        if(ptrb >= &txbuf[TXBUF_LENGTH])
        {
            ptrb = &txbuf[0];
        }

    }
    /*  do not send NULL-Value */

    /* Clear buffer a */
    while(*tmpPtr) {
        *tmpPtr = 0;
        tmpPtr++;
    }

    return ptrb;
}

/**
 * @brief UART_txbuf_copy_length is a helper function.
 *
 * This function just copies a given length of data from buffer a to ringbuffer b.
 * Difference to uint8_t *UART_txbuf_copy (uint8_t *ptrb, uint8_t *ptra) is, that it will stop
 * at an ASCII_NULL character.
 *
 * @return (type: uint8_t)
 */
static uint8_t *UART_txbuf_copy_length (uint8_t *ptrb, uint8_t *ptra, uint16_t length)
{
    uint8_t *tmpPtr = ptra;
    uint8_t tmp = 0;

    while(tmp < length)
    {
        *ptrb++=*ptra++;
        if(ptrb >= &txbuf[TXBUF_LENGTH])
        {
            ptrb = &txbuf[0];
        }
        tmp++;

    }
    /*  do not send NULL-Value */
    tmp = 0;
    /* Clear buffer a */
    while(tmp < length) {
        *tmpPtr = 0;
        tmpPtr++;
        tmp++;
    }

    return ptrb;
}
