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
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdCloseAllLed);
}

void OrdinaryBrightmess::SlotOrdinaryCalibration()
{
    //设置标准灯光
    SetBrightnessValue(m_sStandardMachinebrightnessValue);
    //目标机灯光校准
    OrdinaryCalibration(m_sOrdinarybrightnessValue);

    //发送信号到UI更新
    emit SignalCalibrationValueToUI(m_sOrdinarybrightnessValue);

    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdCloseAllLed);

}

void OrdinaryBrightmess::SlotOrdinaryImport()
{
    const QString kstrFileName = QCoreApplication::applicationDirPath()
                              + "/Resources/DrugDetectionMachineParams.json";
    const QString kstrParamsType = "StandardMachineCalibrationParams";

    //读取配置文件
    ReadBrightnessValueParams(kstrFileName,kstrParamsType, m_sStandardMachinebrightnessValue);

    //发送到UI显示
    emit SignalImportValueToUI(m_sStandardMachinebrightnessValue);
}

void OrdinaryBrightmess::SlotOrdinarySave()
{
    //保存参数
    const QString kstrFileName = QCoreApplication::applicationDirPath() + "/Resources/DrugDetectionMachineParams.json";
    const QString kstrOrdinaryParamsType = "OrdinaryMachinebrightnesCalibrate";
    SaveBrightnessValueParams(kstrFileName,kstrOrdinaryParamsType,m_sOrdinarybrightnessValue);
}

void OrdinaryBrightmess::SlotOrdinaryRead()
{
    //读取参数
    const QString kstrFileName = QCoreApplication::applicationDirPath() + "/Resources/DrugDetectionMachineParams.json";
    const QString kstrParamsType = "OrdinaryMachinebrightnesCalibrate";

    BrightnessOrdinaryValue sBrightnessOrdinaryValue;
    ReadBrightnessValueParams(kstrFileName,kstrParamsType, sBrightnessOrdinaryValue);
    //发送信号给UI更新
    emit SignalReadValueToUI(sBrightnessOrdinaryValue);
}

/**
* @brief  findCenterPoint
* 定位色块
* @param   strImagePath 图片路径
* @return  QPoint 色块坐标
*/
QPoint OrdinaryBrightmess::findCenterPoint(QString strImagePath)
{
    LineFinder lineFinder;
    LocationData sLocationData = lineFinder.findCenterPointAndCrossPoints(strImagePath);

    QPoint qCenterPoint;
    qCenterPoint.setX(sLocationData.centerPoint.y + 620);
    qCenterPoint.setY(sLocationData.centerPoint.x);
    qDebug() << "qCenterPoint = " << qCenterPoint;
    return qCenterPoint;
}

/**
* @brief  OrdinaryCalibration 对目标机型进行亮度校准
* @param  brightnessValue  目标机型亮度结构体
* @return
*/
void OrdinaryBrightmess::OrdinaryCalibration(BrightnessOrdinaryValue &sBrightnessValue)
{
    QPoint qCenterPoint;
    InitMachine(qCenterPoint);

    if(GetBrightnessValue().iBrightNo1 > 0)
    {
        GetLightValue(1,qCenterPoint,GetBrightnessValue().iBrightNo1,GetBrightnessValue().iGreenComponentNo1,
                      sBrightnessValue.iBrightNo1,sBrightnessValue.iGreenComponentNo1);
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdCloseAllLed);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    if(GetBrightnessValue().iBrightNo2 > 0)
    {
        GetLightValue(2,qCenterPoint,GetBrightnessValue().iBrightNo2,GetBrightnessValue().iGreenComponentNo2,
                      sBrightnessValue.iBrightNo2,sBrightnessValue.iGreenComponentNo2);
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdCloseAllLed);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    if(GetBrightnessValue().iBrightNo3 > 0)
    {
        GetLightValue(3,qCenterPoint,GetBrightnessValue().iBrightNo3,GetBrightnessValue().iGreenComponentNo3,
                      sBrightnessValue.iBrightNo3,sBrightnessValue.iGreenComponentNo3);
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdCloseAllLed);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    if(GetBrightnessValue().iBrightNo4 > 0)
    {
        GetLightValue(4,qCenterPoint,GetBrightnessValue().iBrightNo4,GetBrightnessValue().iGreenComponentNo4,
                      sBrightnessValue.iBrightNo4,sBrightnessValue.iGreenComponentNo4);
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdCloseAllLed);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    if(GetBrightnessValue().iBrightNo5 > 0)
    {
        GetLightValue(5,qCenterPoint,GetBrightnessValue().iBrightNo5,GetBrightnessValue().iGreenComponentNo5,
                     sBrightnessValue.iBrightNo5,sBrightnessValue.iGreenComponentNo5);
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdCloseAllLed);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    if(GetBrightnessValue().iBrightNo6 > 0)
    {
        GetLightValue(6,qCenterPoint,GetBrightnessValue().iBrightNo6,GetBrightnessValue().iGreenComponentNo6,
                      sBrightnessValue.iBrightNo6,sBrightnessValue.iGreenComponentNo6);
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdCloseAllLed);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    if(GetBrightnessValue().iBrightNo7 > 0)
    {
        GetLightValue(7,qCenterPoint,GetBrightnessValue().iBrightNo7,GetBrightnessValue().iGreenComponentNo7,
                      sBrightnessValue.iBrightNo7,sBrightnessValue.iGreenComponentNo7);
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdCloseAllLed);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }

    if(GetBrightnessValue().iBrightNo8 > 0)
    {
        GetLightValue(8,qCenterPoint,GetBrightnessValue().iBrightNo8,GetBrightnessValue().iGreenComponentNo8,
                      sBrightnessValue.iBrightNo8,sBrightnessValue.iGreenComponentNo8);
    }

    //关所有灯
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdCloseAllLed);
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }
}

/**
 * @brief ReadBrightnessValueParams
 * 保存到配置文件
 * @param brightnessValue 灯光值结构体
 *
 * @return
 */
bool OrdinaryBrightmess::InitMachine(QPoint &qCenterPoint)
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
    OpencvUtility::GetInstance()->GetVideoCapture(&strSaveImagePath);

    qCenterPoint = findCenterPoint(strSaveImagePath);

    if(QFile::exists(strSaveImagePath))
    {
        Mat matImage = imread(strSaveImagePath.toLatin1().data(),-1);

        CalculateGreenComponent calculateGreenComponent;
        for(int iPos = 0;iPos < 4;iPos++)
        {
           calculateGreenComponent.drawRect(matImage,qCenterPoint,30,40,iPos);
        }
        cv::namedWindow( "donkeyaime", cv::WINDOW_NORMAL );
        imshow("donkeyaime",matImage);
        waitKey(0);
    }

    return true;
}

BrightnessOrdinaryValue OrdinaryBrightmess::GetBrightnessValue()
{
    return m_sOrdinarybrightnessValue;
}

void OrdinaryBrightmess::SetBrightnessValue(BrightnessOrdinaryValue sBrightnessValue)
{
    m_sOrdinarybrightnessValue = sBrightnessValue;
}

/**
 * @brief GetLightValue
 * 获取绿色分量标准差最小的灯光值
 * @param
 *        iBrightNo   灯类型
 *        iLightValue 标准差最小的灯光值
 *        dGreenComponent 绿色分量
 * @return
 */
bool OrdinaryBrightmess::GetLightValue(const int iBrightNo,QPoint qCenterPoint,const int /*iStandardBright*/,
                                       const double dStandardGreenComponent,int &iOrdinaryBright,
                                       double &dOrdinaryGreenComponent)
{
    const QString kstrFileName = "grayscale%1.csv";
    QFile qFileName(kstrFileName.arg(iBrightNo));
    if(!qFileName.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      qDebug() << "Cannot open file for writing";
      return false;
    }
    QTextStream qTextStream(&qFileName);

    CalculateGreenComponent calculateGreenComponent;

    const int kiRectR = 30;
    const int kiRectD = 40;

    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::sm_kiCmdCloseAllLed);
    QThread::msleep(100);

    QMap<double,int> qGraySDSumMap;
    qGraySDSumMap.clear();

    QMap<double,double> qGreenComponentMap;
    qGreenComponentMap.clear();

    const int kiMaxCount = 200;
    int iSeptCount = 40000 / kiMaxCount;
    const int kiStartLightValue = 500;
    int iPosLightValue = 0;

    double dAvgSum = 0.0;
    double dStandardSD = 0.0;
    double dGreenSDSum = 0.0;

    QString strSaveImagePath;

    for(int i = 0;i < kiMaxCount;i++)
    {
        dAvgSum = 0.0;
        dStandardSD = 0.0;
        dGreenSDSum = 0.0;
        HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);

        /* 等量增加灯光值*/
        iPosLightValue = kiStartLightValue + i * iSeptCount;
        /*开灯*/
        CHidCmdThread::GetInstance()->AddOpenLedCmd(iBrightNo,iPosLightValue);
        /*等待灯光稳定*/
        while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
        {
            QApplication::processEvents();
        }
        QThread::msleep(10);

        /*拍照*/
        OpencvUtility::GetInstance()->GetVideoCapture(&strSaveImagePath);

        qTextStream << iPosLightValue << ",";

        /*计算绿色分量*/
        double dAvg = 0.0;
        for(int iPos = 0;iPos < 4;iPos++)
        {
            dAvg = 0.0;
            calculateGreenComponent.GetGreenComponentSDAvg(strSaveImagePath,qCenterPoint,kiRectR,kiRectD,iPos,
                                                      dStandardGreenComponent,dAvg,dStandardSD);
            dGreenSDSum += dStandardSD;
            dAvgSum += dAvg;
        }
        qTextStream << dGreenSDSum << ",";
        qTextStream <<( dAvgSum/4) << ",";
        qTextStream <<  "\n";

        qGraySDSumMap.insert(dGreenSDSum,iPosLightValue);
        qGreenComponentMap.insert(dGreenSDSum,dAvgSum/4);
    }
    //得到标准差最小的灯光值
    iOrdinaryBright = qGraySDSumMap.begin().value();
    //得到标准差最小的绿色分量均值
    dOrdinaryGreenComponent = qGreenComponentMap.begin().value();
    qFileName.close();
    return true;
}


/**
 * @brief SaveBrightnessValueParams
 * 保存到配置文件
 * @param brightnessValue 灯光值结构体
 *
 * @return
 */
bool OrdinaryBrightmess::SaveBrightnessValueParams(QString strFileName,QString ParamsType,
                                                   BrightnessOrdinaryValue sBrightnessValue)
{
    QMap<QString,QVariant> strSaveDataMap;
    strSaveDataMap.insert("iBrightNo1",sBrightnessValue.iBrightNo1);
    strSaveDataMap.insert("iBrightNo2",sBrightnessValue.iBrightNo2);
    strSaveDataMap.insert("iBrightNo3",sBrightnessValue.iBrightNo3);
    strSaveDataMap.insert("iBrightNo4",sBrightnessValue.iBrightNo4);
    strSaveDataMap.insert("iBrightNo5",sBrightnessValue.iBrightNo5);
    strSaveDataMap.insert("iBrightNo6",sBrightnessValue.iBrightNo6);
    strSaveDataMap.insert("iBrightNo7",sBrightnessValue.iBrightNo7);
    strSaveDataMap.insert("iBrightNo8",sBrightnessValue.iBrightNo8);

    strSaveDataMap.insert("iGreenComponentNo1",sBrightnessValue.iGreenComponentNo1);
    strSaveDataMap.insert("iGreenComponentNo2",sBrightnessValue.iGreenComponentNo2);
    strSaveDataMap.insert("iGreenComponentNo3",sBrightnessValue.iGreenComponentNo3);
    strSaveDataMap.insert("iGreenComponentNo4",sBrightnessValue.iGreenComponentNo4);
    strSaveDataMap.insert("iGreenComponentNo5",sBrightnessValue.iGreenComponentNo5);
    strSaveDataMap.insert("iGreenComponentNo6",sBrightnessValue.iGreenComponentNo6);
    strSaveDataMap.insert("iGreenComponentNo7",sBrightnessValue.iGreenComponentNo7);
    strSaveDataMap.insert("iGreenComponentNo8",sBrightnessValue.iGreenComponentNo8);

    if(ParamsConfiguration::GetInstance()->SaveParamsToConfigFile(strFileName,ParamsType,strSaveDataMap))
    {
        return true;
    }
    else
    {
        return false;
    }
}


/**
 * @brief ReadBrightnessValueParams
 * 读取配置文件
 * @param brightnessValue 灯光值结构体
 *
 * @return
 */
bool OrdinaryBrightmess::ReadBrightnessValueParams(QString strFileName,QString ParamsType,
                                                   BrightnessOrdinaryValue &sBrightnessValue)
{
    QMap<QString,QVariant> ParamsMap;
    if(ParamsConfiguration::GetInstance()->ReadParamsFromConfigFile(strFileName,ParamsType,&ParamsMap))
    {
        sBrightnessValue.iBrightNo1 = ParamsMap.value("iBrightNo1").toInt();
        sBrightnessValue.iBrightNo2 = ParamsMap.value("iBrightNo2").toInt();
        sBrightnessValue.iBrightNo3 = ParamsMap.value("iBrightNo3").toInt();
        sBrightnessValue.iBrightNo4 = ParamsMap.value("iBrightNo4").toInt();
        sBrightnessValue.iBrightNo5 = ParamsMap.value("iBrightNo5").toInt();
        sBrightnessValue.iBrightNo6 = ParamsMap.value("iBrightNo6").toInt();
        sBrightnessValue.iBrightNo7 = ParamsMap.value("iBrightNo7").toInt();
        sBrightnessValue.iBrightNo8 = ParamsMap.value("iBrightNo8").toInt();

        sBrightnessValue.iGreenComponentNo1 = ParamsMap.value("iGreenComponentNo2").toInt();
        sBrightnessValue.iGreenComponentNo2 = ParamsMap.value("iGreenComponentNo2").toInt();
        sBrightnessValue.iGreenComponentNo3 = ParamsMap.value("iGreenComponentNo3").toInt();
        sBrightnessValue.iGreenComponentNo4 = ParamsMap.value("iGreenComponentNo4").toInt();
        sBrightnessValue.iGreenComponentNo5 = ParamsMap.value("iGreenComponentNo5").toInt();
        sBrightnessValue.iGreenComponentNo6 = ParamsMap.value("iGreenComponentNo6").toInt();
        sBrightnessValue.iGreenComponentNo7 = ParamsMap.value("iGreenComponentNo7").toInt();
        sBrightnessValue.iGreenComponentNo8 = ParamsMap.value("iGreenComponentNo8").toInt();
        return true;
    }
    else
    {
        return false;
    }
}
