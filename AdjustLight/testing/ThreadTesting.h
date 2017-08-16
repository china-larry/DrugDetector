/*****************************************************
  * Copyright: 万孚生物
  * Author: 张良
  * Date: 2017-7-9
  * Description: 实现测试流程，测试算法，测试结果、测试过程中错误 上报。
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/

#include <QObject>
#define PRO_DRUG_DETECTOR
#ifdef PRO_DRUG_DETECTOR
    #include "AdjustLight/qrcodedetector.h"
#else
    #include "qrcodedetector.h"
#endif
#ifndef THREADTESTING_H
#define THREADTESTING_H


enum ENUM_STATUS_TEST       //枚举：运行状态
{
    STATUS_NONE    = 0,     //状态-空
    STATUS_READY,           //状态-待机
    MOVE_THE_MORTOR,        //状态-转动电机
    TAKE_PHOTO,             //状态-拍照
};
Q_DECLARE_METATYPE(ENUM_STATUS_TEST);

enum ENUM_ERR               //枚举：错误信息
{
    ERR_VIDEO_CAPTURE = 0,  //获取照片失败
    ERR_STEP_MOTOR,         //转动电机失败
    ERR_LIGHT,              //开关灯失败
    ERR_DATA,               //参数错误
    ERR_NO_FOUND,            //未找到二维码
    ERR_DECODE,              //二维码解码失败
    ERR_DISCONNECT_USB,     //USB链接失败
    ERR_VIDEOOPENFAILED     //摄像头打开失败
};
Q_DECLARE_METATYPE(ENUM_ERR);

struct TestResultData       //测试结果
{
    int iIndexProject;      // 项目序号
    int iCutoffValue;       // CutOff值
    int iTValue;            // T线 积分值
    int iCValue;            // C线 积分值
    double dRatioValue;     // 待确认
    QString strProgramName; // 项目名称
    QString strResult;      // 结果
    QString strControlLine; // if control line valid
    QString strPicturePath; // 目标区截图路径
};

Q_DECLARE_METATYPE(TestResultData);

class ThreadTesting : public QObject
{
    Q_OBJECT
public:
    explicit ThreadTesting();
    ~ThreadTesting();
signals:
    void SignalTestErr(ENUM_ERR eErr);               //报错 错误信息
    void SignalTestResult(TestResultData sResultTestData);   //每条测试结果
    void SignalTestComplete();                      //测试完成
    void SignalSendCodeInfo(QRCodeInfo sInfoQRCode);       //二维码信息
    void SignalSendQRCodePic(QString strPath);         //二维码照片路径
    void SignalStartMotor();
private slots:
    void _SlotMotorComplete(quint16 iCmdType, bool bResult);
    void _SlotTakePhoto();
    void _SlotMoveStepperMotor();
    void _SlotReceiveErr(EnumTypeErr eErr);
    void _SLotReceiveQRCodeInfo(QRCodeInfo sInfoQRCode);
    void _SlotReceiveQRcodePic(QString strPath);
public:
    void StartTest();                               //启动测试
    void StopTest();                                //停止测试
    QList<int> GetComponentGreenTCup(QString strPath); //获取目标区域绿色分量数据 圆杯
    QList<int> GetComponentGreenSCup(QString strPath); //获取目标区域绿色分量数据 方杯
private:
    int _ImageAnalysisProcess(int *pDataArr, int iOrgLineCenterX, int iPicWide);                       //获取目标线（C线或者T线）的面积积分
    int _ErCMethod2(int *pData, int iBackGround1, int iBackGround2, int iBackGround3,
                    int iBackGround4, int iBackGround5, int iBackGround6, int iPicWide);        //最小二乘法 去除背景
    int _GetTLineLoction(const int *pDataArr, int iOrgCenterxLocation);
    int _GetMinLocation(const int *pSumLineWidth, int iArrLength);
    int _GetValueTC(const QList<int> &iHorizontalProjectionList, TestResultData &resultData);
    int _FindCLine(QList<int> iHorizontalProjectionList);
    int _FindProjectMid(QList<int> iUprightProjectionList, int iPixelLevel, int iPixelSustain);

    void _InitStatus();
    void _ReceivePicPathSCup(QString strPath);
    TestResultData _ReceivePicPathTCup(QString strPath);
    void _ModifNextStep(int iStep, int iPixel);
    void _StatusHandler(bool bResult, ENUM_STATUS_TEST status);
    void _GetTestResult(const InfoProject &projectData, TestResultData &resultData);
    void _GetTestResult(const double concentration, const InfoProject &projectData, TestResultData &resultData);
    double _GetConcentration(const InfoProject &projectData, const TestResultData &resultData);
    QList<int> _FindProjectSCup(QList<int> iUprightProjectionList);
    QList<int> _UprightProjection(QString strImgPath);
    QList<int> _HorizontalProjection(QString strImgPath);
    bool _GetRealLine(int *pDataArr, int iLineCenterX, int iPicWide);
    void _SmoothData(QList<int> &iResultList, int iSmoothValue);
private:
    int                 m_iIndexMovement;
    int                 m_iStepsMoveMotor;
    QThread             m_qTestThread;
    QList<int>          m_iStepList;
    QRCodeInfo          m_QRCodeInfo;
    QRCodeDetector      m_CodeDetoector ;
    ENUM_STATUS_TEST    m_eCurrentStatus;

};

#endif // THREADTESTING_H

