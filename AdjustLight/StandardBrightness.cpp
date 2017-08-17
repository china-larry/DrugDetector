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


/**
 * @brief SignalSendPictureToUI
 * 仪器灯光值槽
 * @param brightnessValue 灯光值结构体
 *
 * @return
 */
void StandardBrightness::SlotGetBrightValue(BrightnessValue sBrightnessValue)
{
    SetBrightness(sBrightnessValue);
    //设置灯光
    if(SetBrightnessValue(sBrightnessValue) == true)
    {
        //获取图片
        QString strSaveImagePath = "";
        if(OpencvUtility::GetInstance()->GetVideoCapture(&strSaveImagePath) == true)
        {
            //获取绿色分量
            GetGreenComponunt(sBrightnessValue.iCupType,strSaveImagePath);
            if(!m_iGreenComponuntList.isEmpty())
            {
                //发送给UI显示
                emit SignalSendPictureToUI(strSaveImagePath,m_iGreenComponuntList);
                return;
            }
            //顺时针转 15 * 10 步
            for(qint16 iStep1 = 0; iStep1 < 15; iStep1++)
            {
                HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
                CHidCmdThread::GetInstance()->AddRotateMotorCmd(20,10,0);

                while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
                {
                    QApplication::processEvents();
                }

                if(OpencvUtility::GetInstance()->GetVideoCapture(&strSaveImagePath) == true)
                {
                    //获取绿色分量
                    GetGreenComponunt(sBrightnessValue.iCupType,strSaveImagePath);
                    if(!m_iGreenComponuntList.isEmpty())
                    {
                        //发送给UI显示
                        emit SignalSendPictureToUI(strSaveImagePath,m_iGreenComponuntList);
                        return;
                    }
                }

            }

            //逆时针转 30 * 10 步
            for(qint16 iStep2 = 0; iStep2 < 30; iStep2++)
            {
                HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
                CHidCmdThread::GetInstance()->AddRotateMotorCmd(20,10,1);

                while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
                {
                    QApplication::processEvents();
                }

                if(OpencvUtility::GetInstance()->GetVideoCapture(&strSaveImagePath) == true)
                {
                    //获取绿色分量
                    GetGreenComponunt(sBrightnessValue.iCupType,strSaveImagePath);
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
}

void StandardBrightness::SetBrightness(BrightnessValue sBrightnessValue)
{
    m_brightnessValue = sBrightnessValue;
}

BrightnessValue StandardBrightness::GetBrightness()
{
    return m_brightnessValue;
}


/**
 * @brief SetBrightness
 * 设置灯光值
 * @param BrightnessValue 灯光值结构体

 * @return
 */
bool StandardBrightness::SetBrightnessValue(BrightnessValue sBrightnessValue)
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
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdCloseAllLed);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    //motorReset
    CHidCmdThread::GetInstance()->AddResetMotorCmd(10);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    //等待电机复位
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    CHidCmdThread::GetInstance()->AddRotateMotorCmd(10,1024,1);

    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    CHidCmdThread::GetInstance()->AddOpenLedCmd(1,sBrightnessValue.iNo1);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    CHidCmdThread::GetInstance()->AddOpenLedCmd(2,sBrightnessValue.iNo2);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    CHidCmdThread::GetInstance()->AddOpenLedCmd(3,sBrightnessValue.iNo3);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    CHidCmdThread::GetInstance()->AddOpenLedCmd(4,sBrightnessValue.iNo4);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
    CHidCmdThread::GetInstance()->AddOpenLedCmd(5,sBrightnessValue.iNo5);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
    CHidCmdThread::GetInstance()->AddOpenLedCmd(6,sBrightnessValue.iNo6);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
    CHidCmdThread::GetInstance()->AddOpenLedCmd(7,sBrightnessValue.iNo7);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
    CHidCmdThread::GetInstance()->AddOpenLedCmd(8,sBrightnessValue.iNo8);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
    return true;
}

/**
 * @brief GetGreenComponunt
 * 获取绿色分量曲线
 * @param
 *        iCupType         杯型信息
 *        strSaveImagePath 输入图片路径
 *        dGreenComponuntList 输出绿色分量数据
 * @return
 */
bool StandardBrightness::GetGreenComponunt(qint16 iCupType,const QString strSaveImagePath)
{
    ThreadTesting threadTesting;
    m_iGreenComponuntList.clear();
    if(0 == iCupType)
    {
        //T-cup
        m_iGreenComponuntList = threadTesting.GetComponentGreenTCup(strSaveImagePath);
    }
    else if(4 == iCupType)
    {
        //S-cup
         m_iGreenComponuntList = threadTesting.GetComponentGreenSCup(strSaveImagePath);
    }
    if(!m_iGreenComponuntList.isEmpty())
    {
        return true;
    }

    return false;
}

/**
 * @brief GetGreenComponunt
 * 保存到配置文件
 * @param brightnessValue 灯光值结构体
 *        dGreenComponuntList 绿色分量数据
 * @return
 */
bool StandardBrightness::SaveStandardParams(QString strFileName,QString strParamsType,BrightnessValue sBrightnessValue,
                        QList<int> iGreenComponuntList)
{
    QMap<QString,QVariant> strSaveDataMap;
    strSaveDataMap.insert("No1",sBrightnessValue.iNo1);
    strSaveDataMap.insert("No2",sBrightnessValue.iNo2);
    strSaveDataMap.insert("No3",sBrightnessValue.iNo3);
    strSaveDataMap.insert("No4",sBrightnessValue.iNo4);
    strSaveDataMap.insert("No5",sBrightnessValue.iNo5);
    strSaveDataMap.insert("No6",sBrightnessValue.iNo6);
    strSaveDataMap.insert("No7",sBrightnessValue.iNo7);
    strSaveDataMap.insert("No8",sBrightnessValue.iNo8);
    strSaveDataMap.insert("CupType",sBrightnessValue.iCupType);
    QMap<QString,QVariant> qLightValueMap;
    for(int i = 0;i < iGreenComponuntList.count();i++)
    {
        qLightValueMap.insert(QString::number(i),iGreenComponuntList.at(i));
    }
    strSaveDataMap.insert("GreenComponunt",qLightValueMap);
    if(ParamsConfiguration::GetInstance()->SaveParamsToConfigFile(strFileName,strParamsType,strSaveDataMap))
    {
        return true;
    }
    else
    {
        return false;
    }
}


/**
 * @brief GetGreenComponunt
 * 从配置文件读取参数
 * @param brightnessValue 灯光值结构体
 *        dGreenComponuntList 绿色分量数据
 * @return
 */
bool StandardBrightness::ReadStandardParams(QString strFileName,QString strParamsType,BrightnessValue &sBrightnessValue,
                        QList<int> &iGreenComponuntList)
{
    QMap<QString,QVariant> qParamsMap;
    if(ParamsConfiguration::GetInstance()->ReadParamsFromConfigFile(strFileName,strParamsType,&qParamsMap))
    {
        sBrightnessValue.iNo1 = qParamsMap.value("No1").toInt();
        sBrightnessValue.iNo2 = qParamsMap.value("No2").toInt();
        sBrightnessValue.iNo3 = qParamsMap.value("No3").toInt();
        sBrightnessValue.iNo4 = qParamsMap.value("No4").toInt();
        sBrightnessValue.iNo5 = qParamsMap.value("No5").toInt();
        sBrightnessValue.iNo6 = qParamsMap.value("No6").toInt();
        sBrightnessValue.iNo7 = qParamsMap.value("No7").toInt();
        sBrightnessValue.iNo8 = qParamsMap.value("No8").toInt();
        sBrightnessValue.iCupType = qParamsMap.value("CupType").toInt();

        QMap<QString,QVariant> qstrLightValueMap = qParamsMap.value("GreenComponunt").toMap();

        QMap<QString,QVariant>::const_iterator qParamsMapIter;
        for (qParamsMapIter = qstrLightValueMap.constBegin(); qParamsMapIter != qstrLightValueMap.constEnd();
             ++qParamsMapIter)
        {
            //写入新内容，原来没有就插入，原来存在就覆盖
            iGreenComponuntList.append(qParamsMapIter.value().toInt());
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
