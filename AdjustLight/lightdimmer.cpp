#include <QFile>
#include <QTextStream>
#include <QTime>
#include <QApplication>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include "lightdimmer.h"
#include "hidopertaionutility.h"
#include "opencvutility.h"
#include "calculategrayscale.h"
#include "protocolutility.h"
#include "CHidCmdThread.h"
#include "ParamsConfiguration.h"

LightDimmer::LightDimmer()
{
    //connect(HIDOpertaionUtility::getInstance(),SIGNAL(SignalOperationComplete(quint16,bool)),this,
    //        SLOT(SlotGetOperationResult(quint16,bool)),Qt::DirectConnection);
}

LightDimmer::~LightDimmer()
{

}

bool LightDimmer::GetLightValue(int *iLightValue,QMap<QString,QVariant> *qLightValueMap)
{
    QFile file("grayscale.csv");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
      qDebug() << "Cannot open file for writing";
      return false;
    }
    QTextStream out(&file);
    CHidCmdThread::GetInstance()->AddOpenLedCmd(4,10000);

    QThread::msleep(50);
    QString strSaveImagePath = "";

    OpencvUtility::getInstance()->QuickGetVideoCapture(&strSaveImagePath);//丢弃
    OpencvUtility::getInstance()->QuickGetVideoCapture(&strSaveImagePath);


    CalculateGrayscale calculategrayscale;
    QPoint point(1180,1024);
    const int iRectR = 30;
    const int iRectD = 30;

    /*
    if(QFile::exists(strSaveImagePath))
    {
        Mat matImage = imread(strSaveImagePath.toLatin1().data(),-1);
        cvNamedWindow( "donkeyaime", CV_WINDOW_AUTOSIZE );
        for(int i = 0;i < 4;i++)
        {
           calculategrayscale.drawRect(matImage,point,iRectR,iRectD,i);
        }
        imshow("donkeyaime",matImage);
    }
    */
    CHidCmdThread::GetInstance()->AddCmdWithoutCmdData(ProtocolUtility::CMD_CLOSE_ALL_LED);
    QThread::msleep(100);

    QMap<double,int> qGraySDSumMap;
    qGraySDSumMap.clear();
    QTime time1;

    QTime time;
    time.start();
    const int iMaxCount = 200;
    int iSeptCount = 40000 / iMaxCount;
    const int iStartLightValue = 500;
    int iPosLightValue = 0;


    double dAvg = 0.0;
    double dSdv = 0.0;
    double dStandardSD = 0.0;
    double dGreenSDSum = 0.0;

    for(int i = 0;i < iMaxCount;i++)
    {
        dAvg = 0.0;
        dSdv = 0.0;
        dStandardSD = 0.0;
        dGreenSDSum = 0.0;

        time1.start();
        HIDOpertaionUtility::GetInstance()->SetDeviceOperate(true);

        /* 等量增加灯光值*/
        iPosLightValue = iStartLightValue + i * iSeptCount;
        /*开灯*/
        CHidCmdThread::GetInstance()->AddOpenLedCmd(3,iPosLightValue);
        /*等待灯光稳定*/
        while (HIDOpertaionUtility::GetInstance()->GetDeviceOperateStates())
        {
            QApplication::processEvents();
        }
        QThread::msleep(10);

        qDebug() << "timeHIDWrite = " << time1.elapsed();
        time1.start();
        /*拍照*/
        //上一帧
        OpencvUtility::getInstance()->QuickGetVideoCapture(&strSaveImagePath);

        qDebug() << "timeGetVideoCapture = " << time1.elapsed();
        time1.start();
        out << iPosLightValue << ",";

        /*计算绿色分量*/
        for(int j = 0;j < 4;j++)
        {
            calculategrayscale.GetGreenComponentSDAvg(strSaveImagePath,point,iRectR,iRectD,j,150.0,&dAvg,&dStandardSD);
            dGreenSDSum += dStandardSD;
            out << dAvg << "," << dSdv << "," << dStandardSD << ",";
        }
        out << dGreenSDSum << ",";
        out <<  "\n";

        qGraySDSumMap.insert(dGreenSDSum,iPosLightValue);
        qLightValueMap->insert(QString::number(iPosLightValue),dGreenSDSum);
        qDebug() << "time getgraySDSum = " << time1.elapsed();
    }
    *iLightValue = qGraySDSumMap.begin().value();

    qDebug() << "qGraySDSumMap.begin().key();" << qGraySDSumMap.begin().key();
    qDebug() << "qGraySDSumMap.begin().value();" << qGraySDSumMap.begin().value();
    qDebug() << "time = " << time.elapsed();
    file.close();

    return true;

}
bool LightDimmer::SaveLightDimmerParams(QString strFileName,QString ParamsType,
                                        LightDimmerValue mLightDimmerValue,
                                        QMap<QString,QVariant> qLightValueMap)
{
    QMap<QString,QVariant> strSaveDataMap;
    strSaveDataMap.insert("LedType",mLightDimmerValue.strLedType);
    strSaveDataMap.insert("LightValue",mLightDimmerValue.iLightValue);
    strSaveDataMap.insert("LightValueMap",QVariant(qLightValueMap));
    if(ParamsConfiguration::getInstance()->SaveParamsToConfigFile(strFileName,ParamsType,strSaveDataMap))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool LightDimmer::ReadLightDimmerParams(QString strFileName,QString ParamsType,
                                        LightDimmerValue *mLightDimmerValue,
                                        QMap<int,double> *qLightValueMap)
{
    QMap<QString,QVariant> ParamsMap;
    if(ParamsConfiguration::getInstance()->ReadParamsFromConfigFile(strFileName,ParamsType,&ParamsMap))
    {
        mLightDimmerValue->strLedType = ParamsMap.value("LedType").toString();
        mLightDimmerValue->iLightValue = ParamsMap.value("LightValue").toInt();
        QMap<QString,QVariant> qstrLightValueMap = ParamsMap.value("LightValueMap").toMap();

        QMap<QString,QVariant>::const_iterator ParamsMapIter;
        for (ParamsMapIter = qstrLightValueMap.constBegin(); ParamsMapIter != qstrLightValueMap.constEnd(); ++ParamsMapIter)
        {
            //写入新内容，原来没有就插入，原来存在就覆盖
            qLightValueMap->insert(ParamsMapIter.key().toInt(),ParamsMapIter.value().toDouble());
        }
        return true;
    }
    else
    {
        return false;
    }
}
/*
//USB命令返回 mCmdType命令类型，result 命令执行结果
void LightDimmer::SlotGetOperationResult(quint16 mCmdType,bool OperatorResult)
{
    if(mCmdType == ProtocolUtility::CMD_ROTATE_MOTOR || mCmdType == ProtocolUtility::CMD_RESET_MOTOR
            || mCmdType == ProtocolUtility::CMD_OPEN_OR_CLOSE_LED)
    {
        SetOperatorResult(OperatorResult);
    }

}
void LightDimmer::SetOperatorResult(bool OperatorResult)
{
    m_bOperatorResult = OperatorResult;
}
bool LightDimmer::GetOperatorResult()
{
    return m_bOperatorResult;
}
*/
