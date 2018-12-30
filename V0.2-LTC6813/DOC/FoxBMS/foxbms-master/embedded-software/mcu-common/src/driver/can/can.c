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
 * @file    can.c
 * @author  foxBMS Team
 * @date    12.07.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  CAN
 *
 * @brief   Driver for the CAN module
 *
 * Implementation of the CAN Interrupts, initialization, buffers,
 * receive and transmit interfaces.
 *
 */

/*================== Includes =============================================*/
#include "can.h"

/*================== Macros and Definitions ===============================*/
#define ID_16BIT_FIFO0          0
#define ID_16BIT_FIFO1          1
#define ID_32BIT_FIFO0          2
#define ID_32BIT_FIFO1          3
#define MSK_16BIT_FIFO0         4
#define MSK_16BIT_FIFO1         5
#define MSK_32BIT               6

/*================== Constant and Variable Definitions ====================*/
uint8_t canNode0_listenonly_mode = 0;
uint8_t canNode1_listenonly_mode = 0;

#if CAN_USE_CAN_NODE0
#if CAN0_USE_TX_BUFFER
CAN_TX_BUFFERELEMENT_s can0_txbufferelements[CAN0_TX_BUFFER_LENGTH];
CAN_TX_BUFFER_s can0_txbuffer = {
    .length = CAN0_TX_BUFFER_LENGTH,
    .buffer = &can0_txbufferelements[0],
};
#endif

#if CAN0_USE_RX_BUFFER
CAN_RX_BUFFERELEMENT_s can0_rxbufferelements[CAN0_RX_BUFFER_LENGTH];
CAN_RX_BUFFER_s can0_rxbuffer = {
    .length = CAN0_RX_BUFFER_LENGTH,
    .buffer = &can0_rxbufferelements[0],
};
#endif

#if CAN0_BUFFER_BYPASS_NUMBER_OF_IDs > 0
uint8_t can0_fastLinkIndex[CAN0_BUFFER_BYPASS_NUMBER_OF_IDs];   /* Link Table for bufferBypassing */
#endif

CAN_ERROR_s CAN0_errorStruct = {
    .canError = HAL_CAN_ERROR_NONE,
    .canErrorCounter = { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
#endif

#if CAN_USE_CAN_NODE1
#if CAN1_USE_TX_BUFFER
CAN_TX_BUFFERELEMENT_s can1_txbufferelements[CAN1_TX_BUFFER_LENGTH];
CAN_TX_BUFFER_s can1_txbuffer = {
        .length = CAN1_TX_BUFFER_LENGTH,
        .buffer = &can1_txbufferelements[0],
};

#endif

#if CAN1_USE_RX_BUFFER
CAN_RX_BUFFERELEMENT_s can1_rxbufferelements[CAN1_RX_BUFFER_LENGTH];
CAN_RX_BUFFER_s can1_rxbuffer = {
        .length = CAN1_RX_BUFFER_LENGTH,
        .buffer = &can1_rxbufferelements[0],
};
#endif

#if CAN1_BUFFER_BYPASS_NUMBER_OF_IDs > 0
uint8_t can1_fastLinkIndex[CAN1_BUFFER_BYPASS_NUMBER_OF_IDs];   /* Link Table for bufferBypassing */
#endif

CAN_ERROR_s CAN1_errorStruct = {
    .canError = HAL_CAN_ERROR_NONE,
    .canErrorCounter = { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};
#endif


#if (CAN1_BUFFER_BYPASS_NUMBER_OF_IDs > 0 && CAN_USE_CAN_NODE1) || (CAN0_BUFFER_BYPASS_NUMBER_OF_IDs > 0 && CAN_USE_CAN_NODE0)
uint8_t fastLinkBuffer[8]; /* data buffer for bypassed message, therefore size = 8 */
#endif

/* ***********************************************************
 *  Dummies for filter initialization and message reception
 *************************************************************/

CAN_FilterConfTypeDef sFilterConfig = {
        /* No need to insert here something */
        .FilterActivation = ENABLE,     /* enable the filter */
};

CanRxMsgTypeDef sReceiveStruct0 = {
        /* No need to insert here something */
};

CanRxMsgTypeDef sReceiveStruct1 = {
        /* No need to insert here something */
};

/*================== Function Prototypes ==================================*/
/* Inits */
static uint8_t CAN_GetNextID(CAN_MSG_RX_TYPE_s* can_RxMsgs, uint8_t numberOfRxIDs, uint8_t startIndex,
        uint8_t filterCase);
static uint8_t CAN_NumberOfNeededFilters(CAN_MSG_RX_TYPE_s* can_RxMsgs, uint8_t* numberOfDifferentIDs, uint32_t* error);
static uint32_t CAN_InitFilter(CAN_HandleTypeDef* ptrHcan, CAN_MSG_RX_TYPE_s* can_RxMsgs, uint8_t numberOfRxMsgs);

/* Interrupts */
static void CAN_Disable_Transmit_IT(CAN_HandleTypeDef* ptrHcan);
static void CAN_TxCpltCallback(CAN_NodeTypeDef_e canNode);
static void CAN_ErrorCallback(CAN_HandleTypeDef* ptrHhcan);
static STD_RETURN_TYPE_e CAN_RxMsg(CAN_NodeTypeDef_e canNode, CAN_HandleTypeDef* ptrHcan, uint8_t FIFONumber);

/* Buffer/Interpreter */
static STD_RETURN_TYPE_e CAN_BufferBypass(CAN_NodeTypeDef_e canNode, uint32_t msgID, uint8_t* rxData, uint8_t DLC,
        uint8_t RTR);
static STD_RETURN_TYPE_e CAN_InterpretReceivedMsg(CAN_NodeTypeDef_e canNode, uint32_t msgID, uint8_t* data, uint8_t DLC,
        uint8_t RTR);

/*================== Function Implementations =============================*/

/* ***************************************
 *  Initialization
 ****************************************/

uint32_t CAN_Init(void) {

    uint32_t retval = 0;

#if CAN_USE_CAN_NODE0
    /* DeInit CAN0 handle */
    if (HAL_CAN_DeInit(&hcan0) != HAL_OK) {
        /* Error deintializing handle -> set error bit */
        retval |= STD_ERR_BIT_0;
    }

    /* Init CAN0-handle */
    if (HAL_CAN_Init(&hcan0) != HAL_OK) {
        /* Error intializing handle -> set error bit */
        retval |= STD_ERR_BIT_1;
    }
    hcan0.pRxMsg = &sReceiveStruct0;    /* default message for handle */

    /* Configure CAN0 hardware filter */
    retval |= CAN_InitFilter(&hcan0, &can0_RxMsgs[0], can_CAN0_rx_length);

    /* Check if more rx messages are bypassed than received */
    if (CAN0_BUFFER_BYPASS_NUMBER_OF_IDs > can_CAN0_rx_length) {
        retval |= STD_ERR_BIT_7;
    }

    /* Enable CAN0 message receive interrupt FIFO0 */
    if (HAL_CAN_Receive_IT(&hcan0, CAN_FIFO0) != HAL_OK) {
        retval |= STD_ERR_BIT_8;
    }
    hcan0.State = HAL_CAN_STATE_READY;
    /* Enable CAN0 message receive interrupt FIFO1 */
    if (HAL_CAN_Receive_IT(&hcan0, CAN_FIFO1) != HAL_OK) {
        retval |= STD_ERR_BIT_9;
    }

    /* set DBF bit to 0 for CAN activity while in debug mode */
    __HAL_CAN_DBG_FREEZE(&hcan0, DISABLE);
#if CAN_USE_STANDBY_CONTROL == 1
    IO_WritePin(CAN_0_TRANS_STANDBY_CONTROL, IO_PIN_SET);
#endif
#endif


#if CAN_USE_CAN_NODE1
    /* DeInit CAN1 handle */
    if (HAL_CAN_DeInit(&hcan1) != HAL_OK) {
        /* Error deintializing handle -> set error bit */
        retval |= STD_ERR_BIT_10;
    }

    /* Init CAN1-handle */
    if (HAL_CAN_Init(&hcan1) != HAL_OK) {
        /* Error intializing handle -> set error bit */
        retval |= STD_ERR_BIT_11;
    }
    hcan1.pRxMsg = &sReceiveStruct1;    /* default message for handle */

    /* Configure CAN1 hardware filter */
    retval |= CAN_InitFilter(&hcan1, &can1_RxMsgs[0], can_CAN1_rx_length);

    /* Check if more RX messages are bypassed than received */
    if (CAN1_BUFFER_BYPASS_NUMBER_OF_IDs > can_CAN1_rx_length) {
        retval |= STD_ERR_BIT_12;
    }

    /* Enable CAN1 message receive interrupt FIFO0 */
    if (HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0) != HAL_OK) {
        retval |= STD_ERR_BIT_13;
    }
    /* Enable CAN1 message receive interrupt FIFO1 */
    hcan1.State = HAL_CAN_STATE_READY;
    if (HAL_CAN_Receive_IT(&hcan1, CAN_FIFO1) != HAL_OK) {
        retval |= STD_ERR_BIT_14;
    }

    /* set DBF bit to 0 for CAN activity while in debug mode */
    __HAL_CAN_DBG_FREEZE(&hcan1, DISABLE);
#if CAN_USE_STANDBY_CONTROL == 1
    IO_WritePin(CAN_1_TRANS_STANDBY_CONTROL, IO_PIN_SET);
#endif
#endif
    return retval;
}

/**
 * @brief  Initializes message filtering
 * @retval 0: if no error occurred, otherwise error code
 */
static uint32_t CAN_InitFilter(CAN_HandleTypeDef* ptrHcan, CAN_MSG_RX_TYPE_s* can_RxMsgs, uint8_t numberOfRxMsgs) {
    /* Contains the occurrence of of the different filter cases     *
     * [0] - ID List mode 16bit routed on FIFO0                     *
     * [1] - ID List mode 16bit routed on FIFO1                     *
     * [2] - ID List mode 32bit routed on FIFO0                     *
     * [3] - ID List mode 32bit routed on FIFO1                     *
     * [4] - Mask mode 16bit routed on FIFO0                        *
     * [5] - Mask mode 16bit routed on FIFO1                        *
     * [6] - Mask mode 32bit                                        */
    uint8_t numberOfDifferentIDs[7] = { 0, 0, 0, 0, 0, 0, 0 };
    static uint8_t filterNumber = 0;   /* Number of the filter to be initialized */
    uint32_t retval = 0;

    /* Calculate number of needed filter banks */
    uint8_t numberNeededFilters = CAN_NumberOfNeededFilters(can_RxMsgs, &numberOfDifferentIDs[0], &retval);

    if (numberNeededFilters <= CAN_NUMBER_OF_FILTERBANKS) {
        uint8_t j = 0;  /* Counts the number of initialized instances per case */
        uint8_t posRxMsgs = 0;  /* Iterator for can_RxMsgs[] */
        uint8_t numberRegistersUsed = 0;  /* Counts how many register space is already used in each filter bank (max. 64bit) */
        uint8_t caseID = 0;  /* indicates the actual filter mode that will be initialized */

        if (ptrHcan->Instance  ==  CAN2) {
            /* Set start slave bank filter */
            sFilterConfig.BankNumber = filterNumber;
        }
        for (caseID = 0; caseID < 2; caseID++) {
            /* ID List mode 16bit routed on FIFO0 or FIFO1 */

            if (numberOfDifferentIDs[caseID] > 0) {
                j = 0;
                while (j < numberOfDifferentIDs[caseID]) {
                    /* Until all IDs in that filter case are treated */

                    posRxMsgs = CAN_GetNextID(can_RxMsgs, numberOfRxMsgs, posRxMsgs, caseID);  /*  Get array position of next ID */

                    switch (numberRegistersUsed) {
                        case 0:  /* 1st ID per filter bank */
                            sFilterConfig.FilterIdHigh = ((can_RxMsgs[posRxMsgs].ID << 5)
                                    | can_RxMsgs[posRxMsgs].RTR << 4);
                            j++;
                            break;

                        case 1:  /* 2nd ID */
                            sFilterConfig.FilterIdLow = ((can_RxMsgs[posRxMsgs].ID << 5)
                                    | can_RxMsgs[posRxMsgs].RTR << 4);
                            j++;
                            break;

                        case 2:  /* 3rd ID */
                            sFilterConfig.FilterMaskIdHigh = ((can_RxMsgs[posRxMsgs].ID << 5)
                                    | can_RxMsgs[posRxMsgs].RTR << 4);
                            j++;
                            break;

                        case 3:  /* 4th ID */
                            sFilterConfig.FilterMaskIdLow = ((can_RxMsgs[posRxMsgs].ID << 5)
                                    | can_RxMsgs[posRxMsgs].RTR << 4);
                            j++;
                            break;
                    }
                    numberRegistersUsed = j % 4;    /* space for 4 IDs a 16 bit in one filter bank */
                    if ((numberRegistersUsed  ==  0 && j > 1) || j  ==  numberOfDifferentIDs[caseID]) {
                        /* all registers in filter bank used OR no more IDs in that case */
                        sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
                        sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
                        if (caseID  ==  ID_16BIT_FIFO0) {
                            sFilterConfig.FilterFIFOAssignment = CAN_FIFO0;
                        } else if (caseID  ==  ID_16BIT_FIFO1) {
                            sFilterConfig.FilterFIFOAssignment = CAN_FIFO1;
                        }
                        sFilterConfig.FilterNumber = filterNumber;
                        HAL_CAN_ConfigFilter(ptrHcan, &sFilterConfig);    /* initialize filter bank */
                        filterNumber++;     /* increment filter number */
                    }
                    posRxMsgs++;    /* increment array position to find next valid ID */
                }
                posRxMsgs = 0;      /* reset variables for next case */
                numberRegistersUsed = 0;
            }
        }
        for (caseID = 2; caseID < 6; caseID++) {
            /* ID List mode 32bit routed on FIFO0 or FIFO1; Mask mode 16bit routed on FIFO0 or FIFO1 */
            j = 0;
            if (numberOfDifferentIDs[caseID] > 0) {
                while (j < numberOfDifferentIDs[caseID]) {
                    /* Until all IDs in that filter case are treated */

                    posRxMsgs = CAN_GetNextID(can_RxMsgs, numberOfRxMsgs, posRxMsgs, caseID);  /*  Get array position of next ID */

                    switch (numberRegistersUsed) {
                        case 0:  /* first 32bit per filter bank */
                            if (caseID  ==  ID_32BIT_FIFO0 || caseID  ==  ID_32BIT_FIFO1) {   /* list mode 32bit */

                                sFilterConfig.FilterIdHigh = ((can_RxMsgs[posRxMsgs].ID << 3) >> 16);  /* 1 << 2 is for setting IDE bit to receive extended identifiers */
                                sFilterConfig.FilterIdLow = (uint16_t)((can_RxMsgs[posRxMsgs].ID << 3) | 1 << 2
                                        | can_RxMsgs[posRxMsgs].RTR << 1);
                            } else if (caseID  ==  MSK_16BIT_FIFO0 || caseID  ==  MSK_16BIT_FIFO1) {  /* mask mode 16bit */

                                sFilterConfig.FilterIdHigh = ((can_RxMsgs[posRxMsgs].ID << 5)
                                        | can_RxMsgs[posRxMsgs].RTR << 4);
                                sFilterConfig.FilterMaskIdHigh = can_RxMsgs[posRxMsgs].mask;
                                sFilterConfig.FilterIdLow = 0x0000;      /* set second register to 0xFFFF, */
                                sFilterConfig.FilterMaskIdLow = 0xFFFF;  /* otherwise all messages would be received */
                            }
                            j++;
                            break;

                        case 1:  /* second 32bit per filter bank */
                            if (caseID  ==  ID_32BIT_FIFO0 || caseID  ==  ID_32BIT_FIFO1) {   /* list mode 32bit */

                                sFilterConfig.FilterMaskIdHigh = ((can_RxMsgs[posRxMsgs].ID << 3) >> 16);  /*  1 << 2 is for setting IDE bit to receive extended identifiers */
                                sFilterConfig.FilterMaskIdLow = (uint16_t)((can_RxMsgs[posRxMsgs].ID << 3) | 1 << 2
                                        | can_RxMsgs[posRxMsgs].RTR << 1);
                            } else if (caseID  ==  MSK_16BIT_FIFO0 || caseID  ==  MSK_16BIT_FIFO1) {  /* mask mode 16bit */

                                sFilterConfig.FilterIdLow = ((can_RxMsgs[posRxMsgs].ID << 5)
                                        | can_RxMsgs[posRxMsgs].RTR << 4);
                                sFilterConfig.FilterMaskIdLow = can_RxMsgs[posRxMsgs].mask;
                            }
                            j++;
                            break;
                    }
                    numberRegistersUsed = j % 2;    /* Space for two IDs a 32bit or two mask a 16bit */
                    if ((numberRegistersUsed  ==  0 && j > 1) || j  ==  numberOfDifferentIDs[caseID]) {
                        /* all registers in filter bank used OR no more IDs in that case */
                        if (caseID  ==  ID_32BIT_FIFO0 || caseID  ==  ID_32BIT_FIFO1) {
                            sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
                            sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
                            if (caseID  ==  ID_32BIT_FIFO0)
                                sFilterConfig.FilterFIFOAssignment = CAN_FIFO0;
                            else
                                sFilterConfig.FilterFIFOAssignment = CAN_FIFO1;
                        } else if (caseID  ==  MSK_16BIT_FIFO0 || caseID  ==  MSK_16BIT_FIFO1) {
                            sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
                            sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
                            if (caseID  ==  MSK_16BIT_FIFO0)
                                sFilterConfig.FilterFIFOAssignment = CAN_FIFO0;
                            else
                                sFilterConfig.FilterFIFOAssignment = CAN_FIFO1;
                        }
                        sFilterConfig.FilterNumber = filterNumber;
                        HAL_CAN_ConfigFilter(ptrHcan, &sFilterConfig);    /* initialize filter bank */
                        filterNumber++;     /* increment filter number */
                    }
                    posRxMsgs++;    /* increment array position to find next valid ID */
                }
                posRxMsgs = 0;      /* reset variables for next case */
                numberRegistersUsed = 0;
            }
        }
        j = 0;
        if (numberOfDifferentIDs[MSK_32BIT] > 0) {
            /* Mask mode 32bit */

            while (j < numberOfDifferentIDs[MSK_32BIT]) {    /*  Get array position of next ID */
                /* Until all IDs in that filter case are treated */
                posRxMsgs = CAN_GetNextID(can_RxMsgs, numberOfRxMsgs, posRxMsgs, MSK_32BIT);

                sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
                sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
                sFilterConfig.FilterIdHigh = ((can_RxMsgs[posRxMsgs].ID << 3) >> 16);  /* 1 << 2 is for setting IDE bit to receive extended identifiers */
                sFilterConfig.FilterIdLow = (uint16_t)((can_RxMsgs[posRxMsgs].ID << 3) | 1 << 2
                        | can_RxMsgs[posRxMsgs].RTR << 1);
                sFilterConfig.FilterMaskIdHigh = can_RxMsgs[posRxMsgs].mask >> 16;
                sFilterConfig.FilterMaskIdLow = (uint16_t)(can_RxMsgs[posRxMsgs].mask);
                sFilterConfig.FilterFIFOAssignment = can_RxMsgs[posRxMsgs].fifo;
                sFilterConfig.FilterNumber = filterNumber;
                HAL_CAN_ConfigFilter(ptrHcan, &sFilterConfig);
                filterNumber++;
                posRxMsgs++;
                j++;
            }
        }
    } else {
        /* Too many filterbanks needed! Check the value of CAN_NUMBER_OF_FILTERBANKS */
        /* If correct, try to reduce the IDs through masks or optimize used filter bank space. */
        /* Number of different filter cases can be evaluated in numberOfDifferentIDs[]. One */
        /* filter bank can filter as many messages as followed: */
        /* 4 IDs in list mode 16bit */
        /* 2 IDs in list mode 32bit and mask mode 16bit */
        /* 1 ID in 32bit mask mode */
        retval |= STD_ERR_BIT_6;
    }
    return retval;
}

/**
 * @brief  Returns the number of filters that have to be initialized
 *
 * @param can_RxMsgs:              pointer to receive message struct
 * @param numberOfDifferentIDs:    pointer to array, where to store the specific number of different IDs
 *
 * @retval number of needed filters
 */
static uint8_t CAN_NumberOfNeededFilters(CAN_MSG_RX_TYPE_s* can_RxMsgs, uint8_t* numberOfDifferentIDs, uint32_t* error) {
    static uint8_t retVal = 0;      /* static so save the number of filters from CAN0 and add to the ones from CAN1 */
    uint16_t can_rx_length = 0;

    if (can_RxMsgs  ==  &can0_RxMsgs[0]) {
        can_rx_length = can_CAN0_rx_length;
    } else if (can_RxMsgs  ==  &can1_RxMsgs[0]) {
        can_rx_length = can_CAN1_rx_length;
    } else {
        can_rx_length = 0;
    }

    for (int i = 0; i < can_rx_length; i++) {

#if CAN0_BUFFER_BYPASS_NUMBER_OF_IDs > 0 && CAN_USE_CAN_NODE0 == 1

        if (can_RxMsgs  ==  &can0_RxMsgs[0]) {
            /* Set buffer bypass IDs link table */
            for (int k = 0; k < CAN0_BUFFER_BYPASS_NUMBER_OF_IDs; k++) {
                if (can_RxMsgs[i].ID  ==  can0_bufferBypass_RxMsgs[k]) {
                    /* bypass ID  ==  ID in message receive struct */

                    can0_fastLinkIndex[k] = i;   /* set for can_bufferBypass_RxMsgs[k] link to array index */
                    break;
                }
            }
        }
#endif
#if CAN1_BUFFER_BYPASS_NUMBER_OF_IDs > 0 && CAN_USE_CAN_NODE1 == 1

        if (can_RxMsgs  ==  &can1_RxMsgs[0]) {
            /* Set buffer bypass IDs link table */
            for (int k = 0; k < CAN1_BUFFER_BYPASS_NUMBER_OF_IDs; k++) {
                if (can1_RxMsgs[i].ID  ==  can1_bufferBypass_RxMsgs[k]) {
                    /* bypass ID  ==  ID in message receive struct */

                    can1_fastLinkIndex[k] = i;   /* set for can1_bufferBypass_RxMsgs[k] link to array index */
                    break;
                }
            }
        }
#endif
        if (can_RxMsgs[i].mask  ==  0 && IS_CAN_STDID(can_RxMsgs[i].ID)) {

            /* ID List mode 16bit */

            if (can_RxMsgs[i].fifo  ==  CAN_FIFO0) {
                numberOfDifferentIDs[ID_16BIT_FIFO0]++;
            } else if (can_RxMsgs[i].fifo  ==  CAN_FIFO1) {
                numberOfDifferentIDs[ID_16BIT_FIFO1]++;
            } else {
                /* Invalid FIFO selection; check can_RxMsgs[i].fifo value */
                *error |= STD_ERR_BIT_2;
            }
        } else if (can_RxMsgs[i].mask  ==  0 && IS_CAN_EXTID(can_RxMsgs[i].ID)) {
            /* ID List mode 32bit */

            if (can_RxMsgs[i].fifo  ==  CAN_FIFO0) {
                numberOfDifferentIDs[ID_32BIT_FIFO0]++;
            } else if (can_RxMsgs[i].fifo  ==  CAN_FIFO1) {
                numberOfDifferentIDs[ID_32BIT_FIFO1]++;
            } else {
                /* Invalid FIFO selection; check can_RxMsgs[i].fifo value */
                *error |= STD_ERR_BIT_3;
            }
        } else if (can_RxMsgs[i].mask > 0 && IS_CAN_STDID(can_RxMsgs[i].ID)) {
            /* Mask mode 16bit */

            if (can_RxMsgs[i].fifo  ==  CAN_FIFO0) {
                numberOfDifferentIDs[MSK_16BIT_FIFO0]++;
            } else if (can_RxMsgs[i].fifo  ==  CAN_FIFO1) {
                numberOfDifferentIDs[MSK_16BIT_FIFO1]++;
            } else {
                /* Invalid FIFO selection; check can_RxMsgs[i].fifo value */
                *error |= STD_ERR_BIT_4;
            }
        } else if (can_RxMsgs[i].mask > 0 && IS_CAN_EXTID(can_RxMsgs[i].ID)) {
            /* Mask mode 32bit */

            numberOfDifferentIDs[MSK_32BIT]++;
        } else {
            /* Invalid ID > IS_CAN_EXTID; check can_RxMsgs[i].ID value */
            *error |= STD_ERR_BIT_5;
            break;
        }
    }
    for (int i = 0; i < 2; i++) {
        if (numberOfDifferentIDs[i] > 0) {
            retVal += (numberOfDifferentIDs[i] + 2) / 4;    /* 4 IDs per filter; rounding up */
        }
    }
    for (int i = 2; i < 6; i++) {
        if (numberOfDifferentIDs[i] > 0) {
            retVal += (numberOfDifferentIDs[i] + 1) / 2;   /* 2 IDs per filter; rounding up */
        }
    }
    if (numberOfDifferentIDs[MSK_32BIT] > 0) {
        retVal += numberOfDifferentIDs[6];                  /* 1 ID per filter */
    }
    return retVal;
}

/**
 * @brief  Returns the next index of wished filter ID setting in CAN_MSG_RX_TYPE_t can_RxMsgs[CAN_NUMBER_OF_RX_IDs]
 *
 * @param can_RxMsgs:     pointer to receive message struct
 * @param numberOfRxIDs:  count of that type of receive message in can_RxMsgs struct
 * @param startIndex:     index where to start searching
 * @param filterCase:     specifies the object what will be found
 *
 * @retval returns index
 */
static uint8_t CAN_GetNextID(CAN_MSG_RX_TYPE_s* can_RxMsgs, uint8_t numberOfRxIDs, uint8_t startIndex,
        uint8_t filterCase) {
    uint8_t retVal = 0;
    uint8_t i = startIndex;
    while (i < numberOfRxIDs) {
        if (filterCase  ==  ID_16BIT_FIFO0 && can_RxMsgs[i].mask == 0&& IS_CAN_STDID(can_RxMsgs[i].ID) && can_RxMsgs[i].fifo == CAN_FIFO0) {
            retVal = i;
            break;
        } else if (filterCase == ID_16BIT_FIFO1 && can_RxMsgs[i].mask == 0&& IS_CAN_STDID(can_RxMsgs[i].ID) && can_RxMsgs[i].fifo == CAN_FIFO1) {
            retVal = i;
            break;
        } else if (filterCase == ID_32BIT_FIFO0 && can_RxMsgs[i].mask == 0&& !IS_CAN_STDID(can_RxMsgs[i].ID) && IS_CAN_EXTID(can_RxMsgs[i].ID) && can_RxMsgs[i].fifo == CAN_FIFO0) {
            retVal = i;
            break;
        } else if (filterCase == ID_32BIT_FIFO1 && can_RxMsgs[i].mask == 0&& !IS_CAN_STDID(can_RxMsgs[i].ID) && IS_CAN_EXTID(can_RxMsgs[i].ID) && can_RxMsgs[i].fifo == CAN_FIFO1) {
            retVal = i;
            break;
        } else if (filterCase == MSK_16BIT_FIFO0 && can_RxMsgs[i].mask > 0&& IS_CAN_STDID(can_RxMsgs[i].ID) && can_RxMsgs[i].fifo == CAN_FIFO0) {
            retVal = i;
            break;
        } else if (filterCase == MSK_16BIT_FIFO1 && can_RxMsgs[i].mask > 0&& IS_CAN_STDID(can_RxMsgs[i].ID) && can_RxMsgs[i].fifo == CAN_FIFO1) {
            retVal = i;
            break;
        } else if (filterCase == MSK_32BIT && can_RxMsgs[i].mask > 0&& !IS_CAN_STDID(can_RxMsgs[i].ID) && IS_CAN_EXTID(can_RxMsgs[i].ID)) {
            retVal = i;
            break;
        }
        i++;
    }

    return retVal;
}

/* ***************************************
 *  Interrupt handling
 ****************************************/

void CAN_TX_IRQHandler(CAN_HandleTypeDef* ptrHcan) {

    /* Check End of transmission flag */
    if (__HAL_CAN_GET_IT_SOURCE(ptrHcan, CAN_IT_TME)) {
        if ((__HAL_CAN_TRANSMIT_STATUS(ptrHcan, CAN_TXMAILBOX_0))
                || (__HAL_CAN_TRANSMIT_STATUS(ptrHcan, CAN_TXMAILBOX_1))
                || (__HAL_CAN_TRANSMIT_STATUS(ptrHcan, CAN_TXMAILBOX_2))) {
            /* Call transmit function */
            CAN_Disable_Transmit_IT(ptrHcan);
        }
    }
}


void CAN_RX_IRQHandler(CAN_NodeTypeDef_e canNode, CAN_HandleTypeDef* ptrHcan) {
    /* Check End of reception flag for FIFO0 */
    if ((__HAL_CAN_GET_IT_SOURCE(ptrHcan, CAN_IT_FMP0)) && (__HAL_CAN_MSG_PENDING(ptrHcan, CAN_FIFO0) != 0)) {
        /* Call receive function */
        CAN_RxMsg(canNode, ptrHcan, CAN_FIFO0);        /* change towards HAL_CAN_IRQHandler */
    }

    /* Check End of reception flag for FIFO1 */
    if ((__HAL_CAN_GET_IT_SOURCE(ptrHcan, CAN_IT_FMP1)) && (__HAL_CAN_MSG_PENDING(ptrHcan, CAN_FIFO1) != 0)) {
        /* Call receive function */
        CAN_RxMsg(canNode, ptrHcan, CAN_FIFO1);        /* change towards HAL_CAN_IRQHandler */
    }
}

void CAN_Error_IRQHandler(CAN_NodeTypeDef_e canNode, CAN_HandleTypeDef* ptrHcan) {
    CAN_ERROR_s* errorStruct;

    if (canNode == CAN_NODE1) {
#if CAN_USE_CAN_NODE1 == 1
        errorStruct = &CAN1_errorStruct;
#endif
    } else {
#if CAN_USE_CAN_NODE0 == 1
        errorStruct = &CAN0_errorStruct;
#endif
    }
    /* Check Error Warning Flag */
    if ((__HAL_CAN_GET_FLAG(ptrHcan, CAN_FLAG_EWG)) && (__HAL_CAN_GET_IT_SOURCE(ptrHcan, CAN_IT_EWG))
            && (__HAL_CAN_GET_IT_SOURCE(ptrHcan, CAN_IT_ERR))) {
        /* Set CAN error code to EWG error */        /* This bit is set by hardware when the warning limit has been    */
        ptrHcan->ErrorCode |= HAL_CAN_ERROR_EWG;     /* reached (Receive Error Counter or Transmit Error Counter>=96)  */
        errorStruct->canErrorCounter[0]++;           /* until error counter 127 write error frames dominant on can bus */
                                                     /* increment error occurrence of error warning state              */
        /* Clear Error Warning Flag */
        __HAL_CAN_CLEAR_FLAG(ptrHcan, CAN_FLAG_EWG);
    }

    /* Check Error Passive Flag */
    if ((__HAL_CAN_GET_FLAG(ptrHcan, CAN_FLAG_EPV)) && (__HAL_CAN_GET_IT_SOURCE(ptrHcan, CAN_IT_EPV))
            && (__HAL_CAN_GET_IT_SOURCE(ptrHcan, CAN_IT_ERR))) {
        /* Set CAN error code to EPV error */       /* This bit is set by hardware when the Error Passive limit has been */
        ptrHcan->ErrorCode |= HAL_CAN_ERROR_EPV;    /* reached (Receive Error Counter or Transmit Error Counter>127)     */
        errorStruct->canErrorCounter[1]++;          /* write error frames recessive on can bus                           */
                                                    /* increment error occurrence of error passive state                 */
        /* Clear Error Passive Flag */
        __HAL_CAN_CLEAR_FLAG(ptrHcan, CAN_FLAG_EPV);
    }

    /* Check Bus-Off Flag */
    if ((__HAL_CAN_GET_FLAG(ptrHcan, CAN_FLAG_BOF)) && (__HAL_CAN_GET_IT_SOURCE(ptrHcan, CAN_IT_BOF))
            && (__HAL_CAN_GET_IT_SOURCE(ptrHcan, CAN_IT_ERR))) {
        /* Set CAN error code to BOF error */      /* This bit is set by hardware when it enters the bus-off state. The */
        ptrHcan->ErrorCode |= HAL_CAN_ERROR_BOF;   /* bus-off state is entered on TEC overflow, greater than 255        */
        errorStruct->canErrorCounter[2]++;         /* increment error occurrence of bus-off state                       */

        /* Clear Bus-Off Flag */
        __HAL_CAN_CLEAR_FLAG(ptrHcan, CAN_FLAG_BOF);
    }

    /* Check Last error code Flag */
    if ((!HAL_IS_BIT_CLR(ptrHcan->Instance->ESR, CAN_ESR_LEC)) && (__HAL_CAN_GET_IT_SOURCE(ptrHcan, CAN_IT_LEC))
            && (__HAL_CAN_GET_IT_SOURCE(ptrHcan, CAN_IT_ERR))) {
        switch (ptrHcan->Instance->ESR & CAN_ESR_LEC) {
            case (CAN_ESR_LEC_0):                         /* STUFF ERROR --- When five consecutive bits of the same level have been transmitted by a node, it will add a */
                /* Set CAN error code to STF error */     /* sixth bit of the opposite level to the outgoing bit stream. The receivers will remove  this extra bit.This is */
                ptrHcan->ErrorCode |= HAL_CAN_ERROR_STF;  /* done to avoid excessive DC components on the bus, but it also gives the receivers an extra opportunity to detect */
                                                          /* errors: if more than five consecutive bits of the same level occurs on the bus, a Stuff Error is signaled. */
                errorStruct->canErrorCounter[3]++;        /* increment error occurrence of stuff error */
                break;

            case (CAN_ESR_LEC_1):                         /* FORM ERROR --- Some parts of the CAN message have a fixed format, i.e. the standard defines exactly what levels */
                /* Set CAN error code to FOR error */     /* must occur and when. (Those parts are the CRC Delimiter, ACK Delimiter, End of Frame, and also the Intermission,*/
                ptrHcan->ErrorCode |= HAL_CAN_ERROR_FOR;  /* but there are some extra special error checking rules for that.) If a CAN controller detects an invalid value */
                                                          /* in one of these fixed fields, a Form Error is signaled. */
                errorStruct->canErrorCounter[4]++;        /* increment error occurrence of form error */
                break;

            case (CAN_ESR_LEC_1 | CAN_ESR_LEC_0):         /* ACKNOWLEDGMENT ERROR --- All nodes on the bus that correctly receives a message (regardless of their being */
                /* Set CAN error code to ACK error */     /* interested of its contents or not) are expected to send a dominant level in the so-called Acknowledgement*/
                ptrHcan->ErrorCode |= HAL_CAN_ERROR_ACK;  /* Slot in the message. The transmitter will transmit a recessive level here. If the transmitter can detect a */
                                                          /* dominant level in the ACK slot, an Acknowledgement Error is signaled. */
                errorStruct->canErrorCounter[5]++;        /* increment error occurrence of acknowledgment error */
                break;

            case (CAN_ESR_LEC_2):                        /* BIT RECESSIVE ERROR --- Each transmitter on the CAN bus monitors (i.e. reads back) the */
                /* Set CAN error code to BR error */     /* transmitted signal level. If the bit level actually read differs from the one transmitted, */
                ptrHcan->ErrorCode |= HAL_CAN_ERROR_BR;  /* a Bit Error (No bit error is raised during the arbitration process.) */
                errorStruct->canErrorCounter[6]++;       /* increment error occurrence of bit recessive error */
                break;

            case (CAN_ESR_LEC_2 | CAN_ESR_LEC_0):        /* BIT DOMINANT ERROR --- Each transmitter on the CAN bus monitors (i.e. reads back) the */
                /* Set CAN error code to BD error */     /* transmitted signal level. If the bit level actually read differs from the one transmitted, */
                ptrHcan->ErrorCode |= HAL_CAN_ERROR_BD;  /* a Bit Error (No bit error is raised during the arbitration process.) */
                errorStruct->canErrorCounter[7]++;       /* increment error occurrence of bit dominant error */
                break;

            case (CAN_ESR_LEC_2 | CAN_ESR_LEC_1):       /* CRC ERROR --- Each message features a 15-bit Cyclic Redundancy Checksum (CRC), and any node that detects */
                /* Set CAN error code to CRC error */   /* a different CRC in the message than what it has calculated itself will signal an CRC Error. */
                ptrHcan->ErrorCode |= HAL_CAN_ERROR_CRC;
                errorStruct->canErrorCounter[8]++;      /* increment error occurrence of crc error */
                break;

            default:
                break;
        }

        /* Clear Last error code Flag */
        ptrHcan->Instance->ESR &= ~(CAN_ESR_LEC);
    }

    /* Call Error callback function */
    if (ptrHcan->ErrorCode != HAL_CAN_ERROR_NONE) {

        /* Set last error code in CAN_errorStruct */
        errorStruct->canError = ptrHcan->ErrorCode;

        CAN_ErrorCallback(ptrHcan);
    }

    /* Disable SCE Interrupts */
    __HAL_CAN_DISABLE_IT(ptrHcan, CAN_IT_BOF);
    __HAL_CAN_DISABLE_IT(ptrHcan, CAN_IT_WKU);
    __HAL_CAN_DISABLE_IT(ptrHcan, CAN_IT_SLK);
    __HAL_CAN_DISABLE_IT(ptrHcan, CAN_IT_EWG);
    __HAL_CAN_DISABLE_IT(ptrHcan, CAN_IT_EPV);
    __HAL_CAN_DISABLE_IT(ptrHcan, CAN_IT_LEC);
    __HAL_CAN_DISABLE_IT(ptrHcan, CAN_IT_ERR);

    /* Set CAN error code to none */
    ptrHcan->ErrorCode = HAL_CAN_ERROR_NONE;

    /* Set the CAN state ready to be able to start again the process */
    ptrHcan->State = HAL_CAN_STATE_READY;
}

/**
 * @brief  Disables transmit mailbox empty interrupt and calls callback
 * functions for transmitting messages from buffer
 * @retval E_OK if transmission successful, otherwise E_NOT_OK
 */
static void CAN_Disable_Transmit_IT(CAN_HandleTypeDef* ptrHcan) {
    /* Disable Transmit mailbox empty Interrupt */
    __HAL_CAN_DISABLE_IT(ptrHcan, CAN_IT_TME);

    if (ptrHcan->State  ==  HAL_CAN_STATE_BUSY_TX_RX) {
        /* Change CAN state */
        ptrHcan->State = HAL_CAN_STATE_BUSY_RX;
    } else {
        /* Change CAN state */
        ptrHcan->State = HAL_CAN_STATE_READY;
    }

#if CAN0_USE_TX_BUFFER
    if (ptrHcan->Instance  ==  CAN2) {
        CAN_TxCpltCallback(CAN_NODE0);
    }
#endif
#if CAN1_USE_TX_BUFFER
    /* No need for callback, if no buffer is used */
    if (ptrHcan->Instance  ==  CAN1) {
        /* Transmission complete callback */
        CAN_TxCpltCallback(CAN_NODE1);

    }
#endif
}

/**
 * @brief  Transmission complete callback in non blocking mode
 *
 * @param  canNode: canNode that transmitted a message
 *
 * @retval none (void)
 */
static void CAN_TxCpltCallback(CAN_NodeTypeDef_e canNode) {
    STD_RETURN_TYPE_e retVal = E_NOT_OK;

    CAN_TX_BUFFER_s* can_txbuffer = NULL;

    if (canNode  ==  CAN_NODE0) {
#if CAN_USE_CAN_NODE0 == 1
        can_txbuffer = &can0_txbuffer;
#endif
    } else if (canNode  ==  CAN_NODE1) {
#if CAN_USE_CAN_NODE1 == 1
        can_txbuffer = &can1_txbuffer;
#endif
    }
    if (can_txbuffer != NULL) {
        /* No Error during start of transmission */
        if ((can_txbuffer->ptrWrite  ==  can_txbuffer->ptrRead)
                && (can_txbuffer->buffer[can_txbuffer->ptrRead].newMsg  ==  0)) {
            /* nothing to transmit, buffer is empty */
            retVal = E_NOT_OK;
        } else {
            retVal = CAN_TxMsgBuffer(canNode);
            if (retVal  ==  E_OK) {

            } else {
                retVal = E_NOT_OK;        /* Error during transmission, retransmit message later */
            }
        }
    } else {
        /* no transmit buffer active */
        retVal = E_NOT_OK;
    }
}

/**
 * @brief  Error occured callback
 * @param  ptrHcan: pointer to a CAN_HandleTypeDef structure that contains
 *         the configuration information for the specified CAN.
 * @retval None
 */
static void CAN_ErrorCallback(CAN_HandleTypeDef* ptrHcan) {

}

/* ***************************************
 *  Transmit message
 ****************************************/

STD_RETURN_TYPE_e CAN_TxMsg(CAN_NodeTypeDef_e canNode, uint32_t msgID, uint8_t* ptrMsgData, uint32_t msgLength,
        uint32_t RTR) {
    STD_RETURN_TYPE_e retVal = E_NOT_OK;
    CanTxMsgTypeDef Message;
    CAN_HandleTypeDef *ptrHcan;

    if (canNode  ==  CAN_NODE0) {

        if (canNode0_listenonly_mode)
            ptrHcan = NULL;
        else
            ptrHcan = &hcan0;
    } else if (canNode  ==  CAN_NODE1) {

        if (canNode1_listenonly_mode)
            ptrHcan = NULL;
        else
            ptrHcan = &hcan1;
    } else {
        ptrHcan = NULL;
    }

    if ((IS_CAN_STDID(msgID) || IS_CAN_EXTID(msgID)) && IS_CAN_DLC(msgLength) && ptrHcan != NULL) {
        if (IS_CAN_STDID(msgID)) {
            Message.StdId = msgID;
            Message.IDE = CAN_ID_STD;
        } else {
            Message.ExtId = msgID;
            Message.IDE = CAN_ID_EXT;
        }
        Message.DLC = msgLength;
        for (int i = 0; i < msgLength; i++) {
            /* copy message data in handle transmit structure */
            Message.Data[i] = ptrMsgData[i];
        }
        Message.RTR = RTR;
        ptrHcan->pTxMsg = &Message;
        retVal = HAL_CAN_Transmit_IT(ptrHcan);
    } else {
        retVal = E_NOT_OK;
    }

    return retVal;
}


STD_RETURN_TYPE_e CAN_Send(CAN_NodeTypeDef_e canNode, uint32_t msgID, uint8_t* ptrMsgData, uint32_t msgLength,
        uint32_t RTR) {
    STD_RETURN_TYPE_e retVal = E_NOT_OK;
    uint8_t tmptxbuffer_wr;

    CAN_TX_BUFFER_s* can_txbuffer = NULL;

    if (canNode  ==  CAN_NODE0) {
#if CAN_USE_CAN_NODE0 == 1
        can_txbuffer = &can0_txbuffer;
#endif
    } else if (canNode  ==  CAN_NODE1) {
#if CAN_USE_CAN_NODE1 == 1
        can_txbuffer = &can1_txbuffer;
#endif
    }

    if (can_txbuffer != NULL) {

        tmptxbuffer_wr = can_txbuffer->ptrWrite;

        if (tmptxbuffer_wr  ==  can_txbuffer->ptrRead) {
            if (can_txbuffer->buffer[tmptxbuffer_wr].newMsg  ==  0) {
                /* free buffer space for message */

                can_txbuffer->ptrWrite++;
                can_txbuffer->ptrWrite = can_txbuffer->ptrWrite % can_txbuffer->length;
                retVal = E_OK;
            } else {
                /* buffer full */
                retVal = E_NOT_OK;
            }
        } else {
            can_txbuffer->ptrWrite++;
            can_txbuffer->ptrWrite = can_txbuffer->ptrWrite % can_txbuffer->length;
            retVal = E_OK;
        }
    }


    if (retVal  ==  E_OK && (IS_CAN_STDID(msgID) || IS_CAN_EXTID(msgID)) && IS_CAN_DLC(msgLength)) {
        /* if buffer free and valid CAN identifier */

        can_txbuffer->buffer[tmptxbuffer_wr].newMsg = 1;
        if (IS_CAN_STDID(msgID)) {
            can_txbuffer->buffer[tmptxbuffer_wr].msg.StdId = msgID;
            can_txbuffer->buffer[tmptxbuffer_wr].msg.IDE = CAN_ID_STD;   /* don't use extended ID */
        } else {
            can_txbuffer->buffer[tmptxbuffer_wr].msg.ExtId = msgID;
            can_txbuffer->buffer[tmptxbuffer_wr].msg.IDE = CAN_ID_EXT;   /* use extended ID */
        }

        can_txbuffer->buffer[tmptxbuffer_wr].msg.RTR = RTR;
        can_txbuffer->buffer[tmptxbuffer_wr].msg.DLC = msgLength;   /* Data length of the frame that will be transmitted */

        /* copy message data in handle transmit structure */
        can_txbuffer->buffer[tmptxbuffer_wr].msg.Data[0] = ptrMsgData[0];
        can_txbuffer->buffer[tmptxbuffer_wr].msg.Data[1] = ptrMsgData[1];
        can_txbuffer->buffer[tmptxbuffer_wr].msg.Data[2] = ptrMsgData[2];
        can_txbuffer->buffer[tmptxbuffer_wr].msg.Data[3] = ptrMsgData[3];
        can_txbuffer->buffer[tmptxbuffer_wr].msg.Data[4] = ptrMsgData[4];
        can_txbuffer->buffer[tmptxbuffer_wr].msg.Data[5] = ptrMsgData[5];
        can_txbuffer->buffer[tmptxbuffer_wr].msg.Data[6] = ptrMsgData[6];
        can_txbuffer->buffer[tmptxbuffer_wr].msg.Data[7] = ptrMsgData[7];

        retVal = E_OK;
    } else {
        retVal = E_NOT_OK;
    }

    return retVal;
}


STD_RETURN_TYPE_e CAN_TxMsgBuffer(CAN_NodeTypeDef_e canNode) {
    STD_RETURN_TYPE_e retVal = E_NOT_OK;

    CAN_TX_BUFFER_s* can_txbuffer = NULL;
    CAN_HandleTypeDef* ptrHcan = NULL;

    if (canNode  ==  CAN_NODE0) {
#if CAN_USE_CAN_NODE0 == 1
        if (!canNode0_listenonly_mode) {

            can_txbuffer = &can0_txbuffer;
            ptrHcan = &hcan0;
        }
#endif
    } else if (canNode  ==  CAN_NODE1) {
#if CAN_USE_CAN_NODE1 == 1
        if (!canNode1_listenonly_mode) {

            can_txbuffer = &can1_txbuffer;
            ptrHcan = &hcan1;
        }
#endif
    }
    if (can_txbuffer != NULL) {
        if ((can_txbuffer->ptrWrite  ==  can_txbuffer->ptrRead)
                && (can_txbuffer->buffer[can_txbuffer->ptrRead].newMsg  ==  0)) {
            /* nothing to transmit, buffer is empty */
            retVal = E_NOT_OK;
        } else {
            ptrHcan->pTxMsg = &can_txbuffer->buffer[can_txbuffer->ptrRead].msg;
            retVal = HAL_CAN_Transmit_IT(ptrHcan);
            if (retVal  ==  E_OK) {
                /* No Error during start of transmission */
                can_txbuffer->buffer[can_txbuffer->ptrRead].newMsg = 0;    /* Msg is sent, set newMsg to 0, to allow writing of new data in buffer space */
                can_txbuffer->ptrRead++;
                can_txbuffer->ptrRead = can_txbuffer->ptrRead % can_txbuffer->length;

            } else {
                retVal = E_NOT_OK;        /* Error during transmission, retransmit message later */
            }
        }
    } else {
        /* no transmit buffer active */
        retVal = E_NOT_OK;
    }

    return retVal;
}

/* ***************************************
 *  Receive message
 ****************************************/

/**
 * @brief  Receives CAN messages and stores them either in RxBuffer or in hcan
 *
 * @param  canNode:    canNode which received the message
 * @param  ptrHcan:    pointer to a CAN_HandleTypeDef structure that contains
 *                     the message information of the specified CAN.
 * @param  FIFONumber: FIFO in which the message has been received
 * @retval none (void)
 */
static STD_RETURN_TYPE_e CAN_RxMsg(CAN_NodeTypeDef_e canNode, CAN_HandleTypeDef* ptrHcan, uint8_t FIFONumber) {
    STD_RETURN_TYPE_e retVal = E_NOT_OK;
    uint8_t bypassLinkIndex = 0;
    uint32_t msgID;

#if CAN0_USE_RX_BUFFER || CAN1_USE_RX_BUFFER
    uint32_t* can_bufferbypass_rxmsgs = NULL;
    uint32_t bufferbypasslength = 0;
    CAN_RX_BUFFER_s* can_rxbuffer = NULL;
    CAN_MSG_RX_TYPE_s* can_rxmsgs = NULL;
    uint8_t* can_fastLinkIndex = NULL;
#endif

    /* Set pointer on respective RxBuffer */
    if (canNode  ==  CAN_NODE1) {
#if CAN1_USE_RX_BUFFER && CAN_USE_CAN_NODE1 == 1
        can_rxbuffer = &can1_rxbuffer;
#if CAN1_BUFFER_BYPASS_NUMBER_OF_IDs > 0
        can_rxmsgs = &can1_RxMsgs[0];
        can_bufferbypass_rxmsgs = &can1_bufferBypass_RxMsgs[0];
        bufferbypasslength = CAN1_BUFFER_BYPASS_NUMBER_OF_IDs;
        can_fastLinkIndex = &can1_fastLinkIndex[0];
#endif
#endif
    } else if (canNode  ==  CAN_NODE0) {
#if CAN0_USE_RX_BUFFER && CAN_USE_CAN_NODE0 == 1
        can_rxbuffer = &can0_rxbuffer;
#if CAN0_BUFFER_BYPASS_NUMBER_OF_IDs > 0
        can_rxmsgs = &can0_RxMsgs[0];
        can_bufferbypass_rxmsgs = &can0_bufferBypass_RxMsgs[0];
        bufferbypasslength = CAN0_BUFFER_BYPASS_NUMBER_OF_IDs;
        can_fastLinkIndex = &can0_fastLinkIndex[0];
#endif
#endif
    }

    /* Get message ID */
    ptrHcan->pRxMsg->IDE = (uint8_t)0x04 & ptrHcan->Instance->sFIFOMailBox[FIFONumber].RIR;
    if (ptrHcan->pRxMsg->IDE  ==  CAN_ID_STD) {
        msgID = (uint32_t)0x000007FF & (ptrHcan->Instance->sFIFOMailBox[FIFONumber].RIR >> 21);
    } else {
        msgID = (uint32_t)0x1FFFFFFF & (ptrHcan->Instance->sFIFOMailBox[FIFONumber].RIR >> 3);
    }

#if CAN1_BUFFER_BYPASS_NUMBER_OF_IDs > 0 || CAN0_BUFFER_BYPASS_NUMBER_OF_IDs > 0
    if (can_bufferbypass_rxmsgs != NULL) {
        /* only needed when messages are bypassed */

        for (bypassLinkIndex = 0; bypassLinkIndex < bufferbypasslength; bypassLinkIndex++) {
            if (msgID  ==  can_bufferbypass_rxmsgs[bypassLinkIndex]) {
                break;
            }
        }
    }
#endif
    if (bypassLinkIndex >= bufferbypasslength && can_rxbuffer != NULL) {
        /* ##### Use buffer / Copy data in buffer ##### */

#if CAN0_USE_RX_BUFFER || CAN1_USE_RX_BUFFER
        /* NO NEED TO DISABLE INTERRUPTS, BECAUSE FUNCTION IS CALLED FROM ISR */

        /* Set to 1 to mark message as new received. Set to 0 when reading message from buffer */
        can_rxbuffer->buffer[can_rxbuffer->ptrWrite].newMsg = 1;

        /* Get message ID */
        can_rxbuffer->buffer[can_rxbuffer->ptrWrite].ID = msgID;
        can_rxbuffer->buffer[can_rxbuffer->ptrWrite].RTR = (uint8_t)0x02
                & ptrHcan->Instance->sFIFOMailBox[FIFONumber].RIR;

        /* Get the DLC */
        can_rxbuffer->buffer[can_rxbuffer->ptrWrite].DLC = (uint8_t)0x0F
                & ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDTR;

        /* Get the data field */
        can_rxbuffer->buffer[can_rxbuffer->ptrWrite].Data[0] = (uint8_t)0xFF
                & ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDLR;
        can_rxbuffer->buffer[can_rxbuffer->ptrWrite].Data[1] = (uint8_t)0xFF
                & (ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 8);
        can_rxbuffer->buffer[can_rxbuffer->ptrWrite].Data[2] = (uint8_t)0xFF
                & (ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 16);
        can_rxbuffer->buffer[can_rxbuffer->ptrWrite].Data[3] = (uint8_t)0xFF
                & (ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 24);
        can_rxbuffer->buffer[can_rxbuffer->ptrWrite].Data[4] = (uint8_t)0xFF
                & ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDHR;
        can_rxbuffer->buffer[can_rxbuffer->ptrWrite].Data[5] = (uint8_t)0xFF
                & (ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 8);
        can_rxbuffer->buffer[can_rxbuffer->ptrWrite].Data[6] = (uint8_t)0xFF
                & (ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 16);
        can_rxbuffer->buffer[can_rxbuffer->ptrWrite].Data[7] = (uint8_t)0xFF
                & (ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 24);

        /* Increment write pointer */
        can_rxbuffer->ptrWrite++;
        can_rxbuffer->ptrWrite = can_rxbuffer->ptrWrite % can_rxbuffer->length;
#endif
    } else if (bypassLinkIndex < bufferbypasslength && can_rxmsgs != NULL && can_fastLinkIndex != NULL) {
        /* ##### Buffer active but bypassed ##### */

#if (CAN1_BUFFER_BYPASS_NUMBER_OF_IDs > 0 || CAN0_BUFFER_BYPASS_NUMBER_OF_IDs > 0) && (CAN_USE_CAN_NODE0 == 1 || CAN_USE_CAN_NODE1 == 1)
        /* copy data in handle and call buffer bypass function */
        uint8_t DLC, RTR;

        /* Get the DLC */
        DLC = (uint8_t)0x0F & ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDTR;

        /* Get the data field */
        fastLinkBuffer[0] = (uint8_t)0xFF & ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDLR;
        fastLinkBuffer[1] = (uint8_t)0xFF & (ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 8);
        fastLinkBuffer[2] = (uint8_t)0xFF & (ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 16);
        fastLinkBuffer[3] = (uint8_t)0xFF & (ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 24);
        fastLinkBuffer[4] = (uint8_t)0xFF & ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDHR;
        fastLinkBuffer[5] = (uint8_t)0xFF & (ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 8);
        fastLinkBuffer[6] = (uint8_t)0xFF & (ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 16);
        fastLinkBuffer[7] = (uint8_t)0xFF & (ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 24);

        /* Get the RTR */
        RTR = (uint8_t)0x02 & ptrHcan->Instance->sFIFOMailBox[FIFONumber].RIR;

        /* Call callback function */
        if (can_rxmsgs[can_fastLinkIndex[bypassLinkIndex]].func != NULL) {
            can_rxmsgs[can_fastLinkIndex[bypassLinkIndex]].func(msgID, &fastLinkBuffer[0], DLC, RTR);
        } else {
            /* No callback function defined */
            CAN_BufferBypass(canNode, msgID, &fastLinkBuffer[0], DLC, RTR);
        }
#endif
    } else {
        /* ##### Buffer not active ##### */

        CAN_MSG_RX_TYPE_s* msgRXstruct;
        uint8_t length;
        if (canNode == CAN_NODE0) {
            msgRXstruct = &can0_RxMsgs[0];
            length = can_CAN0_rx_length;
        } else {
            msgRXstruct = &can1_RxMsgs[0];
            length = can_CAN1_rx_length;
        }

        /* Set the Id */
        ptrHcan->pRxMsg->StdId = msgID;

        ptrHcan->pRxMsg->RTR = (uint8_t)0x02 & ptrHcan->Instance->sFIFOMailBox[FIFONumber].RIR;
        /* Get the DLC */
        ptrHcan->pRxMsg->DLC = (uint8_t)0x0F & ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDTR;

        /* Get the data field */
        ptrHcan->pRxMsg->Data[0] = (uint8_t)0xFF & ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDLR;
        ptrHcan->pRxMsg->Data[1] = (uint8_t)0xFF & (ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 8);
        ptrHcan->pRxMsg->Data[2] = (uint8_t)0xFF & (ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 16);
        ptrHcan->pRxMsg->Data[3] = (uint8_t)0xFF & (ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDLR >> 24);
        ptrHcan->pRxMsg->Data[4] = (uint8_t)0xFF & ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDHR;
        ptrHcan->pRxMsg->Data[5] = (uint8_t)0xFF & (ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 8);
        ptrHcan->pRxMsg->Data[6] = (uint8_t)0xFF & (ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 16);
        ptrHcan->pRxMsg->Data[7] = (uint8_t)0xFF & (ptrHcan->Instance->sFIFOMailBox[FIFONumber].RDHR >> 24);

        uint8_t i = 0;
        for (; i < length; i++) {
            if (msgRXstruct[i].ID == msgID) {
                break;
            }
        }

        /* Interpret received message */
        if (msgRXstruct[i].func != NULL) {
            msgRXstruct[i].func(msgID, ptrHcan->pRxMsg->Data, ptrHcan->pRxMsg->DLC, ptrHcan->pRxMsg->RTR);
        } else {
            CAN_InterpretReceivedMsg(canNode, ptrHcan->pRxMsg->StdId, ptrHcan->pRxMsg->Data, ptrHcan->pRxMsg->DLC,
                ptrHcan->pRxMsg->RTR);
        }
    }

    /* Release the FIFO */

    /* Release FIFO0 */
    if (FIFONumber  ==  CAN_FIFO0) {
        __HAL_CAN_FIFO_RELEASE(ptrHcan, CAN_FIFO0);
    } else {
    /* Release FIFO1 */
    /* FIFONumber  ==  CAN_FIFO1 */
        __HAL_CAN_FIFO_RELEASE(ptrHcan, CAN_FIFO1);
    }

    /* Return function status */
    retVal = E_OK;

    return retVal;
}


STD_RETURN_TYPE_e CAN_ReceiveBuffer(CAN_NodeTypeDef_e canNode, Can_PduType* msg) {
    /* E_OK is returned, if buffer is empty and interpret function is called successful */
    STD_RETURN_TYPE_e retVal = E_NOT_OK;

#if CAN0_USE_RX_BUFFER || CAN1_USE_RX_BUFFER

    CAN_RX_BUFFER_s* can_rxbuffer = NULL;

#if CAN0_USE_RX_BUFFER && CAN_USE_CAN_NODE0 == 1
    if (canNode  ==  CAN_NODE0) {
        can_rxbuffer = &can0_rxbuffer;
    }
#endif
#if CAN1_USE_RX_BUFFER && CAN_USE_CAN_NODE1 == 1
    if (canNode  ==  CAN_NODE1) {
        can_rxbuffer = &can1_rxbuffer;
    }
#endif

    if (msg  ==  NULL) {
        /* null pointer to message data struct */
        can_rxbuffer = NULL;
    }

    if (can_rxbuffer->ptrWrite
            != can_rxbuffer->ptrRead&& can_rxbuffer->buffer[can_rxbuffer->ptrRead].newMsg  ==  1 && can_rxbuffer != NULL) {
        /* buffer not empty */
        msg->id = can_rxbuffer->buffer[can_rxbuffer->ptrRead].ID;
        msg->dlc = can_rxbuffer->buffer[can_rxbuffer->ptrRead].DLC;

        for (int i = 0; i < 8; i++) {
            msg->sdu[i] = can_rxbuffer->buffer[can_rxbuffer->ptrRead].Data[i];
        }

        /* Set to 0 to mark buffer entry as read. Set to 1 when writing message into buffer */
        can_rxbuffer->buffer[can_rxbuffer->ptrRead].newMsg = 0;

        /* Move to next buffer element */
        can_rxbuffer->ptrRead++;
        can_rxbuffer->ptrRead = can_rxbuffer->ptrRead % can_rxbuffer->length;
        retVal = E_OK;
    }
#endif

    return retVal;
}

/**
 * @brief  Receives a bypassed CAN message and interprets it
 *
 * @param  canNode: canNode on which the message has been received
 * @param  msgID:   message ID
 * @param  data:    pointer to the message data
 * @param  DLC:     length of received data
 * @param  RTR:     RTR bit of received message
 *
 * @retval E_OK if interpreting was successful, otherwise E_NOT_OK
 */
static STD_RETURN_TYPE_e CAN_BufferBypass(CAN_NodeTypeDef_e canNode, uint32_t msgID, uint8_t* rxData, uint8_t DLC,
        uint8_t RTR) {
    STD_RETURN_TYPE_e retVal = E_OK;

    /* ***************************************************************
     *  Implement wished functionality of received messages here,
     *
     *  if no callback function in CAN_MSG_RX_TYPE_s struct is defined
     *****************************************************************/

    /* Perform SW reset */
    if (msgID == CAN_ID_SOFTWARE_RESET_MSG && DLC == 8) {

        uint8_t reset = 0;

        /* CAN data = FF FF FF FF FF FF FF FF */
        for (uint8_t i = 0; i < DLC; i++) {

            if (rxData[i] != 0xFF)
                reset = 1;
        }
#if CAN_SW_RESET_WITH_DEVICE_ID == 1

/*         /* CAN data = MCU Device ID Byte [0] [1] [2] [3] [4] [5] [6] [7] */
/*         if(rxData[0] == (uint8_t)mcu_unique_deviceID.off0 && data[1] == (uint8_t)(mcu_unique_deviceID.off0 >> 8) &&
                rxData[2] == (uint8_t)(mcu_unique_deviceID.off0 >> 16) && rxData[3] == (uint8_t)(mcu_unique_deviceID.off0 >> 24) &&
                rxData[4] == (uint8_t)mcu_unique_deviceID.off32 && rxData[5] == (uint8_t)(mcu_unique_deviceID.off32 >> 8) &&
                rxData[6] == (uint8_t)(mcu_unique_deviceID.off32 >> 16) && rxData[7] == (uint8_t)(mcu_unique_deviceID.off32 >> 24)) {
            reset = 1;
        }
*/

        if (rxData[0] == 0) {
            if ((CAN_CheckNodeID(&data[5]) == E_OK) || (CAN_CheckUniqueDeviceID(&data[1]) == E_OK) || (CAN_CheckBroadcastID(&data[5]) == E_OK)) {
                reset = 1;
            }
        }
#else
        reset = 1;
#endif

        if (reset == 1)
            HAL_NVIC_SystemReset();

    }
    return retVal;
}

/**
 * @brief  Interprets the received message
 *
 * @param  canNode: canNode on which the message has been received
 * @param  msgID:   message ID
 * @param  data:    pointer to the message data
 * @param  DLC:     length of received data
 * @param  RTR:     RTR bit of received message
 *
 * @return E_OK if interpretation successful, otherwise E_NOT_OK
 */
static STD_RETURN_TYPE_e CAN_InterpretReceivedMsg(CAN_NodeTypeDef_e canNode, uint32_t msgID, uint8_t* data, uint8_t DLC,
        uint8_t RTR) {
    STD_RETURN_TYPE_e retVal = E_NOT_OK;


    /* ***************************************************************
     *  Implement wished functionality of received messages here,
     *
     *  if no callback function in CAN_MSG_RX_TYPE_s struct is defined
     *****************************************************************/

    return retVal;
}

/* ***************************************
 *  Sleep mode
 ****************************************/

void CAN_SetSleepMode(CAN_NodeTypeDef_e canNode) {
    if (canNode  ==  CAN_NODE0) {
        HAL_CAN_Sleep(&hcan0);
    } else if (canNode  ==  CAN_NODE1) {
        HAL_CAN_Sleep(&hcan1);
    }
    return;
}


void CAN_WakeUp(CAN_NodeTypeDef_e canNode) {
    if (canNode  ==  CAN_NODE0) {
        HAL_CAN_WakeUp(&hcan0);
    } else if (canNode  ==  CAN_NODE1) {
        HAL_CAN_WakeUp(&hcan1);
    }
    return;
}
