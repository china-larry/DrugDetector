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

enum ENUM_INVALID_RESULT        //测试结果非法
{
    INVALID_NONE = 0,           //none
    INVALID_CLINE,              //C线 无效
    INCOMPLETE_CLINE,           //C线 有效，显带不全
    INCOMPLETE_TLINE,           //阳性，T 线显带不全
};

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
    ERR_NO_FOUND,           //未找到二维码
    ERR_DECODE,             //二维码解码失败
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
    ENUM_INVALID_RESULT eInvalidType;   //结果无效类型
};

Q_DECLARE_METATYPE(TestResultData);

class ThreadTesting : public QObject
{
    Q_OBJECT
public:
    explicit ThreadTesting();
    ~ThreadTesting();
signals:
    void SignalTestErr(ENUM_ERR eErr);                      //报错 错误信息
    void SignalTestResult(TestResultData sResultTestData);  //每条测试结果
    void SignalTestComplete();                              //测试完成
    void SignalSendQRCodePic(QString strPath);              //定位二维码过程中，二维码照片路径
    void SignalSendCodeInfo(QRCodeInfo sInfoQRCodeStruct);  //收到二维码信息
    void SignalStartQRCode();                               //开始获取二维码
    void SignalSCupPicPath(QString strPath);                //测试过程中，方杯目标区图片路径
    void SignalStartMotor();
private slots:
    void _SlotMotorComplete(quint16 iCmdType, bool bResult);
    void _SlotTakePhoto();
    void _SlotMoveStepperMotor();
    void _SLotReceiveQRCodeInfo(QRCodeInfo sInfoQRCodeStruct);
    void _SlotReceiveErr(EnumTypeErr eErr);
    void _SlotReceiveQRcodePic(QString strPath);
public:
    void StartTest();                                   //启动测试
    void StopTest();                                    //停止测试
    QList<int> GetComponentGreenTCup(QString strPath);  //获取目标区域绿色分量数据 圆杯
    QList<int> GetComponentGreenSCup(QString strPath);  //获取目标区域绿色分量数据 方杯
private:
    int _ImageAnalysisProcess(int *pDataArr, int iOrgLineCenterX, int iPicWide);                       //获取目标线（C线或者T线）的面积积分
    int _ErCMethod2(int *pData, int iBackGround1, int iBackGround2, int iBackGround3,
                    int iBackGround4, int iBackGround5, int iBackGround6, int iPicWide);        //最小二乘法 去除背景
    int _GetTLineLoction(const int *kpDataArr, int iOrgCenterxLocation);
    int _GetMinLocation(const int *kpSumLineWidth, int iArrLength);
    int _GetValueTC(const QList<int> &kiHorizontalProjectionList, TestResultData &sResultTestDataStruct);   //获取C线位置
    int _FindCLine(QList<int> iHorizontalProjectionList);
    int _FindProjectMid(QList<int> iUprightProjectionList, int iPixelLevel, int iPixelSustain);             //定位项目中心点

    void _InitStatus();
    void _ReceivePicPathSCup(QString strPath);      //方杯目标区图片分析
    TestResultData _ReceivePicPathTCup(QString strPath);          //圆杯目标区图片分析
    void _ModifNextStep(int iStep, int iPixel);         //校准电机步数
    void _StatusHandler(bool bResult, ENUM_STATUS_TEST eTestStatus);
    void _GetTestResult(const InfoProject &ksProjectDataStruct, TestResultData &sResultDataStruct);
    void _GetTestResult(const double kdConcentration, const InfoProject &ksProjectDataStruct, TestResultData &sResultDataStruct);
    void _CheckInValid(QString strPathTarget, QList<int> &iHorizontalProjectionList, TestResultData &sResultDataStruct);        //结果有效性检验
    double _UnderLineProportion(QString strImgPath, int iPosition);         //求显带不全比例
    double _GetConcentration(const InfoProject &kprojectDataStruct, const TestResultData &ksResultDataStruct);
    QList<int> _FindProjectSCup(QList<int> iUprightProjectionList);
    QList<int> _UprightProjection(QString strImgPath);      //垂直投影
    QList<int> _HorizontalProjection(QString strImgPath);   //水平投影
    bool _GetRealLine(int *pDataArr, int iLineCenterX, int iPicWide);
    void _SmoothData(QList<int> &iResultList, int iSmoothValue);        //滤波
private:
    int                 m_iIndexMovement;
    int                 m_iStepsMoveMotor;
    QThread             m_qTestThread;
    QList<int>          m_iStepList;
    QRCodeInfo          m_QRCodeInfo;
    QRCodeDetector      m_CodeDetoector ;
    ENUM_STATUS_TEST    m_eCurrentStatus;

    void test();
    int _FindFirstItem(QString strPath);
};

#endif // THREADTESTING_H

