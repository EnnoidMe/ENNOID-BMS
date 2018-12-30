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
 * @file    diag_cfg.h
 * @author  foxBMS Team
 * @date    09.11.2015 (date of creation)
 * @ingroup ENGINE_CONF
 * @prefix  DIAG
 *
 * @brief   Diagnostic module configuration header
 *
 * In this header filer are the different diagnosis channel
 * defines assigned to different diagnosis IDs.
 *
 * Furthermore are the diagnosis error log settings be configured here..
 */

#ifndef DIAG_CFG_H_
#define DIAG_CFG_H_

/*================== Includes =============================================*/
#include "general.h"

#include "batterysystem_cfg.h"
#include "diag_id_cfg.h"

/*================== Macros and Definitions ===============================*/
#define DIAG_ERROR_SENSITIVITY_HIGH         (0)    /* logging at first event */
#define DIAG_ERROR_SENSITIVITY_MID          (5)    /* logging at fifth event */
#define DIAG_ERROR_SENSITIVITY_LOW          (10)   /* logging at tenth event */

#define DIAG_ERROR_VOLTAGE_SENSITIVITY_MSL      (500)   /*!< MSL level for event occurrence if over/under voltage event   */
#define DIAG_ERROR_TEMPERATURE_SENSITIVITY_MSL  (500)   /*!< MSL level for event occurrence if over/under temperature event    */
#define DIAG_ERROR_CURRENT_SENSITIVITY_MSL      (500)   /*!< MSL level for event occurrence if over/under current event        */

#define DIAG_ERROR_SLAVE_TEMP_SENSITIVITY_MSL   (500)   /*!< MSL level for event occurrence if slave PCB temperature event     */

#define DIAG_ERROR_LTC_PEC_SENSITIVITY          (5)
#define DIAG_ERROR_LTC_MUX_SENSITIVITY          (5)
#define DIAG_ERROR_LTC_SPI_SENSITIVITY          (5)

#define DIAG_ERROR_CAN_TIMING_SENSITIVITY       (100)
#define DIAG_ERROR_CAN_TIMING_CC_SENSITIVITY    (100)
#define DIAG_ERROR_CAN_SENSOR_SENSITIVITY       (100)

#define DIAG_ERROR_MAIN_PLUS_SENSITIVITY        (500)
#define DIAG_ERROR_MAIN_MINUS_SENSITIVITY       (500)
#define DIAG_ERROR_PRECHARGE_SENSITIVITY        (500)

#define DIAG_ERROR_INTERLOCK_SENSITIVITY        (0)

/**
 * Number of errors that can be logged
 */
#define DIAG_FAIL_ENTRY_LENGTH              (50)

/**
 * Maximum number of the same error that are logged
 */
#define DIAG_MAX_ENTRIES_OF_ERROR           (5)

/**
 * Number of contactor errors that are logged
 */
#define DIAG_FAIL_ENTRY_CONTACTOR_LENGTH    (50)


/* FIXME simple doxygen comment for each define? */
/* Initialization and startup events: 0-15 */
#define DIAG_CH_FLASHCHECKSUM                              DIAG_ID_5            /*  */
#define DIAG_CH_BKPDIAG_FAILURE                            DIAG_ID_6            /*  */
#define DIAG_CH_WATCHDOGRESET_FAILURE                      DIAG_ID_7            /*  */
#define DIAG_CH_POSTOSINIT_FAILURE                         DIAG_ID_8            /*  */
#define DIAG_CH_CALIB_EEPR_FAILURE                         DIAG_ID_9            /*  */
#define DIAG_CH_CAN_INIT_FAILURE                           DIAG_ID_10           /*  */
#define DIAG_CH_VIC_INIT_FAILURE                           DIAG_ID_11

/* HW-/SW-Runtime events: 16-31 */
#define DIAG_CH_DIV_BY_ZERO_FAILURE                        DIAG_ID_16            /*  */
#define DIAG_CH_UNDEF_INSTRUCTION_FAILURE                  DIAG_ID_17            /*  */
#define DIAG_CH_DATA_BUS_FAILURE                           DIAG_ID_18            /*  */
#define DIAG_CH_INSTRUCTION_BUS_FAILURE                    DIAG_ID_19            /*  */
#define DIAG_CH_HARDFAULT_NOTHANDLED                       DIAG_ID_20            /*  */
#define DIAG_CH_RUNTIME_ERROR_RESERVED_1                   DIAG_ID_21            /*  reserved for future needs */
#define DIAG_CH_RUNTIME_ERROR_RESERVED_2                   DIAG_ID_22            /*  reserved for future needs */
#define DIAG_CH_RUNTIME_ERROR_RESERVED_3                   DIAG_ID_23            /*  reserved for future needs */
#define DIAG_CH_CONFIGASSERT                               DIAG_ID_24            /*  */
#define DIAG_CH_SYSTEMMONITORING_TIMEOUT                   DIAG_ID_25            /*  */


/* Measurement events: 32-47 */
#define DIAG_CH_CANS_MAX_VALUE_VIOLATE                     DIAG_ID_32
#define DIAG_CH_CANS_MIN_VALUE_VIOLATE                     DIAG_ID_33
#define DIAG_CH_CANS_CAN_MOD_FAILURE                       DIAG_ID_34

/**
 * Measured frequency too low or no new value captured during last cycle
 */
#define DIAG_CH_ISOMETER_TIM_ERROR                         DIAG_ID_35

/**
 * Ground error detected
 */
#define DIAG_CH_ISOMETER_GROUNDERROR                       DIAG_ID_36

/**
 * Device error, invalid measurement result
 */
#define DIAG_CH_ISOMETER_ERROR                             DIAG_ID_37

 /**
  * Measurement trustworthy or not, hysteresis to ground error flag
  */
#define DIAG_CH_ISOMETER_MEAS_INVALID                      DIAG_ID_38

/**
 * Cell voltage limits violated
 */
#define DIAG_CH_CELLVOLTAGE_OVERVOLTAGE_MSL                DIAG_ID_39

#define DIAG_CH_CELLVOLTAGE_UNDERVOLTAGE_MSL               DIAG_ID_42

/**
 *  Temperature limits violated
 */
#define DIAG_CH_TEMP_OVERTEMPERATURE_CHARGE_MSL            DIAG_ID_45

#define DIAG_CH_TEMP_OVERTEMPERATURE_DISCHARGE_MSL         DIAG_ID_48

#define DIAG_CH_TEMP_UNDERTEMPERATURE_CHARGE_MSL           DIAG_ID_51


#define DIAG_CH_TEMP_UNDERTEMPERATURE_DISCHARGE_MSL        DIAG_ID_54

/**
 * Overcurrent
 */
#define DIAG_CH_OVERCURRENT_CHARGE_MSL                     DIAG_ID_57
#define DIAG_CH_OVERCURRENT_DISCHARGE_MSL                  DIAG_ID_60




/**
 * LTC
 */
#define DIAG_CH_LTC_SPI                                    DIAG_ID_63
#define DIAG_CH_LTC_PEC                                    DIAG_ID_64
#define DIAG_CH_LTC_MUX                                    DIAG_ID_65


/* Communication events: 50-63*/
/**
 *  CAN timing not coming
 */
#define DIAG_CH_CAN_TIMING                                 DIAG_ID_66
/**
 *  CAN C-C not coming
 */
#define DIAG_CH_CAN_CC_RESPONDING                          DIAG_ID_67
/**
 *  Current sensor not responding anymore
 */
#define DIAG_CH_CURRENT_SENSOR_RESPONDING                  DIAG_ID_68


/* Contactor events: 64-79*/
/**
 * @brief   Opening contactor at over current
 */
#define DIAG_CH_CONTACTOR_DAMAGED                          DIAG_ID_69

/**
 * @brief   counter for contactor opening
 */
#define DIAG_CH_CONTACTOR_OPENING                           DIAG_ID_70

/**
 * @brief   counter for contactor closing
 */
#define DIAG_CH_CONTACTOR_CLOSING                           DIAG_ID_71

/**
 * @brief   Contactor feedback error
 */
#define DIAG_CH_CONTACTOR_MAIN_PLUS_FEEDBACK                DIAG_ID_72
#define DIAG_CH_CONTACTOR_MAIN_MINUS_FEEDBACK               DIAG_ID_73
#define DIAG_CH_CONTACTOR_PRECHARGE_FEEDBACK                DIAG_ID_74
#define DIAG_CH_CONTACTOR_CHARGE_MAIN_PLUS_FEEDBACK         DIAG_ID_75
#define DIAG_CH_CONTACTOR_CHARGE_MAIN_MINUS_FEEDBACK        DIAG_ID_76
#define DIAG_CH_CONTACTOR_CHARGE_PRECHARGE_FEEDBACK         DIAG_ID_77

/**
 * @brief   Interlock feedback error
 */
#define DIAG_CH_INTERLOCK_FEEDBACK                          DIAG_ID_78



#define DIAG_CH_SLAVE_PCB_UNDERTEMPERATURE_MSL              DIAG_ID_79

#define DIAG_CH_SLAVE_PCB_OVERTEMPERATURE_MSL               DIAG_ID_82

/**
 * enable state of diagnosis entry
 */
typedef enum {
    DIAG_ENABLED  = 0,
    DIAG_DISABLED = 1,
} DIAG_ENABLE_STATE_e;


#if CHECK_CAN_TIMING == TRUE
    #define DIAG_CAN_TIMING DIAG_ENABLED
#else
    #define DIAG_CAN_TIMING DIAG_DISABLED
#endif

#if CURRENT_SENSOR_PRESENT == TRUE
    #define DIAG_CAN_SENSOR_PRESENT DIAG_ENABLED
#else
    #define DIAG_CAN_SENSOR_PRESENT DIAG_DISABLED
#endif

/* FIXME is it better to name it DIAG_GROUP_xxx instead of DIAG_xxx_TYPE and */
/**
 * diagnosis groups
 * failure codes FC
 */
typedef enum {
    DIAG_GENERAL_TYPE   = 0x00,     /*!< FC 0x00 - 0x1F */
    DIAG_CELLMON_TYPE   = 0x01,     /*!< FC 0x20 - 0x3F */
    DIAG_COM_TYPE       = 0x02,     /*!< FC 0x40 - 0x5F */
    DIAG_ADC_TYPE       = 0x04,     /*!< FC 0x60 - 0x7F */
    /* FIXME which failure codes for following group? */
    DIAG_CONT_TYPE      = 0x08      /*!< FC             */
} DIAG_TYPE_e;

/**
 * diagnosis recording activation
 */
typedef enum {
    DIAG_RECORDING_ENABLED   = 0x00,    /*!< enable diagnosis event recording   */
    DIAG_RECORDING_DISABLED  = 0x01,    /*!< disable diagnosis event recording  */
} DIAG_TYPE_RECORDING_e;

/* FIXME duplicate comment with enum DIAG_TYPE_e */
/* FIXME some enums are typedefed with DIAG...TYPE_e, some with DIAG_TYPE..._e! Reconsider this */
/**
 * diagnosis types for system monitoring
 */
typedef enum {
    DIAG_SYSMON_CYCLICTASK  = 0x00,     /*!< */
    DIAG_SYSMON_RESERVED    = 0x01      /*!< */
} DIAG_SYSMON_TYPE_e;

/**
 * diagnosis handling type for system monitoring
 */
typedef enum {
    DIAG_SYSMON_HANDLING_DONOTHING             = 0x00,     /*!< */
    DIAG_SYSMON_HANDLING_SWITCHOFFCONTACTOR    = 0x01      /*!< */
} DIAG_SYSMON_HANDLING_TYPE_e;


/**
 * @brief   symbolic names for diagnosis
 */
typedef enum {
    DIAG_OK     = 0,    /*!< diagnosis event ok     */
    DIAG_NOT_OK = 1,    /*!< diagnosis event not ok */
    DIAG_BUSY   = 2     /*!< diagnosis event busy   */
} Diag_ReturnType;


/**
 * @brief listing of system-relevant tasks or functions which are checked by system monitoring
 *
 * diag_sysmon_ch_cfg[]=
 */
typedef enum {
    DIAG_SYSMON_DATABASE_ID         = 0,    /*!< diag entry for database                */
    DIAG_SYSMON_SYS_ID              = 1,    /*!< diag entry for sys              */
    DIAG_SYSMON_BMS_ID              = 2,    /*!< diag entry for bms              */
    DIAG_SYSMON_ILCK_ID              = 2,    /*!< diag entry for bms              */
    DIAG_SYSMON_LTC_ID              = 3,    /*!< diag entry for ltc                     */
    DIAG_SYSMON_ISOGUARD_ID         = 4,    /*!< diag entry for ioguard                 */
    DIAG_SYSMON_CANS_ID             = 5,    /*!< diag entry for can                     */
    DIAG_SYSMON_APPL_CYCLIC_1ms     = 6,    /*!< diag entry for application 10ms task   */
    DIAG_SYSMON_APPL_CYCLIC_10ms    = 7,    /*!< diag entry for application 10ms task   */
    DIAG_SYSMON_APPL_CYCLIC_100ms   = 8,    /*!< diag entry for application 100ms task  */
    DIAG_SYSMON_MODULE_ID_MAX       = 9     /*!< end marker do not delete               */
} DIAG_SYSMON_MODULE_ID_e;

/* FIXME doxygen comment */
/* FIXME is DIAG_CODE_s an appropriate name for this? */
typedef struct {
    uint32_t GENERALmsk;
    uint32_t CELLMONmsk;
    uint32_t COMmsk;
    uint32_t ADCmsk;
} DIAG_CODE_s;


/**
 * Channel configuration of one diag channel
*/
typedef struct {
    DIAG_CH_ID_e id;                        /*!< diagnosis event id diag_id */
    uint8_t description[40];
    DIAG_TYPE_e type;                       /*!< diagnosis group of diag event */
    uint16_t thresholds;                     /*!< threshold for number of events which will be tolerated before generating a notification in both direction (OK or NOT OK)
                                             *   threshold=0: reports the value at first occurence, threshold=1:reports the value at second occurence*/
    DIAG_TYPE_RECORDING_e enablerecording;  /*!< if enabled recording in diag_memory will be activated */
    DIAG_ENABLE_STATE_e state;              /*!< if enabled diagnosis event will be evaluated */
    void (*callbackfunc)(DIAG_CH_ID_e, DIAG_EVENT_e);     /*!< will be called if number of events exceeds threshold (in both direction) with parameter DIAG_EVENT_e */
} DIAG_CH_CFG_s;


/**
 * struct for device Configuration of diag module
 */
typedef struct {
    uint8_t nr_of_ch;       /*!< number of entries in DIAG_CH_CFG_s */
    DIAG_CH_CFG_s *ch_cfg;  /*!< pointer to diag channel config struct */
} DIAG_DEV_s;

/**
 * state (in summary) used for task or function notification
 */
typedef struct {
    uint32_t state;     /*!< state              */
    uint32_t timestamp; /*!< timestamp of state */
} DIAG_SYSMON_NOTIFICATION_s;


/**
 * Channel configuration of one system monitoring channel
 */
typedef struct {
    DIAG_SYSMON_MODULE_ID_e id;                     /*!< the diag type by its symbolic name            */
    DIAG_SYSMON_TYPE_e type;                        /*!< system monitoring types: cyclic or special    */
    uint16_t threshold;                             /*!< max. delay time in ms                         */
    DIAG_TYPE_RECORDING_e enablerecording;          /*!< enabled if set to DIAG_RECORDING_ENABLED      */
    DIAG_SYSMON_HANDLING_TYPE_e handlingtype;       /*!< type of handling of system monitoring errors  */
    DIAG_ENABLE_STATE_e state;                      /*!< enable or disable system monitoring           */
    void (*callbackfunc)(DIAG_SYSMON_MODULE_ID_e);  /*!< */
} DIAG_SYSMON_CH_CFG_s;

/*================== Constant and Variable Definitions ====================*/

/**
 * diag device configuration struct
 */
extern DIAG_DEV_s diag_dev;

/**
 * diag system monitoring struct
 */
extern DIAG_SYSMON_CH_CFG_s diag_sysmon_ch_cfg[];
extern DIAG_CH_CFG_s  diag_ch_cfg[];

/*  FIXME why is it in header at all? and why is it in code at all? not used */
extern DIAG_CODE_s diag_mask;
/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

#endif /* DIAG_CFG_H_ */
