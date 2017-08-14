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

    m_curHIDCmdData.cmdType = ProtocolUtility::CMD_DEV_CLOSE;
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
        connect(s_hidCmdThreadInstance, SIGNAL(finished()), s_hidCmdThreadInstance, SLOT(deleteLater()));
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
    if(ProtocolUtility::CMD_DEV_CLOSE == hidCmdData.cmdType)
    {
        qDebug() << "_________" <<__FUNCTION__;
        HIDOpertaionUtility::GetInstance()->HIDClose();
        SetStopped(true);
        m_hidCmdDataQueue.clear();
        m_curHIDCmdData = hidCmdData;
    }
    else
    {
        qDebug() << "_________" <<__FUNCTION__;
        m_hidCmdDataQueue.enqueue(hidCmdData);
    }

    m_qCmdMutex.unlock();
}

void CHidCmdThread::AddResetMotorCmd(quint16 resetSpeed)
{
    //添加马达复位命令
    HIDCmdData hidCmdData;
    hidCmdData.cmdType = ProtocolUtility::CMD_RESET_MOTOR;
    hidCmdData.byteArrayVect.push_back(ProtocolUtility::GetMotorResetCmd(resetSpeed));
    AddCmd(hidCmdData);
}

void CHidCmdThread::AddRotateMotorCmd(quint16 speed, quint16 step, quint16 direction)
{
    //添加马达转动命令
    HIDCmdData hidCmdData;
    hidCmdData.cmdType = ProtocolUtility::CMD_ROTATE_MOTOR;
    hidCmdData.byteArrayVect.push_back(ProtocolUtility::GetMotorRotateCmd(direction, step, speed));
    AddCmd(hidCmdData);
}


void CHidCmdThread::AddCloseHIDCmd()
{
    qDebug() << "_________" <<__FUNCTION__;
    //添加关闭HID通信命令，杀死HID读线程
    HIDCmdData hidCmdData;
    hidCmdData.cmdType = ProtocolUtility::CMD_DEV_CLOSE;
    AddCmd(hidCmdData);
}

void CHidCmdThread::AddOpenLedCmd(int iLedIndex, quint16 iBrightness)
{
    //添加开灯命令
    HIDCmdData hidCmdData;
    hidCmdData.byteArrayVect.push_back(ProtocolUtility::GetLEDCmd(iLedIndex,iBrightness));
    hidCmdData.cmdType = ProtocolUtility::CMD_OPEN_OR_CLOSE_LED;
    CHidCmdThread::GetInstance()->AddCmd(hidCmdData);
}

void CHidCmdThread::AddWriteDevParamsCmd(DevConfigParams devConfigParams)
{
    m_qCmdMutex.lock();
    m_devConfigParams = devConfigParams;
    m_qCmdMutex.unlock();
    AddCmdWithoutCmdData(ProtocolUtility::CMD_WRITE_PARAM_TO_DEV);
}


void CHidCmdThread::AddCmdWithoutCmdData(quint16 qCmdType)
{
    //添加不带命令数据的命令
    HIDCmdData hidCmdData;
    switch(qCmdType)
    {
        case ProtocolUtility::CMD_CLOSE_ALL_LED://添加关灯命令
        {
            hidCmdData.cmdType = ProtocolUtility::CMD_CLOSE_ALL_LED;
            hidCmdData.byteArrayVect.push_back(ProtocolUtility::GetCloseAllLEDCmd());
            AddCmd(hidCmdData);
            break;
        }
        case ProtocolUtility::CMD_CLOSE_ALL_LED_AND_STOP_MOTOR://添加关灯关马达命令
        {
            hidCmdData.byteArrayVect.push_back(ProtocolUtility::GetCloseAllLEDAndStopMotorCmd());
            hidCmdData.cmdType = ProtocolUtility::CMD_CLOSE_ALL_LED_AND_STOP_MOTOR;
            AddCmd(hidCmdData);
            break;
        }
        case ProtocolUtility::CMD_READ_DEV_VERSION://添加读下位机版本命令
        {
            hidCmdData.cmdType = ProtocolUtility::CMD_READ_DEV_VERSION;
            hidCmdData.byteArrayVect.push_back(ProtocolUtility::GetReadVersionCmd());
            AddCmd(hidCmdData);
            break;
        }
        case ProtocolUtility::CMD_READ_PARAM_FROM_DEV://读取仪器参数
        {
            hidCmdData.cmdType = ProtocolUtility::CMD_READ_PARAM_FROM_DEV;
            hidCmdData.byteArrayVect.clear();
            AddCmd(hidCmdData);
            break;
        }
        case ProtocolUtility::CMD_WRITE_PARAM_TO_DEV://写仪器参数到设备
        {
            hidCmdData.cmdType = ProtocolUtility::CMD_WRITE_PARAM_TO_DEV;
            hidCmdData.byteArrayVect.clear();
            AddCmd(hidCmdData);
            break;
        }
        case ProtocolUtility::CMD_ADD_TEST_COUNT://仪器测量次数加1
        {
            hidCmdData.cmdType = ProtocolUtility::CMD_ADD_TEST_COUNT;
            hidCmdData.byteArrayVect.push_back(ProtocolUtility::GetAddTestCountCmd());
            AddCmd(hidCmdData);
            break;
        }
        case ProtocolUtility::CMD_READ_TEST_COUNT://读取仪器测量次数
        {
            hidCmdData.cmdType = ProtocolUtility::CMD_READ_TEST_COUNT;
            hidCmdData.byteArrayVect.push_back(ProtocolUtility::GetReadTestCountCmd());
            AddCmd(hidCmdData);
            break;
        }
        case ProtocolUtility::CMD_CLEAR_TEST_COUNT://仪器测量次数清零
        {
            hidCmdData.cmdType = ProtocolUtility::CMD_CLEAR_TEST_COUNT;
            hidCmdData.byteArrayVect.push_back(ProtocolUtility::GetClearTestCountCmd());
            AddCmd(hidCmdData);
            break;
        }
        case ProtocolUtility::CMD_UPGRADE_APP_START://
        {
            hidCmdData.cmdType = ProtocolUtility::CMD_UPGRADE_APP_START;
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
    AddCmdWithoutCmdData(ProtocolUtility::CMD_UPGRADE_APP_START);
}

void CHidCmdThread::SetStopped(bool bStopped)
{
    qDebug() << "_________" <<__FUNCTION__;
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
                if(ProtocolUtility::CMD_READ_PARAM_FROM_DEV == m_curHIDCmdData.cmdType)
                {
                    HIDOpertaionUtility::GetInstance()->HIDReadDevParams();
                }
                else if(ProtocolUtility::CMD_WRITE_PARAM_TO_DEV == m_curHIDCmdData.cmdType)
                {
                    HIDOpertaionUtility::GetInstance()->HIDWriteDevParams(m_devConfigParams);
                }
                else if(ProtocolUtility::CMD_UPGRADE_APP_START == m_curHIDCmdData.cmdType)
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

void CHidCmdThread::_SlotHIDCmdComplete(quint16 cmdType, bool result)
{
    if(cmdType == m_curHIDCmdData.cmdType)
    {
        _SetCmdCompleted(true);
    }
}

