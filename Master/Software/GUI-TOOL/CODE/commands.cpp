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

#include "commands.h"
#include <QDebug>
#include <QSettings>

Commands::Commands(QObject *parent) : QObject(parent)
{
    mSendCan = QSettings().value("send_can", false).toBool();
    mCanId = QSettings().value("can_id", 0).toInt();
    mIsLimitedMode = false;

    // Firmware state
    mFirmwareIsUploading = false;
    mFirmwareState = 0;
    mFimwarePtr = 0;
    mFirmwareTimer = 0;
    mFirmwareRetries = 0;
    mFirmwareUploadStatus = "FW Upload Status";
    mCheckNextbmsConfig = false;

    mTimer = new QTimer(this);
    mTimer->setInterval(10);
    mTimer->start();

    mbmsConfig = 0;

    mTimeoutCount = 50;
    mTimeoutFwVer = 0;
    mTimeoutBMSconf = 0;
    mTimeoutValues = 0;
    mTimeoutCells = 0;

    connect(mTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));
}

void Commands::setLimitedMode(bool is_limited)
{
    mIsLimitedMode = is_limited;
}

bool Commands::isLimitedMode()
{
    return mIsLimitedMode;
}

void Commands::setSendCan(bool sendCan, int id)
{
    mSendCan = sendCan;
    QSettings().setValue("send_can", sendCan);

    if (id >= 0) {
        setCanSendId(id);
    }
}

bool Commands::getSendCan()
{
    return mSendCan;
}

void Commands::setCanSendId(unsigned int id)
{
    mCanId = id;
    QSettings().setValue("can_id", id);
}

int Commands::getCanSendId()
{
    return mCanId;
}

void Commands::processPacket(QByteArray data)
{
    VByteArray vb(data);
    COMM_PACKET_ID id = (COMM_PACKET_ID)vb.vbPopFrontUint8();

    switch (id) {
    case COMM_FW_VERSION: {
        mTimeoutFwVer = 0;
        int fw_major;
        int fw_minor;
        QString hw;
        QByteArray uuid;

        if (vb.size() >= 2) {
            fw_major = vb.vbPopFrontInt8();
            fw_minor = vb.vbPopFrontInt8();
            hw = vb.vbPopFrontString();
            if (vb.size() >= 12) {
                uuid.append(vb.left(12));
                vb.remove(0, 12);
            }
        } else {
            fw_major = -1;
            fw_minor = -1;
        }

        emit fwVersionReceived(fw_major, fw_minor, hw, uuid);
    } break;

    case COMM_ERASE_NEW_APP:
    case COMM_WRITE_NEW_APP_DATA:
        firmwareUploadUpdate(!vb.at(0));
        break;

    case COMM_GET_VALUES: {
        mTimeoutValues = 0;
        BMS_VALUES values;

        values.packVoltage      = vb.vbPopFrontDouble32(1e3);
        values.packCurrent      = vb.vbPopFrontDouble32(1e3);

        values.soC              = vb.vbPopFrontUint8();

        values.cVHigh           = vb.vbPopFrontDouble32(1e3);
        values.cVAverage        = vb.vbPopFrontDouble32(1e3);
        values.cVLow            = vb.vbPopFrontDouble32(1e3);
        values.cVMisMatch       = vb.vbPopFrontDouble32(1e3);

        values.loadLCVoltage    = vb.vbPopFrontDouble16(1e2);
        values.loadLCCurrent    = vb.vbPopFrontDouble16(1e2);
        values.loadHCVoltage    = vb.vbPopFrontDouble16(1e2);
        values.loadHCCurrent    = vb.vbPopFrontDouble16(1e2);
        values.auxVoltage       = vb.vbPopFrontDouble16(1e2);
        values.auxCurrent       = vb.vbPopFrontDouble16(1e2);

        values.tempBattHigh     = vb.vbPopFrontDouble16(1e1);
        values.tempBattAverage  = vb.vbPopFrontDouble16(1e1);
        values.tempBMSHigh      = vb.vbPopFrontDouble16(1e1);
        values.tempBMSAverage   = vb.vbPopFrontDouble16(1e1);

        values.opState          = opStateToStr((OperationalStateTypedef)vb.vbPopFrontUint8());
        values.balanceActive    = vb.vbPopFrontUint8();

        emit valuesReceived(values);
    } break;

    case COMM_GET_BMS_CELLS:{
        mTimeoutCells = 0;
        int mCellAmount;
        QVector<double> mCellVoltages;
        mCellVoltages.clear();

        mCellAmount = vb.vbPopFrontUint8();

        for(int cellValuePointer = 0; cellValuePointer < mCellAmount; cellValuePointer++){
            mCellVoltages.append(vb.vbPopFrontDouble16(1e3));
        }

        emit cellsReceived(mCellAmount,mCellVoltages);

       } break;

    case COMM_PRINT:
        emit printReceived(QString::fromLatin1(vb));
        break;

    case COMM_ROTOR_POSITION:
        emit rotorPosReceived(vb.vbPopFrontDouble32(1e5));
        break;

    case COMM_GET_MCCONF:
    case COMM_GET_MCCONF_DEFAULT:
        mTimeoutBMSconf = 0;
        if (mbmsConfig) {
            mbmsConfig->deSerialize(vb);
            mbmsConfig->updateDone();

            if (mCheckNextbmsConfig) {
                mCheckNextbmsConfig = false;
                emit bmsConfigCheckResult(mbmsConfig->checkDifference(&mbmsConfigLast));
            }
        }
        break;

    case COMM_SET_MCCONF:
        emit ackReceived("BMS Write OK");
        break;

    case COMM_STORE_BMS_CONF:
        if (mbmsConfig) {
            mbmsConfig->storingDone();
        }
        break;

    default:
        break;
    }
}

void Commands::getFwVersion()
{
    if (mTimeoutFwVer > 0) {
        return;
    }

    mTimeoutFwVer = mTimeoutCount;

    VByteArray vb;
    vb.vbAppendInt8(COMM_FW_VERSION);
    emitData(vb);
}

void Commands::getValues()
{
    if (mTimeoutValues > 0) {
        return;
    }

    mTimeoutValues = mTimeoutCount;

    VByteArray vb;
    vb.vbAppendInt8(COMM_GET_VALUES);
    emitData(vb);
}

void Commands::getCells()
{
    if (mTimeoutCells > 0) {
        return;
    }

    mTimeoutCells = mTimeoutCount;

    VByteArray vb;
    vb.vbAppendInt8(COMM_GET_BMS_CELLS);
    emitData(vb);
}

void Commands::sendTerminalCmd(QString cmd)
{
    VByteArray vb;
    vb.vbAppendInt8(COMM_TERMINAL_CMD);
    vb.append(cmd.toLatin1());
    emitData(vb);
}

void Commands::setDetect(disp_pos_mode mode)
{
    VByteArray vb;
    vb.vbAppendInt8(COMM_SET_DETECT);
    vb.vbAppendInt8(mode);
    emitData(vb);
}

void Commands::samplePrint(debug_sampling_mode mode, int sample_len, int dec)
{
    VByteArray vb;
    vb.vbAppendInt8(COMM_SAMPLE_PRINT);
    vb.vbAppendInt8(mode);
    vb.vbAppendUint16(sample_len);
    vb.vbAppendUint8(dec);
    emitData(vb);
}

void Commands::getBMSconf()
{
    if (mTimeoutBMSconf > 0) {
        return;
    }

    mTimeoutBMSconf = mTimeoutCount;


    mCheckNextbmsConfig = false;
    VByteArray vb;
    vb.vbAppendInt8(COMM_GET_MCCONF);
    emitData(vb);
}

void Commands::getBMSconfDefault()
{
    if (mTimeoutBMSconf > 0) {
        return;
    }

    mTimeoutBMSconf = mTimeoutCount;

    mCheckNextbmsConfig = false;
    VByteArray vb;
    vb.vbAppendInt8(COMM_GET_MCCONF_DEFAULT);
    emitData(vb);
}

void Commands::setBMSconf(bool check)
{
    if (mbmsConfig) {
        mbmsConfigLast = *mbmsConfig;
        VByteArray vb;
        vb.vbAppendInt8(COMM_SET_MCCONF);
        mbmsConfig->serialize(vb);
        emitData(vb);

        if (check) {
            checkbmsConfig();
        }
    }
}

void Commands::reboot()
{
    VByteArray vb;
    vb.vbAppendInt8(COMM_REBOOT);
    emitData(vb);
}

void Commands::sendAlive()
{
    VByteArray vb;
    vb.vbAppendInt8(COMM_ALIVE);
    emitData(vb);
}

void Commands::pairNrf(int ms)
{
    VByteArray vb;
    vb.vbAppendInt8(COMM_NRF_START_PAIRING);
    vb.vbAppendInt32(ms);
    emitData(vb);
}

void Commands::timerSlot()
{
    if (mFirmwareIsUploading) {
        if (mFirmwareTimer) {
            mFirmwareTimer--;
        } else {
            firmwareUploadUpdate(true);
        }
    }

    if (mTimeoutFwVer > 0) mTimeoutFwVer--;
    if (mTimeoutBMSconf > 0) mTimeoutBMSconf--;
    if (mTimeoutValues > 0) mTimeoutValues--;
    if (mTimeoutCells > 0) mTimeoutCells--;
}

void Commands::emitData(QByteArray data)
{
    // Only allow firmware commands in limited mode
    if (mIsLimitedMode && data.at(0) > COMM_WRITE_NEW_APP_DATA) {
        return;
    }

    if (mSendCan) {
        data.prepend((char)mCanId);
        data.prepend((char)COMM_FORWARD_CAN);
    }

    emit dataToSend(data);
}

void Commands::firmwareUploadUpdate(bool isTimeout)
{
    if (!mFirmwareIsUploading) {
        return;
    }

    const int app_packet_size = 200;
    const int retries = 5;
    const int timeout = 350;

    if (mFirmwareState == 0) {
        mFirmwareUploadStatus = "Buffer Erase";
        if (isTimeout) {
            // Erase timed out, abort.
            mFirmwareIsUploading = false;
            mFimwarePtr = 0;
            mFirmwareUploadStatus = "Buffer Erase Timeout";
        } else {
            mFirmwareState++;
            mFirmwareRetries = retries;
            mFirmwareTimer = timeout;
            firmwareUploadUpdate(true);
        }
    } else if (mFirmwareState == 1) {
        mFirmwareUploadStatus = "CRC/Size Write";
        if (isTimeout) {
            if (mFirmwareRetries > 0) {
                mFirmwareRetries--;
                mFirmwareTimer = timeout;
            } else {
                mFirmwareIsUploading = false;
                mFimwarePtr = 0;
                mFirmwareState = 0;
                mFirmwareUploadStatus = "CRC/Size Write Timeout";
                return;
            }

            quint16 crc = Packet::crc16((const unsigned char*)mNewFirmware.constData(), mNewFirmware.size());

            VByteArray vb;
            vb.append((char)COMM_WRITE_NEW_APP_DATA);
            vb.vbAppendUint32(0);
            vb.vbAppendUint32(mNewFirmware.size());
            vb.vbAppendUint16(crc);
            emitData(vb);
        } else {
            mFirmwareState++;
            mFirmwareRetries = retries;
            mFirmwareTimer = timeout;
            firmwareUploadUpdate(true);
        }
    } else if (mFirmwareState == 2) {
        mFirmwareUploadStatus = "FW Data Write";
        if (isTimeout) {
            if (mFirmwareRetries > 0) {
                mFirmwareRetries--;
                mFirmwareTimer = timeout;
            } else {
                mFirmwareIsUploading = false;
                mFimwarePtr = 0;
                mFirmwareState = 0;
                mFirmwareUploadStatus = "FW Data Write Timeout";
                return;
            }

            int fw_size_left = mNewFirmware.size() - mFimwarePtr;
            int send_size = fw_size_left > app_packet_size ? app_packet_size : fw_size_left;

            VByteArray vb;
            vb.append((char)COMM_WRITE_NEW_APP_DATA);

            if (mFirmwareIsBootloader) {
                vb.vbAppendUint32(mFimwarePtr + (1024 * 2 * 50));
            } else {
                vb.vbAppendUint32(mFimwarePtr + 6);
            }

            vb.append(mNewFirmware.mid(mFimwarePtr, send_size));
            emitData(vb);
        } else {
            mFirmwareRetries = retries;
            mFirmwareTimer = timeout;
            mFimwarePtr += app_packet_size;

            if (mFimwarePtr >= mNewFirmware.size()) {
                mFirmwareIsUploading = false;
                mFimwarePtr = 0;
                mFirmwareState = 0;
                mFirmwareUploadStatus = "FW Upload Done";

                // Upload done. Enter bootloader!
                if (!mFirmwareIsBootloader) {
                    QByteArray buffer;
                    buffer.append((char)COMM_JUMP_TO_BOOTLOADER);
                    emitData(buffer);
                }
            } else {
                firmwareUploadUpdate(true);
            }
        }
    }
}

QString Commands::opStateToStr(OperationalStateTypedef fault)
{
    switch (fault) {
    case OP_STATE_INIT: return "Init";
    case OP_STATE_CHARGING: return "Charging";
    case OP_STATE_PRE_CHARGE: return "PreCharging";
    case OP_STATE_LOAD_ENABLED: return "Load enabled";
    case OP_STATE_BATTERY_DEAD: return "Battery dead";
    case OP_STATE_POWER_DOWN: return "Power down";
    case OP_STATE_EXTERNAL: return "External";
    case OP_STATE_ERROR: return "Error";
    case OP_STATE_ERROR_PRECHARGE: return "ErrPreCharge";
    case OP_STATE_BALANCING: return "Balancing";
    case OP_STATE_CHARGED: return "Charged";
    case OP_STATE_FORCEON: return "ForceOn";
    default: return "Unknown fault";
    }
}

void Commands::setbmsConfig(ConfigParams *bmsConfig)
{
    mbmsConfig = bmsConfig;
    connect(mbmsConfig, SIGNAL(updateRequested()), this, SLOT(getBMSconf()));
    connect(mbmsConfig, SIGNAL(updateRequestDefault()), this, SLOT(getBMSconfDefault()));
}

void Commands::startFirmwareUpload(QByteArray &newFirmware, bool isBootloader)
{
    mFirmwareIsBootloader = isBootloader;
    mFirmwareIsUploading = true;
    mFirmwareState = mFirmwareIsBootloader ? 2 : 0;
    mFimwarePtr = 0;
    mFirmwareTimer = 500;
    mFirmwareRetries = 5;
    mNewFirmware.clear();
    mNewFirmware.append(newFirmware);
    mFirmwareUploadStatus = "Buffer Erase";

    if (mFirmwareIsBootloader) {
        firmwareUploadUpdate(true);
    } else {
        VByteArray vb;
        vb.vbAppendInt8(COMM_ERASE_NEW_APP);
        vb.vbAppendUint32(mNewFirmware.size());
        emitData(vb);
    }
}

double Commands::getFirmwareUploadProgress()
{
    if (mFirmwareIsUploading) {
        return (double)mFimwarePtr / (double)mNewFirmware.size();
    } else {
        return -1.0;
    }
}

QString Commands::getFirmwareUploadStatus()
{
    return mFirmwareUploadStatus;
}

void Commands::cancelFirmwareUpload()
{
    if (mFirmwareIsUploading) {
        mFirmwareIsUploading = false;
        mFimwarePtr = 0;
        mFirmwareState = 0;
        mFirmwareUploadStatus = "Cancelled";
    }
}

void Commands::checkbmsConfig()
{
    mCheckNextbmsConfig = true;
    VByteArray vb;
    vb.vbAppendInt8(COMM_GET_MCCONF);
    emitData(vb);
}

void Commands::storeBMSConfig()
{
    VByteArray vb;
    vb.vbAppendInt8(COMM_STORE_BMS_CONF);
    emitData(vb);
}
