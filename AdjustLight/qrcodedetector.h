/*******************************************************************
 **
 ** Copyright:万孚生物
 ** Author: wwl
 ** Date:2017-07-17
 ** Description: 定位和解析二维码（V2和V5两套二维码解析协议）
 ** ----------------------------------------------------------
 ** History:
 **   1.Author:
 **-----------------------------------------------------------
 **
 ********************************************************************/
#ifndef QRCODEDETECTOR_H
#define QRCODEDETECTOR_H

#include <QtCore/qglobal.h>
#include <QtCore>
#include <QDate>
#include <QVariant>
#include <QMetaType>
#include <QObject>
#include "hidopertaionutility.h"
#include "protocolutility.h"
#include "opencvutility.h"
#include "QZXing.h"

enum EnumTypeCup
{
    TypeTCup = 0,        //圆杯（尿液）
    TypeKCup5 = 1,       //5联旋钮杯（尿液）
    TypeSCup10 = 4,      //10联唾液方杯（唾液）
    TypeKCup6 = 6        //6联旋钮杯（尿液）
};

enum EnumTypeErr
{
    ErrNoFound =0,    //未找到二维码
    ErrDecode         //二维码解码失败
};

enum EnumTypeLight
{
    DownLightGreen = 1,     //绿灯1 下方绿灯
    DownLightWhite = 2,     //白灯1 下方白灯
    UpLightGreen = 3,       //绿灯2 上方绿灯
    UpLightWhite = 4        //白灯3 上方白等
};

struct InfoProject
{
    int     iIndexProgram;      //项目序号
    QString    strProjectName;  //项目名称
    int  dThresholdUp;          //上阈值
    int  dThresholdDown;        //下阈值
    double  dSensitivityUp;     //上灵敏度
    double  dSensitivityDown;   //下灵敏度
};

// 二维码数据
struct QRCodeInfo
{
    QString iProductLot;            // 产品批号（通过读取二维码自动生成）
    QDate qExprationDate;           // 产品有效期（通过读取二维码自动生成）
    QString strProductID;             // 产品ID（通过读取二维码自动生成）
    EnumTypeCup eTypeCup;           //杯类型
    int iProgramCount;              //项目总数
    QString strVerson;              //版本
    QList<InfoProject> listProject; //项目信息
    qint32 iQRCodePosition;         //二维码位置（距离复位需要顺时针转的步数）
};
Q_DECLARE_METATYPE(QRCodeInfo)
class QRCodeDetector : public QObject
{
    Q_OBJECT
public:
    explicit QRCodeDetector();
    ~QRCodeDetector();

    //初始化设备位置和灯光
    bool InitDevice();
    /**
     * @brief TestGetQRCode
     * 获取二维码信息，发送获取二维码信号
     * @param
     * @return
     */
    void TestGetQRCode();

    /**
     * @brief locationQRCode
     * 定位和识别二维码
     * @param strQRCodeInfo 返回识别到的二维码字符串
     *        iQRCodePosition 二维码距离复位位置的距离
     * @return
     */
    bool locationQRCode(QString &strQRCodeInfo,qint32 *iQRCodePosition);

    /**
     * @brief GetQRCodeImage
     * 获取二维码照片
     * @param  strImagePath 输出照片保存的路径
     * @return
     */
    bool GetQRCodeImage(QString *strImagePath);

    /**
     * @brief GetQRCodeImageInfo
     * 识别二维码
     * @param  strImagePath 输入照片保存的路径
     *         strQRCodeInfo 输出解析到的二维码字符串
     * @return
     */
    bool GetQRCodeImageInfo(const QString strImagePath,QString *strQRCodeInfo);

    /**
     * @brief DecodeQrcode
     * 根据V2或V5版本协议 解析二维码字符串
     * @param  strImagePath 输出照片保存的路径
     *         qrCodeInfo  输出解析后的二维码信息
     * @return
     */
    bool DecodeQrcode(const QString strdecode,QRCodeInfo *qrCodeInfo);

    /**
     * @brief DecodeQrcode
     * 根据V2或V5版本协议 解析二维码字符串
     * @param  strImagePath 输出照片保存的路径
     *         qrCodeInfo  输出解析后的二维码信息
     * @return
     */
    bool FindQRcodeLocationRect(IplImage *dstImg);

    //开灯 成功 0 失败-1
    int TestLightUp(EnumTypeLight type);

    //关灯 成功 0 失败-1
    int TestLightDown();

    //封装二维码信息
    QRCodeInfo PackageQRCodeInfo(QString strBatchNumber,                //项目批号
                                 QString strValidityData,               //有效期
                                 QString strCardNumber,                 //卡序列号
                                 QString strCupType,                    //杯型
                                 QString strAllCount,                   //总子条数
                                 QString strVersion,                    //版本
                                 QVector<QStringList> qv_strListitem);  //项目信息

    QString GetProjectName(const int iIndex);
    //void SetOperatorResult(bool OperatorResult);
    //bool GetOperatorResult();
    void SetQRCodePosition(int QRCodePosition);
    int GetQRCodePosition();

    /**
     * @brief mSleep
     * 延时
     * @param   msec 延时的时间（毫秒）

     * @return
     */
    void mSleep(qint32 msec);

    /**
     * @brief ExtractQRCode
     * 提取二维码
     * @param  strSrcImage 输入照片的路径
     *         strDesImage  输出二维码图片
     * @return
     */
    bool ExtractQRCode(QString strSrcImage,QString &strDesImage);

    QZXing *GetZxingDecoder();
    void SetZxingDecoder(QZXing *pZxingDecoder);

signals:
    void SignalQRCodeLocation(QString pathPic);     //定位二维码过程中，每次拍照都要发送该照片的路径
    void SignalQRCodeInfo(QRCodeInfo info);         //定位二维码后，发送二维码信息
    void SignalErrInfo(EnumTypeErr eErr);           //错误信号
    void SignalGetQRCode();                         //开始获取二维码信号

private slots:
    void SlotGetQRcode();                           //
    //void SlotGetOperationResult(quint16 mCmdType,bool OperatorResult);    //USB命令返回 mCmdType命令类型，result 命令执行结果

private:
    //bool m_bOperatorResult;
    int m_iQRCodePosition;         //二维码定位后，距离复位位置的距离（顺时针转到复位位置的步数）
    QZXing *m_pZxingDecoder;
};

#endif // QRCODEDETECTOR_H
