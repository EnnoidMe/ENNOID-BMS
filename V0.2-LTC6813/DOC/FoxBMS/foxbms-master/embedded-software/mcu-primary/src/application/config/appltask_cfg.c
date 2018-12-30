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
 * @file    appltask_cfg.c
 * @author  foxBMS Team
 * @date    26.08.2015 (date of creation)
 * @ingroup APPLICATION_CONF
 * @prefix  APPL
 *
 * @brief   Configuration header for application tasks
 *
 */

/*================== Includes =============================================*/
#include "appltask_cfg.h"

#include "bms.h"
#include "diag.h"
#include "bal.h"
#include "sox.h"
#include "com.h"
#include "led.h"
#include "cansignal.h"
#include "database.h"
#include "meas.h"
#include "algo.h"

/*================== Macros and Definitions ===============================*/

/*================== Constant and Variable Definitions ====================*/
OS_Task_Definition_s appl_tskdef_cyclic_1ms    = {     0,      1,  OS_PRIORITY_NORMAL,        1024/4};
OS_Task_Definition_s appl_tskdef_cyclic_10ms   = {     4,     10,  OS_PRIORITY_BELOW_NORMAL,  1024/4};
OS_Task_Definition_s appl_tskdef_cyclic_100ms  = {    58,    100,  OS_PRIORITY_LOW,           1024/4};

static uint8_t io_initialized = FALSE;
static uint8_t io_direction = 1;
static uint8_t io_counter = 0;
static uint8_t io_cycle = 0;
static uint8_t first_cycle = 0;
static DATA_BLOCK_SLAVE_CONTROL_s example_slave_control;

/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/
void APPL_Cyclic_1ms(void) {

    DIAG_SysMonNotify(DIAG_SYSMON_APPL_CYCLIC_1ms, 0);        /* task is running, state = ok */

    /* User specific implementations:   */
    /*   ...                            */
    /*   ...                            */
    BMS_Trigger();
}

void APPL_Cyclic_10ms(void) {

    DIAG_SysMonNotify(DIAG_SYSMON_APPL_CYCLIC_10ms, 0);        /* task is running, state = ok */

    /* User specific implementations:   */
    /*   ...                            */
    /*   ...                            */
    CANS_MainFunction();

    LED_Ctrl();

    SOC_Calculation();
    SOF_Calculation();

#if BUILD_MODULE_ENABLE_COM
    COM_Decoder();
#endif

    ALGO_MonitorExecutionTime();

}

void APPL_Cyclic_100ms(void) {
    uint8_t i;
    /* uint8_t j; /* used for DEMO only */

    DIAG_SysMonNotify(DIAG_SYSMON_APPL_CYCLIC_100ms, 0);        /* task is running, state = ok */

    /* User specific implementations:   */
    /*   ...                            */
    /*   ...                            */
    BAL_Trigger();

    ALGO_MainFunction();

#if BUILD_MODULE_ENABLE_COM
        COM_printHelpCommand();
#endif

    if (first_cycle<10) {
        first_cycle++;
    } else if (first_cycle == 10){
        /************************************************************
        DEMO, write to external slave EEPROM, must NOT be done to often, or EEPROM will be worn out
        DB_ReadBlock(&example_slave_control, DATA_BLOCK_ID_SLAVE_CONTROL);
        example_slave_control.eeprom_write_address_to_use = 0x162;
        for (j=0; j<BS_NR_OF_MODULES; j++) {
            example_slave_control.eeprom_value_write[j] = 0x3E;
        }
        DB_WriteBlock(&example_slave_control, DATA_BLOCK_ID_SLAVE_CONTROL);
        MEAS_Request_EEPROM_Write();
        ************************************************************/
        first_cycle++;
    } else {
        /* Shifting 1 up and down (light cycle demo if LEDs are connected to the port-expander) */
        if (io_initialized == FALSE) {
            io_initialized = TRUE;
            for (i=0; i<BS_NR_OF_MODULES; i++) {
                io_cycle = 1;
            }
        }
        else {
            if (io_counter%2==0) {
                if (io_direction == 1) {
                    if (io_cycle != 128) {
                        io_cycle <<= 1;
                    } else {
                        io_direction = 0;
                    }
                }
                if (io_direction == 0) {
                    if (io_cycle != 1) {
                        io_cycle >>= 1;
                    } else {
                        io_direction = 1;
                    }
                }
            }

        }

        DB_ReadBlock(&example_slave_control, DATA_BLOCK_ID_SLAVE_CONTROL);
        for (i=0; i<BS_NR_OF_MODULES; i++) {
            example_slave_control.io_value_out[i] = ~io_cycle;
        }

        if (io_counter%2 == 0) {

            /************************************************************
            DEMO, write to port-expander
            DB_WriteBlock(&example_slave_control, DATA_BLOCK_ID_SLAVE_CONTROL);
            MEAS_Request_IO_Write();
            ************************************************************/
        }
        else if (io_counter%3 == 0) {
            /************************************************************
            DEMO, read from port-expander
            MEAS_Request_IO_Read();
            ************************************************************/
        }
        else if (io_counter%11 == 0) {
            /************************************************************
            DEMO, read external temperature sensor on slaves
            MEAS_Request_Temperature_Read();
            ************************************************************/
        }
        else if (io_counter%17 == 0) {
            /************************************************************
            DEMO, read balancing feedback on slaves
            MEAS_Request_BalancingFeedback_Read();
            ************************************************************/
        }
        else if (io_counter%29 == 0) {
            /************************************************************
            DEMO, read from external slave EEPROM
            example_slave_control.eeprom_read_address_to_use = 0x162;
            DB_WriteBlock(&example_slave_control, DATA_BLOCK_ID_SLAVE_CONTROL);
            MEAS_Request_EEPROM_Read();
            ************************************************************/

        }

        io_counter++;
    }
}
