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
 * @file    can.h
 * @author  foxBMS Team
 * @date    12.07.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  CAN
 *
 * @brief   Header for the driver for the CAN module
 *
 * Provides the interfaces for initialization, receive
 * and transmit handling
 *
 */

#ifndef CAN_H_
#define CAN_H_

/*================== Includes =============================================*/
#include "can_cfg.h"

/*================== Macros and Definitions ===============================*/

#define CAN0_USE_TX_BUFFER       CAN0_USE_TRANSMIT_BUFFER
#define CAN0_USE_RX_BUFFER       CAN0_USE_RECEIVE_BUFFER

#define CAN0_TX_BUFFER_LENGTH    CAN0_TRANSMIT_BUFFER_LENGTH
#define CAN0_RX_BUFFER_LENGTH    CAN0_RECEIVE_BUFFER_LENGTH

#define CAN1_USE_TX_BUFFER       CAN1_USE_TRANSMIT_BUFFER
#define CAN1_USE_RX_BUFFER       CAN1_USE_RECEIVE_BUFFER

#define CAN1_TX_BUFFER_LENGTH    CAN1_TRANSMIT_BUFFER_LENGTH
#define CAN1_RX_BUFFER_LENGTH    CAN1_RECEIVE_BUFFER_LENGTH

typedef enum {
    CAN_ERROR_NONE = HAL_CAN_ERROR_NONE, /*!< No error             */
    CAN_ERROR_EWG = HAL_CAN_ERROR_EWG, /*!< EWG error            */
    CAN_ERROR_EPV = HAL_CAN_ERROR_EPV, /*!< EPV error            */
    CAN_ERROR_BOF = HAL_CAN_ERROR_BOF, /*!< BOF error            */
    CAN_ERROR_STF = HAL_CAN_ERROR_STF, /*!< Stuff error          */
    CAN_ERROR_FOR = HAL_CAN_ERROR_FOR, /*!< Form error           */
    CAN_ERROR_ACK = HAL_CAN_ERROR_ACK, /*!< Acknowledgment error */
    CAN_ERROR_BR = HAL_CAN_ERROR_BR, /*!< Bit recessive        */
    CAN_ERROR_BD = HAL_CAN_ERROR_BD, /*!< LEC dominant         */
    CAN_ERROR_CRC = HAL_CAN_ERROR_CRC, /*!< LEC transfer error   */
} CAN_ErrorTypeDef_e;

typedef enum {
    CAN_NODE1 = 0, /* CAN1 */
    CAN_NODE0 = 1, /* CAN0 */
} CAN_NodeTypeDef_e;

typedef struct CAN_ERROR {
    CAN_ErrorTypeDef_e canError;
    uint16_t canErrorCounter[10 - 1];  /* One slot for every error from HAL_CAN_ErrorTypeDef; */
                                       /* No space for NoError */
} CAN_ERROR_s;

typedef struct CAN_RX_BUFFERELEMENT {
    uint32_t ID;
    uint8_t DLC;
    uint8_t RTR;
    uint8_t Data[8];
    uint8_t newMsg;
} CAN_RX_BUFFERELEMENT_s;

typedef struct CAN_RX_BUFFER {
    uint8_t ptrRead;
    uint8_t ptrWrite;
    uint8_t length;
    CAN_RX_BUFFERELEMENT_s* buffer;
} CAN_RX_BUFFER_s;

typedef struct CAN_TX_BUFFERELEMENT {
    CanTxMsgTypeDef msg;
    uint8_t newMsg;
} CAN_TX_BUFFERELEMENT_s;

typedef struct CAN_TX_BUFFER {
    uint8_t ptrRead;
    uint8_t ptrWrite;
    uint8_t length;
    CAN_TX_BUFFERELEMENT_s* buffer;
} CAN_TX_BUFFER_s;

/*================== Constant and Variable Definitions ====================*/
/**
 * @brief  CAN listen only transceiver mode of CAN node 0
 */
extern uint8_t canNode0_listenonly_mode;

/**
 * @brief  CAN listen only transceiver mode of CAN node 1
 */
extern uint8_t canNode1_listenonly_mode;
/*================== Function Prototypes ==================================*/
/* Init */

/**
 * @brief  Initializes CAN settings and message filtering
 *
 * @retval 0: if initialization successful, otherwise errorcode
 */
extern uint32_t CAN_Init(void);

/* Interrupt handling */

/**
 * @brief  Handles CAN TX interrupt request
 * @param  ptrHcan: pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval none (void)
 */
extern void CAN_TX_IRQHandler(CAN_HandleTypeDef* ptrHcan);

/**
 * @brief  Handles CAN RX interrupt request
 *
 * @param  canNode: canNode that received a message
 * @param  ptrHcan: pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 *
 * @retval None
 */
extern void CAN_RX_IRQHandler(CAN_NodeTypeDef_e canNode, CAN_HandleTypeDef* ptrHcan);

/**
 * @brief   Handles CAN error interrupt request
 *
 * @param   canNode:
 * @param   ptrHcan: pointer to a CAN_HandleTypeDef structure that contains
 *          the configuration information for the specified CAN.
 *
 * @retval  None
 */
extern void CAN_Error_IRQHandler(CAN_NodeTypeDef_e canNode, CAN_HandleTypeDef* ptrHcan);

/* Transmit Message */

/**
 * @brief  Transmits message directly on the CAN bus
 *
 * @param  canNode: canNode on which the message shall be transmitted
 * @param  msgID:    ID of the message that will be transmitted
 * @param  ptrMsgData:    pointer to the data that shall be transmitted
 * @param  msgLength:    Specifies the data length
 * @param  RTR: Specifies the type of frame for the message that will be transmitted.
 *
 * @retval E_OK if transmission successful, otherwise E_NOT_OK
 */
extern STD_RETURN_TYPE_e CAN_TxMsg(CAN_NodeTypeDef_e canNode, uint32_t msgID, uint8_t* ptrMsgData,
        uint32_t msgLength, uint32_t RTR);

/**
 * @brief  Add message to transmit buffer, message will be transmitted shortly after.
 *
 *         ------------------------ IMPORTANT!!!! --------------------------------
 *         Make sure that this function is not interrupted by the operating system
 *         during its execution.
 *
 * @param  canNode: canNode on which the message shall be transmitted
 * @param  msgID:    ID of the message that will be transmitted
 * @param  ptrMsgData:    pointer to a uint8_t array that contains the message that will be transmitted
 * @param  msgLength:    length of the message that will be transmitted
 *                 This parameter can be a value of CAN_identifier_type.
 * @param  RTR     Specifies the type of frame for the message that will be transmitted.
 *                 This parameter can be a value of CAN_remote_transmission_request
 *
 * @retval E_OK if successful, E_NOT_OK if buffer is full or error occurred
 */
extern STD_RETURN_TYPE_e CAN_Send(CAN_NodeTypeDef_e canNode, uint32_t msgID, uint8_t* ptrMsgData,
        uint32_t msgLength, uint32_t RTR);

/**
 * @brief  Transmits a can message from transmit buffer
 *
 *         ------------------------ IMPORTANT!!!! --------------------------------
 *         Make sure that this function is not interrupted by the operating system
 *         during its execution.
 *
 * @param canNode:  canNode on which the message shall be transmitted
 *
 * @retval E_OK if transmission successful, otherwise E_NOT_OK
 */
extern STD_RETURN_TYPE_e CAN_TxMsgBuffer(CAN_NodeTypeDef_e canNode);

/* Read Message */

/**
 * @brief  Reads a can message from RxBuffer
 *
 * @param canNode canNode on which a message has been received
 * @param msg     message that has been received
 *
 * @retval E_OK if reception successful, if buffer empty or invalid pointer E_NOT_OK
 */
extern STD_RETURN_TYPE_e CAN_ReceiveBuffer(CAN_NodeTypeDef_e canNode, Can_PduType* msg);

/* Sleep mode */

/**
 * @brief  Set CAN to sleep mode
 *
 * @param canNode  canNode which shall be put to sleep mode
 *
 * @retval none (void)
 */
extern void CAN_SetSleepMode(CAN_NodeTypeDef_e canNode);

/**
 * @brief  Wake CAN up from sleep mode
 *
 * @param canNode  canNode which shall be waken up from sleep mode
 *
 * @retval none.
 */
extern void CAN_WakeUp(CAN_NodeTypeDef_e canNode);

/*================== Function Implementations =============================*/

/* ***************************************
 *  CAN Fault Confinement
 ****************************************/

/* Fault confinement is a checking mechanism that makes it possible to distinguish between short disturbances
 (e.g. switching noise from a nearby power cable couples into the transmission media) and permanent failures
 (e.g. a node is malfunctioning and disturbs the bus).

 Manipulation of the error counters is asymmetric. On a successful transmission, or reception, of a message,
 the respective error counter is decremented if it had not been at zero. In the case of a transmit or receive
 error the counters are incremented, but by a value greater than the value they would be decrement by following
 a successful message transaction.

 If a node detects a local error condition (e.g. due to local conducted noise, application software, etc.),
 its resulting error flag (primary error flag) will subsequently cause all other nodes to respond with an error
 flag too (secondary error flags). It is important that a distinction is made between the nodes that detected an
 error first and the nodes which responded to the primary error flag. If a node transmits an active error frame,
 and it monitors a dominant bit after the sixth bit of its error flag, it considers itself as the node that has
 detected the error first. In the case where a node detects errors first too often, it is regarded as malfunctioning,
 and its impact to the network has to be limited. Therefore, a node can be in one of three possible error states:

 ERROR ACTIVE - Both of its error counters are less than 128. It takes part fully in bus communication and signals
 an error by transmission of an active error frame.This consists of sequence of 6 dominant bits followed
 by 8 recessive bits, all other nodes respond with the appropriate error flag, in response to the
 violation of the bit stuffing rule.

 ERROR PASSIVE - A node goes into error passive state if at least one of its error counters is greater than 127. It
 still takes part in bus activities, but it sends a passive error frame only, on errors. Furthermore,
 an error passive node has to wait an additional time (Suspend Transmission Field, 8 recessive bits after
 Intermission Field) after transmission of a message, before it can initiate a new data transfer. The
 primary passive error flag consists of 6 passive bits and thus is transparent on the bus and will
 not jam communications.

 BUS OFF - If the Transmit Error Counter of a CAN controller exceeds 255, it goes into the bus off state. It is
 disconnected from the bus (using internal logic) and does not take part in bus activities anymore. In order
 to reconnect the protocol controller, a so-called Bus Off recovery sequence has to be executed. This usually
 involves the re-initialization and configuration of the CAN controller by the host system, after which it
 will wait for 128 * 11 recessive bit times before it commences communication.

 */

/* ***************************************
 *  CAN Error Confinement Rules
 ****************************************/

/* REC: Receive Error Counter, TEC: Transmit Error Counter */

/* - When a receiver detects an error, the REC will be increased by 1, except when the detected error was a Bit Error
 during the sending of an Active error Flag or an Overload Flag.

 - When a receiver detects a dominant bit as the first bit after sending an Error Flag, the REC will be increased by 8.

 - When a transmitter sends an Error Flag, the TEC is increased by 8.
 Exception 1: If the transmitter is Error Passive and detects an ACK Error because of not detecting a dominant ACK
 and does not detect a dominant bit while sending its Passive Error Flag.
 Exception 2: If the transmitter sends an Error Flag because a Stuff Error occurred during arbitration, and should
 have been recessive, and has been sent as recessive but monitored as dominant.

 - If the transmitter detects a Bit Error while sending an Active Error Flag or an Overload Frame, the TEC is increased by 8.

 - If a receiver detects a Bit Error while sending an Active Error Flag or an Overload Flag, the REC is increased by 8.

 - Any node tolerates up to 7 consecutive dominant bits after sending an Active Error Flag, Passive Error Flag or Overload Flag.
 After detecting the fourteenth consecutive dominant bit (in case of an Active Error Flag or an Overload Flag) or after
 detecting the eighth consecutive dominant bit following a Passive Error Flag, and after each sequence of additional eight
 consecutive dominant bits, ever y transmitter increases its TEC by 8 and every receiver increases its REC by 8.

 - After successful transmission of a frame (getting ACK and no error until EOF is finished), the TEC is decreased by 1 unless it was already 0.

 - After the successful reception of a frame (reception without error up to the ACK Slot and the successful sending of the ACK bit),
 the REC is decreased by 1, if it was between 1 and 127. If the REC was 0, it stays 0, and if it was greater than 127, then it
 will be set to a value between 119 and 127.

 - A node is Error Passive when the TEC equals or exceeds 128, or when the REC equals or exceeds 128. An error condition
 letting a node become Error Passive causes the node to send an Active Error Flag.

 - A node is Bus Off when the TEC is greater than or equal to 256.

 - An Error Passive node becomes Error Active again when both the TEC and the REC are less than or equal to 127.

 - A node which is Bus Off is permitted to become Error Active (no longer Bus Off) with its error counters both set to 0
 after 128 occurrence of 11 consecutive recessive bits have been monitored on the bus.
 */

#endif /* CAN_H_ */
