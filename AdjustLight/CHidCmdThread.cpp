#include "CHidCmdThread.h"
#include <QApplication>
#include <QTime>
#include "HidOpertaionUtility.h"
#include "ProtocolUtility.h"
#include <QDebug>

CHidCmdThread* CHidCmdThread::s_hidCmdThreadInstance = NULL;

CHidCmdThread::CHidCmdThread(QObject *parent):QThread(parent)
{
    m_bStopped = true;
    connect(HIDOpertaionUtility::GetInstance(), SIGNAL(SignalOperationComplete(quint16,bool)),this,
            SLOT(_SlotHIDCmdComplete(quint16,bool)));

    m_curHIDCmdData.cmdType = ProtocolUtility::s_iCmdDevClose;
}

CHidCmdThread::~CHidCmdThread()
{
    disconnect(HIDOpertaionUtility::GetInstance(), SIGNAL(SignalOperationComplete(quint16,bool)),this,
                                                       SLOT(_SlotHIDCmdComplete(quint16,bool)));
}

CHidCmdThread *CHidCmdThread::GetInstance()
{
    if(NULL == s_hidCmdThreadInstance)
    {
        s_hidCmdThreadInstance = new CHidCmdThread();
        //connect(s_hidCmdThreadInstance, SIGNAL(finished()), s_hidCmdThreadInstance, SLOT(deleteLater()));
    }

    return  s_hidCmdThreadInstance;
}

void CHidCmdThread::ClearCmd()
{
    m_qCmdMutex.lock();
    m_hidCmdDataQueue.clear();
    m_qCmdMutex.unlock();
}

void CHidCmdThread::AddCmd(HIDCmdData hidCmdData)
{
    m_qCmdMutex.lock();
    //停止命令需要插队执行,其他命令清空
    if(ProtocolUtility::s_iCmdDevClose == hidCmdData.cmdType)
    {
        HIDOpertaionUtility::GetInstance()->HIDClose();
        SetStopped(true);
        m_hidCmdDataQueue.clear();
        m_curHIDCmdData = hidCmdData;
    }
    else
    {
        m_hidCmdDataQueue.enqueue(hidCmdData);
    }

    m_qCmdMutex.unlock();
}

void CHidCmdThread::AddResetMotorCmd(quint16 resetSpeed)
{
    //添加马达复位命令
    HIDCmdData hidCmdData;
    hidCmdData.cmdType = ProtocolUtility::s_iCmdResetMotor;
    hidCmdData.byteArrayVect.push_back(ProtocolUtility::GetMotorResetCmd(resetSpeed));
    AddCmd(hidCmdData);
}

void CHidCmdThread::AddRotateMotorCmd(quint16 speed, quint16 step, quint16 direction)
{
    //添加马达转动命令
    HIDCmdData hidCmdData;
    hidCmdData.cmdType = ProtocolUtility::s_iCmdRotateMotor;
    hidCmdData.byteArrayVect.push_back(ProtocolUtility::GetMotorRotateCmd(direction, step, speed));
    AddCmd(hidCmdData);
}


void CHidCmdThread::AddCloseHIDCmd()
{
    //添加关闭HID通信命令，杀死HID读线程
    HIDCmdData hidCmdData;
    hidCmdData.cmdType = ProtocolUtility::s_iCmdDevClose;
    AddCmd(hidCmdData);
}

void CHidCmdThread::AddOpenLedCmd(int iLedIndex, quint16 iBrightness)
{
    //添加开灯命令
    HIDCmdData hidCmdData;
    hidCmdData.byteArrayVect.push_back(ProtocolUtility::GetLEDCmd(iLedIndex,iBrightness));
    hidCmdData.cmdType = ProtocolUtility::s_iCmdOpenOrCloseLed;
    CHidCmdThread::GetInstance()->AddCmd(hidCmdData);
}

void CHidCmdThread::AddWriteDevParamsCmd(DevConfigParams devConfigParams)
{
    m_qCmdMutex.lock();
    m_devConfigParams = devConfigParams;
    m_qCmdMutex.unlock();
    AddCmdWithoutCmdData(ProtocolUtility::s_iCmdWriteParamToDev);
}


void CHidCmdThread::AddCmdWithoutCmdData(quint16 qCmdType)
{
    //添加不带命令数据的命令
    HIDCmdData hidCmdData;
    switch(qCmdType)
    {
        case ProtocolUtility::s_iCmdCloseAllLed://添加关灯命令
        {
            hidCmdData.cmdType = ProtocolUtility::s_iCmdCloseAllLed;
            hidCmdData.byteArrayVect.push_back(ProtocolUtility::GetCloseAllLEDCmd());
            AddCmd(hidCmdData);
            break;
        }
        case ProtocolUtility::s_iCmdCloseAllLedAndStopMotor://添加关灯关马达命令
        {
            hidCmdData.byteArrayVect.push_back(ProtocolUtility::GetCloseAllLEDAndStopMotorCmd());
            hidCmdData.cmdType = ProtocolUtility::s_iCmdCloseAllLedAndStopMotor;
            AddCmd(hidCmdData);
            break;
        }
        case ProtocolUtility::s_iCmdReadDevVersion://添加读下位机版本命令
        {
            hidCmdData.cmdType = ProtocolUtility::s_iCmdReadDevVersion;
            hidCmdData.byteArrayVect.push_back(ProtocolUtility::GetReadVersionCmd());
            AddCmd(hidCmdData);
            break;
        }
        case ProtocolUtility::s_iCmdReadParamFromDev://读取仪器参数
        {
            hidCmdData.cmdType = ProtocolUtility::s_iCmdReadParamFromDev;
            hidCmdData.byteArrayVect.clear();
            AddCmd(hidCmdData);
            break;
        }
        case ProtocolUtility::s_iCmdWriteParamToDev://写仪器参数到设备
        {
            hidCmdData.cmdType = ProtocolUtility::s_iCmdWriteParamToDev;
            hidCmdData.byteArrayVect.clear();
            AddCmd(hidCmdData);
            break;
        }
        case ProtocolUtility::s_iCmdAddTestCount://仪器测量次数加1
        {
            hidCmdData.cmdType = ProtocolUtility::s_iCmdAddTestCount;
            hidCmdData.byteArrayVect.push_back(ProtocolUtility::GetAddTestCountCmd());
            AddCmd(hidCmdData);
            break;
        }
        case ProtocolUtility::s_iCmdReadTestCount://读取仪器测量次数
        {
            hidCmdData.cmdType = ProtocolUtility::s_iCmdReadTestCount;
            hidCmdData.byteArrayVect.push_back(ProtocolUtility::GetReadTestCountCmd());
            AddCmd(hidCmdData);
            break;
        }
        case ProtocolUtility::s_iCmdClearTestCount://仪器测量次数清零
        {
            hidCmdData.cmdType = ProtocolUtility::s_iCmdClearTestCount;
            hidCmdData.byteArrayVect.push_back(ProtocolUtility::GetClearTestCountCmd());
            AddCmd(hidCmdData);
            break;
        }
        case ProtocolUtility::s_iCmdUpgradeAppStart://
        {
            hidCmdData.cmdType = ProtocolUtility::s_iCmdUpgradeAppStart;
            hidCmdData.byteArrayVect.clear();
            AddCmd(hidCmdData);
            break;
        }

        default:
            break;
    }
}

void CHidCmdThread::AddUpgradeSubControlCmd(QString qFilePathStr)
{
    m_qCmdMutex.lock();
    m_qFilePathStr = qFilePathStr;
    m_qCmdMutex.unlock();
    AddCmdWithoutCmdData(ProtocolUtility::s_iCmdUpgradeAppStart);
}

void CHidCmdThread::SetStopped(bool bStopped)
{
    m_qStoppedMutex.lock();
    m_bStopped = bStopped;
    m_qStoppedMutex.unlock();
}

bool CHidCmdThread::GetStopped()
{
    return m_bStopped;
}

void CHidCmdThread::run()
{

    HIDOpertaionUtility::GetInstance()->HIDOpen();//默认打开设备
    m_bStopped = false;
    while(!m_bStopped)
    {
        if(m_hidCmdDataQueue.size()>0)
        {
            m_qCmdMutex.lock();
            m_curHIDCmdData = m_hidCmdDataQueue.dequeue();
            m_qCmdMutex.unlock();
            _SetCmdCompleted(false);
            if(m_curHIDCmdData.byteArrayVect.size() == 1)
            {
                HIDOpertaionUtility::GetInstance()->HIDWrite(m_curHIDCmdData.byteArrayVect.at(0));
            }
            else
            {
                if(ProtocolUtility::s_iCmdReadParamFromDev == m_curHIDCmdData.cmdType)
                {
                    HIDOpertaionUtility::GetInstance()->HIDReadDevParams();
                }
                else if(ProtocolUtility::s_iCmdWriteParamToDev == m_curHIDCmdData.cmdType)
                {
                    HIDOpertaionUtility::GetInstance()->HIDWriteDevParams(m_devConfigParams);
                }
                else if(ProtocolUtility::s_iCmdUpgradeAppStart == m_curHIDCmdData.cmdType)
                {
                    HIDOpertaionUtility::GetInstance()->HIDUpgradeSubControl(m_qFilePathStr);
                }
            }
            QTime oldTime=QTime::currentTime();
            while(!m_bCmdCompleted)//等待命令完成
            {
                int iMsecs=oldTime.msecsTo(QTime::currentTime());
                if(iMsecs>CMD_WAIT_TIME)
                {
                    break;
                }
            }
        }
        qApp->processEvents();
        msleep(5);
    }
    qDebug() << "_________" <<__FUNCTION__;

}

void CHidCmdThread::_SetCmdCompleted(bool bCmdCompleted)
{
    m_qCmdCompleteMutex.lock();
    m_bCmdCompleted = bCmdCompleted;
    m_qCmdCompleteMutex.unlock();
}

void CHidCmdThread::_SlotHIDCmdComplete(quint16 cmdType, bool /*result*/)
{
    if(cmdType == m_curHIDCmdData.cmdType)
    {
        _SetCmdCompleted(true);
    }
}

