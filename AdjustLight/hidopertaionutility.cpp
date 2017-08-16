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
    m_ReadThread = NULL;
    m_HidHandle = NULL;
    m_IsDeviceOpened = false;
    connect(this, SIGNAL(SignalHIDWrite(QByteArray)), this, SLOT(_SlotWrite(QByteArray)),
            Qt::QueuedConnection);
    connect(this, SIGNAL(SignalHIDOpen()), this, SLOT(_SlotOpen()), Qt::QueuedConnection);
    connect(this, SIGNAL(SignalHIDClose()), this, SLOT(_SlotClose()), Qt::DirectConnection/*Qt::QueuedConnection*/);
    connect(this, SIGNAL(SignalReadDevParams()), this, SLOT(_SlotReadDevParams()), Qt::QueuedConnection);
    connect(this, SIGNAL(SignalWriteDevParams(DevConfigParams)), this,
            SLOT(_SlotWriteDevParams(DevConfigParams)), Qt::QueuedConnection);
    connect(this, SIGNAL(SignalHIDUpgradeSubControl(QString)),this, SLOT(_SlotUpgradeSubControl(QString)));
    moveToThread(&m_WorkThread);
    connect(&m_WorkThread, SIGNAL(started()), this, SLOT(_SlotLoadDll()));
    connect(&m_WorkThread, SIGNAL(finished()), this, SLOT(_SlotUnloadDll()));
    //connect(&mWorkThread, SIGNAL(finished()), this, SLOT(deleteLater()));
    m_WorkThread.start();
    m_DevVersion = "";
    m_DevConfigParamsByte = new quint8[sizeof(DevConfigParams)];
}

HIDOpertaionUtility::~HIDOpertaionUtility()
{
    delete m_DevConfigParamsByte;
}

void HIDOpertaionUtility::_SlotLoadDll()
{
    //导入dll,并获取dll包含的操作函数指针
    m_ReadThread = new HIDReadThread();
    //connect(mReadThread, SIGNAL(finished()), mReadThread, SLOT(deleteLater()));
    m_WorkHandle = QThread::currentThreadId();
}

void HIDOpertaionUtility::_SlotUnloadDll()
{

}


void HIDOpertaionUtility::HIDWrite(QByteArray writeByteArray)
{
    //如果在HID后台操作线程则直接调用,否则需通过消息转发,以便把相应操作转到HID后台操作线程
    if(QThread::currentThreadId() == m_WorkHandle)
    {
        _SlotWrite(writeByteArray);
    }
    else
        emit SignalHIDWrite(writeByteArray);
}

void HIDOpertaionUtility::HIDOpen()
{
    //如果在HID后台操作线程则直接调用,否则需通过消息转发,以便把相应操作转到HID后台操作线程
    if(QThread::currentThreadId() == m_WorkHandle)
    {
        _SlotOpen();
    }
    else
    {
        emit SignalHIDOpen();
    }
}

void HIDOpertaionUtility::HIDClose()
{
    _SetWaitForAck(false);
    _SetWaitForReturn(false);
    //如果在HID后台操作线程则直接调用,否则需通过消息转发,以便把相应操作转到HID后台操作线程
    if(QThread::currentThreadId() == m_WorkHandle)
    {
        _SlotClose();
    }
    else
    {
        emit SignalHIDClose();
    }

}

bool HIDOpertaionUtility::HIDRead(quint8 *recvDataBuf, int delaytime)
{
    bool result = false;
    if(m_HidHandle)
    {
        result = ReadHidData(m_HidHandle, recvDataBuf, delaytime);
    }
    return result;
}

bool HIDOpertaionUtility::IsDeviceOpen()
{
    return m_IsDeviceOpened;
}

bool HIDOpertaionUtility::_SlotOpen()
{
    //打开设备,开启设备读线程
    bool isOpen = false;
    if(m_HidHandle > 0)
    {
        //如已打开需关闭再重开
        _SlotClose();
    }
    //if(mOpenFunc)
    {
        m_HidHandle = OpenMyHIDDevice(USB_PID, USB_VID);
        if(m_HidHandle > 0)
        {
            m_IsDeviceOpened = true;
            if(!m_ReadThread)
            {
                m_ReadThread = new HIDReadThread();
            }

            m_ReadThread->start();
            isOpen = true;
        }
    }
    if(isOpen == false)
    {
        SignalErrInfo(EnumTypeErr::ErrNoConnectUSB);
    }
    return isOpen;
}

bool HIDOpertaionUtility::_SlotClose()
{
    //关闭所有灯与电机立刻停止
    _ExecuteCmdWithAck(ProtocolUtility::GetCloseAllLEDAndStopMotorCmd());
    bool result = false;
    if(m_HidHandle > 0)
    {
        CloseDev(m_HidHandle);
    }
    m_IsDeviceOpened = false;//用于关闭读取线程
    m_HidHandle = NULL;
    return result;
}

bool HIDOpertaionUtility::_SendCmdToDev(QByteArray writeByteArray)
{
    if(m_HidHandle)
    {
        //发送字节，最大发送64个
        //目前发现发送到设备的命令必须是64个字节，少于64个字节发送命令执行失败，待确认原因
        quint8 writeByte[CMD_LEN]={0};
        for(int i= 0;i<writeByteArray.length();i++)
        {
            writeByte[i] = (quint8)writeByteArray.at(i);
        }
        int writeResult = WriteHidData(m_HidHandle, (quint8*)&writeByte, CMD_LEN);
        if (writeResult > 0)
            return true;
        else
            return false;
    }
    else
        return false;
}

bool HIDOpertaionUtility::_SlotWrite(QByteArray qWriteByteArray)
{
    bool result = false;
    m_iCmdType = 1;
    if(m_HidHandle > 0)
    {
        m_iCmdType = ((qWriteByteArray.at(5)<<8) && 0xFF00)|(qWriteByteArray.at(6)&0x00FF);
        switch (m_iCmdType) {
        case ProtocolUtility::s_iCmdRotateMotor://电机转动命令
        {
            result = _RotateMotor(qWriteByteArray);
            break;
        }
        case ProtocolUtility::s_iCmdResetMotor://电机复位指令
        {
            result = _ExecuteCmdWithAckAndReturn(qWriteByteArray,120);
            break;
        }
        case ProtocolUtility::s_iCmdOpenOrCloseLed://LED灯开关命令
        case ProtocolUtility::s_iCmdCloseAllLed://关闭所有灯指令
        case ProtocolUtility::s_iCmdCloseAllLedAndStopMotor://关闭所有灯与电机立刻停止
        {
            result = _ExecuteCmdWithAck(qWriteByteArray);
            break;
        }
        case ProtocolUtility::s_iCmdReadDevVersion://读取仪器下位机软件版本
        {
            result = _ExecuteCmdWithReturn(qWriteByteArray,100);
            break;
        }
        case ProtocolUtility::s_iCmdReadTestCount:
        case ProtocolUtility::s_iCmdAddTestCount:
        case ProtocolUtility::s_iCmdClearTestCount:
        {
            result = _ExecuteCmdWithReturn(qWriteByteArray,10);
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
    if(m_IsWaitForAck)
    {
        if((quint8)data.at(1)==quint8(m_iCmdType))
        {
            _SetAckResult(true);
        }
        _SetWaitForAck(false);

    }
    else if(m_IsWaitForReturn)
    {
        switch (quint8(m_iCmdType)) {
        case ProtocolUtility::s_iCmdRotateMotor:
        case ProtocolUtility::s_iCmdResetMotor:
        case ProtocolUtility::s_iCmdCloseAllLed:
        case ProtocolUtility::s_iCmdCloseAllLedAndStopMotor:
        case ProtocolUtility::s_iCmdClearTestCount:
        case ProtocolUtility::s_iCmdUpgradeAppEnd:
        {
            if(data.at(5) == (m_iCmdType/256) && (data.at(6) == quint8(m_iCmdType))
                    && data.at(1) == 0x01 && data.at(2) == 0x01)
            {
                _SetReturnResult(true);
            }
            break;
        }
        case ProtocolUtility::s_iCmdReadParamFromDev:
        {
            if(data.at(5) == (m_iCmdType/256) && (data.at(6) == quint8(m_iCmdType))
                    && data.at(1) == 0x01 && data.at(2) == 0x01)
            {
                _SetReturnResult(true);
                quint8 cmdLen = data.at(3);
                cmdLen = cmdLen * 256 + data.at(4);
                //if (ReadCRC8(data, cmdLen - 1) != GetReadbyte[cmdLen]) return null;
                quint8 pkgNum = data.at(DATA_START_INDEX-1);//
                cmdLen -= DATA_START_INDEX;

                int index = (pkgNum-1)*PARAM_PACKAGE_DATA_LEN;
                for (int i = 0; i < cmdLen; i++)
                {
                    m_DevConfigParamsByte[index] = data.at(DATA_START_INDEX + i);
                    index++;
                }
                if(pkgNum == PARAM_PACKAGE_SIZE)
                {
                    memcpy(&m_Params, m_DevConfigParamsByte,sizeof(DevConfigParams));
                    emit SignalReceiveDevParams(m_Params);
                }
            }
            break;
        }
        case ProtocolUtility::s_iCmdReadDevVersion:
        {
            if(data.at(5) == (m_iCmdType/256) && (data.at(6) == quint8(m_iCmdType))
                    && data.at(1) == 0x01 && data.at(2) == 0x01)
            {
                _SetReturnResult(true);
                quint8 cmdLen = data.at(3);
                cmdLen = cmdLen * 256 + data.at(4);
                //if (ReadCRC8(data, cmdLen - 1) != GetReadbyte[cmdLen]) return null;
                cmdLen -= DATA_START_INDEX;
                m_DevVersion.clear();
                for (int i = 0; i < cmdLen; i++)
                {
                    m_DevVersion.push_back(data.at(DATA_START_INDEX + i));
                }
                qDebug()<<"Version:"<<m_DevVersion;
                emit SignalReceiveDevVersion(m_DevVersion);
            }
            break;
        }
        case ProtocolUtility::s_iCmdAddTestCount:
        case ProtocolUtility::s_iCmdReadTestCount:
        {
            if(data.at(5) == (m_iCmdType/256) && (data.at(6) == quint8(m_iCmdType))
                    && data.at(1) == 0x01 && data.at(2) == 0x01)
            {
                _SetReturnResult(true);
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
                    m_DevVersion.clear();
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
        case ProtocolUtility::s_iCmdUpgradeAppStart:
        {
            if((data.at(5) == (m_iCmdType/256))
                    && (data.at(6) == quint8(m_iCmdType))
                    && (data.at(1) == 0x01)
                    && (data.at(2) == 0x01)
                    && (data.at(DATA_START_INDEX) == 0x5A)
                    && (data.at(DATA_START_INDEX+1) == 0x5A ))
            {
                _SetReturnResult(true);
            }
            break;
        }
        case ProtocolUtility::s_iCmdUpgradeAppData:
        {
            if(data.at(5) == (m_iCmdType/256) && (data.at(6) == quint8(m_iCmdType))
                    && (data.at(1) == 0x01) && (data.at(2)) == 0x01
                    && (data.at(DATA_START_INDEX) == 0xAB) && (data.at(DATA_START_INDEX+1) == 0xAB) )
            {
                _SetReturnResult(true);
            }
            break;
        }
        default:
            break;
        }
        _SetWaitForReturn(false);
    }
}

QString HIDOpertaionUtility::GetVersion()
{
    return m_DevVersion;
}

void HIDOpertaionUtility::HIDReadDevParams()
{
    //如果在HID后台操作线程则直接调用,否则需通过消息转发,以便把相应操作转到HID后台操作线程
    if(QThread::currentThreadId() == m_WorkHandle)
    {
        _SlotReadDevParams();
    }
    else
        emit SignalReadDevParams();
}

void HIDOpertaionUtility::HIDWriteDevParams(DevConfigParams devConfigParams)
{
    //如果在HID后台操作线程则直接调用,否则需通过消息转发,以便把相应操作转到HID后台操作线程
    if(QThread::currentThreadId() == m_WorkHandle)
    {
        _SlotWriteDevParams(devConfigParams);
    }
    else
        emit SignalWriteDevParams(devConfigParams);

}

void HIDOpertaionUtility::_SetWaitForAck(bool isWaitForAck)
{
    m_AckMutex.lock();
    m_IsWaitForAck = isWaitForAck;
    m_AckMutex.unlock();
}

void HIDOpertaionUtility::_SetAckResult(bool ackResult)
{
    m_AckMutex.lock();
    m_AckResult = ackResult;
    m_AckMutex.unlock();
}

void HIDOpertaionUtility::_SetWaitForReturn(bool isWaitForReturn)
{
    m_ResultMutex.lock();
    m_IsWaitForReturn = isWaitForReturn;
    m_ResultMutex.unlock();
}

void HIDOpertaionUtility::_SetReturnResult(bool returnResult)
{
    m_ResultMutex.lock();
    m_ReturnResult = returnResult;
    m_ResultMutex.unlock();
}


bool HIDOpertaionUtility::_GetCmdACK()
{
    _SetAckResult(false);
    _SetWaitForAck(true);
    QTime oldTime=QTime::currentTime();
    while(m_IsWaitForAck)
    {
        int seconds=oldTime.secsTo(QTime::currentTime());
        if(seconds>ACK_TIME_OUT_SECOND)
        {
            break;
        }
    }
    qDebug()<<"ACK result:"<<m_AckResult;
    return m_AckResult;
}

bool HIDOpertaionUtility::_GetCmdReturn(int delaySeconds)
{
    _SetReturnResult(false);
    _SetWaitForReturn(true);
    QTime oldTime=QTime::currentTime();
    while(m_IsWaitForReturn)
    {
        int seconds=oldTime.secsTo(QTime::currentTime());
        if(seconds > delaySeconds)
        {
            break;
        }
    }
    return m_ReturnResult;
}

bool HIDOpertaionUtility::_ExecuteCmdWithAckAndReturn(QByteArray writeByteArray,
                                                     int delaySeconds)
{
    bool result = false;
    if(!_SendCmdToDev(writeByteArray))
        return result;
    if(!_GetCmdACK())
        return result;
    if(!_GetCmdReturn(delaySeconds))
        return result;
    result = true;
    return result;
}

bool HIDOpertaionUtility::_ExecuteCmdWithAck(QByteArray writeByteArray)
{
    bool result = false;
    if(!_SendCmdToDev(writeByteArray))
        return result;
    if(!_GetCmdACK())
        return result;
    result = true;
    return result;
}

bool HIDOpertaionUtility::_ExecuteCmdWithReturn(QByteArray writeByteArray,
                                                     int delaySeconds)
{
    bool result = false;
    if(!_SendCmdToDev(writeByteArray))
        return result;
    if(!_GetCmdReturn(delaySeconds))
        return result;
    result = true;
    return result;
}


//bool HIDOpertaionUtility::OpenOrCloseLed(QByteArray writeByteArray)
//{
//    return ExecuteCmdWithAck(writeByteArray);
//}

bool HIDOpertaionUtility::_RotateMotor(QByteArray writeByteArray)
{
    if(writeByteArray.length()<12)//非法命令
        return false;
    quint16 step = writeByteArray.at(DATA_START_INDEX)*256+writeByteArray.at(DATA_START_INDEX+1);
    quint16 speed = writeByteArray.at(DATA_START_INDEX+2)*256+writeByteArray.at(DATA_START_INDEX+3);

    int delaySeconds =  (speed * step * 0.015);
    return _ExecuteCmdWithAckAndReturn(writeByteArray, delaySeconds);

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

bool HIDOpertaionUtility::_SlotReadDevParams()
{
    bool result = false;
    memset(m_DevConfigParamsByte, 0,sizeof(DevConfigParams));//清空设备配置参数临时数组内容
    m_iCmdType = ProtocolUtility::s_iCmdReadParamFromDev;
    QVector<QByteArray> readDevParamsVect = ProtocolUtility::GetReadParamsFromDevCmd();
    for(quint8 pkgNum = 1;pkgNum<=readDevParamsVect.size();pkgNum++)
    {
        if(!_ExecuteCmdWithReturn(readDevParamsVect[pkgNum-1],3))
        {
            emit SignalOperationComplete(ProtocolUtility::s_iCmdReadParamFromDev, result);
            return result;
        }
    }
    result = true;
    emit SignalOperationComplete(ProtocolUtility::s_iCmdReadParamFromDev, result);
    return result;
}

bool HIDOpertaionUtility::_SlotWriteDevParams(DevConfigParams devConfigParams)
{
    bool result = false;
    m_iCmdType = ProtocolUtility::s_iCmdWriteParamToDev;
    QVector<QByteArray> writeDevParamsVect = ProtocolUtility::
            GetWriteParamFromDevCmd(devConfigParams);
    for(quint8 pkgNum = 1;pkgNum<=10;pkgNum++)
    {
        if(!_ExecuteCmdWithAck(writeDevParamsVect[pkgNum-1]))
        {
            emit SignalOperationComplete(ProtocolUtility::s_iCmdWriteParamToDev, result);
            return result;
        }
    }
    result = true;
    emit SignalOperationComplete(ProtocolUtility::s_iCmdWriteParamToDev, result);
    return result;
}

void HIDOpertaionUtility::_EmitUpgradeErrorSignal(bool result)
{
    emit SignalOperationComplete(m_iCmdType, result);
    emit SignalUpgradeError(QString::fromLocal8Bit("升级失败"));
}

bool HIDOpertaionUtility::_SlotUpgradeSubControl(QString qFilePathStr)
{
    bool result = false;
    //升级步骤1 升级开始命令发送
    m_iCmdType = ProtocolUtility::s_iCmdUpgradeAppStart;
    QByteArray qCmdByteArray = ProtocolUtility::GetUpgradeAppStartCmd();
    if(_ExecuteCmdWithReturn(qCmdByteArray, 50))
    {
        emit SignalUpgradeValue(5);//升级开始和升级结束命令各占5%的进度
        //升级步骤2 升级文件数据传输
        m_iCmdType = ProtocolUtility::s_iCmdUpgradeAppData;
        QVector<QByteArray> upgradeDataCmdVect = ProtocolUtility::GetUpgradeAppCmd(qFilePathStr);
        for(quint8 pkgNum = 1;pkgNum<=upgradeDataCmdVect.size();pkgNum++)
        {
            if(!_ExecuteCmdWithReturn(upgradeDataCmdVect[pkgNum-1], 50))
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
        m_iCmdType = ProtocolUtility::s_iCmdUpgradeAppEnd;
        qCmdByteArray = ProtocolUtility::GetUpgradeAppEndCmd();
        if(!_ExecuteCmdWithReturn(qCmdByteArray, 50))
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
    m_DeviceOperateMutex.lock();
    m_bIsDeviceOperate = bIsOperate;
    m_DeviceOperateMutex.unlock();
}

bool HIDOpertaionUtility::GetDeviceOperateStates()
{
    //DeviceOperateMutex.lock();
    return m_bIsDeviceOperate;
    //DeviceOperateMutex.unlock();
}

void HIDOpertaionUtility::HIDUpgradeSubControl(QString filePath)
{
    //如果在HID后台操作线程则直接调用,否则需通过消息转发,以便把相应操作转到HID后台操作线程
    if(QThread::currentThreadId() == m_WorkHandle)
    {
        _SlotUpgradeSubControl(filePath);
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

    quint8 readByte[CMD_LEN] = {0xFF};

    //读线程必须是另外一个HIDOpertaionUtility操作线程打开仪器后才有存在意义
    while(HIDOpertaionUtility::GetInstance()->IsDeviceOpen())
    {

        //ReadHidDataFunc最后一个参数负1则读线程堵塞直到数据读取成功
        int readRetVal = HIDOpertaionUtility::GetInstance()->HIDRead((quint8*)(&readByte), -1);
        //qDebug() << "readRetVal = " << readRetVal;
        if(readRetVal > 0)
        {
            //QMutexLocker locker(&mDataMutex);
            QByteArray data;
            for(int i=0; i < CMD_LEN; i++)
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
}
