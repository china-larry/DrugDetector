/*****************************************************
  * Copyright: 万孚生物
  * Author: 刘青
  * Date: 2017-7-9
  * Description: 实现测试页功能，用户数据输入，测试流程操作，测试数据显示
  * -------------------------------------------------------------------------
  * History:
  *
  *
  *
  * -------------------------------------------------------------------------
  ****************************************************/
#ifndef CDETECTORPAGE_H
#define CDETECTORPAGE_H

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QMenu>
#include <QComboBox>
#include <QTableWidget>
#include <QList>
#include <QPrinter>
#include "CommonDataWidget/CLabelLineEditWidget.h"
#include "CommonDataWidget/CLabelDateWidget.h"
#include "CommonDataWidget/CLabelDateTimeWidget.h"
#include "CommonDataWidget/CLabelCommoBoxWidget.h"
#include "CommonDataWidget/CPrintPreviewWidget.h"
#include "AdjustLight/testing/ThreadTesting.h"
#include "PublicFunction.h"
#include "AdjustLight/VideoThread.h"

//
class CDetectorPage : public QWidget
{
    Q_OBJECT
public:
    explicit CDetectorPage(QWidget *parent = nullptr);
    ~CDetectorPage();

signals:
    void SignalStartTest();// 开始测试
    void SignalStartQRCode();// 开始获取二维码
    void SignalHaveQRCodeInfo(int iProgramCount);// 获得二维码，告知主界面，项目总数，进度条进度
    void SignalTestProgramIndex(int iProgramIndex);// 测试结果Index，用于主界面状态栏显示
    void SignalEndTest();// 流程测试结束，数据传输main
    void SignalStopTest();// 主动停止测试
public slots:
    void SlotStartQRCode();// 开始查找二维码
    void SlotReceiveQRCodeImage(QString strImagePath);// 接受二维码图片数据
    void SlotReceiveQRCodeInfo(QRCodeInfo sQRCodeInfoStruct);// 接受二维码信息
    void SlotReceiveTestResultData(TestResultData sTestResultDataStruct);// 接受每条测试结果数据
    void SlotReceiveSCupImagePath(QString strImagePath);// 方杯两个图片数据
    void SlotEndTest();// 流程测试结束
    void SlotReceiveTestError(ENUM_ERR eTestError);// 错误信号
    void SlotFuseImageOK();// 拼接完成
    void SlotDeleteImageOK();// 删除完成
private slots:
    // 控件
    void _SlotCheckReadTestDevice();// 开始测试
    void _SlotStopTest();// 主动停止测试
    // 打印PDF
    void _SlotPrintToPDF();
public:
    QList<TestResultData*> GetTestResultData();// 获得测试结果
    DetectorPageUserData GetUserData(); // 获得被测者信息
    QString GetTestPrintImagePath();// 获取需要打印的图片路径
    void SetCupType(QStringList strCupTypeList);// 设置杯类型
    void StopTest();// 主动停止测试，关闭UI
    void SetAutoTest(bool bAutoTest);
    void SetUserName(QString strUserName);
    void SetTestDelayTime(int iTime);
private:
    void _LoadQss();
    QGroupBox *_CreateDonorDetailsGroup();
    QGroupBox *_CreateProductDetailsGroup();
    QGroupBox *_CreateResultsGroup();
    void _InitWidget();
    void _InitLayout();
    // 数据输入判定
    bool _GetValidData();
    void _InitThreadTesting();// 初始化测试线程
    // 更新摄像头图片
    void _SetCamaraImage(QString strImagePath);

    // 打印
    void _ReplaceCubeHtmlData(QString &strHtml);// 替换html中数据位测试数据, cube杯型，方杯
    void _ReplaceCupHtmlData(QString &strHtml);// 替换html中数据位测试数据, cup杯型， 圆杯
    // 测试结果打印格式（动态数量）
    QString _GetResultsDataHtml();
    // 拼接图片
    void _FuseTCupImage();// 圆杯
    void _FuseTCubeImage();// 方杯


private:
    // donor details 1line
    QLabel *m_pDonorNameLabel;
    QCheckBox *m_pTemperatureNormalCBox;
    // last first donor id
    CLabelLineEditWidget *m_pLastNameWidget;
    CLabelLineEditWidget *m_pFirstNameWidget;
    CLabelLineEditWidget *m_pDonorIDWidget;
    // data of birth email
    CLabelDateWidget *m_pBirthDateWidget;
    CLabelLineEditWidget *m_pEmailAddressWidget;
    // test time
    CLabelDateTimeWidget *m_pTestTimeWidget;
    CLabelLineEditWidget *m_pTestingSiteWidget;
    // reason of test
    QLabel *m_pReasonfoTestLabel;
    QCheckBox *m_pPreEmploymentCBox;
    QCheckBox *m_pRandomCBox;
    QCheckBox *m_pReasonableSuspicionCauseCBox;
    QCheckBox *m_pPostAccidentCBox;
    QCheckBox *m_pReturnToDutyCBox;
    QCheckBox *m_pFollowUpCBox;
    QCheckBox *m_pOtherReasonForTestCBox;
    QLineEdit *m_pOtherReasonCommentsLineEdit;
    //////////////////////////////////////
    // Product Details
    QStringList m_strCupTypeList;// 杯类型数据
    CLabelCommoBoxWidget *m_pProductDefinitionWidget;
    CLabelLineEditWidget *m_pProductLotWidget;
    // expiration date
    CLabelLineEditWidget *m_pExpirationWidget;
    CLabelLineEditWidget *m_pProductIDWidget;
    //////////////////////////////////////
    // push button
    QPushButton *m_pReadTestDeviceButton;
    QPushButton *m_pStopTestButton;
    QPushButton *m_pPrintPriviewButton;
    //////////////////////////////////////
    // results
    QLabel *m_pCamaraLabel;// 摄像头
    QTableWidget *m_pResultsTableWidget;// 表格数据
    // 获取数据
    ThreadTesting *m_pThreadTesting; // 测试线程
    QRCodeInfo m_sQRCodeInfoStruct;       // 二维码数据，项目总数
    QList<TestResultData*> m_pTestResultDataList;// 下位机的测试结果列表
    DetectorPageUserData m_sDetectorPageUserDataStruct;// 用户列表数据
    QString m_strUserName;// 登陆用户名
    QStringList m_strSCupImagePathList;// 方杯的两个图片，单独发送拼接保存
    // 拼接图片
    CFuseImageThread *m_pFuseImageThread;
    QString m_strTestPrintImagePath;// 保存打印所需的测试过程图片，拼接合成，为当前时间，保存路径为result_image
    // 删除测试图片
    CDeleteImageThread *m_pDeleteImageThread;
    QString m_strTestImageDir;// 测试图片存放的目录
    // 初始化摄像头
    VideoThread *m_pVideoThread;
    // 自动测试
    bool m_bAutoTest;// 是否不停的测试
    int m_iTestDelayTime;// 测试延迟/孵化时间（秒）
    // 打印
    QString m_strHtml;
    CPrintPreviewWidget *m_pPrintPreviewWidget;
};

#endif // CDETECTORPAGE_H
