#include <QObject>
#include "AdjustLight/qrcodedetector.h"

#ifndef THREADTESTING_H
#define THREADTESTING_H

struct intClass
{
    bool IsSucessuful;
    QVector<int> IntSubv;
};

enum ENUM_STATUS_TEST
{
    STATUS_NONE    = 0,
    STATUS_READY,
    MOVE_THE_MORTOR,
    TAKE_PHOTO,
};
enum ENUM_ERR
{
    ERR_VIDEO_CAPTURE = 0,
    ERR_STEP_MOTOR,
    ERR_LIGHT,
    ERR_DATA,
};
struct TestResultData
{
    int iIndexProject;          //项目序号
    int iCutoffValue;            //
    int iTValue;                   // 待确认
    int iCValue;                   // 待确认
    double dRatioValue;       // 待确认
    QString strProgramName;// 项目名称
    QString strResult;          // 结果
    QString strControlLine;   // if control line valid 待确认
    QString strPicturePath;  // 待确认
};

class ThreadTesting : public QObject
{
    Q_OBJECT
public:
    explicit ThreadTesting();
    ~ThreadTesting();
    void StartTest();                               //启动测试
signals:
    void SignalErr(ENUM_ERR err);                   //报错 错误信息
    void SignalTestResult(TestResultData result);   //每条测试结果
    void SignalTestComplete();                      //测试完成
    void SignalStartMotor();
    void SignalSendCodeInfo(QRCodeInfo info);       //二维码信息
private slots:
    void _SlotMotorComplete(quint16 mCmdType, bool result);
    void _SlotStatusHandler(bool result, ENUM_STATUS_TEST status);
    void _SlotTakePhoto();
    void _SlotMoveStepperMotor();
    void _SLotReceiveQRCode(QRCodeInfo info);
private:
    int         m_iIndexProject;
    int         m_iStepsMoveMotor;
    QThread     m_thread_test;
    QList<int>  m_iStepList;
    QRCodeInfo  m_QRCodeInfo;
    ENUM_STATUS_TEST    m_eCurrentStatus;
    QRCodeDetector      m_CodeDetoector ;

    TestResultData _ReceivePicPath(QString path);
    void _SlotReceivePicPath2(QString path);
    QList<int> _UprightProjection(QString strImgPath);
    QList<int> _HorizontalProjection(QString strImgPath);
    intClass _GetRealLine(int *A1, int lineCenterX, int PicW);
    double _GetConcentration(const InfoProject &projectData, const TestResultData &resultData);
    void _GetTestResult(const double concentration, const InfoProject &projectData, TestResultData &resultData);
    void _GetTestResult(const InfoProject &projectData, TestResultData &resultData);
    void _ModifNextStep(int step, int pixel);
    void _InitStatus();
    int _ImageAnalysisProcess(int *A1, int Orglinecenterx, int PicW);
    int _ErCMethod2(int *A1, int b1, int b2, int b3, int b4, int b5, int b6, int PicW);
    int _GetTLineLoction(const int *A1, int OrgCenterxLocation);
    int _GetMinLocation(const int *sumLineWidth, int arr_Length);
    int _GetValueTC(const QList<int> &iHorizontalProjectionList, TestResultData &resultData);
    int _FindCLine(QList<int> iHorizontalProjectionList);
    int FindProjectMid(QList<int> iUprightProjectionList, int iPixelLevel, int iPixelSustain);
    QList<int> _FindProjectSCup(QList<int> iUprightProjectionList);
};

#endif // THREADTESTING_H

