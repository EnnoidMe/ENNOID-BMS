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
 * @file    uart.h
 * @author  foxBMS Team
 * @date    26.08.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  UART
 *
 * @brief   Headers for the driver for the UART
 *
 */

#ifndef UART_H_
#define UART_H_

/*================== Includes =============================================*/
#include "uart_cfg.h"

/*================== Macros and Definitions ===============================*/
#define RXBUF_LENGTH    0x100    /* 256 Byte */
#define TXBUF_LENGTH    0x300    /* 768 Byte */

#define ACK_SYMBOL  0x06
#define NAK_SYMBOL  0x15
#define STX_SYMBOL  0x02
#define ETX_SYMBOL  0x03

#define UART_COM_RECEIVEBUFFER_LENGTH    100
/*================== Constant and Variable Definitions ====================*/
extern char uart_com_receivedbyte[UART_COM_RECEIVEBUFFER_LENGTH];
extern uint8_t uart_com_receive_slot;

/*================== Function Prototypes ==================================*/

/**
 * @brief UART_Init provides initialization of the UART module.
 *
 * It needs to be called during startup, send/receive will only work
 * after initialization is completed.
 * This function configures parameters of the UART interface and
 * enables the needed interrupts.
 *
 * Available baudrate is dependent on Sysclock.
 */
extern void UART_Init(void);

/**
 * @brief HAL_UART_CustomIRQHandler is responsible for handling all requests.
 *
 * This custom interrupt handler does all the handling for
 * sending/receiving data, reset and error handling.
 * In case of send/receive a subfunction is called which handles further processing.
 */
extern void HAL_UART_CustomIRQHandler(UART_HandleTypeDef *huart);

/**
 * @brief UART_vWrite provides an interface to send data.
 *
 *         ------------------------ IMPORTANT!!!! --------------------------------
 *         Make sure that this function is not interrupted by the operating system
 *         during its execution.
 *
 * This function copies data from input buffer to transmit ringbuffer
 * and processes this data in case it's not already busy transmitting data.
 * The copying will stop as soon as an ASCII NULL character is detected.
 * To avoid data corruption caused by simultaneous accesses some kind of
 * critical section is used.
 *
 * @return (type: uint8_t)
 */
extern void UART_vWrite(const uint8_t *source);

/**
 * @brief UART_vWrite provides an interface to send data.
 *
 *         ------------------------ IMPORTANT!!!! --------------------------------
 *         Make sure that this function is not interrupted by the operating system
 *         during its execution.
 *
 * This function copies data from input buffer to transmit ringbuffer
 * and processes this data in case it's not already busy transmitting data.
 * To avoid data corruption caused by simultaneous accesses some kind of
 * critical section is used. The difference between UART_vWrite(const uint8_t *source) and
 * this function is, that this one does not stop at an ASCII_NULL character. Therefore writing of non ASCII characters
 * is possible without having to worry that the data might represent an ASCII NULL character.
 *
 * @return (type: uint8_t)
 */
extern void UART_vWrite_intbuf(const uint8_t *source, uint16_t length);

/*================== Function Implementations =============================*/


#endif /* UART_H_ */
