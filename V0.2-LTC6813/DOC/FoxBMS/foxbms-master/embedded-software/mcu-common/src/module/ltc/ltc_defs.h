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

#ifndef LTC_DEFS_H_
#define LTC_DEFS_H_

/*================== Includes =============================================*/
#include "general.h"
#include "database.h"
#include "spi.h"

/*================== Macros and Definitions ===============================*/


/*================== Constant and Variable Definitions ====================*/

/**
 * Voltage measurement mode
 * 1: 01 27kHz Mode (Fast) 14kHz Mode
 * 2: 10 7kHz Mode (Normal) 3kHz Mode
 * 3: 11 26Hz Mode (Filtered) 2kHz Mode
 */
typedef enum {
    LTC_ADCMODE_UNDEFINED       = 0,    /*!< ADC measurement mode is not defined        */
    LTC_ADCMODE_FAST_DCP0       = 1,    /*!< ADC measurement mode: Fast with DCP0       */
    LTC_ADCMODE_NORMAL_DCP0     = 2,    /*!< ADC measurement mode: Normal with DCP0     */
    LTC_ADCMODE_FILTERED_DCP0   = 3,    /*!< ADC measurement mode: Filtered with DCP0   */
    LTC_ADCMODE_FAST_DCP1       = 4,    /*!< ADC measurement mode: Fast with DCP1       */
    LTC_ADCMODE_NORMAL_DCP1     = 5,    /*!< ADC measurement mode: Normal with DCP1     */
    LTC_ADCMODE_FILTERED_DCP1   = 6,    /*!< ADC measurement mode: Filtered with DCP1   */
} LTC_ADCMODE_e;

/**
 * Number of measured channels
 */
typedef enum {
    LTC_ADCMEAS_UNDEFINED               = 0,    /*!< not defined                            */
    LTC_ADCMEAS_ALLCHANNEL              = 1,    /*!< all ADC channels are measured          */
    LTC_ADCMEAS_SINGLECHANNEL_GPIO1     = 2,    /*!< only a single ADC channel (GPIO1) is measured  */
    LTC_ADCMEAS_SINGLECHANNEL_GPIO2     = 3,    /*!< only a single ADC channel (GPIO2) is measured  */
    LTC_ADCMEAS_SINGLECHANNEL_GPIO3     = 4,    /*!< only a single ADC channel (GPIO3) is measured  */
    LTC_ADCMEAS_SINGLECHANNEL_GPIO4     = 5,    /*!< only a single ADC channel (GPIO4) is measured  */
    LTC_ADCMEAS_SINGLECHANNEL_GPIO5     = 6,    /*!< only a single ADC channel (GPIO5) is measured  */
    LTC_ADCMEAS_SINGLECHANNEL_TWOCELLS  = 7,    /*!< only two cell voltages are measured */
    LTC_ADCMEAS_ALLCHANNEL_SC           = 8,    /*!< all ADC channels + sum of cells are measured   */
} LTC_ADCMEAS_CHAN_e;

/**
 * States of the LTC state machine
 */
typedef enum {
    /* Init-Sequence */
    LTC_STATEMACH_UNINITIALIZED             = 0,    /*!<    */
    LTC_STATEMACH_INITIALIZATION            = 1,    /*!<    */
    LTC_STATEMACH_REINIT                    = 2,    /*!<    */
    LTC_STATEMACH_INITIALIZED               = 3,    /*!< LTC is initialized                             */
    /* Voltage Measurement Cycle */
    LTC_STATEMACH_IDLE                      = 4,    /*!<    */
    LTC_STATEMACH_STARTMEAS                 = 5,    /*!<    */
    LTC_STATEMACH_READVOLTAGE               = 6,    /*!<    */
    LTC_STATEMACH_MUXMEASUREMENT            = 7,    /*!< Mux (Temperature and Balancing) Measurement    */
    LTC_STATEMACH_MUXMEASUREMENT_FINISHED   = 12,   /*!<    */
    LTC_STATEMACH_BALANCECONTROL            = 14,   /*!<    */
    LTC_STATEMACH_ALLGPIOMEASUREMENT        = 15,   /*!<    */
    LTC_STATEMACH_READALLGPIO               = 16,   /*!<    */
    LTC_STATEMACH_READVOLTAGE_2CELLS        = 17,
    LTC_STATEMACH_STARTMEAS_2CELLS          = 18,
    LTC_STATEMACH_USER_IO_CONTROL           = 19,   /*!< Control of the user port expander              */
    LTC_STATEMACH_USER_IO_FEEDBACK          = 20,   /*!< Control of the user port expander              */
    LTC_STATEMACH_EEPROM_READ               = 21,   /*!< Control of the external EEPROM                 */
    LTC_STATEMACH_EEPROM_WRITE              = 22,   /*!< Control of the external EEPROM                 */
    LTC_STATEMACH_TEMP_SENS_READ            = 23,   /*!< Control of the external temperature sensor     */
    LTC_STATEMACH_BALANCEFEEDBACK           = 24,
    LTC_STATEMACH_OPENWIRE_CHECK            = 25,
    LTC_STATEMACH_DEVICE_PARAMETER          = 26,
    LTC_STATEMACH_ADC_ACCURACY              = 27,
    LTC_STATEMACH_DIGITAL_FILTER            = 28,
    LTC_STATEMACH_VOLTMEAS_SUMOFCELLS       = 29,
    LTC_STATEMACH_EEPROM_READ_UID           = 30,   /*!< Control of the external EEPROM                 */
    LTC_STATEMACH_UNDEFINED                 = 31,   /*!< undefined state                                */
    LTC_STATEMACH_RESERVED1                 = 0x80, /*!< reserved state                                 */
    LTC_STATEMACH_ERROR_SPIFAILED           = 0xF0, /*!< Error-State: SPI error                         */
    LTC_STATEMACH_ERROR_PECFAILED           = 0xF1, /*!< Error-State: PEC error                         */
    LTC_STATEMACH_ERROR_MUXFAILED           = 0xF2, /*!< Error-State: multiplexer error                 */
    LTC_STATEMACH_ERROR_INITIALIZATION      = 0xF3, /*!< Error-State: initialization error              */
} LTC_STATEMACH_e;

/**
 * General substates
 */
typedef enum {
    LTC_ENTRY               = 0,    /*!< Substate entry state       */
    LTC_ERROR_ENTRY         = 1,    /*!< Substate entry error       */
    LTC_ERROR_PROCESSED     = 2,    /*!< Substate error processed   */
} LTC_STATEMACH_SUB_e;

/**
 * Substates for the uninitialized state
 */
typedef enum {
    LTC_ENTRY_UNINITIALIZED     = 0,    /*!< Initialize-sequence */
} LTC_STATEMACH_UNINITIALIZED_SUB_e;

/**
 * Substates for the initialization state
 */
typedef enum {
    /* Init-Sequence */
    LTC_ENTRY_INITIALIZATION            = 0,    /*!<    */
    LTC_START_INIT_INITIALIZATION       = 1,    /*!<    */
    LTC_RE_ENTRY_INITIALIZATION         = 2,    /*!<    */
    LTC_READ_INITIALIZATION_REGISTER    = 3,    /*!<    */
    LTC_EXIT_INITIALIZATION             = 4,    /*!<    */
} LTC_STATEMACH_INITIALIZATION_SUB_e;

/**
 * Substates for the uninitialized state
 */
typedef enum {
    LTC_ENTRY_INITIALIZED   = 0,    /*!<    */
} LTC_STATEMACH_INITIALIZED_SUB_e;

/**
 * Substates for the read voltage state
 */
typedef enum {
    /* Init-Sequence */
    LTC_READ_VOLTAGE_REGISTER_A_RDCVA_READVOLTAGE   = 0,    /*!<    */
    LTC_READ_VOLTAGE_REGISTER_B_RDCVB_READVOLTAGE   = 1,    /*!<    */
    LTC_READ_VOLTAGE_REGISTER_C_RDCVC_READVOLTAGE   = 2,    /*!<    */
    LTC_READ_VOLTAGE_REGISTER_D_RDCVD_READVOLTAGE   = 3,    /*!<    */
    LTC_READ_VOLTAGE_REGISTER_E_RDCVE_READVOLTAGE   = 4,    /*!<    */
    LTC_READ_VOLTAGE_REGISTER_F_RDCVF_READVOLTAGE   = 5,    /*!<    */
    LTC_EXIT_READVOLTAGE                            = 6,    /*!<    */
    LTC_READ_AUXILIARY_REGISTER_A_RDAUXA  = 7,    /*!<    */
    LTC_READ_AUXILIARY_REGISTER_B_RDAUXB  = 8,    /*!<    */
    LTC_READ_AUXILIARY_REGISTER_C_RDAUXC  = 9,    /*!<    */
    LTC_READ_AUXILIARY_REGISTER_D_RDAUXD  = 10,   /*!<    */
    LTC_EXIT_READAUXILIARY_ALLGPIOS                   = 11,   /*!<    */
} LTC_STATEMACH_READVOLTAGE_SUB_e;

/**
 * Substates for the cell voltage + SC measurement state
 */
typedef enum {
    LTC_VOLTMEAS_SC_TRIGGER_CONVERSION  = 0,
    LTC_VOLTMEAS_SC_READ_SC             = 1,
    LTC_EXIT_VOLTMEAS_SC                = 2,
}LTC_STATEMACH_READVOLTAGE_SC_SUB_e;

/**
 * Substates for the balance control state
 */
typedef enum {
    /* Init-Sequence */
    LTC_CONFIG_BALANCECONTROL           = 0,    /*!<    */
    LTC_CONFIG2_BALANCECONTROL          = 1,    /*!<    */
    LTC_CONFIG2_BALANCECONTROL_END      = 2,    /*!<    */
    LTC_REQUEST_FEEDBACK_BALANCECONTROL = 3,    /*!<    */
    LTC_READ_FEEDBACK_BALANCECONTROL    = 4,    /*!<    */
    LTC_SAVE_FEEDBACK_BALANCECONTROL    = 5,    /*!<    */
    LTC_EXIT_BALANCECONTROL             = 6,    /*!<    */
    LTC_STATEMACH_STARTMUXMEASUREMENT   = 7,
    LTC_STATEMACH_MUXCONFIGURATION_INIT = 8,    /*!<    */
    LTC_STATEMACH_MUXMEASUREMENT_CONFIG = 9,    /*!< Configuration of the multiplexers              */
    LTC_STATEMACH_READMUXMEASUREMENT    = 11,   /*!<    */
    LTC_STATEMACH_STOREMUXMEASUREMENT   = 12,   /*!<    */
} LTC_STATEMACH_BALANCECONTROL_SUB;

/**
 * Substates for open-wire check
 */
typedef enum {
    LTC_REQUEST_PULLUP_CURRENT_OPENWIRE_CHECK   = 0,    /*!<    */
    LTC_READ_VOLTAGES_PULLUP_OPENWIRE_CHECK     = 1,    /*!<    */
    LTC_REQUEST_PULLDOWN_CURRENT_OPENWIRE_CHECK = 2,    /*!<    */
    LTC_READ_VOLTAGES_PULLDOWN_OPENWIRE_CHECK   = 3,    /*!<    */
    LTC_PERFORM_OPENWIRE_CHECK                  = 4
}LTC_STATEMACH_OPENWIRECHECK_SUB;

/**
 * Substates for diagnosis state
 */
typedef enum {
    LTC_REQUEST_DEVICE_PARAMETER                        = 0,    /*!<    */
    LTC_DEVICE_PARAMETER_READ_STATUS_REGISTER_A         = 1,
    LTC_DEVICE_PARAMETER_READ_STATUS_REGISTER_B         = 2,
    LTC_REQUEST_REDUNDANCY_CHECK                        = 3,
    LTC_REDUNDANCY_READ_STATUS_REGISTER_A               = 4,
    LTC_REDUNDANCY_READ_STATUS_REGISTER_B               = 5,
    LTC_EXIT_DEVICE_PARAMETER                           = 6,
} LTC_STATEMACH_DIAGNOSIS_SUB_e;


/**
 * Substates for ADC accuracy check
 */
typedef enum {
    LTC_REQUEST_ADC1_VERIFICATION      = 0,
    LTC_READ_ADC1_VERIFICATION_VALUE   = 1,
    LTC_SAVE_ADC1_VERIFICATION_VALUE   = 2,
    LTC_REQUEST_ADC2_VERIFICATION      = 3,
    LTC_READ_ADC2_VERIFICATION_VALUE   = 4,
    LTC_EXIT_ADC_ACCURACY_CHECK        = 5,
} LTC_STATEMACH_ADC_ACCURACY_CHECK_SUB_e;


/**
 * Substates for digital filter check
 */
typedef enum {
    LTC_REQUEST_CLEAR_CELLVOLTAGES                  = 0,
    LTC_REQUEST_CLEAR_AUX_REGISTER                  = 1,
    LTC_REQUEST_CLEAR_STATUS_REGISTER               = 2,
    LTC_REQUEST_DIGITAL_FILTER_CHECK_CELLVOLTAGES   = 3,
    LTC_DIGITAL_FILTER_SELF_TEST_1_CELLVOLTAGES     = 4,
    LTC_DIGITAL_FILTER_SELF_TEST_1_AUX              = 5,
    LTC_DIGITAL_FILTER_READ_STATUS_REGISTER_A       = 6,
    LTC_DIGITAL_FILTER_READ_STATUS_REGISTER_B       = 7,
    LTC_DIGITAL_FILTER_SELF_TEST_1_STATUS           = 8,
    LTC_DIGITAL_FILTER_SELF_TEST_2_CELLVOLTAGES     = 9,
    LTC_DIGITAL_FILTER_SELF_TEST_2_AUX              = 10,
    LTC_DIGITAL_FILTER_READ_STATUS_REGISTER_A_2     = 11,
    LTC_DIGITAL_FILTER_READ_STATUS_REGISTER_B_2     = 12,
    LTC_DIGITAL_FILTER_SELF_TEST_2_STATUS           = 13,
    LTC_EXIT_DIGITAL_FILTER_CHECK                   = 14,
}LTC_STATEMACH_DIGITAL_FILTER_SUB_e;


/**
 * Substates for the user IO control state
 */
typedef enum {
    LTC_USER_IO_SET_OUTPUT_REGISTER                                 = 0,    /*!<    */
    LTC_USER_IO_READ_INPUT_REGISTER                                 = 1,    /*!<    */
    LTC_USER_IO_SEND_CLOCK_STCOMM                                   = 2,    /*!<    */
    LTC_USER_IO_READ_I2C_TRANSMISSION_RESULT_RDCOMM                 = 3,    /*!<    */
    LTC_USER_IO_SAVE_DATA                                           = 4,    /*!<    */
    LTC_USER_IO_FINISHED                                            = 5,    /*!<    */
} LTC_STATEMACH_USER_IO_CONTROL_e;

/**
 * Substates for the EEPROM control state
 */
typedef enum {
    LTC_EEPROM_SET_READ_ADDRESS                                     = 0,    /*!<    */
    LTC_EEPROM_READ_DATA1                                           = 1,    /*!<    */
    LTC_EEPROM_READ_DATA2                                           = 2,    /*!<    */
    LTC_EEPROM_WRITE_DATA1                                          = 3,    /*!<    */
    LTC_EEPROM_WRITE_DATA2                                          = 4,    /*!<    */
    LTC_EEPROM_SEND_CLOCK_STCOMM1                                   = 5,    /*!<    */
    LTC_EEPROM_SEND_CLOCK_STCOMM2                                   = 6,    /*!<    */
    LTC_EEPROM_SEND_CLOCK_STCOMM3                                   = 7,    /*!<    */
    LTC_EEPROM_SEND_CLOCK_STCOMM4                                   = 8,    /*!<    */
    LTC_EEPROM_READ_I2C_TRANSMISSION_RESULT_RDCOMM                  = 9,    /*!<    */
    LTC_EEPROM_SAVE_READ                                            = 10,    /*!<    */
    LTC_EEPROM_FINISHED                                             = 11,    /*!<    */
} LTC_STATEMACH_EEPROM_READ_UID_e;

/**
 * Substates for the temperature sensor control state
 */
typedef enum {
    LTC_TEMP_SENS_SEND_DATA1                                        = 0,    /*!<    */
    LTC_TEMP_SENS_READ_DATA1                                        = 1,    /*!<    */
    LTC_TEMP_SENS_READ_DATA3                                        = 2,    /*!<    */
    LTC_TEMP_SENS_SEND_CLOCK_STCOMM1                                = 3,    /*!<    */
    LTC_TEMP_SENS_SEND_CLOCK_STCOMM2                                = 4,    /*!<    */
    LTC_TEMP_SENS_READ_I2C_TRANSMISSION_RESULT_RDCOMM               = 5,    /*!<    */
    LTC_TEMP_SENS_SAVE_TEMP                                         = 6,    /*!<    */
    LTC_TEMP_SENS_FINISHED                                          = 7,    /*!<    */
} LTC_STATEMACH_TEMP_SENS_READ_e;

/**
 * Substates for the multiplexer measurement configuration state
 */
typedef enum {
    /* Init-Sequence */
    LTC_SET_MUX_CHANNEL_WRCOMM_MUXMEASUREMENT_CONFIG                = 0,    /*!<    */
    LTC_SEND_CLOCK_STCOMM_MUXMEASUREMENT_CONFIG                     = 1,    /*!<    */
    LTC_READ_I2C_TRANSMISSION_RESULT_RDCOMM_MUXMEASUREMENT_CONFIG   = 2,    /*!<    */
    LTC_READ_I2C_TRANSMISSION_CHECK_MUXMEASUREMENT_CONFIG           = 3,    /*!<    */
    LTC_START_GPIO_MEASUREMENT_MUXMEASUREMENT_CONFIG                = 4,    /*!<    */
} LTC_STATEMACH_MUXMEASUREMENT_CONFIG_SUB_e;

/**
 * Substates for the all-GPIO multiplexer measurement state
 */
typedef enum {
    /* Init-Sequence */
    LTC_READ_AUXILIARY_REGISTER_A_RAUXA_READALLGPIO = 0,    /*!<    */
    LTC_READ_AUXILIARY_REGISTER_B_RAUXB_READALLGPIO = 1,    /*!<    */
    LTC_READ_AUXILIARY_REGISTER_C_RAUXC_READALLGPIO = 2,    /*!<    */
    LTC_READ_AUXILIARY_REGISTER_D_RAUXD_READALLGPIO = 3,    /*!<    */
    LTC_EXIT_READALLGPIO                            = 4,    /*!<    */
} LTC_STATEMACH_READALLGPIO_SUB_e;

/**
 * Substates for the single GPIO multiplexer measurement state
 */
typedef enum {
    /* Init-Sequence */
    LTC_READ_AUXILIARY_REGISTER_A_RAUXA_MUXMEASUREMENT  = 0,    /*!<    */
    LTC_SAVE_MUX_MEASUREMENT_MUXMEASUREMENT             = 1,    /*!<    */
} LTC_STATEMACH_MUXMEASUREMENT_SUB_e;

/**
 * State requests for the LTC statemachine
 */
typedef enum {
    LTC_STATE_INIT_REQUEST                = LTC_STATEMACH_INITIALIZATION,           /*!<    */
    LTC_STATE_USER_IO_WRITE_REQUEST       = LTC_STATEMACH_USER_IO_CONTROL,          /*!<    */
    LTC_STATE_USER_IO_READ_REQUEST        = LTC_STATEMACH_USER_IO_FEEDBACK,          /*!<    */
    LTC_STATE_USER_IO_REQUEST             = LTC_STATEMACH_USER_IO_CONTROL,          /*!<    */
    LTC_STATE_EEPROM_READ_REQUEST         = LTC_STATEMACH_EEPROM_READ    ,          /*!<    */
    LTC_STATE_EEPROM_WRITE_REQUEST        = LTC_STATEMACH_EEPROM_WRITE    ,          /*!<    */
    LTC_STATE_EEPROM_READ_UID_REQUEST     = LTC_STATEMACH_EEPROM_READ    ,          /*!<    */
    LTC_STATE_TEMP_SENS_READ_REQUEST      = LTC_STATEMACH_TEMP_SENS_READ,           /*!<    */
    LTC_STATE_BALANCEFEEDBACK_REQUEST     = LTC_STATEMACH_BALANCEFEEDBACK,
    LTC_STATE_REINIT_REQUEST              = LTC_STATEMACH_REINIT,                   /*!<    */
    LTC_STATE_IDLE_REQUEST                = LTC_STATEMACH_IDLE,                     /*!<    */
    LTC_STATE_VOLTAGEMEASUREMENT_REQUEST  = LTC_STATEMACH_STARTMEAS,                /*!<    */
    LTC_STATE_VOLTAGEMEASUREMENT_2CELLS_REQUEST = LTC_STATEMACH_STARTMEAS_2CELLS,   /*!<    */
    LTC_STATE_VOLTAGEMEASUREMENT_SC_REQUEST = LTC_STATEMACH_VOLTMEAS_SUMOFCELLS,    /*!<    */
    LTC_STATE_READVOLTAGE_REQUEST         = LTC_STATEMACH_READVOLTAGE,              /*!<    */
    LTC_STATE_READVOLTAGE_2CELLS_REQUEST  = LTC_STATEMACH_READVOLTAGE_2CELLS,
    LTC_STATE_MUXMEASUREMENT_REQUEST      = LTC_STATEMACH_MUXCONFIGURATION_INIT,      /*!<    */
    LTC_STATE_BALANCECONTROL_REQUEST      = LTC_STATEMACH_BALANCECONTROL,           /*!<    */
    LTC_STATEMACH_BALANCEFEEDBACK_REQUEST = LTC_STATEMACH_BALANCEFEEDBACK,
    LTC_STATE_ALLGPIOMEASUREMENT_REQUEST  = LTC_STATEMACH_ALLGPIOMEASUREMENT,       /*!<    */
    LTC_STATE_OPENWIRE_CHECK_REQUEST      = LTC_STATEMACH_OPENWIRE_CHECK,
    LTC_STATEMACH_DEVICE_PARAMETER_REQUEST = LTC_STATEMACH_DEVICE_PARAMETER,
    LTC_STATEMACH_ADC_ACCURACY_REQUEST    = LTC_STATEMACH_ADC_ACCURACY,
    LTC_STATEMACH_DIGITAL_FILTER_REQUEST = LTC_STATEMACH_DIGITAL_FILTER,
    LTC_STATE_NO_REQUEST                  = LTC_STATEMACH_RESERVED1,                /*!<    */
} LTC_STATE_REQUEST_e;

/**
 * Possible return values when state requests are made to the LTC statemachine
 */
typedef enum {
    LTC_OK                  = 0,    /*!< LTC --> ok                             */
    LTC_BUSY_OK             = 1,    /*!< LTC under load --> ok                  */
    LTC_REQUEST_PENDING     = 2,    /*!< requested to be executed               */
    LTC_ILLEGAL_REQUEST     = 3,    /*!< Request can not be executed            */
    LTC_SPI_ERROR           = 4,    /*!< Error state: Source: SPI               */
    LTC_PEC_ERROR           = 5,    /*!< Error state: Source: PEC               */
    LTC_MUX_ERROR           = 6,    /*!< Error state: Source: MUX               */
    LTC_INIT_ERROR          = 7,    /*!< Error state: Source: Initialization    */
    LTC_OK_FROM_ERROR       = 8,    /*!< Return from error --> ok               */
    LTC_ERROR               = 20,   /*!< General error state                    */
    LTC_ALREADY_INITIALIZED = 30,   /*!< Initialization of LTC already finished */
    LTC_ILLEGAL_TASK_TYPE   = 99,   /*!< Illegal                                */
} LTC_RETURN_TYPE_e;

/**
 * Variable to define if the LTC should measure are restart the initialization sequence
 */
typedef enum {
    LTC_HAS_TO_MEASURE  = 0,    /*!< measurement state of the LTC       */
    LTC_HAS_TO_REINIT   = 1,    /*!< re-initialization state of the LTC */
    LTC_HAS_TO_MEASURE_2CELLS = 2, /*!< measurement state of the LTC    */
} LTC_TASK_TYPE_e;


typedef struct {
    uint8_t muxID;  /*!< multiplexer ID 0 - 3       */
    uint8_t muxCh;  /*!< multiplexer channel 0 - 7   */
} LTC_MUX_CH_CFG_s;

typedef struct {
    uint8_t nr_of_steps;        /*!< number of steps in the multiplexer sequence   */
    LTC_MUX_CH_CFG_s *seqptr;   /*!< pointer to the multiplexer sequence   */
} LTC_MUX_SEQUENZ_s;

/**
 * This struct contains pointer to used data buffers
 */
typedef struct {
    DATA_BLOCK_CELLVOLTAGE_s *cellvoltage;
    DATA_BLOCK_CELLTEMPERATURE_s *celltemperature;
    DATA_BLOCK_MINMAX_s *minmax;
    DATA_BLOCK_BALANCING_FEEDBACK_s *balancing_feedback;
    DATA_BLOCK_USER_MUX_s *user_mux;
    DATA_BLOCK_BALANCING_CONTROL_s *balancing_control;
    DATA_BLOCK_SLAVE_CONTROL_s *user_io_control;
    DATA_BLOCK_OPENWIRE_s *openWire_check;       /* Wie genau open wire check behandeln? Was genau abspeichern? */
    DATA_BLOCK_LTC_DEVICE_PARAMETER_s *deviceParameter;
    DATA_BLOCK_LTC_ADC_ACCURACY_s * adcAccuracy;
    int * openWire_buffer;                   /* BS_NR_OF_BAT_CELLS */
    uint8_t * spi_TX_withPEC;                /* 12 byte */
    uint8_t * spi_RX_withPEC;                /* 12 byte */
    uint16_t * GPIOVoltages;                 /* LTC2_NUMBER_OF_GPIOS * NR_OF_LTCs */
    uint16_t * valid_GPIOPECs;               /* NR_OF_LTCs */
}LTC_DATAPTR_s;

/**
 * This struct contains error counter and pointer to used error buffers
 */
typedef struct {
    uint32_t * errPECCnt;       /* array length: Number of used LTCs */
    uint32_t * errSPICnt;       /* array length: Number of used LTCs */
    uint8_t * ltcStatus;        /* array length: Number of used LTCs */
    uint8_t errPECRetryCnt;
    uint8_t errSPIRetryCnt;
    uint8_t errOccurred;
    uint32_t nrOfConsecutiveErrors;
}LTC_ERROR_s;

/**
 * This struct contains the measurement configuration for the LTC
 * Measurement is deactivated with value = LTC_STATE_NO_REQUEST
 */
typedef struct {
    LTC_STATE_REQUEST_e measVoltage;                /* activated = LTC_STATE_VOLTAGEMEASUREMENT_REQUEST */
    LTC_STATE_REQUEST_e measVoltage2Cells;          /* activated = LTC_STATE_VOLTAGEMEASUREMENT_2CELLS_REQUEST */
    LTC_STATE_REQUEST_e measVoltageSumofCells;      /* activated = LTC_STATE_VOLTAGEMEASUREMENT_SC_REQUEST */
    LTC_STATE_REQUEST_e measMux;                    /* activated = LTC_STATE_MUXMEASUREMENT_REQUEST */
    LTC_STATE_REQUEST_e balancing;                  /* activated = LTC_STATE_BALANCECONTROL_REQUEST */
    LTC_STATE_REQUEST_e balancing_feedback;         /* activated = LTC_STATE_BALANCEFEEDBACK_REQUEST */
    LTC_STATE_REQUEST_e measAllGPIO;                /* activated = LTC_STATE_ALLGPIOMEASUREMENT_REQUEST */
    LTC_STATE_REQUEST_e userIO;                     /* activated = LTC_STATE_USER_IO_REQUEST */
    LTC_STATE_REQUEST_e readEEPROM;                 /* activated = LTC_STATE_EEPROM_READ_UID_REQUEST */
    LTC_STATE_REQUEST_e measTemperature;            /* activated = LTC_STATE_TEMP_SENS_READ_REQUEST */
    LTC_STATE_REQUEST_e openWireCheck;              /* activated = LTC_STATE_OPENWIRE_CHECK_REQUEST */
    LTC_STATE_REQUEST_e deviceParameterCheck;       /* activated = LTC_STATEMACH_DEVICE_PARAMETER_REQUEST */
    LTC_STATE_REQUEST_e accuracyADCverification;    /* activated = LTC_STATEMACH_ADC_ACCURACY_REQUEST */
    LTC_STATE_REQUEST_e digitalFilterCheck;         /* activated = LTC_STATEMACH_DIGITAL_FILTER_REQUEST */
    uint8_t taskCycleCnt;           /* holds the current state machine index */
    uint8_t numberActiveOfStates;   /* number of active states */
    uint8_t activeStates[12];       /* array holds the different substates that are executed one after another */
                                    /* maximum number of states : 12 */
}LTC_CONFIG_s;


/**
 * This structure contains all the variables relevant for the LTC state machine.
 * The user can get the current state of the LTC state machine with this variable
 */
typedef struct {
    uint16_t timer;                         /*!< time in ms before the state machine processes the next state, e.g. in counts of 1ms    */
    LTC_TASK_TYPE_e taskMode;                /*!< current task of the state machine                                                      */
    LTC_STATE_REQUEST_e statereq;           /*!< current state request made to the state machine                                        */
    LTC_STATEMACH_e state;                  /*!< state of Driver State Machine                                                          */
    uint8_t substate;                       /*!< current substate of the state machine                                                  */
    LTC_STATEMACH_e laststate;              /*!< previous state of the state machine                                                    */
    uint8_t lastsubstate;                   /*!< previous substate of the state machine                                                 */
    uint8_t configuration[6];               /*!< holds the configuration of the ltc (configuration register)                            */
    LTC_ADCMODE_e adcMode;                  /*!< current LTC ADCmeasurement mode (fast, normal or filtered)                                             */
    LTC_ADCMODE_e voltMeasMode;             /*!< current LTC ADCmeasurement mode (fast, normal or filtered)                                             */
    LTC_ADCMODE_e gpioMeasMode;             /*!< current LTC ADCmeasurement mode (fast, normal or filtered)                                             */
    LTC_ADCMODE_e adcModereq;               /*!< requested LTC ADCmeasurement mode (fast, normal or filtered)                                           */
    LTC_ADCMEAS_CHAN_e adcMeasCh;           /*!< current number of channels measured for GPIOS (one at a time for multiplexers or all five GPIOs)       */
    LTC_ADCMEAS_CHAN_e adcMeasChreq;        /*!< requested number of channels measured for GPIOS (one at a time for multiplexers or all five GPIOs)     */
    uint8_t numberOfMeasuredMux;            /*!< number of multiplexer channels measured by the LTC chip before a voltage measurement is made           */
    uint32_t ErrPECCounter;                 /*!< counts the number of times there was A PEC (CRC) error during communication with LTC*/
    uint8_t ErrRetryCounter;                /*!< counts how many times the drivers retried to communicate with LTC in case of a communication error*/
    uint32_t ErrRequestCounter;             /*!< counts the number of illegal requests to the LTC state machine */
    uint8_t triggerentry;                   /*!< counter for re-entrance protection (function running flag) */
    uint32_t commandDataTransferTime;       /*!< time needed for sending an instruction to the LTC, followed by data transfer from the LTC   */
    uint32_t commandTransferTime;           /*!< time needed for sending an instruction to the LTC                                           */
    uint32_t gpioClocksTransferTime;        /*!< time needed for sending 72 clock signal to the LTC, used for I2C communication              */
    uint32_t VoltageSampleTime;             /*!< time stamp at which the cell voltage were measured                                          */
    uint32_t muxSampleTime;                 /*!< time stamp at which a multiplexer input was measured                                        */
    LTC_MUX_CH_CFG_s *muxmeas_seqptr;       /*!< pointer to the multiplexer sequence to be measured (contains a list of elements [multiplexer id, multiplexer channels]) (1,-1)...(3,-1),(0,1),...(0,7) */
    LTC_MUX_CH_CFG_s *muxmeas_seqendptr;    /*!< point to the end of the multiplexer sequence; pointer to ending point of sequence */
    uint8_t muxmeas_nr_end;                 /*!< number of multiplexer channels that have to be measured; end number of sequence, where measurement is finished*/
    SPI_HandleType_s *spiHandle;           /*!< pointer to SPI Handle the LTC is connected to                                               */
    LTC_DATAPTR_s ltcData;                  /*!< contains pointer to the local data buffer                                                   */
    uint8_t instanceID;                     /*!< number to distinguish between different ltc states, starting with 0,1,2,3....8              */
    uint8_t nrBatcellsPerModule;            /*!< number of cells per module                                                                  */
    uint8_t busSize;                        /*!< number of connected LTCs to parallel bus network                                            */
    LTC_ERROR_s errStatus;                  /*!< contains pointer to local error buffer and error indicators                                 */
    uint8_t * ltcIDs;                       /*!< array with LTC IDs                                                                          */
    uint8_t cntDeviceRD;                    /*!< current Index of array ltcIDs to determine device ID                                        */
    uint32_t ctrlCallCnt;                   /*!< counts the LTC2_CTRL calls                                                                  */
    uint8_t taskCycleCnt;                   /*!< counts the current task cycle                                                               */
    uint8_t reusageMeasurementMode;         /*!< flag that indicates if currently any state is reused i.e. cell voltage measurement          */
    LTC_CONFIG_s ltcConfig;                 /*!< struct that holds the measurement configuration of the ltc network                          */
    uint8_t first_measurement_made;         /*!< flag that indicates if the first measurement cycle was completed                            */
    STD_RETURN_TYPE_e ltc_muxcycle_finished;/*!< flag that indictes if the measurement sequence of the multiplexers is finished              */
    STD_RETURN_TYPE_e check_spi_flag;       /*!< indicates if interrupt flag or timer must be considered*/
    STD_RETURN_TYPE_e balance_control_done; /*!< indicates if balance control was done*/
} LTC_STATE_s;

/*================== Function Prototypes ==================================*/


/*================== Function Implementations =============================*/

#endif /* LTC_DEFS_H_ */
