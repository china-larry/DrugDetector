#include "OrdinaryBrightmess.h"
#include <QApplication>
#include <QTime>
#include <QFile>
#include "ParamsConfiguration.h"
#include "CalculateGreenComponent.h"
#include "CHidCmdThread.h"
#include "ProtocolUtility.h"
#include "HidOpertaionUtility.h"
#include "OpencvUtility.h"
#include "LineFinder.h"

OrdinaryBrightmess::OrdinaryBrightmess()
{

}

OrdinaryBrightmess::~OrdinaryBrightmess()
{

}

void OrdinaryBrightmess::SlotOrdinaryCalibration()
{
    //设置标准灯光
    SetBrightnessValue(m_StandardMachinebrightnessValue);
    //目标机灯光校准
    OrdinaryCalibration(m_OrdinarybrightnessValue);
    //发送信号到UI更新
    emit SignalCalibrationValueToUI(m_OrdinarybrightnessValue);
}

void OrdinaryBrightmess::SlotOrdinaryImport()
{
    const QString strFileName = QCoreApplication::applicationDirPath() + "/Resources/DrugDetectionMachineParams.json";
    const QString strParamsType = "StandardMachineCalibrationParams";

    ReadBrightnessValueParams(strFileName,strParamsType, m_StandardMachinebrightnessValue);

    emit SignalImportValueToUI(m_StandardMachinebrightnessValue);
}

void OrdinaryBrightmess::SlotOrdinarySave()
{
    //保存参数
    const QString strFileName = QCoreApplication::applicationDirPath() + "/Resources/DrugDetectionMachineParams.json";
    const QString strOrdinaryParamsType = "OrdinaryMachinebrightnesCalibrate";
    SaveBrightnessValueParams(strFileName,strOrdinaryParamsType,m_OrdinarybrightnessValue);
}

void OrdinaryBrightmess::SlotOrdinaryRead()
{
    //读取参数
    const QString strFileName = QCoreApplication::applicationDirPath() + "/Resources/DrugDetectionMachineParams.json";
    const QString strParamsType = "OrdinaryMachinebrightnesCalibrate";

    BrightnessOrdinaryValue brightnessOrdinaryValue;
    ReadBrightnessValueParams(strFileName,strParamsType, brightnessOrdinaryValue);
    //发送信号给UI更新
    emit SignalReadValueToUI(brightnessOrdinaryValue);
}

QPoint OrdinaryBrightmess::findCenterPoint(QString strImagePath)
{
    LineFinder lineFinder;
    LocationData locationData = lineFinder.findCenterPointAndCrossPoints(strImagePath);

    QPoint qCenterPoint;
    qCenterPoint.setX(locationData.centerPoint.y + 620);
    qCenterPoint.setY(locationData.centerPoint.x);
    qDebug() << "qCenterPoint = " << qCenterPoint;
    return qCenterPoint;
}

void OrdinaryBrightmess::OrdinaryCalibration(BrightnessOrdinaryValue &brightnessValue)
{
    QPoint qCenterPoint;
    InitMachine(qCenterPoint);

    if(GetBrightnessValue().iBrightNo1 > 0)
    {
        GetLightValue(1,qCenterPoint,GetBrightnessValue().iBrightNo1,GetBrightnessValue().iGreenComponentNo1,
                      brightnessValue.iBrightNo1,brightnessValue.iGreenComponentNo1);
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::s_iCmdCloseAllLed);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    if(GetBrightnessValue().iBrightNo2 > 0)
    {
        GetLightValue(2,qCenterPoint,GetBrightnessValue().iBrightNo2,GetBrightnessValue().iGreenComponentNo2,
                      brightnessValue.iBrightNo2,brightnessValue.iGreenComponentNo2);
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::s_iCmdCloseAllLed);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    if(GetBrightnessValue().iBrightNo3 > 0)
    {
        GetLightValue(3,qCenterPoint,GetBrightnessValue().iBrightNo3,GetBrightnessValue().iGreenComponentNo3,
                      brightnessValue.iBrightNo3,brightnessValue.iGreenComponentNo3);
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::s_iCmdCloseAllLed);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    if(GetBrightnessValue().iBrightNo4 > 0)
    {
        GetLightValue(4,qCenterPoint,GetBrightnessValue().iBrightNo4,GetBrightnessValue().iGreenComponentNo4,
                      brightnessValue.iBrightNo4,brightnessValue.iGreenComponentNo4);
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::s_iCmdCloseAllLed);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    if(GetBrightnessValue().iBrightNo5 > 0)
    {
        GetLightValue(5,qCenterPoint,GetBrightnessValue().iBrightNo5,GetBrightnessValue().iGreenComponentNo5,
                     brightnessValue.iBrightNo5,brightnessValue.iGreenComponentNo5);
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::s_iCmdCloseAllLed);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    if(GetBrightnessValue().iBrightNo6 > 0)
    {
        GetLightValue(6,qCenterPoint,GetBrightnessValue().iBrightNo6,GetBrightnessValue().iGreenComponentNo6,
                      brightnessValue.iBrightNo6,brightnessValue.iGreenComponentNo6);
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::s_iCmdCloseAllLed);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    if(GetBrightnessValue().iBrightNo7 > 0)
    {
        GetLightValue(7,qCenterPoint,GetBrightnessValue().iBrightNo7,GetBrightnessValue().iGreenComponentNo7,
                      brightnessValue.iBrightNo7,brightnessValue.iGreenComponentNo7);
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::s_iCmdCloseAllLed);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    if(GetBrightnessValue().iBrightNo8 > 0)
    {
        GetLightValue(8,qCenterPoint,GetBrightnessValue().iBrightNo8,GetBrightnessValue().iGreenComponentNo8,
                      brightnessValue.iBrightNo8,brightnessValue.iGreenComponentNo8);
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::s_iCmdCloseAllLed);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
}

bool OrdinaryBrightmess::InitMachine(QPoint &CenterPoint)
{
    qDebug() << "InitMachine";

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
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::s_iCmdCloseAllLed);

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

    //唾液杯(Cube-Cup) 机器转到测试窗口位置
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    CHidCmdThread::GetInstance()->AddRotateMotorCmd(10,1024,0);

    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    CHidCmdThread::GetInstance()->AddOpenLedCmd(2,10000);
    /*等待灯光稳定*/
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
    QThread::msleep(10);

    QString strSaveImagePath = "";
    OpencvUtility::getInstance()->GetVideoCapture(&strSaveImagePath);

    CenterPoint = findCenterPoint(strSaveImagePath);

    if(QFile::exists(strSaveImagePath))
    {
        Mat matImage = imread(strSaveImagePath.toLatin1().data(),-1);

        CalculateGreenComponent calculateGreenComponent;
        for(int i = 0;i < 4;i++)
        {
           calculateGreenComponent.drawRect(matImage,CenterPoint,30,40,i);
        }
        cv::namedWindow( "donkeyaime", cv::WINDOW_NORMAL );
        imshow("donkeyaime",matImage);
        waitKey(0);
    }

    return true;
}

BrightnessOrdinaryValue OrdinaryBrightmess::GetBrightnessValue()
{
    return m_OrdinarybrightnessValue;
}

void OrdinaryBrightmess::SetBrightnessValue(BrightnessOrdinaryValue brightnessValue)
{
    m_OrdinarybrightnessValue = brightnessValue;
}

bool OrdinaryBrightmess::GetLightValue(const int iBrightNo,QPoint qCenterPoint,const int /*iStandardBright*/,
                                       const double dStandardGreenComponent,int &iOrdinaryBright,
                                       double &dOrdinaryGreenComponent)
{
    QString strFileName = "grayscale%1.csv";
    QFile file(strFileName.arg(iBrightNo));
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      qDebug() << "Cannot open file for writing";
      return false;
    }
    QTextStream out(&file);

    CalculateGreenComponent calculateGreenComponent;

    const int iRectR = 30;
    const int iRectD = 40;

    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::s_iCmdCloseAllLed);
    QThread::msleep(100);

    QMap<double,int> qGraySDSumMap;
    qGraySDSumMap.clear();

    QMap<double,double> qGreenComponentMap;
    qGreenComponentMap.clear();

    const int iMaxCount = 200;
    int iSeptCount = 40000 / iMaxCount;
    const int iStartLightValue = 500;
    int iPosLightValue = 0;

    double dAvgSum = 0.0;
    double dStandardSD = 0.0;
    double dGreenSDSum = 0.0;

    QString strSaveImagePath;

    for(int i = 0;i < iMaxCount;i++)
    {
        dAvgSum = 0.0;
        dStandardSD = 0.0;
        dGreenSDSum = 0.0;
        HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);

        /* 等量增加灯光值*/
        iPosLightValue = iStartLightValue + i * iSeptCount;
        /*开灯*/
        CHidCmdThread::GetInstance()->AddOpenLedCmd(iBrightNo,iPosLightValue);
        /*等待灯光稳定*/
        while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
        {
            QApplication::processEvents();
        }
        QThread::msleep(10);

        /*拍照*/
        OpencvUtility::getInstance()->GetVideoCapture(&strSaveImagePath);

        out << iPosLightValue << ",";

        /*计算绿色分量*/
        double dAvg = 0.0;
        for(int j = 0;j < 4;j++)
        {
            dAvg = 0.0;
            calculateGreenComponent.GetGreenComponentSDAvg(strSaveImagePath,qCenterPoint,iRectR,iRectD,j,
                                                      dStandardGreenComponent,dAvg,dStandardSD);
            dGreenSDSum += dStandardSD;
            dAvgSum += dAvg;
        }
        out << dGreenSDSum << ",";
        out <<( dAvgSum/4) << ",";
        out <<  "\n";

        qGraySDSumMap.insert(dGreenSDSum,iPosLightValue);
        qGreenComponentMap.insert(dGreenSDSum,dAvgSum/4);
    }
    //得到标准差最小的灯光值
    iOrdinaryBright = qGraySDSumMap.begin().value();
    //得到标准差最小的绿色分量均值
    dOrdinaryGreenComponent = qGreenComponentMap.begin().value();
    file.close();
    return true;
}
bool OrdinaryBrightmess::SaveBrightnessValueParams(QString strFileName,QString ParamsType,
                                                   BrightnessOrdinaryValue brightnessValue)
{
    QMap<QString,QVariant> strSaveDataMap;
    strSaveDataMap.insert("iBrightNo1",brightnessValue.iBrightNo1);
    strSaveDataMap.insert("iBrightNo2",brightnessValue.iBrightNo2);
    strSaveDataMap.insert("iBrightNo3",brightnessValue.iBrightNo3);
    strSaveDataMap.insert("iBrightNo4",brightnessValue.iBrightNo4);
    strSaveDataMap.insert("iBrightNo5",brightnessValue.iBrightNo5);
    strSaveDataMap.insert("iBrightNo6",brightnessValue.iBrightNo6);
    strSaveDataMap.insert("iBrightNo7",brightnessValue.iBrightNo7);
    strSaveDataMap.insert("iBrightNo8",brightnessValue.iBrightNo8);

    strSaveDataMap.insert("iGreenComponentNo1",brightnessValue.iGreenComponentNo1);
    strSaveDataMap.insert("iGreenComponentNo2",brightnessValue.iGreenComponentNo2);
    strSaveDataMap.insert("iGreenComponentNo3",brightnessValue.iGreenComponentNo3);
    strSaveDataMap.insert("iGreenComponentNo4",brightnessValue.iGreenComponentNo4);
    strSaveDataMap.insert("iGreenComponentNo5",brightnessValue.iGreenComponentNo5);
    strSaveDataMap.insert("iGreenComponentNo6",brightnessValue.iGreenComponentNo6);
    strSaveDataMap.insert("iGreenComponentNo7",brightnessValue.iGreenComponentNo7);
    strSaveDataMap.insert("iGreenComponentNo8",brightnessValue.iGreenComponentNo8);

    if(ParamsConfiguration::getInstance()->SaveParamsToConfigFile(strFileName,ParamsType,strSaveDataMap))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool OrdinaryBrightmess::ReadBrightnessValueParams(QString strFileName,QString ParamsType,
                                                   BrightnessOrdinaryValue &brightnessValue)
{
    QMap<QString,QVariant> ParamsMap;
    if(ParamsConfiguration::getInstance()->ReadParamsFromConfigFile(strFileName,ParamsType,&ParamsMap))
    {
        brightnessValue.iBrightNo1 = ParamsMap.value("iBrightNo1").toInt();
        brightnessValue.iBrightNo2 = ParamsMap.value("iBrightNo2").toInt();
        brightnessValue.iBrightNo3 = ParamsMap.value("iBrightNo3").toInt();
        brightnessValue.iBrightNo4 = ParamsMap.value("iBrightNo4").toInt();
        brightnessValue.iBrightNo5 = ParamsMap.value("iBrightNo5").toInt();
        brightnessValue.iBrightNo6 = ParamsMap.value("iBrightNo6").toInt();
        brightnessValue.iBrightNo7 = ParamsMap.value("iBrightNo7").toInt();
        brightnessValue.iBrightNo8 = ParamsMap.value("iBrightNo8").toInt();

        brightnessValue.iGreenComponentNo1 = ParamsMap.value("iGreenComponentNo2").toInt();
        brightnessValue.iGreenComponentNo2 = ParamsMap.value("iGreenComponentNo2").toInt();
        brightnessValue.iGreenComponentNo3 = ParamsMap.value("iGreenComponentNo3").toInt();
        brightnessValue.iGreenComponentNo4 = ParamsMap.value("iGreenComponentNo4").toInt();
        brightnessValue.iGreenComponentNo5 = ParamsMap.value("iGreenComponentNo5").toInt();
        brightnessValue.iGreenComponentNo6 = ParamsMap.value("iGreenComponentNo6").toInt();
        brightnessValue.iGreenComponentNo7 = ParamsMap.value("iGreenComponentNo7").toInt();
        brightnessValue.iGreenComponentNo8 = ParamsMap.value("iGreenComponentNo8").toInt();
        return true;
    }
    else
    {
        return false;
    }
}
