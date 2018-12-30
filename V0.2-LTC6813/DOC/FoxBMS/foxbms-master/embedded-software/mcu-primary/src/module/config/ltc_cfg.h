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
 * @file    ltc_cfg.h
 * @author  foxBMS Team
 * @date    18.02.2015 (date of creation)
 * @ingroup DRIVERS_CONF
 * @prefix  LTC
 *
 * @brief    Headers for the configuration for the LTC monitoring chip.
 *
 */

#ifndef LTC_CFG_H_
#define LTC_CFG_H_

/*================== Includes =============================================*/
#include "general.h"

#include "batterysystem_cfg.h"
#include "ltc_defs.h"

/*================== Macros and Definitions ===============================*/
/**
 * @ingroup CONFIG_LTC
 * If set to 1 LTC driver is configured to use foxBMS slave boards version 1.x
 * If set to 2 LTC driver is configured to use foxBMS slave boards version 2.x
*/
#define SLAVE_BOARD_VERSION 2



/* #define LTC_DISCARD_PEC TRUE */
#define LTC_DISCARD_PEC FALSE

#define LTC_GOTO_MUX_CHECK TRUE
/* #define LTC_GOTO_MUX_CHECK FALSE */

/* #define LTC_DISCARD_MUX_CHECK TRUE */
#define LTC_DISCARD_MUX_CHECK FALSE



/**
 * Number of used LTC-ICs
 */

#define LTC_N_LTC                       BS_NR_OF_MODULES
/**
 * Number of multiplexer used per LTC-IC
 */
#define LTC_N_MUX_PER_LTC               3

/**
 * Number of channels per multiplexer
 */
#define LTC_N_MUX_CHANNELS_PER_MUX      8

/**
 * Number of multiplexer measurements per LTC cycle
 */
#define LTC_NUMBER_OF_MUX_MEASUREMENTS_PER_CYCLE 8

/**
 * Number of multiplexed channels per LTC-IC
 */
#define LTC_N_MUX_CHANNELS_PER_LTC      (LTC_N_MUX_PER_LTC*LTC_N_MUX_CHANNELS_PER_MUX)

/**
 * Number of LTC-ICs per battery module
 */
#define LTC_NUMBER_OF_LTC_PER_MODULE    1

/**
 * Measurement modus for voltages
 */
#define LTC_VOLTAGE_MEASUREMENT_MODE    LTC_ADCMODE_NORMAL_DCP0
/* #define LTC_VOLTAGE_MEASUREMENT_MODE LTC_ADCMODE_FILTERED_DCP0 */
/* #define LTC_VOLTAGE_MEASUREMENT_MODE LTC_ADCMODE_FAST_DCP0 */

/**
 *  Measurement modus for GPIOs
 */
#define LTC_GPIO_MEASUREMENT_MODE   LTC_ADCMODE_NORMAL_DCP0
/* #define LTC_GPIO_MEASUREMENT_MODE LTC_ADCMODE_FILTERED_DCP0 */
/* #define LTC_GPIO_MEASUREMENT_MODE LTC_ADCMODE_FAST_DCP0 */

/**
 * SPI1 is used for communication with LTC
 */
#define LTC_SPI_HANDLE      &spi_devices[0]

#define LTC_SPI_INSTANCE    *LTC_SPI_HANDLE.Instance

#define LTC_SPI_PRESCALER   *LTC_SPI_HANDLE.Init.BaudRatePrescaler

/**
 * start definition of LTC timings
 * Twake (see LTC datasheet)
 */
#define LTC_TWAKE_US    300
/**
 * start definition of LTC timings
 * Tready (see LTC datasheet)
 */
#define LTC_TREADY_US   10
/**
 * start definition of LTC timings
 * Tidle (see LTC datasheet)
 */
#define LTC_TIDLE_US    6700

/**
 * LTC statemachine short time definition in ms
 */
#define LTC_STATEMACH_SHORTTIME     1

/**
 * time for the first initialization of the daisy chain
 * see LTC6804 datasheet page 41
 */
#define LTC_STATEMACH_DAISY_CHAIN_FIRST_INITIALIZATION_TIME     ((LTC_TWAKE_US*LTC_N_LTC)/1000)
/**
 * time for the second initialization of the daisy chain
 * see LTC6804 datasheet page 41
 */
#define LTC_STATEMACH_DAISY_CHAIN_SECOND_INITIALIZATION_TIME    ((LTC_TREADY_US*LTC_N_LTC)/1000)


/*
 * Timings of Voltage Cell and GPIO measurement for all cells or all GPIO
 */

/**
 * ~1.1ms Measurement+Calibration Cycle Time When Starting from the REFUP State in Fast Mode
 * unit: ms
 */
#define LTC_STATEMACH_MEAS_ALL_FAST_TCYCLE          2

/**
 * ~2.3ms Measurement+Calibration Cycle Time When Starting from the REFUP State in Normal Mode
 * unit: ms
 */
#define LTC_STATEMACH_MEAS_ALL_NORMAL_TCYCLE        3

/**
 * ~201ms Measurement+Calibration Cycle Time When Starting from the REFUP State in Filtered Mode
 * unit: ms
 */
#define LTC_STATEMACH_MEAS_ALL_FILTERED_TCYCLE      202


/*
 * Timings of Voltage Cell and GPIO measurement for a pair of cells or a single GPIO
 */

/**
 *  ~0.201ms Measurement+Calibration Cycle Time When Starting from the REFUP State in Fast Mode
 *  unit: ms
 */
#define LTC_STATEMACH_MEAS_SINGLE_FAST_TCYCLE       1

/**
 * ~0.405ms Measurement+Calibration Cycle Time When Starting from the REFUP State in Normal Mode
 * unit: ms
 */
#define LTC_STATEMACH_MEAS_SINGLE_NORMAL_TCYCLE     1

/**
 * ~34 ms Measurement+Calibration Cycle Time When Starting from the REFUP State in Filtered Mode
 * unit: ms
 */
#define LTC_STATEMACH_MEAS_SINGLE_FILTERED_TCYCLE   35

/**
 * LTC statemachine sequence error timing in ms
 */
#define LTC_STATEMACH_SEQERRTTIME   5
/**
 * LTC statemachine CRC-transmission error timing in ms
 */
#define LTC_STATEMACH_PECERRTIME    1

/**
 * Callback for action after initialization of LTC daisy chain
 */
#define LTC_IF_INITIALIZED_CALLBACK()

/**
 * Maximum number of re-tries in case of CRC error during the communication with daisy chain
 * before going into error state
 */
#define LTC_TRANSMIT_PECERRLIMIT    10

/**
 * Maximum number of re-tries in case of SPI error during the communication with daisy chain
 * before going into error state
 */
#define LTC_TRANSMIT_SPIERRLIMIT    3

/**
 * If set to 1, check if multiplexers acknowledged transmission
 */
#define LTC_READCOM     0

/**
 * Number of Bytes to be transmitted in daisy-chain
 * For first 4 Bytes:
 *  - 2 Bytes: command
 *  - 2 Bytes: CRC
 * Following Bytes: Data
 *  - 6 Bytes data per LTC
 *  - 2 Bytes CRC per LTC
 */
#define LTC_N_BYTES_FOR_DATA_TRANSMISSION   (4+(8*LTC_N_LTC))

/**
 * Number of Bytes to be transmitted in daisy-chain
 * Data
 *  - 6 Bytes data per LTC
 */
#define LTC_N_BYTES_FOR_DATA_TRANSMISSION_DATA_ONLY   (0+(6*LTC_N_LTC))


/* Transmit functions */
#define LTC_SendWakeUp()                SPI_Transmit(LTC_SPI_HANDLE, (uint8_t *) ltc_cmdDummy, 1)
#define LTC_SendI2CCmd(txbuf)           SPI_Transmit(LTC_SPI_HANDLE, txbuf, 4+9)
#define LTC_SendData(txbuf)             SPI_Transmit(LTC_SPI_HANDLE, txbuf, LTC_N_BYTES_FOR_DATA_TRANSMISSION)
#define LTC_SendCmd(command)            SPI_Transmit(LTC_SPI_HANDLE, (uint8_t *) command, 4)
#define LTC_ReceiveData(txbuf,rxbuf)    SPI_TransmitReceive(LTC_SPI_HANDLE, txbuf, rxbuf, LTC_N_BYTES_FOR_DATA_TRANSMISSION)


/*================== Constant and Variable Definitions ====================*/

/**
 * Definition of the multiplexer measurement sequence
 */
extern LTC_MUX_SEQUENZ_s ltc_mux_seq;

/**
 * On the foxBMS slave board there are 6 multiplexer inputs dedicated to temperature
 * sensors by default.
 * Lookup table between temperature sensors and battery cells
 */
extern const uint8_t ltc_muxsensortemperatur_cfg[BS_NR_OF_TEMP_SENSORS_PER_MODULE];

/**
 * Lookup table to indicate which voltage inpus are used
 */
extern const uint8_t ltc_voltage_input_used[BS_MAX_SUPPORTED_CELLS];

/*================== Function Prototypes ==================================*/

/**
 * @brief   converts a raw voltage from multiplexer to a temperature value in Celsius.
 *
 * The temperatures are read from NTC elements via voltage dividers.
 * This function implements the look-up table between voltage and temperature,
 * taking into account the NTC characteristics and the voltage divider.
 *
 * @param   v_adc            voltage read from the multiplexer in V
 *
 * @return  temperature     temperature value in Celsius
 */
extern float LTC_Convert_MuxVoltages_to_Temperatures(float v_adc);

/*================== Function Implementations =============================*/

#endif /* LTC_CFG_H_ */
