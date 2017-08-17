#include "CalibrationAlogrithm.h"
#include <vector>
#include <algorithm>
#include <QApplication>
#include <QtAlgorithms>
#include <QTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include "HidOpertaionUtility.h"
#include "CHidCmdThread.h"
#include "OpencvUtility.h"
#include "ParamsConfiguration.h"

CalibrationAlogrithm::CalibrationAlogrithm()
{

}

CalibrationAlogrithm::~CalibrationAlogrithm()
{

}

/**
 * @brief TraverseLedLight
 * 遍历所有LED亮度
 * @param mParams 输入类型参数和亮度参数
 *        iTargetUpGreenLightValue    输出上绿灯光值
 *        iTargetDownGreenLightValue  输出下绿灯光值
 *        iTargetLeftGreenLightValue  输出左绿灯光值
 *        iTargetRightGreenLightValue 输出右绿灯光值
 * @return
 */
void CalibrationAlogrithm::TraverseLedLight(StandardMachineCalibrateParams sParams,
                                            StandardMachineCalibrateLight &sStandardMachineCalibrateLight)
{
    QFile qFileName("CalibrationAlogrithm.csv");
    if(!qFileName.open(QIODevice::WriteOnly | QIODevice::Text))
    {
       qDebug() << "Cannot open file for writing";
       return;
    }
    QTextStream qTextOutStream(&qFileName);

    double dTCValueC6 = 0.0;
    double dTCValueC8 = 0.0;
    double dZValue = 0.0;
    std::vector<double> dTCValueC6Vector;
    std::vector<double> dTCValueC8Vector;
    std::vector<double> dZValueVector;

    sStandardMachineCalibrateLight.dZMaxValue = 0.0;
    double dTestZMaxValue = 0.0;
    qint16 ipos = 0;

    // 1.机器复位
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    CHidCmdThread::GetInstance()->AddResetMotorCmd(10);

    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }


    if(sParams.iCapType == 1)
    {
        //唾液杯(Cube-Cup) 机器转到C6C8测试窗口位置
        HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
        CHidCmdThread::GetInstance()->AddRotateMotorCmd(20,100,0);

        while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
        {
            QApplication::processEvents();
        }
    }
//    else if(mParams.iCapType == 0)
//    {
//        //尿液杯(T-cup) 机器转到C6测试窗口位置
//        HIDOpertaionUtility::getInstance()->SetDeviceOperate(true);
//        CHidCmdThread::GetInstance()->AddRotateMotorCmd(20,100,0);
//    }

    for(qint16 iUpGreenLightValue = sParams.iUpGreenLightValueMin;iUpGreenLightValue < sParams.iUpGreenLightValueMax;
        iUpGreenLightValue += sParams.iUpGreenLightValueStep)                //上绿灯
    {
        HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
        CHidCmdThread::GetInstance()->AddOpenLedCmd(1,iUpGreenLightValue);

        while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
        {
            QThread::msleep(50);
            QApplication::processEvents();
        }
        for(qint16 iDownGreenLightValue = sParams.iDownGreenLightValueMin;    //下绿灯
            iDownGreenLightValue < sParams.iDownGreenLightValueMax;
            iDownGreenLightValue += sParams.iDownGreenLightValueStep)
        {
            HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
            CHidCmdThread::GetInstance()->AddOpenLedCmd(2,iDownGreenLightValue);

            while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
            {
                QThread::msleep(50);
                QApplication::processEvents();
            }
            for(qint16 iLeftGreenLightValue = sParams.iLeftGreenLightValueMin; //左绿灯
                iLeftGreenLightValue < sParams.iLeftGreenLightValueMax;
                iLeftGreenLightValue += sParams.iLeftGreenLightValueStep)
            {
                HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
                CHidCmdThread::GetInstance()->AddOpenLedCmd(3,iLeftGreenLightValue);

                while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
                {
                    QThread::msleep(50);
                    QApplication::processEvents();
                }
                dTCValueC6Vector.clear();
                dTCValueC8Vector.clear();
                dZValueVector.clear();
                for(qint16 iRightGreenLightValue = sParams.iRightGreenLightValueMin;  //右绿灯
                    iRightGreenLightValue < sParams.iRightGreenLightValueMax;
                    iRightGreenLightValue += sParams.iRightGreenLightValueStep)
                {
                    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
                    CHidCmdThread::GetInstance()->AddOpenLedCmd(4,iRightGreenLightValue);

                    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
                    {
                        QThread::msleep(50);
                        QApplication::processEvents();
                    }


                    if(sParams.iCapType == 0)
                    {
                        //尿液杯(T-cup) 机器转到C6测试窗口位置
                        HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
                        CHidCmdThread::GetInstance()->AddRotateMotorCmd(20,100,0);

                        while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
                        {
                            QThread::msleep(50);
                            QApplication::processEvents();
                        }
                    }

                    for(qint16 iSignalLightValueTestTimes = 0;
                        iSignalLightValueTestTimes < sParams.iSignalLightValueTestCount;
                        iSignalLightValueTestTimes++)
                    {
                        qTextOutStream << iUpGreenLightValue << ",";
                        qTextOutStream << iDownGreenLightValue << ",";
                        qTextOutStream << iLeftGreenLightValue << ",";
                        qTextOutStream << iRightGreenLightValue << ",";
                        if(sParams.iCapType == 0 || sParams.iCapType == 1)
                        {
                            //获取 C6 T/C的值
                            GetTCResult(sParams.iCapType,0,dTCValueC6);
                            dTCValueC6Vector.push_back(dTCValueC6);
                            qTextOutStream << dTCValueC6 << ",";
                        }

                        if(sParams.iCapType == 1)
                        {
                            //获取 C8 T/C的值
                            GetTCResult(sParams.iCapType,1,dTCValueC8);
                            dTCValueC8Vector.push_back(dTCValueC8);
                            qTextOutStream << dTCValueC8 << ",";

                        }
                    }

                    if(sParams.iCapType == 0)
                    {
                        //机器转到C8拍照位置
                        HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
                        CHidCmdThread::GetInstance()->AddRotateMotorCmd(20,100,1);

                        while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
                        {
                            QThread::msleep(50);
                            QApplication::processEvents();
                        }

                        for(qint16 iSignalLightValueTestTimes = 0;
                            iSignalLightValueTestTimes < sParams.iSignalLightValueTestCount;
                            iSignalLightValueTestTimes++)
                        {
                            //获取 C8 T/C的值
                            GetTCResult(sParams.iCapType,1,dTCValueC8);

                            dTCValueC8Vector.push_back(dTCValueC8);
                        }
                    }

                    //计算Z值
                    dZValue = CalculateZ(dTCValueC6Vector,dTCValueC8Vector);
                    qTextOutStream << dZValue << ",";
                    qTextOutStream <<  "\n";
                    dZValueVector.push_back(dZValue);
                }
                //返回Z最大值和最大值的下标
                GetMaxValue(dZValueVector,dTestZMaxValue,ipos);

                if(dTestZMaxValue > sStandardMachineCalibrateLight.dZMaxValue)
                {
                    sStandardMachineCalibrateLight.dZMaxValue = dTestZMaxValue;
                    sStandardMachineCalibrateLight.iUpGreenLightCalibrateValue = iUpGreenLightValue;
                    sStandardMachineCalibrateLight.iDownGreenLightCalibrateValue = iDownGreenLightValue;
                    sStandardMachineCalibrateLight.iLeftGreenLightCalibrateValue = iLeftGreenLightValue;
                    sStandardMachineCalibrateLight.iRightGreenLightCalibrateValue = sParams.iRightGreenLightValueMin
                                                                           + ipos * sParams.iRightGreenLightValueStep;
                }
            }
        }
    }
    qFileName.close();
}

/**
 * @brief GetResult
 * 获取C6和C8 的 T/C值
 * @param iCapType杯型  iPaper试纸类型
 * @return dTCValue
 */
bool CalibrationAlogrithm::GetTCResult(qint16 /*iCapType*/,qint16 /*iPaper*/,double &dTCValue)
{
    //随机数参数测试数据
    QString strSaveImagePath = "";
    OpencvUtility::GetInstance()->GetVideoCapture(&strSaveImagePath);

    QTime qTime= QTime::currentTime();
    qsrand(qTime.msec() + qTime.second() * 1000);
    dTCValue = qrand();
    return true;
}

/**
 * @brief CalculateZ
 * 计算Z值 Z=(AVG(c6)-AVG(c8))/(STD(c6)+STD(c8))
 * @param dCTC6Vector dCTC8Vector
 * @return double
 */
double CalibrationAlogrithm::CalculateZ(const std::vector<double> &dCTC6Vector,
                                        const std::vector<double> &dCTC8Vector)
{
    double dZValue = 0.0;
    double dCT6sum = std::accumulate(std::begin(dCTC6Vector), std::end(dCTC6Vector), 0.0);
    double dAvgCT6mean =  dCT6sum / dCTC6Vector.size(); //C6 T/C均值

    double dStdCT6accum  = 0.0;
    std::for_each (std::begin(dCTC6Vector), std::end(dCTC6Vector), [&](const double dValue)
    {
        dStdCT6accum  += (dValue - dAvgCT6mean) * (dValue - dAvgCT6mean); //C8 T/C标准差
    });

    double dCT8sum = std::accumulate(std::begin(dCTC8Vector), std::end(dCTC8Vector), 0.0);
    double dAvgCT8mean =  dCT8sum / dCTC8Vector.size(); //C8 T/C均值

    double dStdCT8accum  = 0.0;
    std::for_each (std::begin(dCTC8Vector), std::end(dCTC8Vector), [&](const double dValue)
    {
        dStdCT8accum  += (dValue - dAvgCT8mean) * (dValue - dAvgCT8mean); //C8 T/C标准差
    });

    if(dStdCT6accum + dStdCT8accum != 0)
    {
         dZValue = (dAvgCT6mean - dAvgCT8mean) / (dStdCT6accum + dStdCT8accum);
    }
    return dZValue;
}

/**
 * @brief GetMaxValue
 * 计算vector最大值
 * @param dMaxValue  输出最大值
 *        ipos       输出最大值下标
 * @return bool
 */
bool CalibrationAlogrithm::GetMaxValue(std::vector<double> dVector,double &dMaxValue,qint16 &ipos)
{
    std::vector<double>::iterator dMaxIter = std::max_element(std::begin(dVector), std::end(dVector));
    dMaxValue = *dMaxIter;
    ipos = std::distance(std::begin(dVector), dMaxIter);
    return true;
}

/**
 * @brief SaveLedLightParams
 * 保存标准机灯光参数到配置文件
 * @param strFileName 文件名
 *        strParamsType  参数类型
 *        mStandardMachineCalibrateLight    标准机灯光校准值结构体
 * @return 成功 true  失败   false
 */
bool CalibrationAlogrithm::SaveLedLightParams(QString strFileName,QString strParamsType,
                              StandardMachineCalibrateLight sStandardMachineCalibrateLight)
{
    QMap<QString,QVariant> strSaveDataMap;

    strSaveDataMap.insert("CapType",sStandardMachineCalibrateLight.iCapType);
    strSaveDataMap.insert("UpGreenLightCalibrateValue",
                          sStandardMachineCalibrateLight.iUpGreenLightCalibrateValue);
    strSaveDataMap.insert("DownGreenLightCalibrateValue",
                          sStandardMachineCalibrateLight.iDownGreenLightCalibrateValue);
    strSaveDataMap.insert("LeftGreenLightCalibrateValue",
                          sStandardMachineCalibrateLight.iLeftGreenLightCalibrateValue);
    strSaveDataMap.insert("RightGreenLightCalibrateValue",
                          sStandardMachineCalibrateLight.iRightGreenLightCalibrateValue);
    strSaveDataMap.insert("ZMaxValue",sStandardMachineCalibrateLight.dZMaxValue);
    //保存参数到配置文件
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
 * @brief ReadLedLightParams
 * 从配置文件读取标准机灯光参数
 * @param strFileName 文件名
 *        strParamsType  参数类型
 *        mStandardMachineCalibrateLight    标准机灯光校准值结构体
 * @return 成功 true  失败   false
 */
bool CalibrationAlogrithm::ReadLedLightParams(QString strFileName,QString strParamsType,
                        StandardMachineCalibrateLight *pStandardMachineCalibrateLight)
{
    QMap<QString,QVariant> ParamsMap;
    //从配置文件中读取参数
    if(ParamsConfiguration::GetInstance()->ReadParamsFromConfigFile(strFileName,strParamsType,&ParamsMap))
    {
        pStandardMachineCalibrateLight->iCapType = ParamsMap.value("CapType").toInt();
        pStandardMachineCalibrateLight->iUpGreenLightCalibrateValue =
                ParamsMap.value("UpGreenLightCalibrateValue").toInt();
        pStandardMachineCalibrateLight->iDownGreenLightCalibrateValue =
                ParamsMap.value("DownGreenLightCalibrateValue").toInt();
        pStandardMachineCalibrateLight->iLeftGreenLightCalibrateValue =
                ParamsMap.value("LeftGreenLightCalibrateValue").toInt();
        pStandardMachineCalibrateLight->iRightGreenLightCalibrateValue =
                ParamsMap.value("RightGreenLightCalibrateValue").toInt();
        pStandardMachineCalibrateLight->dZMaxValue = ParamsMap.value("ZMaxValue").toDouble();
        return true;
    }
    else
    {
        return false;
    }
}

