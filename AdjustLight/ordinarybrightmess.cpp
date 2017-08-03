#include "OrdinaryBrightmess.h"
#include <QApplication>
#include <QTime>
#include <QFile>
#include "ParamsConfiguration.h"
#include "calculategrayscale.h"
#include "CHidCmdThread.h"
#include "protocolutility.h"
#include "hidopertaionutility.h"
#include "opencvutility.h"
#include "linefinder.h"

OrdinaryBrightmess::OrdinaryBrightmess()
{

}

void OrdinaryBrightmess::SlotOrdinaryCalibration()
{
    BrightnessValue StandardbrightnessValue;
    StandardbrightnessValue.iBrightNo1 = 300;
    StandardbrightnessValue.iBrightNo2 = 300;
    StandardbrightnessValue.iBrightNo3 = 300;
    StandardbrightnessValue.iBrightNo4 = 300;
    StandardbrightnessValue.iBrightNo5 = 300;
    StandardbrightnessValue.iBrightNo6 = 300;
    StandardbrightnessValue.iBrightNo7 = 300;
    StandardbrightnessValue.iBrightNo8 = 300;

    StandardbrightnessValue.iGreenComponentNo1 = 200;
    StandardbrightnessValue.iGreenComponentNo2 = 200;
    StandardbrightnessValue.iGreenComponentNo3 = 200;
    StandardbrightnessValue.iGreenComponentNo4 = 200;
    StandardbrightnessValue.iGreenComponentNo5 = 200;
    StandardbrightnessValue.iGreenComponentNo6 = 200;
    StandardbrightnessValue.iGreenComponentNo7 = 200;
    StandardbrightnessValue.iGreenComponentNo8 = 200;

    SetBrightnessValue(StandardbrightnessValue);
    BrightnessValue brightnessValue;
    OrdinaryCalibration(&brightnessValue);

    qDebug() << "brightnessValue.iBrightNo1 = "<< brightnessValue.iBrightNo1;
    qDebug() << "brightnessValue.iBrightNo2 = " << brightnessValue.iBrightNo2;
    qDebug() << "brightnessValue.iBrightNo3 = " << brightnessValue.iBrightNo3;
    qDebug() << "brightnessValue.iBrightNo4 = " << brightnessValue.iBrightNo4;
    qDebug() << "brightnessValue.iBrightNo5 = " << brightnessValue.iBrightNo5;
    qDebug() << "brightnessValue.iBrightNo6 = " << brightnessValue.iBrightNo6;
    qDebug() << "brightnessValue.iBrightNo7 = " << brightnessValue.iBrightNo7;
    qDebug() << "brightnessValue.iBrightNo8 = " << brightnessValue.iBrightNo8;

    qDebug() << "brightnessValue.iGreenComponentNo1 = " << brightnessValue.iGreenComponentNo1;
    qDebug() << "brightnessValue.iGreenComponentNo2 = " << brightnessValue.iGreenComponentNo2;
    qDebug() << "brightnessValue.iGreenComponentNo3 = " << brightnessValue.iGreenComponentNo3;
    qDebug() << "brightnessValue.iGreenComponentNo4 = " << brightnessValue.iGreenComponentNo4;
    qDebug() << "brightnessValue.iGreenComponentNo5 = " << brightnessValue.iGreenComponentNo5;
    qDebug() << "brightnessValue.iGreenComponentNo6 = " << brightnessValue.iGreenComponentNo6;
    qDebug() << "brightnessValue.iGreenComponentNo7 = " << brightnessValue.iGreenComponentNo7;
    qDebug() << "brightnessValue.iGreenComponentNo8 = " << brightnessValue.iGreenComponentNo8;
    emit SignalBrightnessValueToUI(brightnessValue);
}

QPoint OrdinaryBrightmess::findCenterPoint(QString strImagePath)
{
    LineFinder lineFinder;
    LocationData locationData = lineFinder.findCenterPointAndCrossPoints(strImagePath);
    qDebug() << "centerPoint.x = " << locationData.centerPoint.x;
    qDebug() << "centerPoint.y = " << locationData.centerPoint.y;
    QPoint qCenterPoint;
    qCenterPoint.setX(locationData.centerPoint.y + 650);
    qCenterPoint.setY(locationData.centerPoint.x);
    qDebug() << "qCenterPoint = " << qCenterPoint;
    return qCenterPoint;
}

void OrdinaryBrightmess::OrdinaryCalibration(BrightnessValue *brightnessValue)
{
    QPoint qCenterPoint;
    InitMachine(&qCenterPoint);

    if(GetBrightnessValue().iBrightNo1 > 0)
    {
        GetLightValue(1,qCenterPoint,GetBrightnessValue().iBrightNo1,GetBrightnessValue().iGreenComponentNo1,
                      &(brightnessValue->iBrightNo1),&(brightnessValue->iGreenComponentNo1));
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_CLOSE_ALL_LED);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    if(GetBrightnessValue().iBrightNo2 > 0)
    {
        GetLightValue(2,qCenterPoint,GetBrightnessValue().iBrightNo2,GetBrightnessValue().iGreenComponentNo2,
                      &(brightnessValue->iBrightNo2),&(brightnessValue->iGreenComponentNo2));
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_CLOSE_ALL_LED);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    if(GetBrightnessValue().iBrightNo3 > 0)
    {
        GetLightValue(3,qCenterPoint,GetBrightnessValue().iBrightNo3,GetBrightnessValue().iGreenComponentNo3,
                      &(brightnessValue->iBrightNo3),&(brightnessValue->iGreenComponentNo3));
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_CLOSE_ALL_LED);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    if(GetBrightnessValue().iBrightNo4 > 0)
    {
        GetLightValue(4,qCenterPoint,GetBrightnessValue().iBrightNo4,GetBrightnessValue().iGreenComponentNo4,
                      &(brightnessValue->iBrightNo4),&(brightnessValue->iGreenComponentNo4));
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_CLOSE_ALL_LED);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    if(GetBrightnessValue().iBrightNo5 > 0)
    {
        GetLightValue(5,qCenterPoint,GetBrightnessValue().iBrightNo5,GetBrightnessValue().iGreenComponentNo5,
                     &(brightnessValue->iBrightNo5),&(brightnessValue->iGreenComponentNo5));
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_CLOSE_ALL_LED);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    if(GetBrightnessValue().iBrightNo6 > 0)
    {
        GetLightValue(6,qCenterPoint,GetBrightnessValue().iBrightNo6,GetBrightnessValue().iGreenComponentNo6,
                      &(brightnessValue->iBrightNo6),&(brightnessValue->iGreenComponentNo6));
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_CLOSE_ALL_LED);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    if(GetBrightnessValue().iBrightNo7 > 0)
    {
        GetLightValue(7,qCenterPoint,GetBrightnessValue().iBrightNo7,GetBrightnessValue().iGreenComponentNo7,
                      &(brightnessValue->iBrightNo7),&(brightnessValue->iGreenComponentNo7));
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_CLOSE_ALL_LED);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    if(GetBrightnessValue().iBrightNo8 > 0)
    {
        GetLightValue(8,qCenterPoint,GetBrightnessValue().iBrightNo8,GetBrightnessValue().iGreenComponentNo8,
                      &(brightnessValue->iBrightNo8),&(brightnessValue->iGreenComponentNo8));
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_CLOSE_ALL_LED);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
}

bool OrdinaryBrightmess::InitMachine(QPoint *CenterPoint)
{
    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_CLOSE_ALL_LED);
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
    OpencvUtility::getInstance()->QuickGetVideoCapture(&strSaveImagePath);//丢弃
    OpencvUtility::getInstance()->QuickGetVideoCapture(&strSaveImagePath);

    *CenterPoint = findCenterPoint(strSaveImagePath);

    if(QFile::exists(strSaveImagePath))
    {
        Mat matImage = imread(strSaveImagePath.toLatin1().data(),-1);
        cvNamedWindow( "donkeyaime", CV_WINDOW_AUTOSIZE );
        CalculateGrayscale calculategrayscale;
        for(int i = 0;i < 4;i++)
        {
           calculategrayscale.drawRect(matImage,*CenterPoint,30,30,i);
        }
        imshow("donkeyaime",matImage);
        waitKey(0);
    }

    return true;
}

BrightnessValue OrdinaryBrightmess::GetBrightnessValue()
{
    return m_brightnessValue;
}

void OrdinaryBrightmess::SetBrightnessValue(BrightnessValue brightnessValue)
{
    m_brightnessValue = brightnessValue;
}

bool OrdinaryBrightmess::GetLightValue(const int iBrightNo,QPoint qCenterPoint,const qint16 /*iStandardBright*/,
                                       const double dStandardGreenComponent,qint16 *iOrdinaryBright,
                                       double *dOrdinaryGreenComponent)
{
    QString strFileName = "grayscale%1.csv";
    QFile file(strFileName.arg(iBrightNo));
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      qDebug() << "Cannot open file for writing";
      return false;
    }
    QTextStream out(&file);

    CalculateGrayscale calculategrayscale;

    const int iRectR = 30;
    const int iRectD = 40;

    //QPoint point(1180,1024);

    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_CLOSE_ALL_LED);
    QThread::msleep(100);

    QMap<double,int> qGraySDSumMap;
    qGraySDSumMap.clear();

    QMap<double,double> qGreenComponentMap;
    qGreenComponentMap.clear();

    const int iMaxCount = 200;
    int iSeptCount = 40000 / iMaxCount;
    const int iStartLightValue = 500;
    int iPosLightValue = 0;

    double dAvg = 0.0;
    double dStandardSD = 0.0;
    double dGreenSDSum = 0.0;

    QString strSaveImagePath;

    for(int i = 0;i < iMaxCount;i++)
    {
        dAvg = 0.0;
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
        //上一帧
        OpencvUtility::getInstance()->QuickGetVideoCapture(&strSaveImagePath);

        out << iPosLightValue << ",";

        /*计算绿色分量*/
        for(int j = 0;j < 4;j++)
        {
            calculategrayscale.GetGreenComponentSDAvg(strSaveImagePath,qCenterPoint,iRectR,iRectD,j,
                                                      dStandardGreenComponent,&dAvg,&dStandardSD);
            dGreenSDSum += dStandardSD;
            dAvg += dAvg;
        }
        out << dGreenSDSum << ",";
        out <<  "\n";

        qGraySDSumMap.insert(dGreenSDSum,iPosLightValue);
        qGreenComponentMap.insert(dGreenSDSum,dAvg/4);
    }
    //得到标准差最小的灯光值
    *iOrdinaryBright = qGraySDSumMap.begin().value();
    //得到标准差最小的绿色分量值
    *dOrdinaryGreenComponent = qGreenComponentMap.begin().value();
    file.close();
    return true;
}
bool OrdinaryBrightmess::SaveBrightnessValueParams(QString strFileName,QString ParamsType,
                                                   BrightnessValue brightnessValue)
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
                                                   BrightnessValue *brightnessValue)
{
    QMap<QString,QVariant> ParamsMap;
    if(ParamsConfiguration::getInstance()->ReadParamsFromConfigFile(strFileName,ParamsType,&ParamsMap))
    {
        brightnessValue->iBrightNo1 = ParamsMap.value("iBrightNo1").toInt();
        brightnessValue->iBrightNo2 = ParamsMap.value("iBrightNo2").toInt();
        brightnessValue->iBrightNo3 = ParamsMap.value("iBrightNo3").toInt();
        brightnessValue->iBrightNo4 = ParamsMap.value("iBrightNo4").toInt();
        brightnessValue->iBrightNo5 = ParamsMap.value("iBrightNo5").toInt();
        brightnessValue->iBrightNo6 = ParamsMap.value("iBrightNo6").toInt();
        brightnessValue->iBrightNo7 = ParamsMap.value("iBrightNo7").toInt();
        brightnessValue->iBrightNo8 = ParamsMap.value("iBrightNo8").toInt();

        brightnessValue->iGreenComponentNo1 = ParamsMap.value("iGreenComponentNo2").toInt();
        brightnessValue->iGreenComponentNo2 = ParamsMap.value("iGreenComponentNo2").toInt();
        brightnessValue->iGreenComponentNo3 = ParamsMap.value("iGreenComponentNo3").toInt();
        brightnessValue->iGreenComponentNo4 = ParamsMap.value("iGreenComponentNo4").toInt();
        brightnessValue->iGreenComponentNo5 = ParamsMap.value("iGreenComponentNo5").toInt();
        brightnessValue->iGreenComponentNo6 = ParamsMap.value("iGreenComponentNo6").toInt();
        brightnessValue->iGreenComponentNo7 = ParamsMap.value("iGreenComponentNo7").toInt();
        brightnessValue->iGreenComponentNo8 = ParamsMap.value("iGreenComponentNo8").toInt();
        return true;
    }
    else
    {
        return false;
    }
}
