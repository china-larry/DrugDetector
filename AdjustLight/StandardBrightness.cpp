#include "StandardBrightness.h"
#include <QApplication>
#include "CHidCmdThread.h"
#include "HidOpertaionUtility.h"
#include "ProtocolUtility.h"
#include "OpencvUtility.h"
#include "ParamsConfiguration.h"
#include "testing/ThreadTesting.h"

StandardBrightness::StandardBrightness()
{

}

StandardBrightness::~StandardBrightness()
{

}

void StandardBrightness::SlotGetBrightValue(BrightnessValue brightnessValue)
{
    SetBrightness(brightnessValue);
    //设置灯光
    if(SetBrightnessValue(brightnessValue) == true)
    {
        //获取图片
        QString strSaveImagePath = "";
        if(OpencvUtility::getInstance()->GetVideoCapture(&strSaveImagePath) == true)
        {
            //获取绿色分量
            GetGreenComponunt(brightnessValue.iCupType,strSaveImagePath);
            if(!m_iGreenComponuntList.isEmpty())
            {
                //发送给UI显示
                emit SignalSendPictureToUI(strSaveImagePath,m_iGreenComponuntList);
                return;
            }
            //顺时针转 15 * 10 步
            for(qint16 step1 = 0;step1 < 15;step1++)
            {
                HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
                CHidCmdThread::GetInstance()->AddRotateMotorCmd(20,10,0);

                while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
                {
                    QApplication::processEvents();
                }

                //获取绿色分量
                GetGreenComponunt(brightnessValue.iCupType,strSaveImagePath);
                if(!m_iGreenComponuntList.isEmpty())
                {
                    //发送给UI显示
                    emit SignalSendPictureToUI(strSaveImagePath,m_iGreenComponuntList);
                    return;
                }
            }


            //逆时针转 30 * 10 步
            for(qint16 step2 = 0;step2 < 30;step2++)
            {
                HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
                CHidCmdThread::GetInstance()->AddRotateMotorCmd(20,10,1);

                while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
                {
                    QApplication::processEvents();
                }

                //获取绿色分量
                GetGreenComponunt(brightnessValue.iCupType,strSaveImagePath);
                if(!m_iGreenComponuntList.isEmpty())
                {
                    //发送给UI显示
                    emit SignalSendPictureToUI(strSaveImagePath,m_iGreenComponuntList);
                    return;
                }
            } 
        }
    }
}

void StandardBrightness::SetBrightness(BrightnessValue brightnessValue)
{
    m_brightnessValue = brightnessValue;
}

BrightnessValue StandardBrightness::GetBrightness()
{
    return m_brightnessValue;
}

bool StandardBrightness::SetBrightnessValue(BrightnessValue brightnessValue)
{

    //打开设备
    if(CHidCmdThread::GetInstance()->GetStopped())
    {
        CHidCmdThread::GetInstance()->start();
    }
    else
    {
        CHidCmdThread::GetInstance()->SetStopped(true);
        while(CHidCmdThread::GetInstance()->isRunning())
        {
            continue;
        }
        CHidCmdThread::GetInstance()->start();
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_CLOSE_ALL_LED);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    CHidCmdThread::GetInstance()->AddOpenLedCmd(1,brightnessValue.iNo1);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    CHidCmdThread::GetInstance()->AddOpenLedCmd(2,brightnessValue.iNo2);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    CHidCmdThread::GetInstance()->AddOpenLedCmd(3,brightnessValue.iNo3);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    CHidCmdThread::GetInstance()->AddOpenLedCmd(4,brightnessValue.iNo4);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
    CHidCmdThread::GetInstance()->AddOpenLedCmd(5,brightnessValue.iNo5);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
    CHidCmdThread::GetInstance()->AddOpenLedCmd(6,brightnessValue.iNo6);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
    CHidCmdThread::GetInstance()->AddOpenLedCmd(7,brightnessValue.iNo7);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
    CHidCmdThread::GetInstance()->AddOpenLedCmd(8,brightnessValue.iNo8);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
    return true;
}

// 获取绿色分量曲线
bool StandardBrightness::GetGreenComponunt(qint16 iCupType,const QString strSaveImagePath)
{
//    ThreadTesting threadTesting;
//    m_iGreenComponuntList.clear();
//    m_iGreenComponuntList = threadTesting.ReceivePicPath(strSaveImagePath);
//    if(!m_iGreenComponuntList.isEmpty())
//    {
//        return true;
//    }

    return false;
}

//保存数据
bool StandardBrightness::SaveStandardParams(QString strFileName,QString ParamsType,BrightnessValue brightnessValue,
                        QList<int> iGreenComponuntList)
{
    QMap<QString,QVariant> strSaveDataMap;
    strSaveDataMap.insert("No1",brightnessValue.iNo1);
    strSaveDataMap.insert("No2",brightnessValue.iNo2);
    strSaveDataMap.insert("No3",brightnessValue.iNo3);
    strSaveDataMap.insert("No4",brightnessValue.iNo4);
    strSaveDataMap.insert("No5",brightnessValue.iNo5);
    strSaveDataMap.insert("No6",brightnessValue.iNo6);
    strSaveDataMap.insert("No7",brightnessValue.iNo7);
    strSaveDataMap.insert("No8",brightnessValue.iNo8);
    strSaveDataMap.insert("CupType",brightnessValue.iCupType);
    QMap<QString,QVariant> qLightValueMap;
    for(int i = 0;i < iGreenComponuntList.count();i++)
    {
        qLightValueMap.insert(QString::number(i),iGreenComponuntList.at(i));
    }
    strSaveDataMap.insert("GreenComponunt",qLightValueMap);
    if(ParamsConfiguration::getInstance()->SaveParamsToConfigFile(strFileName,ParamsType,strSaveDataMap))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool StandardBrightness::ReadStandardParams(QString strFileName,QString ParamsType,BrightnessValue &brightnessValue,
                        QList<int> &iGreenComponuntList)
{
    QMap<QString,QVariant> ParamsMap;
    if(ParamsConfiguration::getInstance()->ReadParamsFromConfigFile(strFileName,ParamsType,&ParamsMap))
    {
        brightnessValue.iNo1 = ParamsMap.value("No1").toInt();
        brightnessValue.iNo2 = ParamsMap.value("No2").toInt();
        brightnessValue.iNo3 = ParamsMap.value("No3").toInt();
        brightnessValue.iNo4 = ParamsMap.value("No4").toInt();
        brightnessValue.iNo5 = ParamsMap.value("No5").toInt();
        brightnessValue.iNo6 = ParamsMap.value("No6").toInt();
        brightnessValue.iNo7 = ParamsMap.value("No7").toInt();
        brightnessValue.iNo8 = ParamsMap.value("No8").toInt();
        brightnessValue.iCupType = ParamsMap.value("CupType").toInt();

        QMap<QString,QVariant> qstrLightValueMap = ParamsMap.value("GreenComponunt").toMap();

        QMap<QString,QVariant>::const_iterator ParamsMapIter;
        for (ParamsMapIter = qstrLightValueMap.constBegin(); ParamsMapIter != qstrLightValueMap.constEnd(); ++ParamsMapIter)
        {
            //写入新内容，原来没有就插入，原来存在就覆盖
            iGreenComponuntList.append(ParamsMapIter.value().toInt());
        }
        return true;
    }
    else
    {
        return false;
    }
}

void StandardBrightness::SlotSaveBrightnessValue()
{
    const QString strFileName = QCoreApplication::applicationDirPath() + "/Resources/DrugDetectionMachineParams.json";
    const QString strParamsType = "StandardMachineCalibrate";
    SaveStandardParams(strFileName,strParamsType,m_brightnessValue,m_iGreenComponuntList);
}
