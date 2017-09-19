#include "HidOpertaionUtility.h"

#include<QLibrary>
#include <QDebug>
#include <iostream>
#include <QTimer>
#include <QCoreApplication>
#include <QTime>
#include <QApplication>

#include "ProtocolUtility.h"

#define ACK_TIME_OUT_SECOND 5
HIDOpertaionUtility* HIDOpertaionUtility::instance = NULL;

HIDOpertaionUtility *HIDOpertaionUtility::GetInstance()
{
    qRegisterMetaType<DevConfigParams>("DevConfigParams");
    if(instance == NULL)
    {
        instance = new HIDOpertaionUtility();

    }
    return instance;
}


HIDOpertaionUtility::HIDOpertaionUtility()
{
    m_pReadThread = NULL;
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

    m_WriteHidTimer = new QTimer;
    connect(m_WriteHidTimer,SIGNAL(timeout()),this,SLOT(_SlotmWriteHidTimeOut()));
    m_WorkThread.start();
    m_DevVersion = "";
    m_pDevConfigParamsByte = new quint8[sizeof(DevConfigParams)];
    m_bIsUpdateFlag = false;
    m_bIsUpdateDataSendOver = false;
}

HIDOpertaionUtility::~HIDOpertaionUtility()
{
    delete m_pDevConfigParamsByte;
    if(instance != NULL)
    {
        delete instance;
    }

    if(m_pReadThread != NULL)
    {
        delete m_pReadThread;
    }
}

void HIDOpertaionUtility::_SlotLoadDll()
{
    //导入dll,并获取dll包含的操作函数指针
    m_pReadThread = new HIDReadThread();

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
    {
        emit SignalHIDWrite(writeByteArray);
    }

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

bool HIDOpertaionUtility::HIDRead(quint8 *pRecvDataBuf, int iDelaytime)
{
    bool bResult = false;
    if(m_HidHandle)
    {
        bResult = ReadHidData(m_HidHandle, pRecvDataBuf, iDelaytime);
    }
    return bResult;
}

bool HIDOpertaionUtility::IsDeviceOpen()
{
    return m_IsDeviceOpened;
}

bool HIDOpertaionUtility::_SlotOpen()
{
    //打开设备,开启设备读线程
    _SetWaitForAck(false);
    bool bIsOpen = false;
    if(m_HidHandle > 0)
    {
        //如已打开需关闭再重开
        m_bIsUSBConnect = false;
        _SlotClose();
    }
    //打开设备
    m_HidHandle = OpenMyHIDDevice(kiUsbPid, kiUsbVid);
    if((qint32)m_HidHandle > 0)
    {
        m_bIsUSBConnect = true;
        m_IsDeviceOpened = true;
        if(!m_pReadThread)
        {
            m_pReadThread = new HIDReadThread();
        }
        //开启线程
        m_pReadThread->start();
        bIsOpen = true;
    }

    if(bIsOpen == false)
    {
        //发送USB连接错误信号
        SignalErrInfo(EnumTypeErr::ErrNoConnectUSB);
    }
    return bIsOpen;
}

bool HIDOpertaionUtility::_SlotClose()
{
    //关闭所有灯与电机立刻停止
    _ExecuteCmdWithAck(ProtocolUtility::GetCloseAllLEDAndStopMotorCmd());
    bool bResult = false;
    if(m_HidHandle > 0)
    {
        CloseDev(m_HidHandle);
    }
    m_IsDeviceOpened = false;//用于关闭读取线程
    m_HidHandle = NULL;
    return bResult;
}

bool HIDOpertaionUtility::_SendCmdToDev(QByteArray writeByteArray)
{
    if(m_HidHandle)
    {
        //发送字节，最大发送64个
        //目前发现发送到设备的命令必须是64个字节，少于64个字节发送命令执行失败，待确认原因
        quint8 iWriteByte[kiCmdLen] = {0};
//        qDebug() << "writeByteArray.length() = " << writeByteArray.length();
//        qDebug() << "writeByteArray = " << writeByteArray;
        for(int i= 0; i < writeByteArray.length(); i++)
        {
            iWriteByte[i] = static_cast <quint8> (writeByteArray.at(i));
        }
        int iWriteResult = WriteHidData(m_HidHandle,
                                        reinterpret_cast <quint8*> (&iWriteByte) /*(quint8*)&iWriteByte*/,
                                        kiCmdLen);
        if (iWriteResult > 0)
        {
            return true;
        }
        else
        {
            return false;
        }

    }
    else
    {
        return false;
    }

}

void HIDOpertaionUtility::_SlotmWriteHidTimeOut()
{
    m_bWriteHidTimeOut = true;
    _SlotOpen();
}
bool HIDOpertaionUtility::_SlotWrite(QByteArray qWriteByteArray)
{
    bool bResult = false;
    m_iCmdType = 1;
    if(m_HidHandle > 0)
    {
        m_iCmdType = ((qWriteByteArray.at(5)<<8) && 0xFF00) | (qWriteByteArray.at(6)&0x00FF);
        switch (m_iCmdType)
        {
            case ProtocolUtility::sm_kiCmdRotateMotor://电机转动命令
            {
                bResult = _RotateMotor(qWriteByteArray);
                break;
            }
            case ProtocolUtility::sm_kiCmdResetMotor://电机复位指令
            {
                bResult = _ExecuteCmdWithAckAndReturn(qWriteByteArray,120);
                break;
            }
            case ProtocolUtility::sm_kiCmdOpenOrCloseLed://LED灯开关命令
            case ProtocolUtility::sm_kiCmdCloseAllLed://关闭所有灯指令
            case ProtocolUtility::sm_kiCmdCloseAllLedAndStopMotor://关闭所有灯与电机立刻停止
            {
                bResult = _ExecuteCmdWithAck(qWriteByteArray);
                break;
            }
            case ProtocolUtility::sm_kiCmdReadDevVersion://读取仪器下位机软件版本
            {
                bResult = _ExecuteCmdWithReturn(qWriteByteArray,100);
                break;
            }
            case ProtocolUtility::sm_kiCmdReadTestCount:
            case ProtocolUtility::sm_kiCmdAddTestCount:
            case ProtocolUtility::sm_kiCmdClearTestCount:
            {
                bResult = _ExecuteCmdWithReturn(qWriteByteArray,10);
                break;
            }
            case ProtocolUtility::sm_kiCmdUpgradeAppFlag:
            {
                bResult = _ExecuteCmdWithReturn(qWriteByteArray,100);
                break;
            }
            case ProtocolUtility::sm_kiCmdWriteSerialNumberToDev:
            {
                bResult = _ExecuteCmdWithReturn(qWriteByteArray,10);
                break;
            }
            case ProtocolUtility::sm_kiCmdReadSerialNumberFromDev:
            {
                bResult = _ExecuteCmdWithReturn(qWriteByteArray,10);
                break;
            }
            default:
                break;
        }
    }
    emit SignalOperationComplete(m_iCmdType,bResult);
    SetDeviceOperate(false);
    return bResult;
}


void HIDOpertaionUtility::ReceiveNewCmdFromDev(QByteArray qDataByteArray)
{
    //毒检设备所有消息统一在这里处理，消息有两种类型ACK和结果，
    //因为有命令需要等待ACK和结果，因而这里使用类的成员变量mIsWaitForAck
    // /mIsWaitForReturn/mAckResult/mReturnResult进行状态同步
    if(m_bIsUSBConnect == false)
    {
        return;
    }
//    qDebug() << "m_IsWaitForAck = " << m_IsWaitForAck;
//    qDebug() << "qDataByteArray = " <<qDataByteArray;
    if(m_IsWaitForAck)
    {
        if(static_cast <quint8> (qDataByteArray.at(1)) == static_cast <quint8> (m_iCmdType))
        {
            _SetAckResult(true);
        }
        _SetWaitForAck(false);

    }
    else if(m_IsWaitForReturn)
    {
        switch (quint8(m_iCmdType))
        {
            case ProtocolUtility::sm_kiCmdRotateMotor:                //电机转动命令
            case ProtocolUtility::sm_kiCmdResetMotor:                 //电机复位指令
            case ProtocolUtility::sm_kiCmdCloseAllLed:                //电机复位指令
            case ProtocolUtility::sm_kiCmdCloseAllLedAndStopMotor:    //关闭所有灯与电机立刻停止
            case ProtocolUtility::sm_kiCmdClearTestCount:             //仪器测量次数清零
            case ProtocolUtility::sm_kiCmdUpgradeAppEnd:              //仪器下位机程序升级结束
            {
                if((quint8)qDataByteArray.at(5) == (quint8)(m_iCmdType / 256)
                        && (quint8)(qDataByteArray.at(6) == static_cast <quint8> (m_iCmdType))
                        && (quint8)qDataByteArray.at(1) == 0x01
                        && (quint8)qDataByteArray.at(2) == 0x01)
                {
                    _SetReturnResult(true);
                }
                break;
            }
            case ProtocolUtility::sm_kiCmdReadParamFromDev:           //读取仪器参数
            {
                if((quint8)qDataByteArray.at(5) == (m_iCmdType / 256)
                        && ((quint8)qDataByteArray.at(6) == static_cast <quint8> (m_iCmdType))
                        && (quint8)qDataByteArray.at(1) == 0x01
                        && (quint8)qDataByteArray.at(2) == 0x01)
                {
                    _SetReturnResult(true);
                    quint8 iCmdLen = qDataByteArray.at(3);
                    iCmdLen = iCmdLen * 256 + qDataByteArray.at(4);
                    //if (ReadCRC8(data, cmdLen - 1) != GetReadbyte[cmdLen]) return null;
                    quint8 iPkgNum = qDataByteArray.at(kiDataStartIndex-1);//
                    iCmdLen -= kiDataStartIndex;

                    int iIndex = (iPkgNum-1)*kiParamPackageDataLen;
                    for (int i = 0; i < iCmdLen; i++)
                    {
                        m_pDevConfigParamsByte[iIndex] = qDataByteArray.at(kiDataStartIndex + i);
                        iIndex++;
                    }
                    if(iPkgNum == kiParamPackageSize)
                    {
                        memcpy(&m_Params, m_pDevConfigParamsByte,sizeof(DevConfigParams));
                        emit SignalReceiveDevParams(m_Params);
                    }
                }
                break;
            }
            case ProtocolUtility::sm_kiCmdReadDevVersion:             //读取仪器下位机软件版本
            {
            //qDebug()<<"qDataByteArray:" << qDataByteArray;
                if((quint8)qDataByteArray.at(5) == (m_iCmdType/256)
                        && ((quint8)qDataByteArray.at(6) == static_cast <quint8> (m_iCmdType))
                        && (quint8)qDataByteArray.at(1) == 0x01
                        && (quint8)qDataByteArray.at(2) == 0x01)
                {
                    _SetReturnResult(true);
                    quint8 iCmdLen = qDataByteArray.at(3);
                    iCmdLen = iCmdLen * 256 + qDataByteArray.at(4);
                    //if (ReadCRC8(data, cmdLen - 1) != GetReadbyte[cmdLen]) return null;
                    iCmdLen -= kiDataStartIndex;
                    m_DevVersion.clear();
                    for (int i = 0; i < iCmdLen; i++)
                    {
                        m_DevVersion.push_back(qDataByteArray.at(kiDataStartIndex + i));
                    }
                    qDebug()<<"Version:"<<m_DevVersion;
                    emit SignalReceiveDevVersion(m_DevVersion);
                }
                break;
            }
            case ProtocolUtility::sm_kiCmdAddTestCount:               //仪器测量次数加1
            case ProtocolUtility::sm_kiCmdReadTestCount:              //仪器测量次数读取
            {
                if((quint8)qDataByteArray.at(5) == (m_iCmdType/256)
                        && ((quint8)qDataByteArray.at(6) == static_cast <quint8> (m_iCmdType))
                        && (quint8)qDataByteArray.at(1) == 0x01
                        && (quint8)qDataByteArray.at(2) == 0x01)
                {
                    _SetReturnResult(true);
                    quint8 iCmdLen = qDataByteArray.at(3);
                    iCmdLen = iCmdLen * 256 + qDataByteArray.at(4);
                    //if (ReadCRC8(data, cmdLen - 1) != GetReadbyte[cmdLen]) return null;
                    iCmdLen -= kiDataStartIndex;
                    if(iCmdLen != 4)
                    {
                        qDebug()<<"读取/添加测试次数命令返回错误";
                    }
                    else
                    {
                        m_DevVersion.clear();
                        quint8 iTestCountArray[4];
                        for (int i = 0; i < 4; i++)
                        {
                            iTestCountArray[i] = qDataByteArray.at(kiDataStartIndex + i);
                        }
                        m_qTestCount=((iTestCountArray[0]<<8) && 0xFF000000)
                                | ((iTestCountArray[1]<<8) && 0x00FF0000)
                                | ((iTestCountArray[2]<<8) && 0x0000FF00)
                                | (iTestCountArray[3] & 0x000000FF);
                        qDebug()<< "Test Count:" << m_qTestCount;
                        emit SignalReceiveTestCount(m_qTestCount);
                    }
                }
                break;
            }
            case ProtocolUtility::sm_kiCmdUpgradeAppStart:             //仪器下位机程序升级开始
            {
                if(((quint8)qDataByteArray.at(5) == (m_iCmdType/256))
                        && ((quint8)qDataByteArray.at(6) == static_cast <quint8> (m_iCmdType))
                        && ((quint8)qDataByteArray.at(1) == 0x01)
                        && ((quint8)qDataByteArray.at(2) == 0x01)
                        && ((quint8)qDataByteArray.at(kiDataStartIndex) == 0xA5/*0x5A*/)
                        && ((quint8)qDataByteArray.at(kiDataStartIndex+1) == 0xA5/*0x5A*/ ))
                {
                    _SetReturnResult(true);
                }
                break;
            }
            case ProtocolUtility::sm_kiCmdUpgradeAppData:              //仪器下位机程序升级数据
            {
                if((quint8)qDataByteArray.at(5) == (m_iCmdType/256)
                        && ((quint8)qDataByteArray.at(6) == static_cast <quint8> (m_iCmdType))
                        //&& ((quint8)qDataByteArray.at(1) == 0x01)
                        //&& ((quint8)qDataByteArray.at(2)) == 0x01
                        && ((quint8)qDataByteArray.at(kiDataStartIndex) == 0xAB)
                        && ((quint8)qDataByteArray.at(kiDataStartIndex+1) == 0xAB) )
                {
                    _SetReturnResult(true);
                }
                break;
            }
            case ProtocolUtility::sm_kiCmdUpgradeAppFlag:         //仪器下位机升级标志
            {
                qDebug() << "m_iCmdType = " << m_iCmdType;
                if((quint8)qDataByteArray.at(5) == (m_iCmdType/256)
                        && ((quint8)qDataByteArray.at(6) == static_cast <quint8> (m_iCmdType))
                        && ((quint8)qDataByteArray.at(1) == 0x01)
                        && ((quint8)qDataByteArray.at(2) == 0x01)
                        && ((quint8)qDataByteArray.at(kiDataStartIndex) == 0xAB)
                        && ((quint8)qDataByteArray.at(kiDataStartIndex+1) == 0xAB)
                        && ((quint8)qDataByteArray.at(kiDataStartIndex+2) == 0x5A)
                        && ((quint8)qDataByteArray.at(kiDataStartIndex+3) == 0xA5))
                {
                    m_bIsUpdateFlag = true;
                    _SetReturnResult(true);
                }
                break;
            }
            case ProtocolUtility::sm_kiCmdUpgradeAppSendDataOver:
            {
                qDebug() << "m_iCmdType = " << m_iCmdType;
                if((quint8)qDataByteArray.at(5) == static_cast <quint8> (m_iCmdType/256)
                        && ((quint8)qDataByteArray.at(6) == static_cast <quint8> (m_iCmdType))
                        && ((quint8)qDataByteArray.at(1) == 0x01)
                        && ((quint8)qDataByteArray.at(2) == 0x00))

                {
                    m_bIsUpdateDataSendOver = true;
                    _SetReturnResult(true);
                }
                break;
            }
            case ProtocolUtility::sm_kiCmdWriteSerialNumberToDev:
            {
                qDebug() << "sm_kiCmdWriteSerialNumberToDev = " << ProtocolUtility::sm_kiCmdWriteSerialNumberToDev;
                if((quint8)qDataByteArray.at(5) == (m_iCmdType / 256)
                    && (quint8)(qDataByteArray.at(6) == static_cast <quint8> (m_iCmdType))
                    && (quint8)qDataByteArray.at(1) == 0x01
                    && (quint8)qDataByteArray.at(2) == 0x01)
                {
                    //m_strDevSerialNumber
                    _SetReturnResult(true);
                    quint8 iCmdLen = qDataByteArray.at(3);
                    iCmdLen = iCmdLen * 256 + qDataByteArray.at(4);
                    iCmdLen -= kiDataStartIndex;
                    m_strDevSerialNumber.clear();
                    for (int i = 0; i < iCmdLen; i++)
                    {
                        quint8 iData = qDataByteArray.at(kiDataStartIndex + i);
                        QString strData = QString::number(iData,16);
                        if(strData.length() == 1)
                        {
                            strData = "0" + strData;    //补 0
                        }
                        //m_strDevSerialNumber.push_back(strData);
                        m_strDevSerialNumber.insert(0,strData);
                    }
                    qDebug() << "m_strDevSerialNumber = " << m_strDevSerialNumber;
                }
                break;
            }
            case ProtocolUtility::sm_kiCmdReadSerialNumberFromDev:
            {
                qDebug() << "sm_kiCmdReadSerialNumberFromDev = " << ProtocolUtility::sm_kiCmdReadSerialNumberFromDev;
                if((quint8)qDataByteArray.at(5) == (m_iCmdType / 256)
                    && ((quint8)qDataByteArray.at(6) == static_cast <quint8> (m_iCmdType))
                    && (quint8)qDataByteArray.at(1) == 0x01
                    && (quint8)qDataByteArray.at(2) == 0x01)
                {
                    _SetReturnResult(true);
                    quint8 iCmdLen = qDataByteArray.at(3);
                    iCmdLen = iCmdLen * 256 + qDataByteArray.at(4);
                    iCmdLen -= kiDataStartIndex;
                    m_strDevSerialNumber.clear();
                    m_strDevSerialNumber.clear();
                    for (int i = 0; i < iCmdLen; i++)
                    {
                        quint8 iData = qDataByteArray.at(kiDataStartIndex + i);
                        QString strData = QString::number(iData,16);
                        if(strData.length() == 1)
                        {
                            strData = "0" + strData;    //补 0
                        }
                        //m_strDevSerialNumber.push_back(strData);
                        m_strDevSerialNumber.insert(0,strData);
                        SignalReceiveDevSerialNumber(m_strDevSerialNumber);
                    }
                    qDebug() << "m_strDevSerialNumber = " << m_strDevSerialNumber;
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
    {
        emit SignalReadDevParams();
    }

}

void HIDOpertaionUtility::HIDWriteDevParams(DevConfigParams devConfigParams)
{
    //如果在HID后台操作线程则直接调用,否则需通过消息转发,以便把相应操作转到HID后台操作线程
    if(QThread::currentThreadId() == m_WorkHandle)
    {
        _SlotWriteDevParams(devConfigParams);
    }
    else
    {
        emit SignalWriteDevParams(devConfigParams);
    }


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
    QTime qOldTime = QTime::currentTime();
    while(m_IsWaitForAck)
    {
        int iSeconds = qOldTime.secsTo(QTime::currentTime());
        if(iSeconds > ACK_TIME_OUT_SECOND)
        {
            break;
        }
    }
    //qDebug()<<"ACK result:"<<m_AckResult;
    return m_AckResult;
}

bool HIDOpertaionUtility::_GetCmdReturn(int delaySeconds)
{
    _SetReturnResult(false);
    _SetWaitForReturn(true);
    QTime qOldTime = QTime::currentTime();
    while(m_IsWaitForReturn)
    {
        int iSeconds = qOldTime.secsTo(QTime::currentTime());
        if(iSeconds > delaySeconds)
        {
            break;
        }
    }
    return m_ReturnResult;
}

bool HIDOpertaionUtility::_ExecuteCmdWithAckAndReturn(QByteArray writeByteArray,int iDelaySeconds)
{
    bool bResult = false;
    //发送命令到设备
    if(!_SendCmdToDev(writeByteArray))
    {
        return bResult;
    }

    //获取应答
    if(!_GetCmdACK())
    {
        return bResult;
    }

    //获取命令返回时间
    if(!_GetCmdReturn(iDelaySeconds))
    {
        return bResult;
    }

    bResult = true;
    return bResult;
}

bool HIDOpertaionUtility::_ExecuteCmdWithAck(QByteArray writeByteArray)
{
    bool bResult = false;
    //发送命令到设备
    if(!_SendCmdToDev(writeByteArray))
    {
        return bResult;
    }
    //获取应答
    if(!_GetCmdACK())
    {
        return bResult;
    }

    bResult = true;
    return bResult;
}

bool HIDOpertaionUtility::_ExecuteCmdWithReturn(QByteArray writeByteArray,int iDelaySeconds)
{
    bool bResult = false;
    //发送命令到设备
    if(!_SendCmdToDev(writeByteArray))
    {
        qDebug() << "_SendCmdToDev = " << bResult;
        return bResult;
    }

     //获取命令返回时间
    if(!_GetCmdReturn(iDelaySeconds))
    {
        qDebug() << "_GetCmdReturn = " << bResult;
        return bResult;
    }

    bResult = true;
    return bResult;
}


//bool HIDOpertaionUtility::OpenOrCloseLed(QByteArray writeByteArray)
//{
//    return ExecuteCmdWithAck(writeByteArray);
//}

bool HIDOpertaionUtility::_RotateMotor(QByteArray writeByteArray)
{
    if(writeByteArray.length() < 12)//非法命令
    {
        return false;
    }

    quint16 iStep = writeByteArray.at(kiDataStartIndex) * 256 + writeByteArray.at(kiDataStartIndex+1);
    quint16 iSpeed = writeByteArray.at(kiDataStartIndex + 2) * 256 + writeByteArray.at(kiDataStartIndex+3);

    int iDelaySeconds =  (iSpeed * iStep * 0.015);
    bool bResult = _ExecuteCmdWithAckAndReturn(writeByteArray, iDelaySeconds);
    return bResult;

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
    bool bResult = false;
    memset(m_pDevConfigParamsByte, 0,sizeof(DevConfigParams));//清空设备配置参数临时数组内容
    m_iCmdType = ProtocolUtility::sm_kiCmdReadParamFromDev;
    QVector<QByteArray> readDevParamsVect = ProtocolUtility::GetReadParamsFromDevCmd();
    for(quint8 iPkgNum = 1; iPkgNum <= readDevParamsVect.size(); iPkgNum++)
    {
        if(!_ExecuteCmdWithReturn(readDevParamsVect[iPkgNum-1],3))
        {
            emit SignalOperationComplete(ProtocolUtility::sm_kiCmdReadParamFromDev, bResult);
            return bResult;
        }
    }
    bResult = true;
    emit SignalOperationComplete(ProtocolUtility::sm_kiCmdReadParamFromDev, bResult);
    return bResult;
}

bool HIDOpertaionUtility::_SlotWriteDevParams(DevConfigParams devConfigParams)
{
    bool bResult = false;
    m_iCmdType = ProtocolUtility::sm_kiCmdWriteParamToDev;
    QVector<QByteArray> writeDevParamsVector = ProtocolUtility::GetWriteParamFromDevCmd(devConfigParams);
    for(quint8 iPkgNum = 1; iPkgNum <= 10; iPkgNum++)
    {
        if(!_ExecuteCmdWithAck(writeDevParamsVector[iPkgNum - 1]))
        {
            emit SignalOperationComplete(ProtocolUtility::sm_kiCmdWriteParamToDev, bResult);
            return bResult;
        }
    }
    bResult = true;
    emit SignalOperationComplete(ProtocolUtility::sm_kiCmdWriteParamToDev, bResult);
    return bResult;
}

void HIDOpertaionUtility::_EmitUpgradeErrorSignal(bool bResult)
{
    emit SignalOperationComplete(m_iCmdType, bResult);
    emit SignalUpgradeError(QString::fromLocal8Bit("升级失败"));
}

bool HIDOpertaionUtility::_SlotUpgradeSubControl(QString strFilePath)
{
    bool bResult = false;
    //升级步骤1 升级开始命令发送
    m_iCmdType = ProtocolUtility::sm_kiCmdUpgradeAppStart;
    QByteArray qCmdByteArray = ProtocolUtility::GetUpgradeAppStartCmd();
    qDebug() << "qCmdByteArray = " << qCmdByteArray;
    if(_ExecuteCmdWithReturn(qCmdByteArray, 20))
    {
        emit SignalUpgradeValue(5);//升级开始和升级结束命令各占5%的进度
        //升级步骤2 升级文件数据传输
        m_bIsUpdateDataSendOver = false;

        m_iCmdType = ProtocolUtility::sm_kiCmdUpgradeAppData;
        QVector<QByteArray> upgradeDataCmdVect = ProtocolUtility::GetUpgradeAppCmd(strFilePath);
        for(int iPkgNum = 1; iPkgNum <= upgradeDataCmdVect.size(); iPkgNum++)
        {
            qDebug() << "iPkgNum =" << iPkgNum;
            qDebug() << "upgradeDataCmdVect[iPkgNum - 1] =" << upgradeDataCmdVect[iPkgNum - 1];
//            if(!_ExecuteCmdWithReturn(upgradeDataCmdVect[iPkgNum - 1], 3))
//            {
//                QThread::msleep(1000);
//                qDebug() << "iPkgNum =" << iPkgNum;
//                qDebug() << "upgradeDataCmdVect[iPkgNum - 1] =" << upgradeDataCmdVect[iPkgNum - 1];
//                if(!_ExecuteCmdWithReturn(upgradeDataCmdVect[iPkgNum - 1], 3))
//                {
//                    _EmitUpgradeErrorSignal(bResult);
//                    return bResult;
//                }
//                else
//                {
//                    emit SignalUpgradeValue(5 + iPkgNum * 90 / upgradeDataCmdVect.size());
//                }
//            }

            for(int iPos = 0; iPos < 10; iPos++)
            {
                qDebug() << "iPkgNum =" << iPkgNum;
                qDebug() << "upgradeDataCmdVect[iPkgNum - 1] =" << upgradeDataCmdVect[iPkgNum - 1];
                if(_ExecuteCmdWithReturn(upgradeDataCmdVect[iPkgNum - 1], 3))
                {
                    break;
                }
                if(iPos >= 9)
                {
                    _EmitUpgradeErrorSignal(bResult);
                    return bResult;
                }
                QThread::msleep(100);
            }
//            while(!_ExecuteCmdWithReturn(upgradeDataCmdVect[iPkgNum - 1], 3))
//            {
//                qDebug() << "iPkgNum =" << iPkgNum;
//                qDebug() << "upgradeDataCmdVect[iPkgNum - 1] =" << upgradeDataCmdVect[iPkgNum - 1];
//                iPos++;
//                if(iPos >= 10)
//                {
//                    _EmitUpgradeErrorSignal(bResult);
//                    return bResult;
//                }
//            }
            emit SignalUpgradeValue(5 + iPkgNum * 90 / upgradeDataCmdVect.size());
            QThread::msleep(50);
        }

        m_iCmdType = ProtocolUtility::sm_kiCmdUpgradeAppSendDataOver;
        QByteArray qCmdByteArray = ProtocolUtility::GetUpgradeAppDataOverCmd();
        qDebug() << "qCmdByteArray = " << qCmdByteArray;
        if(!_ExecuteCmdWithReturn(qCmdByteArray, 20))
        {
            QThread::msleep(1000);
            if(!_ExecuteCmdWithReturn(qCmdByteArray, 20))
            {
                _EmitUpgradeErrorSignal(bResult);
                return bResult;
            }
//            while(!m_bIsUpdateDataSendOver)
//            {
//                 QApplication::processEvents();
//                 QThread::msleep(50);
//            }
        }

        //升级步骤3 升级结束命令发送
        m_iCmdType = ProtocolUtility::sm_kiCmdUpgradeAppEnd;
        qCmdByteArray = ProtocolUtility::GetUpgradeAppEndCmd();
        if(!_ExecuteCmdWithReturn(qCmdByteArray, 50))
        {
            _EmitUpgradeErrorSignal(bResult);
            return bResult;
        }
        bResult = true;
        emit SignalOperationComplete(m_iCmdType, bResult);
        emit SignalUpgradeFinish();
        emit SignalUpgradeValue(100);//升级完成，进度100%
    }
    else
    {
         _EmitUpgradeErrorSignal(bResult);
    }
    return bResult;
}

void HIDOpertaionUtility::SetDeviceOperate(bool bIsOperate)
{
    m_DeviceOperateMutex.lock();
    m_bIsDeviceOperate = bIsOperate;
    m_DeviceOperateMutex.unlock();
}

bool HIDOpertaionUtility::GetUpdateFlag()
{
    return m_bIsUpdateFlag;
}

void HIDOpertaionUtility::SetUpdateFlag(bool bIsUpdate)
{
    m_bIsUpdateFlag = bIsUpdate;
}

bool HIDOpertaionUtility::GetDeviceOperateStates()
{
    //DeviceOperateMutex.lock();
    return m_bIsDeviceOperate;
    //DeviceOperateMutex.unlock();
}

void HIDOpertaionUtility::HIDUpgradeSubControl(QString strFilePath)
{
    //如果在HID后台操作线程则直接调用,否则需通过消息转发,以便把相应操作转到HID后台操作线程
    if(QThread::currentThreadId() == m_WorkHandle)
    {
        _SlotUpgradeSubControl(strFilePath);
    }
    else
    {
        emit SignalHIDUpgradeSubControl(strFilePath);
    }

}

bool HIDOpertaionUtility::CheckDeviceConnection()
{
    CloseDev(m_HidHandle);
    m_HidHandle = NULL;
    quint8 *Readbyte = new quint8[100];
    m_HidHandle = OpenMyHIDDevice(kiUsbPid, kiUsbVid);
    if (m_HidHandle < 0)
    {
        CloseDev(m_HidHandle);
        delete Readbyte;
        m_bIsUSBConnect = false;
        return false;
    }
    if (m_HidHandle > 0)
    {
        int recl = -1;
        recl = ReadHidData(m_HidHandle,Readbyte,-1);
        if (recl < 0)
        {
            //qDebug() << "USB pull out";
            CloseDev(m_HidHandle);
            delete Readbyte;
            m_bIsUSBConnect = false;
            return false;
        }
        else
        {
            delete Readbyte;
            m_bIsUSBConnect = true;
            return true;
        }
    }

}

void HIDOpertaionUtility::SetDeviceConnection(bool)
{
    m_bIsUSBConnect = true;

}

QString HIDOpertaionUtility::GetDeviceSerialNumber()
{
    return m_strDevSerialNumber;
}

HIDReadThread::HIDReadThread()
{

}

HIDReadThread::~HIDReadThread()
{

}

void HIDReadThread::run()
{

    quint8 iReadByte[kiCmdLen] = {0xFF};

    //读线程必须是另外一个HIDOpertaionUtility操作线程打开仪器后才有存在意义
    while(HIDOpertaionUtility::GetInstance()->IsDeviceOpen())
    {

        //ReadHidDataFunc最后一个参数负1则读线程堵塞直到数据读取成功
        bool iReadRetVal = HIDOpertaionUtility::GetInstance()->HIDRead((quint8*)(&iReadByte), -1);
        //qDebug() << "iReadRetVal = " << iReadRetVal;

        if(iReadRetVal == true)
        {
            //QMutexLocker locker(&mDataMutex);
            m_DataMutex.lock();
            QByteArray bDataByteArray;
            for(int i = 0; i < kiCmdLen; i++)
            {
                //qDebug() << "iReadByte[i] = " << iReadByte[i];
                bDataByteArray.push_back(iReadByte[i]);
            }
            //qDebug()<<"ReadByte:"<< bDataByteArray;
            HIDOpertaionUtility::GetInstance()->ReceiveNewCmdFromDev(bDataByteArray);
            //                emit SignalReceiveNewCmd(data);
            //emit SignalReceiveNewCmd();
            //qDebug()<<"ReadByte:"<<bDataByteArray.toHex();
            //msleep(200);
            m_DataMutex.unlock();
        }
        //else
        {
            msleep(500);
        }
    }
}
