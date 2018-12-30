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
 * @file    meas.c
 * @author  foxBMS Team
 * @date    11.11.2016 (date of creation)
 * @ingroup DRIVERS
 * @prefix  MEAS
 *
 * @brief   Driver for the measurements needed by the BMS (e.g., I,V,T).
 *
 */

/*================== Includes =============================================*/
#include "meas.h"
#include "ltc.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/


/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/


void MEAS_Ctrl(void)
{

}


extern uint8_t MEAS_Request_IO_Write(void){

    STD_RETURN_TYPE_e retval = E_NOT_OK;

    if (LTC_SetStateRequest(LTC_STATE_USER_IO_WRITE_REQUEST) == LTC_OK) {
        retval = E_OK;
    }

    return (retval);
}

extern uint8_t MEAS_Request_IO_Read(void){

    STD_RETURN_TYPE_e retval = E_NOT_OK;

    if (LTC_SetStateRequest(LTC_STATE_USER_IO_READ_REQUEST) == LTC_OK) {
        retval = E_OK;
    }

    return (retval);
}

extern uint8_t MEAS_Request_Temperature_Read(void){

    STD_RETURN_TYPE_e retval = E_NOT_OK;

    if (LTC_SetStateRequest(LTC_STATE_TEMP_SENS_READ_REQUEST) == LTC_OK) {
        retval = E_OK;
    }

    return (retval);
}

extern uint8_t MEAS_Request_BalancingFeedback_Read(void){

    STD_RETURN_TYPE_e retval = E_NOT_OK;

    if (LTC_SetStateRequest(LTC_STATE_BALANCEFEEDBACK_REQUEST) == LTC_OK) {
        retval = E_OK;
    }

    return (retval);
}


extern uint8_t MEAS_Request_EEPROM_Read(void){

    STD_RETURN_TYPE_e retval = E_NOT_OK;

    if (LTC_SetStateRequest(LTC_STATE_EEPROM_READ_REQUEST) == LTC_OK) {
        retval = E_OK;
    }

    return (retval);
}

extern uint8_t MEAS_Request_EEPROM_Write(void){

    STD_RETURN_TYPE_e retval = E_NOT_OK;

    if (LTC_SetStateRequest(LTC_STATE_EEPROM_WRITE_REQUEST) == LTC_OK) {
        retval = E_OK;
    }

    return (retval);
}


extern STD_RETURN_TYPE_e MEAS_IsFirstMeasurementCycleFinished(void){

    uint8_t retval = FALSE;

    retval    = LTC_IsFirstMeasurementCycleFinished();

    return (retval);
}


extern STD_RETURN_TYPE_e MEAS_StartMeasurement(void){

    STD_RETURN_TYPE_e retval = E_NOT_OK;

    if (LTC_SetStateRequest(LTC_STATE_INIT_REQUEST) == LTC_OK) {
        retval = E_OK;
    }

    return retval;
}
