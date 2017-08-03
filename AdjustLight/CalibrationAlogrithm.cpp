#include "CalibrationAlogrithm.h"
#include <vector>
#include <algorithm>
#include <QApplication>
#include <QtAlgorithms>
#include <QTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include "hidopertaionutility.h"
#include "CHidCmdThread.h"
#include "opencvutility.h"
#include "ParamsConfiguration.h"

CalibrationAlogrithm::CalibrationAlogrithm()
{

}

/**
 * @brief TraverseLedLight
 *  唾液杯校准：1.机器复位，机器转到拍照位置
 *            2.直接拍照30次，从每张图片需获取5个窗口区域，左边两条C6试纸，右边两条C8试纸，获取T、C值
 *            3.求30次测试C6、C8的均值，标准差，求Z值，保存
 *
 * 尿液杯校准： 1.机器复位，机器转到C6测试窗口位置
 *              2.直接拍照30次，从每张图片需获取C6的窗口区域，获取C的T、C值
 *              3.转动到C8测试窗口位置，直接拍照30次，从每张图片需获取C8的窗口区域，获取C的T、C值
 *              4.求30次测试C6、C8的均值，标准差，求Z值，保存
 * @param
 * @return
 */
void CalibrationAlogrithm::TraverseLedLight(StandardMachineCalibrateParams mParams,
                                            StandardMachineCalibrateLight *mStandardMachineCalibrateLight)
{
    QFile file("CalibrationAlogrithm.csv");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      qDebug() << "Cannot open file for writing";
      return;
    }
    QTextStream out(&file);

    double dTCValueC6 = 0.0;
    double dTCValueC8 = 0.0;
    double dZValue = 0.0;
    std::vector<double> dTCValueC6Vector;
    std::vector<double> dTCValueC8Vector;
    std::vector<double> dZValueVector;

    mStandardMachineCalibrateLight->dZMaxValue = 0.0;
    double dTestZMaxValue = 0.0;
    qint16 ipos = 0;

    // 1.机器复位
    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
    CHidCmdThread::GetInstance()->AddResetMotorCmd(10);

    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
    {
        QApplication::processEvents();
    }


    if(mParams.iCapType == 1)
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

    for(qint16 iUpGreenLightValue = mParams.iUpGreenLightValueMin;iUpGreenLightValue < mParams.iUpGreenLightValueMax;
        iUpGreenLightValue += mParams.iUpGreenLightValueStep)                //上绿灯
    {
        HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
        CHidCmdThread::GetInstance()->AddOpenLedCmd(1,iUpGreenLightValue);

        while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
        {
            QThread::msleep(50);
            QApplication::processEvents();
        }
        for(qint16 iDownGreenLightValue = mParams.iDownGreenLightValueMin;    //下绿灯
            iDownGreenLightValue < mParams.iDownGreenLightValueMax;
            iDownGreenLightValue += mParams.iDownGreenLightValueStep)
        {
            HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
            CHidCmdThread::GetInstance()->AddOpenLedCmd(2,iDownGreenLightValue);

            while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
            {
                QThread::msleep(50);
                QApplication::processEvents();
            }
            for(qint16 iLeftGreenLightValue = mParams.iLeftGreenLightValueMin; //左绿灯
                iLeftGreenLightValue < mParams.iLeftGreenLightValueMax;
                iLeftGreenLightValue += mParams.iLeftGreenLightValueStep)
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
                for(qint16 iRightGreenLightValue = mParams.iRightGreenLightValueMin;  //右绿灯
                    iRightGreenLightValue < mParams.iRightGreenLightValueMax;
                    iRightGreenLightValue += mParams.iRightGreenLightValueStep)
                {
                    HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);
                    CHidCmdThread::GetInstance()->AddOpenLedCmd(4,iRightGreenLightValue);

                    while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
                    {
                        QThread::msleep(50);
                        QApplication::processEvents();
                    }


                    if(mParams.iCapType == 0)
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
                        iSignalLightValueTestTimes < mParams.iSignalLightValueTestCount;
                        iSignalLightValueTestTimes++)
                    {
                        out << iUpGreenLightValue << ",";
                        out << iDownGreenLightValue << ",";
                        out << iLeftGreenLightValue << ",";
                        out << iRightGreenLightValue << ",";
                        if(mParams.iCapType == 0 || mParams.iCapType == 1)
                        {
                            //获取 C6 T/C的值
                            GetTCResult(mParams.iCapType,0,dTCValueC6);
                            dTCValueC6Vector.push_back(dTCValueC6);
                            out << dTCValueC6 << ",";
                        }

                        if(mParams.iCapType == 1)
                        {
                            //获取 C8 T/C的值
                            GetTCResult(mParams.iCapType,1,dTCValueC8);
                            dTCValueC8Vector.push_back(dTCValueC8);
                            out << dTCValueC8 << ",";

                        }
                    }

                    if(mParams.iCapType == 0)
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
                            iSignalLightValueTestTimes < mParams.iSignalLightValueTestCount;
                            iSignalLightValueTestTimes++)
                        {
                            //获取 C8 T/C的值
                            GetTCResult(mParams.iCapType,1,dTCValueC8);

                            dTCValueC8Vector.push_back(dTCValueC8);
                        }
                    }

                    //计算Z值
                    dZValue = CalculateZ(dTCValueC6Vector,dTCValueC8Vector);
                    out << dZValue << ",";
                    out <<  "\n";
                    dZValueVector.push_back(dZValue);
                }
                //返回Z最大值和最大值的下标
                GetMaxValue(dZValueVector,dTestZMaxValue,ipos);

                if(dTestZMaxValue > mStandardMachineCalibrateLight->dZMaxValue)
                {
                    mStandardMachineCalibrateLight->dZMaxValue = dTestZMaxValue;
                    mStandardMachineCalibrateLight->iUpGreenLightCalibrateValue = iUpGreenLightValue;
                    mStandardMachineCalibrateLight->iDownGreenLightCalibrateValue = iDownGreenLightValue;
                    mStandardMachineCalibrateLight->iLeftGreenLightCalibrateValue = iLeftGreenLightValue;
                    mStandardMachineCalibrateLight->iRightGreenLightCalibrateValue = mParams.iRightGreenLightValueMin
                                                                           + ipos * mParams.iRightGreenLightValueStep;
                }
            }
        }
    }
    file.close();
}

//获取 T/C 值
bool CalibrationAlogrithm::GetTCResult(qint16 iCapType,qint16 iPaper,double &dTCValue)
{
    //随机数参数测试数据
    QString strSaveImagePath = "";
    OpencvUtility::getInstance()->QuickGetVideoCapture(&strSaveImagePath);

    QTime t= QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);
    dTCValue = qrand();
    return true;
}

//Z值计算方式：Z=(AVG(c6)-AVG(c8))/(STD(c6)+STD(c8))
double CalibrationAlogrithm::CalculateZ(const std::vector<double> &dCTC6Vector,const std::vector<double> &dCTC8Vector)
{
    double dZValue = 0.0;
    double dCT6sum = std::accumulate(std::begin(dCTC6Vector), std::end(dCTC6Vector), 0.0);
    double dAvgCT6mean =  dCT6sum / dCTC6Vector.size(); //C6 T/C均值

    double dStdCT6accum  = 0.0;
    std::for_each (std::begin(dCTC6Vector), std::end(dCTC6Vector), [&](const double d)
    {
        dStdCT6accum  += (d-dAvgCT6mean)*(d-dAvgCT6mean); //C8 T/C标准差
    });

    double dCT8sum = std::accumulate(std::begin(dCTC8Vector), std::end(dCTC8Vector), 0.0);
    double dAvgCT8mean =  dCT8sum / dCTC8Vector.size(); //C8 T/C均值

    double dStdCT8accum  = 0.0;
    std::for_each (std::begin(dCTC8Vector), std::end(dCTC8Vector), [&](const double d)
    {
        dStdCT8accum  += (d-dAvgCT8mean)*(d-dAvgCT8mean); //C8 T/C标准差
    });

    if(dStdCT6accum + dStdCT8accum != 0)
    {
         dZValue = (dAvgCT6mean - dAvgCT8mean) / (dStdCT6accum + dStdCT8accum);
    }
    return dZValue;
}

//获取容器最大值和最大值的下标
bool CalibrationAlogrithm::GetMaxValue(std::vector<double> dVector,double &dMaxValue,qint16 &ipos)
{
    std::vector<double>::iterator dMaxIter = std::max_element(std::begin(dVector), std::end(dVector));
    dMaxValue = *dMaxIter;
    ipos = std::distance(std::begin(dVector), dMaxIter);
    return true;
}

bool CalibrationAlogrithm::SaveLedLightParams(QString strFileName,QString ParamsType,
                              StandardMachineCalibrateLight mStandardMachineCalibrateLight)
{
    QMap<QString,QVariant> strSaveDataMap;

    strSaveDataMap.insert("CapType",mStandardMachineCalibrateLight.iCapType);
    strSaveDataMap.insert("UpGreenLightCalibrateValue",mStandardMachineCalibrateLight.iUpGreenLightCalibrateValue);
    strSaveDataMap.insert("DownGreenLightCalibrateValue",mStandardMachineCalibrateLight.iDownGreenLightCalibrateValue);
    strSaveDataMap.insert("LeftGreenLightCalibrateValue",mStandardMachineCalibrateLight.iLeftGreenLightCalibrateValue);
    strSaveDataMap.insert("RightGreenLightCalibrateValue",mStandardMachineCalibrateLight.iRightGreenLightCalibrateValue);
    strSaveDataMap.insert("ZMaxValue",mStandardMachineCalibrateLight.dZMaxValue);
    if(ParamsConfiguration::getInstance()->SaveParamsToConfigFile(strFileName,ParamsType,strSaveDataMap))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool CalibrationAlogrithm::ReadLedLightParams(QString strFileName,QString ParamsType,
                        StandardMachineCalibrateLight *mStandardMachineCalibrateLight)
{
    QMap<QString,QVariant> ParamsMap;
    if(ParamsConfiguration::getInstance()->ReadParamsFromConfigFile(strFileName,ParamsType,&ParamsMap))
    {
        mStandardMachineCalibrateLight->iCapType = ParamsMap.value("CapType").toInt();
        mStandardMachineCalibrateLight->iUpGreenLightCalibrateValue = ParamsMap.value("UpGreenLightCalibrateValue").toInt();
        mStandardMachineCalibrateLight->iDownGreenLightCalibrateValue = ParamsMap.value("DownGreenLightCalibrateValue").toInt();
        mStandardMachineCalibrateLight->iLeftGreenLightCalibrateValue = ParamsMap.value("LeftGreenLightCalibrateValue").toInt();
        mStandardMachineCalibrateLight->iRightGreenLightCalibrateValue = ParamsMap.value("RightGreenLightCalibrateValue").toInt();
        mStandardMachineCalibrateLight->dZMaxValue = ParamsMap.value("ZMaxValue").toDouble();
        return true;
    }
    else
    {
        return false;
    }
}

