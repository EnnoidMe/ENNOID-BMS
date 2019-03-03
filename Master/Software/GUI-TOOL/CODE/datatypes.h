/*
    Original copyright 2018 Benjamin Vedder benjamin@vedder.se and the VESC Tool project ( https://github.com/vedderb/vesc_tool )
    Now forked to:
    Danny Bokma github@diebie.nl

    This file is part of BMS Tool.

    DieBieMS Tool is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DieBieMS Tool is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DATATYPES_H
#define DATATYPES_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <stdint.h>

typedef struct {
    QString name;
    QString systemPath;
    bool isVesc;
} VSerialInfo_t;

typedef enum {
    CFG_T_UNDEFINED = 0,
    CFG_T_DOUBLE,
    CFG_T_INT,
    CFG_T_QSTRING,
    CFG_T_ENUM,
    CFG_T_BOOL
} CFG_T;

typedef enum {
    VESC_TX_UNDEFINED = 0,
    VESC_TX_UINT8,
    VESC_TX_INT8,
    VESC_TX_UINT16,
    VESC_TX_INT16,
    VESC_TX_UINT32,
    VESC_TX_INT32,
    VESC_TX_DOUBLE16,
    VESC_TX_DOUBLE32,
    VESC_TX_DOUBLE32_AUTO
} VESC_TX_T;

typedef enum {
    FAULT_CODE_NONE = 0,
    FAULT_CODE_OVER_VOLTAGE,
    FAULT_CODE_UNDER_VOLTAGE,
    FAULT_CODE_DRV,
    FAULT_CODE_ABS_OVER_CURRENT,
    FAULT_CODE_OVER_TEMP_FET,
    FAULT_CODE_OVER_TEMP_MOTOR
} mc_fault_code;

typedef enum {
    DISP_POS_MODE_NONE = 0,
    DISP_POS_MODE_INDUCTANCE,
    DISP_POS_MODE_OBSERVER,
    DISP_POS_MODE_ENCODER,
    DISP_POS_MODE_PID_POS,
    DISP_POS_MODE_PID_POS_ERROR,
    DISP_POS_MODE_ENCODER_OBSERVER_ERROR
} disp_pos_mode;

struct MC_VALUES {
    Q_GADGET

    Q_PROPERTY(double v_in MEMBER v_in)
    Q_PROPERTY(double temp_mos MEMBER temp_mos)
    Q_PROPERTY(double temp_motor MEMBER temp_motor)
    Q_PROPERTY(double current_motor MEMBER current_motor)
    Q_PROPERTY(double current_in MEMBER current_in)
    Q_PROPERTY(double id MEMBER id)
    Q_PROPERTY(double iq MEMBER iq)
    Q_PROPERTY(double rpm MEMBER rpm)
    Q_PROPERTY(double duty_now MEMBER duty_now)
    Q_PROPERTY(double amp_hours MEMBER amp_hours)
    Q_PROPERTY(double amp_hours_charged MEMBER amp_hours_charged)
    Q_PROPERTY(double watt_hours MEMBER watt_hours)
    Q_PROPERTY(double watt_hours_charged MEMBER watt_hours_charged)
    Q_PROPERTY(int tachometer MEMBER tachometer)
    Q_PROPERTY(int tachometer_abs MEMBER tachometer_abs)
    Q_PROPERTY(double position MEMBER position)
    Q_PROPERTY(mc_fault_code fault_code MEMBER fault_code)
    Q_PROPERTY(QString fault_str MEMBER fault_str)

public:
    double v_in;
    double temp_mos;
    double temp_motor;
    double current_motor;
    double current_in;
    double id;
    double iq;
    double rpm;
    double duty_now;
    double amp_hours;
    double amp_hours_charged;
    double watt_hours;
    double watt_hours_charged;
    int tachometer;
    int tachometer_abs;
    double position;
    mc_fault_code fault_code;
    QString fault_str;
};

Q_DECLARE_METATYPE(MC_VALUES)

struct BMS_VALUES {
    Q_GADGET

    Q_PROPERTY(double packVoltage MEMBER packVoltage)
    Q_PROPERTY(double packCurrent MEMBER packCurrent)
    Q_PROPERTY(int soC MEMBER soC)
    Q_PROPERTY(double cVHigh MEMBER cVHigh)
    Q_PROPERTY(double cVAverage MEMBER cVAverage)
    Q_PROPERTY(double cVLow MEMBER cVLow)
    Q_PROPERTY(double cVMisMatch MEMBER cVMisMatch)
    Q_PROPERTY(double loadLCVoltage MEMBER loadLCVoltage)
    Q_PROPERTY(double loadLCCurrent MEMBER loadLCCurrent)
    Q_PROPERTY(double loadHCVoltage MEMBER loadHCVoltage)
    Q_PROPERTY(double loadHCCurrent MEMBER loadHCCurrent)
    Q_PROPERTY(double auxVoltage MEMBER auxVoltage)
    Q_PROPERTY(double auxCurrent MEMBER auxCurrent)
    Q_PROPERTY(double tempBattHigh MEMBER tempBattHigh)
    Q_PROPERTY(double tempBattAverage MEMBER tempBattAverage)
    Q_PROPERTY(double tempBMSHigh MEMBER tempBMSHigh)
    Q_PROPERTY(double tempBMSAverage MEMBER tempBMSAverage)
    Q_PROPERTY(QString opState MEMBER opState)
    Q_PROPERTY(int balanceActive MEMBER balanceActive)
    Q_PROPERTY(QString faultState MEMBER faultState)

public:
    double packVoltage;
    double packCurrent;
    int    soC;
    double cVHigh;
    double cVAverage;
    double cVLow;
    double cVMisMatch;
    double loadLCVoltage;
    double loadLCCurrent;
    double loadHCVoltage;
    double loadHCCurrent;
    double auxVoltage;
    double auxCurrent;
    double tempBattHigh;
    double tempBattAverage;
    double tempBMSHigh;
    double tempBMSAverage;
    QString opState;
    int    balanceActive;
    QString faultState;
};

Q_DECLARE_METATYPE(BMS_VALUES)

typedef enum {
    DEBUG_SAMPLING_OFF = 0,
    DEBUG_SAMPLING_NOW,
    DEBUG_SAMPLING_START,
    DEBUG_SAMPLING_TRIGGER_START,
    DEBUG_SAMPLING_TRIGGER_FAULT,
    DEBUG_SAMPLING_TRIGGER_START_NOSEND,
    DEBUG_SAMPLING_TRIGGER_FAULT_NOSEND,
    DEBUG_SAMPLING_SEND_LAST_SAMPLES
} debug_sampling_mode;

typedef enum {
    COMM_FW_VERSION = 0,
    COMM_JUMP_TO_BOOTLOADER,
    COMM_ERASE_NEW_APP,
    COMM_WRITE_NEW_APP_DATA,
    COMM_GET_VALUES,
    COMM_SET_DUTY,
    COMM_SET_CURRENT,
    COMM_SET_CURRENT_BRAKE,
    COMM_SET_RPM,
    COMM_SET_POS,
    COMM_SET_HANDBRAKE,
    COMM_SET_DETECT,
    COMM_SET_SERVO_POS,
    COMM_SET_MCCONF,
    COMM_GET_MCCONF,
    COMM_GET_MCCONF_DEFAULT,
    COMM_SET_APPCONF,
    COMM_GET_APPCONF,
    COMM_GET_APPCONF_DEFAULT,
    COMM_SAMPLE_PRINT,
    COMM_TERMINAL_CMD,
    COMM_PRINT,
    COMM_ROTOR_POSITION,
    COMM_EXPERIMENT_SAMPLE,
    COMM_DETECT_MOTOR_PARAM,
    COMM_DETECT_MOTOR_R_L,
    COMM_DETECT_MOTOR_FLUX_LINKAGE,
    COMM_DETECT_ENCODER,
    COMM_DETECT_HALL_FOC,
    COMM_REBOOT,
    COMM_ALIVE,
    COMM_GET_DECODED_PPM,
    COMM_GET_DECODED_ADC,
    COMM_GET_DECODED_CHUK,
    COMM_FORWARD_CAN,
    COMM_SET_CHUCK_DATA,
    COMM_CUSTOM_APP_DATA,
    COMM_NRF_START_PAIRING,
    COMM_STORE_BMS_CONF = 50,
    COMM_GET_BMS_CELLS
} COMM_PACKET_ID;

typedef struct {
    int js_x;
    int js_y;
    int acc_x;
    int acc_y;
    int acc_z;
    bool bt_c;
    bool bt_z;
} chuck_data;

struct bldc_detect {
    Q_GADGET

    Q_PROPERTY(double cycle_int_limit MEMBER cycle_int_limit)
    Q_PROPERTY(double bemf_coupling_k MEMBER bemf_coupling_k)
    Q_PROPERTY(QVector<int> hall_table MEMBER hall_table)
    Q_PROPERTY(int hall_res MEMBER hall_res)

public:
    double cycle_int_limit;
    double bemf_coupling_k;
    QVector<int> hall_table;
    int hall_res;
};

Q_DECLARE_METATYPE(bldc_detect)

typedef enum {
    NRF_PAIR_STARTED = 0,
    NRF_PAIR_OK,
    NRF_PAIR_FAIL
} NRF_PAIR_RES;

typedef enum {
    OP_STATE_INIT = 0,									// 0
    OP_STATE_CHARGING,									// 1
    OP_STATE_PRE_CHARGE,								// 2
    OP_STATE_LOAD_ENABLED,								// 3
    OP_STATE_BATTERY_DEAD,								// 4
    OP_STATE_POWER_DOWN,								// 5
    OP_STATE_EXTERNAL,									// 6
    OP_STATE_ERROR,										// 7
    OP_STATE_ERROR_PRECHARGE,						    // 8
    OP_STATE_BALANCING,									// 9
    OP_STATE_CHARGED,									// 10
    OP_STATE_FORCEON,									// 11
} OperationalStateTypedef;

#endif // DATATYPES_H
