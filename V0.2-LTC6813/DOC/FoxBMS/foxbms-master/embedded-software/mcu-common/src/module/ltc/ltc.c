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
 * @file    ltc.c
 * @author  foxBMS Team
 * @date    01.09.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  LTC
 *
 * @brief   Driver for the LTC monitoring chip.
 *
 */

/*================== Includes =============================================*/
#include "ltc.h"

#include "database.h"
#include "diag.h"
#include "ltc_pec.h"
#include "os.h"

/*================== Macros and Definitions ===============================*/

/**
 * LTC COMM definitions
 */

#define LTC_ICOM_START              0x60
#define LTC_ICOM_STOP               0x10
#define LTC_ICOM_BLANK              0x00
#define LTC_ICOM_NO_TRANSMIT        0x70
#define LTC_FCOM_MASTER_ACK         0x00
#define LTC_FCOM_MASTER_NACK        0x08
#define LTC_FCOM_MASTER_NACK_STOP   0x09

#define LTC_MAX_SUPPORTED_CELLS         12

/**
 * Saves the last state and the last substate
 */
#define LTC_SAVELASTSTATES()    ltc_state.laststate = ltc_state.state; \
                                ltc_state.lastsubstate = ltc_state.substate

/*================== Constant and Variable Definitions ====================*/

static uint8_t ltc_used_cells_index = 0;

static DATA_BLOCK_CELLVOLTAGE_s ltc_cellvoltage;
static DATA_BLOCK_CELLTEMPERATURE_s ltc_celltemperature;
static DATA_BLOCK_MINMAX_s ltc_minmax;
static DATA_BLOCK_BALANCING_FEEDBACK_s ltc_balancing_feedback;
static DATA_BLOCK_USER_MUX_s ltc_user_mux;
static DATA_BLOCK_BALANCING_CONTROL_s ltc_balancing_control;
static DATA_BLOCK_SLAVE_CONTROL_s ltc_slave_control;
static DATA_BLOCK_ALLGPIOVOLTAGE_s ltc_allgpiovoltage;

static LTC_ERRORTABLE_s LTC_ErrorTable[BS_NR_OF_MODULES];  /* init in LTC_ResetErrorTable-function */


static LTC_STATE_s ltc_state = {
    .timer                   = 0,
    .statereq                = LTC_STATE_NO_REQUEST,
    .state                   = LTC_STATEMACH_UNINITIALIZED,
    .substate                = 0,
    .laststate               = LTC_STATEMACH_UNINITIALIZED,
    .lastsubstate            = 0,
    .adcModereq              = LTC_ADCMODE_FAST_DCP0,
    .adcMode                 = LTC_ADCMODE_FAST_DCP0,
    .adcMeasChreq            = LTC_ADCMEAS_UNDEFINED,
    .adcMeasCh               = LTC_ADCMEAS_UNDEFINED,
    .numberOfMeasuredMux     = 32,
    .triggerentry            = 0,
    .ErrRetryCounter         = 0,
    .ErrRequestCounter       = 0,
    .VoltageSampleTime       = 0,
    .muxSampleTime           = 0,
    .commandDataTransferTime = 3,
    .commandTransferTime     = 3,
    .gpioClocksTransferTime  = 3,
    .muxmeas_seqptr          = NULL_PTR,
    .muxmeas_seqendptr       = NULL_PTR,
    .muxmeas_nr_end          = 0,
    .first_measurement_made  = FALSE,
    .ltc_muxcycle_finished   = E_NOT_OK,
    .check_spi_flag          = FALSE,
    .balance_control_done     = FALSE,
};

static const uint8_t ltc_cmdDummy[1]={0x00};
static const uint8_t ltc_cmdWRCFG[4]={0x00, 0x01, 0x3D, 0x6E};
static const uint8_t ltc_cmdWRCFG2[4]={0x00, 0x24, 0xB1, 0x9E};

static const uint8_t ltc_cmdRDCVA[4] = {0x00, 0x04, 0x07, 0xC2};
static const uint8_t ltc_cmdRDCVB[4] = {0x00, 0x06, 0x9A, 0x94};
static const uint8_t ltc_cmdRDCVC[4] = {0x00, 0x08, 0x5E, 0x52};
static const uint8_t ltc_cmdRDCVD[4] = {0x00, 0x0A, 0xC3, 0x04};
static const uint8_t ltc_cmdRDCVE[4] = {0x00, 0x09, 0xD5, 0x60};
static const uint8_t ltc_cmdRDCVF[4] = {0x00, 0x0B, 0x48, 0x36};
static const uint8_t ltc_cmdWRCOMM[4] = {0x07, 0x21, 0x24, 0xB2};
static const uint8_t ltc_cmdSTCOMM[4] = {0x07, 0x23, 0xB9, 0xE4};
static const uint8_t ltc_cmdRDCOMM[4] = {0x07, 0x22, 0x32, 0xD6};
static const uint8_t ltc_cmdRDAUXA[4] = {0x00, 0x0C, 0xEF, 0xCC};
static const uint8_t ltc_cmdRDAUXB[4] = {0x00, 0x0E, 0x72, 0x9A};
static const uint8_t ltc_cmdRDAUXC[4] = {0x00, 0x0D, 0x64, 0xFE};
static const uint8_t ltc_cmdRDAUXD[4] = {0x00, 0x0F, 0xF9, 0xA8};

/* static const uint8_t ltc_cmdMUTE[4] = {0x00, 0x28, 0xE8, 0x0E};                    !< MUTE discharging via S pins */
/* static const uint8_t ltc_cmdUNMUTE[4] = {0x00, 0x29, 0x63, 0x3C};                  !< UN-MUTE discharging via S pins */

/* LTC I2C commands */
/* static const uint8_t ltc_I2CcmdDummy[6] = {0x7F, 0xF9, 0x7F, 0xF9, 0x7F, 0xF9};      !< dummy command (no transmit) */

static const uint8_t ltc_I2CcmdTempSens0[6] = {0x69, 0x08, 0x00, 0x09, 0x7F, 0xF9};  /*!< sets the internal data pointer of the temperature sensor (address 0x48) to 0x00 */
static const uint8_t ltc_I2CcmdTempSens1[6] = {0x69, 0x18, 0x0F, 0xF0, 0x0F, 0xF9};  /*!< reads two data bytes from the temperature sensor */

static const uint8_t ltc_I2CcmdPortExpander1[6] = {0x64, 0x18, 0x0F, 0xF9, 0x7F, 0xF9};  /*!< reads one data byte from the port expander */

/* Cells */
static const uint8_t ltc_cmdADCV_normal_DCP0[4] = {0x03, 0x60, 0xF4, 0x6C};        /*!< All cells, normal mode, discharge not permitted (DCP=0)    */
static const uint8_t ltc_cmdADCV_normal_DCP1[4] = {0x03, 0x70, 0xAF, 0x42};        /*!< All cells, normal mode, discharge permitted (DCP=1)        */
static const uint8_t ltc_cmdADCV_filtered_DCP0[4] = {0x03, 0xE0, 0xB0, 0x4A};      /*!< All cells, filtered mode, discharge not permitted (DCP=0)  */
static const uint8_t ltc_cmdADCV_filtered_DCP1[4] = {0x03, 0xF0, 0xEB, 0x64};      /*!< All cells, filtered mode, discharge permitted (DCP=1)      */
static const uint8_t ltc_cmdADCV_fast_DCP0[4] = {0x02, 0xE0, 0x38, 0x06};          /*!< All cells, fast mode, discharge not permitted (DCP=0)      */
static const uint8_t ltc_cmdADCV_fast_DCP1[4] = {0x02, 0xF0, 0x63, 0x28};          /*!< All cells, fast mode, discharge permitted (DCP=1)          */
static const uint8_t ltc_cmdADCV_fast_DCP0_twocells[4] = {0x02, 0xE1, 0xb3, 0x34}; /*!< Two cells (1 and 7), fast mode, discharge not permitted (DCP=0) */

/* GPIOs  */
static const uint8_t ltc_cmdADAX_normal_GPIO1[4] = {0x05, 0x61, 0x58, 0x92};      /*!< Single channel, GPIO 1, normal mode   */
static const uint8_t ltc_cmdADAX_filtered_GPIO1[4] = {0x05, 0xE1, 0x1C, 0xB4};    /*!< Single channel, GPIO 1, filtered mode */
static const uint8_t ltc_cmdADAX_fast_GPIO1[4] = {0x04, 0xE1, 0x94, 0xF8};        /*!< Single channel, GPIO 1, fast mode     */
static const uint8_t ltc_cmdADAX_normal_GPIO2[4] = {0x05, 0x62, 0x4E, 0xF6};      /*!< Single channel, GPIO 2, normal mode   */
static const uint8_t ltc_cmdADAX_filtered_GPIO2[4] = {0x05, 0xE2, 0x0A, 0xD0};    /*!< Single channel, GPIO 2, filtered mode */
static const uint8_t ltc_cmdADAX_fast_GPIO2[4] = {0x04, 0xE2, 0x82, 0x9C};        /*!< Single channel, GPIO 2, fast mode     */
static const uint8_t ltc_cmdADAX_normal_GPIO3[4] = {0x05, 0x63, 0xC5, 0xC4};      /*!< Single channel, GPIO 3, normal mode   */
static const uint8_t ltc_cmdADAX_filtered_GPIO3[4] = {0x05, 0xE3, 0x81, 0xE2};    /*!< Single channel, GPIO 3, filtered mode */
static const uint8_t ltc_cmdADAX_fast_GPIO3[4] = {0x04, 0xE3, 0x09, 0xAE};        /*!< Single channel, GPIO 3, fast mode     */
/* static const uint8_t ltc_cmdADAX_normal_GPIO4[4] = {0x05, 0x64, 0x62, 0x3E};      !< Single channel, GPIO 4, normal mode   */
/* static const uint8_t ltc_cmdADAX_filtered_GPIO4[4] = {0x05, 0xE4, 0x26, 0x18};    !< Single channel, GPIO 4, filtered mode */
/* static const uint8_t ltc_cmdADAX_fast_GPIO4[4] = {0x04, 0xE4, 0xAE, 0x54};        !< Single channel, GPIO 4, fast mode     */
/* static const uint8_t ltc_cmdADAX_normal_GPIO5[4] = {0x05, 0x65, 0xE9, 0x0C};      !< Single channel, GPIO 5, normal mode   */
/* static const uint8_t ltc_cmdADAX_filtered_GPIO5[4] = {0x05, 0xE5, 0xAD, 0x2A};    !< Single channel, GPIO 5, filtered mode */
/* static const uint8_t ltc_cmdADAX_fast_GPIO5[4] = {0x04, 0xE5, 0x25, 0x66};        !< Single channel, GPIO 5, fast mode     */
static const uint8_t ltc_cmdADAX_normal_ALLGPIOS[4] = {0x05, 0x60, 0xD3, 0xA0};   /*!< All channels, normal mode             */
static const uint8_t ltc_cmdADAX_filtered_ALLGPIOS[4] = {0x05, 0xE0, 0x97, 0x86}; /*!< All channels, filtered mode           */
static const uint8_t ltc_cmdADAX_fast_ALLGPIOS[4] = {0x04, 0xE0, 0x1F, 0xCA};     /*!< All channels, fast mode               */

static uint8_t ltc_RXPECbuffer[LTC_N_BYTES_FOR_DATA_TRANSMISSION];
static uint8_t ltc_TXPECbuffer[LTC_N_BYTES_FOR_DATA_TRANSMISSION];
static uint8_t ltc_TXBuffer[LTC_N_BYTES_FOR_DATA_TRANSMISSION_DATA_ONLY];

static uint8_t ltc_TXBufferClock[4+9];
static uint8_t ltc_TXPECBufferClock[4+9];


/*================== Function Prototypes ==================================*/

static void LTC_Initialize_Database(void);
static void LTC_SaveBalancingFeedback(uint8_t *DataBufferSPI_RX);
static void LTC_Get_BalancingControlValues(void);

static STD_RETURN_TYPE_e LTC_BalanceControl(uint8_t registerSet);

static void LTC_ResetErrorTable(void);
static STD_RETURN_TYPE_e LTC_Init(void);

static STD_RETURN_TYPE_e LTC_StartVoltageMeasurement(LTC_ADCMODE_e adcMode, LTC_ADCMEAS_CHAN_e  adcMeasCh);
static STD_RETURN_TYPE_e LTC_StartGPIOMeasurement(LTC_ADCMODE_e adcMode, LTC_ADCMEAS_CHAN_e  adcMeasCh);
static uint16_t LTC_Get_MeasurementTCycle(LTC_ADCMODE_e adcMode, LTC_ADCMEAS_CHAN_e  adcMeasCh);

static void LTC_SaveRXtoVoltagebuffer(uint8_t registerSet, uint8_t *rxBuffer, uint8_t PEC_valid);

static void LTC_SaveRXtoGPIOBuffer(uint8_t registerSet, uint8_t *rxBuffer);

static STD_RETURN_TYPE_e LTC_RX_PECCheck(uint8_t *DataBufferSPI_RX_with_PEC);
static STD_RETURN_TYPE_e LTC_RX(uint8_t *Command, uint8_t *DataBufferSPI_RX_with_PEC);
static STD_RETURN_TYPE_e LTC_TX(uint8_t *Command, uint8_t *DataBufferSPI_TX, uint8_t *DataBufferSPI_TX_with_PEC);
static void LTC_SetMUXChCommand(uint8_t *DataBufferSPI_TX, uint8_t mux, uint8_t channel);
static uint8_t LTC_SendEEPROMReadCommand(uint8_t *DataBufferSPI_TX, uint8_t *DataBufferSPI_TX_with_PEC, uint8_t step);
static void LTC_SetEEPROMReadCommand(uint8_t step, uint8_t *DataBufferSPI_TX);
static void LTC_EEPROMSaveReadValue(uint8_t *rxBuffer);
static uint8_t LTC_SendEEPROMWriteCommand(uint8_t *DataBufferSPI_TX, uint8_t *DataBufferSPI_TX_with_PEC, uint8_t step);
static void LTC_SetEEPROMWriteCommand(uint8_t step, uint8_t *DataBufferSPI_TX);
static uint8_t LTC_SetMuxChannel(uint8_t *DataBufferSPI_TX, uint8_t *DataBufferSPI_TX_with_PEC, uint8_t mux, uint8_t channel);
static uint8_t LTC_SetPortExpander(uint8_t *DataBufferSPI_TX, uint8_t *DataBufferSPI_TX_with_PEC);
static void LTC_PortExpanderSaveValues(uint8_t *rxBuffer);
static void LTC_TempSensSaveTemp(uint8_t *rxBuffer);

static STD_RETURN_TYPE_e LTC_I2CClock(uint8_t *DataBufferSPI_TX, uint8_t *DataBufferSPI_TX_with_PEC);
static STD_RETURN_TYPE_e LTC_Send_I2C_Command(uint8_t *DataBufferSPI_TX, uint8_t *DataBufferSPI_TX_with_PEC, uint8_t *cmd_data);

static uint8_t LTC_I2CCheckACK(uint8_t *DataBufferSPI_RX, int mux);

static void LTC_SaveMuxMeasurement(uint8_t *DataBufferSPI_RX, LTC_MUX_CH_CFG_s  *muxseqptr);


static uint32_t LTC_GetSPIClock(void);
static void LTC_SetTransferTimes(void);

static LTC_RETURN_TYPE_e LTC_CheckStateRequest(LTC_STATE_REQUEST_e statereq);


/*================== Function Implementations =============================*/

/*================== Public functions =====================================*/

/*================== Static functions =====================================*/
/**
 * @brief   in the database, initializes the fields related to the LTC drivers.
 *
 * This function loops through all the LTC-related data fields in the database
 * and sets them to 0. It should be called in the initialization or re-initialization
 * routine of the LTC driver.
 */
static void LTC_Initialize_Database(void) {

    uint16_t i = 0;

    ltc_cellvoltage.state = 0;
    ltc_cellvoltage.timestamp = 0;
    ltc_minmax.voltage_min = 0;
    ltc_minmax.voltage_max = 0;
    ltc_minmax.voltage_module_number_min = 0;
    ltc_minmax.voltage_module_number_max = 0;
    ltc_minmax.voltage_cell_number_min = 0;
    ltc_minmax.voltage_cell_number_max = 0;
    for (i=0; i < BS_NR_OF_BAT_CELLS; i++) {
        ltc_cellvoltage.voltage[i] = 0;
    }

    ltc_celltemperature.state = 0;
    ltc_celltemperature.timestamp = 0;
    ltc_minmax.temperature_min = 0;
    ltc_minmax.temperature_max = 0;
    ltc_minmax.temperature_module_number_min = 0;
    ltc_minmax.temperature_module_number_max = 0;
    ltc_minmax.temperature_sensor_number_min = 0;
    ltc_minmax.temperature_sensor_number_max = 0;
    for (i=0; i < BS_NR_OF_TEMP_SENSORS; i++) {
        ltc_celltemperature.temperature[i] = 0;
    }

    ltc_balancing_feedback.state = 0;
    ltc_balancing_feedback.timestamp = 0;
    ltc_balancing_control.state = 0;
    ltc_balancing_control.timestamp = 0;
    for (i=0; i < BS_NR_OF_BAT_CELLS; i++) {
        ltc_balancing_feedback.value[i] = 0;
        ltc_balancing_control.balancing_state[i] = 0;
    }

    ltc_slave_control.state = 0;
    ltc_slave_control.timestamp = 0;
    ltc_slave_control.previous_timestamp = 0;
    for (i=0; i < BS_NR_OF_MODULES; i++) {
        ltc_slave_control.io_value_in[i] = 0;
        ltc_slave_control.io_value_out[i] = 0;
        ltc_slave_control.external_sensor_temperature[i] = 0;
        ltc_slave_control.eeprom_value_read[i] = 0;
        ltc_slave_control.eeprom_value_write[i] = 0;
    }
    ltc_slave_control.eeprom_read_address_last_used = 0xFFFFFFFF;
    ltc_slave_control.eeprom_read_address_to_use = 0xFFFFFFFF;
    ltc_slave_control.eeprom_write_address_last_used = 0xFFFFFFFF;
    ltc_slave_control.eeprom_write_address_to_use = 0xFFFFFFFF;

    ltc_allgpiovoltage.timestamp = 0;
    ltc_allgpiovoltage.previous_timestamp = 0;
    ltc_allgpiovoltage.state = 0;
    for (i=0; i < BS_NR_OF_MODULES * BS_NR_OF_GPIOS_PER_MODULE; i++) {
        ltc_allgpiovoltage.gpiovoltage[i] = 0;
    }

    DB_WriteBlock(&ltc_cellvoltage, DATA_BLOCK_ID_CELLVOLTAGE);
    DB_WriteBlock(&ltc_celltemperature, DATA_BLOCK_ID_CELLTEMPERATURE);
    DB_WriteBlock(&ltc_minmax, DATA_BLOCK_ID_MINMAX);
    DB_WriteBlock(&ltc_balancing_feedback, DATA_BLOCK_ID_BALANCING_FEEDBACK_VALUES);
    DB_WriteBlock(&ltc_balancing_control, DATA_BLOCK_ID_BALANCING_CONTROL_VALUES);
    DB_WriteBlock(&ltc_slave_control, DATA_BLOCK_ID_SLAVE_CONTROL);

    for (i=0; i < (8*2*BS_NR_OF_MODULES); i++) {
        ltc_user_mux.value[i] = 0;
    }
    ltc_user_mux.previous_timestamp = 0;
    ltc_user_mux.timestamp = 0;
    ltc_user_mux.state = 0;

}

/**
 * @brief   stores the measured voltages in the database.
 *
 * This function loops through the data of all modules in the LTC daisy-chain that are
 * stored in the LTC_CellVoltages buffer and writes them in the database.
 * At each write iteration, the variable named "state" and related to voltages in the
 * database is incremented.
 *
 */
extern void LTC_SaveVoltages(void) {

    uint16_t i = 0;
    uint16_t j = 0;
    uint16_t min = 0;
    uint16_t max = 0;
    uint32_t mean = 0;
    uint8_t module_number_min = 0;
    uint8_t module_number_max = 0;
    uint8_t cell_number_min = 0;
    uint8_t cell_number_max = 0;

    max = min = ltc_cellvoltage.voltage[0];
    mean = 0;
    for (i=0; i < BS_NR_OF_MODULES; i++) {
        for (j=0; j < BS_NR_OF_BAT_CELLS_PER_MODULE; j++) {
            mean += ltc_cellvoltage.voltage[i*(BS_NR_OF_BAT_CELLS_PER_MODULE)+j];
            if (ltc_cellvoltage.voltage[i*(BS_NR_OF_BAT_CELLS_PER_MODULE)+j] < min) {
                min = ltc_cellvoltage.voltage[i*(BS_NR_OF_BAT_CELLS_PER_MODULE)+j];
                module_number_min = i;
                cell_number_min = j;
            }
            if (ltc_cellvoltage.voltage[i*(BS_NR_OF_BAT_CELLS_PER_MODULE)+j] > max) {
                max = ltc_cellvoltage.voltage[i*(BS_NR_OF_BAT_CELLS_PER_MODULE)+j];
                module_number_max = i;
                cell_number_max = j;
            }
        }
    }
    mean /= (BS_NR_OF_BAT_CELLS);

    DB_ReadBlock(&ltc_minmax, DATA_BLOCK_ID_MINMAX);
    ltc_cellvoltage.state++;
    ltc_minmax.state++;
    ltc_minmax.voltage_mean = mean;
    ltc_minmax.previous_voltage_min = ltc_minmax.voltage_min;
    ltc_minmax.voltage_min = min;
    ltc_minmax.voltage_module_number_min = module_number_min;
    ltc_minmax.voltage_cell_number_min = cell_number_min;
    ltc_minmax.previous_voltage_max = ltc_minmax.voltage_max;
    ltc_minmax.voltage_max = max;
    ltc_minmax.voltage_module_number_max = module_number_max;
    ltc_minmax.voltage_cell_number_max = cell_number_max;
    DB_WriteBlock(&ltc_cellvoltage, DATA_BLOCK_ID_CELLVOLTAGE);
    DB_WriteBlock(&ltc_minmax, DATA_BLOCK_ID_MINMAX);

}

/**
 * @brief   stores the measured temperatures and the measured multiplexer feedbacks in the database.
 *
 * This function loops through the temperature and multiplexer feedback data of all modules
 * in the LTC daisy-chain that are stored in the LTC_MultiplexerVoltages buffer and writes
 * them in the database.
 * At each write iteration, the variables named "state" and related to temperatures and multiplexer feedbacks
 * in the database are incremented.
 *
 */
extern void LTC_SaveTemperatures(void) {

    uint16_t i = 0;
    uint16_t j = 0;
    int16_t min = 0;
    uint16_t max = 0;
    int32_t mean = 0;
    uint8_t module_number_min = 0;
    uint8_t module_number_max = 0;
    uint8_t sensor_number_min = 0;
    uint8_t sensor_number_max = 0;

    max = min = ltc_celltemperature.temperature[0];
    mean = 0;
    for (i=0; i < BS_NR_OF_MODULES; i++) {
        for (j=0; j < BS_NR_OF_TEMP_SENSORS_PER_MODULE; j++) {
            mean += ltc_celltemperature.temperature[i*(BS_NR_OF_TEMP_SENSORS_PER_MODULE)+j];
            if (ltc_celltemperature.temperature[i*(BS_NR_OF_TEMP_SENSORS_PER_MODULE)+j] < min) {
                min = ltc_celltemperature.temperature[i*(BS_NR_OF_TEMP_SENSORS_PER_MODULE)+j];
                module_number_min = i;
                sensor_number_min = j;
            }
            if (ltc_celltemperature.temperature[i*(BS_NR_OF_TEMP_SENSORS_PER_MODULE)+j] > max) {
                max = ltc_celltemperature.temperature[i*(BS_NR_OF_TEMP_SENSORS_PER_MODULE)+j];
                module_number_max = i;
                sensor_number_max = j;
            }
        }
    }
    mean /= (BS_NR_OF_TEMP_SENSORS);

    DB_ReadBlock(&ltc_minmax, DATA_BLOCK_ID_MINMAX);
    ltc_celltemperature.state++;
    ltc_minmax.state++;
    ltc_minmax.temperature_mean = mean;
    ltc_minmax.temperature_min = min;
    ltc_minmax.temperature_module_number_min = module_number_min;
    ltc_minmax.temperature_sensor_number_min = sensor_number_min;
    ltc_minmax.temperature_max = max;
    ltc_minmax.temperature_module_number_max = module_number_max;
    ltc_minmax.temperature_sensor_number_max = sensor_number_max;
    DB_WriteBlock(&ltc_celltemperature, DATA_BLOCK_ID_CELLTEMPERATURE);
    DB_WriteBlock(&ltc_minmax, DATA_BLOCK_ID_MINMAX);
}

/**
 * @brief   stores the measured GPIOs in the database.
 *
 * This function loops through the data of all modules in the LTC daisy-chain that are
 * stored in the ltc_allgpiovoltage buffer and writes them in the database.
 * At each write iteration, the variable named "state" and related to voltages in the
 * database is incremented.
 *
 */
extern void LTC_SaveAllGPIOMeasurement(void) {

    ltc_allgpiovoltage.state++;
    DB_WriteBlock(&ltc_allgpiovoltage, DATA_BLOCK_ID_ALLGPIOVOLTAGE);

}

/**
 * @brief   stores the measured balancing feedback values in the database.
 *
 * This function stores the global balancing feedback value measured on GPIO3 of the LTC into the database
 *
 */
static void LTC_SaveBalancingFeedback(uint8_t *DataBufferSPI_RX) {

    uint16_t i = 0;
    uint16_t val_i = 0;

    for (i=0; i < LTC_N_LTC; i++) {

        val_i = DataBufferSPI_RX[8+1*i*8] | (DataBufferSPI_RX[8+1*i*8+1] << 8);    /* raw value, GPIO3 */

            ltc_balancing_feedback.value[i] = val_i;

    }

    ltc_balancing_feedback.state++;
    DB_WriteBlock(&ltc_balancing_feedback, DATA_BLOCK_ID_BALANCING_FEEDBACK_VALUES);

}


/**
 * @brief   gets the balancing orders from the database.
 *
 * This function gets the balancing control from the database. Balancing control
 * is set by the BMS. The LTC driver only executes the balancing orders.
 */
static void LTC_Get_BalancingControlValues(void) {
    DB_ReadBlock(&ltc_balancing_control, DATA_BLOCK_ID_BALANCING_CONTROL_VALUES);
}



/**
 * @brief   re-entrance check of LTC state machine trigger function
 *
 * This function is not re-entrant and should only be called time- or event-triggered.
 * It increments the triggerentry counter from the state variable ltc_state.
 * It should never be called by two different processes, so if it is the case, triggerentry
 * should never be higher than 0 when this function is called.
 *
 *
 * @return  retval  0 if no further instance of the function is active, 0xff else
 *
 */
uint8_t LTC_CheckReEntrance(void) {
    uint8_t retval = 0;

    OS_TaskEnter_Critical();
    if (!ltc_state.triggerentry) {
        ltc_state.triggerentry++;
    } else {
        retval = 0xFF;    /* multiple calls of function */
    }
    OS_TaskExit_Critical();

    return (retval);
}

/**
 * @brief   gets the current state request.
 *
 * This function is used in the functioning of the LTC state machine.
 *
 * @return  retval  current state request, taken from LTC_STATE_REQUEST_e
 */
extern LTC_STATE_REQUEST_e LTC_GetStateRequest(void) {

    LTC_STATE_REQUEST_e retval = LTC_STATE_NO_REQUEST;

    OS_TaskEnter_Critical();
    retval    = ltc_state.statereq;
    OS_TaskExit_Critical();

    return (retval);
}

/**
 * @brief   gets the current state.
 *
 * This function is used in the functioning of the LTC state machine.
 *
 * @return  current state, taken from LTC_STATEMACH_e
 */
extern LTC_STATEMACH_e LTC_GetState(void) {
    return (ltc_state.state);
}

/**
 * @brief   transfers the current state request to the state machine.
 *
 * This function takes the current state request from ltc_state and transfers it to the state machine.
 * It resets the value from ltc_state to LTC_STATE_NO_REQUEST
 *
 * @param   *busIDptr       bus ID, main or backup (deprecated)
 * @param   *adcModeptr     LTC ADCmeasurement mode (fast, normal or filtered)
 * @param   *adcMeasChptr   number of channels measured for GPIOS (one at a time for multiplexers or all five GPIOs)
 *
 * @return  retVal          current state request, taken from LTC_STATE_REQUEST_e
 *
 */
LTC_STATE_REQUEST_e LTC_TransferStateRequest(uint8_t *busIDptr, LTC_ADCMODE_e *adcModeptr, LTC_ADCMEAS_CHAN_e *adcMeasChptr) {

    LTC_STATE_REQUEST_e retval = LTC_STATE_NO_REQUEST;

    OS_TaskEnter_Critical();
    retval    = ltc_state.statereq;
    *adcModeptr = ltc_state.adcModereq;
    *adcMeasChptr = ltc_state.adcMeasChreq;
    ltc_state.statereq = LTC_STATE_NO_REQUEST;
    OS_TaskExit_Critical();

    return (retval);
}

LTC_RETURN_TYPE_e LTC_SetStateRequest(LTC_STATE_REQUEST_e statereq) {

    LTC_RETURN_TYPE_e retVal = LTC_STATE_NO_REQUEST;

    OS_TaskEnter_Critical();
    retVal = LTC_CheckStateRequest(statereq);

    if (retVal == LTC_OK || retVal == LTC_BUSY_OK || retVal == LTC_OK_FROM_ERROR) {
            ltc_state.statereq   = statereq;
        }
    OS_TaskExit_Critical();

    return (retVal);
}

void LTC_Trigger(void) {

    STD_RETURN_TYPE_e retVal = E_OK;
    LTC_STATE_REQUEST_e statereq = LTC_STATE_NO_REQUEST;
    uint8_t tmpbusID = 0;
    LTC_ADCMODE_e tmpadcMode = LTC_ADCMODE_UNDEFINED;
    LTC_ADCMEAS_CHAN_e tmpadcMeasCh = LTC_ADCMEAS_UNDEFINED;
    uint8_t PEC_valid = FALSE;

    /* Check re-entrance of function */
    if (LTC_CheckReEntrance())
        return;

    DIAG_SysMonNotify(DIAG_SYSMON_LTC_ID, 0);        /* task is running, state = ok */

    if (ltc_state.check_spi_flag == FALSE) {
        if (ltc_state.timer) {
            if (--ltc_state.timer) {
                ltc_state.triggerentry--;
                return;    /* handle state machine only if timer has elapsed */
            }
        }
    } else {
        if (SPI_IsTransmitOngoing() == TRUE) {
            if (ltc_state.timer) {
                if (--ltc_state.timer) {
                    ltc_state.triggerentry--;
                    return;    /* handle state machine only if timer has elapsed */
                }
            }
        }
    }


    switch (ltc_state.state) {
        /****************************UNINITIALIZED***********************************/
        case LTC_STATEMACH_UNINITIALIZED:
            /* waiting for Initialization Request */
            statereq = LTC_TransferStateRequest(&tmpbusID, &tmpadcMode, &tmpadcMeasCh);
            if (statereq == LTC_STATE_INIT_REQUEST) {

                LTC_SAVELASTSTATES();
                ltc_state.timer = LTC_STATEMACH_SHORTTIME;
                ltc_state.state = LTC_STATEMACH_INITIALIZATION;
                ltc_state.substate = LTC_ENTRY_UNINITIALIZED;
                ltc_state.adcMode = tmpadcMode;
                ltc_state.adcMeasCh = tmpadcMeasCh;
            } else if (statereq == LTC_STATE_NO_REQUEST) {

                /* no actual request pending */
            } else {
                ltc_state.ErrRequestCounter++;   /* illegal request pending */
            }
            break;

        /****************************INITIALIZATION**********************************/
        case LTC_STATEMACH_INITIALIZATION:

            LTC_SetTransferTimes();
            ltc_state.muxmeas_seqptr = ltc_mux_seq.seqptr;
            ltc_state.muxmeas_nr_end = ltc_mux_seq.nr_of_steps;
            ltc_state.muxmeas_seqendptr = ((LTC_MUX_CH_CFG_s *)ltc_mux_seq.seqptr)+ltc_mux_seq.nr_of_steps;  /* last sequence + 1 */

            if (ltc_state.substate == LTC_ENTRY_INITIALIZATION) {

                LTC_SAVELASTSTATES();
                retVal = LTC_SendWakeUp();        /* Send dummy byte to wake up the daisy chain */

                if ((retVal != E_OK)) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                    ltc_state.timer = LTC_STATEMACH_SHORTTIME;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                    ltc_state.substate = LTC_RE_ENTRY_INITIALIZATION;
                    ltc_state.timer = LTC_STATEMACH_DAISY_CHAIN_FIRST_INITIALIZATION_TIME;
                }

            } else if (ltc_state.substate == LTC_RE_ENTRY_INITIALIZATION) {

                LTC_SAVELASTSTATES();
                retVal = LTC_SendWakeUp();  /* Send dummy byte again to wake up the daisy chain */

                if ((retVal != E_OK)) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                    ltc_state.timer = LTC_STATEMACH_SHORTTIME;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                    ltc_state.substate = LTC_START_INIT_INITIALIZATION;
                    ltc_state.timer = LTC_STATEMACH_DAISY_CHAIN_SECOND_INITIALIZATION_TIME;
                }

            } else if (ltc_state.substate == LTC_START_INIT_INITIALIZATION) {

                retVal = LTC_Init();  /* Initialize main LTC loop */
                ltc_state.lastsubstate = ltc_state.substate;

                if ((retVal != E_OK)) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                }

                ltc_state.substate = LTC_EXIT_INITIALIZATION;
                ltc_state.timer = ltc_state.commandDataTransferTime;

            } else if (ltc_state.substate == LTC_EXIT_INITIALIZATION) {
            /* in daisy-chain mode, there is no confirmation of the initialization */
                LTC_SAVELASTSTATES();
                LTC_Initialize_Database();
                LTC_ResetErrorTable();
                ltc_state.timer = LTC_STATEMACH_SHORTTIME;
                ltc_state.state = LTC_STATEMACH_INITIALIZED;
                ltc_state.substate = LTC_ENTRY_INITIALIZATION;
            }
            break;

        /****************************INITIALIZED*************************************/
        case LTC_STATEMACH_INITIALIZED:
            LTC_IF_INITIALIZED_CALLBACK();
            LTC_SAVELASTSTATES();
            ltc_state.timer = LTC_STATEMACH_SHORTTIME;
            ltc_state.state = LTC_STATEMACH_STARTMEAS;
            ltc_state.substate = LTC_ENTRY;
            break;

        /****************************START MEASUREMENT*******************************/
        case LTC_STATEMACH_STARTMEAS:

            ltc_state.adcMode = LTC_VOLTAGE_MEASUREMENT_MODE;
            ltc_state.adcMeasCh = LTC_ADCMEAS_ALLCHANNEL;

            ltc_state.check_spi_flag = FALSE;
            retVal = LTC_StartVoltageMeasurement(ltc_state.adcMode, ltc_state.adcMeasCh);

            if ((retVal != E_OK)) {
                DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                ltc_state.timer = LTC_STATEMACH_SHORTTIME;
            } else {
                DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                ltc_state.timer = ltc_state.commandTransferTime + LTC_Get_MeasurementTCycle(ltc_state.adcMode, ltc_state.adcMeasCh);
            }
                ltc_state.state = LTC_STATEMACH_READVOLTAGE;
                ltc_state.substate = LTC_READ_VOLTAGE_REGISTER_A_RDCVA_READVOLTAGE;

            break;

        /****************************READ VOLTAGE************************************/
        case LTC_STATEMACH_READVOLTAGE:

            if (ltc_state.substate == LTC_READ_VOLTAGE_REGISTER_A_RDCVA_READVOLTAGE) {

                ltc_state.check_spi_flag = TRUE;
                SPI_SetTransmitOngoing();
                retVal = LTC_RX((uint8_t*)(ltc_cmdRDCVA), ltc_RXPECbuffer);
                if (retVal != E_OK) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                    ltc_state.timer = 0;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                    ltc_state.timer = ltc_state.commandDataTransferTime+10;
                }
                    ltc_state.substate = LTC_READ_VOLTAGE_REGISTER_B_RDCVB_READVOLTAGE;

                break;

            } else if (ltc_state.substate == LTC_READ_VOLTAGE_REGISTER_B_RDCVB_READVOLTAGE) {

                if (LTC_RX_PECCheck(ltc_RXPECbuffer) != E_OK) {
                    DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_NOK, 0, NULL_PTR);
                    PEC_valid = FALSE;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_OK, 0, NULL_PTR);
                    PEC_valid = TRUE;
                }
                LTC_SaveRXtoVoltagebuffer(0, ltc_RXPECbuffer, PEC_valid);

                SPI_SetTransmitOngoing();
                retVal = LTC_RX((uint8_t*)(ltc_cmdRDCVB), ltc_RXPECbuffer);
                if (retVal != E_OK) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                    ltc_state.timer = 0;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                    ltc_state.timer = ltc_state.commandDataTransferTime+10;
                }
                    ltc_state.substate = LTC_READ_VOLTAGE_REGISTER_C_RDCVC_READVOLTAGE;


                break;

            } else if (ltc_state.substate == LTC_READ_VOLTAGE_REGISTER_C_RDCVC_READVOLTAGE) {

                if (LTC_RX_PECCheck(ltc_RXPECbuffer) != E_OK) {
                    DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_NOK, 0, NULL_PTR);
                    PEC_valid = FALSE;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_OK, 0, NULL_PTR);
                    PEC_valid = TRUE;
                }
                LTC_SaveRXtoVoltagebuffer(1, ltc_RXPECbuffer, PEC_valid);

                SPI_SetTransmitOngoing();
                retVal = LTC_RX((uint8_t*)(ltc_cmdRDCVC), ltc_RXPECbuffer);
                if (retVal != E_OK) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                    ltc_state.timer = 0;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                    ltc_state.timer = ltc_state.commandDataTransferTime+10;
                }
                    ltc_state.substate = LTC_READ_VOLTAGE_REGISTER_D_RDCVD_READVOLTAGE;

                break;

            } else if (ltc_state.substate == LTC_READ_VOLTAGE_REGISTER_D_RDCVD_READVOLTAGE) {

                if (LTC_RX_PECCheck(ltc_RXPECbuffer) != E_OK) {
                    DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_NOK, 0, NULL_PTR);
                    PEC_valid = FALSE;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_OK, 0, NULL_PTR);
                    PEC_valid = TRUE;
                }
                LTC_SaveRXtoVoltagebuffer(2, ltc_RXPECbuffer, PEC_valid);

                SPI_SetTransmitOngoing();
                retVal = LTC_RX((uint8_t*)(ltc_cmdRDCVD), ltc_RXPECbuffer);
                if (retVal != E_OK) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                    ltc_state.timer = 0;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                    ltc_state.timer = ltc_state.commandDataTransferTime+10;
                }

                if (BS_MAX_SUPPORTED_CELLS > 12) {
                    ltc_state.substate = LTC_READ_VOLTAGE_REGISTER_E_RDCVE_READVOLTAGE;
                } else {
                    ltc_state.substate = LTC_EXIT_READVOLTAGE;
                }

                break;

            } else if (ltc_state.substate == LTC_READ_VOLTAGE_REGISTER_E_RDCVE_READVOLTAGE) {

                if (LTC_RX_PECCheck(ltc_RXPECbuffer) != E_OK) {
                    DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_NOK, 0, NULL_PTR);
                    PEC_valid = FALSE;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_OK, 0, NULL_PTR);
                    PEC_valid = TRUE;
                }
                LTC_SaveRXtoVoltagebuffer(3, ltc_RXPECbuffer, PEC_valid);

                SPI_SetTransmitOngoing();
                retVal = LTC_RX((uint8_t*)(ltc_cmdRDCVE), ltc_RXPECbuffer);
                if (retVal != E_OK) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                    ltc_state.timer = 0;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                    ltc_state.timer = ltc_state.commandDataTransferTime+10;
                }

                if (BS_MAX_SUPPORTED_CELLS > 15) {
                    ltc_state.substate = LTC_READ_VOLTAGE_REGISTER_F_RDCVF_READVOLTAGE;
                } else {
                    ltc_state.substate = LTC_EXIT_READVOLTAGE;
                }

                break;

            } else if (ltc_state.substate == LTC_READ_VOLTAGE_REGISTER_F_RDCVF_READVOLTAGE) {

                if (LTC_RX_PECCheck(ltc_RXPECbuffer) != E_OK) {
                    DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_NOK, 0, NULL_PTR);
                    PEC_valid = FALSE;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_OK, 0, NULL_PTR);
                    PEC_valid = TRUE;
                }
                LTC_SaveRXtoVoltagebuffer(4, ltc_RXPECbuffer, PEC_valid);

                SPI_SetTransmitOngoing();
                retVal = LTC_RX((uint8_t*)(ltc_cmdRDCVF), ltc_RXPECbuffer);
                if (retVal != E_OK) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                    ltc_state.timer = 0;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                    ltc_state.timer = ltc_state.commandDataTransferTime+10;
                }
                    ltc_state.substate = LTC_EXIT_READVOLTAGE;

                break;

            } else if (ltc_state.substate == LTC_EXIT_READVOLTAGE) {

                if (LTC_RX_PECCheck(ltc_RXPECbuffer) != E_OK) {
                    DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_NOK, 0, NULL_PTR);
                    PEC_valid = FALSE;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_OK, 0, NULL_PTR);
                    PEC_valid = TRUE;
                }
                if (BS_MAX_SUPPORTED_CELLS == 12) {
                    LTC_SaveRXtoVoltagebuffer(3, ltc_RXPECbuffer, PEC_valid);
                } else if (BS_MAX_SUPPORTED_CELLS == 15) {
                    LTC_SaveRXtoVoltagebuffer(4, ltc_RXPECbuffer, PEC_valid);
                } else if (BS_MAX_SUPPORTED_CELLS == 18) {
                    LTC_SaveRXtoVoltagebuffer(5, ltc_RXPECbuffer, PEC_valid);
                }

                LTC_SaveVoltages();

                ltc_state.check_spi_flag = FALSE;
                ltc_state.state = LTC_STATEMACH_MUXMEASUREMENT;
                ltc_state.substate = LTC_STATEMACH_MUXCONFIGURATION_INIT;
                ltc_state.timer = 0;

            }

            break;


        /****************************MULTIPLEXED MEASUREMENT CONFIGURATION***********/
        case LTC_STATEMACH_MUXMEASUREMENT:

            if (ltc_state.substate == LTC_STATEMACH_MUXCONFIGURATION_INIT) {

                ltc_state.adcMode = LTC_GPIO_MEASUREMENT_MODE;
                ltc_state.adcMeasCh = LTC_ADCMEAS_SINGLECHANNEL_GPIO1;

                if (ltc_state.muxmeas_seqptr >= ltc_state.muxmeas_seqendptr) {
                    /* last step of sequence reached (or no sequence configured) */

                    ltc_state.muxmeas_seqptr = ltc_mux_seq.seqptr;
                    ltc_state.muxmeas_nr_end = ltc_mux_seq.nr_of_steps;
                    ltc_state.muxmeas_seqendptr = ((LTC_MUX_CH_CFG_s *)ltc_mux_seq.seqptr)+ltc_mux_seq.nr_of_steps;  /* last sequence + 1 */

                    LTC_SaveTemperatures();

                    if (LTC_IsFirstMeasurementCycleFinished() == FALSE) {
                        LTC_SetFirstMeasurementCycleFinished();
                    }

                }

                ltc_state.check_spi_flag = TRUE;
                SPI_SetTransmitOngoing();
                retVal = LTC_SetMuxChannel(ltc_TXBuffer, ltc_TXPECbuffer,
                                            ltc_state.muxmeas_seqptr->muxID,  /* mux */
                                            ltc_state.muxmeas_seqptr->muxCh  /* channel */);
                if (retVal != E_OK) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                    ++ltc_state.muxmeas_seqptr;
                    ltc_state.state = LTC_STATEMACH_MUXMEASUREMENT;
                    ltc_state.substate = LTC_STATEMACH_MUXCONFIGURATION_INIT;
                    ltc_state.timer = 0;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                    ltc_state.substate = LTC_SEND_CLOCK_STCOMM_MUXMEASUREMENT_CONFIG;
                    ltc_state.timer = ltc_state.commandDataTransferTime+10;
                }

                break;

            } else if (ltc_state.substate == LTC_SEND_CLOCK_STCOMM_MUXMEASUREMENT_CONFIG) {

                if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                }

                SPI_SetTransmitOngoing();
                retVal = LTC_I2CClock(ltc_TXBufferClock, ltc_TXPECBufferClock);
                if (retVal != E_OK) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                    ltc_state.timer = 0;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                    ltc_state.timer = ltc_state.gpioClocksTransferTime+10;
                }

                if (LTC_GOTO_MUX_CHECK == TRUE) {
                    ltc_state.substate = LTC_READ_I2C_TRANSMISSION_RESULT_RDCOMM_MUXMEASUREMENT_CONFIG;
                } else {
                    ltc_state.substate = LTC_STATEMACH_MUXMEASUREMENT;
                }

                break;

            } else if (ltc_state.substate == LTC_READ_I2C_TRANSMISSION_RESULT_RDCOMM_MUXMEASUREMENT_CONFIG) {

                if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                }

                SPI_SetTransmitOngoing();
                retVal = LTC_RX((uint8_t*)ltc_cmdRDCOMM, ltc_RXPECbuffer);
                if (retVal != E_OK) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                    ltc_state.timer = 0;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                    ltc_state.timer = ltc_state.commandDataTransferTime;
                }

                ltc_state.substate = LTC_READ_I2C_TRANSMISSION_CHECK_MUXMEASUREMENT_CONFIG;

                break;

            } else if (ltc_state.substate == LTC_READ_I2C_TRANSMISSION_CHECK_MUXMEASUREMENT_CONFIG) {

                if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                }

                if (LTC_RX_PECCheck(ltc_RXPECbuffer) != E_OK) {
                    DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_NOK, 0, NULL_PTR);
                } else {
                    DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_OK, 0, NULL_PTR);
                }
                /* if CRC OK: check multiplexer answer on i2C bus */
                if (LTC_I2CCheckACK(ltc_RXPECbuffer, ltc_state.muxmeas_seqptr->muxID) != E_OK) {
                    DIAG_Handler(DIAG_CH_LTC_MUX, DIAG_EVENT_NOK, 0, NULL_PTR);
                } else {
                    DIAG_Handler(DIAG_CH_LTC_MUX, DIAG_EVENT_OK, 0, NULL_PTR);
                }
                ltc_state.substate = LTC_STATEMACH_MUXMEASUREMENT;
                ltc_state.timer = 0;

                break;

            } else if (ltc_state.substate == LTC_STATEMACH_MUXMEASUREMENT) {

                if (ltc_state.muxmeas_seqptr->muxCh == 0xFF) {
                    /* actual multiplexer is switched off, so do not make a measurement and follow up with next step (mux configuration) */
                    ++ltc_state.muxmeas_seqptr;         /*  go further with next step of sequence
                                                            ltc_state.numberOfMeasuredMux not decremented, this does not count as a measurement */
                    ltc_state.state = LTC_STATEMACH_STARTMEAS;
                    ltc_state.substate = LTC_ENTRY;
                    ltc_state.timer = 0;
                    break;
                } else {

                    if (LTC_GOTO_MUX_CHECK == FALSE) {
                        if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        }
                    }

                    ltc_state.check_spi_flag = FALSE;
                    /* user multiplexer type -> connected to GPIO2! */
                    if (ltc_state.muxmeas_seqptr->muxID == 1 || ltc_state.muxmeas_seqptr->muxID == 2) {
                        retVal = LTC_StartGPIOMeasurement(ltc_state.adcMode, LTC_ADCMEAS_SINGLECHANNEL_GPIO2);
                    } else {
                        retVal = LTC_StartGPIOMeasurement(ltc_state.adcMode, LTC_ADCMEAS_SINGLECHANNEL_GPIO1);
                    }
                }
                if (retVal != E_OK) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                    ltc_state.timer = 0;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                    /* ltc_state.timer = ltc_state.commandTransferTime + LTC_Get_MeasurementTCycle(ltc_state.adcMode, ltc_state.adcMeasCh);    wait, ADAX-Command */
                    ltc_state.timer = ltc_state.commandTransferTime + LTC_Get_MeasurementTCycle(ltc_state.adcMode, LTC_ADCMEAS_SINGLECHANNEL_GPIO2);  /*  wait, ADAX-Command */
                }

                ltc_state.substate = LTC_STATEMACH_READMUXMEASUREMENT;

                break;

            } else if (ltc_state.substate == LTC_STATEMACH_READMUXMEASUREMENT) {

                ltc_state.check_spi_flag = TRUE;

                SPI_SetTransmitOngoing();
                retVal = LTC_RX((uint8_t*)(ltc_cmdRDAUXA), ltc_RXPECbuffer);
                if (retVal != E_OK) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                    ltc_state.timer = 0;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                    ltc_state.timer = ltc_state.commandDataTransferTime+10;
                }
                    ltc_state.substate = LTC_STATEMACH_STOREMUXMEASUREMENT;

                break;

            } else if (ltc_state.substate == LTC_STATEMACH_STOREMUXMEASUREMENT) {

                if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                }

                if (LTC_RX_PECCheck(ltc_RXPECbuffer) != E_OK) {
                    DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_NOK, 0, NULL_PTR);
                } else {
                    DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_OK, 0, NULL_PTR);
                    LTC_SaveMuxMeasurement(ltc_RXPECbuffer, ltc_state.muxmeas_seqptr);
                }

                ++ltc_state.muxmeas_seqptr;

                ltc_state.timer = 0;
                if (ltc_state.balance_control_done == TRUE) {
                    statereq = LTC_TransferStateRequest(&tmpbusID, &tmpadcMode, &tmpadcMeasCh);
                    if (statereq == LTC_STATE_USER_IO_WRITE_REQUEST) {
                        ltc_state.state = LTC_STATEMACH_USER_IO_CONTROL;
                        ltc_state.substate = LTC_USER_IO_SET_OUTPUT_REGISTER;
                        ltc_state.balance_control_done = FALSE;
                    } else if (statereq == LTC_STATE_USER_IO_READ_REQUEST) {
                        ltc_state.state = LTC_STATEMACH_USER_IO_FEEDBACK;
                        ltc_state.substate = LTC_USER_IO_READ_INPUT_REGISTER;
                        ltc_state.balance_control_done = FALSE;
                    } else if (statereq == LTC_STATE_EEPROM_READ_REQUEST) {
                        ltc_state.state = LTC_STATEMACH_EEPROM_READ;
                        ltc_state.substate = LTC_EEPROM_READ_DATA1;
                        ltc_state.balance_control_done = FALSE;
                    } else if (statereq == LTC_STATE_EEPROM_WRITE_REQUEST) {
                        ltc_state.state = LTC_STATEMACH_EEPROM_WRITE;
                        ltc_state.substate = LTC_EEPROM_WRITE_DATA1;
                        ltc_state.balance_control_done = FALSE;
                    } else if (statereq == LTC_STATE_TEMP_SENS_READ_REQUEST) {
                        ltc_state.state = LTC_STATEMACH_TEMP_SENS_READ;
                        ltc_state.substate = LTC_TEMP_SENS_SEND_DATA1;
                        ltc_state.balance_control_done = FALSE;
                    } else if (statereq == LTC_STATEMACH_BALANCEFEEDBACK_REQUEST) {
                        ltc_state.state = LTC_STATEMACH_BALANCEFEEDBACK;
                        ltc_state.substate = LTC_ENTRY;
                        ltc_state.balance_control_done = FALSE;
                    } else {
                        ltc_state.state = LTC_STATEMACH_BALANCECONTROL;
                        ltc_state.substate = LTC_CONFIG_BALANCECONTROL;
                        ltc_state.balance_control_done = TRUE;
                    }
                } else {
                    ltc_state.state = LTC_STATEMACH_BALANCECONTROL;
                    ltc_state.substate = LTC_CONFIG_BALANCECONTROL;
                    ltc_state.balance_control_done = TRUE;
                }

                break;

            }

            break;

        /****************************BALANCE CONTROL*********************************/
        case LTC_STATEMACH_BALANCECONTROL:

            if (ltc_state.substate == LTC_CONFIG_BALANCECONTROL) {

                SPI_SetTransmitOngoing();
                retVal = LTC_BalanceControl(0);
                if (retVal != E_OK) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                    ltc_state.timer = 0;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                    ltc_state.timer = ltc_state.commandDataTransferTime;
                }
                ltc_state.substate = LTC_CONFIG2_BALANCECONTROL;

                break;

            } else if (ltc_state.substate == LTC_CONFIG2_BALANCECONTROL) {

                if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                    ltc_state.state = LTC_STATEMACH_STARTMEAS;
                    ltc_state.substate = LTC_ENTRY;
                    ltc_state.timer = 0;
                    break;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                }

                if (BS_NR_OF_BAT_CELLS_PER_MODULE > 12) {
                    SPI_SetTransmitOngoing();
                    retVal = LTC_BalanceControl(1);
                    if (retVal != E_OK) {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                        ltc_state.timer = 0;
                    } else {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        ltc_state.timer = ltc_state.commandDataTransferTime;
                    }
                    ltc_state.substate = LTC_CONFIG2_BALANCECONTROL_END;
                } else {
                    /* 12 cells, balancing control finished */
                    ltc_state.check_spi_flag = FALSE;
                    ltc_state.timer = 0;
                    ltc_state.state = LTC_STATEMACH_STARTMEAS;
                    ltc_state.substate = LTC_ENTRY;
                }

                break;

            } else if (ltc_state.substate == LTC_CONFIG2_BALANCECONTROL_END) {

                if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                    ltc_state.state = LTC_STATEMACH_STARTMEAS;
                    ltc_state.substate = LTC_ENTRY;
                    ltc_state.timer = 0;
                    break;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                }
                /* More than 12 cells, balancing control finished */
                ltc_state.check_spi_flag = FALSE;
                ltc_state.timer = 0;
                ltc_state.state = LTC_STATEMACH_STARTMEAS;
                ltc_state.substate = LTC_ENTRY;

                break;
            }
            break;

            /****************************START MEASUREMENT*******************************/
            case LTC_STATEMACH_ALLGPIOMEASUREMENT:

                ltc_state.adcMode = LTC_GPIO_MEASUREMENT_MODE;
                ltc_state.adcMeasCh = LTC_ADCMEAS_ALLCHANNEL;

                ltc_state.check_spi_flag = FALSE;
                retVal = LTC_StartGPIOMeasurement(ltc_state.adcMode, ltc_state.adcMeasCh);

                if ((retVal != E_OK)) {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                    ltc_state.timer = LTC_STATEMACH_SHORTTIME;
                } else {
                    DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                    ltc_state.timer = ltc_state.commandTransferTime + LTC_Get_MeasurementTCycle(ltc_state.adcMode, ltc_state.adcMeasCh);
                    ltc_state.state = LTC_STATEMACH_READALLGPIO;
                    ltc_state.substate = LTC_READ_AUXILIARY_REGISTER_A_RDAUXA;
                }

                break;

            /****************************READ ALL GPIO VOLTAGE************************************/
            case LTC_STATEMACH_READALLGPIO:

                if (ltc_state.substate == LTC_READ_AUXILIARY_REGISTER_A_RDAUXA) {

                    ltc_state.check_spi_flag = TRUE;
                    SPI_SetTransmitOngoing();
                    retVal = LTC_RX((uint8_t*)(ltc_cmdRDAUXA), ltc_RXPECbuffer);
                    if (retVal != E_OK) {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                        ltc_state.timer = 0;
                    } else {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        ltc_state.timer = ltc_state.commandDataTransferTime+10;
                    }
                        ltc_state.substate = LTC_READ_AUXILIARY_REGISTER_B_RDAUXB;

                    break;

                } else if (ltc_state.substate == LTC_READ_AUXILIARY_REGISTER_B_RDAUXB) {

                    if (LTC_RX_PECCheck(ltc_RXPECbuffer) != E_OK) {
                        DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_NOK, 0, NULL_PTR);
                    } else {
                        DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_OK, 0, NULL_PTR);
                        LTC_SaveRXtoGPIOBuffer(0, ltc_RXPECbuffer);
                    }

                    SPI_SetTransmitOngoing();
                    retVal = LTC_RX((uint8_t*)(ltc_cmdRDAUXB), ltc_RXPECbuffer);
                    if (retVal != E_OK) {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                        ltc_state.timer = 0;
                    } else {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        ltc_state.timer = ltc_state.commandDataTransferTime+10;
                    }

                    if (BS_MAX_SUPPORTED_CELLS > 12) {
                        ltc_state.substate = LTC_READ_AUXILIARY_REGISTER_C_RDAUXC;
                    } else {
                        ltc_state.substate = LTC_EXIT_READAUXILIARY_ALLGPIOS;
                    }

                    break;

                } else if (ltc_state.substate == LTC_READ_AUXILIARY_REGISTER_C_RDAUXC) {

                    if (LTC_RX_PECCheck(ltc_RXPECbuffer) != E_OK) {
                        DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_NOK, 0, NULL_PTR);
                    } else {
                        DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_OK, 0, NULL_PTR);
                        LTC_SaveRXtoGPIOBuffer(1, ltc_RXPECbuffer);
                    }

                    SPI_SetTransmitOngoing();
                    retVal = LTC_RX((uint8_t*)(ltc_cmdRDAUXC), ltc_RXPECbuffer);
                    if (retVal != E_OK) {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                        ltc_state.timer = 0;
                    } else {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        ltc_state.timer = ltc_state.commandDataTransferTime+10;
                    }
                        ltc_state.substate = LTC_READ_AUXILIARY_REGISTER_D_RDAUXD;

                    break;

                } else if (ltc_state.substate == LTC_READ_AUXILIARY_REGISTER_D_RDAUXD) {

                    if (LTC_RX_PECCheck(ltc_RXPECbuffer) != E_OK) {
                        DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_NOK, 0, NULL_PTR);
                    } else {
                        DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_OK, 0, NULL_PTR);
                        LTC_SaveRXtoGPIOBuffer(2, ltc_RXPECbuffer);
                    }

                    SPI_SetTransmitOngoing();
                    retVal = LTC_RX((uint8_t*)(ltc_cmdRDAUXD), ltc_RXPECbuffer);
                    if (retVal != E_OK) {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                        ltc_state.timer = 0;
                    } else {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        ltc_state.timer = ltc_state.commandDataTransferTime+10;
                    }
                        ltc_state.substate = LTC_EXIT_READAUXILIARY_ALLGPIOS;

                    break;

                } else if (ltc_state.substate == LTC_EXIT_READAUXILIARY_ALLGPIOS) {

                    if (LTC_RX_PECCheck(ltc_RXPECbuffer) != E_OK) {
                        DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_NOK, 0, NULL_PTR);
                    } else {
                        DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_OK, 0, NULL_PTR);
                        if (BS_MAX_SUPPORTED_CELLS == 12) {
                            LTC_SaveRXtoGPIOBuffer(1, ltc_RXPECbuffer);
                        } else if (BS_MAX_SUPPORTED_CELLS > 12) {
                            LTC_SaveRXtoGPIOBuffer(3, ltc_RXPECbuffer);
                        }
                    }

                    LTC_SaveAllGPIOMeasurement();

                    ltc_state.timer = 0;
                    if (ltc_state.balance_control_done == TRUE) {
                        statereq = LTC_TransferStateRequest(&tmpbusID, &tmpadcMode, &tmpadcMeasCh);
                        if (statereq == LTC_STATE_USER_IO_WRITE_REQUEST) {
                            ltc_state.state = LTC_STATEMACH_USER_IO_CONTROL;
                            ltc_state.substate = LTC_USER_IO_SET_OUTPUT_REGISTER;
                            ltc_state.balance_control_done = FALSE;
                        } else if (statereq == LTC_STATE_USER_IO_READ_REQUEST) {
                            ltc_state.state = LTC_STATEMACH_USER_IO_FEEDBACK;
                            ltc_state.substate = LTC_USER_IO_READ_INPUT_REGISTER;
                            ltc_state.balance_control_done = FALSE;
                        } else if (statereq == LTC_STATE_EEPROM_READ_REQUEST) {
                            ltc_state.state = LTC_STATEMACH_EEPROM_READ;
                            ltc_state.substate = LTC_EEPROM_READ_DATA1;
                            ltc_state.balance_control_done = FALSE;
                        } else if (statereq == LTC_STATE_EEPROM_WRITE_REQUEST) {
                            ltc_state.state = LTC_STATEMACH_EEPROM_WRITE;
                            ltc_state.substate = LTC_EEPROM_WRITE_DATA1;
                            ltc_state.balance_control_done = FALSE;
                        } else if (statereq == LTC_STATE_TEMP_SENS_READ_REQUEST) {
                            ltc_state.state = LTC_STATEMACH_TEMP_SENS_READ;
                            ltc_state.substate = LTC_TEMP_SENS_SEND_DATA1;
                            ltc_state.balance_control_done = FALSE;
                        } else if (statereq == LTC_STATEMACH_BALANCEFEEDBACK_REQUEST) {
                            ltc_state.state = LTC_STATEMACH_BALANCEFEEDBACK;
                            ltc_state.substate = LTC_ENTRY;
                            ltc_state.balance_control_done = FALSE;
                        } else {
                            ltc_state.state = LTC_STATEMACH_BALANCECONTROL;
                            ltc_state.substate = LTC_CONFIG_BALANCECONTROL;
                            ltc_state.balance_control_done = TRUE;
                        }
                    } else {
                        ltc_state.state = LTC_STATEMACH_BALANCECONTROL;
                        ltc_state.substate = LTC_CONFIG_BALANCECONTROL;
                        ltc_state.balance_control_done = TRUE;
                    }

                }

                break;


            /****************************BALANCE FEEDBACK*********************************/
            case LTC_STATEMACH_BALANCEFEEDBACK:

                 if (ltc_state.substate == LTC_ENTRY) {

                    ltc_state.adcMode = LTC_ADCMODE_NORMAL_DCP0;
                    ltc_state.adcMeasCh = LTC_ADCMEAS_SINGLECHANNEL_GPIO3;

                    ltc_state.check_spi_flag = FALSE;
                    retVal = LTC_StartGPIOMeasurement(ltc_state.adcMode, ltc_state.adcMeasCh);
                    if (retVal != E_OK) {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                        ltc_state.timer = 0;
                    } else {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        ltc_state.timer = ltc_state.commandDataTransferTime + LTC_Get_MeasurementTCycle(ltc_state.adcMode, ltc_state.adcMeasCh);;
                    }
                    ltc_state.substate = LTC_READ_FEEDBACK_BALANCECONTROL;
                    break;

                } else if (ltc_state.substate == LTC_READ_FEEDBACK_BALANCECONTROL) {

                    ltc_state.check_spi_flag = TRUE;
                    SPI_SetTransmitOngoing();
                    retVal = LTC_RX((uint8_t*)ltc_cmdRDAUXA, ltc_RXPECbuffer);  /* read AUXA register */
                    if (retVal != E_OK) {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                        ltc_state.timer = 0;
                    } else {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        ltc_state.timer = ltc_state.commandDataTransferTime;
                    }
                    ltc_state.substate = LTC_SAVE_FEEDBACK_BALANCECONTROL;

                } else if (ltc_state.substate == LTC_SAVE_FEEDBACK_BALANCECONTROL) {

                    if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                        ltc_state.state = LTC_STATEMACH_STARTMEAS;
                        ltc_state.substate = LTC_ENTRY;
                        ltc_state.timer = 0;
                        break;
                    } else {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                    }

                    if (LTC_RX_PECCheck(ltc_RXPECbuffer) != E_OK) {
                        DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_NOK, 0, NULL_PTR);
                    } else {
                        DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_OK, 0, NULL_PTR);
                        LTC_SaveBalancingFeedback(ltc_RXPECbuffer);
                    }

                    ltc_state.state = LTC_STATEMACH_STARTMEAS;
                    ltc_state.substate = LTC_ENTRY;
                    ltc_state.timer = 0;

                    break;
                }

                break;

                /****************************BOARD TEMPERATURE SENSOR*********************************/
                case LTC_STATEMACH_TEMP_SENS_READ:

                    if (ltc_state.substate == LTC_TEMP_SENS_SEND_DATA1) {

                        ltc_state.check_spi_flag = TRUE;
                        SPI_SetTransmitOngoing();
                        retVal = LTC_Send_I2C_Command(ltc_TXBuffer, ltc_TXPECbuffer, (uint8_t*)ltc_I2CcmdTempSens0);

                        if (retVal != E_OK) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ++ltc_state.muxmeas_seqptr;
                            ltc_state.state = LTC_STATEMACH_STARTMEAS;
                            ltc_state.substate = LTC_ENTRY;
                            ltc_state.timer = 0;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                            ltc_state.substate = LTC_TEMP_SENS_SEND_CLOCK_STCOMM1;
                            ltc_state.timer = ltc_state.commandDataTransferTime+10;
                        }

                        break;

                    } else if (ltc_state.substate == LTC_TEMP_SENS_SEND_CLOCK_STCOMM1) {

                        if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.state = LTC_STATEMACH_STARTMEAS;
                            ltc_state.substate = LTC_ENTRY;
                            ltc_state.timer = 0;
                            break;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        }

                        SPI_SetTransmitOngoing();
                        retVal = LTC_I2CClock(ltc_TXBufferClock, ltc_TXPECBufferClock);
                        if (retVal != E_OK) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.timer = 0;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                            ltc_state.timer = ltc_state.gpioClocksTransferTime+10;
                        }

                        ltc_state.substate = LTC_TEMP_SENS_READ_DATA1;
                        break;

                    } else if (ltc_state.substate == LTC_TEMP_SENS_READ_DATA1) {

                        if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.state = LTC_STATEMACH_STARTMEAS;
                            ltc_state.substate = LTC_ENTRY;
                            ltc_state.timer = 0;
                            break;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        }

                        SPI_SetTransmitOngoing();
                        retVal = LTC_Send_I2C_Command(ltc_TXBuffer, ltc_TXPECbuffer, (uint8_t*)ltc_I2CcmdTempSens1);

                        if (retVal != E_OK) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ++ltc_state.muxmeas_seqptr;
                            ltc_state.state = LTC_STATEMACH_STARTMEAS;
                            ltc_state.substate = LTC_ENTRY;
                            ltc_state.timer = 0;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                            ltc_state.substate = LTC_TEMP_SENS_SEND_CLOCK_STCOMM2;
                            ltc_state.timer = ltc_state.commandDataTransferTime+10;
                        }

                        break;

                    } else if (ltc_state.substate == LTC_TEMP_SENS_SEND_CLOCK_STCOMM2) {

                        if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.state = LTC_STATEMACH_STARTMEAS;
                            ltc_state.substate = LTC_ENTRY;
                            ltc_state.timer = 0;
                            break;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        }

                        SPI_SetTransmitOngoing();
                        retVal = LTC_I2CClock(ltc_TXBufferClock, ltc_TXPECBufferClock);
                        if (retVal != E_OK) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.timer = 0;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                            ltc_state.timer = ltc_state.gpioClocksTransferTime+10;
                        }

                        ltc_state.substate = LTC_TEMP_SENS_READ_I2C_TRANSMISSION_RESULT_RDCOMM;
                        break;



                    }  else if (ltc_state.substate == LTC_TEMP_SENS_READ_I2C_TRANSMISSION_RESULT_RDCOMM) {

                        if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.state = LTC_STATEMACH_STARTMEAS;
                            ltc_state.substate = LTC_ENTRY;
                            ltc_state.timer = 0;
                            break;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        }

                        SPI_SetTransmitOngoing();
                        retVal = LTC_RX((uint8_t*)ltc_cmdRDCOMM, ltc_RXPECbuffer);
                        if (retVal != E_OK) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.timer = 0;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                            ltc_state.timer = ltc_state.commandDataTransferTime;
                        }

                        ltc_state.substate = LTC_TEMP_SENS_SAVE_TEMP;

                        break;

                    } else if (ltc_state.substate == LTC_TEMP_SENS_SAVE_TEMP) {

                        if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.state = LTC_STATEMACH_STARTMEAS;
                            ltc_state.substate = LTC_ENTRY;
                            ltc_state.timer = 0;
                            break;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        }

                        if (LTC_RX_PECCheck(ltc_RXPECbuffer) != E_OK) {
                            DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_NOK, 0, NULL_PTR);
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_OK, 0, NULL_PTR);
                            LTC_TempSensSaveTemp(ltc_RXPECbuffer);
                        }

                        ltc_state.state = LTC_STATEMACH_STARTMEAS;
                        ltc_state.substate = LTC_ENTRY;
                        ltc_state.timer = 0;

                        break;
                    }

                    break;

                /****************************MULTIPLEXED MEASUREMENT CONFIGURATION***********/
                case LTC_STATEMACH_USER_IO_CONTROL:

                    if (ltc_state.substate == LTC_USER_IO_SET_OUTPUT_REGISTER) {

                        ltc_state.check_spi_flag = TRUE;
                        SPI_SetTransmitOngoing();
                        retVal = LTC_SetPortExpander(ltc_TXBuffer, ltc_TXPECbuffer);

                        if (retVal != E_OK) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ++ltc_state.muxmeas_seqptr;
                            ltc_state.state = LTC_STATEMACH_STARTMEAS;
                            ltc_state.substate = LTC_ENTRY;
                            ltc_state.timer = 0;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                            ltc_state.substate = LTC_SEND_CLOCK_STCOMM_MUXMEASUREMENT_CONFIG;
                            ltc_state.timer = ltc_state.commandDataTransferTime+10;
                        }

                        break;

                    } else if (ltc_state.substate == LTC_SEND_CLOCK_STCOMM_MUXMEASUREMENT_CONFIG) {

                        if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.state = LTC_STATEMACH_STARTMEAS;
                            ltc_state.substate = LTC_ENTRY;
                            ltc_state.timer = 0;
                            break;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        }

                        ltc_state.check_spi_flag = FALSE;
                        retVal = LTC_I2CClock(ltc_TXBufferClock, ltc_TXPECBufferClock);
                        if (retVal != E_OK) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.timer = 0;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                            ltc_state.timer = ltc_state.gpioClocksTransferTime;
                        }

                        ltc_state.state = LTC_STATEMACH_STARTMEAS;
                        ltc_state.substate = LTC_ENTRY;

                        break;
                        }
                    break;

            /****************************MULTIPLEXED MEASUREMENT CONFIGURATION***********/
            case LTC_STATEMACH_USER_IO_FEEDBACK:

                if (ltc_state.substate == LTC_USER_IO_READ_INPUT_REGISTER) {

                    ltc_state.check_spi_flag = TRUE;
                    SPI_SetTransmitOngoing();
                    retVal = LTC_Send_I2C_Command(ltc_TXBuffer, ltc_TXPECbuffer, (uint8_t*)ltc_I2CcmdPortExpander1);

                    if (retVal != E_OK) {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                        ++ltc_state.muxmeas_seqptr;
                        ltc_state.state = LTC_STATEMACH_STARTMEAS;
                        ltc_state.substate = LTC_ENTRY;
                        ltc_state.timer = 0;
                    } else {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        ltc_state.substate = LTC_USER_IO_SEND_CLOCK_STCOMM;
                        ltc_state.timer = ltc_state.commandDataTransferTime+10;
                    }

                    break;

                } else if (ltc_state.substate == LTC_USER_IO_SEND_CLOCK_STCOMM) {

                    if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                        ltc_state.state = LTC_STATEMACH_STARTMEAS;
                        ltc_state.substate = LTC_ENTRY;
                        ltc_state.timer = 0;
                        break;
                    } else {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                    }

                    ltc_state.check_spi_flag = FALSE;
                    retVal = LTC_I2CClock(ltc_TXBufferClock, ltc_TXPECBufferClock);
                    if (retVal != E_OK) {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                        ltc_state.timer = 0;
                    } else {
                        DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        ltc_state.timer = ltc_state.gpioClocksTransferTime;
                    }

                    ltc_state.substate = LTC_USER_IO_READ_I2C_TRANSMISSION_RESULT_RDCOMM;

                    break;

                    } else if (ltc_state.substate == LTC_USER_IO_READ_I2C_TRANSMISSION_RESULT_RDCOMM) {

                        if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.state = LTC_STATEMACH_STARTMEAS;
                            ltc_state.substate = LTC_ENTRY;
                            ltc_state.timer = 0;
                            break;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        }

                        SPI_SetTransmitOngoing();
                        retVal = LTC_RX((uint8_t*)ltc_cmdRDCOMM, ltc_RXPECbuffer);
                        if (retVal != E_OK) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.timer = 0;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                            ltc_state.timer = ltc_state.commandDataTransferTime;
                        }

                        ltc_state.substate = LTC_USER_IO_SAVE_DATA;

                        break;

                    } else if (ltc_state.substate == LTC_USER_IO_SAVE_DATA) {

                        if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.state = LTC_STATEMACH_STARTMEAS;
                            ltc_state.substate = LTC_ENTRY;
                            ltc_state.timer = 0;
                            break;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        }

                        if (LTC_RX_PECCheck(ltc_RXPECbuffer) != E_OK) {
                            DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_NOK, 0, NULL_PTR);
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_OK, 0, NULL_PTR);
                            LTC_PortExpanderSaveValues(ltc_RXPECbuffer);
                        }

                        ltc_state.state = LTC_STATEMACH_STARTMEAS;
                        ltc_state.substate = LTC_ENTRY;
                        ltc_state.timer = 0;

                        break;
                    }

                break;

                /****************************EEPROM READ*********************************/
                case LTC_STATEMACH_EEPROM_READ:

                    if (ltc_state.substate == LTC_EEPROM_READ_DATA1) {

                        ltc_state.check_spi_flag = TRUE;
                        SPI_SetTransmitOngoing();
                        retVal = LTC_SendEEPROMReadCommand(ltc_TXBuffer, ltc_TXPECbuffer, 0);

                        if (retVal != E_OK) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ++ltc_state.muxmeas_seqptr;
                            ltc_state.state = LTC_STATEMACH_STARTMEAS;
                            ltc_state.substate = LTC_ENTRY;
                            ltc_state.timer = 0;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                            ltc_state.substate = LTC_EEPROM_SEND_CLOCK_STCOMM1;
                            ltc_state.timer = ltc_state.commandDataTransferTime+10;
                        }

                        break;

                    } else if (ltc_state.substate == LTC_EEPROM_SEND_CLOCK_STCOMM1) {

                        if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.state = LTC_STATEMACH_STARTMEAS;
                            ltc_state.substate = LTC_ENTRY;
                            ltc_state.timer = 0;
                            break;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        }

                        SPI_SetTransmitOngoing();
                        retVal = LTC_I2CClock(ltc_TXBufferClock, ltc_TXPECBufferClock);
                        if (retVal != E_OK) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.timer = 0;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                            ltc_state.timer = ltc_state.gpioClocksTransferTime+10;
                        }

                        ltc_state.substate = LTC_EEPROM_READ_DATA2;
                        break;

                    } else if (ltc_state.substate == LTC_EEPROM_READ_DATA2) {

                        if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.state = LTC_STATEMACH_STARTMEAS;
                            ltc_state.substate = LTC_ENTRY;
                            ltc_state.timer = 0;
                            break;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        }

                        SPI_SetTransmitOngoing();
                        retVal = LTC_SendEEPROMReadCommand(ltc_TXBuffer, ltc_TXPECbuffer, 1);

                        if (retVal != E_OK) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.timer = 0;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                            ltc_state.timer = ltc_state.commandDataTransferTime+10;
                        }

                        ltc_state.substate = LTC_EEPROM_SEND_CLOCK_STCOMM2;

                        break;

                    } else if (ltc_state.substate == LTC_EEPROM_SEND_CLOCK_STCOMM2) {

                        if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.state = LTC_STATEMACH_STARTMEAS;
                            ltc_state.substate = LTC_ENTRY;
                            ltc_state.timer = 0;
                            break;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        }

                        SPI_SetTransmitOngoing();
                        retVal = LTC_I2CClock(ltc_TXBufferClock, ltc_TXPECBufferClock);
                        if (retVal != E_OK) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.timer = 0;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                            ltc_state.timer = ltc_state.gpioClocksTransferTime+10;
                        }

                        ltc_state.substate = LTC_EEPROM_READ_I2C_TRANSMISSION_RESULT_RDCOMM;
                        break;

                    }  else if (ltc_state.substate == LTC_EEPROM_READ_I2C_TRANSMISSION_RESULT_RDCOMM) {

                        if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.state = LTC_STATEMACH_STARTMEAS;
                            ltc_state.substate = LTC_ENTRY;
                            ltc_state.timer = 0;
                            break;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        }

                        SPI_SetTransmitOngoing();
                        retVal = LTC_RX((uint8_t*)ltc_cmdRDCOMM, ltc_RXPECbuffer);
                        if (retVal != E_OK) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.timer = 0;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                            ltc_state.timer = ltc_state.commandDataTransferTime+10;
                        }

                        ltc_state.substate = LTC_EEPROM_SAVE_READ;

                        break;

                    } else if (ltc_state.substate == LTC_EEPROM_SAVE_READ) {

                        if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                            ltc_state.state = LTC_STATEMACH_STARTMEAS;
                            ltc_state.substate = LTC_ENTRY;
                            ltc_state.timer = 0;
                            break;
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                        }

                        if (LTC_RX_PECCheck(ltc_RXPECbuffer) != E_OK) {
                            DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_NOK, 0, NULL_PTR);
                        } else {
                            DIAG_Handler(DIAG_CH_LTC_PEC, DIAG_EVENT_OK, 0, NULL_PTR);
                            LTC_EEPROMSaveReadValue(ltc_RXPECbuffer);
                        }

                        ltc_state.state = LTC_STATEMACH_STARTMEAS;
                        ltc_state.substate = LTC_ENTRY;
                        ltc_state.timer = 0;

                        break;

                    }

                    break;

                    /****************************EEPROM READ*********************************/
                    case LTC_STATEMACH_EEPROM_WRITE:

                        if (ltc_state.substate == LTC_EEPROM_WRITE_DATA1) {

                            ltc_state.check_spi_flag = TRUE;
                            SPI_SetTransmitOngoing();
                            retVal = LTC_SendEEPROMWriteCommand(ltc_TXBuffer, ltc_TXPECbuffer, 0);

                            if (retVal != E_OK) {
                                DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                                ++ltc_state.muxmeas_seqptr;
                                ltc_state.state = LTC_STATEMACH_STARTMEAS;
                                ltc_state.substate = LTC_ENTRY;
                                ltc_state.timer = 0;
                            } else {
                                DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                                ltc_state.substate = LTC_EEPROM_SEND_CLOCK_STCOMM3;
                                ltc_state.timer = ltc_state.commandDataTransferTime+10;
                            }

                            break;

                        } else if (ltc_state.substate == LTC_EEPROM_SEND_CLOCK_STCOMM3) {

                            if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                                DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                                ltc_state.state = LTC_STATEMACH_STARTMEAS;
                                ltc_state.substate = LTC_ENTRY;
                                ltc_state.timer = 0;
                                break;
                            } else {
                                DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                            }

                            SPI_SetTransmitOngoing();
                            retVal = LTC_I2CClock(ltc_TXBufferClock, ltc_TXPECBufferClock);
                            if (retVal != E_OK) {
                                DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                                ltc_state.timer = 0;
                            } else {
                                DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                                ltc_state.timer = ltc_state.gpioClocksTransferTime+10;
                            }

                            ltc_state.substate = LTC_EEPROM_WRITE_DATA2;
                            break;

                        } else if (ltc_state.substate == LTC_EEPROM_WRITE_DATA2) {

                            if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                                DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                                ltc_state.state = LTC_STATEMACH_STARTMEAS;
                                ltc_state.substate = LTC_ENTRY;
                                ltc_state.timer = 0;
                                break;
                            } else {
                                DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                            }

                            SPI_SetTransmitOngoing();
                            retVal = LTC_SendEEPROMWriteCommand(ltc_TXBuffer, ltc_TXPECbuffer, 1);

                            if (retVal != E_OK) {
                                DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                                ltc_state.timer = 0;
                            } else {
                                DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                                ltc_state.timer = ltc_state.commandDataTransferTime+10;
                            }

                            ltc_state.substate = LTC_EEPROM_SEND_CLOCK_STCOMM4;

                            break;

                        } else if (ltc_state.substate == LTC_EEPROM_SEND_CLOCK_STCOMM4) {

                            if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                                DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                                ltc_state.state = LTC_STATEMACH_STARTMEAS;
                                ltc_state.substate = LTC_ENTRY;
                                ltc_state.timer = 0;
                                break;
                            } else {
                                DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                            }

                            SPI_SetTransmitOngoing();
                            retVal = LTC_I2CClock(ltc_TXBufferClock, ltc_TXPECBufferClock);
                            if (retVal != E_OK) {
                                DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                                ltc_state.timer = 0;
                            } else {
                                DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                                ltc_state.timer = ltc_state.gpioClocksTransferTime+10;
                            }

                            ltc_state.substate = LTC_EEPROM_FINISHED;
                            break;

                        }  else if (ltc_state.substate == LTC_EEPROM_FINISHED) {

                            if (ltc_state.timer == 0 && SPI_IsTransmitOngoing() == TRUE) {
                                DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_NOK, 0, NULL_PTR);
                                ltc_state.state = LTC_STATEMACH_STARTMEAS;
                                ltc_state.substate = LTC_ENTRY;
                                ltc_state.timer = 0;
                                break;
                            } else {
                                DIAG_Handler(DIAG_CH_LTC_SPI, DIAG_EVENT_OK, 0, NULL_PTR);
                            }

                            ltc_state.state = LTC_STATEMACH_STARTMEAS;
                            ltc_state.substate = LTC_ENTRY;
                            ltc_state.timer = 0;

                            break;

                        }

                        break;

        /****************************DEFAULT**************************/
        default:
            break;
    }

    ltc_state.triggerentry--;        /* reentrance counter */
}



/**
 * @brief   saves the multiplexer values read from the LTC daisy-chain.
 *
 * After a voltage measurement was initiated on GPIO 1 to read the currently selected
 * multiplexer voltage, the results is read via SPI from the daisy-chain.
 * This function is called to store the result from the transmission in a buffer.
 *
 * @param   *DataBufferSPI_RX   buffer containing the data obtained from the SPI transmission
 * @param   muxseqptr           pointer to the multiplexer sequence, which configures the currently selected multiplexer ID and channel
 */
static void LTC_SaveMuxMeasurement(uint8_t *rxBuffer, LTC_MUX_CH_CFG_s  *muxseqptr) {   /* pointer to measurement Sequence of Mux- and Channel-Configurations (1,0xFF)...(3,0xFF),(0,1),...(0,7)) */

    uint16_t i = 0;
    uint16_t val_ui = 0;
    int16_t temperature = 0;
    uint8_t sensor_idx = 0;
    uint8_t ch_idx = 0;

    if (muxseqptr->muxCh == 0xFF)
        return; /* Channel 0xFF means that the multiplexer is deactivated, therefore no measurement will be made and saved*/

    /* user multiplexer type -> connected to GPIO2! */
    if (muxseqptr->muxID == 1 || muxseqptr->muxID == 2) {
        for (i=0; i < LTC_N_LTC; i++) {
            if (muxseqptr->muxID == 1)
                ch_idx = 0 + muxseqptr->muxCh;    /* channel index 0..7 */
            else
                ch_idx = 8 + muxseqptr->muxCh;    /* channel index 8..15 */

            if (ch_idx < 2*8) {
                val_ui =*((uint16_t *)(&rxBuffer[6+1*i*8]));        /* raw values, all mux on all LTCs */
                ltc_user_mux.value[i*8*2+ch_idx] = (uint16_t)(((float)(val_ui))*100e-6*1000.0);  /* Unit -> in V -> in mV */
            }
        }
    } else {
        /* temperature multiplexer type -> connected to GPIO1! */
        for (i=0; i < LTC_N_LTC; i++) {
            val_ui = *((uint16_t *)(&rxBuffer[4+i*8]));
            /* GPIO voltage in 100uV -> * 0.1 ----  conversion to V from mV * 0.001 ----- -> 0.0001 */
            temperature = (int16_t)LTC_Convert_MuxVoltages_to_Temperatures((float)(val_ui)*0.0001);        /* Unit Celsius */
            sensor_idx = ltc_muxsensortemperatur_cfg[muxseqptr->muxCh];
            /* if wrong configuration: exit and write nothing */
            if (sensor_idx >= BS_NR_OF_TEMP_SENSORS_PER_MODULE)
                return;
            ltc_celltemperature.temperature[i*(BS_NR_OF_TEMP_SENSORS_PER_MODULE)+sensor_idx] = temperature;
        }
    }
}



/**
 * @brief   saves the voltage values read from the LTC daisy-chain.
 *
 * After a voltage measurement was initiated to measure the voltages of the cells,
 * the result is read via SPI from the daisy-chain.
 * There are 6 register to read _(A,B,C,D,E,F) to get all cell voltages.
 * Only one register can be read at a time.
 * This function is called to store the result from the transmission in a buffer.
 *
 * @param   registerSet    voltage register that was read (voltage register A,B,C,D,E or F)
 * @param   *rxBuffer      buffer containing the data obtained from the SPI transmission
 * @param   PEC_valid      tells the functions if the PEC is valid or not, if not, manage indices but do not store
 *
 */
static void LTC_SaveRXtoVoltagebuffer(uint8_t registerSet, uint8_t *rxBuffer, uint8_t PEC_valid) {

    uint16_t i = 0;
    uint16_t j = 0;
    uint16_t i_offset = 0;
    uint16_t voltage_index = 0;
    uint16_t val_ui = 0;
    uint16_t voltage = 0;
    uint8_t incrementations = 0;

    if (registerSet == 0) {
    /* RDCVA command -> voltage register group A */
        i_offset = 0;
    } else if (registerSet == 1) {
    /* RDCVB command -> voltage register group B */
        i_offset = 3;
    } else if (registerSet == 2) {
    /* RDCVC command -> voltage register group C */
        i_offset = 6;
    } else if (registerSet == 3) {
    /* RDCVD command -> voltage register group D */
        i_offset = 9;
    } else if (registerSet == 4) {
    /* RDCVD command -> voltage register group E (only for 15 and 18 cell version) */
        i_offset = 12;
    } else if (registerSet == 5) {
    /* RDCVD command -> voltage register group F (only for 18 cell version) */
        i_offset = 15;
    } else {
        return;
    }

    //////////////////////////////////////////////////
    /* reinitialize index counter at begin of cycle */
    if (i_offset == 0) {
        ltc_used_cells_index = 0;
    }
    //////////////////////////////////////////////////

    /* Retrieve data without command and CRC*/
    for (i=0; i < LTC_N_LTC; i++) {

        incrementations = 0;

        /* parse all three voltages (3 * 2bytes) contained in one register */
        for (j=0; j < 3; j++) {

            /* index considering maximum number of cells */
            voltage_index = j+i_offset;

            if (ltc_voltage_input_used[voltage_index] == 1) {
                val_ui = *((uint16_t *)(&rxBuffer[4+2*j+i*8]));
                voltage = (uint16_t)(((float)(val_ui))*100e-6*1000.0);        /* Unit V -> in mV */
                if (PEC_valid == TRUE) {
                    ltc_cellvoltage.voltage[ltc_used_cells_index+i*(BS_NR_OF_BAT_CELLS_PER_MODULE)] = voltage;
                }

                ltc_used_cells_index++;
                incrementations++;

                if (ltc_used_cells_index > BS_NR_OF_BAT_CELLS_PER_MODULE) {
                    return;
                }

            }

        }
        /* restore start value for next module */
        if (i < LTC_N_LTC-1) {
            ltc_used_cells_index -= incrementations;
        }
    }

}

/**
 * @brief   saves the GPIO voltage values read from the LTC daisy-chain.
 *
 * After a voltage measurement was initiated to measure the voltages on all GPIOs,
 * the result is read via SPI from the daisy-chain. In order to read the result of all GPIO measurements,
 * it is necessary to read auxiliary register A and B.
 * Only one register can be read at a time.
 * This function is called to store the result from the transmission in a buffer.
 *
 * @param   registerSet    voltage register that was read (auxiliary register A, B, C or D)
 * @param   *rxBuffer      buffer containing the data obtained from the SPI transmission
 *
 */
static void LTC_SaveRXtoGPIOBuffer(uint8_t registerSet, uint8_t *rxBuffer) {

    uint16_t i = 0;
    uint8_t i_offset = 0;

    if(registerSet == 0) {
    /* RDAUXA command -> GPIO register group A */
        i_offset = 0;
        /* Retrieve data without command and CRC*/
        for(i=0; i < LTC_N_LTC; i++) {
            /* values received in 100uV -> divide by 10 to convert to mV */
            ltc_allgpiovoltage.gpiovoltage[0 + i_offset + BS_NR_OF_GPIOS_PER_MODULE*i]= *((uint16_t *)(&rxBuffer[4+i*8]))/10;
            ltc_allgpiovoltage.gpiovoltage[1 + i_offset + BS_NR_OF_GPIOS_PER_MODULE*i]= *((uint16_t *)(&rxBuffer[6+i*8]))/10;
            ltc_allgpiovoltage.gpiovoltage[2 + i_offset + BS_NR_OF_GPIOS_PER_MODULE*i]= *((uint16_t *)(&rxBuffer[8+i*8]))/10;
        }
    }
    else if(registerSet == 1) {
    /* RDAUXB command -> GPIO register group B */
        i_offset = 3;
        /* Retrieve data without command and CRC*/
        for(i=0; i < LTC_N_LTC; i++) {
            /* values received in 100uV -> divide by 10 to convert to mV */
            ltc_allgpiovoltage.gpiovoltage[0 + i_offset + BS_NR_OF_GPIOS_PER_MODULE*i]= *((uint16_t *)(&rxBuffer[4+i*8]))/10;
            ltc_allgpiovoltage.gpiovoltage[1 + i_offset + BS_NR_OF_GPIOS_PER_MODULE*i]= *((uint16_t *)(&rxBuffer[6+i*8]))/10;
        }
    }
    else if(registerSet == 2) {
    /* RDAUXC command -> GPIO register group C, for 18 cell version */
        i_offset = 5;
        /* Retrieve data without command and CRC*/
        for(i=0; i < LTC_N_LTC; i++) {
            /* values received in 100uV -> divide by 10 to convert to mV */
            ltc_allgpiovoltage.gpiovoltage[0 + i_offset + BS_NR_OF_GPIOS_PER_MODULE*i]= *((uint16_t *)(&rxBuffer[4+i*8]))/10;
            ltc_allgpiovoltage.gpiovoltage[1 + i_offset + BS_NR_OF_GPIOS_PER_MODULE*i]= *((uint16_t *)(&rxBuffer[6+i*8]))/10;
            ltc_allgpiovoltage.gpiovoltage[2 + i_offset + BS_NR_OF_GPIOS_PER_MODULE*i]= *((uint16_t *)(&rxBuffer[8+i*8]))/10;
        }
    }
    else if(registerSet == 3) {
    /* RDAUXD command -> GPIO register group D, for 18 cell version */
        i_offset = 8;
        /* Retrieve data without command and CRC*/
        for(i=0; i < LTC_N_LTC; i++) {
            /* values received in 100uV -> divide by 10 to convert to mV */
            ltc_allgpiovoltage.gpiovoltage[0 + i_offset + BS_NR_OF_GPIOS_PER_MODULE*i]= *((uint16_t *)(&rxBuffer[4+i*8]))/10;
        }
    }
    else
        return;
}


/**
 * @brief   checks if the multiplexers acknowledged transmission.
 *
 * The RDCOMM command can be used to read the answer of the multiplexers to a
 * I2C transmission.
 * This function determines if the communication with the multiplexers was
 * successful or not.
 * The array LTC_ErrorTable is updated to locate the multiplexers that did not
 * acknowledge transmission.
 *
 * @param   *DataBufferSPI_RX    data obtained from the SPI transmission
 * @param   mux                  multiplexer to be addressed (multiplexer ID)
 *
 * @return  mux_error            0 is there was no error, 1 if there was errors
 */
static uint8_t LTC_I2CCheckACK(uint8_t *DataBufferSPI_RX, int mux) {
    uint8_t mux_error = E_OK;
    uint16_t i = 0;

    for (i=0; i < BS_NR_OF_MODULES; i++) {
        if (mux == 0) {
            if ((DataBufferSPI_RX[4+1+LTC_NUMBER_OF_LTC_PER_MODULE*i*8] & 0x0F) != 0x07) {    /* ACK = 0xX7 */
                if (LTC_DISCARD_MUX_CHECK == FALSE) {
                    LTC_ErrorTable[i].mux0 = 1;
                }
                mux_error = E_NOT_OK;
            } else {
                LTC_ErrorTable[i].mux0 = 0;
            }
        }
        if (mux == 1) {
            if ((DataBufferSPI_RX[4+1+LTC_NUMBER_OF_LTC_PER_MODULE*i*8] & 0x0F) != 0x27) {
                if (LTC_DISCARD_MUX_CHECK == FALSE) {
                    LTC_ErrorTable[i].mux1 = 1;
                }
                mux_error = E_NOT_OK;
            } else {
                LTC_ErrorTable[i].mux1 = 0;
            }
        }
        if (mux == 2) {
            if ((DataBufferSPI_RX[4+1+LTC_NUMBER_OF_LTC_PER_MODULE*i*8] & 0x0F) != 0x47) {
                if (LTC_DISCARD_MUX_CHECK == FALSE) {
                    LTC_ErrorTable[i].mux2 = 1;
                }
                mux_error = E_NOT_OK;
            } else {
                LTC_ErrorTable[i].mux2 = 0;
            }
        }
        if (mux == 3) {
            if ((DataBufferSPI_RX[4+1+LTC_NUMBER_OF_LTC_PER_MODULE*i*8] & 0x0F) != 0x67) {
                if (LTC_DISCARD_MUX_CHECK == FALSE) {
                    LTC_ErrorTable[i].mux3 = 1;
                }
                mux_error = E_NOT_OK;
            } else {
                LTC_ErrorTable[i].mux3 = 0;
            }
        }
    }

    if (LTC_DISCARD_MUX_CHECK == TRUE) {
        return 0;
    } else {
        return mux_error;
    }

}



/*
 * @brief   initialize the daisy-chain.
 *
 * To initialize the LTC6804 daisy-chain, a dummy byte (0x00) is sent.
 *
 * @return  retVal  E_OK if dummy byte was sent correctly by SPI, E_NOT_OK otherwise
 *
 */
static STD_RETURN_TYPE_e LTC_Init(void) {

    STD_RETURN_TYPE_e statusSPI = E_NOT_OK;
    STD_RETURN_TYPE_e retVal = E_OK;

    uint8_t PEC_Check[6];
    uint16_t PEC_result = 0;
    uint16_t i = 0;


    /* set REFON bit to 1 */
    /* data for the configuration */
    for (i=0; i < LTC_N_LTC; i++) {

        /* FC = disable all pull-downs, REFON = 1 */
        ltc_TXBuffer[0+(1*i)*6] = 0xFC;
        ltc_TXBuffer[1+(1*i)*6] = 0x00;
        ltc_TXBuffer[2+(1*i)*6] = 0x00;
        ltc_TXBuffer[3+(1*i)*6] = 0x00;
        ltc_TXBuffer[4+(1*i)*6] = 0x00;
        ltc_TXBuffer[5+(1*i)*6] = 0x00;
    }

    /* now construct the message to be sent: it contains the wanted data, PLUS the needed PECs */
    ltc_TXPECbuffer[0] = ltc_cmdWRCFG[0];
    ltc_TXPECbuffer[1] = ltc_cmdWRCFG[1];
    ltc_TXPECbuffer[2] = ltc_cmdWRCFG[2];
    ltc_TXPECbuffer[3] = ltc_cmdWRCFG[3];

    for (i=0; i < LTC_N_LTC; i++) {

        PEC_Check[0] = ltc_TXPECbuffer[4+i*8] = ltc_TXBuffer[0+i*6];
        PEC_Check[1] = ltc_TXPECbuffer[5+i*8] = ltc_TXBuffer[1+i*6];
        PEC_Check[2] = ltc_TXPECbuffer[6+i*8] = ltc_TXBuffer[2+i*6];
        PEC_Check[3] = ltc_TXPECbuffer[7+i*8] = ltc_TXBuffer[3+i*6];
        PEC_Check[4] = ltc_TXPECbuffer[8+i*8] = ltc_TXBuffer[4+i*6];
        PEC_Check[5] = ltc_TXPECbuffer[9+i*8] = ltc_TXBuffer[5+i*6];

        PEC_result = LTC_pec15_calc(6, PEC_Check);
        ltc_TXPECbuffer[10+i*8]=(uint8_t)((PEC_result>>8)&0xff);
        ltc_TXPECbuffer[11+i*8]=(uint8_t)(PEC_result&0xff);
    }  /* end for */

    statusSPI = LTC_SendData(ltc_TXPECbuffer);

    if (statusSPI != E_OK) {
        retVal = E_NOT_OK;
    }

    retVal = statusSPI;

    return retVal;
}




/*
 * @brief   sets the balancing according to the control values read in the database.
 *
 * To set balancing for the cells, the corresponding bits have to be written in the configuration register.
 * The LTC driver only executes the balancing orders written by the BMS in the database.
 *
 * @param registerSet   Register Set, 0: cells 1 to 12 (WRCFG), 1: cells 13 to 15/18 (WRCFG2)
 *
 * @return              E_OK if dummy byte was sent correctly by SPI, E_NOT_OK otherwise
 *
 */
static STD_RETURN_TYPE_e LTC_BalanceControl(uint8_t registerSet) {

    STD_RETURN_TYPE_e retVal = E_OK;

    uint16_t i = 0;
    uint16_t j = 0;

    LTC_Get_BalancingControlValues();

    if (registerSet == 0) {  /* cells 1 to 12, WRCFG */

        for (j=0; j < BS_NR_OF_MODULES; j++) {

            i = BS_NR_OF_MODULES-j-1;

            /* FC = disable all pull-downs, REFON = 1 (reference always on), DTEN off, ADCOPT = 0 */
            ltc_TXBuffer[0+(i)*6] = 0xFC;
            ltc_TXBuffer[1+(i)*6] = 0x00;
            ltc_TXBuffer[2+(i)*6] = 0x00;
            ltc_TXBuffer[3+(i)*6] = 0x00;
            ltc_TXBuffer[4+(i)*6] = 0x00;
            ltc_TXBuffer[5+(i)*6] = 0x00;

            if (ltc_balancing_control.balancing_state[j*(BS_NR_OF_BAT_CELLS_PER_MODULE)+0] == 1) {
                ltc_TXBuffer[4+(i)*6]|=0x01;
            }
            if (ltc_balancing_control.balancing_state[j*(BS_NR_OF_BAT_CELLS_PER_MODULE)+1] == 1) {
                ltc_TXBuffer[4+(i)*6]|=0x02;
            }
            if (ltc_balancing_control.balancing_state[j*(BS_NR_OF_BAT_CELLS_PER_MODULE)+2] == 1) {
                ltc_TXBuffer[4+(i)*6]|=0x04;
            }
            if (ltc_balancing_control.balancing_state[j*(BS_NR_OF_BAT_CELLS_PER_MODULE)+3] == 1) {
                ltc_TXBuffer[4+(i)*6]|=0x08;
            }
            if (ltc_balancing_control.balancing_state[j*(BS_NR_OF_BAT_CELLS_PER_MODULE)+4] == 1) {
                ltc_TXBuffer[4+(i)*6]|=0x10;
            }
            if (ltc_balancing_control.balancing_state[j*(BS_NR_OF_BAT_CELLS_PER_MODULE)+5] == 1) {
                ltc_TXBuffer[4+(i)*6]|=0x20;
            }
            if (ltc_balancing_control.balancing_state[j*(BS_NR_OF_BAT_CELLS_PER_MODULE)+6] == 1) {
                ltc_TXBuffer[4+(i)*6]|=0x40;
            }
            if (ltc_balancing_control.balancing_state[j*(BS_NR_OF_BAT_CELLS_PER_MODULE)+7] == 1) {
                ltc_TXBuffer[4+(i)*6]|=0x80;
            }
            if (ltc_balancing_control.balancing_state[j*(BS_NR_OF_BAT_CELLS_PER_MODULE)+8] == 1) {
                ltc_TXBuffer[5+(i)*6]|=0x01;
            }
            if (ltc_balancing_control.balancing_state[j*(BS_NR_OF_BAT_CELLS_PER_MODULE)+9] == 1) {
                ltc_TXBuffer[5+(i)*6]|=0x02;
            }
            if (ltc_balancing_control.balancing_state[j*(BS_NR_OF_BAT_CELLS_PER_MODULE)+10] == 1) {
                ltc_TXBuffer[5+(i)*6]|=0x04;
            }
            if (ltc_balancing_control.balancing_state[j*(BS_NR_OF_BAT_CELLS_PER_MODULE)+11] == 1) {
                ltc_TXBuffer[5+(i)*6]|=0x08;
            }
        }
        retVal = LTC_TX((uint8_t*)ltc_cmdWRCFG, ltc_TXBuffer, ltc_TXPECbuffer);
    } else if (registerSet == 1) {  /* cells 13 to 15/18 WRCFG2 */

        for (j=0; j < BS_NR_OF_MODULES; j++) {

            i = BS_NR_OF_MODULES-j-1;

            /* 0x0F = disable pull-downs on GPIO6-9 */
            ltc_TXBuffer[0+(i)*6] = 0x0F;
            ltc_TXBuffer[1+(i)*6] = 0x00;
            ltc_TXBuffer[2+(i)*6] = 0x00;
            ltc_TXBuffer[3+(i)*6] = 0x00;
            ltc_TXBuffer[4+(i)*6] = 0x00;
            ltc_TXBuffer[5+(i)*6] = 0x00;

            if (ltc_balancing_control.balancing_state[j*(BS_NR_OF_BAT_CELLS_PER_MODULE)+12] == 1) {
                ltc_TXBuffer[0+(i)*6] |= 0x10;
            }
            if (ltc_balancing_control.balancing_state[j*(BS_NR_OF_BAT_CELLS_PER_MODULE)+13] == 1) {
                ltc_TXBuffer[0+(i)*6] |= 0x20;
            }
            if (ltc_balancing_control.balancing_state[j*(BS_NR_OF_BAT_CELLS_PER_MODULE)+14] == 1) {
                ltc_TXBuffer[0+(i)*6] |= 0x40;
            }
            if (BS_NR_OF_BAT_CELLS_PER_MODULE > 15) {

                if (ltc_balancing_control.balancing_state[j*(BS_NR_OF_BAT_CELLS_PER_MODULE)+15] == 1) {
                    ltc_TXBuffer[0+(i)*6] |= 0x80;
                }
                if (ltc_balancing_control.balancing_state[j*(BS_NR_OF_BAT_CELLS_PER_MODULE)+16] == 1) {
                    ltc_TXBuffer[1+(i)*6] |= 0x01;
                }
                if (ltc_balancing_control.balancing_state[j*(BS_NR_OF_BAT_CELLS_PER_MODULE)+17] == 1) {
                    ltc_TXBuffer[1+(i)*6] |= 0x02;
                }
            }
        }
        retVal = LTC_TX((uint8_t*)ltc_cmdWRCFG2, ltc_TXBuffer, ltc_TXPECbuffer);
    } else {
        return E_NOT_OK;
    }
    return retVal;
}


/*
 * @brief   resets the error table.
 *
 * This function should be called during initialization or before starting a new measurement cycle
 *
 */
static void LTC_ResetErrorTable(void) {
    uint16_t i = 0;

    for (i=0; i < BS_NR_OF_MODULES; i++) {
        LTC_ErrorTable[i].LTC = 0;
        LTC_ErrorTable[i].mux0 = 0;
        LTC_ErrorTable[i].mux1 = 0;
        LTC_ErrorTable[i].mux2 = 0;
        LTC_ErrorTable[i].mux3 = 0;
    }
}


/**
 * @brief   brief missing
 *
 * Gets the measurement time needed by the LTC chip, depending on the measurement mode and the number of channels.
 * For all cell voltages or all 5 GPIOS, the measurement time is the same.
 * For 2 cell voltages or 1 GPIO, the measurement time is the same.
 * As a consequence, this function is used for cell voltage and for GPIO measurement.
 *
 * @param   adcMode     LTC ADCmeasurement mode (fast, normal or filtered)
 * @param   adcMeasCh   number of channels measured for GPIOS (one at a time for multiplexers or all five GPIOs)
 *                      or number of cell voltage measured (2 cells or all cells)
 *
 * @return  retVal      measurement time in ms
 */
static uint16_t LTC_Get_MeasurementTCycle(LTC_ADCMODE_e adcMode, LTC_ADCMEAS_CHAN_e  adcMeasCh) {

    uint16_t retVal = LTC_STATEMACH_MEAS_ALL_NORMAL_TCYCLE;  /* default */

    if (adcMeasCh == LTC_ADCMEAS_ALLCHANNEL) {

        if (adcMode == LTC_ADCMODE_FAST_DCP0 || adcMode == LTC_ADCMODE_FAST_DCP1) {

            retVal = LTC_STATEMACH_MEAS_ALL_FAST_TCYCLE;
        } else if (adcMode == LTC_ADCMODE_NORMAL_DCP0 || adcMode == LTC_ADCMODE_NORMAL_DCP1) {

            retVal = LTC_STATEMACH_MEAS_ALL_NORMAL_TCYCLE;
        } else if (adcMode == LTC_ADCMODE_FILTERED_DCP0 || adcMode == LTC_ADCMODE_FILTERED_DCP1) {

            retVal = LTC_STATEMACH_MEAS_ALL_FILTERED_TCYCLE;
        }
    } else if (adcMeasCh == LTC_ADCMEAS_SINGLECHANNEL_GPIO1 || adcMeasCh == LTC_ADCMEAS_SINGLECHANNEL_GPIO2
            || adcMeasCh == LTC_ADCMEAS_SINGLECHANNEL_GPIO3 || adcMeasCh == LTC_ADCMEAS_SINGLECHANNEL_GPIO4
            || adcMeasCh == LTC_ADCMEAS_SINGLECHANNEL_GPIO5 || adcMeasCh == LTC_ADCMEAS_SINGLECHANNEL_TWOCELLS) {

        if (adcMode == LTC_ADCMODE_FAST_DCP0 || adcMode == LTC_ADCMODE_FAST_DCP1) {

            retVal = LTC_STATEMACH_MEAS_SINGLE_FAST_TCYCLE;
        } else if (adcMode == LTC_ADCMODE_NORMAL_DCP0 || adcMode == LTC_ADCMODE_NORMAL_DCP1) {

            retVal = LTC_STATEMACH_MEAS_SINGLE_NORMAL_TCYCLE;
        } else if (adcMode == LTC_ADCMODE_FILTERED_DCP0 || adcMode == LTC_ADCMODE_FILTERED_DCP1) {

            retVal = LTC_STATEMACH_MEAS_SINGLE_FILTERED_TCYCLE;
        }
    } else {

        retVal = LTC_STATEMACH_MEAS_ALL_NORMAL_TCYCLE;
    }

    return retVal;
}


/**
 * @brief   tells the LTC daisy-chain to start measuring the voltage on all cells.
 *
 * This function sends an instruction to the daisy-chain via SPI, in order to start voltage measurement for all cells.
 *
 * @param   adcMode     LTC ADCmeasurement mode (fast, normal or filtered)
 * @param   adcMeasCh   number of cell voltage measured (2 cells or all cells)
 *
 * @return  retVal      E_OK if dummy byte was sent correctly by SPI, E_NOT_OK otherwise
 *
 */
static STD_RETURN_TYPE_e LTC_StartVoltageMeasurement(LTC_ADCMODE_e adcMode, LTC_ADCMEAS_CHAN_e adcMeasCh) {

    STD_RETURN_TYPE_e retVal = E_OK;

    if (adcMeasCh == LTC_ADCMEAS_ALLCHANNEL) {
        if (adcMode == LTC_ADCMODE_FAST_DCP0) {

            retVal = LTC_SendCmd(ltc_cmdADCV_fast_DCP0);
        } else if (adcMode == LTC_ADCMODE_NORMAL_DCP0) {

            retVal = LTC_SendCmd(ltc_cmdADCV_normal_DCP0);
        } else if (adcMode == LTC_ADCMODE_FILTERED_DCP0) {

            retVal = LTC_SendCmd(ltc_cmdADCV_filtered_DCP0);
        } else if (adcMode == LTC_ADCMODE_FAST_DCP1) {

            retVal = LTC_SendCmd(ltc_cmdADCV_fast_DCP1);
        } else if (adcMode == LTC_ADCMODE_NORMAL_DCP1) {

            retVal = LTC_SendCmd(ltc_cmdADCV_normal_DCP1);
        } else if (adcMode == LTC_ADCMODE_FILTERED_DCP1) {

            retVal = LTC_SendCmd(ltc_cmdADCV_filtered_DCP1);
        } else {
            retVal = E_NOT_OK;
        }
    } else if (adcMeasCh == LTC_ADCMEAS_SINGLECHANNEL_TWOCELLS) {
        if (adcMode == LTC_ADCMODE_FAST_DCP0) {

            retVal = LTC_SendCmd(ltc_cmdADCV_fast_DCP0_twocells);
        } else {
            retVal = E_NOT_OK;
        }
    } else {
        retVal = E_NOT_OK;
    }
    return retVal;
}


/**
 * @brief   tells LTC daisy-chain to start measuring the voltage on GPIOS.
 *
 * This function sends an instruction to the daisy-chain via SPI to start the measurement.
 *
 * @param   adcMode     LTC ADCmeasurement mode (fast, normal or filtered)
 * @param   adcMeasCh   number of channels measured for GPIOS (one at a time, typically when multiplexers are used, or all five GPIOs)
 *
 * @return  retVal      E_OK if dummy byte was sent correctly by SPI, E_NOT_OK otherwise
 *
 */
static STD_RETURN_TYPE_e LTC_StartGPIOMeasurement(LTC_ADCMODE_e adcMode, LTC_ADCMEAS_CHAN_e  adcMeasCh) {

    STD_RETURN_TYPE_e retVal;

    if (adcMeasCh == LTC_ADCMEAS_ALLCHANNEL) {

        if (adcMode == LTC_ADCMODE_FAST_DCP0 || adcMode == LTC_ADCMODE_FAST_DCP1) {

            retVal = LTC_SendCmd(ltc_cmdADAX_fast_ALLGPIOS);
        } else if (adcMode == LTC_ADCMODE_FILTERED_DCP0 || adcMode == LTC_ADCMODE_FILTERED_DCP1) {

            retVal = LTC_SendCmd(ltc_cmdADAX_filtered_ALLGPIOS);
        } else {
            /*if(adcMode == LTC_ADCMODE_NORMAL_DCP0 || adcMode == LTC_ADCMODE_NORMAL_DCP1)*/
            retVal = LTC_SendCmd(ltc_cmdADAX_normal_ALLGPIOS);
        }
    } else if (adcMeasCh == LTC_ADCMEAS_SINGLECHANNEL_GPIO1) {
        /* Single Channel */
        if (adcMode == LTC_ADCMODE_FAST_DCP0 || adcMode == LTC_ADCMODE_FAST_DCP1) {

            retVal = LTC_SendCmd(ltc_cmdADAX_fast_GPIO1);
        } else if (adcMode == LTC_ADCMODE_FILTERED_DCP0 || adcMode == LTC_ADCMODE_FILTERED_DCP1) {

            retVal = LTC_SendCmd(ltc_cmdADAX_filtered_GPIO1);
        } else {
            /*if(adcMode == LTC_ADCMODE_NORMAL_DCP0 || adcMode == LTC_ADCMODE_NORMAL_DCP1)*/

            retVal = LTC_SendCmd(ltc_cmdADAX_normal_GPIO1);
        }
    } else if (adcMeasCh == LTC_ADCMEAS_SINGLECHANNEL_GPIO2) {
        /* Single Channel */
        if (adcMode == LTC_ADCMODE_FAST_DCP0 || adcMode == LTC_ADCMODE_FAST_DCP1) {

            retVal = LTC_SendCmd(ltc_cmdADAX_fast_GPIO2);
        } else if (adcMode == LTC_ADCMODE_FILTERED_DCP0 || adcMode == LTC_ADCMODE_FILTERED_DCP1) {

            retVal = LTC_SendCmd(ltc_cmdADAX_filtered_GPIO2);
        } else {
            /*if(adcMode == LTC_ADCMODE_NORMAL_DCP0 || adcMode == LTC_ADCMODE_NORMAL_DCP1)*/

            retVal = LTC_SendCmd(ltc_cmdADAX_normal_GPIO2);
        }
    } else if (adcMeasCh == LTC_ADCMEAS_SINGLECHANNEL_GPIO3) {
        /* Single Channel */
        if (adcMode == LTC_ADCMODE_FAST_DCP0 || adcMode == LTC_ADCMODE_FAST_DCP1) {

            retVal = LTC_SendCmd(ltc_cmdADAX_fast_GPIO3);
        } else if (adcMode == LTC_ADCMODE_FILTERED_DCP0 || adcMode == LTC_ADCMODE_FILTERED_DCP1) {

            retVal = LTC_SendCmd(ltc_cmdADAX_filtered_GPIO3);
        } else {
            /*if(adcMode == LTC_ADCMODE_NORMAL_DCP0 || adcMode == LTC_ADCMODE_NORMAL_DCP1)*/

            retVal = LTC_SendCmd(ltc_cmdADAX_normal_GPIO3);
        }
    } else {
        retVal = E_NOT_OK;
    }

    return retVal;
}


/**
 * @brief   checks if the data received from the daisy-chain is not corrupt.
 *
 * This function computes the PEC (CRC) from the data received by the daisy-chain.
 * It compares it with the PEC sent by the LTCs.
 * If there are errors, the array LTC_ErrorTable is updated to locate the LTCs in daisy-chain
 * that transmitted corrupt data.
 *
 * @param   *DataBufferSPI_RX_with_PEC   data obtained from the SPI transmission
 *
 * @return  retVal                       E_OK if PEC check is OK, E_NOT_OK otherwise
 *
 */
static STD_RETURN_TYPE_e LTC_RX_PECCheck(uint8_t *DataBufferSPI_RX_with_PEC) {

    uint16_t i = 0;
    STD_RETURN_TYPE_e retVal = E_OK;
    uint8_t PEC_TX[2];
    uint16_t PEC_result = 0;
    uint8_t PEC_Check[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    /* check all PECs and put data without command and PEC in DataBufferSPI_RX (easier to use) */
    for (i=0; i < LTC_N_LTC; i++) {

        PEC_Check[0] = DataBufferSPI_RX_with_PEC[4+i*8];
        PEC_Check[1] = DataBufferSPI_RX_with_PEC[5+i*8];
        PEC_Check[2] = DataBufferSPI_RX_with_PEC[6+i*8];
        PEC_Check[3] = DataBufferSPI_RX_with_PEC[7+i*8];
        PEC_Check[4] = DataBufferSPI_RX_with_PEC[8+i*8];
        PEC_Check[5] = DataBufferSPI_RX_with_PEC[9+i*8];

        PEC_result = LTC_pec15_calc(6, PEC_Check);
        PEC_TX[0]=(uint8_t)((PEC_result>>8)&0xff);
        PEC_TX[1]=(uint8_t)(PEC_result&0xff);

        /* if calculated PEC not equal to received PEC */
        if ((PEC_TX[0] != DataBufferSPI_RX_with_PEC[10+i*8]) || (PEC_TX[1] != DataBufferSPI_RX_with_PEC[11+i*8])) {

            /* update error table of the corresponding LTC */
            if (LTC_DISCARD_PEC == FALSE) {
                LTC_ErrorTable[i].LTC = 1;
            }
            retVal = E_NOT_OK;

        } else {
            /* update error table of the corresponding LTC */
            LTC_ErrorTable[i].LTC = 0;
            retVal = E_OK;
        }
    }

    if (LTC_DISCARD_PEC == TRUE) {
        return E_OK;
    } else {
        return (retVal);
    }
}


/**
 * @brief   send command to the LTC daisy-chain and receives data from the LTC daisy-chain.
 *
 * This is the core function to receive data from the LTC6804 daisy-chain.
 * A 2 byte command is sent with the corresponding PEC. Example: read configuration register (RDCFG).
 * Only command has to be set, the function calculates the PEC automatically.
 * The data send is:
 * 2 bytes (COMMAND) 2 bytes (PEC)
 * The data received is:
 * 6 bytes (LTC1) 2 bytes (PEC) + 6 bytes (LTC2) 2 bytes (PEC) + 6 bytes (LTC3) 2 bytes (PEC) + ... + 6 bytes (LTC{LTC_N_LTC}) 2 bytes (PEC)
 *
 * The function does not check the PECs. This has to be done elsewhere.
 *
 * @param   *Command                    command sent to the daisy-chain
 * @param   *DataBufferSPI_RX_with_PEC  data to sent to the daisy-chain, i.e. data to be sent + PEC
 *
 * @return  statusSPI                   E_OK if SPI transmission is OK, E_NOT_OK otherwise
 *
 */
static STD_RETURN_TYPE_e LTC_RX(uint8_t *Command, uint8_t *DataBufferSPI_RX_with_PEC) {

    STD_RETURN_TYPE_e statusSPI = E_OK;
    uint16_t i = 0;

    /* DataBufferSPI_RX_with_PEC contains the data to receive.
       The transmission function checks the PECs.
       It constructs DataBufferSPI_RX, which contains the received data without PEC (easier to use). */

    for (i=0; i < LTC_N_BYTES_FOR_DATA_TRANSMISSION; i++) {
        ltc_TXPECbuffer[i] = 0x00;
    }

    ltc_TXPECbuffer[0] = Command[0];
    ltc_TXPECbuffer[1] = Command[1];
    ltc_TXPECbuffer[2] = Command[2];
    ltc_TXPECbuffer[3] = Command[3];

    statusSPI = LTC_ReceiveData(ltc_TXPECbuffer, DataBufferSPI_RX_with_PEC);

    if (statusSPI != E_OK) {

        return E_NOT_OK;
    } else {
        return E_OK;
    }
}



/**
 * @brief   sends command and data to the LTC daisy-chain.
 *
 * This is the core function to transmit data to the LTC6804 daisy-chain.
 * The data sent is:
 * COMMAND + 6 bytes (LTC1) + 6 bytes (LTC2) + 6 bytes (LTC3) + ... + 6 bytes (LTC{LTC_N_LTC})
 * A 2 byte command is sent with the corresponding PEC. Example: write configuration register (WRCFG).
 * THe command has to be set and then the function calculates the PEC automatically.
 * The function calculates the needed PEC to send the data to the daisy-chain. The sent data has the format:
 * 2 byte-COMMAND (2 bytes PEC) + 6 bytes (LTC1) (2 bytes PEC) + 6 bytes (LTC2) (2 bytes PEC) + 6 bytes (LTC3) (2 bytes PEC) + ... + 6 bytes (LTC{LTC_N_LTC}) (2 bytes PEC)
 *
 * The function returns 0. The only way to check if the transmission was successful is to read the results of the write operation.
 * (example: read configuration register after writing to it)
 *
 * @param   *Command                    command sent to the daisy-chain
 * @param   *DataBufferSPI_TX           data to be sent to the daisy-chain
 * @param   *DataBufferSPI_TX_with_PEC  data to sent to the daisy-chain, i.e. data to be sent + PEC (calculated by the function)
 *
 * @return                              E_OK if SPI transmission is OK, E_NOT_OK otherwise
 *
 */
static STD_RETURN_TYPE_e LTC_TX(uint8_t *Command, uint8_t *DataBufferSPI_TX, uint8_t *DataBufferSPI_TX_with_PEC) {

    uint16_t i = 0;
    STD_RETURN_TYPE_e statusSPI = E_NOT_OK;
    uint16_t PEC_result = 0;
    uint8_t PEC_Check[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    /*  DataBufferSPI_TX contains the data to send.
        The transmission function calculates the needed PEC.
        With it constructs DataBufferSPI_TX_with_PEC.
        It corresponds to the data effectively received/sent. */
    for (i=0; i < LTC_N_BYTES_FOR_DATA_TRANSMISSION; i++) {
        DataBufferSPI_TX_with_PEC[i] = 0x00;
    }

    DataBufferSPI_TX_with_PEC[0] = Command[0];
    DataBufferSPI_TX_with_PEC[1] = Command[1];
    DataBufferSPI_TX_with_PEC[2] = Command[2];
    DataBufferSPI_TX_with_PEC[3] = Command[3];

    /* Calculate PEC of all data (1 PEC value for 6 bytes) */
    for (i=0; i < LTC_N_LTC; i++) {

        PEC_Check[0] = DataBufferSPI_TX_with_PEC[4+i*8] = DataBufferSPI_TX[0+i*6];
        PEC_Check[1] = DataBufferSPI_TX_with_PEC[5+i*8] = DataBufferSPI_TX[1+i*6];
        PEC_Check[2] = DataBufferSPI_TX_with_PEC[6+i*8] = DataBufferSPI_TX[2+i*6];
        PEC_Check[3] = DataBufferSPI_TX_with_PEC[7+i*8] = DataBufferSPI_TX[3+i*6];
        PEC_Check[4] = DataBufferSPI_TX_with_PEC[8+i*8] = DataBufferSPI_TX[4+i*6];
        PEC_Check[5] = DataBufferSPI_TX_with_PEC[9+i*8] = DataBufferSPI_TX[5+i*6];

        PEC_result = LTC_pec15_calc(6, PEC_Check);
        DataBufferSPI_TX_with_PEC[10+i*8]=(uint8_t)((PEC_result>>8)&0xff);
        DataBufferSPI_TX_with_PEC[11+i*8]=(uint8_t)(PEC_result&0xff);
    }

    statusSPI = LTC_SendData(DataBufferSPI_TX_with_PEC);

    if (statusSPI != E_OK) {
        return E_NOT_OK;
    } else {
        return E_OK;
    }
}

/**
 * @brief   configures the data that will be sent to the LTC daisy-chain to configure multiplexer channels.
 *
 * This function does not sent the data to the multiplexer daisy-chain. This is done
 * by the function LTC_SetMuxChannel(), which calls LTC_SetMUXChCommand()..
 *
 * @param   *DataBufferSPI_TX      data to be sent to the daisy-chain to configure the multiplexer channels
 * @param   mux                    multiplexer ID to be configured (0,1,2 or 3)
 * @param   channel                multiplexer channel to be configured (0 to 7)
 *
 */
static void LTC_SetMUXChCommand(uint8_t *DataBufferSPI_TX, uint8_t mux, uint8_t channel) {

    uint16_t i = 0;

    for (i=0; i < LTC_N_LTC; i++) {

#if SLAVE_BOARD_VERSION == 2

        /* using ADG728 */
        uint8_t address = 0x4C | (mux % 3);
        uint8_t data = 1 << (channel % 8);
        if (channel == 0xFF) {  /* no channel selected, output of multiplexer is high impedance */
            data = 0x00;
        }

#else

        /* using LTC1380 */
        uint8_t address = 0x48 | (mux % 4);
        uint8_t data = 0x08 | (channel % 8);
        if (channel == 0xFF) {  /* no channel selected, output of multiplexer is high impedance */
            data = 0x00;
        }

#endif

        DataBufferSPI_TX[0 + i * 6] = LTC_ICOM_START | (address >> 3);        /* 0x6 : LTC6804: ICOM START from Master */
        DataBufferSPI_TX[1 + i * 6] = LTC_FCOM_MASTER_NACK | (address << 5);
        DataBufferSPI_TX[2 + i * 6] = LTC_ICOM_BLANK | (data >> 4);
        DataBufferSPI_TX[3 + i * 6] = LTC_FCOM_MASTER_NACK_STOP | (data << 4);
        DataBufferSPI_TX[4 + i * 6] = LTC_ICOM_NO_TRANSMIT;        /* 0x1 : ICOM-STOP */
        DataBufferSPI_TX[5 + i * 6] = 0x00;        /* 0x0 : dummy (Dn) */
                                                   /* 9: MASTER NACK + STOP (FCOM) */
    }
}



/**
 * @brief   sends data to the LTC daisy-chain to read EEPROM on slaves.
 *
 *
 * @param        *DataBufferSPI_TX              data to be sent to the daisy-chain to configure the multiplexer channels
 * @param        *DataBufferSPI_TX_with_PEC     data to be sent to the daisy-chain to configure the multiplexer channels, with PEC (calculated by the function)
 * @param        step                           first or second stage of read process (0 or 1)
 * @param        address                        read address (18 bits)
 *
 * @return       E_OK if SPI transmission is OK, E_NOT_OK otherwise
 */
static uint8_t LTC_SendEEPROMReadCommand(uint8_t *DataBufferSPI_TX, uint8_t *DataBufferSPI_TX_with_PEC, uint8_t step) {

    STD_RETURN_TYPE_e statusSPI = E_NOT_OK;

    /* send WRCOMM to send I2C message to choose channel */
    LTC_SetEEPROMReadCommand(step, DataBufferSPI_TX);
    statusSPI = LTC_TX((uint8_t*)ltc_cmdWRCOMM, DataBufferSPI_TX, DataBufferSPI_TX_with_PEC);

    if (statusSPI != E_OK) {
        return E_NOT_OK;
    } else {
        return E_OK;
    }
}



/**
 * @brief   configures the data that will be sent to the LTC daisy-chain to read EEPROM on slaves.
 *
 * @param   step                   first or second stage of read process (0 or 1)
 * @param   *DataBufferSPI_TX      data to be sent to the daisy-chain
 * @param   address                read address (18 bits)
 *
 */
static void LTC_SetEEPROMReadCommand(uint8_t step, uint8_t *DataBufferSPI_TX) {

    uint16_t i = 0;
    uint32_t address = 0;
    uint8_t address0 = 0;
    uint8_t address1 = 0;
    uint8_t address2 = 0;

    DB_ReadBlock(&ltc_slave_control, DATA_BLOCK_ID_SLAVE_CONTROL);

    address = ltc_slave_control.eeprom_read_address_to_use;

    address &= 0x3FFFF;
    address0 = address>>16;
    address1 = (address&0xFFFF)>>8;
    address2 = address&0xFF;

    if (step == 0) {

        for (i=0; i < LTC_N_LTC; i++) {

            DataBufferSPI_TX[0 + i * 6] = LTC_ICOM_START | (0x0A);        /* 0x6 : LTC6804: ICOM START from Master */
            DataBufferSPI_TX[1 + i * 6] = LTC_FCOM_MASTER_NACK | (((address0 & 0x03) << 5) | 0x00);
            DataBufferSPI_TX[2 + i * 6] = LTC_ICOM_BLANK | (address1 >> 4);
            DataBufferSPI_TX[3 + i * 6] = LTC_FCOM_MASTER_NACK | (address1 << 4);
            DataBufferSPI_TX[4 + i * 6] = LTC_ICOM_BLANK | (address2 >> 4);
            DataBufferSPI_TX[5 + i * 6] = LTC_FCOM_MASTER_NACK | (address2 << 4);

        }

    } else {  /* step == 1 */

        for (i=0; i < LTC_N_LTC; i++) {

            DataBufferSPI_TX[0 + i * 6] = LTC_ICOM_START | (0x0A);        /* 0x6 : LTC6804: ICOM START from Master */
            DataBufferSPI_TX[1 + i * 6] = LTC_FCOM_MASTER_NACK | (((address0 & 0x03) << 5) | 0x10);
            DataBufferSPI_TX[2 + i * 6] = LTC_ICOM_BLANK | 0x0F;
            DataBufferSPI_TX[3 + i * 6] = LTC_FCOM_MASTER_NACK_STOP | 0xF0;
            DataBufferSPI_TX[4 + i * 6] = LTC_ICOM_NO_TRANSMIT | 0x00;
            DataBufferSPI_TX[5 + i * 6] = LTC_FCOM_MASTER_NACK_STOP | 0x00;

        }
    }
}


/**
 * @brief   saves the read values of the external EEPROMs read from the LTC daisy-chain.
 *
 *
 * @param   *rxBuffer      buffer containing the data obtained from the SPI transmission
 *
 */
static void LTC_EEPROMSaveReadValue(uint8_t *rxBuffer) {

    uint16_t i = 0;

    DB_ReadBlock(&ltc_slave_control, DATA_BLOCK_ID_SLAVE_CONTROL);

    for (i=0; i < LTC_N_LTC; i++) {
        ltc_slave_control.eeprom_value_read[i] = (rxBuffer[6+i*8] << 4)|((rxBuffer[7+i*8] >> 4));
    }

    ltc_slave_control.eeprom_read_address_last_used = ltc_slave_control.eeprom_read_address_to_use;
    ltc_slave_control.eeprom_read_address_to_use = 0xFFFFFFFF;

    DB_WriteBlock(&ltc_slave_control, DATA_BLOCK_ID_SLAVE_CONTROL);
}



/**
 * @brief   sends data to the LTC daisy-chain to write EEPROM on slaves.
 *
 *
 * @param        *DataBufferSPI_TX              data to be sent to the daisy-chain to configure the multiplexer channels
 * @param        *DataBufferSPI_TX_with_PEC     data to be sent to the daisy-chain to configure the multiplexer channels, with PEC (calculated by the function)
 * @param        step                           first or second stage of read process (0 or 1)
 * @param        address                        read address (18 bits)
 *
 * @return       E_OK if SPI transmission is OK, E_NOT_OK otherwise
 */
static uint8_t LTC_SendEEPROMWriteCommand(uint8_t *DataBufferSPI_TX, uint8_t *DataBufferSPI_TX_with_PEC, uint8_t step) {

    STD_RETURN_TYPE_e statusSPI = E_NOT_OK;

    /* send WRCOMM to send I2C message to write EEPROM */
    LTC_SetEEPROMWriteCommand(step, DataBufferSPI_TX);
    statusSPI = LTC_TX((uint8_t*)ltc_cmdWRCOMM, DataBufferSPI_TX, DataBufferSPI_TX_with_PEC);

    if (statusSPI != E_OK) {
        return E_NOT_OK;
    } else {
        return E_OK;
    }
}



/**
 * @brief   configures the data that will be sent to the LTC daisy-chain to write EEPROM on slaves.
 *
 * @param   step                   first or second stage of read process (0 or 1)
 * @param   *DataBufferSPI_TX      data to be sent to the daisy-chain
 * @param   address                read address (18 bits)
 *
 */
static void LTC_SetEEPROMWriteCommand(uint8_t step, uint8_t *DataBufferSPI_TX) {

    uint16_t i = 0;
    uint32_t address = 0;
    uint8_t data = 0;
    uint8_t address0 = 0;
    uint8_t address1 = 0;
    uint8_t address2 = 0;

    DB_ReadBlock(&ltc_slave_control, DATA_BLOCK_ID_SLAVE_CONTROL);

    address = ltc_slave_control.eeprom_write_address_to_use;

    address &= 0x3FFFF;
    address0 = address>>16;
    address1 = (address&0xFFFF)>>8;
    address2 = address&0xFF;

    if (step == 0) {

        for (i=0; i < LTC_N_LTC; i++) {

            DataBufferSPI_TX[0 + i * 6] = LTC_ICOM_START | (0x0A);        /* 0x6 : LTC6804: ICOM START from Master */
            DataBufferSPI_TX[1 + i * 6] = LTC_FCOM_MASTER_NACK | (((address0 & 0x03) << 5) | 0x00);
            DataBufferSPI_TX[2 + i * 6] = LTC_ICOM_BLANK | (address1 >> 4);
            DataBufferSPI_TX[3 + i * 6] = LTC_FCOM_MASTER_NACK | (address1 << 4);
            DataBufferSPI_TX[4 + i * 6] = LTC_ICOM_BLANK | (address2 >> 4);
            DataBufferSPI_TX[5 + i * 6] = LTC_FCOM_MASTER_NACK | (address2 << 4);

        }

    } else {  /* step == 1 */

        for (i=0; i < LTC_N_LTC; i++) {

            data = ltc_slave_control.eeprom_value_write[i];

            DataBufferSPI_TX[0 + i * 6] = LTC_ICOM_BLANK | (data >> 4);        /* 0x6 : LTC6804: ICOM START from Master */
            DataBufferSPI_TX[1 + i * 6] = LTC_FCOM_MASTER_NACK_STOP | (data << 4);
            DataBufferSPI_TX[2 + i * 6] = LTC_ICOM_NO_TRANSMIT | 0x00;
            DataBufferSPI_TX[3 + i * 6] = LTC_FCOM_MASTER_NACK_STOP | 0x00;
            DataBufferSPI_TX[4 + i * 6] = LTC_ICOM_NO_TRANSMIT | 0x00;
            DataBufferSPI_TX[5 + i * 6] = LTC_FCOM_MASTER_NACK_STOP | 0x00;

        }

        ltc_slave_control.eeprom_write_address_last_used = ltc_slave_control.eeprom_write_address_to_use;
        ltc_slave_control.eeprom_write_address_to_use = 0xFFFFFFFF;

        DB_WriteBlock(&ltc_slave_control, DATA_BLOCK_ID_SLAVE_CONTROL);

    }

}


/**
 * @brief   sends data to the LTC daisy-chain to configure multiplexer channels.
 *
 * This function calls the function LTC_SetMUXChCommand() to set the data.
 *
 * @param        *DataBufferSPI_TX              data to be sent to the daisy-chain to configure the multiplexer channels
 * @param        *DataBufferSPI_TX_with_PEC     data to be sent to the daisy-chain to configure the multiplexer channels, with PEC (calculated by the function)
 * @param        mux                    multiplexer ID to be configured (0,1,2 or 3)
 * @param        channel                multiplexer channel to be configured (0 to 7)
 *
 * @return       E_OK if SPI transmission is OK, E_NOT_OK otherwise
 */
static uint8_t LTC_SetMuxChannel(uint8_t *DataBufferSPI_TX, uint8_t *DataBufferSPI_TX_with_PEC, uint8_t mux, uint8_t channel ) {

    STD_RETURN_TYPE_e statusSPI = E_NOT_OK;

    /* send WRCOMM to send I2C message to choose channel */
    LTC_SetMUXChCommand(DataBufferSPI_TX, mux, channel);
    statusSPI = LTC_TX((uint8_t*)ltc_cmdWRCOMM, DataBufferSPI_TX, DataBufferSPI_TX_with_PEC);

    if (statusSPI != E_OK) {
        return E_NOT_OK;
    } else {
        return E_OK;
    }
}



/**
 * @brief   sends data to the LTC daisy-chain to communicate via I2C
 *
 * This function initiates an I2C signal sent by the LTC6804 on the slave boards
 *
 * @param        *DataBufferSPI_TX              data to be sent to the daisy-chain to configure the EEPROM
 * @param        *DataBufferSPI_TX_with_PEC     data to be sent to the daisy-chain to configure the EEPROM, with PEC (calculated by the function)
 * @param        cmd_daa                        command data to be sent
 *
 * @return       E_OK if SPI transmission is OK, E_NOT_OK otherwise
 */
static STD_RETURN_TYPE_e LTC_Send_I2C_Command(uint8_t *DataBufferSPI_TX, uint8_t *DataBufferSPI_TX_with_PEC, uint8_t *cmd_data) {

    STD_RETURN_TYPE_e statusSPI = E_NOT_OK;

    uint16_t i = 0;

    for (i=0; i < BS_NR_OF_MODULES; i++) {

        DataBufferSPI_TX[0+i*6] = cmd_data[0];
        DataBufferSPI_TX[1+i*6] = cmd_data[1];

        DataBufferSPI_TX[2+i*6] = cmd_data[2];
        DataBufferSPI_TX[3+i*6] = cmd_data[3];

        DataBufferSPI_TX[4+i*6] = cmd_data[4];
        DataBufferSPI_TX[5+i*6] = cmd_data[5];
    }

    /* send WRCOMM to send I2C message to choose channel */
    statusSPI = LTC_TX((uint8_t*)ltc_cmdWRCOMM, DataBufferSPI_TX, DataBufferSPI_TX_with_PEC);

    if (statusSPI != E_OK) {
        return E_NOT_OK;
    } else {
        return E_OK;
    }
}


/**
 * @brief   saves the temperature value of the external temperature sensors read from the LTC daisy-chain.
 *
 * This function saves the temperature value received from the external temperature sensors
 *
 * @param   *rxBuffer      buffer containing the data obtained from the SPI transmission
 *
 */
static void LTC_TempSensSaveTemp(uint8_t *rxBuffer) {

    uint16_t i = 0;
    uint8_t temp_tmp[2];
    uint16_t val_i = 0;

    DB_ReadBlock(&ltc_slave_control, DATA_BLOCK_ID_SLAVE_CONTROL);

    for (i=0; i < LTC_N_LTC; i++) {
        temp_tmp[0] = (rxBuffer[6+i*8] << 4) | ((rxBuffer[7+i*8] >> 4));
        temp_tmp[1] = (rxBuffer[8+i*8] << 4) | ((rxBuffer[9+i*8] >> 4));
        val_i = (temp_tmp[0] << 8) | (temp_tmp[1]);
        val_i = val_i>>8;
        ltc_slave_control.external_sensor_temperature[i] = val_i;
    }

    DB_WriteBlock(&ltc_slave_control, DATA_BLOCK_ID_SLAVE_CONTROL);
}

/**
 * @brief   sends data to the LTC daisy-chain to control the user port expander
 *
 * This function sends a control byte to the register of the user port expander
 *
 * @param        *DataBufferSPI_TX              data to be sent to the daisy-chain to configure the multiplexer channels
 * @param        *DataBufferSPI_TX_with_PEC     data to be sent to the daisy-chain to configure the multiplexer channels, with PEC (calculated by the function)
 *
 * @return       E_OK if SPI transmission is OK, E_NOT_OK otherwise
 */
static uint8_t LTC_SetPortExpander(uint8_t *DataBufferSPI_TX, uint8_t *DataBufferSPI_TX_with_PEC ) {

    STD_RETURN_TYPE_e statusSPI = E_NOT_OK;

    uint16_t i = 0;
    uint8_t output_data = 0;

    DB_ReadBlock(&ltc_slave_control, DATA_BLOCK_ID_SLAVE_CONTROL);

    for (i=0; i < BS_NR_OF_MODULES; i++) {
        output_data = ltc_slave_control.io_value_out[BS_NR_OF_MODULES-1-i];

        DataBufferSPI_TX[0+i*6] = LTC_ICOM_START | 0x04;     /* 6: ICOM0 start condition, 4: upper nibble of PCA8574 address */
        DataBufferSPI_TX[1+i*6] = 0 | LTC_FCOM_MASTER_NACK;  /* 0: lower nibble of PCA8574 address + R/W bit, 8: FCOM0 master NACK */

        DataBufferSPI_TX[2+i*6] = LTC_ICOM_BLANK | (output_data>>4);  /* 0: ICOM1 blank, x: upper nibble of PCA8574 data register (0 == pin low) */
        DataBufferSPI_TX[3+i*6] = (uint8_t)(output_data << 4) | LTC_FCOM_MASTER_NACK_STOP;  /* x: lower nibble of PCA8574 data register, 9: FCOM1 master NACK + STOP */

        DataBufferSPI_TX[4+i*6] = LTC_ICOM_NO_TRANSMIT;  /* 7: no transmission, F: dummy data */
        DataBufferSPI_TX[5+i*6] = 0;  /* F: dummy data, 9: FCOM2 master NACK + STOP */
    }

    /* send WRCOMM to send I2C message to choose channel */
    statusSPI = LTC_TX((uint8_t*)ltc_cmdWRCOMM, DataBufferSPI_TX, DataBufferSPI_TX_with_PEC);

    if (statusSPI != E_OK) {
        return E_NOT_OK;
    } else {
        return E_OK;
    }
}

/**
 * @brief   saves the received values of the external port expander read from the LTC daisy-chain.
 *
 * This function saves the received data byte from the external port expander
 *
 * @param   *rxBuffer      buffer containing the data obtained from the SPI transmission
 *
 */
static void LTC_PortExpanderSaveValues(uint8_t *rxBuffer) {
    uint16_t i = 0;
    uint8_t val_i;

    DB_ReadBlock(&ltc_slave_control, DATA_BLOCK_ID_SLAVE_CONTROL);

    /* extract data */
    for (i=0; i < LTC_N_LTC; i++) {
        val_i = (rxBuffer[6+i*8] << 4) | ((rxBuffer[7+i*8] >> 4));
        ltc_slave_control.io_value_in[i] = val_i;
    }

    DB_WriteBlock(&ltc_slave_control, DATA_BLOCK_ID_SLAVE_CONTROL);
}

/**
 * @brief   sends 72 clock pulses to the LTC daisy-chain.
 *
 * This function is used for the communication with the multiplexers via I2C on the GPIOs.
 * It send the command STCOMM to the LTC daisy-chain.
 *
 * @param   *DataBufferSPI_TX           data to be sent to the daisy-chain, set to 0xFF
 * @param   *DataBufferSPI_TX_with_PEC  data to be sent to the daisy-chain  with PEC (calculated by the function)
 *
 * @return  statusSPI                   E_OK if clock pulses were sent correctly by SPI, E_NOT_OK otherwise
 *
 */
static STD_RETURN_TYPE_e LTC_I2CClock(uint8_t *DataBufferSPI_TX, uint8_t *DataBufferSPI_TX_with_PEC) {

    uint16_t i = 0;
    STD_RETURN_TYPE_e statusSPI = E_NOT_OK;

    for (i=0; i < 4+9; i++) {
        DataBufferSPI_TX_with_PEC[i] = 0xFF;
    }

    DataBufferSPI_TX_with_PEC[0] = ltc_cmdSTCOMM[0];
    DataBufferSPI_TX_with_PEC[1] = ltc_cmdSTCOMM[1];
    DataBufferSPI_TX_with_PEC[2] = ltc_cmdSTCOMM[2];
    DataBufferSPI_TX_with_PEC[3] = ltc_cmdSTCOMM[3];

    statusSPI = LTC_SendI2CCmd(DataBufferSPI_TX_with_PEC);

    return statusSPI;
}



/**
 * @brief   gets the frequency of the SPI clock.
 *
 * This function reads the configuration from the SPI handle directly.
 *
 * @return    frequency of the SPI clock
 */
static uint32_t LTC_GetSPIClock(void) {

    uint32_t SPI_Clock = 0;

    if (LTC_SPI_INSTANCE == SPI2 || LTC_SPI_INSTANCE == SPI3) {
        /* SPI2 and SPI3 are connected to APB1 (PCLK1) */
        /* The prescaler setup bits LTC_SPI_PRESCALER corresponds to the bits 5:3 in the SPI_CR1 register */
        /* Reference manual p.909 */
        /* The shift by 3 puts the bits 5:3 to the first position */
        /* Division are made by powers of 2 which corresponds to shifting to the right */
        /* Then 0 corresponds to divide by 2, 1 corresponds to divide by 4... so 1 has to be added to the value of the configuration bits */

        SPI_Clock = HAL_RCC_GetPCLK1Freq()>>( (LTC_SPI_PRESCALER>>3)+1);
    }

    if (LTC_SPI_INSTANCE == SPI1 || LTC_SPI_INSTANCE == SPI4 || LTC_SPI_INSTANCE == SPI5 || LTC_SPI_INSTANCE == SPI6) {
        /* SPI1, SPI4, SPI5 and SPI6 are connected to APB2 (PCLK2) */
        /* The prescaler setup bits LTC_SPI_PRESCALER corresponds to the bits 5:3 in the SPI_CR1 register */
        /* Reference manual p.909 */
        /* The shift by 3 puts the bits 5:3 to the first position */
        /* Division are made by powers of 2 which corresponds to shifting to the right */
        /* Then 0 corresponds to divide by 2, 1 corresponds to divide by 4... so 1 has to be added to the value of the configuration bits */

        SPI_Clock = HAL_RCC_GetPCLK2Freq()>>( (LTC_SPI_PRESCALER>>3)+1);
    }

    return SPI_Clock;
}




/**
 * @brief   sets the transfer time needed to receive/send data with the LTC daisy-chain.
 *
 * This function gets the clock frequency and uses the number of LTCs in the daisy-chain.
 *
 */
static void LTC_SetTransferTimes(void) {

    uint32_t transferTime_us = 0;
    uint32_t SPI_Clock = 0;

    SPI_Clock = LTC_GetSPIClock();

    /* Transmission of a command and data */
    /* Multiplication by 1000*1000 to get us */
    transferTime_us = (8*1000*1000)/(SPI_Clock);
    transferTime_us *= LTC_N_BYTES_FOR_DATA_TRANSMISSION;
    transferTime_us = transferTime_us + SPI_WAKEUP_WAIT_TIME;
    ltc_state.commandDataTransferTime = (transferTime_us/1000)+1;

    /* Transmission of a command */
    /* Multiplication by 1000*1000 to get us */
    transferTime_us = ((4)*8*1000*1000)/(SPI_Clock);
    transferTime_us = transferTime_us + SPI_WAKEUP_WAIT_TIME;
    ltc_state.commandTransferTime = (transferTime_us/1000)+1;

    /* Transmission of a command + 9 clocks */
    /* Multiplication by 1000*1000 to get us */
    transferTime_us = ((4+9)*8*1000*1000)/(SPI_Clock);
    transferTime_us = transferTime_us + SPI_WAKEUP_WAIT_TIME;
    ltc_state.gpioClocksTransferTime = (transferTime_us/1000)+1;
}



/**
 * @brief   checks the state requests that are made.
 *
 * This function checks the validity of the state requests.
 * The resuls of the checked is returned immediately.
 *
 * @param   statereq    state request to be checked
 *
 * @return              result of the state request that was made, taken from LTC_RETURN_TYPE_e
 */
static LTC_RETURN_TYPE_e LTC_CheckStateRequest(LTC_STATE_REQUEST_e statereq) {

    if (ltc_state.statereq == LTC_STATE_NO_REQUEST) {

        /* init only allowed from the uninitialized state */
        if (statereq == LTC_STATE_INIT_REQUEST) {

            if (ltc_state.state == LTC_STATEMACH_UNINITIALIZED) {
                return LTC_OK;
            } else {
                return LTC_ALREADY_INITIALIZED;
            }
        }

        return LTC_OK;

    } else {
        return LTC_REQUEST_PENDING;
    }
}



/**
 * @brief   gets the measurement initialization status.
 *
 * @return  retval  TRUE if a first measurement cycle was made, FALSE otherwise
 *
 */
extern uint8_t LTC_IsFirstMeasurementCycleFinished(void) {
    uint8_t retval = FALSE;

    OS_TaskEnter_Critical();
    retval    = ltc_state.first_measurement_made;
    OS_TaskExit_Critical();

    return (retval);
}

/**
 * @brief   sets the measurement initialization status.
 *
 * @return  none
 *
 */
extern void LTC_SetFirstMeasurementCycleFinished(void) {
    OS_TaskEnter_Critical();
    ltc_state.first_measurement_made = TRUE;
    OS_TaskExit_Critical();
}



/**
 * @brief   gets the measurement initialization status.
 *
 * @return  retval  TRUE if a first measurement cycle was made, FALSE otherwise
 *
 */
extern STD_RETURN_TYPE_e LTC_GetMuxSequenceState(void) {
    STD_RETURN_TYPE_e retval = FALSE;

    retval    = ltc_state.ltc_muxcycle_finished;

    return (retval);
}
