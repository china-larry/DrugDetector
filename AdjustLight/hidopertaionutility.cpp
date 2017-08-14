#include "HidOpertaionUtility.h"

#include<QLibrary>
#include <QDebug>
#include <iostream>
#include <QTimer>
#include <QCoreApplication>
#include <QTime>

#include "ProtocolUtility.h"

Q_DECLARE_METATYPE(DevConfigParams)
#define ACK_TIME_OUT_SECOND 20
HIDOpertaionUtility* HIDOpertaionUtility::instance = NULL;

HIDOpertaionUtility *HIDOpertaionUtility::GetInstance()
{
    if(instance == NULL)
    {
        instance = new HIDOpertaionUtility();

    }
    return instance;
}


HIDOpertaionUtility::HIDOpertaionUtility()
{
    mReadThread = NULL;
    mHidHandle = NULL;
    mIsDeviceOpened = false;
    connect(this, SIGNAL(SignalHIDWrite(QByteArray)), this, SLOT(SlotWrite(QByteArray)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(SignalHIDOpen()), this, SLOT(SlotOpen()), Qt::QueuedConnection);
    connect(this, SIGNAL(SignalHIDClose()), this, SLOT(SlotClose()), Qt::DirectConnection/*Qt::QueuedConnection*/);
    connect(this, SIGNAL(SignalReadDevParams()), this, SLOT(SlotReadDevParams()), Qt::QueuedConnection);
    connect(this, SIGNAL(SignalWriteDevParams(DevConfigParams)), this,
            SLOT(SlotWriteDevParams(DevConfigParams)), Qt::QueuedConnection);
    connect(this, SIGNAL(SignalHIDUpgradeSubControl(QString)),this, SLOT(SlotUpgradeSubControl(QString)));
    moveToThread(&mWorkThread);
    connect(&mWorkThread, SIGNAL(started()), this, SLOT(SlotLoadDll()));
    connect(&mWorkThread, SIGNAL(finished()), this, SLOT(SlotUnloadDll()));
    connect(&mWorkThread, SIGNAL(finished()), this, SLOT(deleteLater()));
    mWorkThread.start();
    mDevVersion = "";
    mDevConfigParamsByte = new quint8[sizeof(DevConfigParams)];
}

HIDOpertaionUtility::~HIDOpertaionUtility()
{
    delete mDevConfigParamsByte;
}

void HIDOpertaionUtility::SlotLoadDll()
{
    //导入dll,并获取dll包含的操作函数指针
    mReadThread = new HIDReadThread();
    connect(mReadThread, SIGNAL(finished()), mReadThread, SLOT(deleteLater()));
    mWorkHandle = QThread::currentThreadId();
}

void HIDOpertaionUtility::SlotUnloadDll()
{

}


void HIDOpertaionUtility::HIDWrite(QByteArray writeByteArray)
{
    //如果在HID后台操作线程则直接调用,否则需通过消息转发,以便把相应操作转到HID后台操作线程
    if(QThread::currentThreadId() == mWorkHandle)
    {
        SlotWrite(writeByteArray);
    }
    else
        emit SignalHIDWrite(writeByteArray);
}

void HIDOpertaionUtility::HIDOpen()
{
    //如果在HID后台操作线程则直接调用,否则需通过消息转发,以便把相应操作转到HID后台操作线程
    if(QThread::currentThreadId() == mWorkHandle)
    {
        SlotOpen();
    }
    else
    {
        emit SignalHIDOpen();
    }
}

void HIDOpertaionUtility::HIDClose()
{
    SetWaitForAck(false);
    SetWaitForReturn(false);
    //如果在HID后台操作线程则直接调用,否则需通过消息转发,以便把相应操作转到HID后台操作线程
    if(QThread::currentThreadId() == mWorkHandle)
    {
        qDebug() << "_________" <<__FUNCTION__;
        SlotClose();
    }
    else
    {
        emit SignalHIDClose();
    }

}

bool HIDOpertaionUtility::HIDRead(quint8 *recvDataBuf, int delaytime)
{
    bool result = false;
    if(mHidHandle)
    {
        result = ReadHidData(mHidHandle, recvDataBuf, delaytime);
    }
    return result;
}

bool HIDOpertaionUtility::IsDeviceOpen()
{
    return mIsDeviceOpened;
}

bool HIDOpertaionUtility::SlotOpen()
{
    //打开设备,开启设备读线程
    bool isOpen = false;
    if(mHidHandle>0)
    {
        //如已打开需关闭再重开
        SlotClose();
    }
    //if(mOpenFunc)
    {
        mHidHandle = OpenMyHIDDevice(USB_PID, USB_VID);
        if(mHidHandle>0)
        {
            mIsDeviceOpened = true;
            if(!mReadThread)
                mReadThread = new HIDReadThread();
            mReadThread->start();
            isOpen = true;
        }
    }
    if(isOpen == false)
    {
        SignalErrInfo(EnumTypeErr::ErrNoConnectUSB);
    }
    return isOpen;
}

bool HIDOpertaionUtility::SlotClose()
{
    //关闭所有灯与电机立刻停止
    qDebug() << "_________" <<__FUNCTION__;
    qDebug() <<"_______" << __LINE__;
    ExecuteCmdWithAck(ProtocolUtility::GetCloseAllLEDAndStopMotorCmd());
    qDebug() <<"_______" << __LINE__;
    bool result = false;
    if(mHidHandle > 0)
    {
        qDebug() <<"_______" << __LINE__;
        qDebug() << "_________" <<__FUNCTION__;
        CloseDev(mHidHandle);
    }
    qDebug() <<"_______" << __LINE__;
    mIsDeviceOpened = false;//用于关闭读取线程
    mHidHandle = NULL;
    return result;
}

bool HIDOpertaionUtility::SendCmdToDev(QByteArray writeByteArray)
{
    if(mHidHandle)
    {
        //发送字节，最大发送64个
        //目前发现发送到设备的命令必须是64个字节，少于64个字节发送命令执行失败，待确认原因
        quint8 writeByte[CMD_LEN]={0};
        for(int i= 0;i<writeByteArray.length();i++)
        {
            writeByte[i] = (quint8)writeByteArray.at(i);
        }
        int writeResult = WriteHidData(mHidHandle, (quint8*)&writeByte, CMD_LEN);
        if (writeResult > 0)
            return true;
        else
            return false;
    }
    else
        return false;
}

bool HIDOpertaionUtility::SlotWrite(QByteArray qWriteByteArray)
{
    bool result = false;
    m_iCmdType = 1;
    if(mHidHandle>0)
    {
        m_iCmdType = ((qWriteByteArray.at(5)<<8) && 0xFF00)|(qWriteByteArray.at(6)&0x00FF);
        switch (m_iCmdType) {
        case ProtocolUtility::CMD_ROTATE_MOTOR://电机转动命令
        {
            result = RotateMotor(qWriteByteArray);
            break;
        }
        case ProtocolUtility::CMD_RESET_MOTOR://电机复位指令
        {
            result = ExecuteCmdWithAckAndReturn(qWriteByteArray,120);
            break;
        }
        case ProtocolUtility::CMD_OPEN_OR_CLOSE_LED://LED灯开关命令
        case ProtocolUtility::CMD_CLOSE_ALL_LED://关闭所有灯指令
        case ProtocolUtility::CMD_CLOSE_ALL_LED_AND_STOP_MOTOR://关闭所有灯与电机立刻停止
        {
            result = ExecuteCmdWithAck(qWriteByteArray);
            break;
        }
        case ProtocolUtility::CMD_READ_DEV_VERSION://读取仪器下位机软件版本
        {
            result = ExecuteCmdWithReturn(qWriteByteArray,100);
            break;
        }
        case ProtocolUtility::CMD_READ_TEST_COUNT:
        case ProtocolUtility::CMD_ADD_TEST_COUNT:
        case ProtocolUtility::CMD_CLEAR_TEST_COUNT:
        {
            result = ExecuteCmdWithReturn(qWriteByteArray,10);
            break;
        }
        default:
            break;
        }
    }
    emit SignalOperationComplete(m_iCmdType,result);
    SetDeviceOperate(false);
    return result;
}


void HIDOpertaionUtility::ReceiveNewCmdFromDev(QByteArray data)
{
    //毒检设备所有消息统一在这里处理，消息有两种类型ACK和结果，
    //因为有命令需要等待ACK和结果，因而这里使用类的成员变量mIsWaitForAck
    // /mIsWaitForReturn/mAckResult/mReturnResult进行状态同步
    if(mIsWaitForAck)
    {
        if((quint8)data.at(1)==quint8(m_iCmdType))
        {
            SetAckResult(true);
        }
        SetWaitForAck(false);

    }
    else if(mIsWaitForReturn)
    {
        switch (quint8(m_iCmdType)) {
        case ProtocolUtility::CMD_ROTATE_MOTOR:
        case ProtocolUtility::CMD_RESET_MOTOR:
        case ProtocolUtility::CMD_CLOSE_ALL_LED:
        case ProtocolUtility::CMD_CLOSE_ALL_LED_AND_STOP_MOTOR:
        case ProtocolUtility::CMD_CLEAR_TEST_COUNT:
        case ProtocolUtility::CMD_UPGRADE_APP_END:
        {
            if(data.at(5) == (m_iCmdType/256) && (data.at(6) == quint8(m_iCmdType))
                    && data.at(1) == 0x01 && data.at(2) == 0x01)
            {
                SetReturnResult(true);
            }
            break;
        }
        case ProtocolUtility::CMD_READ_PARAM_FROM_DEV:
        {
            if(data.at(5) == (m_iCmdType/256) && (data.at(6) == quint8(m_iCmdType))
                    && data.at(1) == 0x01 && data.at(2) == 0x01)
            {
                SetReturnResult(true);
                quint8 cmdLen = data.at(3);
                cmdLen = cmdLen * 256 + data.at(4);
                //if (ReadCRC8(data, cmdLen - 1) != GetReadbyte[cmdLen]) return null;
                quint8 pkgNum = data.at(DATA_START_INDEX-1);//
                cmdLen -= DATA_START_INDEX;

                int index = (pkgNum-1)*PARAM_PACKAGE_DATA_LEN;
                for (int i = 0; i < cmdLen; i++)
                {
                    mDevConfigParamsByte[index] = data.at(DATA_START_INDEX + i);
                    index++;
                }
                if(pkgNum == PARAM_PACKAGE_SIZE)
                {
                    memcpy(&mParams, mDevConfigParamsByte,sizeof(DevConfigParams));
                    emit SignalReceiveDevParams(mParams);
                }
            }
            break;
        }
        case ProtocolUtility::CMD_READ_DEV_VERSION:
        {
            if(data.at(5) == (m_iCmdType/256) && (data.at(6) == quint8(m_iCmdType))
                    && data.at(1) == 0x01 && data.at(2) == 0x01)
            {
                SetReturnResult(true);
                quint8 cmdLen = data.at(3);
                cmdLen = cmdLen * 256 + data.at(4);
                //if (ReadCRC8(data, cmdLen - 1) != GetReadbyte[cmdLen]) return null;
                cmdLen -= DATA_START_INDEX;
                mDevVersion.clear();
                for (int i = 0; i < cmdLen; i++)
                {
                    mDevVersion.push_back(data.at(DATA_START_INDEX + i));
                }
                qDebug()<<"Version:"<<mDevVersion;
                emit SignalReceiveDevVersion(mDevVersion);
            }
            break;
        }
        case ProtocolUtility::CMD_ADD_TEST_COUNT:
        case ProtocolUtility::CMD_READ_TEST_COUNT:
        {
            if(data.at(5) == (m_iCmdType/256) && (data.at(6) == quint8(m_iCmdType))
                    && data.at(1) == 0x01 && data.at(2) == 0x01)
            {
                SetReturnResult(true);
                quint8 cmdLen = data.at(3);
                cmdLen = cmdLen * 256 + data.at(4);
                //if (ReadCRC8(data, cmdLen - 1) != GetReadbyte[cmdLen]) return null;
                cmdLen -= DATA_START_INDEX;
                if(cmdLen != 4)
                {
                    qDebug()<<"读取/添加测试次数命令返回错误";
                }
                else
                {
                    mDevVersion.clear();
                    quint8 testCountArray[4];
                    for (int i = 0; i < 4; i++)
                    {
                        testCountArray[i] = data.at(DATA_START_INDEX + i);
                    }
                    m_qTestCount=((testCountArray[0]<<8) && 0xFF000000)
                            |((testCountArray[1]<<8) && 0x00FF0000)
                            |((testCountArray[2]<<8) && 0x0000FF00)
                            |(testCountArray[3]&0x000000FF);
                    qDebug()<<"Test Count:"<<m_qTestCount;
                    emit SignalReceiveTestCount(m_qTestCount);
                }
            }
            break;
        }
        case ProtocolUtility::CMD_UPGRADE_APP_START:
        {
            if((data.at(5) == (m_iCmdType/256))
                    && (data.at(6) == quint8(m_iCmdType))
                    && (data.at(1) == 0x01)
                    && (data.at(2) == 0x01)
                    && (data.at(DATA_START_INDEX) == 0x5A)
                    && (data.at(DATA_START_INDEX+1) == 0x5A ))
            {
                SetReturnResult(true);
            }
            break;
        }
        case ProtocolUtility::CMD_UPGRADE_APP_DATA:
        {
            if(data.at(5) == (m_iCmdType/256) && (data.at(6) == quint8(m_iCmdType))
                    && (data.at(1) == 0x01) && (data.at(2)) == 0x01
                    && (data.at(DATA_START_INDEX) == 0xAB) && (data.at(DATA_START_INDEX+1) == 0xAB) )
            {
                SetReturnResult(true);
            }
            break;
        }
        default:
            break;
        }
        SetWaitForReturn(false);
    }
}

QString HIDOpertaionUtility::GetVersion()
{
    return mDevVersion;
}

void HIDOpertaionUtility::HIDReadDevParams()
{
    //如果在HID后台操作线程则直接调用,否则需通过消息转发,以便把相应操作转到HID后台操作线程
    if(QThread::currentThreadId() == mWorkHandle)
    {
        SlotReadDevParams();
    }
    else
        emit SignalReadDevParams();
}

void HIDOpertaionUtility::HIDWriteDevParams(DevConfigParams devConfigParams)
{
    //如果在HID后台操作线程则直接调用,否则需通过消息转发,以便把相应操作转到HID后台操作线程
    if(QThread::currentThreadId() == mWorkHandle)
    {
        SlotWriteDevParams(devConfigParams);
    }
    else
        emit SignalWriteDevParams(devConfigParams);

}

void HIDOpertaionUtility::SetWaitForAck(bool isWaitForAck)
{
    mAckMutex.lock();
    mIsWaitForAck = isWaitForAck;
    mAckMutex.unlock();
}

void HIDOpertaionUtility::SetAckResult(bool ackResult)
{
    mAckMutex.lock();
    mAckResult = ackResult;
    mAckMutex.unlock();
}

void HIDOpertaionUtility::SetWaitForReturn(bool isWaitForReturn)
{
    mResultMutex.lock();
    mIsWaitForReturn = isWaitForReturn;
    mResultMutex.unlock();
}

void HIDOpertaionUtility::SetReturnResult(bool returnResult)
{
    mResultMutex.lock();
    mReturnResult = returnResult;
    mResultMutex.unlock();
}


bool HIDOpertaionUtility::GetCmdACK()
{
    SetAckResult(false);
    SetWaitForAck(true);
    QTime oldTime=QTime::currentTime();
    while(mIsWaitForAck)
    {
        int seconds=oldTime.secsTo(QTime::currentTime());
        if(seconds>ACK_TIME_OUT_SECOND)
        {
            break;
        }
    }
    qDebug()<<"ACK result:"<<mAckResult;
    return mAckResult;
}

bool HIDOpertaionUtility::GetCmdReturn(int delaySeconds)
{
    SetReturnResult(false);
    SetWaitForReturn(true);
    QTime oldTime=QTime::currentTime();
    while(mIsWaitForReturn)
    {
        int seconds=oldTime.secsTo(QTime::currentTime());
        if(seconds>delaySeconds)
        {
            break;
        }
    }
    return mReturnResult;
}

bool HIDOpertaionUtility::ExecuteCmdWithAckAndReturn(QByteArray writeByteArray,
                                                     int delaySeconds)
{
    bool result = false;
    if(!SendCmdToDev(writeByteArray))
        return result;
    if(!GetCmdACK())
        return result;
    if(!GetCmdReturn(delaySeconds))
        return result;
    result = true;
    return result;
}

bool HIDOpertaionUtility::ExecuteCmdWithAck(QByteArray writeByteArray)
{
    bool result = false;
    if(!SendCmdToDev(writeByteArray))
        return result;
    if(!GetCmdACK())
        return result;
    result = true;
    return result;
}

bool HIDOpertaionUtility::ExecuteCmdWithReturn(QByteArray writeByteArray,
                                                     int delaySeconds)
{
    bool result = false;
    if(!SendCmdToDev(writeByteArray))
        return result;
    if(!GetCmdReturn(delaySeconds))
        return result;
    result = true;
    return result;
}


//bool HIDOpertaionUtility::OpenOrCloseLed(QByteArray writeByteArray)
//{
//    return ExecuteCmdWithAck(writeByteArray);
//}

bool HIDOpertaionUtility::RotateMotor(QByteArray writeByteArray)
{
    if(writeByteArray.length()<12)//非法命令
        return false;
    quint16 step = writeByteArray.at(DATA_START_INDEX)*256+writeByteArray.at(DATA_START_INDEX+1);
    quint16 speed = writeByteArray.at(DATA_START_INDEX+2)*256+writeByteArray.at(DATA_START_INDEX+3);

    int delaySeconds =  (speed * step * 0.015);
    return ExecuteCmdWithAckAndReturn(writeByteArray, delaySeconds);

}

//bool HIDOpertaionUtility::ResetMotor(QByteArray writeByteArray)
//{
//    return ExecuteCmdWithAckAndReturn(writeByteArray,120);
//}

//bool HIDOpertaionUtility::CloseAllLED(QByteArray writeByteArray)
//{
//    return ExecuteCmdWithAck(writeByteArray);
//}

//bool HIDOpertaionUtility::CloseAllLedAndStopMotor(QByteArray writeByteArray)
//{
//    return ExecuteCmdWithAck(writeByteArray);
//}

//bool HIDOpertaionUtility::ReadDevVersion(QByteArray writeByteArray)
//{
//    return ExecuteCmdWithReturn(writeByteArray,100);
//}

//bool HIDOpertaionUtility::_ReadTestCount(QByteArray qWriteByteArray)
//{
//    return ExecuteCmdWithReturn(qWriteByteArray,10);
//}

//bool HIDOpertaionUtility::_AddTestCount(QByteArray qWriteByteArray)
//{
//    return ExecuteCmdWithReturn(qWriteByteArray,10);
//}

//bool HIDOpertaionUtility::_ClearTestCount(QByteArray qWriteByteArray)
//{
//    return ExecuteCmdWithReturn(qWriteByteArray,10);
//}

bool HIDOpertaionUtility::SlotReadDevParams()
{
    bool result = false;
    memset(mDevConfigParamsByte, 0,sizeof(DevConfigParams));//清空设备配置参数临时数组内容
    m_iCmdType = ProtocolUtility::CMD_READ_PARAM_FROM_DEV;
    QVector<QByteArray> readDevParamsVect = ProtocolUtility::GetReadParamsFromDevCmd();
    for(quint8 pkgNum = 1;pkgNum<=readDevParamsVect.size();pkgNum++)
    {
        if(!ExecuteCmdWithReturn(readDevParamsVect[pkgNum-1],3))
        {
            emit SignalOperationComplete(ProtocolUtility::CMD_READ_PARAM_FROM_DEV, result);
            return result;
        }
    }
    result = true;
    emit SignalOperationComplete(ProtocolUtility::CMD_READ_PARAM_FROM_DEV, result);
    return result;
}

bool HIDOpertaionUtility::SlotWriteDevParams(DevConfigParams devConfigParams)
{
    bool result = false;
    m_iCmdType = ProtocolUtility::CMD_WRITE_PARAM_TO_DEV;
    QVector<QByteArray> writeDevParamsVect = ProtocolUtility::
            GetWriteParamFromDevCmd(devConfigParams);
    for(quint8 pkgNum = 1;pkgNum<=10;pkgNum++)
    {
        if(!ExecuteCmdWithAck(writeDevParamsVect[pkgNum-1]))
        {
            emit SignalOperationComplete(ProtocolUtility::CMD_WRITE_PARAM_TO_DEV, result);
            return result;
        }
    }
    result = true;
    emit SignalOperationComplete(ProtocolUtility::CMD_WRITE_PARAM_TO_DEV, result);
    return result;
}

void HIDOpertaionUtility::_EmitUpgradeErrorSignal(bool result)
{
    emit SignalOperationComplete(m_iCmdType, result);
    emit SignalUpgradeError(QString::fromLocal8Bit("升级失败"));
}

bool HIDOpertaionUtility::SlotUpgradeSubControl(QString qFilePathStr)
{
    bool result = false;
    //升级步骤1 升级开始命令发送
    m_iCmdType = ProtocolUtility::CMD_UPGRADE_APP_START;
    QByteArray qCmdByteArray = ProtocolUtility::GetUpgradeAppStartCmd();
    if(ExecuteCmdWithReturn(qCmdByteArray, 50))
    {
        emit SignalUpgradeValue(5);//升级开始和升级结束命令各占5%的进度
        //升级步骤2 升级文件数据传输
        m_iCmdType = ProtocolUtility::CMD_UPGRADE_APP_DATA;
        QVector<QByteArray> upgradeDataCmdVect = ProtocolUtility::GetUpgradeAppCmd(qFilePathStr);
        for(quint8 pkgNum = 1;pkgNum<=upgradeDataCmdVect.size();pkgNum++)
        {
            if(!ExecuteCmdWithReturn(upgradeDataCmdVect[pkgNum-1], 50))
            {
                _EmitUpgradeErrorSignal(result);
                return result;
            }
            else
            {
                emit SignalUpgradeValue(pkgNum*90/upgradeDataCmdVect.size());
            }
        }
        //升级步骤3 升级结束命令发送
        m_iCmdType = ProtocolUtility::CMD_UPGRADE_APP_END;
        qCmdByteArray = ProtocolUtility::GetUpgradeAppEndCmd();
        if(!ExecuteCmdWithReturn(qCmdByteArray, 50))
        {
            _EmitUpgradeErrorSignal(result);
            return result;
        }
        result = true;
        emit SignalOperationComplete(m_iCmdType, result);
        emit SignalUpgradeFinish();
        emit SignalUpgradeValue(100);//升级完成，进度100%
    }
    else
    {
         _EmitUpgradeErrorSignal(result);
    }
    return result;
}

void HIDOpertaionUtility::SetDeviceOperate(bool bIsOperate)
{
    DeviceOperateMutex.lock();
    qDebug() << "mbIsDeviceOperate = " << mbIsDeviceOperate;
    mbIsDeviceOperate = bIsOperate;
    DeviceOperateMutex.unlock();
}

bool HIDOpertaionUtility::GetDeviceOperateStates()
{
    //DeviceOperateMutex.lock();
    return mbIsDeviceOperate;
    //DeviceOperateMutex.unlock();
}

void HIDOpertaionUtility::HIDUpgradeSubControl(QString filePath)
{
    //如果在HID后台操作线程则直接调用,否则需通过消息转发,以便把相应操作转到HID后台操作线程
    if(QThread::currentThreadId() == mWorkHandle)
    {
        SlotUpgradeSubControl(filePath);
    }
    else
        emit SignalHIDUpgradeSubControl(filePath);
}

HIDReadThread::HIDReadThread(QObject *parent) :
    QThread(parent)
{
}

void HIDReadThread::run()
{

    quint8 readByte[CMD_LEN]={0xFF};

    //读线程必须是另外一个HIDOpertaionUtility操作线程打开仪器后才有存在意义
    while(HIDOpertaionUtility::GetInstance()->IsDeviceOpen())
    {

        //ReadHidDataFunc最后一个参数负1则读线程堵塞直到数据读取成功
        int readRetVal = HIDOpertaionUtility::GetInstance()->HIDRead((quint8*)(&readByte), -1);
        //qDebug() << "readRetVal = " << readRetVal;
        if(readRetVal>0)
        {
            //QMutexLocker locker(&mDataMutex);
            QByteArray data;
            for(int i=0;i<CMD_LEN;i++)
            {
                data.push_back(readByte[i]);
            }
            HIDOpertaionUtility::GetInstance()->ReceiveNewCmdFromDev(data);
            //                emit SignalReceiveNewCmd(data);
            //emit SignalReceiveNewCmd();
            //qDebug()<<"ReadByte:"<<data.toHex();
        }
        //else
            msleep(500);
    }
    qDebug() << __FUNCTION__;
}
