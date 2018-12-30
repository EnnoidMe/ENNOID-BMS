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
 * @file    ltc.h
 * @author  foxBMS Team
 * @date    01.09.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  LTC
 *
 * @brief   Headers for the driver for the LTC monitoring chip.
 *
 */

#ifndef LTC_H_
#define LTC_H_

/*================== Includes =============================================*/
#include "ltc_cfg.h"

/*================== Macros and Definitions ===============================*/

#if SLAVE_BOARD_VERSION == 1
#elif SLAVE_BOARD_VERSION == 2
#else
#error Please select the slave board version you want to use. Configuration file: \src\module\config\ltc_cfg.h
#endif

typedef struct {
    uint8_t LTC;  /*!<    */
    uint8_t mux0;                               /*!<    */
    uint8_t mux1;                               /*!<    */
    uint8_t mux2;                               /*!<    */
    uint8_t mux3;                               /*!<    */
} LTC_ERRORTABLE_s;

/*================== Constant and Variable Definitions ====================*/

/*================== Function Prototypes ==================================*/

/**
 * @brief   trigger function for the LTC driver state machine.
 *
 * This function contains the sequence of events in the LTC state machine.
 * It must be called time-triggered, every 1ms.
 */
extern void LTC_Trigger(void);

/**
 * @brief   sets the current state request of the state variable ltc_state.
 *
 * This function is used to make a state request to the state machine,e.g, start voltage measurement,
 * read result of voltage measurement, re-initialization
 * It calls LTC_CheckStateRequest() to check if the request is valid.
 * The state request is rejected if is not valid.
 * The result of the check is returned immediately, so that the requester can act in case
 * it made a non-valid state request.
 *
 * @param   statereq                state request to set
 * @param   adcModereq              LTC ADCmeasurement mode (fast, normal or filtered)
 * @param   adcMeasChreq            number of channels measured for GPIOS (one at a time for multiplexers or all five GPIOs)
 * @param   numberOfMeasuredMux     number of  multiplexer inputs measured between two cell voltage measurements
 *
 * @return  retVal                  current state request, taken from LTC_STATE_REQUEST_e
 */
extern LTC_RETURN_TYPE_e LTC_SetStateRequest(LTC_STATE_REQUEST_e statereq);

extern void LTC_SetFirstMeasurementCycleFinished(void);
extern uint8_t LTC_IsFirstMeasurementCycleFinished(void);
extern STD_RETURN_TYPE_e LTC_GetMuxSequenceState(void);

extern void LTC_SaveAllGPIOs(void);
extern void LTC_SaveVoltages(void);
extern void LTC_SaveTemperatures(void);
extern void LTC_SaveAllGPIOMeasurement(void);
extern LTC_STATE_REQUEST_e LTC_GetStateRequest(void);
extern LTC_STATEMACH_e LTC_GetState(void);

/*================== Function Implementations =============================*/

#endif /* LTC_H_ */
