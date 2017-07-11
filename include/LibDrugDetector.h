#ifndef LIBDRUGDETECTOR_H
#define LIBDRUGDETECTOR_H

#include <QtCore/qglobal.h>
#include <QtCore>
#include <QDate>
#include <QVariant>
#include <QMetaType>

#if defined(LIBDRUGDETECTOR_LIBRARY)
#  define LIBDRUGDETECTORSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBDRUGDETECTORSHARED_EXPORT Q_DECL_IMPORT
#endif
// 二维码传送数据
struct QRCodeInfo
{
    int iProductLot;                // 产品批号（通过读取二维码自动生成）
    QDate qExprationDate;     // 产品有效期（通过读取二维码自动生成）
    int iProductID;                 // 产品ID（通过读取二维码自动生成）
    int iProgramCount;
};
Q_DECLARE_METATYPE(QRCodeInfo)
// 检测结果数据
struct TestResultData
{
    int iCutoffValue;            //
    int iTValue;                   // 待确认
    int iCValue;                   // 待确认
    double dRatioValue;       // 待确认
    QString strProgramName;// 项目名称
    QString strResult;          // 结果
    QString strControlLine;   // if control line valid 待确认
    QString qstrPicturePath;  // 待确认
};
Q_DECLARE_METATYPE(TestResultData)
class LIBDRUGDETECTORSHARED_EXPORT LibDrugDetector
{
    Q_OBJECT
public:
    explicit LibDrugDetector();


public slots:

signals:
    void SignalSendQRCodeInfo(QVariant sQRCodeInfo);// 发送二维码获取的产品信息，结构体
    void SingalSendResultData(QVariant sTestResultData);// 发送测试结果信息，结构体
    void SingalEndTest();// 测试结果传输完毕

public:
    void UIEndTest();// UI主动中断测试
private:
    void SignalExample();// demo 发送例子


private:
    QRCodeInfo m_sQRCodeInfo;
    TestResultData m_sTestResultData;

};

#endif // LIBDRUGDETECTOR_H
